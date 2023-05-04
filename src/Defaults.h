#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <cmath>
#include <vector>
#include <array>
#include <map>
#include <string>

#include <FL/Enumerations.H>
#include <FL/fl_draw.H>

#include "../lib/AikaPi/AikaPi.h"

// --- Enums ---

namespace LAB_DEFAULTS
{
  bool compareDouble (double a, double b, double epsilon = 1e-9);
  bool isWithinRange (double value, double min, double max);
}

namespace LE
{
  enum class DISPLAY_MODE
  {
    SCREEN,
    REPEATED
  };
}

namespace GUI_LBL
{
  static std::map <LE::DISPLAY_MODE, std::string> DISPLAY_MODE = 
  {
    {LE::DISPLAY_MODE::SCREEN,    "Screen"},
    {LE::DISPLAY_MODE::REPEATED,  "Repeated"}
  };
}

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

enum LE_SPI_DMA_NUMBER_OF_BUFFERS
{
  LE_SPI_DMA_NUMBER_OF_BUFFERS_SINGLE,
  LE_SPI_DMA_NUMBER_OF_BUFFERS_DOUBLE
};

// --- General Raspberry Pi ---
namespace LABSOFT_GENERAL
{
  constexpr float DISPLAY_UPDATE_RATE (1.0 / 25.0); // 25 fps
}


constexpr double LAB_PWM_FREQUENCY  = 100'000'000.0; 

// because SPI core clock is fixed to 250MHz in boot/config.txt
// this is a divisor of 32
constexpr double LAB_SPI_FREQUENCY  = 10'000'000;  // final

// --- PWM ---
struct LAB_PWM_PACING_DMA_DATA 
{ 
  AP_DMA_CB cbs[15];

  uint32_t pwm_val;
};


namespace LABC
{
  namespace PIN
  {
    constexpr unsigned PWM = 12;

    constexpr unsigned OSC_MUX_SCALER_A0_CHAN_0   = 27;
    constexpr unsigned OSC_MUX_SCALER_A1_CHAN_0   = 22;
    constexpr unsigned OSC_MUX_SCALER_A0_CHAN_1   = 23;
    constexpr unsigned OSC_MUX_SCALER_A1_CHAN_1   = 24;
    constexpr unsigned OSC_COUPLING_SELECT_CHAN_0 = 14;
    constexpr unsigned OSC_COUPLING_SELECT_CHAN_1 = 15; 
  };

  namespace DMA_CHAN
  {
    constexpr unsigned PWM_PACING  = 7;
    constexpr unsigned OSC_RX      = 8;
    constexpr unsigned OSC_TX      = 9;
  };

  namespace SPI
  {
    constexpr double FREQUENCY = 10'000'000.0;
  };

  namespace PWM
  {
    constexpr int DMA_PACING_CHAN = 0;
  };

  namespace CLKMAN
  {
    constexpr double FREQUENCY = 100'000'000.0;
  };

