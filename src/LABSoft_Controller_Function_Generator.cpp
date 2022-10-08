#include "LABSoft_Controller_Function_Generator.h"

#include <cstring>

#include "Defaults.h"

LABSoft_Controller_Function_Generator::
LABSoft_Controller_Function_Generator (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

// callback functions
void LABSoft_Controller_Function_Generator::
cb_run_stop (Fl_Light_Button *w, 
             void            *data)
{
  Channel_Signal *a = m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
    display ()->channel_signals ()->channel_signal_vector (LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER);

  if (w->value () == 0)
    {
      // backend
      m_LAB->m_Function_Generator->is_running (false);

      // frontend
      a->is_enabled (false);

      w->label ("Run");
    }
  else 
    {
      // backend
      m_LAB->m_Function_Generator->is_running (true);

      // frontend
      a->is_enabled (true);

      w->label ("Stop");
    }
}

void LABSoft_Controller_Function_Generator::
cb_wave_type (Fl_Input_Choice *w, 
              void            *data)
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
  else 
    wave_type = SINE;

  // backend
  m_LAB->m_Function_Generator->channel_signals ()->channel_signal_vector
     (LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->
      function_wave_type (wave_type);
  m_LAB->m_Function_Generator->update ();

  // front end
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->display ()->
    channel_signals ()->channel_signal_vector (LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->
      function_wave_type (wave_type);
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->update ();
}

void LABSoft_Controller_Function_Generator:: 
cb_frequency (Fl_Input_Choice *w, 
              void            *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // backend
  m_LAB->m_Function_Generator->channel_signals ()->channel_signal_vector
     (LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->function_frequency (value);
  m_LAB->m_Function_Generator->update ();

  // front end
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->display ()->
    channel_signals ()->channel_signal_vector (LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->
      function_frequency (value);
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->update ();
}

void LABSoft_Controller_Function_Generator:: 
cb_period (Fl_Input_Choice *w, 
           void            *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  //backend
  m_LAB->m_Function_Generator->channel_signals ()->channel_signal_vector
     (LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->function_amplitude (1.0 / value);
  m_LAB->m_Function_Generator->update ();

  // front end
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
    display ()->channel_signals ()->channel_signal_vector (LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->
      function_frequency (1.0 / value);
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->update ();
}

void LABSoft_Controller_Function_Generator:: 
cb_amplitude (Fl_Input_Choice *w, 
              void            *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // backend
  m_LAB->m_Function_Generator->channel_signals ()->channel_signal_vector
     (LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->function_amplitude (value);
  m_LAB->m_Function_Generator->update ();

  // front end
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
    display ()->channel_signals ()->channel_signal_vector (LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->
    function_amplitude (value);
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->update ();
}

void LABSoft_Controller_Function_Generator:: 
cb_y_offset (Fl_Input_Choice *w, 
             void            *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // change in backend
  m_LAB->m_Function_Generator->channel_signals ()->channel_signal_vector
     (LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->function_y_offset (value);
  m_LAB->m_Function_Generator->update ();

  // front end
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
    display ()->channel_signals ()->channel_signal_vector (LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->
      function_y_offset (value);
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->update ();
}

void LABSoft_Controller_Function_Generator:: 
cb_duty_cycle (Fl_Input_Choice *w, 
               void            *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // change in backend
  m_LAB->m_Function_Generator->channel_signals ()->channel_signal_vector
     (LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->function_duty_cycle (value);
  m_LAB->m_Function_Generator->update ();

  // front end
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
    display ()->channel_signals ()->channel_signal_vector (LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->
    function_duty_cycle (value);
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->update ();
}

void LABSoft_Controller_Function_Generator:: 
cb_phase (Fl_Input_Choice *w, 
          void            *data)
{
  float value = LAB_Globals_label_to_scaled_value (w->value ());

  // change in backend
  m_LAB->m_Function_Generator->channel_signals ()->channel_signal_vector
     (LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->function_phase (value);
  m_LAB->m_Function_Generator->update ();

  // front end
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
    display ()->channel_signals ()->channel_signal_vector (LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER)->
    function_phase (value);
  m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->update ();
}

// EOF




