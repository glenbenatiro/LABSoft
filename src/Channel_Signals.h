#ifndef CHANNEL_SIGNALS_H
#define CHANNEL_SIGNALS_H

#include <vector>
#include <cstdint>

#include "Defaults.h"

// Number of pixels	7680×4320
// True 8K pixel width = 7690

constexpr int MAX_DISPLAY_WIDTH = 7680;


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
  volatile uint32_t m_raw_data[LAB_LOGIC_ANALYZER_MAX_NUMBER_OF_SAMPLES];

  // this contains the converted data from m_raw_data
  bool m_converted_data[LAB_LOGIC_ANALYZER_MAX_NUMBER_OF_SAMPLES];
};

struct Channel_Signal_Oscilloscope 
{
  double sampling_rate;
  double time_per_division;

  std::vector<std::vector<int>> pixel_points;
  std::vector<double>         voltage_samples;
};

class Channel_Signal
{
  public:
    // --- variables ---
    bool m_is_enabled = false;
    
    Channel_Signal_Oscilloscope osc;

    unsigned m_working_samples;

    int     m_voltage_per_division_unit_scaler = 
              CHANNEL_SIGNAL_VOLTAGE_PER_DIVISION_UNIT_SCALER;

    double  m_sample_rate           = CHANNEL_SIGNAL_SAMPLE_RATE,
            m_voltage_per_division  = CHANNEL_SIGNAL_VOLTAGE_PER_DIVISION,
            m_time_per_division     = CHANNEL_SIGNAL_TIME_PER_DIVISION,
            m_vertical_offset       = CHANNEL_SIGNAL_VERTICAL_OFFSET,
            m_horizontal_offset     = CHANNEL_SIGNAL_HORIZONTAL_OFFSET;

    
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
    Channel_Signal (unsigned number_of_samples = 0, unsigned display_width = MAX_DISPLAY_WIDTH)
    {
      // oscilloscope 
      osc.voltage_samples.resize (number_of_samples);
      osc.pixel_points.resize (display_width, std::vector<int>(2));

      //
    }
    
    // setter
    void enable ();
    void disable ();
    void volts_per_division (double value);
    void vertical_offset (double value);

    // getter
    bool is_enabled ()
    {
      return m_is_enabled;
    }

    double voltage_per_division ();
    double vertical_offset ();
    double time_per_division ();
    double horizontal_offset ();
    unsigned working_samples ();
};


// --- Channel_Signals ---
class Channel_Signals
{
  private:


  public:
    unsigned m_working_samples = 0;

    std::vector<Channel_Signal> m_chans;

    Channel_Signals (unsigned number_of_channels  = 1,
                     unsigned number_of_samples   = 0,
                     unsigned display_width       = 0);

   ~Channel_Signals ();
    
    int size ()
    {
      return m_chans.size ();
    }
};

#endif