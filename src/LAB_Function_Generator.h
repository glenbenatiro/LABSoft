#ifndef LAB_FUNCTION_GENERATOR
#define LAB_FUNCTION_GENERATOR

#include "LAB_Core.h"
#include "Channel_Signal.h"
#include "Defaults.h"
#include "LAB_AD9833.h"

class LAB_Function_Generator 
{
  public:
    LAB_Core       *m_LAB_Core;
    Channel_Signal  m_channel_signal[LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS];

    // API for the ADC: AD9833
    LAB_AD9833 m_func_gen_ic;

  public:
    // functions
         LAB_Function_Generator (LAB_Core *_LAB_Core);

    void start  (int channel);
    void stop   (int channel);
  
    // setters
    void wave_type  (int channel, WaveType _WaveType);
    void frequency  (int channel, double frequency);
    void period     (int channel, double period);
    void amplitude  (int channel, double amplitude);
    void offset     (int channel, double offset);
    void duty_cycle (int channel, double duty_cycle);
    void phase      (int channel, double phase);
};

#endif
