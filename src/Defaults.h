#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <cstdint>
#include <array>
#include <cmath>

#include <FL/Enumerations.H>

#include "Utility.h"
#include "LabelValue.h"

constexpr int DEBUG = 1;

constexpr int LAB_PWM_FREQUENCY     = 20'000'000; // this is to be extracted from 25MHz master clk
constexpr int LAB_SPI_FREQUENCY     = 8'000'000; // in hz
constexpr int LAB_AUX_SPI_FREQUENCY = 100'000;

constexpr int   LAB_ADC_RESOLUTION            = 4096;
constexpr int   LAB_ADC_RESOLUTION_IN_BITS    = 12;
constexpr int   LAB_ADC_RESOLUTION_IN_INTEGER = pow (2, LAB_ADC_RESOLUTION_IN_BITS);
constexpr float LAB_ADC_REFERENCE_VOLTAGE     = 5.0;

constexpr float DISPLAY_UPDATE_RATE = (1.0 / 30.0); // in seconds, 25fps

// General LAB
#define RANDOM_VALUE_GENERATOR_WAIT_MS  5
#define PRE_FL_AWAKE_SLEEP_MS  10


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






// Channel_Signals

// Channel_Signal
constexpr double CHANNEL_SIGNAL_VOLTAGE_PER_DIVISION  = 1.0; // volts/div
constexpr int    CHANNEL_SIGNAL_VOLTAGE_PER_DIVISION_UNIT_SCALER  = 0; // 1 is 10 to the power of 0
constexpr double CHANNEL_SIGNAL_TIME_PER_DIVISION  = 1.0; // s
constexpr double CHANNEL_SIGNAL_SAMPLE_RATE = 1.0; // hz
constexpr double CHANNEL_SIGNAL_VERTICAL_OFFSET = 0.0; // volts
constexpr double CHANNEL_SIGNAL_HORIZONTAL_OFFSET = 0.0; // seconds

constexpr double    CHANNEL_SIGNAL_FUNCTION_AMPLITUDE   = 1.0;  // volts
constexpr double    CHANNEL_SIGNAL_FUNCTION_FREQUENCY   = 1.0;  // hz
constexpr double    CHANNEL_SIGNAL_FUNCTION_PHASE       = 0.0;  // degrees
constexpr double    CHANNEL_SIGNAL_FUNCTION_Y_OFFSET    = 0.0;  // volts
constexpr double    CHANNEL_SIGNAL_FUNCTION_DUTY_CYCLE  = 50.0; // %
// constexpr WaveType  CHANNEL_SIGNAL_FUNCTION_WAVE_TYPE   = SINE;


// LAB Oscilloscope
constexpr int     LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS     = 2;
constexpr int     LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES      = 3200;
constexpr int     LAB_OSCILLOSCOPE_MAX_NUMBER_OF_CHANNELS = 10;
constexpr double  LAB_OSCILLOSCOPE_SAMPLE_RATE            = 200'000; 
constexpr double  LAB_OSCILLOSCOPE_SAMPLE_PERIOD          = (1.0 / LAB_OSCILLOSCOPE_SAMPLE_RATE);
constexpr int     LAB_OSCILLOSCOPE_ADC_CE                 = 0; // CE0 or CE1

// LABSoft Oscilloscope
#define LABSOFT_OSCILLOSCOPE_CHANNEL_1_VOLTAGE_PER_DIVISION "1 V/div"
#define LABSOFT_OSCILLOSCOPE_CHANNEL_1_Y_OFFSET            "0 V"
#define LABSOFT_OSCILLOSCOPE_CHANNEL_2_VOLTAGE_PER_DIVISION "1 V/div"
#define LABSOFT_OSCILLOSCOPE_CHANNEL_2_Y_OFFSET            "0 V"
#define LABSOFT_OSCILLOSCOPE_X_OFFSET                     "0 s"
#define LABSOFT_OSCILLOSCOPE_TRIGGER_LEVEL                "0 v"
#define LABSOFT_OSCILLOSCOPE_DISPLAY_MODE                 "Repeated"

