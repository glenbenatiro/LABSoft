#ifndef CHANNEL_SIGNAL_H
#define CHANNEL_SIGNAL_H

#include <vector>

#include "Defaults.h"

class Channel_Signal
{
  public:
    // --- variables ---
    bool    m_is_enabled = false;

    int     m_volts_per_division_unit_scaler = 
              CHANNEL_SIGNAL_VOLTS_PER_DIVISION_UNIT_SCALER;

    double  m_sample_rate         = CHANNEL_SIGNAL_SAMPLE_RATE,
            m_volts_per_division  = CHANNEL_SIGNAL_VOLTS_PER_DIVISION,
            m_y_offset            = CHANNEL_SIGNAL_Y_OFFSET;

    uint32_t m_raw_values[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES_PER_CHANNEL];
    std::vector<double>           m_values;
    std::vector<std::vector<int>> m_pixel_points;
    
    // for function generator
    double  m_function_amplitude    = CHANNEL_SIGNAL_FUNCTION_AMPLITUDE,
            m_function_frequency    = CHANNEL_SIGNAL_FUNCTION_FREQUENCY,
            m_function_phase        = CHANNEL_SIGNAL_FUNCTION_PHASE, 
            m_function_y_offset     = CHANNEL_SIGNAL_FUNCTION_Y_OFFSET, 
            m_function_duty_cycle   = CHANNEL_SIGNAL_FUNCTION_DUTY_CYCLE;

    WaveType  m_function_wave_type  = CHANNEL_SIGNAL_FUNCTION_WAVE_TYPE;

    // --- functions ---
    Channel_Signal (int number_of_samples = 0, 
                    int display_width = 0);
};

#endif