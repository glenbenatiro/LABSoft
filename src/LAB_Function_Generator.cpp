#include "LAB_Function_Generator.h"

LAB_Function_Generator::
LAB_Function_Generator ()
{

}

// setters
void LAB_Function_Generator:: 
LAB_Function_Generator_set_wave_type (WaveType wave_type)
{
  m_wave_type = wave_type;
}

void LAB_Function_Generator::
LAB_Function_Generator_set_amplitude (float amplitude)
{
  m_amplitude = amplitude;
}

// callback functions
void LAB_Function_Generator:: 
LAB_Function_Generator_cb_function_generator_fl_light_button_start_stop (Fl_Light_Button *w, 
                                                                         void            *data)
{
  LABSoft_Oscilloscope_Display_Group *grp = static_cast<LABSoft_Oscilloscope_Display_Group *>(data);

  if (w->value () == 0)
    {
      m_flag_global_enable = false;

      grp->disp ()->m_flag_global_enable = false;
      grp->redraw ();

      w->label ("Stopped");
    }
  else 
    {
      m_flag_global_enable = true;

      grp->disp ()->m_flag_global_enable = true;
      grp->disp ()->LABSoft_Oscilloscope_Display_generate_wave ();
      grp->disp ()->LABSoft_Oscilloscope_Display_reload ();
      grp->redraw ();

      w->label ("Running");
    }
}



void LAB_Function_Generator:: 
LAB_Function_Generator_cb_function_generator_fl_input_choice_time_per_division (Fl_Input_Choice *w,
                                                                                void            *data)
{
  float value = LAB_Globals_label_to_value (w->input ()->value ());

  printf ("value: %f\n", value);

  // change values in the function generator display
  LABSoft_Oscilloscope_Display_Group *grp = static_cast<LABSoft_Oscilloscope_Display_Group *>(data);
  grp->disp ()->LABSoft_Oscilloscope_Display_set_time_per_division (value);
  grp->disp ()->LABSoft_Oscilloscope_Display_generate_wave ();
  grp->disp ()->LABSoft_Oscilloscope_Display_reload ();
  grp->redraw ();
}

void LAB_Function_Generator::
LAB_Function_Generator_cb_function_generator_fl_input_choice_amplitude (Fl_Input_Choice *w, 
                                                                        void            *data)
{
  // get the numerical value and unit in the input label
  float value = LAB_Globals_label_to_value (w->input ()->value ());

  m_amplitude = value;

  LAB_Function_Generator_set_amplitude (value);

  // change values in the function generator display
  LABSoft_Oscilloscope_Display_Group *grp = static_cast<LABSoft_Oscilloscope_Display_Group *>(data);
  grp->disp ()->LABSoft_Oscilloscope_Display_set_amplitude (value);
  grp->disp ()->LABSoft_Oscilloscope_Display_generate_wave ();
  grp->disp ()->LABSoft_Oscilloscope_Display_reload ();
  grp->redraw ();
}

// setters
void LAB_Function_Generator:: 
LAB_Function_Generator_set_period_or_frequency (Fl_Input_Choice *w, 
                                                void            *data)
{
  // get the contents of the input field as a string
  const char *label= w->label ();

  // get the numerical value and unit in the input label
  float value = LAB_Globals_label_to_value (w->input ()->value ());

  printf ("frequency value: %.9f\n", value);

  m_amplitude = value;

  // change values in the function generator display
  LABSoft_Oscilloscope_Display_Group *grp = static_cast<LABSoft_Oscilloscope_Display_Group *>(data);
  grp->disp ()->LABSoft_Oscilloscope_Display_set_frequency (value);
  grp->disp ()->LABSoft_Oscilloscope_Display_generate_wave ();
  grp->disp ()->LABSoft_Oscilloscope_Display_reload ();
  grp->redraw ();
}

// callback functions
void LAB_Function_Generator:: 
LAB_Function_Generator_cb_function_generator_fl_input_choice_wave_type (Fl_Input_Choice *w, 
                                                                        void            *data)
{
  WaveType wave_type;
  const char *value = w->input ()->value ();

  if (!strcmp (value, "Sine"))
    wave_type = SINE;
  else if (!strcmp (value, "Square"))
    wave_type = SQUARE;
  else if (!strcmp (value, "Triangle"))
    wave_type = TRIANGLE;
  else if (!strcmp (value, "DC"))
    wave_type = DC;
  else
    wave_type = SINE;

  LAB_Function_Generator_set_wave_type (wave_type);

  LABSoft_Oscilloscope_Display_Group *grp = static_cast<LABSoft_Oscilloscope_Display_Group *>(data);
  grp->disp ()->LABSoft_Oscilloscope_Display_set_wave_type (wave_type);
  grp->disp ()->LABSoft_Oscilloscope_Display_generate_wave ();
  grp->disp ()->LABSoft_Oscilloscope_Display_reload ();
  grp->redraw ();
}

void LAB_Function_Generator::
LAB_Function_Generator_cb_function_generator_fl_input_choice_period (Fl_Input_Choice *w, 
                                                                     void            *data)
{
  LAB_Function_Generator_set_period_or_frequency (w, data);
}

void LAB_Function_Generator::
LAB_Function_Generator_cb_function_generator_fl_input_choice_frequency (Fl_Input_Choice *w, 
                                                                       void            *data)
{
  LAB_Function_Generator_set_period_or_frequency (w, data);
}


void LAB_Function_Generator::
LAB_Function_Generator_cb_function_generator_fl_input_choice_offset (Fl_Input_Choice *w, void *data)
{

}

void LAB_Function_Generator::
LAB_Function_Generator_cb_function_generator_fl_input_choice_volts_per_division (Fl_Input_Choice *w, 
                                                                                 void            *data)
{
  // get the numerical value and unit in the input label
  float value = LAB_Globals_label_to_value (w->input ()->value ());

  // change values in the function generator display
  LABSoft_Oscilloscope_Display_Group *grp = static_cast<LABSoft_Oscilloscope_Display_Group *>(data);
  grp->disp ()->LABSoft_Oscilloscope_Display_set_volts_per_division (value);
  grp->disp ()->LABSoft_Oscilloscope_Display_generate_wave ();
  grp->disp ()->LABSoft_Oscilloscope_Display_reload ();
  grp->redraw ();
}

void LAB_Function_Generator::
LAB_Function_Generator_cb_function_generator_fl_input_choice_phase (Fl_Input_Choice *w, 
                                                                                 void            *data)
{
  // get the numerical value and unit in the input label
  float value = LAB_Globals_label_to_value (w->input ()->value ());

  // change values in the function generator display
  LABSoft_Oscilloscope_Display_Group *grp = static_cast<LABSoft_Oscilloscope_Display_Group *>(data);
  grp->disp ()->LABSoft_Oscilloscope_Display_set_phase (value);
  grp->disp ()->LABSoft_Oscilloscope_Display_generate_wave ();
  grp->disp ()->LABSoft_Oscilloscope_Display_reload ();
  grp->redraw ();
}

// EOF