constexpr const char* LABSOFT_OSCILLOSCOPE_TIME_PER_DIVISION = "5 ms/div";

// LABSoft Oscilloscope Display Group
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_UPPER_PADDING      50
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LOWER_PADDING      50
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LEFT_PADDING       80
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_RIGHT_PADDING      65
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_PADDING    10
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_PADDING    22
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_INTERSPACE = 35;
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_UNIT_PADDING = 20;
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LABEL_SIZE         10
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_DEFAULT_LABEL_COLOR        FL_WHITE
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS     10
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_COLUMNS  10
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_OFFSET           0.0
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION  1.0 // seconds
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_BACKGROUND_COLOR   FL_BLACK
constexpr double LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_VOLTAGE_PER_DIVISION = 1.0; // volts

constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_CHANNELS = LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS;
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_MAX_NUMBER_OF_CHANNELS = LAB_OSCILLOSCOPE_MAX_NUMBER_OF_CHANNELS;

// LABSoft Oscilloscope Display 
static std::array<int, LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_MAX_NUMBER_OF_CHANNELS>
  LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNELS_GRAPH_COLOR = 
{
  0x00000003,
  0x00000006,
  0x00000002,
  0x00000001,
};

// LABSoft Oscilloscope Display
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS     = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS  = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_COLUMNS;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_CHANNELS = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_CHANNELS;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_SAMPLES  = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES;
constexpr float LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_VOLTAGE        = 25.0; // in volts

constexpr float LABSOFT_OSCILLOSCOPE_DISPLAY_VOLTAGE_PER_DIVISION = 1.0;

#define LABSOFT_OSCILLOSCOPE_DISPLAY_BACKGROUND_COLOR       FL_BLACK 
#define LABSOFT_OSCILLOSCOPE_DISPLAY_DEFAULT_COLOR          FL_BLACK
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GRID_COLOR             FL_LIGHT3
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_AMPLITUDE 1.0 // volts
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_FREQUENCY 1.0 // hz
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_PHASE     0.0 // degrees
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_Y_OFFSET  0.0 // volts
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_GENERATOR_CHANNEL_NUMBER 0
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION_UNIT_SCALER = 0;
constexpr DisplayMode LABSOFT_OSCILLOSCOPE_DISPLAY_DISPLAY_MODE = SCREEN;

#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_VOLTAGE_PER_DIVISION  1.0
#define LABSOFT_OSCILLOSCOPE_DISPLAY_TIME_PER_DIVISION 1.0

constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_NUMBER_OF_CHANNELS = 
  LAB_OSCILLOSCOPE_MAX_NUMBER_OF_CHANNELS;

static std::array<int, LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_NUMBER_OF_CHANNELS>
  LABSOFT_OSCILLOSCOPE_DISPLAY_CHANNELS_GRAPH_COLOR = 
    LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNELS_GRAPH_COLOR;

// LAB Oscilloscope


// LAB Meter
#define LAB_METER_VALUE 0.0
#define LAB_METER_UNIT_SCALER 1.0

// LABSoft_Voltmeter
#define LABSOFT_VOLTMETER_UNIT 0 

// LABSoft_Ammeter
#define LABSOFT_AMMETER_UNIT 0 

// LABSoft_Ohmmeter
#define LABSOFT_OHMMETER_UNIT 2 

// LAB_Function_Generator
constexpr double SIG_GEN_MIN_FREQ   = 0.1; 
constexpr double SIG_GEN_MAX_FREQ   = 12'500'000; 
constexpr int    SIG_GEN_REF_CLK_HZ = 25'000'000; 
constexpr int    SIG_GEN_2_POW_28   = 268'435'456;

