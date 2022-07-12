#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <poll.h>

#include "LabInABox.h"

LabInABox::LabInABox () : in_chans      {IN_CHANS},
                          sample_count  {SAMPLE_COUNT},
                             {SAMPLE_RATE},
                          m_fd_adc_fifo   {0},
                          adc_fifo_name {FIFO_NAME},
                          lockstep      {LOCKSTEP},
                          verbose       {VERBOSE}
{ 
  std::cout << "DEBUG: LabInABox constructor start.\n";
  
  map_devices ();                           // Map peripheral registers  
  std::cout << "Devices mapped.\n";
  
  map_uncached_mem (&vc_mem, VC_MEM_SIZE);  // Map uncached VideoCore memory
  std::cout << "Uncached VideoCore memory mapped.\n";
  
  pwm_range = (PWM_FREQ * 2) / ; // Set PWM range
  std::cout << "PWM range set.\n";
  
  spi_init (SPI_FREQ);                      // Initialize SPI peripheral
  std::cout << "SPI initialized.\n";
                     
  // fifo_aux_create (adc_fifo_name);              // Create FIFO for ADC storage
  
  adc_dma_init (&vc_mem, sample_count, 0);  // Initialize and ready all DMA controls
  std::cout << "DMA initialized.\n";
  
  adc_stream_start ();                      // Start the DMA 
  std::cout << "PWM started.\n";
  
  std::cout << "DEBUG: LabInABox constructor end.\n";
}  
  
LabInABox::~LabInABox ()
{
  std::cout << "LabInABox deconstructor running...\n";
  
  spi_disable();
  
  dma_stop (DMA_CHAN_A);
  dma_stop (DMA_CHAN_B);
  dma_stop (DMA_CHAN_C);
  
  unmap_periph_mem(&vc_mem);
  unmap_periph_mem(&usec_regs);
  unmap_periph_mem(&pwm_regs);
  unmap_periph_mem(&clk_regs);
  unmap_periph_mem(&spi_regs);
  unmap_periph_mem(&dma_regs);
  unmap_periph_mem(&gpio_regs);
  
  if (adc_fifo_name)
      fifo_aux_destroy (adc_fifo_name, m_fd_adc_fifo);
  if (samp_total)
      printf("Total samples %u, overruns %u\n", samp_total, overrun_total);
      
  exit(0);
}

void
LabInABox::fail (const char *s)
{
  printf (s);
  exit (0);
}



// --- STREAMING DATA ---
void 
LabInABox::run_adc_streaming ()
{
  int n;
  
  std::cout << "DEBUG: Start run_adc_streaming thread.\n";
  
  // adc_stream_start ();
  
  // thread loop
  while (true)
    {
      // check stream flag is enabled
      if (m_flag_run_adc_streaming)
        {   
          // check if fifo is present and opened 
          if (m_fd_adc_fifo)
            {
              std::cout << ".";
              
              // fetch samples from ADC buffer, store it in stream_buff
              if ((n = adc_stream_csv (&vc_mem, stream_buff, STREAM_BUFFLEN, sample_count)) > 0)
                {    
                  
                  
                  //// write stream_buff to FIFO. close if write fail
                  //if (!fifo_write (m_fd_adc_fifo, stream_buff, n))
                    //{
                      //printf ("Stopped streaming\n");
                      //close (m_fd_adc_fifo);
                      //m_fd_adc_fifo = 0;
                      //usleep (100000);
                    //}
                  //else
                    //{
                      //usleep(10);
                    //}
                }
            }
          else 
            { 
              fifo_open_write ();
              // if m_fd_adc_fifo is equal to or less than zero; no fifo present
            }
          }
      else
        {
          break;
        }
    }
  
  std::cout << "DEBUG: End run_adc_streaming thread.\n";
}

