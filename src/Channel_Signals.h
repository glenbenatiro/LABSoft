#ifndef CHANNEL_SIGNAL_H
#define CHANNEL_SIGNAL_H

#include <vector>

#include "LAB_Globals.h"
#include "Defaults.h"

class Channel_Signal
{
  private:
    // --- variables ---
    // general
    bool  m_is_enabled = true;

    // for oscilloscope display
    int   m_number_of_samples_to_use;
    float m_sample_rate;   

    std::vector<float>            m_samples;
    std::vector<std::vector<int>> m_pixel_points;

    // for function generator
    float     m_function_frequency,
              m_function_amplitude,
              m_function_y_offset,
              m_function_duty_cycle,
              m_function_phase;

    WaveType  m_function_wave_type;
              
  public:
    // --- functions ---
          Channel_Signal (int number_of_samples);

    // getters
    bool  is_enabled ();
    int   number_of_samples_to_use ();
    float sample_rate ();
    float function_amplitude ();
    float function_frequency ();
    float function_phase ();
    float function_y_offset ();
    float function_duty_cycle ();

    WaveType                        function_wave_type ();
    std::vector<float>*             samples ();
    std::vector<std::vector<int>>*  pixel_points ();    

    // setters
    void  is_enabled          (bool value);
    void  function_wave_type  (WaveType wave_type);
    void  function_amplitude  (float value);
    void  function_frequency  (float value);
    void  function_phase      (float value);
    void  function_y_offset   (float value);
    void  function_duty_cycle (float value);
};



// ---



class Channel_Signals
{
  private:
    int   m_number_of_channels,
          m_max_number_of_samples;

    bool  m_is_sample_sharing_enabled;

    std::vector<Channel_Signal> m_channel_signal_vector;

  public:
    // functions
    Channel_Signals (int  number_of_channels = 1, 
                     int  max_number_of_samples = 0, 
                     bool is_sample_sharing_enabled = 0);
    
    // getters
    int   number_of_channels ();
    int   max_number_of_samples ();
    bool  is_sample_sharing_enabled ();

    Channel_Signal* channel_signal_vector (int channel);
    
    // setters
    void  number_of_channels (int value);
    void  max_number_of_samples (int value);
    // void  is_sample_sharing_enabled (bool value);
};

#endif