constexpr int LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS = 1;
#define LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER  0
#define LAB_FUNCTION_GENERATOR_WAVE_TYPE   SINE
#define LAB_FUNCTION_GENERATOR_FREQUENCY   1.0
#define LAB_FUNCTION_GENERATOR_PERIOD      1.0
#define LAB_FUNCTION_GENERATOR_AMPLITUDE   1.0
#define LAB_FUNCTION_GENERATOR_Y_OFFSET      0.0
#define LAB_FUNCTION_GENERATOR_DUTY_CYCLE  50.0
#define LAB_FUNCTION_GENERATOR_PHASE       0.0

// constexpr int AD9833_CE_PIN = 

// LABSoft Function Generator
#define LABSOFT_FUNCTION_GENERATOR_HORIZONTAL_POSITION "0 s"
#define LABSOFT_FUNCTION_GENERATOR_TIME_PER_DIVISION   "1 s/div"
#define LABSOFT_FUNCTION_GENERATOR_VERTICAL_OFFSET     "0 s"
#define LABSOFT_FUNCTION_GENERATOR_WAVE_TYPE  "Sine"
#define LABSOFT_FUNCTION_GENERATOR_FREQUENCY  "1 Hz"
#define LABSOFT_FUNCTION_GENERATOR_PERIOD     "1 s"
#define LABSOFT_FUNCTION_GENERATOR_AMPLITUDE  "1 V"
#define LABSOFT_FUNCTION_GENERATOR_Y_OFFSET   "0 V"
#define LABSOFT_FUNCTION_GENERATOR_DUTY_CYCLE "50%"
#define LABSOFT_FUNCTION_GENERATOR_PHASE      "0 deg"
#define LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER 0


// --- Memory ---
// Structure for mapped peripheral or memory


// Round up to nearest page
#define PAGE_ROUNDUP(n) ((n) % PAGE_SIZE == 0 ? (n) : ((n) + PAGE_SIZE) & ~(PAGE_SIZE - 1))

// Size of memory page
#define PAGE_SIZE      0x1000

constexpr int SAMPLE_SIZE       = 4; // 4 bytes per sample
constexpr int PING_BONG_BUFFER  = 2; // 2 buffers that form a ping pong buffer
constexpr int BUFFER_LENGTH     = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES * SAMPLE_SIZE; 
constexpr int BUFFER_COUNT      = LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS;
constexpr int VC_MEM_SIZE    = (PAGE_SIZE + (BUFFER_LENGTH * BUFFER_COUNT * PING_BONG_BUFFER));

// --- DMA ---
constexpr uint32_t DMA_TI_PWM_DREQ  = 5;
constexpr uint32_t DMA_TI_RX_DREQ   = 7;
constexpr uint32_t DMA_TI_TX_DREQ   = 6;
constexpr uint32_t DMA_TI_SRC_DREQ  = 1 << 10;
constexpr uint32_t DMA_TI_SRC_INC   = 1 << 8;
constexpr uint32_t DMA_TI_DEST_DREQ = 1 << 6;
constexpr uint32_t DMA_TI_DEST_INC  = 1 << 4;
constexpr uint32_t DMA_TI_WAIT_RESP = 1 << 3;

constexpr uint32_t DMA_CB_PWM_TI    = (DMA_TI_PWM_DREQ << 16) | DMA_TI_DEST_DREQ | DMA_TI_WAIT_RESP;
constexpr uint32_t DMA_CB_SPI_TX_TI = (DMA_TI_TX_DREQ << 16) | DMA_TI_DEST_DREQ | DMA_TI_SRC_INC | DMA_TI_WAIT_RESP;
constexpr uint32_t DMA_CB_SPI_RX_TI = (DMA_TI_RX_DREQ << 16) | DMA_TI_SRC_DREQ | DMA_TI_DEST_INC | DMA_TI_WAIT_RESP;

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

  volatile uint32_t usecs[2], 
                    states[2], 
                    rxd1[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES], 
                    rxd2[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
} 
ADC_DMA_DATA;

#endif