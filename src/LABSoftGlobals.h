#ifndef LABSOFTGLOBALS_H
#define LABSOFTGLOBALS_H

#include <epoxy/gl.h>

// --- CHANGEABLE GLOBALS ---

#define RPI_VERSION     3

// FIFO name
const char* const FIFO_NAME    {"/tmp/adc.fifo"};
const char* const BUILDER_NAME {"../ui/lab.glade"};

#define STREAM_BUFFLEN	BUFF_LEN * MAX_BUFFS
#define	VERBOSE			    0
#define	LOCKSTEP		    0

#define FMT_USEC        1
#define SAMPLE_COUNT	  3200	 // number of samples per block

// SPI clock frequency
#define MIN_SPI_FREQ    1000000
#define MAX_SPI_FREQ    20000000
#define SPI_FREQ        20000000

// input channels
#define IN_CHANS		1

// Non-cached memory size
#define MAX_SAMPS       16384
#define SAMP_SIZE       4
#define BUFF_LEN        (MAX_SAMPS * SAMP_SIZE)
#define MAX_BUFFS       2
#define VC_MEM_SIZE     (PAGE_SIZE + (BUFF_LEN * MAX_BUFFS))

// Default & max sample rate (samples/sec)
#define SAMPLE_RATE     800000 // it is x4 of value placed   
#define MAX_SAMPLE_RATE 800000

// PWM definitions: divisor, and reload value
#define PWM_FREQ        10000000
#define PWM_VALUE       2

// ADC sample size (2 bytes, with 11 data bits)
#define ADC_RAW_LEN     2

// ADC and DAC chip-enables
#define ADC_CE_NUM      0
#define DAC_CE_NUM      1

// If non-zero, print debug information
#define DEBUG           0

// If non-zero, set PWM clock using VideoCore mailbox
#define USE_VC_CLOCK_SET 0

// Size of memory page
#define PAGE_SIZE       0x1000

#define VERSION "0.1"


// ----------

#if RPI_VERSION == 0
  #define PHYS_REG_BASE PI_01_REG_BASE
  #define CLOCK_HZ      250000000
  #define SPI_CLOCK_HZ  400000000
#elif RPI_VERSION == 1
  #define PHYS_REG_BASE PI_01_REG_BASE
  #define CLOCK_HZ      250000000
  #define SPI_CLOCK_HZ  250000000
#elif RPI_VERSION == 2 || RPI_VERSION == 3
  #define PHYS_REG_BASE PI_23_REG_BASE
  #define CLOCK_HZ      250000000
  #define SPI_CLOCK_HZ  250000000
#elif RPI_VERSION == 4
  #define PHYS_REG_BASE PI_4_REG_BASE
  #define CLOCK_HZ      375000000
  #define SPI_CLOCK_HZ  200000000
#endif

// Location of peripheral registers in physical memory
#define PI_01_REG_BASE  0x20000000  // Pi Zero or 1
#define PI_23_REG_BASE  0x3F000000  // Pi 2 or 3
#define PI_4_REG_BASE   0xFE000000  // Pi 4

// Location of peripheral registers in bus memory
#define BUS_REG_BASE    0x7E000000

// Structure for mapped peripheral or memory
typedef struct {
    int   fd,   // File descriptor
          h,    // Memory handle
          size; // Memory size
    void *bus,  // Bus address
         *virt, // Virtual address
         *phys; // Physical address
} MEM_MAP;

const char* const dma_regstrs[] = 
  {
    "DMA CS",
    "CB_AD", 
    "TI", 
    "SRCE_AD", 
    "DEST_AD",
    "TFR_LEN", 
    "STRIDE", 
    "NEXT_CB", 
    "DEBUG", 
    ""
  };
  
const char* const spi_regstrs[] = 
  {
    "CS", 
    "FIFO", 
    "CLK", 
    "DLEN", 
    "LTOH", 
    "DC", 
    ""
  };



