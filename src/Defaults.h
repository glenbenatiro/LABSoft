#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <cmath>
#include <vector>
#include <array>

#include <FL/Enumerations.H>
#include <FL/fl_draw.H>

#include "../lib/AikaPi/AikaPi.h"

// --- Enums ---

enum LE_UNIT_PREFIX_EXP
{
  LE_UNIT_PREFIX_EXP_GIGA   = 9,
  LE_UNIT_PREFIX_EXP_MEGA   = 6,
  LE_UNIT_PREFIX_EXP_KILO   = 3,
  LE_UNIT_PREFIX_EXP_NONE   = 0,
  LE_UNIT_PREFIX_EXP_MILLI  = -3,
  LE_UNIT_PREFIX_EXP_MICRO  = -6,
  LE_UNIT_PREFIX_EXP_NANO   = -9
};

enum LE_UNIT
{
  LE_UNIT_NONE,
  LE_UNIT_VOLT,
  LE_UNIT_AMP,
  LE_UNIT_OHM,
  LE_UNIT_SEC,
  LE_UNIT_HZ
};

enum LE_LABEL_TYPE
{
  LE_LABEL_TYPE_VOLTS_PER_DIVISION,
  LE_LABEL_TYPE_TIME_PER_DIVISION
};

enum LE_SPI_DMA_BUFFER_COUNT
{
  LE_SPI_DMA_BUFFER_COUNT_SINGLE,
  LE_SPI_DMA_BUFFER_COUNT_DOUBLE
};

enum LE_GRAPH_DISP_MODE 
{
  LE_GRAPH_DISP_MODE_REPEATED,
  LE_GRAPH_DISP_MODE_SCREEN
};


// --- General Raspberry Pi ---
constexpr double LAB_PWM_FREQUENCY  = 100'000'000.0; 
constexpr double LAB_SPI_FREQUENCY  = 8'000'000.0;  // final

// --- PWM ---
struct LAB_PWM_PACING_DMA_DATA 
{ 
  AP_DMA_CB cbs[15];

  uint32_t pwm_val;
};

constexpr unsigned LAB_PWM_DMA_PACING_PWM_CHAN = 0;

constexpr double LAB_PWM_DUTY_CYCLE = 50.0;
constexpr int PWM_CHAN              = 1;
constexpr unsigned PI_MAX_GPIO_PINS = 32; 
constexpr int DEBUG                 = 1;
constexpr int LAB_AUX_SPI_FREQUENCY = 100'000;

constexpr float DISPLAY_UPDATE_RATE = (1.0 / 25.0); // in seconds, 25fps

// Raspberry Pi Zero BCM Pin Assishift_bit_countgnments
// https://pinout.xyz/
constexpr unsigned LAB_PIN_LOGIC_ANALYZER []                      = {0, 26, 1};
constexpr unsigned LAB_RPI_PIN_PWM_CHAN_0                         = 12;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_1 = 14;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_2 = 15;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_0   = 27;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_0   = 22;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_1   = 23;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_1   = 24;

// DMA Channel Use
constexpr unsigned  LAB_DMA_CHAN_PWM_PACING                 = 7;
constexpr unsigned  LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX        = 8;
constexpr unsigned  LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX        = 9;
constexpr unsigned  LAB_DMA_CHAN_OSCILLOSCOPE_SPI_LOAD      = 10;
constexpr unsigned  LAB_DMA_CHAN_LOGIC_ANALYZER_GPIO_STORE  = 11;

// --- Oscilloscope ---
enum LE_OSC_SCALING
{
  LE_OSC_SCALING_QUADRUPLE  = 0, // 00
  LE_OSC_SCALING_UNITY      = 1, // 01
  LE_OSC_SCALING_HALF       = 2, // 10
  LE_OSC_SCALING_FOURTH     = 3, // 11
};

enum LE_OSC_COUPLING
{
  LE_OSC_COUPLING_AC = 0,
  LE_OSC_COUPLING_DC = 1
};

