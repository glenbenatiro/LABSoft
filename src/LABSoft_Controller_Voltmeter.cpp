#include "LABSoft_Controller_Voltmeter.h"

LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Voltmeter:: 
cb_unit      (Fl_Choice *w, void *data)
{

}

void LABSoft_Controller_Voltmeter:: 
cb_run_stop  (Fl_Light_Button *w, void *data)
{
  if (w->value () == 0)
  {
    m_LAB->m_Voltmeter->stop ();

    w->label ("Run");
  }
  else 
  {
    m_LAB->m_Voltmeter->run ();

    w->label ("Stop");
  }
}