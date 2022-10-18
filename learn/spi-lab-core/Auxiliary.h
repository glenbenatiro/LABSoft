#ifndef AUX_H
#define AUX_H

#include <cstdint>


double aux_unit_label_to_unit_scaler (const char *label);
double aux_unit_label_to_unit_power (const char *label);


// --- most likely variables to change --- 
#define SAMPLE_RATE  200000
#define SAMPLE_COUNT 3200   
#define RPI_VERSION  3

#define IN_CHANS     1
#define OSCILLOSCOPE_FIFO_NAME    "/tmp/adc.fifo"

#define FMT_USEC     1

#define VERBOSE      0
#define LOCKSTEP     0

#define DATA_FORMAT  1

#define DISPLAY_UPDATE_RATE 0.04 // 25 Hz/FPS

// if non-zero, print debug info
#define DEBUG       0

// Buffer for streaming output, and raw Rx data
#define STREAM_BUFFLEN  10000

// for oscilloscope streaming
#define OSCILLOSCOPE_STREAMING_SAMPLE_RATE  200000
#define OSCILLOSCOPE_STREAMING_SAMPLE_COUNT 10


// --- set RPi clock and spi speed, depending on model ---
#if RPI_VERSION == 0
  #define PHYS_REG_BASE PI_01_REG_BASE
  #define CLOCK_HZ	  250000000
  #define SPI_CLOCK_HZ  400000000
#elif RPI_VERSION == 1
  #define PHYS_REG_BASE PI_01_REG_BASE
  #define CLOCK_HZ	  250000000
  #define SPI_CLOCK_HZ  250000000
#elif RPI_VERSION == 2 || RPI_VERSION == 3
  #define PHYS_REG_BASE PI_23_REG_BASE
  #define CLOCK_HZ	  250000000
  constexpr int SPI_CLOCK_HZ = 250'000'000;
#elif RPI_VERSION == 4
  #define PHYS_REG_BASE PI_4_REG_BASE
  #define CLOCK_HZ	  375000000
  #define SPI_CLOCK_HZ  200000000
#endif

// Location of peripheral registers in physical memory
#define PI_01_REG_BASE 0x20000000 // Pi Zero or 1
#define PI_23_REG_BASE 0x3F000000 // Pi 2 or 3
#define PI_4_REG_BASE  0xFE000000 // Pi 4

// Location of peripheral registers in bus memory
#define BUS_REG_BASE   0x7E000000

// --- SPI ---
// https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
// page 148

// SPI clock frequency
#define MIN_SPI_FREQ    10000
#define MAX_SPI_FREQ    32000000
#define SPI_FREQ        32000000

// SPI 0 pin definitions
#define SPI0_CE0_PIN    8
#define SPI0_CE1_PIN    7
#define SPI0_MISO_PIN   9
#define SPI0_MOSI_PIN   10
#define SPI0_SCLK_PIN   11

// SPI registers and constants
constexpr int SPI0_BASE = (PHYS_REG_BASE + 0x204000);
constexpr int SPI_CS    = 0x00;
constexpr int SPI_FIFO  = 0x04;
constexpr int SPI_CLK   = 0x08;
constexpr int SPI_DLEN  = 0x0c;
constexpr int SPI_LTOH  = 0x10;
constexpr int SPI_DC    = 0x14;

#define SPI_FIFO_CLR    (3 << 4)
#define SPI_RX_FIFO_CLR (2 << 4)
#define SPI_TX_FIFO_CLR (1 << 4)
#define SPI_TFR_ACT     (1 << 7)
#define SPI_DMA_EN      (1 << 8)
#define SPI_AUTO_CS     (1 << 11)
#define SPI_RXD         (1 << 17)
#define SPI_CE0         0
#define SPI_CE1         1



// --- Aux ---
// fcntl constant to get free FIFO length
#define F_GETPIPE_SZ    1032



// --- ADC ---
// ADC and DAC chip-enables
#define ADC_CE_NUM      0
#define DAC_CE_NUM      1

// Definitions for 2 bytes per ADC sample (11-bit)
#define ADC_REQUEST(c)  {0xc0 | (c)<<5, 0x00}
#define ADC_VOLTAGE(n)  (((n) * 3.3) / 2048.0)
#define ADC_MILLIVOLTS(n) ((int)((((n) * 3300) + 1024) / 2048))
#define ADC_RAW_VAL(d) (((uint16_t)(d) & 0x003f) << 6) | (((uint16_t)(d) & 0xfc00) >> 10)



