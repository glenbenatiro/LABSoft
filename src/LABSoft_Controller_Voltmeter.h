#ifndef LABSOFT_CONTROLLER_VOLTMETER_H
#define LABSOFT_CONTROLLER_VOLTMETER_H

#include <thread>
#include <cstdio>

#include <FL/Fl.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Choice.H>

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Voltmeter
{
  public:
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

    std::thread *thread1, *thread2;

    //
    LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // cb
    void LABSoft_Controller_Voltmeter_cb_fl_light_button_generate_random_values (Fl_Light_Button *w, void *data);
    void LABSoft_Controller_Voltmeter_cb_fl_light_button_start_stop (Fl_Light_Button *w, void *data);
    void LABSoft_Controller_Voltmeter_cb_fl_choice_unit (Fl_Choice *w, void *data);
    void LABSoft_Controller_Voltmeter_update_voltmeter_value ();
};

#endif