  // Oscilloscope
  namespace OSC
  {

  };
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
constexpr unsigned LAB_PIN_LOGIC_ANALYZER []                      = {0, 1, 26};
constexpr unsigned LAB_RPI_PIN_PWM_CHAN_0                         = 12;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_0 = 14;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_1 = 15;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_0   = 27;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_0   = 22;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_1   = 23;
constexpr unsigned LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_1   = 24;
constexpr unsigned LAB_PIN_FUNC_GEN_SIG_GEN_MOSI                  = 4;
constexpr unsigned LAB_PIN_FUNC_GEN_SIG_GEN_MISO                  = 19;
constexpr unsigned LAB_PIN_FUNC_GEN_SIG_GEN_SCLK                  = 6;
constexpr unsigned LAB_PIN_FUNC_GEN_SIG_GEN_CS                    = 13;       

// DMA Channel Use
// dma channels in use after reboot 3b plus = 2 3 4 6
constexpr unsigned  LAB_DMA_CHAN_PWM_PACING = 7;
constexpr unsigned  LAB_DMA_CHAN_OSC_RX     = 8;
constexpr unsigned  LAB_DMA_CHAN_OSC_TX     = 9;
constexpr unsigned  LAB_DMA_CHAN_LOGIC_ANALYZER_GPIO_STORE  = 10;

constexpr uint32_t LAB_DMA_TI_OSC_PWM_PACING  = (DMA_TI_DREQ_PWM << 16) | DMA_TI_DEST_DREQ | DMA_TI_WAIT_RESP;
constexpr uint32_t LAB_DMA_TI_OSC_TX          = (DMA_TI_DREQ_SPI_TX << 16) | DMA_TI_DEST_DREQ | DMA_TI_WAIT_RESP | DMA_TI_SRC_INC;
constexpr uint32_t LAB_DMA_TI_OSC_RX          = (DMA_TI_DREQ_SPI_RX << 16) | DMA_TI_SRC_DREQ | DMA_TI_DEST_INC | DMA_TI_WAIT_RESP;
constexpr uint32_t LAB_DMA_TI_LOGAN_STORE     = (DMA_TI_DREQ_PWM << 16) | DMA_TI_DEST_DREQ | DMA_TI_DEST_INC | DMA_TI_WAIT_RESP;


// --- Oscilloscope ---
enum class LE_OSC_TRIG_MODE
{
  NONE,
  NORMAL,
  AUTO,
};

enum class LE_OSC_TRIG_TYPE
{
  EDGE,
};

enum class LE_OSC_TRIG_CND
{
  RISING,
  FALLING,
};

enum class LE_OSC_SCALING
{
  QUADRUPLE = 0,
  UNITY     = 1,
  HALF      = 2,
  FOURTH    = 4,
};

enum class LE_OSC_COUPLING
{
  DC = 0,
  AC = 1
};

namespace LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP
{
  constexpr int AXIS_LABEL_SIZE     = 9;
  constexpr int X_LABEL_COLOR       = FL_WHITE;
  constexpr int X_LABEL_INTRASPACE  = 18; 
  constexpr int Y_LABEL_COLOR       = FL_FOREGROUND_COLOR;
  constexpr int Y_LABEL_INTERSPACE  = 40; // spacing between columns of y-axis labels
  constexpr int Y_LABEL_UNIT_BOTTOM_MARGIN = 17; // padding of voltage unit of y-axis labels from top of grid
  constexpr int DEFAULT_LABEL_COLOR = FL_WHITE;
  constexpr int BACKGROUND_COLOR    = FL_BLACK;
  constexpr int Y_LABEL_UNIT_LEFT_MARGIN = 10;

  static std::array<int, 10> CHANNEL_COLORS = 
  {
    0x00000003,
    0x00000006,
    0x00000002,
    0x00000001,
  };
}

namespace LABSOFT_OSCILLOSCOPE_DISPLAY
{
  constexpr unsigned  NUMBER_OF_ROWS            = 10;
  constexpr unsigned  NUMBER_OF_COLUMNS         = 10;
  constexpr unsigned  SAMPLE_MARKING_THRESHOLD  = 40;
  constexpr unsigned  SAMPLE_MARKING_AMPLITUDE  = 5;
  constexpr unsigned  SAMPLE_MARKING_THICKNESS  = 1; // Final!
  constexpr int       BACKGROUND_COLOR          = FL_BLACK;
  constexpr int       GRID_COLOR                = FL_WHITE;     
}

namespace LAB_OSCILLOSCOPE
{
  // General
  constexpr unsigned          NUMBER_OF_CHANNELS            = 2;
  constexpr unsigned          NUMBER_OF_SAMPLES             = 2'000;
  constexpr unsigned          SAMPLE_SIZE                   = sizeof (uint32_t);

   // Uncached Oscilloscope DMA Data Info
  constexpr unsigned          BUFFER_LENGTH                 = SAMPLE_SIZE * NUMBER_OF_SAMPLES;
  constexpr unsigned          NUMBER_OF_BUFFERS                  = 2;
  constexpr unsigned          VC_MEM_SIZE                   = (PAGE_SIZE + (NUMBER_OF_CHANNELS * NUMBER_OF_BUFFERS * BUFFER_LENGTH));