// --- ADC ---
// Initialise PWM-paced DMA for ADC sampling
void 
LabInABox::adc_dma_init (MEM_MAP *mp, 
                         int      nsamp, 
                         int      single)
{ 
  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(mp->virt);
  // ADC_DMA_DATA *dp = mp->virt;
  
  ADC_DMA_DATA dma_data = {
    .cbs = 
       {
         // Rx input: read data from usec clock and SPI, into 2 ping-pong buffers
         {SPI_RX_TI, REG (usec_regs, USEC_TIME), MEM (mp, &dp->usecs[0]),                        4,  0, CBS (1), 0}, // 0
         {SPI_RX_TI, REG (spi_regs, SPI_FIFO),   MEM (mp, dp->rxd1), static_cast<uint32_t>(nsamp*4), 0, CBS (2), 0}, // 1
         {SPI_RX_TI, REG (spi_regs, SPI_CS),     MEM (mp, &dp->states[0]),                       4,  0, CBS (3), 0}, // 2
         {SPI_RX_TI, REG (usec_regs, USEC_TIME), MEM (mp, &dp->usecs[1]),                        4,  0, CBS (4), 0}, // 3
         {SPI_RX_TI, REG (spi_regs, SPI_FIFO),   MEM (mp, dp->rxd2), static_cast<uint32_t>(nsamp*4), 0, CBS (5), 0}, // 4
         {SPI_RX_TI, REG (spi_regs, SPI_CS),     MEM (mp, &dp->states[1]),                       4,  0, CBS (0), 0}, // 5
        
         // Tx output: 2 data writes to SPI for chan 0 & 1, or both chan 0
         {SPI_TX_TI, MEM (mp, dp->txd),          REG (spi_regs, SPI_FIFO), 8, 0, CBS (6), 0}, // 6
        
         // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
         {PWM_TI,    MEM (mp, &dp->pwm_val),     REG (pwm_regs, PWM_FIF1), 4, 0, CBS (8), 0}, // 7
         {PWM_TI,    MEM (mp, &dp->samp_size),   REG (spi_regs, SPI_DLEN), 4, 0, CBS (9), 0}, // 8
         {PWM_TI,    MEM (mp, &dp->adc_csd),     REG (spi_regs, SPI_CS),   4, 0, CBS (7), 0}, // 9
       },
     .samp_size = 2, 
     .pwm_val   = pwm_range, 
     .adc_csd   = (SPI_TFR_ACT | SPI_AUTO_CS | SPI_DMA_EN | SPI_FIFO_CLR | ADC_CE_NUM),
     .txd       = {0xd0, static_cast<uint32_t>(in_chans > 1 ? 0xf0 : 0xd0)},
     .usecs     = {0, 0}, 
     .states    = {0, 0}, 
     .rxd1      = {0}, 
     .rxd2      = {0}
    };

  if (single)                                 // If single-shot, stop after first Rx block
    dma_data.cbs[2].next_cb = 0;
    
  memcpy (dp, &dma_data, sizeof(dma_data));    // Copy DMA data into uncached memory
  pwm_init (PWM_FREQ, pwm_range, PWM_VALUE);   // Initialise PWM, with DMA
      
  *REG32 (pwm_regs, PWM_DMAC) = PWM_DMAC_ENAB | PWM_ENAB;
  *REG32 (spi_regs, SPI_DC)   = (8 << 24) | (1 << 16) | (8 << 8) | 1; // Set DMA priorities
  *REG32 (spi_regs, SPI_CS)   = SPI_FIFO_CLR;                         // Clear SPI FIFOs
    
  dma_start (mp, DMA_CHAN_C, &dp->cbs[6], 0); // Start SPI Tx DMA
  dma_start (mp, DMA_CHAN_B, &dp->cbs[0], 0); // Start SPI Rx DMA
  dma_start (mp, DMA_CHAN_A, &dp->cbs[7], 0); // Start PWM DMA, for SPI trigger
}

