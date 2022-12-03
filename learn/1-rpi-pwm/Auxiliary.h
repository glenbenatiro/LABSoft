#ifndef AUX_H
#define AUX_H

#include <cstdint>


double aux_unit_label_to_unit_scaler (const char *label);
double aux_unit_label_to_unit_power (const char *label);

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


// --- most likely variables to change --- 
#define SAMPLE_RATE  200000
#define SAMPLE_COUNT 3200   
#define RPI_VERSION  3

#define IN_CHANS     1
#define OSCILLOSCOPE_FIFO_NAME    "/tmp/adc.fifo"

#define FMT_USEC     1

#define VERBOSE      0
#define LOCKSTEP     0

#define DATA_FORMAT  0

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
  MEM_DISCARDABLE      = 1 << 0,                               // can be resized to 0 at any time. Use for cached data
  MEM_NORMAL           = 0 << 2,                               // normal allocating alias. Don't use from ARM
  MEM_DIRECT           = 1 << 2,                               // 0xC alias uncached
  MEM_COHERENT         = 2 << 2,                               // 0x8 alias. Non-allocating in L2 but coherent
  MEM_ZERO             = 1 << 4,                               // initialise buffer to all zeros
  MEM_NO_INIT          = 1 << 5,                               // don't initialise (default is initialise to all ones)
  MEM_HINT_PERMALOCK   = 1 << 6,                               // Likely to be locked for long periods of time
  MEM_L1_NONALLOCATING = (MEM_DIRECT | MEM_COHERENT) // Allocating in L2
} VC_ALLOC_FLAGS;

// VC flags for unchached DMA memory
#define DMA_MEM_FLAGS static_cast<VC_ALLOC_FLAGS>((MEM_COHERENT | MEM_ZERO))







// --- PWM ---
// PWM definitions: divisor, and reload value
constexpr int LAB_PWM_FREQUENCY      = 5'000'000; // this is to be extracted from 25MHz master clk
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