#ifndef LABSOFT_CONTROLLER_OSCILLOSCOPE_H
#define LABSOFT_CONTROLLER_OSCILLOSCOPE_H

#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Button.H>

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Oscilloscope
{
  public:
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

    LABSoft_Controller_Oscilloscope (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);
    
    // callback functions
    void LABSoft_Controller_Oscilloscope_cb_fl_light_button_generate_sine_wave (Fl_Light_Button *w, void *data);
    void cb_single (Fl_Button *w, void *data);
    void cb_channel_enable_disable (Fl_Light_Button *w, void *data);
    
    void cb_x_offset (Fl_Input_Choice *w, void *data);
    void cb_volts_per_division (Fl_Input_Choice *w, void *data);

    void cb_time_per_division (Fl_Input_Choice *w, void *data);


};

#endif