// --- HELPFUL MACROS ---
// Get virtual 8 and 32-bit pointers to register
#define REG8(m, x)  ((volatile uint8_t *) ((uint32_t)(m.virt)+(uint32_t)(x)))
#define REG32(m, x) ((volatile uint32_t *)((uint32_t)(m.virt)+(uint32_t)(x)))

// Get bus address of register
#define REG_BUS_ADDR(m, x)  ((uint32_t)(m.bus)  + (uint32_t)(x))

// Convert uncached memory virtual address to bus address
#define MEM_BUS_ADDR(mp, a) ((uint32_t)a-(uint32_t)mp->virt+(uint32_t)mp->bus)

// Convert bus address to physical address (for mmap)
#define BUS_PHYS_ADDR(a)    ((void *)((uint32_t)(a)&~0xC0000000))

// Round up to nearest page
#define PAGE_ROUNDUP(n) ((n)%PAGE_SIZE==0 ? (n) : ((n)+PAGE_SIZE)&~(PAGE_SIZE-1))

// Definitions for 2 bytes per ADC sample (11-bit)
#define ADC_REQUEST(c)  {0xc0 | (c)<<5, 0x00}
#define ADC_VOLTAGE(n)  (((n) * 5.0) / 4096.0)
#define ADC_MILLIVOLTS(n) ((int)((((n) * 5000) + 1024) / 4096))
//#define ADC_RAW_VAL(d)  (((uint16_t)(d)<<8 | (uint16_t)(d)>>8) & 0x7ff)
#define ADC_RAW_VAL(d)  (((uint16_t)(d) & 0x003f) << 6) | (((uint16_t)(d) & 0xfc00) >> 10)



// --- GPIO ---
// GPIO register definitions
#define GPIO_BASE       (PHYS_REG_BASE + 0x200000)
#define GPIO_MODE0      0x00
#define GPIO_SET0       0x1c
#define GPIO_CLR0       0x28
#define GPIO_LEV0       0x34
#define GPIO_GPPUD      0x94
#define GPIO_GPPUDCLK0  0x98

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



// --- VIDEOCORE MAILBOX ---
// Mailbox command/response structure
typedef struct {
    uint32_t len,   // Overall length (bytes)
        req,        // Zero for request, 1<<31 for response
        tag,        // Command number
        blen,       // Buffer length (bytes)
        dlen;       // Data length (bytes)
        uint32_t uints[32-5];   // Data (108 bytes maximum)
} VC_MSG __attribute__ ((aligned (16)));

// Videocore mailbox memory allocation flags, see:
// https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
typedef enum {
    MEM_FLAG_DISCARDABLE    = 1<<0, // can be resized to 0 at any time. Use for cached data
    MEM_FLAG_NORMAL         = 0<<2, // normal allocating alias. Don't use from ARM
    MEM_FLAG_DIRECT         = 1<<2, // 0xC alias uncached
    MEM_FLAG_COHERENT       = 2<<2, // 0x8 alias. Non-allocating in L2 but coherent
    MEM_FLAG_ZERO           = 1<<4, // initialise buffer to all zeros
    MEM_FLAG_NO_INIT        = 1<<5, // don't initialise (default is initialise to all ones)
    MEM_FLAG_HINT_PERMALOCK = 1<<6, // Likely to be locked for long periods of time
    MEM_FLAG_L1_NONALLOCATING=(MEM_FLAG_DIRECT | MEM_FLAG_COHERENT) // Allocating in L2
} VC_ALLOC_FLAGS;

// VC flags for unchached DMA memory
#define DMA_MEM_FLAGS (MEM_FLAG_COHERENT | MEM_FLAG_ZERO)



// --- DMA ---
// DMA control block macros
#define NUM_CBS         10
#define REG(r, a)       REG_BUS_ADDR(r, a)
#define MEM(m, a)       MEM_BUS_ADDR(m, a)
#define CBS(n)          MEM_BUS_ADDR(mp, &dp->cbs[(n)])

