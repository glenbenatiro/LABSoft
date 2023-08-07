#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <map>
#include <string>

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

#include "../../lib/AikaPi/AikaPi.h"
#include "../../lib/AD9833/AD9833.h"
#include "../../lib/MCP4XXX/MCP4XXX.h"
#include "../../lib/MCP23S17/MCP23S17.h"

#include "LAB_Enumerations.h"

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
    // oscilloscope
    constexpr unsigned OSC_ADC_SCLK   = 11;
    constexpr unsigned OSC_ADC_MOSI   = 10;
    constexpr unsigned OSC_ADC_MISO   = 9;
    constexpr unsigned OSC_ADC_CS     = 8;
    constexpr unsigned OSC_MUX_C0_A0  = 27;
    constexpr unsigned OSC_MUX_C0_A1  = 22;
    constexpr unsigned OSC_MUX_C1_A0  = 23;
    constexpr unsigned OSC_MUX_C1_A1  = 24;
    constexpr unsigned OSC_RELAY_C0   = 14;
    constexpr unsigned OSC_RELAY_C1   = 15; 

    // function generator
    constexpr unsigned FG_PWG_CS      = 13;
    constexpr unsigned FG_PWG_MISO    = 5;
    constexpr unsigned FG_PWG_MOSI    = 4;
    constexpr unsigned FG_PWG_SCLK    = 6;
    constexpr unsigned FG_DPOTS_SCLK  = 21;
    constexpr unsigned FG_DPOTS_MOSI  = 20;
    constexpr unsigned FG_DPOTS_MISO  = 19;
    constexpr unsigned FG_DPOT0_CS    = 18; 
    constexpr unsigned FG_DPOT1_CS    = 17; 
    constexpr unsigned FG_DPOT2_CS    = 16;

    // logic analyzer

    // Yes, GPIO pins 5 and 19 are used in Function Generator.
    // These are not used in Function Gen MISOs for digipots.
    // I'm relying on the fact that LAB_Logic_Analyzer is the
    // last to be initialized, after Function Generator. 
    // So LAB_Logic_Analyzer's GPIO pin init is called last.

    constexpr unsigned LOGIC_ANALYZER[] = {1, 5, 19, 26};   

    // digital circuit checker
    constexpr unsigned DCC_XPAND_CS   = 25;
    constexpr unsigned DCC_XPAND_MISO = 12;
    constexpr unsigned DCC_XPAND_MOSI = 3;
    constexpr unsigned DCC_XPAND_SCLK = 2;
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

  namespace OSC
  { 
    // General
    constexpr unsigned                  NUMBER_OF_CHANNELS              = 2;
    constexpr unsigned                  NUMBER_OF_SAMPLES               = 2'000;
    constexpr unsigned                  SAMPLE_SIZE                     = sizeof (uint32_t); // bytes

    // Uncached Oscilloscope DMA Data Info
    constexpr unsigned                  NUMBER_OF_BUFFERS               = 2;
    constexpr unsigned                  BUFFER_LENGTH                   = SAMPLE_SIZE * NUMBER_OF_SAMPLES;
    constexpr unsigned                  VC_MEM_SIZE                     = AP::RPI::PAGE_SIZE + (NUMBER_OF_CHANNELS * NUMBER_OF_BUFFERS * BUFFER_LENGTH);

    // ADC Info and Conversions   
    constexpr unsigned                  ADC_RESOLUTION_BITS             = 12; // MCP33111
    constexpr unsigned                  ADC_RESOLUTION_INT              = 1 << ADC_RESOLUTION_BITS;
    constexpr double                    ADC_REFERENCE_VOLTAGE           = 5.0; // 5 volts
    constexpr double                    CONVERSION_REFERENCE_VOLTAGE    = ADC_REFERENCE_VOLTAGE / 2.0;
    constexpr double                    CONVERSION_CONSTANT             = CONVERSION_REFERENCE_VOLTAGE / ((ADC_RESOLUTION_INT - 1) >> 1);
    constexpr unsigned                  RAW_DATA_SHIFT_BIT_COUNT        = (SAMPLE_SIZE * 8) / NUMBER_OF_CHANNELS;
    constexpr uint32_t                  RAW_DATA_POST_SHIFT_MASK        = (1 << RAW_DATA_SHIFT_BIT_COUNT) - 1;
    constexpr unsigned                  ADC_SPI_CHIP_ENABLE             = 0;

    // Vertical
    constexpr unsigned                  DISPLAY_NUMBER_OF_ROWS          = 10;
    constexpr double                    DISPLAY_NUMBER_OF_ROWS_HALF     = DISPLAY_NUMBER_OF_ROWS / 2.0;
    constexpr LABE::OSC::COUPLING       COUPLING                        = LABE::OSC::COUPLING::AC;
    constexpr double                    MAX_VOLTAGE_PER_DIVISION        = 5.0;    // 5 V
    constexpr double                    MIN_VOLTAGE_PER_DIVISION        = 0.0001; // 100 uV
    constexpr double                    VOLTAGE_PER_DIVISION            = 1.0;    // 1 V
    constexpr double                    MAX_VERTICAL_OFFSET             = MAX_VOLTAGE_PER_DIVISION * 
                                                                          (DISPLAY_NUMBER_OF_ROWS / 2.0); // 25 V
    constexpr double                    MIN_VERTICAL_OFFSET             = -1.0 * MAX_VERTICAL_OFFSET; // -25 V
    constexpr double                    VERTICAL_OFFSET                 = 0.0;
    constexpr LABE::OSC::SCALING        SCALING                         = LABE::OSC::SCALING::UNITY;

    // Horizontal
    constexpr unsigned              DISPLAY_NUMBER_OF_COLUMNS       = 10;
    constexpr double                MAX_SAMPLING_RATE               = 200'000.0;  // Hz
    constexpr double                MIN_SAMPLING_RATE               = 1.0;        // Hz
    constexpr double                SAMPLING_RATE                   = 40'000.0;   // Hz
    constexpr unsigned              MAX_SAMPLES                     = NUMBER_OF_SAMPLES;
    constexpr unsigned              MAX_SAMPLES_RECORDING           = 1'000'000;  
    constexpr unsigned              MIN_SAMPLES                     = 2;
    constexpr unsigned              SAMPLES                         = MAX_SAMPLES;
    constexpr double                MAX_TIME_PER_DIVISION           = MAX_SAMPLES / (MIN_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
    constexpr double                MAX_TIME_PER_DIVISION_RECORDING = MAX_SAMPLES_RECORDING / (MIN_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
    constexpr double                MIN_TIME_PER_DIVISION           = MIN_SAMPLES / (MAX_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
    constexpr double                MIN_TIME_PER_DIVISION_SCREEN    = 1.0 / DISPLAY_NUMBER_OF_COLUMNS;
    constexpr double                MIN_TIME_PER_DIVISION_NO_ZOOM   = NUMBER_OF_SAMPLES / (MAX_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
    constexpr double                TIME_PER_DIVISION               = SAMPLES / (SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);  
    constexpr double                MAX_HORIZONTAL_OFFSET           = MAX_SAMPLES / MIN_SAMPLING_RATE;  
    constexpr double                MIN_HORIZONTAL_OFFSET           = (-1) * MAX_HORIZONTAL_OFFSET;  
    constexpr double                HORIZONTAL_OFFSET               = 0.0;

    // Mode
    constexpr LABE::OSC::MODE       MODE                            = (TIME_PER_DIVISION < MIN_TIME_PER_DIVISION_SCREEN) ? 
                                                                        LABE::OSC::MODE::REPEATED : LABE::OSC::MODE::SCREEN;
    
    // Trigger
    constexpr LABE::OSC::TRIG::MODE TRIGGER_MODE                    = LABE::OSC::TRIG::MODE::NONE;
    constexpr unsigned              TRIGGER_SOURCE                  = 0; // Channel 1
    constexpr LABE::OSC::TRIG::TYPE TRIGGER_TYPE                    = LABE::OSC::TRIG::TYPE::EDGE;
    constexpr LABE::OSC::TRIG::CND  TRIGGER_CONDITION               = LABE::OSC::TRIG::CND::RISING;
    constexpr double                MAX_TRIGGER_LEVEL               = MAX_VERTICAL_OFFSET;
    constexpr double                MIN_TRIGGER_LEVEL               = MIN_VERTICAL_OFFSET;
    constexpr double                TRIGGER_LEVEL                   = 0.0;
    constexpr double                FIND_TRIGGER_TIMEOUT            = 2; // seconds

    // Display
    constexpr double                MIN_TIME_PER_DIV_NO_ZOOM        = NUMBER_OF_SAMPLES / (MAX_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
  };

  namespace OSC_DISPLAY
  {
    constexpr unsigned  NUMBER_OF_ROWS                = OSC::DISPLAY_NUMBER_OF_ROWS;
    constexpr unsigned  NUMBER_OF_COLUMNS             = OSC::DISPLAY_NUMBER_OF_COLUMNS;
    constexpr unsigned  MID_COLUMN_INDEX              = (NUMBER_OF_COLUMNS / 2) - 1;
    constexpr unsigned  MID_ROW_INDEX                 = (NUMBER_OF_ROWS / 2) - 1;
    constexpr double    NUMBER_OF_ROWS_HALF           = NUMBER_OF_ROWS / 2.0;             
    constexpr unsigned  NUMBER_OF_MINOR_TICKS         = 10;
    constexpr unsigned  X_AXIS_MAJOR_TICK_LENGTH      = 20;
    constexpr unsigned  X_AXIS_SEMI_MAJOR_TICK_LENGTH = 10;
    constexpr unsigned  X_AXIS_MINOR_TICK_LENGTH      = 5;
    constexpr unsigned  Y_AXIS_MAJOR_TICK_LENGTH      = 12;
    constexpr unsigned  Y_AXIS_SEMI_MAJOR_TICK_LENGTH = 6;
    constexpr unsigned  Y_AXIS_MINOR_TICK_LENGTH      = 3;
    constexpr unsigned  SAMPLE_MARKING_THRESHOLD      = 40;
    constexpr unsigned  SAMPLE_MARKING_AMPLITUDE      = 5;
    constexpr unsigned  SAMPLE_MARKING_THICKNESS      = 1;
    constexpr uint32_t  GRID_COLOR                    = 46; // light gray
    constexpr unsigned  BACKGROUND_COLOR              = FL_BLACK;
  };

  namespace OSC_DISPLAY_GROUP
  {
    constexpr int Y_LABEL_INTERSPACE          = 40; // spacing between columns of y-axis labels
    constexpr int Y_LABEL_UNIT_LEFT_MARGIN    = 10;
    constexpr int Y_LABEL_UNIT_BOTTOM_MARGIN  = 17; // padding of voltage unit of y-axis labels from top of grid
    constexpr int X_LABEL_INTRASPACE          = 18; 
    constexpr int AXIS_LABEL_SIZE             = 9;

    constexpr int X_LABEL_COLOR               = FL_WHITE;
    constexpr int Y_LABEL_COLOR               = FL_FOREGROUND_COLOR;
    constexpr int DEFAULT_LABEL_COLOR         = FL_WHITE;
    constexpr int BACKGROUND_COLOR            = FL_BLACK;
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
    // General
    constexpr unsigned                NUMBER_OF_CHANNELS              = sizeof (PIN::LOGIC_ANALYZER) / sizeof (PIN::LOGIC_ANALYZER[0]);
    constexpr unsigned                NUMBER_OF_SAMPLES               = 2'000;
    constexpr unsigned                SAMPLE_SIZE                     = sizeof (uint32_t); // bytes

    // Uncached Oscilloscope DMA Data Info
    constexpr unsigned                NUMBER_OF_BUFFERS               = 2;
    constexpr unsigned                BUFFER_LENGTH                   = SAMPLE_SIZE * NUMBER_OF_SAMPLES;
    constexpr unsigned                VC_MEM_SIZE                     = AP::RPI::PAGE_SIZE + (NUMBER_OF_CHANNELS * NUMBER_OF_BUFFERS * BUFFER_LENGTH);

    // Horizontal
    constexpr unsigned                DISPLAY_NUMBER_OF_COLUMNS       = 10;
    constexpr double                  MAX_SAMPLING_RATE               = 200'000.0;  // Hz
    constexpr double                  MIN_SAMPLING_RATE               = 1.0;        // Hz
    constexpr double                  SAMPLING_RATE                   = 40'000.0;   // Hz
    constexpr unsigned                MAX_SAMPLES                     = NUMBER_OF_SAMPLES;
    constexpr unsigned                MAX_SAMPLES_RECORDING           = 1'000'000;  
    constexpr unsigned                MIN_SAMPLES                     = 2;
    constexpr unsigned                SAMPLES                         = MAX_SAMPLES;
    constexpr double                  MAX_TIME_PER_DIVISION           = MAX_SAMPLES / (MIN_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
    constexpr double                  MAX_TIME_PER_DIVISION_RECORDING = MAX_SAMPLES_RECORDING / (MIN_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
    constexpr double                  MIN_TIME_PER_DIVISION           = MIN_SAMPLES / (MAX_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
    constexpr double                  MIN_TIME_PER_DIVISION_SCREEN    = 1.0 / DISPLAY_NUMBER_OF_COLUMNS;
    constexpr double                  MIN_TIME_PER_DIVISION_NO_ZOOM   = NUMBER_OF_SAMPLES / (MAX_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
    constexpr double                  MAX_HORIZONTAL_OFFSET           = MAX_SAMPLES / MIN_SAMPLING_RATE;  
    constexpr double                  MIN_HORIZONTAL_OFFSET           = (-1) * MAX_HORIZONTAL_OFFSET;  

    // Mode
    constexpr LABE::LOGAN::MODE       MODE                            = (TIME_PER_DIVISION < MIN_TIME_PER_DIVISION_SCREEN) ? 
                                                                        LABE::LOGAN::MODE::REPEATED : LABE::LOGAN::MODE::SCREEN;
    
    // Trigger
    constexpr LABE::LOGAN::TRIG::MODE TRIGGER_MODE                    = LABE::LOGAN::TRIG::MODE::NONE;
    constexpr double                  FIND_TRIGGER_TIMEOUT            = 2; // seconds

    // Display
    constexpr double                  MIN_TIME_PER_DIV_NO_ZOOM        = NUMBER_OF_SAMPLES / (MAX_SAMPLING_RATE * DISPLAY_NUMBER_OF_COLUMNS);
  };

  namespace LOGAN_DISPLAY
  {
    constexpr unsigned  TOP_INFO_STRIP_HEIGHT                 = 30; 
    constexpr unsigned  DISPLAY_STATUS_BOX_HEIGHT             = TOP_INFO_STRIP_HEIGHT;
    constexpr unsigned  DISPLAY_STATUS_BOX_WIDTH              = 90;
    constexpr unsigned  CHANNEL_BUTTON_HEIGHT                 = 60;
    constexpr unsigned  CHANNEL_BUTTON_WIDTH                  = 180;
    constexpr unsigned  INTERNAL_DISPLAY_BG_COLOR             = FL_WHITE;

    constexpr unsigned  TIME_PER_DIVISION_LABELS_STRIP_HEIGHT = 30;
    constexpr unsigned  TIME_PER_DIVISION_LABELS_TOP_MARGIN   = 5;
    constexpr unsigned  TIME_PER_DIVISION_LABELS_COLOR        = FL_FOREGROUND_COLOR;
    constexpr unsigned  TIME_PER_DIVISION_LABELS_SIZE         = 10;
    constexpr unsigned  TIME_PER_DIVSION_LABELS_LAST_OFFSET   = 20;

    static constexpr uint32_t CHANNEL_COLORS[] = 
    {
      212, // light pink
      60, // green
      152, // violet
      122 // brick?
    };

    constexpr int       CHANNEL_HEIGHT                         = 60;
    constexpr int       CHANNEL_DRAGGER_WIDTH                  = 20;
    constexpr int       CHANNEL_NAME_WIDTH                     = 90;
    constexpr int       CHANNEL_SETTING_WIDTH                  = 30;
    constexpr int       CHANNEL_DIO_WIDTH                      = 60;
    constexpr int       CHANNEL_TRIGGER_WIDTH                  = 40;
    constexpr int       CHANNEL_INFO_WIDTH                     = CHANNEL_DRAGGER_WIDTH + 
                                                                  CHANNEL_NAME_WIDTH + 
                                                                  CHANNEL_SETTING_WIDTH + 
                                                                  CHANNEL_DIO_WIDTH + 
                                                                  CHANNEL_TRIGGER_WIDTH;
    constexpr double  CHANNEL_GRAPH_PEAK_TO_PEAK_SPREAD       = 60.0; // in percent, with the graph widget height as max

    constexpr Fl_Boxtype  GRAPH_BOX         = FL_THIN_DOWN_BOX;
    constexpr unsigned    GRAPH_BG_COLOR    = 7;
    constexpr unsigned    GRAPH_LINE_COLOR  = FL_BLACK;
    constexpr int         GRAPH_LINE_STYLE  = FL_SOLID;
    constexpr int         GRAPH_LINE_WIDTH         = 2;
    constexpr char*       GRAPH_LINE_DASHES = 0;

    constexpr unsigned  NUMBER_OF_COLUMNS        = LOGAN::DISPLAY_NUMBER_OF_COLUMNS;
    constexpr int       BG_COLOR                 = FL_WHITE;
    constexpr int       GROUP_NUMBER_OF_CHANNELS = LOGAN::NUMBER_OF_CHANNELS;
    constexpr int       GRID_COLOR               = FL_BLACK;

    constexpr int       STATUS_WIDTH                          = 90;
  };

  // namespace LOGAN_DISPLAY_GROUP
  // {
  //   constexpr int       X_LABEL_STRIP_HEIGHT  = 30;
  //   constexpr int       X_LABEL_SIZE          = 10;
  //   constexpr unsigned  CHANNEL_BUTTON_HEIGHT = 60;
  //   constexpr unsigned  CHANNEL_BUTTON_WIDTH  = 180;

    
  // }

  namespace DIGITAL_CIRCUIT_CHECKER
  {
    constexpr MCP23S17::PORT  INPUT_PORT    = MCP23S17::PORT::B;
    constexpr MCP23S17::PORT  OUTPUT_PORT   = MCP23S17::PORT::A;
    constexpr double          IC_FREQUENCY  = 100'000.0; // Hz
  };
};

#endif