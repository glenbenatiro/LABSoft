#ifndef LABSOFT_CONTROLLER_VOLTMETER_H
#define LABSOFT_CONTROLLER_VOLTMETER_H

#include <FL/Fl_Light_Button.H>

#include "LABSoft_Controller_Unit.h"

class LABSoft_Controller_Voltmeter : public LABSoft_Controller_Unit
{
  private:
    void init_gui_values ();
    
  public:
    LABSoft_Controller_Voltmeter (LABSoft_Controller& _LABSoft_Controller);

    void cb_run_stop          (Fl_Light_Button *w, void *data);
    void display_update_cycle ();
};

#endif