// --- Helper Macros for Register Access ---
// Get virtual 8 and 32-bit pointers to register
#define REG8(m, x)          ((volatile uint8_t *)  ((uint32_t)(m.virt) + (uint32_t)(x)))
#define REG32(m, x)         ((volatile uint32_t *) ((uint32_t)(m.virt) + (uint32_t)(x)))

// Get bus address of register
#define REG_BUS_ADDR(m, x)  ((uint32_t)(m.bus)  + (uint32_t)(x))

// Convert uncached memory virtual address to bus address
#define MEM_BUS_ADDR(mp, a) ((uint32_t)a - (uint32_t)mp->virt + (uint32_t)mp->bus)

// Convert bus address to physical address (for mmap)
#define BUS_PHYS_ADDR(a)    ((void *)((uint32_t)(a) & ~0xC0000000))



// --- Non-cached memory size ---
#define MAX_SAMPS   16384 // number of unique samples
#define SAMP_SIZE   4 // size of each unique sample in bytes (32 bits)
#define BUFF_LEN    (MAX_SAMPS * SAMP_SIZE) // in bytes
#define MAX_BUFFS   2
#define VC_MEM_SIZE (PAGE_SIZE + (BUFF_LEN * MAX_BUFFS)) // in bytes


// --- Memory ---
// Structure for mapped peripheral or memory
typedef struct {
  int fd,     // File descriptor
      h,      // Memory handle
      size;   // Memory size in bytes
    
  void *bus,  // Bus address
       *virt, // Virtual address
       *phys; // Physical address
} MEM_MAP;

// Round up to nearest page
#define PAGE_ROUNDUP(n) ((n) % PAGE_SIZE == 0 ? (n) : ((n) + PAGE_SIZE) & ~(PAGE_SIZE - 1))

// Size of memory page
#define PAGE_SIZE      0x1000



// --- VIDEOCORE MAILBOX ---
// Mailbox command/response structure
typedef struct {
  uint32_t len,         // Overall length (bytes)
           req,         // Zero for request, 1<<31 for response
           tag,         // Command number
           blen,        // Buffer length (bytes)
           dlen,        // Data length (bytes)
           uints[32-5]; // Data (108 bytes maximum)
  
} VC_MSG __attribute__ ((aligned (16)));

// Videocore mailbox memory allocation flags, see:
// https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
typedef enum {
  MEm_DISCARDABLE      = 1 << 0,                               // can be resized to 0 at any time. Use for cached data
  MEm_NORMAL           = 0 << 2,                               // normal allocating alias. Don't use from ARM
  MEm_DIRECT           = 1 << 2,                               // 0xC alias uncached
  MEm_COHERENT         = 2 << 2,                               // 0x8 alias. Non-allocating in L2 but coherent
  MEm_ZERO             = 1 << 4,                               // initialise buffer to all zeros
  MEm_NO_INIT          = 1 << 5,                               // don't initialise (default is initialise to all ones)
  MEm_HINT_PERMALOCK   = 1 << 6,                               // Likely to be locked for long periods of time
  MEm_L1_NONALLOCATING = (MEm_DIRECT | MEm_COHERENT) // Allocating in L2
} VC_ALLOC_FLAGS;

// VC flags for unchached DMA memory
#define DMA_MEM_FLAGS static_cast<VC_ALLOC_FLAGS>((MEm_COHERENT | MEm_ZERO))



// --- DMA ---
// DMA transfer information for PWM and SPI
#define PWM_TI          (DMA_DEST_DREQ | (DMA_PWM_DREQ << 16) | DMA_WAIT_RESP)
#define SPI_RX_TI       (DMA_SRCE_DREQ | (DMA_SPI_RX_DREQ << 16) | DMA_WAIT_RESP | DMA_CB_DEST_INC)
#define SPI_TX_TI       (DMA_DEST_DREQ | (DMA_SPI_TX_DREQ << 16) | DMA_WAIT_RESP | DMA_CB_SRCE_INC)

// DMA control block macros
#define NUM_CBS         10
#define REG(r, a)       REG_BUS_ADDR(r, a)
#define MEM(m, a)       MEM_BUS_ADDR(m, a)
#define CBS(n)          MEM_BUS_ADDR(mp, &dp->cbs[(n)])

// DMA channels and data requests
#define DMA_CHAN_A      7
#define DMA_CHAN_B      8
#define DMA_CHAN_C      9
#define DMA_PWM_DREQ    5
#define DMA_SPI_TX_DREQ 6
#define DMA_SPI_RX_DREQ 7
#define DMA_BASE        (PHYS_REG_BASE + 0x007000)

