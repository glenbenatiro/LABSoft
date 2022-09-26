#include "LABSoft_Controller_Function_Generator.h"

LABSoft_Controller_Function_Generator::
LABSoft_Controller_Function_Generator (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Function_Generator::
LABSoft_Controller_Function_Generator_cb_fl_light_button_start_stop (Fl_Light_Button *w, void *data)
{
  if (w->value () == 0)
    {
      m_LAB->m_LAB_Function_Generator->m_flag_is_running = false;

      m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_flag_is_display_enabled = false;
      m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_reload_and_draw ();

      w->label ("Run");
      
    }
  else  
    {
      m_LAB->m_LAB_Function_Generator->m_flag_is_running = true;

      m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_flag_is_display_enabled = true;
      m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_reload_and_draw ();

      w->label ("Stop");
    }
}

void LABSoft_Controller_Function_Generator::
LABSoft_Controller_Function_Generator_cb_fl_input_choice_horizontal_position (Fl_Input_Choice *w, void *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->label ());

  // change display only
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_x_offset = value;
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_reload_and_draw ();
}

void LABSoft_Controller_Function_Generator::
LABSoft_Controller_Function_Generator_cb_fl_input_choice_time_per_division (Fl_Input_Choice *w, void *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->label ());

  // change display only
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_time_per_division = value;
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_reload_and_draw ();
}

void LABSoft_Controller_Function_Generator::
LABSoft_Controller_Function_Generator_cb_fl_input_choice_wave_type (Fl_Input_Choice *w, void *data)
{
  const char *label = w->value ();
  WaveType wave_type;

  if (strcmp (label, "Sine") == 0)
    wave_type = SINE;
  else if (strcmp (label, "Square") == 0)
    wave_type = SQUARE;
  else if (strcmp (label, "Triangle") == 0)
    wave_type = TRIANGLE;
  else if (strcmp (label, "DC") == 0)
    wave_type = DC;
  else // default to sine
    wave_type = SINE;

  // backend
  m_LAB->m_LAB_Function_Generator->m_wave_type = wave_type;
  
  // frontend
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_wave_type = wave_type;  
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_renerate_wave_and_draw ();
}
 

void LABSoft_Controller_Function_Generator:: 
LABSoft_Controller_Function_Generator_cb_fl_input_choice_frequency (Fl_Input_Choice *w, void *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // change in backend
  m_LAB->m_LAB_Function_Generator->m_frequency = value;

  // change in frontend
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_frequency = value;
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_renerate_wave_and_draw ();
}

void LABSoft_Controller_Function_Generator:: 
LABSoft_Controller_Function_Generator_cb_fl_input_choice_period (Fl_Input_Choice *w, void *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // change in backend
  m_LAB->m_LAB_Function_Generator->m_period = value;

  // change in frontend
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_period = value;
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_renerate_wave_and_draw ();
}

void LABSoft_Controller_Function_Generator:: 
LABSoft_Controller_Function_Generator_cb_fl_input_choice_amplitude (Fl_Input_Choice *w, void *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // change in backend
  m_LAB->m_LAB_Function_Generator->m_amplitude = value;

  // change in frontend
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_amplitude = value;
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_renerate_wave_and_draw ();
}

void LABSoft_Controller_Function_Generator:: 
LABSoft_Controller_Function_Generator_cb_fl_input_choice_y_offset (Fl_Input_Choice *w, void *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // change in backend
  m_LAB->m_LAB_Function_Generator->m_y_offset = value;

  // change in frontend
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_y_offset = value;
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_renerate_wave_and_draw ();
}

void LABSoft_Controller_Function_Generator:: 
LABSoft_Controller_Function_Generator_cb_fl_input_choice_duty_cycle (Fl_Input_Choice *w, void *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // change in backend
  m_LAB->m_LAB_Function_Generator->m_duty_cycle = value;

  // change in frontend
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_duty_cycle = value;
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_renerate_wave_and_draw ();
}

void LABSoft_Controller_Function_Generator:: 
LABSoft_Controller_Function_Generator_cb_fl_input_choice_phase (Fl_Input_Choice *w, void *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // change in backend
  m_LAB->m_LAB_Function_Generator->m_phase = value;

  // change in frontend
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->m_phase = value;
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->disp ()->LABSoft_Oscilloscope_Display_renerate_wave_and_draw ();
}
