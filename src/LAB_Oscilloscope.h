#ifndef LAB_OSCILLOSCOPE_H
#define LAB_OSCILLOSCOPE_H

#include "LAB_Core.h"
#include "Channel_Signals.h"

class LAB_Oscilloscope 
{
  public:
    LAB_Core *m_LAB_Core;

    bool  m_is_oscilloscope_running = false;

    int   m_number_of_channels  = LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS,
          m_sample_count        = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES_PER_CHANNEL,
          m_sample_rate         = LAB_OSCILLOSCOPE_SAMPLE_RATE;

    uint32_t  m_pwm_range,
              m_usec_start;

    Channel_Signals m_channel_signals;

    // --- functions --- 
    LAB_Oscilloscope  (LAB_Core *_LAB_Core);
   ~LAB_Oscilloscope  ();

    void  run ();
    void  stop ();    
    int   stream_values ();
};

#endif
