// #ifndef JESSPI_H
// #define JESSPI_H

// #include <cstdint>

// // Link to the BCM2385 datasheet:
// // // https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf

// #define RPI_VERSION  3

// #if RPI_VERSION == 0
//   #define PHYS_REG_BASE PI_01_REG_BASE
//   #define CLOCK_HZ	    250000000
//   #define SPI_CLOCK_HZ  400000000
// #elif RPI_VERSION == 1
//   #define PHYS_REG_BASE PI_01_REG_BASE
//   #define CLOCK_HZ	    250000000
//   #define SPI_CLOCK_HZ  250000000
// #elif RPI_VERSION == 2 || RPI_VERSION == 3
//   #define PHYS_REG_BASE PI_23_REG_BASE
//   #define CLOCK_HZ	    250000000
//   #define SPI_CLOCK_HZ  250000000
// #elif RPI_VERSION == 4
//   #define PHYS_REG_BASE PI_4_REG_BASE
//   #define CLOCK_HZ	    375000000
//   #define SPI_CLOCK_HZ  200000000
// #endif

// // Location of peripheral registers in physical memory
// #define PI_01_REG_BASE 0x20000000 // Pi Zero or 1
// #define PI_23_REG_BASE 0x3F000000 // Pi 2 or 3
// #define PI_4_REG_BASE  0xFE000000 // Pi 4

// // Location of peripheral registers in bus memory
// #define BUS_REG_BASE   0x7E000000



// // --- ADC ---
// // ADC and DAC chip-enables
// constexpr int ADC_CE_NUM = 0;
// constexpr int DAC_CE_NUM = 1;



// // --- SPI ---
// // Page 148

// // SPI clock frequency
// #define MIN_SPI_FREQ    10000
// #define MAX_SPI_FREQ    32000000
// #define SPI_FREQ        32000000

// // SPI 0 pin definitions
// #define SPI0_CE0_PIN    8
// #define SPI0_CE1_PIN    7
// #define SPI0_MISO_PIN   9
// #define SPI0_MOSI_PIN   10
// #define SPI0_SCLK_PIN   11

// // SPI registers and constants
// constexpr int SPI0_BASE = (PHYS_REG_BASE + 0x204000);
// constexpr int SPI_CS    = 0x00;
// constexpr int SPI_FIFO  = 0x04;
// constexpr int SPI_CLK   = 0x08;
// constexpr int SPI_DLEN  = 0x0c;
// constexpr int SPI_LTOH  = 0x10;
// constexpr int SPI_DC    = 0x14;

// #define SPI_FIFO_CLR    (3 << 4)
// #define SPI_RX_FIFO_CLR (2 << 4)
// #define SPI_TX_FIFO_CLR (1 << 4)
// #define SPI_TFR_ACT     (1 << 7)
// #define SPI_DMA_EN      (1 << 8)
// #define SPI_AUTO_CS     (1 << 11)
// #define SPI_RXD         (1 << 17)
// #define SPI_CE0         0
// #define SPI_CE1         1



// // --- Auxiliaries---
// constexpr int SPI1_SCLK_PIN = 21;
// constexpr int SPI1_MOSI_PIN = 20;
// constexpr int SPI1_MISO_PIN = 19;
// constexpr int SPI1_CE2_PIN  = 16;

// constexpr int AUX_BASE            = (PHYS_REG_BASE + 0x215000);
// constexpr int AUX_ENABLES         = 0x04;
// constexpr int AUX_SPI0_CNTL0_REG  = 0x80; 
// constexpr int AUX_SPI0_CNTL1_REG  = 0x84;
// constexpr int AUX_SPI0_STAT_REG   = 0x88;
// constexpr int AUX_SPI0_IO_REG     = 0xA0;
// constexpr int AUX_SPI0_PEEK_REG   = 0x8C;
// constexpr int AUX_SPI0_TXHOLD_REG = 0xB0;

// constexpr int AUX_SPI1_ENABLE     = (1 << 1); 



// // --- GPIO --- 
// constexpr int GPIO_BASE       = (PHYS_REG_BASE + 0x200000);
// constexpr int GPIO_MODE0      = 0x00;
// constexpr int GPIO_SET0       = 0x1c;
// constexpr int GPIO_CLR0       = 0x28;
// constexpr int GPIO_LEV0       = 0x34;
// constexpr int GPIO_GPPUD      = 0x94;
// constexpr int GPIO_GPPUDCLK0  = 0x98;

// constexpr int GPIO_IN     = 0;
// constexpr int GPIO_OUT    = 1;
// constexpr int GPIO_ALT0   = 4;
// constexpr int GPIO_ALT1   = 5;
// constexpr int GPIO_ALT2   = 6;
// constexpr int GPIO_ALT3   = 7;
// constexpr int GPIO_ALT4   = 3;
// constexpr int GPIO_ALT5   = 2;
// constexpr int GPIO_NOPULL = 0;
// constexpr int GPIO_PULLDN = 1;
// constexpr int GPIO_PULLUP = 2;



// // --- Clock ---
// constexpr int CLK_BASE      = (PHYS_REG_BASE + 0x101000);
// constexpr int CLK_PWM_CTL   = 0xa0;
// constexpr int CLK_PWM_DIV   = 0xa4;
// constexpr int CLK_PASSWD    = 0x5a000000;
// constexpr int PWM_CLOCK_ID  = 0xa;



