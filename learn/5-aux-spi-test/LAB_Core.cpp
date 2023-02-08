#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <errno.h>
#include <poll.h>
#include <chrono>
#include <thread>
#include <bitset>

#include "LAB_Core.h"

#include "Defaults.h"

LAB_Core::LAB_Core ()
{
  LAB_Core_map_devices ();
  LAB_Core_map_uncached_mem (&m_vc_mem, VC_MEM_SIZE);
 
  spi_init ();  
}

LAB_Core::~LAB_Core ()
{
	
}

// Allocate uncached memory, get bus & phys addresses
void*
LAB_Core::LAB_Core_map_uncached_mem (MemoryMap *mp,
                                     int      size)
{
  void *ret;
  
  mp->size = PAGE_ROUNDUP(size);
  mp->fd   = LAB_Core_mailbox_open ();
  
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
  LAB_Core_map_periph (&m_aux_regs,  (void *) AUX_BASE,  PAGE_SIZE);
  LAB_Core_map_periph (&m_gpio_regs, (void *) GPIO_BASE, PAGE_SIZE);
  LAB_Core_map_periph (&m_regs_dma,  (void *) DMA_BASE,  PAGE_SIZE);
  LAB_Core_map_periph (&m_regs_spi,  (void *) SPI0_BASE, PAGE_SIZE);
  LAB_Core_map_periph (&m_clk_regs,  (void *) CLK_BASE,  PAGE_SIZE);
  LAB_Core_map_periph (&m_regs_pwm,  (void *) PWM_BASE,  PAGE_SIZE);
  LAB_Core_map_periph (&m_regs_usec, (void *) USEC_BASE, PAGE_SIZE);
}

// --- Memory ---
// use mmap to obtain virtual address, given physical
void
LAB_Core::LAB_Core_map_periph (MemoryMap *mp, void *phys, int size)
{
  mp->phys = phys;
  mp->size = PAGE_ROUNDUP(size);
  mp->bus  = (uint8_t *)phys - (uint8_t *)PHYS_REG_BASE + (uint8_t *)BUS_REG_BASE;
  mp->virt = LAB_Core_map_segment (phys, mp->size);
}

