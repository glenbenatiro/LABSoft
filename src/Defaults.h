#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <cmath>
#include <array>

enum DisplayMode 
{
  REPEATED,
  SHIFT,
  SCREEN
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

// General Raspberry Pi
constexpr unsigned PI_MAX_GPIO_PINS = 32; 

constexpr int DEBUG = 1;

constexpr int LAB_PWM_FREQUENCY     = 20'000'000; // this is to be extracted from 25MHz master clk
constexpr int LAB_SPI_FREQUENCY     = 8'000'000; // in hz
constexpr int LAB_AUX_SPI_FREQUENCY = 100'000;

constexpr float DISPLAY_UPDATE_RATE = (1.0 / 25.0); // in seconds, 25fps

// General LAB
#define RANDOM_VALUE_GENERATOR_WAIT_MS  5
#define PRE_FL_AWAKE_SLEEP_MS  10

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


// LAB Oscilloscope
constexpr int     LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS     = 2;
constexpr int     LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES      = 3200;
constexpr int     LAB_OSCILLOSCOPE_MAX_NUMBER_OF_CHANNELS = 10;
constexpr double  LAB_OSCILLOSCOPE_SAMPLE_RATE            = 200'000; 
constexpr double  LAB_OSCILLOSCOPE_SAMPLE_PERIOD          = (1.0 / LAB_OSCILLOSCOPE_SAMPLE_RATE);
constexpr int     LAB_OSCILLOSCOPE_ADC_CE                 = 0; // CE0 or CE1

constexpr int     LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS      = 12;
constexpr int     LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT       = std::pow (2, LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS);
constexpr double  LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE    = 5.0;
constexpr double  LAB_OSCILLOSCOPE_ADC_CONVERSION_CONSTANT  = LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE / ((LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT - 1) >> 1);

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

// LAB Logic Analyzer
constexpr unsigned LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS = 8;
constexpr unsigned LAB_LOGIC_ANALYZER_MAX_NUMBER_OF_SAMPLES = 4096;
constexpr unsigned LAB_LOGIC_ANALYZER_MEMORY_DEPTH = 4096;
constexpr unsigned LAB_LOGIC_ANALYZER_CHANNEL_GPIO_PINS
  [LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS] = {2, 3, 4, 27, 22, 0, 5, 6};

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