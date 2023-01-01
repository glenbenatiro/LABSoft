#ifndef CHANNEL_SIGNALS_H
#define CHANNEL_SIGNALS_H

#include <vector>
#include <cstdint>

#include "Defaults.h"

struct multimeter
{
  unsigned adc_value;
};

struct Channel_Signal_Logic_Analyzer
{
  // the current working sample size
  unsigned m_sample_size = LAB_LOGIC_ANALYZER_MEMORY_DEPTH;

  // this contains the raw data from the GPLEV0 register.
  // 32 bits for the 32 pins of GPIO bank 0
  uint32_t m_raw_data[LAB_LOGIC_ANALYZER_MAX_NUMBER_OF_SAMPLES];

  // this contains the converted data from m_raw_data
  bool m_converted_data[PI_MAX_GPIO_PINS][LAB_LOGIC_ANALYZER_MAX_NUMBER_OF_SAMPLES];
};

class Channel_Signal
{
  private:


  public:
    // --- variables ---
    bool    m_is_enabled = false;

    int     m_voltage_per_division_unit_scaler = 
              CHANNEL_SIGNAL_VOLTAGE_PER_DIVISION_UNIT_SCALER;

    double  m_sample_rate           = CHANNEL_SIGNAL_SAMPLE_RATE,
            m_voltage_per_division  = CHANNEL_SIGNAL_VOLTAGE_PER_DIVISION,
            m_time_per_division     = CHANNEL_SIGNAL_TIME_PER_DIVISION,
            m_vertical_offset       = CHANNEL_SIGNAL_VERTICAL_OFFSET,
            m_horizontal_offset     = CHANNEL_SIGNAL_HORIZONTAL_OFFSET;

    union data
    {
      multimeter m;
      Channel_Signal_Logic_Analyzer logic_analyzer;
    };
    
    // m_raw_values will hold the raw bits read from the ADC
    uint32_t m_raw_values[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES] = { 0 };

    // m_values will hold the converted raw values (raw to actual voltage reading)
    std::vector<float>           m_values;


    std::vector<std::vector<int>> m_pixel_points;
    
    // for function generator
    double  m_function_amplitude    = CHANNEL_SIGNAL_FUNCTION_AMPLITUDE,
            m_function_frequency    = CHANNEL_SIGNAL_FUNCTION_FREQUENCY,
            m_function_phase        = CHANNEL_SIGNAL_FUNCTION_PHASE, 
            m_function_y_offset     = CHANNEL_SIGNAL_FUNCTION_Y_OFFSET, 
            m_function_duty_cycle   = CHANNEL_SIGNAL_FUNCTION_DUTY_CYCLE;

    WaveType  m_function_wave_type;

    // --- functions ---
    Channel_Signal (int number_of_samples = 0, 
                    int display_width = 0);
    
    // setter
    void enable ();
    void disable ();

    // getter
    bool is_enabled ();
};



// --- Channel_Signals ---
class Channel_Signals
{
  public:
    std::vector<Channel_Signal> m_channel_signal_vector;

    Channel_Signals (int number_of_channels = 1,
                     int number_of_samples  = 0,
                     int display_width = 0);
    
    int size ();
};

#endif