// dma channels in use after reboot 3b plus = 2 3 4 6
constexpr int       LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS             = 2;
constexpr int       LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES              = 2'000;
constexpr double    LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE              = 200'000;
constexpr int       LAB_OSCILLOSCOPE_MAX_NUMBER_OF_CHANNELS         = 10;
constexpr double    LAB_OSCILLOSCOPE_MAX_TIME_PER_DIVISION_NO_ZOOM  = (1.0 / LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE);
constexpr unsigned  LAB_OSCILLOSCOPE_SAMPLE_SIZE_BYTES              = 4;  
constexpr unsigned  LAB_OSCILLOSCOPE_BUFFER_LENGTH_BYTES            = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES * LAB_OSCILLOSCOPE_SAMPLE_SIZE_BYTES;
constexpr unsigned  LAB_OSCILLOSCOPE_BUFFER_COUNT                   = 2;
constexpr unsigned  LAB_OSCILLOSCOPE_VC_MEM_SIZE                    = PAGE_SIZE + (LAB_OSCILLOSCOPE_BUFFER_COUNT * LAB_OSCILLOSCOPE_BUFFER_LENGTH_BYTES * LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS);
constexpr double    LAB_OSCILLOSCOPE_SAMPLING_RATE                  = LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE; 
constexpr double    LAB_OSCILLOSCOPE_VOLTAGE_PER_DIVISION           = 1.0;
constexpr double    LAB_OSCILLOSCOPE_VERTICAL_OFFSET                = 0.0;
constexpr double    LAB_OSCILLOSCOPE_TIME_PER_DIVISION              = (1.0 / LAB_OSCILLOSCOPE_SAMPLING_RATE);
constexpr double    LAB_OSCILLOSCOPE_HORIZONTAL_OFFSET              = 0.0;
constexpr int       LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS            = 12;
constexpr int       LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT             = std::pow (2, LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS);
constexpr double    LAB_OSCILLOSCOPE_ACTUAL_ADC_REFERENCE_VOLTAGE   = 5.0;
constexpr double    LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE          = 2.5;
constexpr double    LAB_OSCILLOSCOPE_ADC_CONVERSION_CONSTANT        = LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE / ((LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT - 1) >> 1);
constexpr unsigned  LAB_OSCILLOSCOPE_RAW_DATA_SHIFT_BIT_COUNT       = 32 / LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS;
constexpr uint32_t  LAB_OSCILLOSCOPE_RAW_DATA_POST_SHIFT_MASK       = ((std::pow (2, LAB_OSCILLOSCOPE_RAW_DATA_SHIFT_BIT_COUNT)) - 1);
constexpr int       LAB_OSCILLOSCOPE_ADC_CE                         = 0; // CE0 or CE1

constexpr LE_OSC_COUPLING     LAB_OSCILLOSCOPE_COUPLING             = LE_OSC_COUPLING_DC;
constexpr LE_OSC_SCALING      LAB_OSCILLOSCOPE_SCALING              = LE_OSC_SCALING_UNITY;
constexpr LE_GRAPH_DISP_MODE  LAB_OSCILLOSCOPE_GRAPH_DISP_MODE      = LE_GRAPH_DISP_MODE_REPEATED;

struct LAB_Channel_Data_Oscilloscope
{
  // Channel
  bool is_enabled = false;

  // Vertical
  double          voltage_per_division  = LAB_OSCILLOSCOPE_VOLTAGE_PER_DIVISION;
  double          vertical_offset       = LAB_OSCILLOSCOPE_VERTICAL_OFFSET;
  LE_OSC_SCALING  scaling               = LAB_OSCILLOSCOPE_SCALING;
  LE_OSC_COUPLING coupling              = LAB_OSCILLOSCOPE_COUPLING;

  // Data/Samples
  std::vector <std::array<int, 2>> pixel_points;
  std::array  <double, LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES> samples;
};

class LAB_Parent_Data_Oscilloscope
{
  public:
    bool has_enabled_channels ()
    {
      for (int a = 0; a < channel_data.size (); a++)
      {
        if (channel_data[a].is_enabled)
          return true;
      }

      return false;
    }
    
    bool is_osc_core_running      = false; 
    bool is_osc_frontend_running  = false;

    // Horizontal
    double  time_per_division   = LAB_OSCILLOSCOPE_TIME_PER_DIVISION;
    double  horizontal_offset   = LAB_OSCILLOSCOPE_HORIZONTAL_OFFSET;
    double  sampling_rate       = LAB_OSCILLOSCOPE_SAMPLING_RATE;
    LE_GRAPH_DISP_MODE graph_disp_mode = LAB_OSCILLOSCOPE_GRAPH_DISP_MODE;

