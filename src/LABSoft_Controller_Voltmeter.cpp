#include "LABSoft_Controller_Voltmeter.h"

LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
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
    // m_LABSoft_GUI->oscilloscope_fl_button_run_stop->label ("Run");
  }
}