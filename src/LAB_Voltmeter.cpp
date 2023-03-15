#include "LAB_Voltmeter.h"
#include "LAB.h"

#include <iostream>

LAB_Voltmeter::LAB_Voltmeter (LAB_Core *_LAB_Core, LAB *_LAB)
{
  m_LAB       = _LAB;
  m_LAB_Core  = _LAB_Core;
}

void LAB_Voltmeter:: 
run ()
{
  LAB_Oscilloscope *osc = &(m_LAB->m_Oscilloscope);

  if (osc->m_parent_data.channel_data[0].is_enabled == false)
  {
    m_osc_chan_en_flag = true;
    osc->m_parent_data.channel_data[0].is_enabled == true;
  }  

  if (osc->m_parent_data.is_osc_frontend_running)
  {
    osc->osc_frontend_run_stop (false);
  }

  if (osc->m_parent_data.is_osc_core_running == false)
  {
    osc->osc_core_run_stop (true);
  }

  m_is_running = true; 
}

void LAB_Voltmeter:: 
stop ()
{
  LAB_Oscilloscope *osc = &(m_LAB->m_Oscilloscope);

  osc->osc_core_run_stop (false);

  if (m_osc_chan_en_flag)
  {
    osc->m_parent_data.channel_data[0].is_enabled == false;
    m_osc_chan_en_flag = false;
  }

  m_is_running = false;
}

double LAB_Voltmeter:: 
get_data_sample (unsigned channel)
{
  LAB_Oscilloscope *osc = &(m_LAB->m_Oscilloscope);

  double sample = m_LAB->m_Oscilloscope.m_parent_data.channel_data[channel].samples[13];
  //double scaler = std::pow (10, static_cast<int>(osc->voltmeter_unit));
  double scaler = std::pow (10, 3);
  double scaled_samp = sample * scaler;

  return (sample * scaler);
}