    // Data/Samples
    double w_samp_count = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES; // working sample count
    std::array <uint32_t, LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES>                       raw_sample_buffer;
    std::array <LAB_Channel_Data_Oscilloscope, LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS> channel_data;
};

struct LAB_Oscilloscope_DMA_Data
{
  AP_DMA_CB cbs[10];

  uint32_t  samp_size,
            adc_csd,
            txd[2];

  volatile uint32_t usecs[2] = {0, 0},
                    status[2] = {0, 0},
                    rxd0[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES],
                    rxd1[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
};


// LABSoft Oscilloscope Display
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS     = 10;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS  = 10;
constexpr float LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_VOLTAGE        = 25.0; // in volts
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_TOP_MARGIN         = 50;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_BOTTOM_MARGIN      = 50;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_LEFT_MARGIN        = 80;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_RIGHT_MARGIN       = 65;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_BACKGROUND_COLOR   = FL_BLACK;
constexpr int   LABSOFT_OSCILLOSCOPE_DISPLAY_GRID_COLOR         = FL_WHITE;

// LAB Oscilloscope
constexpr double  LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_NO_ZOOM                 = (LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES) / (LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE * LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS);
constexpr double  LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_GRAPH_DISP_MODE_SCREEN  = 1.0 / LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS;

// LABSoft Oscilloscope Display Group
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_SIZE           = 10;
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_COLOR          = FL_WHITE;
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_INTRASPACE     = 18; 
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_SIZE           = 10;
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_COLOR          = FL_FOREGROUND_COLOR;
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_INTERSPACE     = 35; // spacing between columns of y-axis labels
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_PADDING        = 22; // padding of first column of y-axis labels from left of grid
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_UNIT_MARGIN    = 20; // padding of voltage unit of y-axis labels from top of grid
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_DEFAULT_LABEL_COLOR    = FL_WHITE;
constexpr int LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_BACKGROUND_COLOR       = FL_BLACK;

constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_0_VOLTAGE_PER_DIVISION = "1 V/div";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_0_VERTICAL_OFFSET      = "0 V";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_1_VOLTAGE_PER_DIVISION = "1 V/div";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_1_VERTICAL_OFFSET      = "0 V";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION              = "5 us/div";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_HORIZONTAL_OFFSET              = "0 s";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TRIGGER_LEVEL                  = "0 V";
constexpr int         LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_DISPLAY_MODE                   = (LAB_OSCILLOSCOPE_TIME_PER_DIVISION >= LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_GRAPH_DISP_MODE_SCREEN) ? 1 : 0; // 0 is repeated, 1 is screen

static std::array<int, 10> 
  LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_COLORS = 
{
  0x00000003,
  0x00000006,
  0x00000002,
  0x00000001,
};

// --- LAB Voltmeter ---

// --- Function Generator ---
enum LE_FG_PARAM
{
  LE_FG_PARAM_WAVE_TYPE,
  LE_FG_PARAM_FREQUENCY,
  LE_FG_PARAM_PERIOD,
  LE_FG_PARAM_AMPLITUDE,
  LE_FG_PARAM_VERTICAL_OFFSET,
  LE_FG_PARAM_DUTY_CYCLE,
  LE_FG_PARAM_PHASE
};

enum LE_WAVE_TYPE 
{ 
  LE_WAVE_TYPE_SINE,
  LE_WAVE_TYPE_SQUARE,
  LE_WAVE_TYPE_SQUARE_HALF,
  LE_WAVE_TYPE_SQUARE_FULL,
  LE_WAVE_TYPE_TRIANGLE,
  LE_WAVE_TYPE_DC
};

constexpr unsigned  LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS     = 1;
constexpr double    LAB_FUNCTION_GENERATOR_SIG_GEN_FREQUENCY_MIN  = 0.1; // Hz
constexpr double    LAB_FUNCTION_GENERATOR_SIG_GEN_FREQUENCY_MAX  = 1'000'00; // Hz
constexpr double    LAB_FUNCTION_GENERATOR_DIGI_POT_AMPLITUDE_MIN = -3.3; //V
constexpr double    LAB_FUNCTION_GENERATOR_DIGI_POT_AMPLITUDE_MAX = 3.3; //V
constexpr double    LAB_FUNCTION_GENERATOR_FREQUENCY_MIN          = LAB_FUNCTION_GENERATOR_SIG_GEN_FREQUENCY_MIN;
constexpr double    LAB_FUNCTION_GENERATOR_FREQUENCY_MAX          = 1'000'000; // Hz
constexpr double    LAB_FUNCTION_GENERATOR_AMPLITUDE_MIN          = LAB_FUNCTION_GENERATOR_DIGI_POT_AMPLITUDE_MIN;
constexpr double    LAB_FUNCTION_GENERATOR_AMPLITUDE_MAX          = LAB_FUNCTION_GENERATOR_DIGI_POT_AMPLITUDE_MAX;
constexpr double    LAB_FUNCTION_GENERATOR_VERTICAL_OFFSET_MIN    = -5.0;
constexpr double    LAB_FUNCTION_GENERATOR_VERTICAL_OFFSET_MAX    = 5.0;
constexpr double    LAB_FUNCTION_GENERATOR_DUTY_CYCLE_MIN         = 0.0;
constexpr double    LAB_FUNCTION_GENERATOR_DUTY_CYCLE_MAX         = 100.0;
constexpr double    LAB_FUNCTION_GENERATOR_PHASE_MIN              = -360.0;
constexpr double    LAB_FUNCTION_GENERATOR_PHASE_MAX              = 360.0;
constexpr double    LAB_FUNCTION_GENERATOR_FREQUENCY              = 1'000; // Hz
constexpr double    LAB_FUNCTION_GENERATOR_PERIOD                 = 1.0 / LAB_FUNCTION_GENERATOR_FREQUENCY;
constexpr double    LAB_FUNCTION_GENERATOR_AMPLITUDE              = 1.0; // 1 volt
constexpr double    LAB_FUNCTION_GENERATOR_VERTICAL_OFFSET        = 0.0;
constexpr double    LAB_FUNCTION_GENERATOR_DUTY_CYCLE             = 50.0; // 50% duty cycle
constexpr double    LAB_FUNCTION_GENERATOR_PHASE                  = 0.0; // 0 degree phase = in phase
constexpr LE_WAVE_TYPE LAB_FUNCTION_GENERATOR_WAVE_TYPE           = LE_WAVE_TYPE_SQUARE;

constexpr double SIG_GEN_MIN_FREQ   = 0.1; 
constexpr double SIG_GEN_MAX_FREQ   = 12'500'000; 
constexpr int    SIG_GEN_REF_CLK_HZ = 25'000'000; 
constexpr int    SIG_GEN_2_POW_28   = 268'435'456;

struct LAB_Channel_Data_Function_Generator
{
  // Channel
  bool is_enabled = false;

