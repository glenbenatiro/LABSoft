#include "LAB_Voltmeter.h"
#include "LAB.h"

#include <iostream>

LAB_Voltmeter::LAB_Voltmeter (LAB_Core *_LAB_Core, LAB *_LAB)
{
  m_LAB = _LAB;
  m_LAB_Core = _LAB_Core;
}

void LAB_Voltmeter:: 
run ()
{
  LAB_Oscilloscope *osc = &(m_LAB->m_Oscilloscope);

  if (osc->is_osc_front_running ())
  {
    osc->stop_osc_front ();
  }

  if (!(osc->is_osc_core_running ()))
  {
    osc->run_osc_core ();
  }

  m_is_running = true; 
}

void LAB_Voltmeter:: 
stop ()
{
  m_LAB->m_Oscilloscope.stop_osc_core ();

  m_is_running = false;
}

double LAB_Voltmeter:: 
get_data_sample (unsigned channel)
{
  Channel_Signal_Oscilloscope *osc = &(m_LAB->m_Oscilloscope.
    channel_signals ()->m_chans[channel].osc);
  
  double sample = osc->voltage_samples[13]; 
  double scaler = std::pow (10, static_cast<int>(osc->voltmeter_unit));
  double scaled_samp = sample * scaler;

  return (sample * scaler);
}

void LAB_Voltmeter:: 
unit (unsigned       channel, 
      LE_UNIT_PREFIX_EXP _LE_UNIT_PREFIX_EXP)
{
  m_LAB->m_Oscilloscope.channel_signals ()->m_chans[channel].osc.
    voltmeter_unit = _LE_UNIT_PREFIX_EXP;
}