  // Vertical
  constexpr LE_OSC_COUPLING   COUPLING                      = LE_OSC_COUPLING::DC;
  constexpr double            MIN_VOLTAGE_PER_DIVISION      = 0.0001;
  constexpr double            MAX_VOLTAGE_PER_DIVISION      = 5.0;
  constexpr double            VOLTAGE_PER_DIVISION          = 1.0;
  constexpr double            MIN_VERTICAL_OFFSET           = -1.0 * MAX_VOLTAGE_PER_DIVISION * (LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_ROWS / 2);
  constexpr double            MAX_VERTICAL_OFFSET           = -1.0 * MIN_VERTICAL_OFFSET;
  constexpr double            VERTICAL_OFFSET               = 0.0;
  constexpr LE_OSC_SCALING    SCALING                       = LE_OSC_SCALING::UNITY;

  // Horizontal
  constexpr double            MIN_TIME_PER_DIVISION         = 0.000001;
  constexpr double            MAX_TIME_PER_DIVISION         = 30.0;
  constexpr double            TIME_PER_DIVISION             = 0.005;
  constexpr double            MIN_SAMPLING_RATE             = NUMBER_OF_SAMPLES / (MAX_TIME_PER_DIVISION * LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS);
  constexpr double            MAX_SAMPLING_RATE             = 200'000;
  constexpr double            SAMPLING_RATE                 = NUMBER_OF_SAMPLES / (TIME_PER_DIVISION * LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS);
  constexpr double            MIN_HORIZONTAL_OFFSET         = -100.0; // -100s
  constexpr double            MAX_HORIZONTAL_OFFSET         = 100.0; // +100s  
  constexpr double            HORIZONTAL_OFFSET             = 0.0;

  // Trigger
  constexpr LE_OSC_TRIG_MODE  TRIGGER_MODE                  = LE_OSC_TRIG_MODE::NONE;
  constexpr unsigned          TRIGGER_SOURCE                = 0;
  constexpr LE_OSC_TRIG_TYPE  TRIGGER_TYPE                  = LE_OSC_TRIG_TYPE::EDGE;
  constexpr LE_OSC_TRIG_CND   TRIGGER_CONDITION             = LE_OSC_TRIG_CND::RISING;
  constexpr double            MIN_TRIGGER_LEVEL             = MIN_VERTICAL_OFFSET;
  constexpr double            MAX_TRIGGER_LEVEL             = MAX_VERTICAL_OFFSET;
  constexpr double            TRIGGER_LEVEL                 = 0.0;

  // ADC Info and Conversions
  constexpr unsigned          ADC_RESOLUTION_BITS           = 12;
  constexpr unsigned          ADC_RESOLUTION_INT            = std::pow (2, ADC_RESOLUTION_BITS);
  constexpr double            ADC_REFERENCE_VOLTAGE         = 5.0;
  constexpr double            CONVERSION_REFERENCE_VOLTAGE  = ADC_REFERENCE_VOLTAGE / 2.0;
  constexpr double            CONVERSION_CONSTANT           = CONVERSION_REFERENCE_VOLTAGE / ((ADC_RESOLUTION_INT - 1) >> 1);
  constexpr unsigned          RAW_DATA_SHIFT_BIT_COUNT      = (SAMPLE_SIZE * 8) / NUMBER_OF_CHANNELS;
  constexpr uint32_t          RAW_DATA_POST_SHIFT_MASK      = ((std::pow (2, RAW_DATA_SHIFT_BIT_COUNT)) - 1);
  constexpr unsigned          ADC_SPI0_CHIP_ENABLE          = 0;

  // Display
  constexpr LE::DISPLAY_MODE  OSC_DISP_MODE                 = LE::DISPLAY_MODE::REPEATED;
  constexpr double            MIN_TIME_PER_DIV_NO_ZOOM      = NUMBER_OF_SAMPLES / (MAX_SAMPLING_RATE * LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS);
  constexpr double            MIN_TIME_PER_DIV_DISP_SCREEN  = 1.0 / LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS;
}



struct LAB_Channel_Data_Oscilloscope
{
  // Channel
  bool            is_enabled            = false;