  // Parameters
  LE_WAVE_TYPE  wave_type       = LAB_FUNCTION_GENERATOR_WAVE_TYPE;
  double        frequency       = LAB_FUNCTION_GENERATOR_FREQUENCY;
  double        period          = LAB_FUNCTION_GENERATOR_PERIOD;
  double        amplitude       = LAB_FUNCTION_GENERATOR_AMPLITUDE;
  double        vertical_offset = LAB_FUNCTION_GENERATOR_VERTICAL_OFFSET;
  double        duty_cycle      = LAB_FUNCTION_GENERATOR_DUTY_CYCLE;
  double        phase           = LAB_FUNCTION_GENERATOR_PHASE;
};

struct LAB_Parent_Data_Function_Generator
{
  std::array <LAB_Channel_Data_Function_Generator, LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS> channel_data;
};

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
constexpr unsigned  LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS       = sizeof (LAB_PIN_LOGIC_ANALYZER) / sizeof (LAB_PIN_LOGIC_ANALYZER[0]);
constexpr uint32_t  LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE     = (DMA_TI_DREQ_PWM << 16) | DMA_TI_DEST_DREQ | DMA_TI_DEST_INC | DMA_TI_WAIT_RESP;
constexpr unsigned  LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES        = 2'000;
constexpr double    LAB_LOGIC_ANALYZER_MAX_SAMPLING_RATE        = 200'000;
constexpr unsigned  LAB_LOGIC_ANALYZER_SAMPLING_RATE            = LAB_LOGIC_ANALYZER_MAX_SAMPLING_RATE;
constexpr double    LAB_LOGIC_ANALYZER_MIN_TIME_PER_DIV_NO_ZOOM = 1.0 / LAB_LOGIC_ANALYZER_MAX_SAMPLING_RATE;
constexpr double    LAB_LOGIC_ANALYZER_SAMPLE_PERIOD            = (1.0 / LAB_LOGIC_ANALYZER_SAMPLING_RATE);
constexpr unsigned  LAB_LOGIC_ANALYZER_SAMPLE_SIZE_BYTES        = 4;  // 4 bytes per sample = 32 bits
constexpr unsigned  LAB_LOGIC_ANALYZER_BUFFER_LENGTH_BYTES      = LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * LAB_LOGIC_ANALYZER_SAMPLE_SIZE_BYTES;
constexpr unsigned  LAB_LOGIC_ANALYZER_BUFFER_COUNT             = 2;
constexpr unsigned  LAB_LOGIC_ANALYZER_VC_MEM_SIZE              = PAGE_SIZE + (LAB_LOGIC_ANALYZER_BUFFER_COUNT * LAB_LOGIC_ANALYZER_BUFFER_LENGTH_BYTES * LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS);

struct LAB_Channel_Data_Logic_Analyzer
{
  bool      is_enabled                        = true;
  unsigned  raw_sample_buffer_working_size      = 0;

