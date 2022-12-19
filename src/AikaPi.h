#ifndef AIKAPI_H
#define AIKAPI_H

#include <cstdint>

#include "Utility.h"
#include "Defaults.h"
#include "LAB_Core_Defs.h"

// --- GPIO Registers ---
constexpr int GPFSEL0 = 0x0;

// --- Enums ---

enum PIN_INFO_MODE
{
  PIN_INFO_MODE_NONE,
  PIN_INFO_MODE_SERIAL,
  PIN_INFO_MODE_I2C_SDA,
  PIN_INFO_MODE_I2C_SCL,
  PIN_INFO_MODE_SPI_SCLK,
  PIN_INFO_MODE_SPI_MISO,
  PIN_INFO_MODE_SPI_MOSI,
  PIN_INFO_MODE_SPI_CS
};

enum GPIO_MODES
{
  GPIO_MODE_INPUT   = 0,
  GPIO_MODE_OUTPUT  = 1,
  GPIO_MODE_ALT0    = 4,
  GPIO_MODE_ALT1    = 5, 
  GPIO_MODE_ALT2    = 6,
  GPIO_MODE_ALT3    = 7,
  GPIO_MODE_ALT4    = 3,
  GPIO_MODE_ALT5    = 2
};

enum BB_SPI_FLAG
{
  BB_SPI_FLAG_CPHA    = 0,
  BB_SPI_FLAG_CPOL    = 1,
  BB_SPI_FLAG_CS      = 2,
  BB_SPI_FLAG_TX_LSB  = 14,
  BB_SPI_FLAG_RX_LSB  = 15
};

// --- Structs ---
typedef struct
{

} Pin_Info_Serial;

typedef struct
{
      
} Pin_Info_I2C;

typedef struct
{
  unsigned  CS,
            MISO,
            MOSI,
            SCLK;
  
  unsigned  usage,
            delay;

  unsigned  spi_flags,
            MISO_mode,
            MOSI_mode,
            CS_mode,
            SCLK_mode;

} Pin_Info_SPI;

typedef struct 
{
  int       mode;
  int       gpio;
  uint32_t  baud;

  union 
  {
    Pin_Info_Serial serial;
    Pin_Info_I2C    i2c;
    Pin_Info_SPI    spi;
  };

} Pin_Info;

// --- AikaPi ---

class AikaPi
{
  private: 
    int m_pwm_frequency   = 5'000'000,
        m_pwm_value       = 2;

    Pin_Info m_pin_info [PI_MAX_USER_GPIO + 1];

  public:
    int m_spi_frequency = LAB_SPI_FREQUENCY;

    MemoryMap m_gpio_regs,
            m_dma_regs, 
            m_clk_regs, 
            m_pwm_regs, 
            m_spi_regs, 
            m_usec_regs,
            m_aux_regs;

    MemoryMap m_vc_mem;
       
  public:
    int   m_fifo_fd = 0;

    uint32_t m_pwm_range,  
             m_fifo_size;


    AikaPi ();
   ~AikaPi ();

    // --- General ---
    void    delay (uint32_t microseconds);

    // --- Memory ---
    void     map_periph       (MemoryMap *mp, void *phys, int size);    
    void     map_devices      ();
    void*    map_segment      (void *addr, int size);
    void     unmap_segment    ();
    void*    map_uncached_mem (MemoryMap *mp, int size);
    void     unmap_periph_mem (MemoryMap *mp);
    
    // --- Videocore Mailbox ---
    int  	   open_mbox        (void);
    void     disp_vc_msg      (VC_MSG *msgp);
    void 	   close_mbox       (int fd);
    void     unmap_segment    (void *mem, int  size);
    void*    lock_vc_mem      (int fd, int h);
    uint32_t msg_mbox         (int fd, VC_MSG *msgp);
    uint32_t fset_vc_clock    (int fd, int id, uint32_t freq);
    uint32_t free_vc_mem      (int fd, int h);
    uint32_t unlock_vc_mem    (int fd, int h);
    uint32_t alloc_vc_mem     (int fd, uint32_t size, VC_ALLOC_FLAGS flags);
  
