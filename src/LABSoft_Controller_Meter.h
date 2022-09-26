#ifndef LABSOFT_CONTROLLER_METER_H
#define LABSOFT_CONTROLLER_METER_H

#include <cstdlib>
#include <thread>

#include <FL/Fl_Output.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>

#include "LAB.h"
#include "LAB_Meter.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Meter
{
  public:
    // variables
    LAB           *m_LAB;
    LABSoft_GUI   *m_LABSoft_GUI;
    LAB_Meter     *m_LAB_Meter;
    Fl_Output     *m_Fl_Output;

    std::thread *thread1, 
                *thread2;

    // functions
    LABSoft_Controller_Meter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI, LAB_Meter *_LAB_Meter, Fl_Output *_Fl_Output);

    // callback functions
    void LABSoft_Controller_Meter_cb_fl_choice_unit                          (Fl_Choice       *w, void *data);
    void LABSoft_Controller_Meter_cb_fl_light_button_run_stop                (Fl_Light_Button *w, void *data);
    void LABSoft_Controller_Meter_cb_fl_light_button_generate_random_values  (Fl_Light_Button *w, void *data);
    void LABSoft_Controller_Meter_update_meter_value ();
};

#endif