#include "LABSoft_Controller_Oscilloscope.h"

#include <iostream>
#include <string>

#include <FL/Fl.H>

#include "Defaults.h"

LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

  // Link the LAB_Oscilloscope_Parent_Data struct from LAB_Oscilloscope
  // to the LABSoft_Oscilloscope_Display_Group class in the GUI
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    load_osc_parent_data (&(m_LAB->m_Oscilloscope.m_parent_data));
  
  // reserve() the pixel point vectors in the LAB_Oscilloscope_Parent_Data
  // struct 
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    reserve_pixel_points ();
  
  m_LAB->m_Oscilloscope.time_per_division (LAB_OSCILLOSCOPE_TIME_PER_DIVISION,
    LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS);

  // Do these here, instead of in the LABSoft_Oscilloscope_Display_Group widget,
  // because we already just linked m_parent_data to the said widget
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_voltage_per_division_labels ();
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_time_per_division_labels ();
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_upper_osc_disp_info ();

}

void LABSoft_Controller_Oscilloscope:: 
cb_run_stop (Fl_Light_Button *w, 
             void            *data)
{
  
  if (m_LAB->m_Oscilloscope.is_running ())
  {
    m_LAB->m_Oscilloscope.stop ();
      
  }
  else 
  { 
    m_LAB->m_Oscilloscope.run ();
    
    m_LABSoft_GUI->voltmeter_fl_button_run_stop->label ("Run");
  }
}

void LABSoft_Controller_Oscilloscope:: 
cb_channel_enable_disable (Fl_Light_Button *w,
                           long             data)
{
  unsigned  channel = static_cast<unsigned>(data);
  bool      value   = w->value ();

  m_LAB->m_Oscilloscope.channel_enable_disable (channel, value);

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_voltage_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_horizontal_offset (Fl_Input_Choice *w, 
             void            *data)
{
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Oscilloscope.horizontal_offset (_LabelValue.actual_value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_voltage_per_division (Fl_Input_Choice *w, 
                          long            channel)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.voltage_per_division (channel),
    LABELVALUE_TYPE::VOLTS
  );

  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LAB_OSCILLOSCOPE_MAX_VOLTAGE_PER_DIVISION &&
      lv.actual_value () <= LAB_OSCILLOSCOPE_MIN_VOLTAGE_PER_DIVISION)
    {
      m_LAB->m_Oscilloscope.voltage_per_division (channel, lv.actual_value ());

      return;
    }
  }

  //
  w->value (LabelValue (m_LAB->m_Oscilloscope.voltage_per_division (channel)).
    to_label_text (LABELVALUE_TYPE::VOLTS_PER_DIVISION).c_str ());
}

void LABSoft_Controller_Oscilloscope::
cb_vertical_offset (Fl_Input_Choice *w, 
                    long             channel)
{ 
  m_LAB->m_Oscilloscope.vertical_offset (static_cast<unsigned>(channel),
    LabelValue (w->value ()).actual_value ());
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_voltage_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_ac_coupling (Fl_Light_Button *w, 
                long             channel)
{
  m_LAB->m_Oscilloscope.coupling (static_cast<unsigned>(channel),
    (w->value () == 0) ? LE_OSC_COUPLING::DC : LE_OSC_COUPLING::AC);
}

void LABSoft_Controller_Oscilloscope:: 
cb_scaling (Fl_Choice *w,
            long       channel)
{
  std::string item (w->text ());

  LE_OSC_SCALING scaling;

  if (item == "x4")
  {
    scaling = LE_OSC_SCALING::QUADRUPLE;
  }
  else if (item == "x1")
  {
    scaling = LE_OSC_SCALING::UNITY;
  }
  else if (item == "x0.5")
  {
    scaling = LE_OSC_SCALING::HALF;
  }
  else if (item == "x0.25")
  {
    scaling = LE_OSC_SCALING::FOURTH;
  }
  else 
  {
    scaling = LE_OSC_SCALING::UNITY;
  }

  m_LAB->m_Oscilloscope.scaling (static_cast<unsigned>(channel), scaling);
}

void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division (Fl_Input_Choice *w,
                      long             channel)
{
  LabelValue _LabelValue (w->value ());

  // Backend
  m_LAB->m_Oscilloscope.time_per_division (_LabelValue.actual_value (),
    LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS);

  // Frontend
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_upper_osc_disp_info ();
}

void LABSoft_Controller_Oscilloscope::
cb_test (Fl_Input_Choice *w, long channel)
{

  LabelValue lv (
    w->value (), 
    m_LAB->m_Oscilloscope.voltage_per_division (channel),
    LABELVALUE_TYPE::VOLTS
  );

  // std::cout << "\ninput: " << w->value () << std::endl;
  // std::cout << "is valid: " << lv.is_valid_label_text () << std::endl;
  // std::cout << "actual value: " << lv.actual_value () << std::endl;
  // std::cout << "coeff: " << lv.coefficient () << std::endl;
  // std::cout << "exponent: " << lv.exponent () << std::endl;
  // std::cout << "prefix: " << lv.unit_prefix ()   << std::endl;
  // std::cout << "labelvalue string: " << lv.label_for () << std::endl;
  // std::cout << "full string: " << lv.to_label_text () << std::endl;

  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LAB_OSCILLOSCOPE_MAX_VOLTAGE_PER_DIVISION &&
      lv.actual_value () <= LAB_OSCILLOSCOPE_MIN_VOLTAGE_PER_DIVISION)
    {
      m_LAB->m_Oscilloscope.voltage_per_division (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Oscilloscope.voltage_per_division (channel)).
    to_label_text (LABELVALUE_TYPE::VOLTS).c_str ());
}

// EOF