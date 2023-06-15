#ifndef DEFAULTS_H
#define DEFAULTS_H

// #include <cmath>
#include <vector>
#include <array>
#include <map>
#include <string>

#include <FL/Enumerations.H>
#include <FL/fl_draw.H>

#include "../lib/AikaPi/AikaPi.h"
#include "../lib/AD9833/AD9833.h"
#include "../lib/MCP4XXX/MCP4XXX.h"

// LAB Functions
namespace LABF
{
  bool compare_double   (double a, double b, double epsilon = 1e-9);
  bool is_within_range  (double value, double min, double max);
  int  normalize        (int input, int min_input, int max_input, int min_output, int max_output);
};

// LAB Enumerations
namespace LABE
{
  namespace DMA
  {
    enum class BUFFER_COUNT
    {
      SINGLE,
      DOUBLE
    };
  };
  
  namespace DISPLAY
  {
    enum class MODE
    {
      SCREEN,
      REPEATED,
      SHIFT,
      RECORD
    };

    enum class COLOR : uint32_t
    {
      RED     = 0xFF000000,
      YELLOW  = 0xFFFF0000,
      GREEN   = 0x00FF0000
    };
  };
  
  namespace OSC
  {
    namespace TRIG
    {
      enum class MODE
      {
        NONE,
        AUTO,
        NORMAL
      };

      enum class TYPE
      {
        LEVEL,
        EDGE
      };

      enum class CND
      {
        RISING,
        FALLING,
        EITHER
      };
    }

    enum class SCALING
    {
      QUADRUPLE = 0,
      UNITY     = 1,
      HALF      = 2,
      FOURTH    = 4
    };

    enum class COUPLING
    {
      DC = 0,
      AC = 1
    };

    enum class STATUS
    {
      READY,
      STOP,
      CONFIG,
      AUTO,
      ARMED,
      TRIGGERED,
      DONE
    };
  
  };

  namespace FUNC_GEN
  {
    enum class WAVE_TYPE
    {
      SINE,
      TRIANGLE,
      SQUARE,
      SQUARE_HALF,
      SQUARE_FULL,
      DC
    };
  }
};

// LAB Constants
namespace LABC
{
  namespace LABSOFT
  {
    constexpr float DISPLAY_UPDATE_RATE = 1.0 / 25.0; // 25 fps

    enum class FL_TAB_GROUP_COLOR : uint32_t
    {
      GREEN   = 63,
      DEFAULT = 53
    };
  }

  namespace PIN
  {
    // Oscilloscope
    constexpr unsigned OSC_MUX_SCALER_CHAN_0_A0   = 27;
    constexpr unsigned OSC_MUX_SCALER_CHAN_0_A1   = 22;
    constexpr unsigned OSC_MUX_SCALER_CHAN_1_A0   = 23;
    constexpr unsigned OSC_MUX_SCALER_CHAN_1_A1   = 24;
    constexpr unsigned OSC_COUPLING_SELECT_CHAN_0 = 14;
    constexpr unsigned OSC_COUPLING_SELECT_CHAN_1 = 15; 
  
    // PWM
    constexpr unsigned PWM                        = 12;

    // Function Generator
    constexpr int FUNC_GEN_IC_CS                  = 13;
    constexpr int FUNC_GEN_IC_MISO                = 5; // not used
    constexpr int FUNC_GEN_IC_MOSI                = 4;
    constexpr int FUNC_GEN_IC_SCLK                = 6;

    // Logic Analyzer
    constexpr unsigned LOGIC_ANALYZER []          = {0, 1, 26};
  };

  namespace DMA
  {
    namespace TI
    {
      constexpr uint32_t OSC_PWM_PACING  = (AP::DMA::TI_DATA::PERMAP (AP::DMA::PERIPH_DREQ::PWM))  
                                            | AP::DMA::TI_DATA::DEST_DREQ      
                                            | AP::DMA::TI_DATA::WAIT_RESP
                                            | AP::DMA::TI_DATA::INTEN;

