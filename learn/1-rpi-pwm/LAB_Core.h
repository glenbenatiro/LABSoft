#ifndef LAB_CORE_H
#define LAB_CORE_H


#include "Defaults.h"

class LAB_Core
{
  public:
    int m_spi_frequency = LAB_SPI_FREQUENCY;

    MemoryMap m_gpio_regs,
                   m_dma_regs, 
                   m_clk_regs, 
                   m_pwm_regs, 
                   m_regs_spi, 
                   m_regs_usec;

    MemoryMap m_vc_mem;
       
  public:
    int   m_fifo_fd = 0;

    uint32_t m_pwm_range,  
             m_fifo_size;


    LAB_Core ();
   ~LAB_Core ();

    // --- Memory ---
    void     LAB_Core_map_periph       (MemoryMap *mp, void *phys, int size);    
    void     LAB_Core_map_devices      ();
    void*    LAB_Core_map_segment      (void *addr, int size);
    void     LAB_Core_unmap_segment    ();
    void*    LAB_Core_map_uncached_mem (MemoryMap *mp, int size);
    void     LAB_Core_unmap_periph_mem (MemoryMap *mp);
    
    // --- Videocore Mailbox ---
    int  	   LAB_Core_mailbox_open        (void);
    void     LAB_Core_disp_vc_msg      (VC_MSG *msgp);
    void 	   LAB_Core_mailbox_close       (int fd);
    void     LAB_Core_unmap_segment    (void *mem, int  size);
    void*    LAB_Core_lock_vc_mem      (int fd, int h);
    uint32_t LAB_Core_msg_mbox         (int fd, VC_MSG *msgp);
    uint32_t LAB_Core_fset_vc_clock    (int fd, int id, uint32_t freq);
    uint32_t LAB_Core_free_vc_mem      (int fd, int h);
    uint32_t LAB_Core_unlock_vc_mem    (int fd, int h);
    uint32_t LAB_Core_alloc_vc_mem     (int fd, uint32_t size, MAILBOX_ALLOCATE_MEMORY_FLAGS flags);
  
    // --- Aux ---
    void     LAB_Core_fail             (const char *s);
    void     LAB_Core_terminate        (int sig);
        
    // --- DMA ---
    void     LAB_Core_dma_enable       (int chan);
    void     LAB_Core_dma_start        (MemoryMap *mp, int chan, DMA_CB *cbp, uint32_t csval);
    void     LAB_Core_dma_disp         (int chan);
    void     LAB_Core_dma_stop         (int chan);
    void     LAB_Core_dma_wait         (int chan);
    uint32_t LAB_Core_dma_transfer_len (int chan);
    
    // --- GPIO ---
    void     gpio_write         (unsigned pin, unsigned value);
    void     gpio_mode        (int pin, int mode);
    void     LAB_Core_gpio_pull        (int pin, int pull);
    void     gpio_set         (int pin, int mode, int pull);
    uint8_t  gpio_read          (int pin);
    
    // --- SPI ---
    int       spi_init            ();
    void      spi_clear_rxtx_fifo ();
    void      spi_disp            ();
    void      spi_disable         ();
    void      spi_xfer            (uint8_t *txd, uint8_t *rxd, int length);
    void      spi_cs              (int set);
    int       spi_set_clock_rate  (int value);

    // --- PWM ---
    void     pwm_init         (int freq, int range, int val);
    void     pwm_start        ();
    void     pwm_stop         ();
    void      pwm_set_frequency (float frequency);

    // --- FIFO ---
    int      LAB_Core_fifo_create      (const char *fifo_name);
    int      LAB_Core_fifo_open_write  (const char *fifo_name);
    int      LAB_Core_fifo_write       (int fd, void *data, int dlen);
    uint32_t LAB_Core_fifo_get_free_space   (int fd);
    void     LAB_Core_fifo_destroy     (char *fifo_name, int fd);
    int      LAB_Core_fifo_is_fifo     (const char *fname);
};

#endif