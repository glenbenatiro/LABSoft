#include "LAB_Function_Generator.h"

#include <iostream>

LAB_Function_Generator::
LAB_Function_Generator (LAB_Core *_LAB_Core) :
  m_LAB_Core (_LAB_Core),
  m_func_gen_ic (_LAB_Core)
{
  m_LAB_Core = _LAB_Core;
  init ();
}

void LAB_Function_Generator:: 
start (int channel)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    m_channel_signal[channel].enable ();
  }
}

void LAB_Function_Generator:: 
stop (int channel)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    m_channel_signal[channel].disable ();
  }
}

void LAB_Function_Generator:: 
init ()
{
  
}

void LAB_Function_Generator::
wave_type  (int channel, WaveType _WaveType)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    if (m_channel_signal[channel].is_enabled ())
    {
      // add more code here regarding channel selection in the future if
      // Lab in a Box will have more than 1 function generator channel

      m_func_gen_ic.wave_type (_WaveType);
    }
  }
}

void LAB_Function_Generator::
frequency (int    channel, 
           double frequency)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    if (m_channel_signal[channel].is_enabled ())
    {
      // add more code here regarding channel selection in the future if
      // Lab in a Box will have more than 1 function generator channel

      m_func_gen_ic.frequency (frequency);
    }
  }
}

void LAB_Function_Generator::
period (int channel, double period)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    if (m_channel_signal[channel].is_enabled ())
    {
      // add more code here regarding channel selection in the future if
      // Lab in a Box will have more than 1 function generator channel

      double frequency = (1.0 / period);

      m_func_gen_ic.frequency (frequency);
    }
  }
}

void LAB_Function_Generator::
amplitude  (int    channel,
            double amplitude)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    if (m_channel_signal[channel].is_enabled ())
    {
      // add more code here regarding channel selection in the future if
      // Lab in a Box will have more than 1 function generator channel

      // add code here for amplitude setting
    }
  }
}

void LAB_Function_Generator::
offset  (int    channel,
         double offset)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    if (m_channel_signal[channel].is_enabled ())
    {
      // add more code here regarding channel selection in the future if
      // Lab in a Box will have more than 1 function generator channel

      // add code here for offset setting
    }
  }
}

void LAB_Function_Generator::
duty_cycle (int    channel, 
            double duty_cycle)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    if (m_channel_signal[channel].is_enabled ())
    {
      // add more code here regarding channel selection in the future if
      // Lab in a Box will have more than 1 function generator channel

      m_func_gen_ic.duty_cycle (duty_cycle);
    }
  }
}

void LAB_Function_Generator::
phase (int    channel, 
       double phase)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    if (m_channel_signal[channel].is_enabled ())
    {
      // add more code here regarding channel selection in the future if
      // Lab in a Box will have more than 1 function generator channel

      m_func_gen_ic.phase (phase);
    }
  }
}

// EOF