// Free mapped peripheral or memory
void 
LAB_Core::LAB_Core_unmap_periph_mem (MemoryMap *mp)
{
  if (mp)
    {
      if (mp->fd)
        {
          LAB_Core_unmap_segment (mp->virt, mp->size);
          LAB_Core_unlock_vc_mem (mp->fd, mp->h);
          LAB_Core_free_vc_mem   (mp->fd, mp->h);
          LAB_Core_mailbox_close    (mp->fd);
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
  *REG32(m_regs_dma, DMA_ENABLE) |= (1 << chan);
  *REG32(m_regs_dma, DMA_REG(chan, DMA_CS)) = 1 << 31;
}

// Start DMA, given first control block
void 
LAB_Core::LAB_Core_dma_start (MemoryMap *mp, 
                                int      chan, 
                                DMA_CB  *cbp, 
                                uint32_t csval)
{
  *REG32(m_regs_dma, DMA_REG(chan, DMA_CONBLK_AD)) = MEM_BUS_ADDR(mp, cbp);
  *REG32(m_regs_dma, DMA_REG(chan, DMA_CS))        = 2;         // Clear 'end' flag
  *REG32(m_regs_dma, DMA_REG(chan, DMA_DEBUG))     = 7;         // Clear error bits
  *REG32(m_regs_dma, DMA_REG(chan, DMA_CS))        = 1 | csval; // Start DMA
}

// Return remaining transfer length
uint32_t 
LAB_Core::LAB_Core_dma_transfer_len (int chan)
{
 return (*REG32(m_regs_dma, DMA_REG(chan, DMA_TXFR_LEN)));
}

// Halt current DMA operation by resetting controller
void 
LAB_Core::LAB_Core_dma_stop (int chan)
{
  if (m_regs_dma.virt)
    *REG32(m_regs_dma, DMA_REG(chan, DMA_CS)) = 1 << 31;
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
  const char *m_regs_dmatrs[] = {"DMA CS", 
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
  volatile uint32_t *p = REG32(m_regs_dma, DMA_REG(chan, DMA_CS));
    

  while (m_regs_dmatrs[i][0])
    {
      printf("%-7s %08X ", m_regs_dmatrs[i++], *p++);
      
      if (i % 5 == 0 || m_regs_dmatrs[i][0] == 0)
        printf("\n");
    }
}


// --- Videocore Mailbox ---
// Open mailbox interface, return file descriptor
int 
LAB_Core::LAB_Core_mailbox_open (void)
{
  int fd;

  if ((fd = open ("/dev/vcio", 0)) < 0)
    LAB_Core_fail("Error: can't open VC mailbox\n");
     
  return(fd);
}

// Close mailbox interface
void 
LAB_Core::LAB_Core_mailbox_close (int fd)
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
                                   MAILBOX_ALLOCATE_MEMORY_FLAGS flags)
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
  
  spi_disable();
  //dma_stop(LAB_OSCILLOSCOPE_DMA_CHAN_PWM_PACING);
  //dma_stop(DMA_CHAN_SPI_RX);
  //dma_stop(LAB_OSCILLOSCOPE_DMA_CHAN_SPI_TX);
  
  LAB_Core_unmap_periph_mem (&m_vc_mem);
  LAB_Core_unmap_periph_mem (&m_regs_usec);
  LAB_Core_unmap_periph_mem (&m_regs_pwm);
  LAB_Core_unmap_periph_mem (&m_clk_regs);
  LAB_Core_unmap_periph_mem (&m_regs_spi);
  LAB_Core_unmap_periph_mem (&m_regs_dma);
  LAB_Core_unmap_periph_mem (&m_gpio_regs);
  
    //if (fifo_name)
        //destroy_fifo(fifo_name, fifo_fd);
    //if (samp_total)
        //printf("Total samples %u, overruns %u\n", samp_total, overrun_total);
    //exit(0);
}


// // --- SPI ---
// Initialise SPI0, given desired clock freq; return actual value
int LAB_Core::
spi_init ()
{
  // initialize spi gpio pins on rpi
  gpio_set (SPI0_CE0_PIN,  GPIO_ALT0, GPIO_NOPULL);
  gpio_set (SPI0_CE1_PIN,  GPIO_ALT0, GPIO_NOPULL);
  gpio_set (SPI0_MISO_PIN, GPIO_ALT0, GPIO_PULLUP);
  gpio_set (SPI0_MOSI_PIN, GPIO_ALT0, GPIO_NOPULL);
  gpio_set (SPI0_SCLK_PIN, GPIO_ALT0, GPIO_NOPULL);

  // clear tx and rx fifo. one shot operation
  spi_clear_fifo ();
  
  return (spi_set_clock_rate (m_spi_frequency));
}

void LAB_Core:: 
spi_clear_fifo ()
{
  // *REG32 (m_regs_spi, SPI_CS) = SPI_CS_CLEAR;
  *REG32 (m_regs_spi, SPI_CS) = 0x30;
}

// Set / clear SPI chip select
void 
LAB_Core::spi_cs (int set)
{
  uint32_t csval = *REG32(m_regs_spi, SPI_CS);

  *REG32(m_regs_spi, SPI_CS) = set ? csval | 0x80 : csval & ~0x80;
}

// Transfer SPI bytes
void LAB_Core::
spi_xfer (uint8_t *txd, 
          uint8_t *rxd, 
          int      length)
{
  while (length--)
    {
      *REG8(m_regs_spi, SPI_FIFO) = *txd++;
      
      // wait for rx fifo to contain AT LEAST 1 byte
      while ((*REG32(m_regs_spi, SPI_CS) & (1<<17)) == 0);
      
      *rxd++ = *REG32(m_regs_spi, SPI_FIFO);
    }
}

// Disable SPI
void 
LAB_Core::spi_disable (void)
{
    *REG32(m_regs_spi, SPI_CS) = SPI_CS_CLEAR;
    *REG32(m_regs_spi, SPI_CS) = 0;
}

// Display SPI registers
void LAB_Core::
spi_disp ()
{
  // SPI register strings
  const char *m_regs_spitrs[] = {"CS", 
                               "FIFO", 
                               "CLK", 
                               "DLEN", 
                               "LTOH", 
                               "DC", 
                               ""};

  int i = 0;
  volatile uint32_t *p = REG32(m_regs_spi, SPI_CS);

  while (m_regs_spitrs[i][0])
    printf("%-4s %08X ", m_regs_spitrs[i++], *p++);
  
  printf("\n");
}

// page 156
int LAB_Core:: 
spi_set_clock_rate (int value)
{
  uint32_t divider = 1;

  if (value <= SPI_CLOCK_HZ)
    divider = SPI_CLOCK_HZ / value;

  if (divider > 65535 || divider < 0)
    divider = 65535;

  // DEBUG LANG! please delete after
  divider = 0xF1;

  // set spi frequency as rounded off clock
  *REG32 (m_regs_spi, SPI_CLK) = divider;

  return (SPI_CLOCK_HZ / divider);
}



// --- Utility Peripherals ---
void LAB_Core:: 
aux_spi1_master_enable ()
{
  g_reg_write (m_aux_regs, AUX_ENABLES, 1, 1, 1);
}
    
void LAB_Core:: 
aux_spi1_master_disable ()
{
  g_reg_write (g_reg32 (m_aux_regs, AUX_ENABLES), 0, 1, 1);
}


// --- Utility SPI ---
void LAB_Core:: 
aux_spi0_init ()
{
  gpio_set (SPI1_SCLK_PIN,  GPIO_ALT4, GPIO_NOPULL);
  gpio_set (SPI1_MOSI_PIN,  GPIO_ALT4, GPIO_NOPULL);
  gpio_set (SPI1_MISO_PIN,  GPIO_ALT4, GPIO_PULLUP);
  gpio_set (SPI1_CE2_PIN,   GPIO_ALT4, GPIO_NOPULL);

  aux_spi1_master_enable ();
  aux_spi0_frequency (100'000);
  aux_spi0_chip_selects (0,0,0);
  aux_spi0_mode (0);
  aux_spi0_clock_polarity (0);
  aux_spi0_shift_length (8);
  aux_spi0_shift_MS_first (1);
  aux_spi0_clear_fifos ();
  aux_spi0_enable ();

  //g_reg32_peek (m_aux_regs, AUX_SPI0_CNTL0_REG);
  //g_reg32_peek ("aux enables", m_aux_regs, AUX_ENABLES);
  //g_reg32_peek ("cntl 0", m_aux_regs, AUX_SPI0_CNTL0_REG);
}

void LAB_Core:: 
aux_spi0_enable ()
{
  g_reg_write (g_reg32 (m_aux_regs, AUX_SPI0_CNTL0_REG), 1, 1, 11);
}

void LAB_Core::
aux_spi0_disable ()
{
  g_reg_write (g_reg32 (m_aux_regs, AUX_SPI0_CNTL0_REG), 0, 1, 11);
}

void LAB_Core:: 
aux_spi0_frequency (double frequency)
{
  /// frequency field is only 12 bit in register! 
  uint16_t divider = (static_cast<uint16_t>(((static_cast<double>(CLOCK_HZ)) / 
    (2.0 * frequency)) - 1.0)) & 0x0FFF;

  // printf ("divider: %x\n", divider);

  // double actual = static_cast<double>(CLOCK_HZ) / (2 * (divider + 1));
  // printf ("actual frequency: %f\n", actual);

  // volatile uint32_t *reg = g_reg32 (m_aux_regs, AUX_SPI0_CNTL0_REG);
  // *reg = (*reg & ~(0xFFF << 20)) | (divider << 20);

  g_reg_write (g_reg32 (m_aux_regs, AUX_SPI0_CNTL0_REG), divider, 0xFFF, 20);
}

void LAB_Core::
aux_spi0_chip_selects (bool CE2, 
                      bool CE1, 
                      bool CE0)
{
  uint32_t chip_selects = (CE2 << 2) | (CE1 << 1) | CE0;

  printf ("chip selects: %d\n", chip_selects);

  g_reg_write (m_aux_regs, AUX_SPI0_CNTL0_REG, chip_selects, 
    3, 17);
  
  g_reg32_peek ("for chip selects", m_aux_regs, AUX_SPI0_CNTL0_REG);
}

void LAB_Core:: 
aux_spi0_clear_fifos ()
{
  g_reg_write (m_aux_regs, AUX_SPI0_CNTL0_REG, 1, 1, 9);

  // maybe a small delay is required? 
  std::this_thread::sleep_for (std::chrono::milliseconds (5));

  g_reg_write (m_aux_regs, AUX_SPI0_CNTL0_REG, 0, 1, 9);
}

void LAB_Core::
aux_spi0_mode (uint8_t mode)
{
  switch (mode)
  {
    case 0:
      aux_spi0_clock_polarity (0);
      aux_spi0_in_rising (1);
      aux_spi0_out_rising (0);
      break;
    case 1:
      aux_spi0_clock_polarity (0);
      aux_spi0_in_rising (0);
      aux_spi0_out_rising (1);
      break;
    case 2:
      aux_spi0_clock_polarity (1);
      aux_spi0_in_rising (1);
      aux_spi0_out_rising (0);
      break;
    case 3:
      aux_spi0_clock_polarity (1);
      aux_spi0_in_rising (0);
      aux_spi0_out_rising (1);
      break;
    default: // default is mode 0
      aux_spi0_clock_polarity (0);
      aux_spi0_in_rising (1);
      aux_spi0_out_rising (0);
      break;
  }
}

void LAB_Core:: 
aux_spi0_clock_polarity (bool polarity)
{
  g_reg_write (m_aux_regs, AUX_SPI0_CNTL0_REG, polarity, 1, 7);
}

void LAB_Core::
aux_spi0_in_rising (bool value)
{
  // if 1, data is clocked in on the rising edge of the SPI clock
  // if 0, data is clocked in on the falling edge of the SPI clock
  g_reg_write (m_aux_regs, AUX_SPI0_CNTL0_REG, value, 1, 10);
}

void LAB_Core::
aux_spi0_out_rising (bool value)
{
  // if 1, data is clocked out on the rising edge of the SPI clock
  // if 0, data is clocked out on the falling edge of the SPI clock
  g_reg_write (m_aux_regs, AUX_SPI0_CNTL0_REG, value, 1, 8);
}

// specifies the number of bits to shift
void LAB_Core::
aux_spi0_shift_length (uint8_t value)
{
  g_reg_write (m_aux_regs, AUX_SPI0_CNTL0_REG, value, 6, 0);
}

void LAB_Core::
aux_spi0_shift_MS_first (bool value)
{
  g_reg_write (m_aux_regs, AUX_SPI0_CNTL0_REG, value, 1, 6);
}

void LAB_Core::
aux_spi0_write (char         *buf, 
               unsigned int  length)
{
  char dump[length];

  aux_spi0_read (buf, dump, length);
}

void LAB_Core:: 
aux_spi0_read (char        *txbuf, 
              char         *rxbuf, 
              unsigned int  length)
{
  while (length--)
  {
    //printf ("hai\n");
    *(g_reg32 (m_aux_regs, AUX_SPI0_IO_REG)) = *txbuf++;
    g_reg32_peek (m_aux_regs, AUX_SPI0_IO_REG);
    //aux_spi0_disable ();
    //aux_spi0_enable ();
  

    // indicates the module is busy transferring data (?)
    // or should you use bit count? rx fifo level?
    // while ((*(g_reg32 (m_aux_regs, AUX_SPI0_STAT_REG)) & (1 << 6)) != 0);

    while (((*(g_reg32 (m_aux_regs, AUX_SPI0_STAT_REG))) >> 7) & 0x01);

    *rxbuf++ = *(g_reg32 (m_aux_regs, AUX_SPI0_IO_REG));
    g_reg32_peek (m_aux_regs, AUX_SPI0_IO_REG);
    //aux_spi0_disable ();
  }
}

// --- GPIO ---
// Set input or output with pullups
void 
LAB_Core::gpio_set (int pin, 
                             int mode, 
                             int pull)
{
  gpio_mode (pin, mode);
  LAB_Core_gpio_pull (pin, pull);
}

// Set I/P pullup or pulldown
// pull = 0 disable pull up or down
// pull = 1 for enable pull down
// pull = 2 for enable pull up
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

// select the alternative function of the GPIO pin
void LAB_Core::
gpio_mode (int pin, 
           int mode)
{
  volatile uint32_t *reg   = REG32(m_gpio_regs, GPIO_MODE0) + pin / 10, 
                     shift = (pin % 10) * 3;

  g_reg_write ((g_reg32 (m_gpio_regs, GPIO_MODE0)) + pin / 10, mode, 7, shift);
}

// Set an O/P pin
void 
LAB_Core::gpio_write(unsigned pin, unsigned value)
{
    volatile uint32_t *reg = REG32(m_gpio_regs, val ? GPSET0 : GPCLR0) + pin/32;
    *reg = 1 << (pin % 32);
}

// Get an I/P pin value
uint8_t 
LAB_Core::gpio_read (int pin)
{
  volatile uint32_t *reg = REG32(m_gpio_regs, GPIO_LEV0) + pin/32;
  return (((*reg) >> (pin % 32)) & 1);
}



// --- PWM ---
// Initialise PWM
void 
LAB_Core::pwm_init (int freq, 
                               int range, 
                               int val)
{
  pwm_stop();

  // check channel 1 state
  if (*REG32(m_regs_pwm, PWM_STA) & 0x100)
    {
      printf("PWM bus error\n");
      *REG32(m_regs_pwm, PWM_STA) = 0x100;
    }

  #if USE_VC_CLOCK_SET
    set_vc_clock(mbox_fd, PWM_CLOCK_ID, freq);
  #else
    // see the BCM2385 Audio Clocks datasheet PDF for reference.
    // this is how to change PWM clock speed
    int divi = CLOCK_HZ / freq;

    // CLK_PASSWD is "5a" as written on datasheet
    // https://www.scribd.com/doc/127599939/BCM2835-Audio-clocks#download

    // max PWM operating frequency is 25MHz as written on datasheet

    // 1 << 5 = KILL: kill the clock generator
    // this line stops the clock generator
    *REG32(m_clk_regs, CLK_PWM_CTL) = CLK_PASSWD | (1 << 5);

    // 1 << 7 = BUSY: Clock generator is running
    // this line waits for BUSY to 0, or for clock generator to stop
    while (*REG32(m_clk_regs, CLK_PWM_CTL) & (1 << 7)) ;

    // divi << 12 = DIVI: Integer part of divisor
    // assign divisor to DIVI field
    *REG32(m_clk_regs, CLK_PWM_DIV) = CLK_PASSWD | (divi << 12);

    // 1 << 4 = ENAB: Enable the clock generator
    // this line asserts ENAB to enable the clock generator
    *REG32(m_clk_regs, CLK_PWM_CTL) = CLK_PASSWD | 6 | (1 << 4);

    // // 1 << 7 = BUSY: Clock generator is running
    // this line waits until BUSY is 1, this means clock generator is running
    while ((*REG32(m_clk_regs, CLK_PWM_CTL) & (1 << 7)) == 0) ;
  #endif


  usleep(100);
  *REG32(m_regs_pwm, PWM_RNG1) = range;
  *REG32(m_regs_pwm, PWM_FIF1) = val;
  usleep(100);


  #if PWM_OUT
    gpio_mode(PWM_PIN, GPIO_ALT5);
  #endif

  //gpio_set(PWM_PIN, GPIO_ALT5, 1);
}

// Start PWM operation
void 
LAB_Core::pwm_start ()
{
  *REG32(m_regs_pwm, PWM_CTL) = PWM_CTL_USEF1 | PWM_ENAB;
  usleep(1000);
}

// Stop PWM operation
void 
LAB_Core::pwm_stop ()
{
  if (m_regs_pwm.virt)

  {
    *REG32(m_regs_pwm, PWM_CTL) = 0;
    usleep(1000);
  }
}

void
LAB_Core::pwm_set_frequency (float frequency)
{
  pwm_stop();

  uint32_t range = (m_pwm_frequency * 2) / (frequency * 4);

  // check channel 1 state
  if (*REG32(m_regs_pwm, PWM_STA) & 0x100)
    {
      printf("PWM bus error\n");
      *REG32(m_regs_pwm, PWM_STA) = 0x100;
    }

  // see the BCM2385 Audio Clocks datasheet PDF for reference.
  // this is how to change PWM clock speed
  int divi = CLOCK_HZ / m_pwm_frequency;

  // CLK_PASSWD is "5a" as written on datasheet
  // https://www.scribd.com/doc/127599939/BCM2835-Audio-clocks#download

  // max PWM operating frequency is 25MHz as written on datasheet

  // 1 << 5 = KILL: kill the clock generator
  // this line stops the clock generator
  *REG32(m_clk_regs, CLK_PWM_CTL) = CLK_PASSWD | (1 << 5);

  // 1 << 7 = BUSY: Clock generator is running
  // this line waits for BUSY to 0, or for clock generator to stop
  while (*REG32(m_clk_regs, CLK_PWM_CTL) & (1 << 7)) ;

  // divi << 12 = DIVI: Integer part of divisor
  // assign divisor to DIVI field
  *REG32(m_clk_regs, CLK_PWM_DIV) = CLK_PASSWD | (divi << 12);

  // 1 << 4 = ENAB: Enable the clock generator
  // this line asserts ENAB to enable the clock generator
  *REG32(m_clk_regs, CLK_PWM_CTL) = CLK_PASSWD | 6 | (1 << 4);

  // // 1 << 7 = BUSY: Clock generator is running
  // this line waits until BUSY is 1, this means clock generator is running
  while ((*REG32(m_clk_regs, CLK_PWM_CTL) & (1 << 7)) == 0) ;

  usleep(1000);
  *REG32(m_regs_pwm, PWM_RNG1) = range;
  *REG32(m_regs_pwm, PWM_FIF1) = m_pwm_value;
  usleep(1000);

  pwm_start ();
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