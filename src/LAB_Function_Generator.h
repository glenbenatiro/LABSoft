#ifndef LAB_FUNCTION_GENERATOR
#define LAB_FUNCTION_GENERATOR

#include "LAB_Core.h"
#include "Channel_Signal.h"
#include "Defaults.h"

class LAB_Function_Generator 
{
  public:
    bool m_is_running = false;
    LAB_Core *m_LAB_Core;
    Channel_Signal m_channel_signal[LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS];

  public:
    // --- functions ---
          LAB_Function_Generator (LAB_Core *_LAB_Core);
    void  update ();

    // setters
    void wave_type  (int channel, WaveType value);
    void frequency  (int channel, double value);
    void period     (int channel, double value);
    void amplitude  (int channel, double value);
    void offfset    (int channel, double value);

    // getters
};

#endif