      constexpr uint32_t OSC_TX          = (AP::DMA::TI_DATA::PERMAP (AP::DMA::PERIPH_DREQ::SPI_TX))  
                                            | AP::DMA::TI_DATA::DEST_DREQ         
                                            | AP::DMA::TI_DATA::WAIT_RESP         
                                            | AP::DMA::TI_DATA::SRC_INC
                                            | AP::DMA::TI_DATA::INTEN;

      constexpr uint32_t OSC_RX          = (AP::DMA::TI_DATA::PERMAP (AP::DMA::PERIPH_DREQ::SPI_RX))  
                                            | AP::DMA::TI_DATA::SRC_DREQ         
                                            | AP::DMA::TI_DATA::DEST_INC          
                                            | AP::DMA::TI_DATA::WAIT_RESP;
                                        
      constexpr uint32_t LOGAN_STORE     = (AP::DMA::TI_DATA::PERMAP (AP::DMA::PERIPH_DREQ::PWM))  
                                            | AP::DMA::TI_DATA::SRC_DREQ  
                                            | AP::DMA::TI_DATA::DEST_INC  
                                            | AP::DMA::TI_DATA::WAIT_RESP;
    };

    namespace CHAN
    {
      // DMA channels in-use after reboot (3B+): 2, 3, 4, 6

      constexpr unsigned PWM_PACING       = 7;
      constexpr unsigned OSC_RX           = 8;
      constexpr unsigned OSC_TX           = 9;
      constexpr unsigned LOGAN_GPIO_STORE = 10;
    };
  };

  namespace SPI
  {
    // Please note that LABSoft requires the core clock to be fixed to 
    // 250 MHz in boot/config.txt. This affects the divisor setting.
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

  namespace DISPLAY
  {
    constexpr int DISPLAY_STATUS_BOX_TYPE = FL_BORDER_FRAME;

    static constexpr uint32_t CHAN_COLORS[] = {
      3, // yellow
      6, // cyan
      2, // green
      5, // purple
    };
  };

  // Oscilloscope
  namespace OSC
  { 
    // General
    constexpr unsigned                  NUMBER_OF_CHANNELS            = 2;
    constexpr unsigned                  MAX_NUMBER_OF_SAMPLES         = 2'000;
    constexpr unsigned                  MIN_NUMBER_OF_SAMPLES         = 20;
    constexpr unsigned                  NUMBER_OF_SAMPLES             = MAX_NUMBER_OF_SAMPLES;
    constexpr unsigned                  SAMPLE_SIZE                   = sizeof (uint32_t); // bytes

    // Uncached Oscilloscope DMA Data Info
    constexpr unsigned                  BUFFER_LENGTH                 = SAMPLE_SIZE * NUMBER_OF_SAMPLES;
    constexpr unsigned                  NUMBER_OF_BUFFERS             = 2;
    constexpr unsigned                  VC_MEM_SIZE                   = AP::RPI::PAGE_SIZE + (NUMBER_OF_CHANNELS * NUMBER_OF_BUFFERS * BUFFER_LENGTH);

    // ADC Info and Conversions   
    constexpr unsigned                  ADC_RESOLUTION_BITS           = 12; // MCP33111
    constexpr unsigned                  ADC_RESOLUTION_INT            = 1 << ADC_RESOLUTION_BITS;
    constexpr double                    ADC_REFERENCE_VOLTAGE         = 5.0; // 5 volts
    constexpr double                    CONVERSION_REFERENCE_VOLTAGE  = ADC_REFERENCE_VOLTAGE / 2.0;
    constexpr double                    CONVERSION_CONSTANT           = CONVERSION_REFERENCE_VOLTAGE / ((ADC_RESOLUTION_INT - 1) >> 1);
    constexpr unsigned                  RAW_DATA_SHIFT_BIT_COUNT      = (SAMPLE_SIZE * 8) / NUMBER_OF_CHANNELS;
    constexpr uint32_t                  RAW_DATA_POST_SHIFT_MASK      = (1 << RAW_DATA_SHIFT_BIT_COUNT) - 1;
    constexpr unsigned                  ADC_SPI_CHIP_ENABLE           = 0;

