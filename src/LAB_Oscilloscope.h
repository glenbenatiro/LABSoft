#ifndef LAB_OSCILLOSCOPE_H
#define LAB_OSCILLOSCOPE_H

#include "LAB_Core.h"
#include "Channel_Signals.h"

// forward declare LAB, include LAB.h in cpp file
class LAB;

class LAB_Oscilloscope 
{
  private:
    LAB      *m_LAB;
    LAB_Core *m_LAB_Core;
    MemoryMap m_uncached_adc_dma_data;

  public:
    
    double usec_start = 0;

    bool m_is_master_running = false,
         m_is_osc_core_running = false;

    int   m_number_of_channels            = LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS,
          m_number_of_samples_per_channel = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES,
          m_sample_rate                   = LAB_OSCILLOSCOPE_SAMPLING_RATE,
          m_curr_screen_buffer = 0;

    uint32_t  m_pwm_range,
              m_usec_start;

    Channel_Signals m_channel_signals;

    // --- functions --- 
    LAB_Oscilloscope  (LAB_Core *_LAB_Core, LAB *_LAB);
   ~LAB_Oscilloscope  ();

    void    run ();
    void    stop ();   
    double  time_per_division (double value, unsigned osc_disp_num_cols);
    void    sampling_rate (double value);
    void    buffer_switch (int buffer);
    void    volts_per_division (unsigned channel, double value);
    void    vertical_offset (unsigned channel, double value);
    void    load_data_samples ();
    void    update_dma_data (int osc_disp_mode);
    void    run_osc_core ();
    void    stop_osc_core ();

    //

    bool is_master_running ()
    {
      return m_is_master_running;
    }

    void channel_enable (unsigned channel)
    {
      m_channel_signals.m_chans[channel].m_is_enabled = true;
    }

    void channel_disable (unsigned channel)
    {
      m_channel_signals.m_chans[channel].m_is_enabled = false;
    }

    void display_mode (unsigned channel, int value)
    {
      //m_channel_signals.m_chans[channel].osc.
      m_channel_signals.m_chans[channel].osc.osc_disp_mode = value;
    }
};

#endif