// Fetch samples from ADC buffer, return comma-delimited integer values
// If in lockstep mode, discard new data if FIFO isn't empty
int 
LabInABox::adc_stream_csv (MEM_MAP *mp, 
                           char    *vals, 
                           int      maxlen, 
                           int      nsamp)
{
  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(mp->virt);
  
  uint32_t i, n, usec, slen = 0;
  
    
  for (n = 0; n < 2 && slen == 0 ; n++)
    {
      // for some reason, program doesnt go to dp states n    
      if (dp->states[n])
        {
          std::cout << "b";
         
          samp_total += nsamp;
          memcpy(rx_buff, n ? (void *)dp->rxd2 : (void *)dp->rxd1, nsamp*4);
          usec = dp->usecs[n];
          
          if (dp->states[n^1])
            {
              dp->states[0] = dp->states[1] = 0;
              overrun_total++;
              break;
            }
            
          dp->states[n] = 0;
          
          if (usec_start == 0)
            usec_start = usec;
                
          if (!lockstep || fifo_freespace (m_fd_adc_fifo) >= fifo_size)
            {
              if (data_format == FMT_USEC)
                slen += sprintf(&vals[slen], "%u", usec-usec_start);
                  
              for (i=0; i<nsamp && slen+20<maxlen; i++)
                slen += sprintf(&vals[slen], "%s%4.3f", slen ? "," : "",
                  ADC_VOLTAGE(ADC_RAW_VAL(rx_buff[i])));
                  
              std::cout << ADC_VOLTAGE(ADC_RAW_VAL(rx_buff[i])) << "\n";
                  
              slen += sprintf(&vals[slen], "\n");
              
              if (verbose)
                {
                   for (int i=0; i<nsamp*4; i++)
                      printf("%02X ", *(((uint8_t *)rx_buff)+i));
                      
                  printf("\n");
                }
            }
        }
    }
    
  vals[slen] = 0;
  
  return (slen);
}

void
LabInABox::adc_stream_start ()
{
  pwm_start ();
}

void
LabInABox::adc_stream_stop ()
{
  pwm_stop ();
}



// --- FIFO ---
void
LabInABox::fifo_create ()
{
  fifo_aux_create (FIFO_NAME);
}

// Create a FIFO (named pipe)
int 
LabInABox::fifo_aux_create (const char *fifo_name)
{
  int ok = 0;

  umask(0);
  
  // using actual fifo_name variable
  // S_IRUSR = read permission bit for the owner of the file
  // S_IWUSR = write permission bit for the owner of the file
  // S_IRGRP = read permission bit for the group owner of the file
  // S_IROTH = read permission bit for other users
  if (mkfifo (fifo_name, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH) < 0 && errno != EEXIST)
    {
      printf("FAIL: Can't open FIFO '%s'\n", fifo_name);
    }
  else
    {
      printf("DEBUG: FIFO '%s' created.\n", fifo_name);
      ok = 1;
    }
    
  return (ok);
}

void 
LabInABox::fifo_destroy ()
{
  // fifo_aux_destroy (adc_fifo_name, m_fd_adc_fifo);
  
  std::cout << "i am here!\n";
  
  if (m_fd_adc_fifo > 0)
    {
      if (close (m_fd_adc_fifo) == 0)
        {
          std::cout << "DEBUG: success close m_fd_adc_fifo\n";
        }
      else
        {
          std::cout << "FAIL: fail close m_fd_adc_fifo\n";
        }
    }
  else
    {
      std::cout << "FAIL: m_fd_adc_fifo is less than or equal to zero\n";
    }
    
  if (unlink (FIFO_NAME) != 0)
    std::cout << "FAIL: error in unlinking fifo_name in LabInABox\n";
  else
    std::cout << "DEBUG: success unlinking fifo_name in LabInABox\n";
    
  m_fd_adc_fifo = 0;
}

void 
LabInABox::fifo_aux_destroy (const char *fifo_name,
                             int         fd)
{
  if (fd > 0)
    {
      close (fd);
      std::cout << "DEBUG: fifo fd closed in LabInABox\n";
    }
  else
    {
      std::cout << "FAIL: fifo is less than or equal to zero\n";
    }
    
    if (unlink (fifo_name) < 0)
    std::cout << "FAIL: error in unlinking fifo_name in LabInABox\n";
  else
    std::cout << "DEBUG: success unlinking fifo_name in LabInABox\n";
  
}


