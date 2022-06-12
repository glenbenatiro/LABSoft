#ifndef LABINABOX_H
#define LABINABOX_H

#include <atomic>

#include "LABSoftGlobals.h"

class LabInABox
{
private:
    
  int         in_chans, 
              sample_count, 
              sample_rate,
              m_fd_adc_fifo {0},
              lockstep {0},
              data_format,
              verbose {0};
      
  const char *adc_fifo_name;

  MEM_MAP     gpio_regs, // GPIO registers
              dma_regs,  // DMA registers
              clk_regs,  // Clock registers
              pwm_regs,  // PWM registers
              vc_mem,    // Uncached VideoCore memory 
              spi_regs,  // SPI registers
              usec_regs; // Timer registers
  
  uint32_t    pwm_range,
              samp_total,
              overrun_total,
              fifo_size,
              rx_buff[MAX_SAMPS],
              usec_start;
  
  char        stream_buff[STREAM_BUFFLEN];
  
  typedef struct {
    DMA_CB cbs[NUM_CBS];
    uint32_t samp_size, pwm_val, adc_csd, txd[2];
    volatile uint32_t usecs[2], states[2], rxd1[MAX_SAMPS], rxd2[MAX_SAMPS];
  } ADC_DMA_DATA;
  
  
public:
  LabInABox ();
  ~LabInABox ();
  void fail (const char *s);

  // --- Status flags ---
  std::atomic<bool> m_flag_run_adc_streaming {false};

  // --- STREAMING DATA ---
  void run_adc_streaming (); 
  
  // --- ADC ---
  void adc_dma_init (MEM_MAP *mp, int nsamp, int single);
  void adc_stream_start ();
  int adc_stream_csv (MEM_MAP *mp, char *vals, int maxlen, int nsamp);
  void adc_stream_stop ();
  
  // --- FIFO ---
  void      fifo_create ();
  int     fifo_aux_create (const char *fifo_name);
  void     fifo_destroy ();
  void     fifo_aux_destroy (const char *fifo_name, int fd);
  int      fifo_open_write ();
  int      fifo_aux_open_write (const char *fifo_name);
  uint32_t fifo_freespace (int fd);
  int      fifo_write (int fd, void *data, int dlen);
  int     fifo_close ();
  const char* fifo_get_name ();
  
  // --- MEMORY AND PERIPHERAL  ---
  void map_devices ();
  void *map_periph (MEM_MAP *mp, void *phys, int size);
  void *map_uncached_mem (MEM_MAP *mp, int size);
  void unmap_periph_mem(MEM_MAP *mp);
  void *map_segment (void *addr, int size);
  void unmap_segment (void *mem, int size);

  // --- VIDEOCORE MAILBOX ---
  int open_mbox ();
  void close_mbox (int fd);
  uint32_t msg_mbox (int fd, VC_MSG *msgp);
  uint32_t alloc_vc_mem(int fd, uint32_t size, int flags);
  void *lock_vc_mem (int fd, int h);
  uint32_t unlock_vc_mem (int fd, int h);
  uint32_t free_vc_mem (int fd, int h);
  uint32_t set_vc_clock (int fd, int id, uint32_t freq);
  void disp_vc_msg (VC_MSG *msgp);
    
  // --- SPI ---
  int spi_init (int hz);
  void spi_clear ();
  void spi_cs (int set);
  void spi_xfer (uint8_t *txd, uint8_t *rxd, int len);
  void spi_disable ();
  void spi_disp ();
  
  // --- DMA ---
  void dma_enable (int chan);
  void dma_start (MEM_MAP *mp, int chan, DMA_CB *cbp, uint32_t csval);
  void dma_stop (int chan);
  void dma_disp (int chan);
  uint32_t dma_transfer_len (int chan);

  // --- PWM ---
  void pwm_init (int freq, int range, int val);
  void pwm_start ();
  void pwm_stop ();
          
  // --- GPIO ---
  void gpio_set (int pin, int mode, int pull);
  void gpio_pull (int pin, int pull);
  void gpio_mode (int pin, int mode);
  void gpio_out (int pin, int val);
  uint8_t gpio_in (int pin);
};

#endif

// EOF