    // Vertical
    constexpr unsigned                  DISPLAY_NUMBER_OF_ROWS        = 10;
    constexpr LABE::OSC::COUPLING       COUPLING                      = LABE::OSC::COUPLING::AC;
    constexpr double                    MAX_VOLTAGE_PER_DIVISION      = 5.0;    // 5 V
    constexpr double                    MIN_VOLTAGE_PER_DIVISION      = 0.0001; // 100 uV
    constexpr double                    VOLTAGE_PER_DIVISION          = 1.0;    // 1 V
    constexpr double                    MAX_VERTICAL_OFFSET           = MAX_VOLTAGE_PER_DIVISION * 
                                                                          (DISPLAY_NUMBER_OF_ROWS / 2.0); // 25 V
    constexpr double                    MIN_VERTICAL_OFFSET           = -1.0 * MAX_VERTICAL_OFFSET; // -25 V
    constexpr double                    VERTICAL_OFFSET               = 0.0;
    constexpr LABE::OSC::SCALING        SCALING                       = LABE::OSC::SCALING::UNITY;

    // Horizontal
    constexpr unsigned                  DISPLAY_NUMBER_OF_COLUMNS     = 10;
    constexpr double                    MAX_HORIZONTAL_OFFSET         = 100.0;    // +100s
    constexpr double                    MIN_HORIZONTAL_OFFSET         = -100.0;   // -100s
    constexpr double                    HORIZONTAL_OFFSET             = 0.0;
    constexpr double                    MAX_TIME_PER_DIVISION         = 30.0;     // 30s
    constexpr double                    MIN_TIME_PER_DIVISION         = 0.000001; // 1us
    constexpr double                    MAX_SAMPLING_RATE             = 200'000;  // 200 kHz
    constexpr double                    MIN_SAMPLING_RATE             = NUMBER_OF_SAMPLES / (MAX_TIME_PER_DIVISION * 
                                                                          DISPLAY_NUMBER_OF_COLUMNS); // 6.67 Hz
    constexpr double                    SAMPLING_RATE                 = 40'000;   // 40 kHz
    constexpr double                    TIME_PER_DIVISION             = NUMBER_OF_SAMPLES / (SAMPLING_RATE *
                                                                          DISPLAY_NUMBER_OF_COLUMNS); // 5 ms
    
    // Trigger
    constexpr LABE::OSC::TRIG::MODE     TRIGGER_MODE                  = LABE::OSC::TRIG::MODE::NONE;
    constexpr unsigned                  TRIGGER_SOURCE                = 0; // Channel 1
    constexpr LABE::OSC::TRIG::TYPE     TRIGGER_TYPE                  = LABE::OSC::TRIG::TYPE::EDGE;
    constexpr LABE::OSC::TRIG::CND      TRIGGER_CONDITION             = LABE::OSC::TRIG::CND::RISING;
    constexpr double                    MAX_TRIGGER_LEVEL             = MAX_VERTICAL_OFFSET;
    constexpr double                    MIN_TRIGGER_LEVEL             = MIN_VERTICAL_OFFSET;
    constexpr double                    TRIGGER_LEVEL                 = 0.0;
    constexpr double                    FIND_TRIGGER_TIMEOUT          = 2; // seconds

    // Display
    constexpr LABE::DISPLAY::MODE       OSC_DISP_MODE                 = LABE::DISPLAY::MODE::REPEATED;
    constexpr double                    MIN_TIME_PER_DIV_NO_ZOOM      = NUMBER_OF_SAMPLES / (MAX_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
    constexpr double                    MIN_TIME_PER_DIV_DISP_SCREEN  = 1.0 / DISPLAY_NUMBER_OF_COLUMNS;
  };

