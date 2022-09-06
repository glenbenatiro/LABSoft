#ifndef LAB_FUNCTION_GENERATOR
#define LAB_FUNCTION_GENERATOR

#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Light_Button.H>

#include "LAB_Core.h"
#include "LABSoft_Function_Generator_Display.h"

class LAB_Function_Generator : public LAB_Core
{
  private:
    float m_frequency = 1.0;
    float m_amplitude = 1.0;
    WaveType wave_type = SINE;
    bool m_flag_global_enable = false;
  
  public:
    
    LAB_Function_Generator ();

    // setters
    void LAB_Function_Generator_set_wave_type (WaveType wave_type);
    void LAB_Function_Generator_set_amplitude (float amplitude);   
    void LABSoft_Function_Generator_set_period_or_frequency (Fl_Input_Choice *w, void *data);
                                                                  
    // callback functions
    void LAB_Function_Generator_cb_function_generator_fl_light_button_start_stop (Fl_Input_Choice *w, void *data);
    void LAB_Function_Generator_cb_function_generator_fl_input_choice_wave_type (Fl_Input_Choice *w, void *data);
    void LAB_Function_Generator_cb_function_generator_fl_input_choice_frequency (Fl_Input_Choice *w, void *data);

    void LAB_Function_Generator_cb_function_generator_fl_input_choice_time_per_division (Fl_Input_Choice *w, void *data); 
    void LAB_Function_Generator_cb_function_generator_fl_light_button_start_stop (Fl_Light_Button *w, void *data);
    void LAB_Function_Generator_cb_function_generator_fl_input_choice_amplitude (Fl_Light_Button *w, void *data);
    void LABSoft_Function_Generator_cb_function_generator_fl_input_choice_amplitude (Fl_Input_Choice *w, void *data);
    void LABSoft_Function_Generator_cb_function_generator_fl_input_choice_period (Fl_Input_Choice *w, void *data);    
};

#endif
