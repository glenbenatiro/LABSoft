#include "LABSoft_Controller_Voltmeter.h"

LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Voltmeter:: 
cb_unit (Fl_Choice *w, 
         long       data)
{
  unsigned channel = static_cast<unsigned>(data);

  switch (w->value ())
  {
    case 0:
      m_LAB->m_Voltmeter.unit (channel, LE_UNIT_PREFIX_EXP_NONE);
      break;
    case 1:
      m_LAB->m_Voltmeter.unit (channel, LE_UNIT_PREFIX_EXP_MILLI);
      break;
    default:
      cb_unit (w, 0);
      break;
  }
}

void LABSoft_Controller_Voltmeter:: 
cb_run_stop  (Fl_Button *w, 
              void      *data)
{
  if (m_LAB->m_Voltmeter.is_running ())
  {
    m_LAB->m_Voltmeter.stop ();

    w->label ("Run");
  }
  else 
  {
    m_LAB->m_Voltmeter.run ();

    w->label ("Stop");
    m_LABSoft_GUI->oscilloscope_fl_button_run_stop->label ("Run");
  }
}