    // --- Aux ---
    void     fail             (const char *s);
    void     terminate        (int sig);
        
    // --- DMA ---
    void     dma_enable       (int chan);
    void     dma_start        (MemoryMap *mp, int chan, DMA_CB *cbp, uint32_t csval);
    void     dma_disp         (int chan);
    void     dma_stop         (int chan);
    void     dma_wait         (int chan);
    uint32_t dma_transfer_len (int chan);
    
    // --- GPIO ---
    void      gpio_pull   (int pin, int pull);
    void      gpio_set    (int pin, int mode, int pull);
    bool      gpio_read   (int pin);
    void      gpio_write  (unsigned pin, bool value);
    void      gpio_mode   (int pin, int mode);
    uint32_t  gpio_mode   (unsigned pin);
    
    // --- SPI ---
    int       spi_init            ();
    void      spi_clear_rxtx_fifo ();
    void      spi_disp            ();
    void      spi_disable         ();
    void      spi_xfer            (uint8_t *txd, uint8_t *rxd, int length);
    void      spi_cs              (int set);
    int       spi_set_clock_rate  (int value);

    // --- Bit Bang SPI ---
    int       bb_spi_open (unsigned CS, unsigned MISO, unsigned MOSI, unsigned SCLK, unsigned baud, unsigned spi_flags);
    int       bb_spi_xfer (unsigned CS, char *txbuf, unsigned count);
    int       bb_spi_xfer (unsigned CS, char *txbuf, char *rxbuf, unsigned count);
    uint8_t   bb_spi_xfer_byte (Pin_Info *pi, char txbyte);

    void      bb_spi_start       (Pin_Info *pi);
    void      bb_spi_stop        (Pin_Info *pi);
    void      bb_spi_delay       (Pin_Info *pi);
    void      bb_spi_set_CS      (Pin_Info *pi);
    void      bb_spi_clear_CS    (Pin_Info *pi);
    void      bb_spi_set_SCLK    (Pin_Info *pi);
    void      bb_spi_clear_SCLK  (Pin_Info *pi);



    // --- Utility Peripherals ---
    void aux_spi1_master_enable ();
    void aux_spi1_master_disable ();

    // --- Utility SPI ---
    // NOTE THAT THE FOLLOWING ARE ACTUALLY SPI1, NOT SPI0. 
    // THIS IS A MISNOMER. PLEASE SEE ERRATA DOCUMENT.
    void aux_spi0_init ();
    void aux_spi0_enable ();
    void aux_spi0_disable ();
    void aux_spi0_frequency (double frequency);
    void aux_spi0_chip_selects (bool CE2, bool CE1, bool CE0);
    void aux_spi0_clear_fifos ();
    void aux_spi0_mode (uint8_t mode);
    void aux_spi0_clock_polarity (bool polarity);
    void aux_spi0_in_rising (bool value);
    void aux_spi0_out_rising (bool value);
    void aux_spi0_shift_length (uint8_t value);
    void aux_spi0_shift_out_MS_first (bool value);
    void aux_spi0_shift_in_MS_first (bool value);
    

    void aux_spi0_write (char *buf, unsigned int length);
    void aux_spi0_read (char *txbuf, char *rxbuf, unsigned int length); 
    
    void aux_spi_xfer   (uint8_t channel, char *txbuff, char *rxbuff, uint8_t count);
    void aux_spi_write  (uint8_t channel, char *txbuff,               uint8_t count);


    // --- PWM ---
    void      pwm_init         (int freq, int range, int val);
    void      pwm_start        ();
    void      pwm_stop         ();
    void      pwm_set_frequency (float frequency);

    // --- FIFO ---
    int      fifo_create      (const char *fifo_name);
    int      fifo_open_write  (const char *fifo_name);
    int      fifo_write       (int fd, void *data, int dlen);
    uint32_t fifo_get_free_space   (int fd);
    void     fifo_destroy     (char *fifo_name, int fd);
    int      fifo_is_fifo     (const char *fname);

    // --- Utility ---
    int sleep_nano (int nano);
    // int sleep_secs (int secs);
};

#endif