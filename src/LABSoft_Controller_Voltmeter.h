#ifndef LABSOFT_CONTROLLER_VOLTMETER_H
#define LABSOFT_CONTROLLER_VOLTMETER_H

#include <FL/Fl_Light_Button.H>

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Voltmeter 
{
  private:
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;
    
  public:
    LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    void cb_run_stop (Fl_Light_Button *w, void *data);
    void display_update_cycle ();
};

#endif