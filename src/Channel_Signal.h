#ifndef CHANNEL_SIGNAL_H
#define CHANNEL_SIGNAL_H

#include <vector>
#include <cstdint>

#include "Defaults.h"

struct multimeter
{
  unsigned adc_value;
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

#endif