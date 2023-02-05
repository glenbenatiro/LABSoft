#ifndef LAB_OSCILLOSCOPE_H
#define LAB_OSCILLOSCOPE_H

#include "LAB_Core.h"
#include "Channel_Signals.h"

class LAB_Oscilloscope 
{
  private:

    LAB_Core *m_LAB_Core;
    MemoryMap m_uncached_adc_dma_data;

  public:
    
    double usec_start = 0;

    bool m_is_running = false;

    int   m_number_of_channels            = LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS,
          m_number_of_samples_per_channel = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES,
          m_sample_rate                   = LAB_OSCILLOSCOPE_SAMPLING_RATE,
          m_curr_screen_buffer = 0;

    uint32_t  m_pwm_range,
              m_usec_start;

    Channel_Signals m_channel_signals;


    // --- functions --- 
    LAB_Oscilloscope  (LAB_Core *_LAB_Core);
   ~LAB_Oscilloscope  ();

    void  run ();
    void  stop ();   

    double time_per_division (unsigned channel, double value, unsigned osc_disp_num_cols);

    // this changes PWM speed on board!! 
    // verify no other are affected
    void sampling_rate (int channel, double value);
    void sampling_rate (double value);

    void buffer_switch (int buffer);


    void channel_enable (unsigned channel)
    {
      m_channel_signals.m_chans[channel].m_is_enabled = true;
    }

    void channel_disable (unsigned channel)
    {
      m_channel_signals.m_chans[channel].m_is_enabled = false;
    }
    
    void volts_per_division (unsigned channel, double value);
    void vertical_offset (unsigned channel, double value);

    void load_data_samples ();

  

  

  void display_mode (unsigned channel, int value)
  {
    //m_channel_signals.m_chans[channel].osc.
    m_channel_signals.m_chans[channel].osc.osc_disp_mode = value;
  }

  void update_dma_data (int osc_disp_mode);
};

#endif