// // --- Microsecond Timer ---
// constexpr int USEC_BASE = (PHYS_REG_BASE + 0x3000);
// constexpr int USEC_TIME = 0x04;



// // --- PWM ---
// constexpr int PWM_VALUE = 2;

// constexpr int PWM_BASE  = (PHYS_REG_BASE + 0x20C000);
// constexpr int PWM_CTL   = 0x00;   // Control
// constexpr int PWM_STA   = 0x04;   // Status
// constexpr int PWM_DMAC  = 0x08;   // DMA control
// constexpr int PWM_RNG1  = 0x10;   // Channel 1 range
// constexpr int PWM_DAT1  = 0x14;   // Channel 1 data
// constexpr int PWM_FIF1  = 0x18;   // Channel 1 fifo
// constexpr int PWM_RNG2  = 0x20;   // Channel 2 range
// constexpr int PWM_DAT2  = 0x24;   // Channel 2 data

// // PWM register values
// constexpr int PWM_CTL_RPTL1 = (1 << 2);  // Chan 1: repeat last data when FIFO empty
// constexpr int PWM_CTL_USEF1 = (1 << 5);  // Chan 1: use FIFO
// constexpr int PWM_DMAC_ENAB = (1 << 31); // Start PWM DMA
// constexpr int PWM_ENAB      = 1;         // Enable PWM
// constexpr int PWM_PIN       = 18;        // GPIO pin for PWM output

// // If non-zero, set PWM clock using VideoCore mailbox
// constexpr int USE_VC_CLOCK_SET = 0;



// // --- VideoCore Mailbox ---
// // Mailbox command/response structure
// typedef struct 
// {
//   uint32_t len,         // Overall length (bytes)
//            req,         // Zero for request, 1<<31 for response
//            tag,         // Command number
//            blen,        // Buffer length (bytes)
//            dlen,        // Data length (bytes)
//            uints[32-5]; // Data (108 bytes maximum)
// } VC_MSG __attribute__ ((aligned (16)));

// // Videocore mailbox memory allocation flags, see:
// // https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
// typedef enum 
// {
//   MEM_DISCARDABLE      = 1 << 0, // can be resized to 0 at any time. Use for cached data
//   MEM_NORMAL           = 0 << 2, // normal allocating alias. Don't use from ARM
//   MEM_DIRECT           = 1 << 2, // 0xC alias uncached
//   MEM_COHERENT         = 2 << 2, // 0x8 alias. Non-allocating in L2 but coherent
//   MEM_ZERO             = 1 << 4, // initialise buffer to all zeros
//   MEM_NO_INIT          = 1 << 5, // don't initialise (default is initialise to all ones)
//   MEM_HINT_PERMALOCK   = 1 << 6, // Likely to be locked for long periods of time
//   MEM_L1_NONALLOCATING = (MEM_DIRECT | MEM_COHERENT) // Allocating in L2
// } VC_ALLOC_FLAGS;

// // VC flags for unchached DMA memory
// #define DMA_MEM_FLAGS static_cast<VC_ALLOC_FLAGS>((MEM_COHERENT | MEM_ZERO))


// // DMA
// // DMA channels and data requests
// #define DMA_CHAN_A      7
// #define DMA_CHAN_B      8
// #define DMA_CHAN_C      9
// #define DMA_PWM_DREQ    5
// #define DMA_SPI_TX_DREQ 6
// #define DMA_SPI_RX_DREQ 7
// #define DMA_BASE        (PHYS_REG_BASE + 0x007000)

// // DMA register addresses offset by 0x100 * chan_num
// #define DMA_CS          0x00
// #define DMA_CONBLK_AD   0x04
// #define DMA_TI          0x08
// #define DMA_SRCE_AD     0x0c
// #define DMA_DEST_AD     0x10
// #define DMA_TXFR_LEN    0x14
// #define DMA_STRIDE      0x18
// #define DMA_NEXTCONBK   0x1c
// #define DMA_DEBUG       0x20
// #define DMA_REG(ch, r)  ((r) == DMA_ENABLE ? DMA_ENABLE : (ch) * 0x100 + (r))
// #define DMA_ENABLE      0xff0

// // DMA register values
// #define DMA_WAIT_RESP   (1 << 3)
// #define DMA_CB_DEST_INC (1 << 4)
// #define DMA_DEST_DREQ   (1 << 6)
// #define DMA_CB_SRCE_INC (1 << 8)
// #define DMA_SRCE_DREQ   (1 << 10)
// #define DMA_PRIORITY(n) ((n) << 16)


// // Memory
// constexpr int MEM_PAGE_SIZE = 0x1000;


// struct MemoryMap
// {
//   int fd,   // file descriptor
//       h,    // memory handle
//       size; // memory size in bytes
  
//   void *bus,  // bus address
//        *virt, // virtual address
//        *phys; // physical address
// };



// class JessPi
// {
//   private:
//     MemoryMap m_reg_aux,
//               m_reg_gpio,
//               m_reg_dma,
//               m_reg_spi,
//               m_reg_clk,
//               m_reg_pwm,
//               m_reg_usec;

//   public:
//     JessPi ();
//    ~JessPi ();

//     int initialize ();
//     int terminate ();

//    // SPI

//    // AUX SPI 

//     // Utility
//     void  utility_map_devices ();
//     void  utility_map_peripheral (MemoryMap *_Memory_Map, void *phys_add, int size);
//     int   utility_page_roundup (int page_size); 


// };




// #endif  JESSPI_H