// DMA control block (must be 32-byte aligned)
typedef struct {
    uint32_t ti,    // Transfer info
        srce_ad,    // Source address
        dest_ad,    // Destination address
        tfr_len,    // Transfer length
        stride,     // Transfer stride
        next_cb,    // Next control block
        debug,      // Debug register, zero in control block
        unused;
} DMA_CB __attribute__ ((aligned(32)));

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
#define DMA_REG(ch, r)  ((r)==DMA_ENABLE ? DMA_ENABLE : (ch)*0x100+(r))
#define DMA_ENABLE      0xff0

// DMA register values
#define DMA_WAIT_RESP   (1 << 3)
#define DMA_CB_DEST_INC (1 << 4)
#define DMA_DEST_DREQ   (1 << 6)
#define DMA_CB_SRCE_INC (1 << 8)
#define DMA_SRCE_DREQ   (1 << 10)
#define DMA_PRIORITY(n) ((n) << 16)

// DMA transfer information for PWM and SPI
#define PWM_TI          (DMA_DEST_DREQ | (DMA_PWM_DREQ << 16) | DMA_WAIT_RESP)
#define SPI_RX_TI       (DMA_SRCE_DREQ | (DMA_SPI_RX_DREQ << 16) | DMA_WAIT_RESP | DMA_CB_DEST_INC)
#define SPI_TX_TI       (DMA_DEST_DREQ | (DMA_SPI_TX_DREQ << 16) | DMA_WAIT_RESP | DMA_CB_SRCE_INC)



// --- PWM ---
// PWM controller registers
#define PWM_BASE        (PHYS_REG_BASE + 0x20C000)
#define PWM_CTL         0x00   // Control
#define PWM_STA         0x04   // Status
#define PWM_DMAC        0x08   // DMA control
#define PWM_RNG1        0x10   // Channel 1 range
#define PWM_DAT1        0x14   // Channel 1 data
#define PWM_FIF1        0x18   // Channel 1 fifo
#define PWM_RNG2        0x20   // Channel 2 range
#define PWM_DAT2        0x24   // Channel 2 data

// PWM register values
#define PWM_CTL_RPTL1   (1<<2)  // Chan 1: repeat last data when FIFO empty
#define PWM_CTL_USEF1   (1<<5)  // Chan 1: use FIFO
#define PWM_DMAC_ENAB   (1<<31) // Start PWM DMA
#define PWM_ENAB        1       // Enable PWM
#define PWM_PIN         18      // GPIO pin for PWM output



// --- CLOCK ---
// Clock registers and values
#define CLK_BASE        (PHYS_REG_BASE + 0x101000)
#define CLK_PWM_CTL     0xa0
#define CLK_PWM_DIV     0xa4
#define CLK_PASSWD      0x5a000000
#define PWM_CLOCK_ID    0xa



// --- SPI ---
// SPI 0 pin definitions
#define SPI0_CE0_PIN    8
#define SPI0_CE1_PIN    7
#define SPI0_MISO_PIN   9
#define SPI0_MOSI_PIN   10
#define SPI0_SCLK_PIN   11

// SPI registers
#define SPI0_BASE       (PHYS_REG_BASE + 0x204000)
#define SPI_CS          0x00
#define SPI_FIFO        0x04
#define SPI_CLK         0x08
#define SPI_DLEN        0x0c
#define SPI_LTOH        0x10
#define SPI_DC          0x14
#define SPI_FIFO_CLR    (3 << 4)
#define SPI_RX_FIFO_CLR (2 << 4)
#define SPI_TX_FIFO_CLR (1 << 4)
#define SPI_TFR_ACT     (1 << 7)
#define SPI_DMA_EN      (1 << 8)
#define SPI_AUTO_CS     (1 << 11)
#define SPI_RXD         (1 << 17)
#define SPI_CE0         0
#define SPI_CE1         1



// --- US TIMER ---
// Microsecond timer
#define USEC_BASE       (PHYS_REG_BASE + 0x3000)
#define USEC_TIME       0x04

