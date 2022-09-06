#include <iostream>

#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <errno.h>
#include <poll.h>

#include "LAB_Core.h"

// declare static data members
MEM_MAP LAB_Core::m_gpio_regs,
        LAB_Core::m_dma_regs, 
        LAB_Core::m_clk_regs, 
        LAB_Core::m_pwm_regs, 
        LAB_Core::m_spi_regs, 
        LAB_Core::m_usec_regs;

MEM_MAP LAB_Core::m_vc_mem;

LAB_Core::LAB_Core ()
{
  m_in_chans     = IN_CHANS;
  m_sample_rate  = OSCILLOSCOPE_STREAMING_SAMPLE_RATE;
  m_sample_count = OSCILLOSCOPE_STREAMING_SAMPLE_COUNT;

  m_spi_frequency = SPI_FREQ;

  LAB_Core_init ();
}

LAB_Core::~LAB_Core ()
{
	
}

void
LAB_Core::LAB_Core_init ()
{
  LAB_Core_map_devices ();
  LAB_Core_map_uncached_mem (&m_vc_mem, VC_MEM_SIZE);

  LAB_Core_spi_init (m_spi_frequency);
}

// Allocate uncached memory, get bus & phys addresses
void*
LAB_Core::LAB_Core_map_uncached_mem (MEM_MAP *mp,
                                       int      size)
{
  void *ret;
  
  mp->size = PAGE_ROUNDUP(size);
  mp->fd   = LAB_Core_open_mbox ();
  
  ret = (mp->h    = LAB_Core_alloc_vc_mem (mp->fd, mp->size, DMA_MEM_FLAGS))   > 0 &&
        (mp->bus  = LAB_Core_lock_vc_mem  (mp->fd, mp->h))                    != 0 &&
        (mp->virt = LAB_Core_map_segment  (BUS_PHYS_ADDR(mp->bus), mp->size)) != 0 
        ? mp->virt : 0;
        
  printf("VC mem handle %u, phys %p, virt %p\n", mp->h, mp->bus, mp->virt);
  
  return(ret);
}

void
LAB_Core::LAB_Core_map_devices ()
{
  LAB_Core_map_periph (&m_gpio_regs, (void *) GPIO_BASE, PAGE_SIZE);
  LAB_Core_map_periph (&m_dma_regs,  (void *) DMA_BASE,  PAGE_SIZE);
  LAB_Core_map_periph (&m_spi_regs,  (void *) SPI0_BASE, PAGE_SIZE);
  LAB_Core_map_periph (&m_clk_regs,  (void *) CLK_BASE,  PAGE_SIZE);
  LAB_Core_map_periph (&m_pwm_regs,  (void *) PWM_BASE,  PAGE_SIZE);
  LAB_Core_map_periph (&m_usec_regs, (void *) USEC_BASE, PAGE_SIZE);
}

// --- Memory ---
// use mmap to obtain virtual address, given physical
void
LAB_Core::LAB_Core_map_periph (MEM_MAP *mp, void *phys, int size)
{
  mp->phys = phys;
  mp->size = PAGE_ROUNDUP(size);
  mp->bus  = (uint8_t *)phys - (uint8_t *)PHYS_REG_BASE + (uint8_t *)BUS_REG_BASE;
  mp->virt = LAB_Core_map_segment (phys, mp->size);
}

// Free mapped peripheral or memory
void 
LAB_Core::LAB_Core_unmap_periph_mem (MEM_MAP *mp)
{
  if (mp)
    {
      if (mp->fd)
        {
          LAB_Core_unmap_segment (mp->virt, mp->size);
          LAB_Core_unlock_vc_mem (mp->fd, mp->h);
          LAB_Core_free_vc_mem   (mp->fd, mp->h);
          LAB_Core_close_mbox    (mp->fd);
        }
      else
        {
          LAB_Core_unmap_segment (mp->virt, mp->size);
        }
    }
}