int 
LabInABox::fifo_open_write ()
{
  if (!m_fd_adc_fifo)
    {
      if (m_fd_adc_fifo = fifo_aux_open_write (adc_fifo_name) > 0) // open in nonblockingmode
        {
          printf ("DEBUG: LabInABox opened FIFO '%s' for wriring\n", adc_fifo_name);
          fifo_size = fifo_freespace (m_fd_adc_fifo);
        }
      else
        {
          printf ("FAIL: Failed opening FIFO '%s'.\n", adc_fifo_name, m_fd_adc_fifo);
        }
    }
  
  return (m_fd_adc_fifo);
}

int 
LabInABox::fifo_aux_open_write (const char *fifo_name)
{ 
  
  int f = open (fifo_name, O_WRONLY | O_NONBLOCK);
  
  std::cout << "DEBUG: fifo_fd value in fifo_aux_open_write: " << f <<  "\n";
  
  return (f == -1 ? 0 : f);
}

uint32_t
LabInABox::fifo_freespace (int fd)
{
  return (fcntl (fd, F_GETPIPE_SZ));
}

int
LabInABox::fifo_write (int   fd, 
                       void *data, 
                       int   dlen)
{
  struct pollfd pollfd = {fd, POLLOUT, 0};

  poll(&pollfd, 1, 0);
  
  if (pollfd.revents&POLLOUT && !(pollfd.revents&POLLERR))
    return (fd ? write (fd, data, dlen) : 0);
        
  return (0);
}

int
LabInABox::fifo_close ()
{
  return (close (m_fd_adc_fifo));
}

const char*
LabInABox::fifo_get_name ()
{
  return adc_fifo_name;
}



// --- MEMORY AND PERIPHERAL
void
LabInABox::map_devices ()
{
  map_periph (&gpio_regs, (void *)GPIO_BASE, PAGE_SIZE);
  map_periph (&dma_regs,  (void *)DMA_BASE,  PAGE_SIZE);
  map_periph (&spi_regs,  (void *)SPI0_BASE, PAGE_SIZE);
  map_periph (&clk_regs,  (void *)CLK_BASE,  PAGE_SIZE);
  map_periph (&pwm_regs,  (void *)PWM_BASE,  PAGE_SIZE);
  map_periph (&usec_regs, (void *)USEC_BASE, PAGE_SIZE);
}
  
// Use mmap to obtain virtual address, given physical
void*
LabInABox::map_periph (MEM_MAP *mp, 
                       void    *phys, 
                       int      size)
{
  mp->phys = phys;
  mp->size = PAGE_ROUNDUP (size);
  mp->bus  = (uint8_t *) phys - (uint8_t *) PHYS_REG_BASE + (uint8_t *) BUS_REG_BASE;
  mp->virt = map_segment (phys, mp->size);
   
  return (mp->virt);
}
  
// Allocate uncached mem, get bus & phys addresses
void*
LabInABox::map_uncached_mem (MEM_MAP *mp, 
                             int      size)
{
  void *ret;
    
  mp->size = PAGE_ROUNDUP (size);
  mp->fd = open_mbox ();
    
  ret = (mp->h    = alloc_vc_mem (mp->fd, mp->size, DMA_MEM_FLAGS)) > 0 &&
        (mp->bus  = lock_vc_mem (mp->fd, mp->h)) != 0 &&
        (mp->virt = map_segment (BUS_PHYS_ADDR (mp->bus), mp->size)) != 0 ? mp->virt : 0;
    
  printf("VC mem handle %u, phys %p, virt %p\n", mp->h, mp->bus, mp->virt);
  
  return (ret);
}
  
// Free mapped peripheral or memory
void 
LabInABox::unmap_periph_mem (MEM_MAP *mp)
{
  if (mp)
    {
      if (mp->fd)
        {
          unmap_segment(mp->virt, mp->size);
          unlock_vc_mem(mp->fd, mp->h);
          free_vc_mem(mp->fd, mp->h);
          close_mbox(mp->fd);
        }
      else
        {
          unmap_segment(mp->virt, mp->size);
        }
    }
}
   