  namespace OSC_DISPLAY
  {
    constexpr unsigned NUMBER_OF_ROWS                 = OSC::DISPLAY_NUMBER_OF_ROWS;
    constexpr unsigned NUMBER_OF_COLUMNS              = OSC::DISPLAY_NUMBER_OF_COLUMNS;
    constexpr unsigned NUMBER_OF_MINOR_TICKS          = 10;
    constexpr unsigned X_AXIS_MAJOR_TICK_LENGTH       = 20;
    constexpr unsigned X_AXIS_SEMI_MAJOR_TICK_LENGTH  = 10;
    constexpr unsigned X_AXIS_MINOR_TICK_LENGTH       = 5;
    constexpr unsigned Y_AXIS_MAJOR_TICK_LENGTH       = 12;
    constexpr unsigned Y_AXIS_SEMI_MAJOR_TICK_LENGTH  = 6;
    constexpr unsigned Y_AXIS_MINOR_TICK_LENGTH       = 3;

    namespace COLOR
    {
      constexpr uint32_t GRID = 46;
    };
  };

  namespace VOLTMETER
  {
    constexpr double SAMPLING_RATE = 50'000.0; // Hz
  };

  namespace FUNC_GEN
  {
    constexpr unsigned                  NUMBER_OF_CHANNELS      = 1;
    constexpr double                    MAX_AMPLITUDE_DIGI_POT  = MCP4XXX::MAX_AMPLITUDE; // V
    constexpr double                    MIN_AMPLITUDE_DIGI_POT  = MCP4XXX::MIN_AMPLITUDE; // V
    constexpr double                    MAX_AMPLITUDE           = MCP4XXX::MAX_VDD_VOLTAGE_WRT_VSS; // V
    constexpr double                    MIN_AMPLITUDE           = 0.1; // 100 mV
    constexpr double                    MAX_FREQUENCY           = AD9833::MAX_FREQUENCY;  // Hz
    constexpr double                    MIN_FREQUENCY           = AD9833::MIN_FREQUENCY;  // Hz
    constexpr double                    MAX_PERIOD              = 1.0 / MIN_FREQUENCY;    // s
    constexpr double                    MIN_PERIOD              = 1.0 / MAX_FREQUENCY;    // s
    constexpr double                    MAX_PHASE               = 360.0;                  // degrees
    constexpr double                    MIN_PHASE               = -360.0;                 // degrees
    constexpr double                    MAX_VERTICAL_OFFSET     = 10075;                    // V
    constexpr double                    MIN_VERTICAL_OFFSET     = 75;                   // V

    constexpr LABE::FUNC_GEN::WAVE_TYPE WAVE_TYPE               = LABE::FUNC_GEN::WAVE_TYPE::SINE;
    constexpr double                    AMPLITUDE               = 1.0;
    constexpr double                    FREQUENCY               = 1'000.0;
    constexpr double                    PERIOD                  = 1.0 / FREQUENCY;
    constexpr double                    PHASE                   = 0.0;
    constexpr double                    VERTICAL_OFFSET         = 0.0;

    constexpr double                    IC_FREQUENCY            = 100'000.0;  // Hz

    // see differential amplifier diagram here
    // https://en.wikipedia.org/wiki/Operational_amplifier_applications#Differential_amplifier_.28difference_amplifier.29

    constexpr double                    R1_RESISTANCE           = 1'000.0;  // ohms
    constexpr double                    RF_RESISTANCE           = 1'000.0;  // ohms
    constexpr double                    V1                      = 5.0;      // V   
    constexpr double                    V2                      = 5.0;      // V
    constexpr double                    ROFF_RESISTANCE         = static_cast<uint32_t>
                                                                    (MCP4XXX::RESISTANCE_VERSION::_103); 
                                                                 // total offset digi pot resistance       
  };

  namespace LOGAN
  {
    constexpr unsigned NUMBER_OF_SAMPLES = 2'000;
  };
};


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

namespace GUI_LBL
{
  static std::map <LABE::DISPLAY::MODE, std::string> DISPLAY_MODE = 
  {
    {LABE::DISPLAY::MODE::SCREEN,    "Screen"},
    {LABE::DISPLAY::MODE::REPEATED,  "Repeated"}
  };
}

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

struct LAB_Channel_Data_Oscilloscope
{
  // Channel
  bool            is_enabled            = false;