// --- Virtual Memory ---
// Get virtual memory segment for peripheral regs or physical mem
void*
LAB_Core::LAB_Core_map_segment (void *addr, 
                                  int   size)
{
  int fd;
  void *mem;

  size = PAGE_ROUNDUP(size);
  
  if ((fd = open ("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
    LAB_Core_fail ("Error: can't open /dev/mem, run using sudo\n");
        
  mem = mmap (0, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, (uint32_t)addr);
  
  close(fd);
  
  #if DEBUG
    printf("Map %p -> %p\n", (void *)addr, mem);
  #endif
  
  if (mem == MAP_FAILED)
    LAB_Core_fail("Error: can't map memory\n");
    
  return(mem);
}

// Free mapped memory
void 
LAB_Core::LAB_Core_unmap_segment (void *mem, 
                                    int  size)
{
  if (mem)
    munmap (mem, PAGE_ROUNDUP(size));
}


// --- DMA ---

// Enable and reset DMA
void 
LAB_Core::LAB_Core_dma_enable (int chan)
{
  *REG32(m_dma_regs, DMA_ENABLE) |= (1 << chan);
  *REG32(m_dma_regs, DMA_REG(chan, DMA_CS)) = 1 << 31;
}

// Start DMA, given first control block
void 
LAB_Core::LAB_Core_dma_start (MEM_MAP *mp, 
                                int      chan, 
                                DMA_CB  *cbp, 
                                uint32_t csval)
{
  *REG32(m_dma_regs, DMA_REG(chan, DMA_CONBLK_AD)) = MEM_BUS_ADDR(mp, cbp);
  *REG32(m_dma_regs, DMA_REG(chan, DMA_CS))        = 2;         // Clear 'end' flag
  *REG32(m_dma_regs, DMA_REG(chan, DMA_DEBUG))     = 7;         // Clear error bits
  *REG32(m_dma_regs, DMA_REG(chan, DMA_CS))        = 1 | csval; // Start DMA
}

// Return remaining transfer length
uint32_t 
LAB_Core::LAB_Core_dma_transfer_len (int chan)
{
 return (*REG32(m_dma_regs, DMA_REG(chan, DMA_TXFR_LEN)));
}

// Halt current DMA operation by resetting controller
void 
LAB_Core::LAB_Core_dma_stop (int chan)
{
  if (m_dma_regs.virt)
    *REG32(m_dma_regs, DMA_REG(chan, DMA_CS)) = 1 << 31;
}

void 
LAB_Core::LAB_Core_dma_wait(int chan)
{
  int n = 10000;

  do 
    usleep(10);
  while (LAB_Core_dma_transfer_len (chan) && --n);
    
  if (n == 0)
    printf ("DMA transfer timeout\n");
}

// Display DMA registers
void 
LAB_Core::LAB_Core_dma_disp (int chan)
{
  const char *m_dma_regstrs[] = {"DMA CS", 
                                 "CB_AD", 
                                 "TI", 
                                 "SRCE_AD", 
                                 "DEST_AD",
                                 "TFR_LEN", 
                                 "STRIDE", 
                                 "NEXT_CB", 
                                 "DEBUG", 
                                 ""};
  int i = 0;
  volatile uint32_t *p = REG32(m_dma_regs, DMA_REG(chan, DMA_CS));
    

  while (m_dma_regstrs[i][0])
    {
      printf("%-7s %08X ", m_dma_regstrs[i++], *p++);
      
      if (i % 5 == 0 || m_dma_regstrs[i][0] == 0)
        printf("\n");
    }
}


// --- Videocore Mailbox ---
// Open mailbox interface, return file descriptor
int 
LAB_Core::LAB_Core_open_mbox (void)
{
  int fd;

  if ((fd = open ("/dev/vcio", 0)) < 0)
    LAB_Core_fail("Error: can't open VC mailbox\n");
     
  return(fd);
}

// Close mailbox interface
void 
LAB_Core::LAB_Core_close_mbox (int fd)
{
  if (fd >= 0)
    close (fd);
}

// Send message to mailbox, return first response int, 0 if error
uint32_t 
LAB_Core::LAB_Core_msg_mbox (int     fd, 
                               VC_MSG *msgp)
{
  uint32_t ret = 0, i;

  for (i = msgp->dlen/4; i <= msgp->blen/4; i += 4)
    msgp->uints[i++] = 0;
    
  msgp->len = (msgp->blen + 6) * 4;
  msgp->req = 0;
  
  if (ioctl (fd, _IOWR(100, 0, void *), msgp) < 0)
    printf("VC IOCTL failed\n");
  else if ((msgp->req & 0x80000000) == 0)
    printf("VC IOCTL error\n");
  else if (msgp->req == 0x80000001)
    printf("VC IOCTL partial error\n");
  else
    ret = msgp->uints[0];
      
  #if DEBUG
    disp_vc_msg (msgp);
  #endif
  
  return (ret);
}

// Allocate memory on PAGE_SIZE boundary, return handle
uint32_t 
LAB_Core::LAB_Core_alloc_vc_mem (int            fd, 
                                   uint32_t       size,
                                   VC_ALLOC_FLAGS flags)
{
  VC_MSG msg = {.tag   = 0x3000c, 
                .blen  = 12,
                .dlen  = 12,
                .uints = {PAGE_ROUNDUP(size), 
                          PAGE_SIZE, flags}};
                          
  return (LAB_Core_msg_mbox(fd, &msg));
}

// Lock allocated memory, return bus address
void*
LAB_Core::LAB_Core_lock_vc_mem (int fd, 
                                  int h)
{
  VC_MSG msg = {.tag   = 0x3000d, 
                .blen  = 4, 
                .dlen  = 4, 
                .uints = {static_cast<uint32_t>(h)}};
  
  return (h ? (void *)LAB_Core_msg_mbox (fd, &msg) : 0);
}

// Unlock allocated memory
uint32_t 
LAB_Core::LAB_Core_unlock_vc_mem (int fd, 
                                    int h)
{
  VC_MSG msg = {.tag   = 0x3000e, 
                .blen  = 4, 
                .dlen  = 4, 
                .uints = {static_cast<uint32_t>(h)}};
  
  return(h ? LAB_Core_msg_mbox(fd, &msg) : 0);
}

// Free memory
uint32_t 
LAB_Core::LAB_Core_free_vc_mem (int fd, 
                                  int h)
{
  VC_MSG msg={.tag     = 0x3000f, 
              .blen    = 4,
              .dlen    = 4, 
              .uints   = {static_cast<uint32_t>(h)}};
  
  return(h ? LAB_Core_msg_mbox(fd, &msg) : 0);
}

uint32_t 
LAB_Core::LAB_Core_fset_vc_clock (int      fd, 
                                    int      id, 
                                    uint32_t freq)
{
  VC_MSG msg1 = {.tag   = 0x38001, 
                 .blen  = 8, 
                 .dlen  = 8, 
                 .uints = {static_cast<uint32_t>(id), 1}};
                 
  VC_MSG msg2 = {.tag   = 0x38002, 
                 .blen  = 12,
                 .dlen  = 12,
                 .uints = {static_cast<uint32_t>(id), freq, 0}};
  
  LAB_Core_msg_mbox    (fd, &msg1);
  LAB_Core_disp_vc_msg (&msg1);
  
  LAB_Core_msg_mbox    (fd, &msg2);
  LAB_Core_disp_vc_msg (&msg2);
  
  return(0);
}

// Display mailbox message
void 
LAB_Core::LAB_Core_disp_vc_msg(VC_MSG *msgp)
{
    int i;

    printf("VC msg len=%X, req=%X, tag=%X, blen=%x, dlen=%x, data ",
        msgp->len, msgp->req, msgp->tag, msgp->blen, msgp->dlen);
    for (i=0; i<msgp->blen/4; i++)
        printf("%08X ", msgp->uints[i]);
    printf("\n");
}



// -- Aux ---
// Catastrophic failure in initial setup
void 
LAB_Core::LAB_Core_fail (const char *s)
{
  printf (s);
  LAB_Core_terminate (0);
}

// Free memory & peripheral mapping and exit
void 
LAB_Core::LAB_Core_terminate (int sig)
{
  printf("Closing\n");
  
  LAB_Core_spi_disable();
  //dma_stop(DMA_CHAN_A);
  //dma_stop(DMA_CHAN_B);
  //dma_stop(DMA_CHAN_C);
  
  LAB_Core_unmap_periph_mem (&m_vc_mem);
  LAB_Core_unmap_periph_mem (&m_usec_regs);
  LAB_Core_unmap_periph_mem (&m_pwm_regs);
  LAB_Core_unmap_periph_mem (&m_clk_regs);
  LAB_Core_unmap_periph_mem (&m_spi_regs);
  LAB_Core_unmap_periph_mem (&m_dma_regs);
  LAB_Core_unmap_periph_mem (&m_gpio_regs);
  
    //if (fifo_name)
        //destroy_fifo(fifo_name, fifo_fd);
    //if (samp_total)
        //printf("Total samples %u, overruns %u\n", samp_total, overrun_total);
    //exit(0);
}


// // --- SPI ---
// Initialise SPI0, given desired clock freq; return actual value
int 
LAB_Core::LAB_Core_spi_init (int hz)
{
  int f, div = (SPI_CLOCK_HZ / hz + 1) & ~1;

  LAB_Core_gpio_set (SPI0_CE0_PIN,  GPIO_ALT0, GPIO_NOPULL);
  LAB_Core_gpio_set (SPI0_CE1_PIN,  GPIO_ALT0, GPIO_NOPULL);
  LAB_Core_gpio_set (SPI0_MISO_PIN, GPIO_ALT0, GPIO_PULLUP);
  LAB_Core_gpio_set (SPI0_MOSI_PIN, GPIO_ALT0, GPIO_NOPULL);
  LAB_Core_gpio_set (SPI0_SCLK_PIN, GPIO_ALT0, GPIO_NOPULL);
 
  while (div == 0 || (f = SPI_CLOCK_HZ / div) > MAX_SPI_FREQ)
    div += 2;
      
  *REG32(m_spi_regs, SPI_CS)  = 0x30;
  *REG32(m_spi_regs, SPI_CLK) = div;  
  return (f);
}

// Clear SPI FIFOs
void 
LAB_Core::LAB_Core_spi_clear (void)
{
  *REG32(m_spi_regs, SPI_CS) = SPI_FIFO_CLR;
}

// Set / clear SPI chip select
void 
LAB_Core::LAB_Core_spi_cs (int set)
{
  uint32_t csval = *REG32(m_spi_regs, SPI_CS);

  *REG32(m_spi_regs, SPI_CS) = set ? csval | 0x80 : csval & ~0x80;
}

// Transfer SPI bytes
void 
LAB_Core::LAB_Core_spi_xfer (uint8_t *txd, 
                               uint8_t *rxd, 
                               int      len)
{
  while (len--)
    {
      *REG8(m_spi_regs, SPI_FIFO) = *txd++;
      
      while ((*REG32(m_spi_regs, SPI_CS) & (1<<17)) == 0);
      
      *rxd++ = *REG32(m_spi_regs, SPI_FIFO);
    }
}

// Disable SPI
void 
LAB_Core::LAB_Core_spi_disable (void)
{
    *REG32(m_spi_regs, SPI_CS) = SPI_FIFO_CLR;
    *REG32(m_spi_regs, SPI_CS) = 0;
}

// Display SPI registers
void 
LAB_Core::LAB_Core_spi_disp (void)
{
  // SPI register strings
  const char *m_spi_regstrs[] = {"CS", 
                               "FIFO", 
                               "CLK", 
                               "DLEN", 
                               "LTOH", 
                               "DC", 
                               ""};

  int i = 0;
  volatile uint32_t *p = REG32(m_spi_regs, SPI_CS);

  while (m_spi_regstrs[i][0])
    printf("%-4s %08X ", m_spi_regstrs[i++], *p++);
  
  printf("\n");
}



// --- GPIO ---
// Set input or output with pullups
void 
LAB_Core::LAB_Core_gpio_set (int pin, 
                             int mode, 
                             int pull)
{
  LAB_Core_gpio_mode (pin, mode);
  LAB_Core_gpio_pull (pin, pull);
}

// Set I/P pullup or pulldown
void
LAB_Core::LAB_Core_gpio_pull (int pin,
                                int pull)
{
  volatile uint32_t *reg = REG32(m_gpio_regs, GPIO_GPPUDCLK0) + pin / 32;
  *REG32(m_gpio_regs, GPIO_GPPUD) = pull;
  usleep(2);
  
  *reg = pin << (pin % 32);
  usleep(2);
    
  *REG32(m_gpio_regs, GPIO_GPPUD) = 0;

  *reg = 0;
}

// Set input or output
void 
LAB_Core::LAB_Core_gpio_mode (int pin, 
                                int mode)
{
  volatile uint32_t *reg   = REG32(m_gpio_regs, GPIO_MODE0) + pin / 10, 
                     shift = (pin % 10) * 3;

  *reg = (*reg & ~(7 << shift)) | (mode << shift);
}

// Set an O/P pin
void 
LAB_Core::LAB_Core_gpio_out(int pin, int val)
{
    volatile uint32_t *reg = REG32(m_gpio_regs, val ? GPIO_SET0 : GPIO_CLR0) + pin/32;
    *reg = 1 << (pin % 32);
}

// Get an I/P pin value
uint8_t 
LAB_Core::LAB_Core_gpio_in (int pin)
{
  volatile uint32_t *reg = REG32(m_gpio_regs, GPIO_LEV0) + pin/32;
  return (((*reg) >> (pin % 32)) & 1);
}



// --- PWM ---
// Initialise PWM
void 
LAB_Core::LAB_Core_pwm_init (int freq, 
                               int range, 
                               int val)
{
  LAB_Core_pwm_stop();

  // check channel 1 state
  if (*REG32(m_pwm_regs, PWM_STA) & 0x100)
    {
      printf("PWM bus error\n");
      *REG32(m_pwm_regs, PWM_STA) = 0x100;
    }

  #if USE_VC_CLOCK_SET
    set_vc_clock(mbox_fd, PWM_CLOCK_ID, freq);
  #else
    int divi = CLOCK_HZ / freq;

    *REG32(m_clk_regs, CLK_PWM_CTL) = CLK_PASSWD | (1 << 5);

    while (*REG32(m_clk_regs, CLK_PWM_CTL) & (1 << 7)) ;
    *REG32(m_clk_regs, CLK_PWM_DIV) = CLK_PASSWD | (divi << 12);
    *REG32(m_clk_regs, CLK_PWM_CTL) = CLK_PASSWD | 6 | (1 << 4);
    while ((*REG32(m_clk_regs, CLK_PWM_CTL) & (1 << 7)) == 0) ;
  #endif
    usleep(100);
    *REG32(m_pwm_regs, PWM_RNG1) = range;
    *REG32(m_pwm_regs, PWM_FIF1) = val;
    usleep(100);
  #if PWM_OUT
    gpio_mode(PWM_PIN, GPIO_ALT5);
  #endif
}

// Start PWM operation
void 
LAB_Core::LAB_Core_pwm_start ()
{
  *REG32(m_pwm_regs, PWM_CTL) = PWM_CTL_USEF1 | PWM_ENAB;
}

// Stop PWM operation
void 
LAB_Core::LAB_Core_pwm_stop ()
{
  if (m_pwm_regs.virt)

  {
    *REG32(m_pwm_regs, PWM_CTL) = 0;
    usleep(100);
  }
}

// --- FIFO ---
int      
LAB_Core::LAB_Core_fifo_create (const char *fifo_name)
{
  int ok = 0;

  umask(0);

  if (mkfifo (fifo_name, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0 && errno != EEXIST)
    printf("Can't open FIFO '%s'\n", fifo_name);
  else
    ok = 1;

  return (ok);
}

int      
LAB_Core::LAB_Core_fifo_open_write (const char *fifo_name)
{
  int f = open (fifo_name, O_WRONLY | O_NONBLOCK);

  return(f == -1 ? 0 : f);
}

int      
LAB_Core::LAB_Core_fifo_write (int fd, void *data, int dlen)
{
  struct pollfd pollfd = {fd, POLLOUT, 0};

  poll(&pollfd, 1, 0);

  if (pollfd.revents&POLLOUT && !(pollfd.revents&POLLERR))
    return(fd ? write (fd, data, dlen) : 0);

  return (0);
}

uint32_t 
LAB_Core::LAB_Core_fifo_get_free_space (int fd)
{
  return (fcntl(fd, F_GETPIPE_SZ));
}

void     
LAB_Core::LAB_Core_fifo_destroy (char *fifo_name, 
                                   int   fd)
{
  if (fd > 0)
    close(fd);

  unlink (fifo_name);
}

// Check if fifo exists
int LAB_Core:: 
LAB_Core_fifo_is_fifo(const char *fname)
{
  struct stat stat_p;
  stat(fname, &stat_p);
  
  return(S_ISFIFO(stat_p.st_mode));
}


// EOF