// Get virtual memory segment for peripheral regs or physical mem
void*
LabInABox::map_segment (void *addr, 
                        int   size)
{
  int fd;
  void *mem;
    
  size = PAGE_ROUNDUP (size);
    
  if ((fd = open ("/dev/mem", O_RDWR|O_SYNC|O_CLOEXEC)) < 0)
    {
      std::cout << "fd: " << fd << "\n";
      perror("The error is ");
      fail ("Error: can't open /dev/mem, run using sudo\n");
    }
      
  mem = mmap (0, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, (uint32_t) addr);
  close (fd);
    
  if (mem == MAP_FAILED)
    fail ("Error: can't map memory\n");
    
  return (mem);  
}
  
// Free mapped memory
void 
LabInABox::unmap_segment (void *mem, 
                          int   size)
{
  if (mem)
    munmap(mem, PAGE_ROUNDUP(size));
}


  
// --- VIDEOCORE MAILBOX ---
// Open mailbox interface, return file descriptor
int
LabInABox::open_mbox ()
{
  int fd;
  
  if ((fd = open ("/dev/vcio", 0)) < 0)
    fail ("Error: can't open VC mailbox");
    
  return (fd);
}
  
// Close mailbox interface
void 
LabInABox::close_mbox (int fd)
{
  if (fd >= 0)
    close (fd);
}
  
// Send message to mailbox, return first response int, 0 if error
uint32_t 
LabInABox::msg_mbox (int     fd, 
                     VC_MSG *msgp)
{
  uint32_t ret = 0, i;
    
  for (i = msgp->dlen/4; i <= msgp->blen/4; i += 4)
    msgp->uints[i++] = 0;
    
  msgp->len = (msgp->blen + 6) * 4;
  msgp->req = 0;
    
  if (ioctl(fd, _IOWR(100, 0, void *), msgp) < 0)
    printf("VC IOCTL failed\n");
  else if ((msgp->req&0x80000000) == 0)
    printf("VC IOCTL error\n");
  else if (msgp->req == 0x80000001)
    printf("VC IOCTL partial error\n");
  else
    ret = msgp->uints[0];
        
  return (ret);
}
  
// Allocate memory on PAGE_SIZE boundary, return handle
uint32_t 
LabInABox::alloc_vc_mem (int      fd, 
                         uint32_t size, 
                         int      flags)
{
  VC_MSG msg = {.tag   = 0x3000c, 
                .blen  = 12, 
                .dlen  = 12,
                .uints = {PAGE_ROUNDUP (size), 
                          PAGE_SIZE,
                          static_cast<uint32_t>(flags)}
               };
          
  return (msg_mbox (fd, &msg));
}
  
// Lock allocated memory, return bus address
void*
LabInABox::lock_vc_mem (int fd,
                        int h)
{
  VC_MSG msg = {.tag   = 0x3000d, 
                .blen  = 4, 
                .dlen  = 4, 
                .uints = {static_cast<uint32_t>(h)}
               };
      
  return (h ? (void *)msg_mbox (fd, &msg) : 0);
}
  
// Unlock allocated memory
uint32_t 
LabInABox::unlock_vc_mem (int fd, 
                          int h)
{
  VC_MSG msg = {.tag   = 0x3000e, 
                .blen  = 4, 
                .dlen  = 4, 
                .uints = {static_cast<uint32_t>(h)}
               };
      
  return (h ? msg_mbox (fd, &msg) : 0);
}
  
// Free memory
uint32_t
LabInABox::free_vc_mem (int fd,
                        int h)
{
  VC_MSG msg = {.tag   = 0x3000f, 
                .blen  = 4, 
                .dlen  = 4, 
                .uints = {static_cast<uint32_t>(h)}
               };
      
  return (h ? msg_mbox (fd, &msg) : 0);
}
  
uint32_t 
LabInABox::set_vc_clock (int      fd, 
                         int      id, 
                         uint32_t freq)
{
  VC_MSG msg1 = {.tag   = 0x38001, 
                 .blen  = 8, 
                 .dlen  = 8, 
                 .uints = {static_cast<uint32_t>(id), 1}
                };
                
  VC_MSG msg2 = {.tag   = 0x38002, 
                 .blen  = 12, 
                 .dlen  = 12, 
                 .uints = {static_cast<uint32_t>(id), freq, 0}
                };
  
  msg_mbox (fd, &msg1);
  disp_vc_msg (&msg1);
  
  msg_mbox (fd, &msg2);
  disp_vc_msg (&msg2);
      
  return(0);
}

