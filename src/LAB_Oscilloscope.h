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
    AP_MemoryMap m_uncached_adc_dma_data;

  public:
    
    double usec_start = 0;

    bool  m_is_osc_front_running = false,
          m_is_osc_core_running  = false;

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
    void    switch_dma_buffer (int buffer);
    void    volts_per_division (unsigned channel, double value);
    void    vertical_offset (unsigned channel, double value);
    void    load_data_samples ();
    void    update_dma_data (int graph_disp_mode);
    void    channel_enable ();
    double  time_per_division ();
    void    config_dma_control_blocks ();
    int     coupling  (unsigned channel, LABE_OSC_COUPLING _LABE_OSC_COUPLING);
    int     scaling   (unsigned channel, LE_OSC_SCALING _LE_OSC_SCALING);

    // --- Inline Functions ---

    void run_master ()
    {
      run_osc_core ();
      run_osc_front ();
    }

    void stop_master ()
    {
      stop_osc_core ();
      stop_osc_front ();
    }

    void run_osc_front ()
    {
      m_is_osc_front_running = true;
    }

    void stop_osc_front ()
    {
      m_is_osc_front_running = false;
    }

    void run_osc_core ()
    {
      m_LAB_Core->pwm_start (LAB_PWM_DMA_PACING_PWM_CHAN);
      m_is_osc_core_running = true;
    }

    void stop_osc_core ()
    {
      m_LAB_Core->pwm_stop (LAB_PWM_DMA_PACING_PWM_CHAN);
      m_is_osc_core_running = false;
    }   

    bool is_running ()
    {
      if (m_is_osc_front_running && m_is_osc_core_running)
      {
        return (true);
      }
      else 
      {
        return (false);
      }
    }

    bool is_osc_core_running ()
    {
      return (m_is_osc_core_running);
    }

    bool is_osc_front_running ()
    {
      return (m_is_osc_front_running);
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
      m_channel_signals.m_chans[channel].osc.graph_disp_mode = value;
    }

    Channel_Signals* channel_signals ()
    {
      return (&m_channel_signals);
    }
};

#endif
