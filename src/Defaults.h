#ifndef DEFAULTS_H
#define DEFAULTS_H

#include "../lib/AikaPi/AikaPi.h"

#include <cmath>
#include <array>

// --- Enums ---

enum BUFFER_SWITCH
{
  SINGLE_BUFFER,
  DOUBLE_BUFFER
};

enum DisplayMode 
{
  OSC_DISP_MODE_REPEATED,
  OSC_DISP_MODE_SCREEN
};

enum WaveType 
{ 
  SINE,
  SQUARE,
  SQUARE_HALF,
  SQUARE_FULL,
  TRIANGLE,
  DC
};

// ---

// General Raspberry Pi
constexpr double LAB_PWM_FREQUENCY  = 15'000'000.0; // this is to be extracted from 25MHz master clk
constexpr double LAB_PWM_DUTY_CYCLE = 50.0;
constexpr double LAB_SPI_FREQUENCY  = 5'000'000.0;  // final
constexpr int PWM_CHAN = 1;
constexpr unsigned PI_MAX_GPIO_PINS = 32; 
constexpr int DEBUG = 1;
constexpr int LAB_AUX_SPI_FREQUENCY = 100'000;

constexpr float DISPLAY_UPDATE_RATE = (1.0 / 25.0); // in seconds, 25fps

// DMA Channel Use
constexpr unsigned  LAB_LOGIC_ANALYZER_DMA_CHAN_GPIO_STORE  = 10;

// Channel_Signals

// Channel_Signal
constexpr double CHANNEL_SIGNAL_VOLTAGE_PER_DIVISION  = 1.0; // volts/div
constexpr int    CHANNEL_SIGNAL_VOLTAGE_PER_DIVISION_UNIT_SCALER  = 0; // 1 is 10 to the power of 0
constexpr double CHANNEL_SIGNAL_TIME_PER_DIVISION  = 1.0; // s
constexpr double CHANNEL_SIGNAL_SAMPLE_RATE = 1.0; // hz
constexpr double CHANNEL_SIGNAL_VERTICAL_OFFSET = 0.0; // volts
constexpr double CHANNEL_SIGNAL_HORIZONTAL_OFFSET = 0.0; // seconds

constexpr double  CHANNEL_SIGNAL_FUNCTION_AMPLITUDE   = 1.0;  // volts
constexpr double  CHANNEL_SIGNAL_FUNCTION_FREQUENCY   = 1.0;  // hz
constexpr double  CHANNEL_SIGNAL_FUNCTION_PHASE       = 0.0;  // degrees
constexpr double  CHANNEL_SIGNAL_FUNCTION_Y_OFFSET    = 0.0;  // volts
constexpr double  CHANNEL_SIGNAL_FUNCTION_DUTY_CYCLE  = 50.0; // %

// constexpr WaveType  CHANNEL_SIGNAL_FUNCTION_WAVE_TYPE   = SINE;


/// --- OSCILLOSCOPE ---


// LAB Oscilloscope
// dma channels in use after reboot 3b plus = 2 3 4 6
constexpr unsigned  LAB_OSCILLOSCOPE_DMA_CHAN_PWM_PACING    = 7;
constexpr unsigned  LAB_OSCILLOSCOPE_DMA_CHAN_SPI_RX        = 8;
constexpr unsigned  LAB_OSCILLOSCOPE_DMA_CHAN_SPI_TX        = 9;
constexpr int       LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS     = 2;
constexpr int       LAB_OSCILLOSCOPE_MAX_NUMBER_OF_CHANNELS = 10;
constexpr int       LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES      = 2000;
constexpr double    LAB_OSCILLOSCOPE_SAMPLING_RATE          = 200'000; 
constexpr double    LAB_OSCILLOSCOPE_SAMPLE_PERIOD          = (1.0 / LAB_OSCILLOSCOPE_SAMPLING_RATE);
constexpr double    LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE      = 200'000;
constexpr unsigned  LAB_OSCILLOSCOPE_SAMPLE_SIZE_BYTES      = 4;  // 4 bytes per sample = 32 bits
constexpr unsigned  LAB_OSCILLOSCOPE_BUFFER_LENGTH_BYTES    = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES * LAB_OSCILLOSCOPE_SAMPLE_SIZE_BYTES;
constexpr unsigned  LAB_OSCILLOSCOPE_BUFFER_COUNT           = 2;
constexpr unsigned  LAB_OSCILLOSCOPE_VC_MEM_SIZE            = PAGE_SIZE + (LAB_OSCILLOSCOPE_BUFFER_COUNT * LAB_OSCILLOSCOPE_BUFFER_LENGTH_BYTES * LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS);