// Display mailbox message
void 
LabInABox::disp_vc_msg (VC_MSG *msgp)
{
  int i;

  printf("VC msg len=%X, req=%X, tag=%X, blen=%x, dlen=%x, data ",
    msgp->len, msgp->req, msgp->tag, msgp->blen, msgp->dlen);
    
  for (i=0; i<msgp->blen/4; i++)
    printf("%08X ", msgp->uints[i]);
    
  printf("\n");
}

 
  
// --- SPI ---
// Initialise SPI0, given desired clock freq; return actual value
int 
LabInABox::spi_init (int hz)
{
  int f, div = (SPI_CLOCK_HZ / hz + 1) & ~1;

  gpio_set (SPI0_CE0_PIN,  GPIO_ALT0, GPIO_NOPULL);
  gpio_set (SPI0_CE1_PIN,  GPIO_ALT0, GPIO_NOPULL);
  gpio_set (SPI0_MISO_PIN, GPIO_ALT0, GPIO_PULLUP);
  gpio_set (SPI0_MOSI_PIN, GPIO_ALT0, GPIO_NOPULL);
  gpio_set (SPI0_SCLK_PIN, GPIO_ALT0, GPIO_NOPULL);
  
  while (div == 0 || (f = SPI_CLOCK_HZ / div) > MAX_SPI_FREQ)
    div += 2;
    
  *REG32 (spi_regs, SPI_CS)  = 0x30;
  *REG32 (spi_regs, SPI_CLK) = div;
  
  return (f);
}

// Clear SPI FIFOs
void 
LabInABox::spi_clear ()
{
  *REG32(spi_regs, SPI_CS) = SPI_FIFO_CLR;
}

// Set / clear SPI chip select
void 
LabInABox::spi_cs (int set)
{
  uint32_t csval = *REG32(spi_regs, SPI_CS);

  *REG32(spi_regs, SPI_CS) = set ? csval | 0x80 : csval & ~0x80;
}

// Transfer SPI bytes
void 
LabInABox::spi_xfer (uint8_t *txd, 
                     uint8_t *rxd, 
                     int      len)
{
  while (len--)
    {
      *REG8(spi_regs, SPI_FIFO) = *txd++;
      while((*REG32(spi_regs, SPI_CS) & (1<<17)) == 0) ;
      *rxd++ = *REG32(spi_regs, SPI_FIFO);
    }
}

// Disable SPI
void 
LabInABox::spi_disable ()
{
  *REG32(spi_regs, SPI_CS) = SPI_FIFO_CLR;
  *REG32(spi_regs, SPI_CS) = 0;
}

// Display SPI registers
void 
LabInABox::spi_disp ()
{
  int i = 0;
  volatile uint32_t *p = REG32 (spi_regs, SPI_CS);
  
  while (spi_regstrs[i][0])
    printf("%-4s %08X ", spi_regstrs[i++], *p++);
  printf("\n");
}



// --- DMA ---
// Enable and reset DMA
void 
LabInABox::dma_enable (int chan)
{
  *REG32(dma_regs, DMA_ENABLE) |= (1 << chan);
  *REG32(dma_regs, DMA_REG(chan, DMA_CS)) = 1 << 31;
}

// Start DMA, given first control block
void
LabInABox::dma_start (MEM_MAP *mp, 
                      int      chan, 
                      DMA_CB  *cbp, 
                      uint32_t csval)
{
  *REG32 (dma_regs, DMA_REG(chan, DMA_CONBLK_AD)) = MEM_BUS_ADDR(mp, cbp);
  *REG32 (dma_regs, DMA_REG(chan, DMA_CS)) = 2;        // Clear 'end' flag
  *REG32 (dma_regs, DMA_REG(chan, DMA_DEBUG)) = 7;     // Clear error bits
  *REG32 (dma_regs, DMA_REG(chan, DMA_CS)) = 1|csval;  // Start DMA
}

// Halt current DMA operation by resetting controller
void 
LabInABox::dma_stop (int chan)
{
  if (dma_regs.virt)
    *REG32 (dma_regs, DMA_REG(chan, DMA_CS)) = 1 << 31;
}