  // Vertical
  double          voltage_per_division  = LAB_OSCILLOSCOPE::VOLTAGE_PER_DIVISION;
  double          vertical_offset       = LAB_OSCILLOSCOPE::VERTICAL_OFFSET;
  LE_OSC_SCALING  scaling               = LAB_OSCILLOSCOPE::SCALING;
  LE_OSC_COUPLING coupling              = LAB_OSCILLOSCOPE::COUPLING;

  // Data/Samples
  std::array  <double, LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES> samples;
  std::vector <std::array<int, 2>>                          pixel_points;
};

class LAB_Parent_Data_Oscilloscope
{
  public:    
    // Horizontal
    double            time_per_division = LAB_OSCILLOSCOPE::TIME_PER_DIVISION;
    double            sampling_rate     = LAB_OSCILLOSCOPE::SAMPLING_RATE;
    double            horizontal_offset = LAB_OSCILLOSCOPE::HORIZONTAL_OFFSET;

    // Trigger 
    LE_OSC_TRIG_MODE  trig_mode         = LAB_OSCILLOSCOPE::TRIGGER_MODE;
    unsigned          trig_source       = LAB_OSCILLOSCOPE::TRIGGER_SOURCE;
    LE_OSC_TRIG_TYPE  trig_type         = LAB_OSCILLOSCOPE::TRIGGER_TYPE;
    LE_OSC_TRIG_CND   trig_condition    = LAB_OSCILLOSCOPE::TRIGGER_CONDITION;
    double            trig_level        = LAB_OSCILLOSCOPE::TRIGGER_LEVEL;

    bool              trig_flag_no_trig_found_yet = true;

    // Display  
    LE::DISPLAY_MODE  disp_mode         = LAB_OSCILLOSCOPE::OSC_DISP_MODE;


    // Data/Samples
    double            w_samp_count      = LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES;

    std::array<
      uint32_t, 
      LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES
    > raw_sample_buffer;

    std::array <
      LAB_Channel_Data_Oscilloscope, 
      LAB_OSCILLOSCOPE::NUMBER_OF_CHANNELS
    > channel_data;

    // State
    bool is_osc_core_running      = false; 
    bool is_osc_frontend_running  = false;

    // --- Functions ---
    bool has_enabled_channels ()
    {
      for (int a = 0; a < channel_data.size (); a++)
      {
        if (channel_data[a].is_enabled)
          return true;
      }

      return false;
    }
};

struct LAB_DMA_Data_Oscilloscope
{
  AP_DMA_CB cbs[15];

  uint32_t  spi_samp_size,
            spi_cs,
            spi_cs_fifo_reset,
            pwm_duty_cycle,
            txd;

  volatile uint32_t status[LAB_OSCILLOSCOPE::NUMBER_OF_CHANNELS];

