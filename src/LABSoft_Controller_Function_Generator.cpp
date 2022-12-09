#include "LABSoft_Controller_Function_Generator.h"

#include <iostream>
#include <cstring>

#include "Defaults.h"
#include "Auxiliary.h"

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
  int channel = (int)data;

  if (w->value () == 0)
  {
    m_LAB->m_Function_Generator->stop (channel);
  }
  else
  {
    m_LAB->m_Function_Generator->start (channel);
  }
}

void LABSoft_Controller_Function_Generator:: 
cb_frequency (Fl_Input_Choice *w, 
              void            *data)
{
  int channel = (int)data;
  LabelValue _LabelValue (w->value ());
  m_LAB->m_Function_Generator->frequency (channel, _LabelValue.actual_value ());
}

void LABSoft_Controller_Function_Generator:: 
cb_period (Fl_Input_Choice *w, 
           void            *data)
{
  int channel = (int)data;
  LabelValue _LabelValue (w->value ());
  m_LAB->m_Function_Generator->period (channel, _LabelValue.actual_value ());
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

  
}


void LABSoft_Controller_Function_Generator:: 
cb_amplitude (Fl_Input_Choice *w, 
              void            *data)
{
  // double value = g_get_actual_value_from_label (w->value ());

  // // backend
  // Channel_Signal* chn_b = &(m_LAB->m_Function_Generator->m_channel_signals.
  //   m_channel_signal_vector[LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER]);
  
  // // frontend 
  // Channel_Signal* chn_f = &(m_LABSoft_GUI->
  //   function_generator_labsoft_oscilloscope_display_group_display->m_display->
  //     m_channel_signals.
  //       m_channel_signal_vector[LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER]);
  
  // // set value
  // chn_b->m_function_amplitude = chn_f->m_function_amplitude = value;

  // // update
  // m_LAB->m_Function_Generator->update ();
  // m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
  //   update_fg ();
}

void LABSoft_Controller_Function_Generator:: 
cb_y_offset (Fl_Input_Choice *w, 
             void            *data)
{
  // double value = g_get_actual_value_from_label (w->value ());

  // // backend
  // Channel_Signal* chn_b = &(m_LAB->m_Function_Generator->m_channel_signals.
  //   m_channel_signal_vector[LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER]);
  
  // // frontend 
  // Channel_Signal* chn_f = &(m_LABSoft_GUI->
  //   function_generator_labsoft_oscilloscope_display_group_display->m_display->
  //     m_channel_signals.
  //       m_channel_signal_vector[LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER]);
  
  // // set value
  // chn_b->m_function_y_offset = chn_f->m_function_y_offset = value;

  // // update
  // m_LAB->m_Function_Generator->update ();
  // m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
  //   update_fg ();
}

void LABSoft_Controller_Function_Generator:: 
cb_duty_cycle (Fl_Input_Choice *w, 
               void            *data)
{
  // double value = g_get_actual_value_from_label (w->value ());

  // // backend
  // Channel_Signal* chn_b = &(m_LAB->m_Function_Generator->m_channel_signals.
  //   m_channel_signal_vector[LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER]);
  
  // // frontend 
  // Channel_Signal* chn_f = &(m_LABSoft_GUI->
  //   function_generator_labsoft_oscilloscope_display_group_display->m_display->
  //     m_channel_signals.
  //       m_channel_signal_vector[LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER]);
  
  // // set value
  // chn_b->m_function_duty_cycle = chn_f->m_function_duty_cycle = value;

  // // update
  // m_LAB->m_Function_Generator->update ();
  // m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
  //   update_fg ();
}

void LABSoft_Controller_Function_Generator:: 
cb_phase (Fl_Input_Choice *w, 
          void            *data)
{
  // double value = g_get_actual_value_from_label (w->value ());

  // // backend
  // Channel_Signal* chn_b = &(m_LAB->m_Function_Generator->m_channel_signals.
  //   m_channel_signal_vector[LAB_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER]);
  
  // // frontend 
  // Channel_Signal* chn_f = &(m_LABSoft_GUI->
  //   function_generator_labsoft_oscilloscope_display_group_display->m_display->
  //     m_channel_signals.
  //       m_channel_signal_vector[LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER]);
  
  // // set value
  // chn_b->m_function_phase = chn_f->m_function_phase = value;

  // // update
  // m_LAB->m_Function_Generator->update ();
  // m_LABSoft_GUI->function_generator_labsoft_oscilloscope_display_group_display->
  //   update_fg ();
}

// EOF




