#include "LAB_Function_Generator.h"

LAB_Function_Generator::
LAB_Function_Generator ()
{
  printf ("DEBUG: LAB_Function_Generator constructor start\n");
  printf ("DEBUG: LAB_Function_Generator constructor end\n");
}

// setters
void LAB_Function_Generator_set_wave_type (WaveType wave_type)
{
  m_wave_type = wave_type;
}

void LAB_Function_Generator_set_amplitude (float amplitude)
{
  m_amplitude = amplitude;
}

// callback functions
void LAB_Function_Generator:: 
LAB_Function_Generator_cb_function_generator_fl_light_button_start_stop (Fl_Input_Choice *w, 
                                                                         void            *data)
{
  LABSoft_Function_Generator *disp = static_cast<LABSoft_Function_Generator_Display *>(data);

  if (w->value () == 0)
    {
      m_flag_global_enable = false;

      disp->m_flag_global_enable = false;
      disp->LABSoft_Function_Generator_reload_and_draw ();
  
      w->label () = "Stopped";
    }
  else 
    {
      m_flag_global_enable = true;

      disp->m_flag_global_enable = true;
      disp->LABSoft_Function_Generator_reload_and_draw ();

      w->label () = "Running";
    }
}

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
  else
    wave_type = SINE;

  LAB_Function_Generator_set_wave_type (wave_type);

  static_cast<LABSoft_Function_Generator_Display *>(data)->LABSoft_Function_Generator_Display_set_wave_type (wave_type);
}

void LAB_Function_Generator:: 
LAB_Function_Generator_cb_function_generator_fl_input_choice_frequency (Fl_Input_Choice *w, 
                                                                        void            *data)
{
  LABSoft_Function_Generator_set_period_or_frequency (w, data);
}


void LAB_Function_Generator:: 
LAB_Function_Generator_cb_function_generator_fl_input_choice_time_per_division (Fl_Input_Choice *w,
                                                                                void            *data)
{
  char *s, *unit;
  float value;

  const Fl_Menu_Item *item = w->menubutton()->mvalue();

  // get label
  s = strtok (strdup (item->label()), " ");

  // get numerical value in label
  value = atof (s);

  printf ("initial value: %f\n", value);

  // get unit as char*
  s = strtok (NULL, "/");
  unit = s;

  printf ("unit: %s\n", unit);

  // scale numerical value based on unit
  value = value * (unit == "s" ? 1.0
                    : (unit == "ms" ? 0.001
                      : (unit == "us" ? 0.000001
                        : (unit == "ns" ? 0.000000001
                          : 0.000000001))));

  _LABSoft_Oscilloscope_Display->LABSoft_Oscilloscope_Display_set_time_per_division (value);

  _LABSoft_Oscilloscope_Display->LABSoft_Oscilloscope_Display_generate_wave (1, 3.0, 2.0, SINE);

  _LABSoft_Oscilloscope_Display->redraw ();
}

void LAB_Function_Generator:: 
LAB_Function_Generator_cbfunction_generator_cb_fl_light_button_start_stop (Fl_Light_Button *w, 
                                                                        void            *data)
{
  LABSoft_Oscilloscope_Display *display = static_cast<LABSoft_Oscilloscope_Display *>(data);

  if (w->value () == 0)
    {
      m_flag_global_enable = false;
      display->LABSoft_Oscilloscope_Display_display_disable ();

      w->label ("Stopped");
    }
  else
    {
      m_flag_global_enable = true;
      display->LABSoft_Oscilloscope_Display_display_enable ();

      w->label ("Running");
    }
}

void LAB_Function_Generator::
LAB_Function_Generator_cb_function_generator_fl_input_choice_amplitude (Fl_Light_Button *w, 
                                                                        void            *data)
{
  m_amplitude = atof(w->input ()->value ()); 
}

// setters
void LABSoft_Function_Generator:: 
LABSoft_Function_Generator_set_period_or_frequency (Fl_Input_Choice *w, 
                                                    void            *data)
{
  // get the contents of the input field as a string
  const char *label= w->label ();

  // get the numerical value and unit in the input label
  float value =  GLOBAL_label_to_value (w->input ()->value ());



}

// callback functions
void LABSoft_Function_Generator_Display::
LABSoft_Function_Generator_Display_cb_function_generator_fl_input_choice_frequency (Fl_Input_Choice *w, 
                                                                                    void            *data)
{
  LABSoft_Function_Generator_set_period_or_frequency (w, data);
}

void LABSoft_Function_Generator_Display::
LABSoft_Function_Generator_Display_cb_function_generator_fl_input_choice_period (Fl_Input_Choice *w, 
                                                                                    void            *data)
{
  LABSoft_Function_Generator_set_period_or_frequency (w, data);
}

// EOF