  // Data/Samples
  std::vector <std::array<int, 2>> pixel_points;
  std::array <bool, LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES> samples;
};

class LAB_Parent_Data_Logic_Analyzer
{
  public:
    bool has_enabled_channels ()
    {
      for (int a = 0; a < channel_data.size (); a++)
      {
        if (channel_data[a].is_enabled)
          return true;
      }

      return false;
    }

    bool      is_enabled          = false;
    double    sampling_rate       = LAB_LOGIC_ANALYZER_SAMPLING_RATE;
    double    time_per_division   = 0.0;
    double    horizontal_offset            = 0.0;
    unsigned  w_samp_count  = 0;

    std::array <uint32_t, 
      LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES>   raw_sample_buffer;
      
    std::array <LAB_Channel_Data_Logic_Analyzer, 
      LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS>  channel_data;

    LE_GRAPH_DISP_MODE graph_disp_mode  = LE_GRAPH_DISP_MODE_REPEATED;
};

struct LAB_Logic_Analyzer_DMA_Data
{
  AP_DMA_CB cbs[15];
  
  uint32_t  samp_size,
            pwm_data,
            buffer_ok_flag = 0x1;

  volatile uint32_t usecs[2],
                    status[2],
                    rxd0[LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES],
                    rxd1[LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES];
};

// LABSoft Logic Analyzer Display Group
constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_STRIP_HEIGHT      = 30;
constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_CHANNEL_MENU_BUTTON_WIDTH = 120;
constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_TOP_MARGIN        = 5;
constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LAST_LABEL_OFFSET       = 18;
constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_SIZE              = 10;
constexpr int LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_COLOR             = FL_FOREGROUND_COLOR;

// LABSoft Logic Analyzer Display
constexpr unsigned  LABSOFT_LOGIC_ANALYZER_DISPLAY_NUMBER_OF_COLUMNS            = 10;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_BG_COLOR                     = FL_WHITE;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_NUMBER_OF_CHANNELS     = LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GRID_COLOR                   = FL_BLACK;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_STYLE             = FL_SOLID;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_COLOR             = FL_BLACK;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_WIDTH             = 0;
constexpr char*     LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_DASHES            = 0;
constexpr double    LAB_LOGIC_ANALYZER_MIN_TIME_PER_DIV_GRAPH_DISP_MODE_SCREEN  = 1.0 / (LABSOFT_LOGIC_ANALYZER_DISPLAY_NUMBER_OF_COLUMNS);

constexpr const char* LABSOFT_LOGIC_ANALYZER_MEMORY_DEPTH     = "4096";
constexpr const char* LABSOFT_LOGIC_ANALYZER_SAMPLE_RATE      = "1 kHz";
constexpr const char* LABSOFT_LOGIC_ANALYZER_TIME_PER_DIVISON = "1 ms/div";
constexpr const char* LABSOFT_LOGIC_ANALYZER_POSITION         = " 0 s";

#endif

// EOF