//

// Macros for GLSL strings
#define VALSTR(s) #s
#define SL_DEF(s) "#define " #s " " VALSTR(s) "\n"
#define SL(s) s "\n"

#define USE_ES          1

#define OSC_FIFO_NAME   "/tmp/adc.fifo"
//#define WIN_SIZE        640, 480
#define MAX_CHANS       16      // Max number of I/P chans
#define NUM_CHANS       1       // Default number of I/P chans
#define GRID_CHAN       0       // Channel num used by grid
#define TRACE1_CHAN     1       // Channel num used by first trace
#define MAX_TRACES      (TRACE1_CHAN + MAX_CHANS)
#define X_MARGIN        10      // Margin added around the grid
#define Y_MARGIN        10
#define LINE_WIDTH      2.0     // Width of plot line
#define NORM_XMIN      -1.0     // Normalised window coordinates
#define NORM_XMAX       1.0
#define NORM_YMIN      -1.0
#define NORM_YMAX       1.0
#define TRACE_YMAX      2.0     // Max analog value for each trace
#define GRID_DIVS       10,8    // Number of divisions in grid
#define CLEAR_COLOUR    0.0, 0.0, 0.0, 0.0 // Normalised background colour
#define MAX_VALS        10000   // Maximum number of I/P values
#define NUM_VALS        1000    // Default number of I/P values
#define ZEN(z)          ((z)+0.1)   // Z-value to enable drawing
#define TIMER_MSEC      1000        // Time for FPS calculation
#define GLUT_MODE       GLUT_SINGLE // Single or double-buffering

// GLSL fragment shader
char const frag_shader[] =
#if USE_ES
    SL("#version 300 es")
    SL("precision mediump float;")
    SL("flat in vec4 f_color;")
    SL("layout(location = 0) out vec4 fragColor;")
    SL("void main(void) {")
    SL("    fragColor = f_color;")
    SL("}");
#else
    SL("#version 120")
    SL("varying vec4 f_color;")
    SL("void main(void) {")
    SL("    gl_FragColor = f_color;")
    SL("}");
#endif

// GLSL vertex shader
char const vert_shader[] =
#if USE_ES
    SL("#version 300 es")
    SL("precision mediump float;")
    SL("in vec3 coord3d;")
    SL("flat out vec4 f_color;")
#else
    SL("#version 120")
    SL("attribute vec3 coord3d;")
    SL("varying vec4 f_color;")
#endif
    SL_DEF(MAX_TRACES)
    SL("uniform vec4 u_colours[MAX_TRACES];")
    SL("uniform vec2 u_scoffs[MAX_TRACES];")
    SL("vec2 scoff;")
    SL("int zint;")
    SL("bool zen;")
    SL("void main(void) {")
    SL("    zint = int(coord3d.z);")
    SL("    zen = fract(coord3d.z) > 0.0;")
    SL("    scoff = u_scoffs[zint];")
    SL("    gl_Position = vec4(coord3d.x, coord3d.y*scoff.x + scoff.y, 0, 1);\n")
    SL("    f_color = zen && zint<MAX_TRACES ? u_colours[zint] : vec4(0, 0, 0, 0);")
    SL("};");
    
 // Macro to convert hex colour RGB value to normalised RGBA value
#define COLR(x) {(x>>16&255)/255.0, (x>>8&255)/255.0, (x&255)/255.0, 1}

// Normalised colour values for each trace. First is grid
GLfloat const trace_colours[MAX_TRACES][4] = {{1, 1, 1, 0.5},
    COLR(0xffff00), COLR(0x800000), COLR(0xff0000), COLR(0xff9900),
    COLR(0xffff00), COLR(0x00ff00), COLR(0x0000ff), COLR(0xff00ff),
    COLR(0x969696), COLR(0xffffcc), COLR(0x000000), COLR(0x800000),
    COLR(0xff0000), COLR(0xff9900), COLR(0xffff00), COLR(0x00ff00)};

#endif

// EOF