  // Vertical
  double              voltage_per_division = LABC::OSC::VOLTAGE_PER_DIVISION;
  double              vertical_offset      = LABC::OSC::VERTICAL_OFFSET;
  LABE::OSC::SCALING  scaling              = LABC::OSC::SCALING;
  LABE::OSC::COUPLING coupling             = LABC::OSC::COUPLING;

  // Data/Samples
  std::array  <double, LABC::OSC::NUMBER_OF_SAMPLES> samples;
  std::vector <std::array<int, 2>>                          pixel_points;
};

class LAB_Parent_Data_Oscilloscope
{
  public:    
    // State 
    bool                  is_osc_core_running     = false; 
    bool                  is_osc_frontend_running = false;
    LABE::OSC::STATUS     status                  = LABE::OSC::STATUS::READY;
    bool                  single                  = false;

    // Horizontal
    double                horizontal_offset       = LABC::OSC::HORIZONTAL_OFFSET;
    double                time_per_division       = LABC::OSC::TIME_PER_DIVISION;
    unsigned              samples                 = LABC::OSC::NUMBER_OF_SAMPLES;
    double                sampling_rate           = LABC::OSC::SAMPLING_RATE;

    // Display
    LABE::DISPLAY::MODE   display_mode            = LABC::OSC::OSC_DISP_MODE;

    // Data/Samples
    double                w_samp_count            = LABC::OSC::NUMBER_OF_SAMPLES;

    std::array<
      uint32_t, 
      LABC::OSC::NUMBER_OF_SAMPLES
    > raw_sample_buffer;

    std::array <
      LAB_Channel_Data_Oscilloscope, 
      LABC::OSC::NUMBER_OF_CHANNELS
    > channel_data;

    // Trigger 
    LABE::OSC::TRIG::MODE trig_mode               = LABC::OSC::TRIGGER_MODE;
    unsigned              trig_source             = LABC::OSC::TRIGGER_SOURCE;
    LABE::OSC::TRIG::TYPE trig_type               = LABC::OSC::TRIGGER_TYPE;
    LABE::OSC::TRIG::CND  trig_condition          = LABC::OSC::TRIGGER_CONDITION;
    double                trig_level              = LABC::OSC::TRIGGER_LEVEL;
    uint32_t              trig_level_bits         = (LABC::OSC::ADC_RESOLUTION_INT - 1) / 2;
    unsigned              find_trig_sample_skip   = 4;

    unsigned              trig_buff_index             = 0;
    unsigned              trig_index              = 0;
    bool                  find_trigger            = false; 
    bool                  trig_frame_ready        = false;
    bool                  trig_found              = false;

    struct TriggerBuffers
    {
      std::array<
        std::array<uint32_t, LABC::OSC::NUMBER_OF_SAMPLES>,
        LABC::OSC::NUMBER_OF_CHANNELS
      > pre_trig;

      std::array<
        std::array<uint32_t, LABC::OSC::NUMBER_OF_SAMPLES>,
        LABC::OSC::NUMBER_OF_CHANNELS
      > post_trigger;

      std::array<uint32_t, LABC::OSC::NUMBER_OF_SAMPLES> assembled_block;
    } trig_buffs;      

    // Record
    unsigned record_number_of_samples = 0;

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
};

struct LAB_DMA_Data_Oscilloscope
{
  AP::DMA::CTL_BLK cbs[15];

  uint32_t  spi_samp_size,
            spi_cs,
            spi_cs_fifo_reset,
            pwm_duty_cycle,
            txd;

  volatile uint32_t status[LABC::OSC::NUMBER_OF_CHANNELS];