  volatile uint32_t rxd [LAB_OSCILLOSCOPE::NUMBER_OF_CHANNELS]
                        [LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES];
};

// LABSoft Oscilloscope Display Group
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_0_VOLTAGE_PER_DIVISION = "1 V";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_0_VERTICAL_OFFSET      = "0 V";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_1_VOLTAGE_PER_DIVISION = "1 V";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_1_VERTICAL_OFFSET      = "0 V";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION              = "5 ms";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_SAMPLING_RATE                  = "40 kHz";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_HORIZONTAL_OFFSET              = "0 s";
constexpr const char* LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TRIGGER_LEVEL                  = "0 V";
constexpr int         LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_DISPLAY_MODE                   = (LAB_OSCILLOSCOPE::TIME_PER_DIVISION >= LAB_OSCILLOSCOPE::MIN_TIME_PER_DIV_DISP_SCREEN) ? 1 : 0; // 0 is repeated, 1 is screen


// --- LAB Voltmeter ---

// --- Function Generator ---

namespace LAB_FUNCTION_GENERATOR
{
  constexpr double  SIGNAL_GENERATOR_MIN_FREQUENCY  = 0.1;
  constexpr double  SIGNAL_GENERATOR_MAX_FREQUENCY  = 12'500'000.0;
  constexpr double  MIN_FREQUENCY                   = SIGNAL_GENERATOR_MIN_FREQUENCY;
  constexpr double  MAX_FREQUENCY                   = 1'000'000.0;
}

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

namespace LAB_CONST
{
  namespace FUNC_GEN
  {
    constexpr unsigned      NUMBER_OF_CHANNELS      = 1;
    constexpr double        SIG_GEN_FREQUENCY_MIN   = 0.1; // Hz
    constexpr double        SIG_GEN_FREQUENCY_MAX   = 1'000'00; // Hz
    constexpr double        DIGI_POT_AMPLITUDE_MIN  = -3.3; //V
    constexpr double        DIGI_POT_AMPLITUDE_MAX  = 3.3; //V
    constexpr double        FREQUENCY_MIN           = SIG_GEN_FREQUENCY_MIN;
    constexpr double        FREQUENCY_MAX           = 1'000'000; // Hz
    constexpr double        AMPLITUDE_MIN           = DIGI_POT_AMPLITUDE_MIN;
    constexpr double        AMPLITUDE_MAX           = DIGI_POT_AMPLITUDE_MAX;
    constexpr double        VERTICAL_OFFSET_MIN     = -5.0;
    constexpr double        VERTICAL_OFFSET_MAX     = 5.0;
    constexpr double        DUTY_CYCLE_MIN          = 0.0;
    constexpr double        DUTY_CYCLE_MAX          = 100.0;
    constexpr double        PHASE_MIN               = -360.0;
    constexpr double        PHASE_MAX               = 360.0;
    constexpr double        FREQUENCY               = 1'000; // Hz
    constexpr double        PERIOD                  = 1.0 / FREQUENCY;
    constexpr double        AMPLITUDE               = 1.0; // 1 volt
    constexpr double        VERTICAL_OFFSET         = 0.0;
    constexpr double        DUTY_CYCLE              = 50.0; // 50% duty cycle
    constexpr double        PHASE                   = 0.0; // 0 degree phase = in phase
    constexpr LE_WAVE_TYPE  WAVE_TYPE               = LE_WAVE_TYPE_SQUARE;
  }
}

constexpr double SIG_GEN_MIN_FREQ   = 0.1; 
constexpr double SIG_GEN_MAX_FREQ   = 12'500'000; 
constexpr int    SIG_GEN_REF_CLK_HZ = 25'000'000; 
constexpr int    SIG_GEN_2_POW_28   = 268'435'456;

struct LAB_Channel_Data_Function_Generator
{
  // Channel
  bool is_enabled = false;

  // Parameters
  LE_WAVE_TYPE  wave_type       = LAB_CONST::FUNC_GEN::WAVE_TYPE;
  double        frequency       = LAB_CONST::FUNC_GEN::FREQUENCY;
  double        period          = LAB_CONST::FUNC_GEN::PERIOD;
  double        amplitude       = LAB_CONST::FUNC_GEN::AMPLITUDE;
  double        vertical_offset = LAB_CONST::FUNC_GEN::VERTICAL_OFFSET;
  double        duty_cycle      = LAB_CONST::FUNC_GEN::DUTY_CYCLE;
  double        phase           = LAB_CONST::FUNC_GEN::PHASE;
};

struct LAB_Parent_Data_Function_Generator
{
  std::array <LAB_Channel_Data_Function_Generator, LAB_CONST::FUNC_GEN::NUMBER_OF_CHANNELS> channel_data;
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
namespace LABSOFT_LOGIC_ANALYZER_DISPLAY
{
  constexpr unsigned  NUMBER_OF_COLUMNS = 10;
}

namespace LAB_LOGIC_ANALYZER
{
  // General
  constexpr unsigned  NUMBER_OF_CHANNELS  = sizeof (LAB_PIN_LOGIC_ANALYZER) / sizeof (LAB_PIN_LOGIC_ANALYZER[0]);
  constexpr unsigned  NUMBER_OF_SAMPLES   = 2'000;
  constexpr unsigned  SAMPLE_SIZE         = sizeof (uint32_t);

