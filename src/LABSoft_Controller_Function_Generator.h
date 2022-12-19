#ifndef LABSOFT_CONTROLLER_MULTIMETER_H
#define LABSOFT_CONTROLLER_MULTIMETER_H

#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Input_Choice.H>

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Function_Generator
{
  public:
    // variables
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

    // functions
          LABSoft_Controller_Function_Generator (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // callback functions
    void  cb_run_stop    (Fl_Light_Button *w, long data);
    void  cb_wave_type   (Fl_Input_Choice *w, long data);
    void  cb_frequency   (Fl_Input_Choice *w, long data);
    void  cb_period      (Fl_Input_Choice *w, long data);
    void  cb_amplitude   (Fl_Input_Choice *w, long data);
    void  cb_offset      (Fl_Input_Choice *w, long data);
    void  cb_duty_cycle  (Fl_Input_Choice *w, long data);
    void  cb_phase       (Fl_Input_Choice *w, long data);
};

#endif