// DMA register addresses offset by 0x100 * chan_num
#define DMA_CS          0x00
#define DMA_CONBLK_AD   0x04
#define DMA_TI          0x08
#define DMA_SRCE_AD     0x0c
#define DMA_DEST_AD     0x10
#define DMA_TXFR_LEN    0x14
#define DMA_STRIDE      0x18
#define DMA_NEXTCONBK   0x1c
#define DMA_DEBUG       0x20
#define DMA_REG(ch, r)  ((r) == DMA_ENABLE ? DMA_ENABLE : (ch) * 0x100 + (r))
#define DMA_ENABLE      0xff0

// DMA register values
#define DMA_WAIT_RESP   (1 << 3)
#define DMA_CB_DEST_INC (1 << 4)
#define DMA_DEST_DREQ   (1 << 6)
#define DMA_CB_SRCE_INC (1 << 8)
#define DMA_SRCE_DREQ   (1 << 10)
#define DMA_PRIORITY(n) ((n) << 16)

// DMA control block (must be 32-byte aligned)
// https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
// 4.2.1.1 Control Block Data Structure, page 40
typedef struct {
  uint32_t ti,      // Transfer info
           srce_ad, // Source address
           dest_ad, // Destination address
           tfr_len, // Transfer length in bytes
           stride,  // Transfer stride
           next_cb, // Next control block
           debug,   // Debug register, zero in control block
           unused;
} DMA_CB __attribute__ ((aligned(32)));

typedef struct {
  DMA_CB cbs[NUM_CBS];

  uint32_t samp_size, 
           pwm_val, 
           adc_csd, 
           txd[2];

  volatile uint32_t 
           usecs[2], 
           states[2], 
           rxd1[MAX_SAMPS], 
           rxd2[MAX_SAMPS];
} 
ADC_DMA_DATA;




// --- PWM ---
// PWM definitions: divisor, and reload value
#define PWM_FREQ      1000000
#define PWM_VALUE     2

// If non-zero, set PWM clock using VideoCore mailbox
#define USE_VC_CLOCK_SET 0

// PWM controller registers
#define PWM_BASE      (PHYS_REG_BASE + 0x20C000)
#define PWM_CTL       0x00   // Control
#define PWM_STA       0x04   // Status
#define PWM_DMAC      0x08   // DMA control
#define PWM_RNG1      0x10   // Channel 1 range
#define PWM_DAT1      0x14   // Channel 1 data
#define PWM_FIF1      0x18   // Channel 1 fifo
#define PWM_RNG2      0x20   // Channel 2 range
#define PWM_DAT2      0x24   // Channel 2 data

// PWM register values
#define PWM_CTL_RPTL1 (1 << 2)  // Chan 1: repeat last data when FIFO empty
#define PWM_CTL_USEF1 (1 << 5)  // Chan 1: use FIFO
#define PWM_DMAC_ENAB (1 << 31) // Start PWM DMA
#define PWM_ENAB      1         // Enable PWM
#define PWM_PIN       18        // GPIO pin for PWM output



// --- GPIO --- 
// GPIO register definitions
#define GPIO_BASE      (PHYS_REG_BASE + 0x200000)
#define GPIO_MODE0     0x00
#define GPIO_SET0      0x1c
#define GPIO_CLR0      0x28
#define GPIO_LEV0      0x34
#define GPIO_GPPUD     0x94
#define GPIO_GPPUDCLK0 0x98

// GPIO I/O definitions
#define GPIO_IN         0
#define GPIO_OUT        1
#define GPIO_ALT0       4
#define GPIO_ALT1       5
#define GPIO_ALT2       6
#define GPIO_ALT3       7
#define GPIO_ALT4       3
#define GPIO_ALT5       2
#define GPIO_NOPULL     0
#define GPIO_PULLDN     1
#define GPIO_PULLUP     2



// --- Clock ---
// Clock registers and values
#define CLK_BASE     (PHYS_REG_BASE + 0x101000)
#define CLK_PWM_CTL  0xa0
#define CLK_PWM_DIV  0xa4
#define CLK_PASSWD   0x5a000000
#define PWM_CLOCK_ID 0xa



// --- Microsecond Timer ---
// Microsecond timer
#define USEC_BASE (PHYS_REG_BASE + 0x3000)
#define USEC_TIME 0x04



#endif