  // Horizontal
  constexpr double            MIN_TIME_PER_DIVISION         = 0.000001;
  constexpr double            MAX_TIME_PER_DIVISION         = 30.0;
  constexpr double            TIME_PER_DIVISION             = 0.005;
  constexpr double            MIN_SAMPLING_RATE             = NUMBER_OF_SAMPLES / (MAX_TIME_PER_DIVISION * LABSOFT_LOGIC_ANALYZER_DISPLAY::NUMBER_OF_COLUMNS);
  constexpr double            MAX_SAMPLING_RATE             = 200'000;
  constexpr double            SAMPLING_RATE                 = NUMBER_OF_SAMPLES / (TIME_PER_DIVISION * LABSOFT_LOGIC_ANALYZER_DISPLAY::NUMBER_OF_COLUMNS);
  constexpr double            MIN_HORIZONTAL_OFFSET         = 0.0;
  constexpr double            MAX_HORIZONTAL_OFFSET         = 0.0;  
  constexpr double            HORIZONTAL_OFFSET             = 0.0;

  // Display
  constexpr LE::DISPLAY_MODE  OSC_DISP_MODE                 = LE::DISPLAY_MODE::REPEATED;
  constexpr double            MIN_TIME_PER_DIV_NO_ZOOM      = NUMBER_OF_SAMPLES / (MAX_SAMPLING_RATE * LABSOFT_LOGIC_ANALYZER_DISPLAY::NUMBER_OF_COLUMNS);
  constexpr double            MIN_TIME_PER_DIV_DISP_SCREEN  = 1.0 / LABSOFT_LOGIC_ANALYZER_DISPLAY::NUMBER_OF_COLUMNS;

  // Uncached Logic Analyzer DMA Data Info
  constexpr unsigned BUFFER_LENGTH        = SAMPLE_SIZE * NUMBER_OF_SAMPLES;
  constexpr unsigned NUMBER_OF_BUFFERS         = 2;
  constexpr unsigned VC_MEM_SIZE          = (PAGE_SIZE + (NUMBER_OF_CHANNELS * NUMBER_OF_BUFFERS * BUFFER_LENGTH));
}

namespace LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP
{
  /**
   * TO-DO: Change std::string variables to constexpr conversion functions.
   *        String creation will be in LABSoft_GUI's widgets.
  */

 static std::string SAMPLING_RATE     ("40 kHz");
 static std::string TIME_PER_DIVISION ("5 ms");
}


struct LAB_Channel_Data_Logic_Analyzer
{
  bool      is_enabled                        = true;
  unsigned  raw_sample_buffer_working_size      = 0;

  // Data/Samples
  std::vector <std::array<int, 2>> pixel_points;
  std::array  <bool, LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES> samples;
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
    double    sampling_rate       = LAB_LOGIC_ANALYZER::SAMPLING_RATE;
    double    time_per_division   = 0.0;
    double    horizontal_offset   = 0.0;
    unsigned  w_samp_count        = LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES;

    std::array <uint32_t, 
      LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES>   raw_sample_buffer;
      
    std::array <LAB_Channel_Data_Logic_Analyzer, 
      LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS>  channel_data;

    LE::DISPLAY_MODE disp_mode  = LE::DISPLAY_MODE::REPEATED;
};

struct LAB_DMA_Data_Logic_Analyzer
{
  AP_DMA_CB cbs[15];
  
  uint32_t  buffer_ok_flag = 0x1;

  volatile uint32_t status[2] = {0},
                    rxd[2][LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES] = {0};
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
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_NUMBER_OF_CHANNELS     = LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GRID_COLOR                   = FL_BLACK;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_STYLE             = FL_SOLID;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_COLOR             = FL_BLACK;
constexpr int       LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_WIDTH             = 2;
constexpr char*     LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_DASHES            = 0;
constexpr double    LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_P2P_SPREAD        = 60.0; // in percent


#endif

// EOF