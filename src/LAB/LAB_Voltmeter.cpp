#include "LAB_Voltmeter.h"

#include "LAB.h"

LAB_Voltmeter:: 
LAB_Voltmeter (LAB& _LAB)
  : LAB_Module (_LAB)
{

}

bool LAB_Voltmeter:: 
is_running ()
{
  return (m_is_running);
}

void LAB_Voltmeter:: 
run ()
{
  LAB_Oscilloscope& osc = lab ().m_Oscilloscope;

  if (osc.is_frontend_running ())
  {
    osc.osc_frontend_run_stop (false);
  }

  if (!osc.is_backend_running ())
  {
    osc.osc_core_run_stop (true);
  }

  lab ().m_Oscilloscope.samples       (LABC::OSC::MAX_SAMPLES);
  lab ().m_Oscilloscope.sampling_rate (LABC::VOLTMETER::SAMPLING_RATE);

  for (int a = 0; a < LABC::VOLTMETER::NUMBER_OF_CHANNELS; a++)
  {
    lab ().m_Oscilloscope.coupling (a, LABE::OSC::COUPLING::DC);
  }
  
  m_is_running = true;
}

void LAB_Voltmeter:: 
stop ()
{
  m_LAB.m_Oscilloscope.osc_core_run_stop (false);

  m_is_running = false;
}

double LAB_Voltmeter:: 
read_voltage (unsigned chan)
{
  lab ().m_Oscilloscope.load_data_samples ();
  
  return (lab ().m_Oscilloscope.parent_data ().channel_data[chan].samples[0]);
}