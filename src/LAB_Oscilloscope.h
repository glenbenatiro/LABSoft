#ifndef LAB_OSCILLOSCOPE_H
#define LAB_OSCILLOSCOPE_H

#include "LAB_Core.h"
#include "Channel_Signals.h"

class LAB_Oscilloscope 
{
  private:

    MemoryMap m_uncached_dma_data;

  public:
    LAB_Core *m_LAB_Core;

    bool m_is_running = false;

    int   m_number_of_channels            = LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS,
          m_number_of_samples_per_channel = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES,
          m_sample_rate                   = LAB_OSCILLOSCOPE_SAMPLE_RATE;

    uint32_t  m_pwm_range,
              m_usec_start;

    Channel_Signals m_channel_signals;


    // --- functions --- 
    LAB_Oscilloscope  (LAB_Core *_LAB_Core);
   ~LAB_Oscilloscope  ();

    void  run ();
    void  stop ();   

    void channel_enable (unsigned channel);
    void channel_disable (unsigned channel);
    void volts_per_division (unsigned channel, double value);
    void vertical_offset (unsigned channel, double value);

    void convert_samples (unsigned channel);

    void load_data_samples ();

  // this changes PWM speed on board!! 
  // verify no other are affected
  void sample_rate (int channel, double value);
};

#endif