// Display DMA registers
void 
LabInABox::dma_disp (int chan)
{
  int i = 0;
  volatile uint32_t *p = REG32 (dma_regs, DMA_REG (chan, DMA_CS));
    
  while (dma_regstrs[i][0])
    {
        printf("%-7s %08X ", dma_regstrs[i++], *p++);
        
        if (i%5==0 || dma_regstrs[i][0]==0)
            printf("\n");
    }
}

// Return remaining transfer length
uint32_t 
LabInABox::dma_transfer_len (int chan)
{
  return (*REG32 (dma_regs, DMA_REG (chan, DMA_TXFR_LEN)));
}



// ----- PWM -----
// Initialise PWM
void 
LabInABox::pwm_init (int freq, 
                     int range, 
                     int val)
{
  pwm_stop ();
  
  if (*REG32 (pwm_regs, PWM_STA) & 0x100)
    {
      printf("PWM bus error\n");
      *REG32(pwm_regs, PWM_STA) = 0x100;
    }
    
  #if USE_VC_CLOCK_SET
    set_vc_clock (mbox_fd, PWM_CLOCK_ID, freq);
  #else
    int divi = CLOCK_HZ / freq;
    
    *REG32 (clk_regs, CLK_PWM_CTL) = CLK_PASSWD | (1 << 5);
      
    while (*REG32(clk_regs, CLK_PWM_CTL) & (1 << 7));
    
    *REG32 (clk_regs, CLK_PWM_DIV) = CLK_PASSWD | (divi << 12);
    *REG32 (clk_regs, CLK_PWM_CTL) = CLK_PASSWD | 6 | (1 << 4);
    
    while ((*REG32 (clk_regs, CLK_PWM_CTL) & (1 << 7)) == 0);
  #endif
  
  usleep(100);
  *REG32 (pwm_regs, PWM_RNG1) = range;
  *REG32 (pwm_regs, PWM_FIF1) = val;
  usleep(100);
  
  #if PWM_OUT
      gpio_mode(PWM_PIN, GPIO_ALT5);
  #endif
}

// Start PWM operation
void 
LabInABox::pwm_start ()
{
  *REG32 (pwm_regs, PWM_CTL) = PWM_CTL_USEF1 | PWM_ENAB;
}

// Stop PWM operation
void 
LabInABox::pwm_stop ()
{
  if (pwm_regs.virt)
    {
      *REG32(pwm_regs, PWM_CTL) = 0;
      usleep(100);
    }
}
      
      
      
// --- GPIO ---
// Set input or output with pullups
void 
LabInABox::gpio_set (int pin, 
                     int mode, 
                     int pull)
{
  gpio_mode(pin, mode);
  gpio_pull(pin, pull);
}

// Set I/P pullup or pulldown
void 
LabInABox::gpio_pull (int pin, 
                      int pull)
{
  volatile uint32_t *reg = REG32(gpio_regs, GPIO_GPPUDCLK0) + pin / 32;

  *REG32(gpio_regs, GPIO_GPPUD) = pull;
  usleep(2);
  
  *reg = pin << (pin % 32);
  usleep(2);
  
  *REG32(gpio_regs, GPIO_GPPUD) = 0;
  *reg = 0;
}

// Set input or output
void 
LabInABox::gpio_mode (int pin, 
                      int mode)
{
  volatile uint32_t *reg = REG32(gpio_regs, GPIO_MODE0) + pin / 10, shift = (pin % 10) * 3;
  *reg = (*reg & ~(7 << shift)) | (mode << shift);
}

// Set an O/P pin
void 
LabInABox::gpio_out (int pin, 
                     int val)
{
  volatile uint32_t *reg = REG32(gpio_regs, val ? GPIO_SET0 : GPIO_CLR0) + pin/32;
  *reg = 1 << (pin % 32);
}

// Get an I/P pin value
uint8_t 
LabInABox::gpio_in (int pin)
{
  volatile uint32_t *reg = REG32(gpio_regs, GPIO_LEV0) + pin/32;
  return (((*reg) >> (pin % 32)) & 1);
}




// EOF
