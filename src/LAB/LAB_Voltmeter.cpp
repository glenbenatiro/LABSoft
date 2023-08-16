#include "LAB_Voltmeter.h"

#include "LAB.h"

LAB_Voltmeter:: 
LAB_Voltmeter (LAB_Core*  _LAB_Core,
               LAB*       _LAB)
  : m_LAB_Core (_LAB_Core), m_LAB (_LAB)
{

}

void LAB_Voltmeter:: 
run ()
{
  LAB_Oscilloscope& osc = m_LAB->m_Oscilloscope; 

  if (osc.is_frontend_running ())
  {
    osc.osc_frontend_run_stop (false);
  }

  if (!osc.is_backend_running ())
  {
    osc.osc_core_run_stop (true);
  }

  m_LAB_Core->pwm.frequency (
    LABC::PWM::DMA_PACING_CHAN,
    LABC::VOLTMETER::SAMPLING_RATE
  );

  m_is_running = true;
}

void LAB_Voltmeter:: 
stop ()
{
  m_LAB->m_Oscilloscope.osc_core_run_stop (false);

  m_is_running = false;
}

void LAB_Voltmeter:: 
load_data_samples ()
{
  m_LAB->m_Oscilloscope.load_data_samples ();

  for (int chan = 0; chan < m_samples.size (); chan++)
  {
    m_samples[chan] = m_LAB->m_Oscilloscope.m_parent_data.channel_data[chan].samples[0];
  }
}

bool LAB_Voltmeter:: 
is_running ()
{
  return (m_is_running);
}

// EOF
