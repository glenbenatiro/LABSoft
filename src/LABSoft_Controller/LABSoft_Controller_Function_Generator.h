#ifndef LABSOFT_CONTROLLER_FUNCTION_GENERATOR_H
#define LABSOFT_CONTROLLER_FUNCTION_GENERATOR_H

#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Input_Choice.H>

#include "../LAB/LAB.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"

// forward declare LABSoft_Controller, #include in .cpp file
class LABSoft_Controller;

class LABSoft_Controller_Function_Generator
{
  private:
    LAB*                m_LAB;
    LABSoft_GUI*        m_LABSoft_GUI;
    LABSoft_Controller* m_LABSoft_Controller;
  
  private:
    void init_gui_values (); 

  public:
    LABSoft_Controller_Function_Generator (LAB*                 _LAB, 
                                           LABSoft_GUI*         _LABSoft_GUI,
                                           LABSoft_Controller*  _LABSoft_Controller);
    void update_gui_frequency_elements    ();

    void cb_run_stop        (Fl_Light_Button* w, long channel);
    void cb_wave_type       (Fl_Choice*       w, long channel);
    void cb_amplitude       (Fl_Input_Choice* w, long channel);
    void cb_frequency       (Fl_Input_Choice* w, long channel);
    void cb_period          (Fl_Input_Choice* w, long channel);
    void cb_vertical_offset (Fl_Input_Choice* w, long channel);
    void cb_phase           (Fl_Input_Choice* w, long channel);
};

#endif
