#ifndef LABSOFT_CONTROLLER_MULTIMETER_H
#define LABSOFT_CONTROLLER_MULTIMETER_H

#include <string>

#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Input_Choice.H>

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Function_Generator
{
  public:
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

    LABSoft_Controller_Function_Generator (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // functions

    // callback functions
    void LABSoft_Controller_Function_Generator_cb_fl_light_button_start_stop          (Fl_Light_Button *w, void *data);
    void LABSoft_Controller_Function_Generator_cb_fl_input_choice_horizontal_position (Fl_Input_Choice *w, void *data);
    void LABSoft_Controller_Function_Generator_cb_fl_input_choice_time_per_division   (Fl_Input_Choice *w, void *data);

    void LABSoft_Controller_Function_Generator_cb_fl_input_choice_wave_type (Fl_Input_Choice *w, void *data);
    void LABSoft_Controller_Function_Generator_cb_fl_input_choice_frequency (Fl_Input_Choice *w, void *data);
    void LABSoft_Controller_Function_Generator_cb_fl_input_choice_period (Fl_Input_Choice *w, void *data);
    void LABSoft_Controller_Function_Generator_cb_fl_input_choice_amplitude (Fl_Input_Choice *w, void *data);
    void LABSoft_Controller_Function_Generator_cb_fl_input_choice_y_offset (Fl_Input_Choice *w, void *data);
    void LABSoft_Controller_Function_Generator_cb_fl_input_choice_duty_cycle (Fl_Input_Choice *w, void *data);
    void LABSoft_Controller_Function_Generator_cb_fl_input_choice_phase (Fl_Input_Choice *w, void *data);
};

#endif