#include "LABSoft_Controller_Voltmeter.h"

LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Voltmeter:: 
cb_run_stop  (Fl_Light_Button* w, 
              void*            data)
{
  if (w->value () == 0)
  {
    m_LAB->m_Voltmeter.stop ();
  }
  else 
  {
    m_LAB->m_Voltmeter.run ();

    // 
    m_LABSoft_GUI->oscilloscope_fl_light_button_run_stop->clear ();
  }
}

// EOF