  volatile uint32_t rxd [LABC::OSC::NUMBER_OF_CHANNELS]
                        [LABC::OSC::NUMBER_OF_SAMPLES];
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
constexpr int         LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_DISPLAY_MODE                   = (LABC::OSC::TIME_PER_DIVISION >= LABC::OSC::MIN_TIME_PER_DIV_DISP_SCREEN) ? 1 : 0; // 0 is repeated, 1 is screen


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

constexpr double SIG_GEN_MIN_FREQ   = 0.1; 
constexpr double SIG_GEN_MAX_FREQ   = 12'500'000; 
constexpr int    SIG_GEN_REF_CLK_HZ = 25'000'000; 
constexpr int    SIG_GEN_2_POW_28   = 268'435'456;

struct LAB_Channel_Data_Function_Generator
{
  // Channel
  bool is_enabled = false;

  // Parameters
  LABE::FUNC_GEN::WAVE_TYPE wave_type       = LABC::FUNC_GEN::WAVE_TYPE;
  double                    frequency       = LABC::FUNC_GEN::FREQUENCY;
  double                    period          = LABC::FUNC_GEN::PERIOD;
  double                    amplitude       = LABC::FUNC_GEN::AMPLITUDE;
  double                    vertical_offset = LABC::FUNC_GEN::VERTICAL_OFFSET;
  double                    phase           = LABC::FUNC_GEN::PHASE;
  double                    Rf              = LABC::FUNC_GEN::RF_RESISTANCE;
};

struct LAB_Parent_Data_Function_Generator
{
  bool is_enabled = false;

  std::array <LAB_Channel_Data_Function_Generator, LABC::FUNC_GEN::NUMBER_OF_CHANNELS> channel_data;
};

// LABSoft Function Generator
#define LABSOFT_FUNCTION_GENERATOR_HORIZONTAL_POSITION "0 s"
#define LABSOFT_FUNCTION_GENERATOR_TIME_PER_DIVISION   "1 s/div"
#define LABSOFT_FUNCTION_GENERATOR_VERTICAL_OFFSET     "0 s"
#define LABSOFT_FUNCTION_GENERATOR_WAVE_TYPE  "Sine"
#define LABSOFT_FUNCTION_GENERATOR_FREQUENCY  "1 kHz"
#define LABSOFT_FUNCTION_GENERATOR_PERIOD     "1 ms"
#define LABSOFT_FUNCTION_GENERATOR_AMPLITUDE  "1 V"
#define LABSOFT_FUNCTION_GENERATOR_Y_OFFSET   "0 V"
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
  constexpr LABE::DISPLAY::MODE  OSC_DISP_MODE                 = LABE::DISPLAY::MODE::REPEATED;
  constexpr double            MIN_TIME_PER_DIV_NO_ZOOM      = NUMBER_OF_SAMPLES / (MAX_SAMPLING_RATE * LABSOFT_LOGIC_ANALYZER_DISPLAY::NUMBER_OF_COLUMNS);
  constexpr double            MIN_TIME_PER_DIV_DISP_SCREEN  = 1.0 / LABSOFT_LOGIC_ANALYZER_DISPLAY::NUMBER_OF_COLUMNS;

  // Uncached Logic Analyzer DMA Data Info
  constexpr unsigned BUFFER_LENGTH        = SAMPLE_SIZE * NUMBER_OF_SAMPLES;
  constexpr unsigned NUMBER_OF_BUFFERS         = 2;
  constexpr unsigned VC_MEM_SIZE          = (AP::RPI::PAGE_SIZE + (NUMBER_OF_CHANNELS * NUMBER_OF_BUFFERS * BUFFER_LENGTH));
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

    std::array <
      uint32_t, 
      LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES
    > raw_sample_buffer;
      
    std::array <LAB_Channel_Data_Logic_Analyzer, 
      LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS> channel_data;

    LABE::DISPLAY::MODE display_mode  = LABE::DISPLAY::MODE::REPEATED;
};

struct LAB_DMA_Data_Logic_Analyzer
{
  AP::DMA::CTL_BLK cbs[15];
  
  uint32_t  buffer_ok_flag = 0x1;

  volatile  uint32_t status [LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS];
  volatile  uint32_t rxd    [LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS]
                            [LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES];
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