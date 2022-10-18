#include "LABSoft_Controller_Oscilloscope.h"

#include <iostream>

#include "Globals.h"


LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope_cb_fl_light_button_generate_sine_wave (Fl_Light_Button *w, 
                                                                       void            *data)
{

}

void LABSoft_Controller_Oscilloscope::
cb_single (Fl_Button *w, 
           void      *data)
{
  printf ("DEBUG: single button clicked\n");
}

void LABSoft_Controller_Oscilloscope:: 
cb_channel_enable_disable (Fl_Light_Button *w,
                           void            *data)
{
  int channel = *((int*)data);
  bool value = w->value ();

  // backend
  m_LAB->m_Oscilloscope.m_channel_signals.m_channel_signal_vector[channel].
    m_is_enabled = value;

  // frontend
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    m_display->m_channel_signals.m_channel_signal_vector[channel].
      m_is_enabled = value;
}


void LABSoft_Controller_Oscilloscope::
cb_time_per_division (Fl_Input_Choice *w, 
                      void            *data)
{
  double value    = globals_get_actual_value_from_label (w->value ());
  int unit_scaler = globals_get_unit_prefix_power_scaler (w->value ());

  printf ("value: %f, unit_scaler: %d\n", value, unit_scaler);

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_time_per_division (value, unit_scaler);
}

void LABSoft_Controller_Oscilloscope::
cb_x_offset (Fl_Input_Choice *w, 
             void            *data)
{
  double value = globals_get_actual_value_from_label (w->value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    m_x_offset = value;

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_x_axis_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_volts_per_division (Fl_Input_Choice *w, 
                            void            *data)
{
  int channel = *((int*)data);
  double value    = globals_get_actual_value_from_label (w->value ());
  int unit_scaler = globals_get_unit_prefix_power_scaler (w->value ());

  printf ("value: %f, unit_scaler: %d\n", value, unit_scaler);

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_volts_per_division (value, unit_scaler, channel);
}

