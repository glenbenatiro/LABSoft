#include "LABSoft_Controller_Voltmeter.h"

#include <chrono>

LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

// 
void LABSoft_Controller_Voltmeter:: 
update ()
{

}

// callback functions
void LABSoft_Controller_Voltmeter:: 
cb_unit      (Fl_Choice *w, void *data)
{

}

void LABSoft_Controller_Voltmeter:: 
cb_mode      (Fl_Choice *w, void *data)
{

}

void LABSoft_Controller_Voltmeter:: 
cb_run_stop  (Fl_Light_Button *w, void *data)
{

}

void LABSoft_Controller_Voltmeter:: 
cb_generate_random_values (Fl_Light_Button *w, void *data)
{

}