struct LAB_OSCILLOSCOPE_DMA_DATA
{
  DMA_CB cbs[10];

  uint32_t  samp_size,
            pwm_val, 
            adc_csd,
            txd[2];

  volatile uint32_t usecs[2],
                    states[2],
                    rxd0[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES],
                    rxd1[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
};

// LABSoft Oscilloscope Display Group
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_CHANNELS     = LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS;
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_MAX_NUMBER_OF_CHANNELS = LAB_OSCILLOSCOPE_MAX_NUMBER_OF_CHANNELS;
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
constexpr unsigned LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_COLOR_WHITE = 255;

// LABSoft Oscilloscope Display
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS     = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS  = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_COLUMNS;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_CHANNELS = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_CHANNELS;
constexpr float LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_VOLTAGE        = 25.0; // in volts

constexpr float LABSOFT_OSCILLOSCOPE_DISPLAY_VOLTAGE_PER_DIVISION = 1.0;

static std::array<int, LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_MAX_NUMBER_OF_CHANNELS>
  LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNELS_GRAPH_COLOR = 
{
  0x00000003,
  0x00000006,
  0x00000002,
  0x00000001,
};

#define LABSOFT_OSCILLOSCOPE_DISPLAY_BACKGROUND_COLOR       FL_BLACK 
#define LABSOFT_OSCILLOSCOPE_DISPLAY_DEFAULT_COLOR          FL_BLACK
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GRID_COLOR             FL_LIGHT3
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_AMPLITUDE 1.0 // volts
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_FREQUENCY 1.0 // hz
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_PHASE     0.0 // degrees
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_Y_OFFSET  0.0 // volts
#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_GENERATOR_CHANNEL_NUMBER 0
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION_UNIT_SCALER = 0;
constexpr DisplayMode LABSOFT_OSCILLOSCOPE_DISPLAY_DISPLAY_MODE = OSC_DISP_MODE_SCREEN;

#define LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_VOLTAGE_PER_DIVISION  1.0
#define LABSOFT_OSCILLOSCOPE_DISPLAY_TIME_PER_DIVISION 1.0

constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_NUMBER_OF_CHANNELS = 
  LAB_OSCILLOSCOPE_MAX_NUMBER_OF_CHANNELS;

static std::array<int, LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_NUMBER_OF_CHANNELS>
  LABSOFT_OSCILLOSCOPE_DISPLAY_CHANNELS_GRAPH_COLOR = 
    LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNELS_GRAPH_COLOR;

constexpr int     LAB_OSCILLOSCOPE_ADC_CE                 = 0; // CE0 or CE1

constexpr double LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_OSC_DISP_MODE_SCREEN = 1.0 / LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_COLUMNS;

constexpr int     LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS      = 12;
constexpr int     LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT       = std::pow (2, LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS);
constexpr double  LAB_OSCILLOSCOPE_ACTUAL_ADC_REFERENCE_VOLTAGE    = 5.0;
constexpr double  LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE    = 2.5;
constexpr double  LAB_OSCILLOSCOPE_ADC_CONVERSION_CONSTANT  = LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE / ((LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT - 1) >> 1);


// LABSoft Oscilloscope
#define LABSOFT_OSCILLOSCOPE_CHANNEL_1_VOLTAGE_PER_DIVISION "1 V/div"
#define LABSOFT_OSCILLOSCOPE_CHANNEL_1_Y_OFFSET            "0 V"
#define LABSOFT_OSCILLOSCOPE_CHANNEL_2_VOLTAGE_PER_DIVISION "1 V/div"
#define LABSOFT_OSCILLOSCOPE_CHANNEL_2_Y_OFFSET            "0 V"
#define LABSOFT_OSCILLOSCOPE_X_OFFSET                     "0 s"
#define LABSOFT_OSCILLOSCOPE_TRIGGER_LEVEL                "0 v"
#define LABSOFT_OSCILLOSCOPE_DISPLAY_MODE                 "Repeated"

constexpr const char* LABSOFT_OSCILLOSCOPE_TIME_PER_DIVISION = "1 ms/div";


// LABSoft Oscilloscope Display 



// LAB Oscilloscope
constexpr double  LAB_OSCILLOSCOPE_MAX_TIME_PER_DIV_ZOOM = 
  (LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES) / (LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE * 
    LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS);


// LAB Voltmeter
constexpr const char *LABSOFT_VOLTMETER_FL_OUTPUT_VALUE_LABEL = "0";

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

// LAB Logic Analyzer
struct Channel_Data_Logic_Analyzer
{

};

constexpr uint32_t  LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE = (DMA_TI_DREQ_PWM << 16) | DMA_TI_DEST_DREQ | DMA_TI_DEST_INC | DMA_TI_WAIT_RESP;
constexpr unsigned  LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES    = 2000;
constexpr unsigned  LAB_LOGIC_ANALYZER_SAMPLING_RATE        = 200'000;
constexpr unsigned  LAB_LOGIC_ANALYZER_MAX_SAMPLING_RATE    = 200'000;
constexpr double    LAB_LOGIC_ANALYZER_SAMPLE_PERIOD        = (1.0 / LAB_LOGIC_ANALYZER_SAMPLING_RATE);
constexpr unsigned  LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS   = 8;
constexpr unsigned  LAB_LOGIC_ANALYZER_SAMPLE_SIZE_BYTES    = 4;  // 4 bytes per sample = 32 bits
constexpr unsigned  LAB_LOGIC_ANALYZER_BUFFER_LENGTH_BYTES  = LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * LAB_LOGIC_ANALYZER_SAMPLE_SIZE_BYTES;
constexpr unsigned  LAB_LOGIC_ANALYZER_BUFFER_COUNT         = 2;
constexpr unsigned  LAB_LOGIC_ANALYZER_VC_MEM_SIZE          = PAGE_SIZE + (LAB_LOGIC_ANALYZER_BUFFER_COUNT * LAB_LOGIC_ANALYZER_BUFFER_LENGTH_BYTES * LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS);

constexpr unsigned  LAB_LOGIC_ANALYZER_CHANNELS_GPIO_PINS [LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS] = {2, 3, 4, 27, 22, 0, 5, 6};

struct LAB_LOGIC_ANALYZER_DMA_DATA
{
  DMA_CB cbs[15];
  
  uint32_t  samp_size,
            pwm_data,
            buffer_ok_flag = 0x1;

  volatile uint32_t usecs[2],
                    states[2],
                    rxd0[LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES],
                    rxd1[LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES];
};

// LABSoft Logic Analyzer Display Group
constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_NUMBER_OF_CHANNELS = LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS;
//constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_GRAPH_BACKGROUND_COLOR = 0xFFFFFF00; // white
//constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_GRAPH_NUMBER_OF_COLUMNS = 10;
// constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_GRAPH_GRID_COLOR = 0x000000FF; // kinda light gray
// constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_GRAPH_X_LABEL_STRIP_HEIGHT= 30; // kinda light gray

constexpr const char* LABSOFT_LOGIC_ANALYZER_MEMORY_DEPTH     = "4096";
constexpr const char* LABSOFT_LOGIC_ANALYZER_SAMPLE_RATE      = "1 kHz";
constexpr const char* LABSOFT_LOGIC_ANALYZER_TIME_PER_DIVISON = "1 ms/div";
constexpr const char* LABSOFT_LOGIC_ANALYZER_POSITION         = " 0 s";

#endif

// EOF