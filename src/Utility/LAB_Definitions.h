#ifndef LAB_DEFINITIONS_H
#define LAB_DEFINITIONS_H

#include <array>
#include <vector>

#include "LAB_Constants.h"
#include "LAB_Defaults.h"
#include "LAB_Enumerations.h"

struct LAB_Channel_Data_Oscilloscope
{
  // State
  bool                is_enabled            = false;

  // Vertical
  double              voltage_per_division  = LABC::OSC::VOLTAGE_PER_DIVISION;
  double              vertical_offset       = LABC::OSC::VERTICAL_OFFSET;
  LABE::OSC::SCALING  scaling               = LABC::OSC::SCALING;
  LABE::OSC::COUPLING coupling              = LABC::OSC::COUPLING;

  // Data/Samples/Pixels
  std::array<double, LABC::OSC::NUMBER_OF_SAMPLES>  samples;
  std::vector<double>                               recording_samples;
  std::vector<std::array<int, 2>>                   pixel_points;
};

class LAB_Parent_Data_Oscilloscope
{
  public:    
    // State 
    bool              is_osc_core_running     = false; 
    bool              is_osc_frontend_running = false;
    bool              single                  = false;
    LABE::OSC::STATUS status                  = LABE::OSC::STATUS::READY;
    
    // Mode
    LABE::OSC::MODE mode                      = LABC::OSC::MODE;
    LABE::OSC::MODE last_mode_before_repeated = mode; 
    bool            auto_mode_frame_ready     = false;

    // Horizontal
    double    horizontal_offset               = LABC::OSC::HORIZONTAL_OFFSET;
    double    time_per_division               = LABC::OSC::TIME_PER_DIVISION;
    double    time_per_division_last_repeated = time_per_division;
    double    time_per_division_raw_buffer    = time_per_division;
    unsigned  samples                         = LABC::OSC::NUMBER_OF_SAMPLES;
    unsigned  samples_raw_buffer              = samples;
    double    sampling_rate                   = LABC::OSC::SAMPLING_RATE;

    // Data/Samples/Pixels
    std::vector<uint32_t> recording_raw_sample_buffer;

    std::array<
      uint32_t, 
      LABC::OSC::NUMBER_OF_SAMPLES
    > raw_data_buffer;              

    std::array<
      LAB_Channel_Data_Oscilloscope, 
      LABC::OSC::NUMBER_OF_CHANNELS
    > channel_data;

    double                w_samp_count            = LABC::OSC::NUMBER_OF_SAMPLES;
  
    // Trigger 
    bool                  find_trigger            = false; 
    bool                  trigger_frame_ready     = false;
    bool                  trigger_found           = false;
    bool                  find_trigger_timeout    = false;
    LABE::OSC::TRIG::MODE trigger_mode            = LABC::OSC::TRIGGER_MODE;
    unsigned              trig_source             = LABC::OSC::TRIGGER_SOURCE;
    LABE::OSC::TRIG::TYPE trig_type               = LABC::OSC::TRIGGER_TYPE;
    LABE::OSC::TRIG::CND  trig_condition          = LABC::OSC::TRIGGER_CONDITION;
    double                trigger_level           = LABC::OSC::TRIGGER_LEVEL;
    double                trigger_level_raw_bits  = (LABC::OSC::ADC_RESOLUTION_INT - 1) / 2;
    unsigned              find_trig_sample_skip   = 10;
    unsigned              trig_buff_index         = 0;
    unsigned              trig_index              = 0;
    
    // Debug
    unsigned              buffer_overflow_count   = 0;

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


  public:
    bool has_enabled_channels ()
    {
      for (const auto& e : channel_data)
      {
        if (e.is_enabled)
        {
          return (true);
        }
      }

      return (false);
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

struct LAB_Channel_Data_Logic_Analyzer
{
  bool      is_enabled                      = true;
  unsigned  raw_sample_buffer_working_size  = 0;

  // Data/Samples
  std::vector <std::array<int, 2>> pixel_points;
  std::array  <bool, LABC::LOGAN::NUMBER_OF_SAMPLES> samples;
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

    // State
    bool is_backend_running      = false;
    bool is_frontend_running  = false;

    // Mode
    LABE::LOGAN::MODE mode = LABD::LOGAN::MODE;

    // Horizontal
    double    horizontal_offset             = LABD::LOGAN::HORIZONTAL_OFFSET;
    double    time_per_division             = LABD::LOGAN::TIME_PER_DIVISION;
    double    time_per_division_raw_buffer  = time_per_division;
    unsigned  samples                       = LABD::LOGAN::SAMPLES;
    unsigned  samples_raw_buffer            = samples;
    double    sampling_rate                 = LABD::LOGAN::SAMPLING_RATE;

    std::array <
      uint32_t, 
      LABC::LOGAN::NUMBER_OF_SAMPLES
    > raw_data_buffer;
      
    std::array <LAB_Channel_Data_Logic_Analyzer, 
      LABC::LOGAN::NUMBER_OF_CHANNELS> channel_data;
    
    // Trigger 
    bool                    find_trigger        = false;
    bool                    trigger_frame_ready = false;
    LABE::LOGAN::TRIG::MODE trigger_mode        = LABC::LOGAN::TRIGGER_MODE;
};

struct LAB_DMA_Data_Logic_Analyzer
{
  AP::DMA::CTL_BLK cbs[15];
  
  uint32_t  buffer_ok_flag = 0x1;

  volatile  uint32_t status [LABC::LOGAN::NUMBER_OF_CHANNELS];
  volatile  uint32_t rxd    [LABC::LOGAN::NUMBER_OF_CHANNELS]
                            [LABC::LOGAN::NUMBER_OF_SAMPLES];
};

#endif