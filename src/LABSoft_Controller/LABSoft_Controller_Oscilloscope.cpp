#include "LABSoft_Controller_Oscilloscope.h"

#include <cmath>
#include <string>
#include <iomanip>

#include "LABSoft_Controller.h"
#include "../Utility/LAB_LabelValue.h"
#include "../Utility/LAB_Utility_Functions.h"
#include "../Utility/LABSoft_GUI_Label_Values.h"

LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LAB*                 _LAB, 
                                 LABSoft_GUI*         _LABSoft_GUI,
                                 LABSoft_Controller*  _LABSoft_Controller)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI), 
    m_LABSoft_Controller  (_LABSoft_Controller)
{
  init            ();
  init_gui_values ();
}

void LABSoft_Controller_Oscilloscope:: 
init ()
{
  m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display->
    load_parent_data (m_LAB->m_Oscilloscope.parent_data ());
}

void LABSoft_Controller_Oscilloscope:: 
init_gui_values ()
{
  LAB_Oscilloscope& osc = m_LAB->m_Oscilloscope;
  LABSoft_GUI&      gui = *m_LABSoft_GUI;

  // --- Mode ---
  Fl_Choice* w = m_LABSoft_GUI->oscilloscope_fl_choice_mode;

  w->value (w->find_index (LABS_GUI_VALUES::OSC::MODE.at (m_LAB->
    m_Oscilloscope.mode ()).c_str ()));

  // --- Vertical ---
 
  // Channel 0 Coupling
  osc.coupling (0) == LABE::OSC::COUPLING::AC ? 
    gui.oscilloscope_fl_light_button_channel_0_ac_coupling->set   () : 
    gui.oscilloscope_fl_light_button_channel_0_ac_coupling->clear ();

  // Channel 0 Scaling
  LABSoft_GUI_Fl_Choice_With_Scroll* temp_0 = 
    gui.oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_0_scaling;
  
  temp_0->value (temp_0->find_index (LABS_GUI_VALUES::OSC::SCALING.at (
    m_LAB->m_Oscilloscope.channel_data (0).scaling).c_str ()));

  // Channel 0 Voltage per Division
  gui.oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_voltage_per_division->value (
    LAB_LabelValue (osc.voltage_per_division (0)).to_label_text (
    LAB_LabelValue::UNIT::VOLT_PER_DIVISION).c_str ()
  );

  // Channel 0 Vertical Offset
  gui.oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_vertical_offset->value (
    LAB_LabelValue (osc.vertical_offset (0)).to_label_text (
    LAB_LabelValue::UNIT::VOLT).c_str ()
  );

  // Channel 1 Coupling
  osc.coupling (1) == LABE::OSC::COUPLING::AC ? 
    gui.oscilloscope_fl_light_button_channel_1_ac_coupling->set   () : 
    gui.oscilloscope_fl_light_button_channel_1_ac_coupling->clear ();

  // Channel 1 Scaling
  LABSoft_GUI_Fl_Choice_With_Scroll* temp_1 = 
    gui.oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_1_scaling;
  
  temp_1->value (temp_1->find_index (LABS_GUI_VALUES::OSC::SCALING.at (
    m_LAB->m_Oscilloscope.channel_data (1).scaling).c_str ()));

  // Channel 1 Voltage per Division
  gui.oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_voltage_per_division->value (
    LAB_LabelValue (osc.voltage_per_division (1)).to_label_text (
    LAB_LabelValue::UNIT::VOLT_PER_DIVISION).c_str ()
  );

  // Channel 1 Vertical Offset
  gui.oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_vertical_offset->value (
    LAB_LabelValue (osc.vertical_offset (1)).to_label_text (
    LAB_LabelValue::UNIT::VOLT).c_str ()
  );

  // --- Horizontal ---
  gui.oscilloscope_fl_input_choice_horizontal_offset->value (
    LAB_LabelValue (osc.horizontal_offset ()).to_label_text (
    LAB_LabelValue::UNIT::SECOND).c_str ()
  );

  gui.oscilloscope_fl_input_choice_time_per_division->value (
    LAB_LabelValue (osc.time_per_division ()).to_label_text (
    LAB_LabelValue::UNIT::SECOND_PER_DIVISION).c_str ()
  );

  gui.oscilloscope_fl_input_choice_samples->value (
    LAB_LabelValue (osc.samples ()).to_label_text (
    LAB_LabelValue::UNIT::NONE).c_str ()
  );

  gui.oscilloscope_fl_input_choice_sampling_rate->value (
    LAB_LabelValue (osc.sampling_rate ()).to_label_text (
    LAB_LabelValue::UNIT::HERTZ).c_str ()
  );

  // --- Trigger ---

  
  gui.oscilloscope_fl_input_choice_trigger_level->value (
    LAB_LabelValue (osc.trigger_level ()).to_label_text (
    LAB_LabelValue::UNIT::VOLT).c_str ()
  );
}

void LABSoft_Controller_Oscilloscope:: 
cb_run_stop (Fl_Light_Button* w, 
             void*            data)
{
  bool value = w->value ();

  if (value)
  {
    m_LAB->m_Oscilloscope.run ();
    m_LABSoft_GUI->voltmeter_fl_light_button_run_stop->clear  ();
  }
  else 
  {
    m_LAB->m_Oscilloscope.stop ();
  }
}

void LABSoft_Controller_Oscilloscope:: 
cb_single (Fl_Button* w,
           void*      data)
{
  m_LABSoft_GUI->oscilloscope_fl_light_button_run_stop->clear ();

  m_LAB->m_Oscilloscope.single ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_channel_enable_disable (Fl_Light_Button* w,
                           long             channel)
{
  bool value = w->value ();

  m_LAB->m_Oscilloscope.channel_enable_disable (channel, value);

  m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display->
    update_gui_vertical_elements ();
}

void LABSoft_Controller_Oscilloscope::
cb_ac_coupling (Fl_Light_Button*  w, 
                long              channel)
{
  m_LAB->m_Oscilloscope.coupling (channel, static_cast<bool>(w->value ()) ? 
    LABE::OSC::COUPLING::AC : LABE::OSC::COUPLING::DC);
}

void LABSoft_Controller_Oscilloscope::
cb_voltage_per_division (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, 
                        long                                      channel)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.voltage_per_division (channel),
    LAB_LabelValue::UNIT::VOLT_PER_DIVISION
  );

  if (lv.is_valid ())
  {
    m_LAB->m_Oscilloscope.voltage_per_division (channel, lv.actual_value ());
  }

  w->value (LAB_LabelValue (m_LAB->m_Oscilloscope.voltage_per_division (channel)).
    to_label_text (LAB_LabelValue::UNIT::VOLT_PER_DIVISION).c_str ());

  m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display->
    update_gui_voltage_per_division ();
}

void LABSoft_Controller_Oscilloscope::
cb_vertical_offset (LABSoft_GUI_Fl_Input_Choice_With_Scroll*  w, 
                    long                                      channel)
{ 
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.vertical_offset (channel),
    LAB_LabelValue::UNIT::VOLT
  );

  if (lv.is_valid ())
  {
    m_LAB->m_Oscilloscope.vertical_offset (channel, lv.actual_value ());
  }

  w->value (LAB_LabelValue (m_LAB->m_Oscilloscope.vertical_offset (channel)).
    to_label_text (LAB_LabelValue::UNIT::VOLT).c_str ());
  
  m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display->
    update_gui_voltage_per_division ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_fl_slider_vertical_offset (Fl_Slider*  w,
                              void*       data)
{
  // double max_voltage_per_division = 
  // double min_voltage_per_division = 
}

void LABSoft_Controller_Oscilloscope:: 
cb_scaling (LABSoft_GUI_Fl_Choice_With_Scroll*  w,
            long                                channel)
{
  std::string item (w->text ());

  LABE::OSC::SCALING scaling;

  if (item == "x4")
  {
    scaling = LABE::OSC::SCALING::QUADRUPLE;
  }
  else if (item == "x1")
  {
    scaling = LABE::OSC::SCALING::UNITY;
  }
  else if (item == "x0.5")
  {
    scaling = LABE::OSC::SCALING::HALF;
  }
  else if (item == "x0.25")
  {
    scaling = LABE::OSC::SCALING::FOURTH;
  }
  else 
  {
    scaling = LABE::OSC::SCALING::UNITY;
  }

  m_LAB->m_Oscilloscope.scaling (channel, scaling);

  m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display->
    update_gui_voltage_per_division ();
}

// Horizontal
void LABSoft_Controller_Oscilloscope::
cb_horizontal_offset (Fl_Input_Choice* w, 
                      void*            data)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.horizontal_offset (),
    LAB_LabelValue::UNIT::SECOND
  );

  if (lv.is_valid ())
  {
    m_LAB->m_Oscilloscope.horizontal_offset (lv.actual_value ());
  }

  update_gui_horizontal ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division (Fl_Input_Choice* w,
                      void*            data)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.time_per_division (),
    LAB_LabelValue::UNIT::SECOND_PER_DIVISION
  );

  if (lv.is_valid ())
  {
    m_LAB->m_Oscilloscope.time_per_division (lv.actual_value ());
  }

  update_gui_horizontal ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_samples (Fl_Input_Choice*  w,
            void*             data)
{
  LAB_LabelValue lv (
    w->value (), 
    m_LAB->m_Oscilloscope.samples (),
    LAB_LabelValue::UNIT::NONE
  );

  if (lv.is_valid ())
  {
    m_LAB->m_Oscilloscope.samples (std::round (lv.actual_value ()));
  }

  update_gui_horizontal ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_sampling_rate (Fl_Input_Choice*  w,
                  void*             data)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.sampling_rate (),
    LAB_LabelValue::UNIT::HERTZ
  );
  
  if (lv.is_valid ())
  {
    m_LAB->m_Oscilloscope.sampling_rate (lv.actual_value ());
  }

  // Frontend
  update_gui_horizontal ();
}

void LABSoft_Controller_Oscilloscope::
cb_mode (Fl_Choice* w,  
         void*      data)
{
  std::string str (w->text ());

  LABE::OSC::MODE mode;

  if (str == "Repeated")
  {
    mode = LABE::OSC::MODE::REPEATED;
  }
  else if (str == "Screen")
  {
    mode = LABE::OSC::MODE::SCREEN;
  }
  else if (str == "Record")
  {
    mode = LABE::OSC::MODE::RECORD;
  }
  else 
  {
    throw (std::runtime_error ("Invalid display mode input."));
  }

  m_LAB->m_Oscilloscope.mode (mode);

  update_gui_mode ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_channel_selector (Fl_Toggle_Button*  w,
                     long               channel)
{
  m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display->
    channel_selector (channel);
}

void LABSoft_Controller_Oscilloscope::
cb_display_time_per_division (LABSoft_GUI_Oscilloscope_Display_Internal*  w, 
                              void*                                       data)
{
  double time_per_division = *(static_cast<double*>(data));

  m_LAB->m_Oscilloscope.time_per_division (time_per_division);

  update_gui_horizontal ();
}

void LABSoft_Controller_Oscilloscope::
cb_display_horizontal_offset (LABSoft_GUI_Oscilloscope_Display_Internal*  w, 
                              void*                                       data)
{
  double horizontal_offset = *(static_cast<double*>(data));

  m_LAB->m_Oscilloscope.horizontal_offset (horizontal_offset);

  update_gui_horizontal ();
}

void LABSoft_Controller_Oscilloscope::
cb_trigger_mode (Fl_Choice* w,
                 void*      data)
{
  LABE::OSC::TRIG::MODE mode;
  std::string choice (w->text ());

  if (choice == "None")
  {
    mode = LABE::OSC::TRIG::MODE::NONE;
  }
  else if (choice == "Auto")
  { 
    mode = LABE::OSC::TRIG::MODE::AUTO;
  }
  else if (choice == "Normal")
  {
    mode = LABE::OSC::TRIG::MODE::NORMAL;
  }
  else 
  {
    throw (std::runtime_error ("Invalid trigger mode input."));
  }

  // backend
  m_LAB->m_Oscilloscope.trigger_mode (mode);

  // frontend
  update_trigger_panel_gui ();
}

void LABSoft_Controller_Oscilloscope::
cb_trigger_source (Fl_Choice* w, 
                   void*      data)
{
  unsigned source;
  std::string choice (w->text ());
  
  if (choice == "Channel 1")
  {
    source = 0;
  }
  else if (choice == "Channel 2")
  {
    source = 1;
  }
  else 
  {
    throw (std::runtime_error ("Invalid trigger source input."));
  }

  m_LAB->m_Oscilloscope.trigger_source (source);
}

void LABSoft_Controller_Oscilloscope:: 
cb_trigger_type (Fl_Choice* w,
                 void*      data)
{
  LABE::OSC::TRIG::TYPE type;
  std::string choice (w->text ());

  if (choice == "Edge")
  {
    type = LABE::OSC::TRIG::TYPE::EDGE;
  }
  else if (choice == "Level")
  {
    type = LABE::OSC::TRIG::TYPE::LEVEL;
  }
  else 
  {
    throw (std::runtime_error ("Invalid trigger type input."));
  }
  
  m_LAB->m_Oscilloscope.trigger_type (type);
}

void LABSoft_Controller_Oscilloscope:: 
cb_trigger_condition (Fl_Choice* w,
                      void*      data)
{
  LABE::OSC::TRIG::CND cnd;
  std::string choice (w->text ());

  if (choice == "Rising")
  {
    cnd = LABE::OSC::TRIG::CND::RISING;
  }
  else if (choice == "Falling")
  {
    cnd = LABE::OSC::TRIG::CND::FALLING;
  }
  else if (choice == "Either")
  {
    cnd = LABE::OSC::TRIG::CND::EITHER;
  }
  else 
  {
    throw (std::runtime_error ("Invalid trigger condition input."));
  }

  m_LAB->m_Oscilloscope.trigger_condition (cnd);
}

void LABSoft_Controller_Oscilloscope::
cb_trigger_level (Fl_Input_Choice* w, 
                  void*            data)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.trigger_level (),
    LAB_LabelValue::UNIT::VOLT
  );

  if (lv.is_valid ())
  {
    m_LAB->m_Oscilloscope.trigger_level (lv.actual_value ());
  }

  w->value (LAB_LabelValue (m_LAB->m_Oscilloscope.trigger_level ()).
    to_label_text (LAB_LabelValue::UNIT::VOLT).c_str ());

  // m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display->
  //   update_gui_fl_slider_trigger_level ();
}

void LABSoft_Controller_Oscilloscope::
cb_record (Fl_Button*  w, 
           void*       data)
{

}

void LABSoft_Controller_Oscilloscope::
cb_record_config (Fl_Button* w, 
                  void*      data)
{
  m_LABSoft_GUI->oscilloscope_fl_window_record_config->show ();
  m_LABSoft_GUI->main_fl_tabs->deactivate ();
}

void LABSoft_Controller_Oscilloscope::
cb_record_config_start (Fl_Button*  w,
                        void*       data)
{

}

void LABSoft_Controller_Oscilloscope::
cb_record_config_cancel (Fl_Button* w,
                         void*      data)
{
  m_LABSoft_GUI->oscilloscope_fl_window_record_config->hide ();
  m_LABSoft_GUI->main_fl_tabs->activate ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_export (Fl_Menu_Item*  w,
           void*          data)
{

}
    
void LABSoft_Controller_Oscilloscope:: 
display_update_cycle ()
{  
  m_LAB->m_Oscilloscope.load_data_samples ();

  m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display-> 
    update_display ();

  // TO-DO
  // this is not ideal as the trigger_found flag should not be changed
  // outside of LAB_Oscilloscope as much as possible.
  // but in the meantime, i am putting this here for possible performance
  // improvement. with this, the LAB_Oscilloscope does not have to find
  // for the next trigger as long as the current trigger has not yet been displayed

  if (m_LAB->m_Oscilloscope.m_parent_data.trigger_found)
  {
    m_LAB->m_Oscilloscope.m_parent_data.trigger_found = false;
  }
}

void LABSoft_Controller_Oscilloscope:: 
update_trigger_panel_gui ()
{
  if (m_LAB->m_Oscilloscope.trigger_mode () == LABE::OSC::TRIG::MODE::NONE)
  {
    m_LABSoft_GUI->oscilloscope_fl_choice_trigger_source->deactivate ();
    m_LABSoft_GUI->oscilloscope_fl_choice_trigger_type->deactivate ();
    m_LABSoft_GUI->oscilloscope_fl_choice_trigger_condition->deactivate ();
    m_LABSoft_GUI->oscilloscope_fl_input_choice_trigger_level->deactivate ();
  }
  else 
  {
    m_LABSoft_GUI->oscilloscope_fl_choice_trigger_source->activate ();
    m_LABSoft_GUI->oscilloscope_fl_choice_trigger_type->activate ();
    m_LABSoft_GUI->oscilloscope_fl_choice_trigger_condition->activate ();
    m_LABSoft_GUI->oscilloscope_fl_input_choice_trigger_level->activate ();
  }
}

void LABSoft_Controller_Oscilloscope:: 
update_gui_horizontal ()
{
  LAB_LabelValue horizontal_offset  (m_LAB->m_Oscilloscope.horizontal_offset ());
  LAB_LabelValue time_per_division  (m_LAB->m_Oscilloscope.time_per_division ());
  LAB_LabelValue samples            (m_LAB->m_Oscilloscope.samples ());
  LAB_LabelValue sampling_rate      (m_LAB->m_Oscilloscope.sampling_rate ());

  // 1. Horizontal Offset
  m_LABSoft_GUI->oscilloscope_fl_input_choice_horizontal_offset->value (
    horizontal_offset.to_label_text (LAB_LabelValue::UNIT::SECOND).c_str ()
  );

  // 2. Time per Division
  m_LABSoft_GUI->oscilloscope_fl_input_choice_time_per_division->value (
    time_per_division.to_label_text (LAB_LabelValue::UNIT::SECOND_PER_DIVISION).c_str ()
  );

  // 3. Samples
  m_LABSoft_GUI->oscilloscope_fl_input_choice_samples->value (
    samples.to_label_text (LAB_LabelValue::UNIT::NONE, 3).c_str ()
  );

  // 4. Sampling Rate
  m_LABSoft_GUI->oscilloscope_fl_input_choice_sampling_rate->value (
    sampling_rate.to_label_text (LAB_LabelValue::UNIT::HERTZ).c_str ()
  );

  // 5. Display Mode
  m_LABSoft_GUI->oscilloscope_fl_choice_mode->value (
    m_LABSoft_GUI->oscilloscope_fl_choice_mode->find_index (
      (LABS_GUI_VALUES::OSC::MODE.at (m_LAB->m_Oscilloscope.mode ())).c_str ()
    )
  );

  // 6. Time per Division Labels
  m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display->
    update_gui_time_per_division ();
}

void LABSoft_Controller_Oscilloscope:: 
update_gui_mode ()
{
  LABE::OSC::MODE mode = m_LAB->m_Oscilloscope.mode ();

  if (mode == LABE::OSC::MODE::RECORD)
  {
    m_LABSoft_GUI->oscilloscope_fl_light_button_run_stop->hide ();
    m_LABSoft_GUI->oscilloscope_fl_button_single->hide ();
    m_LABSoft_GUI->oscilloscope_fl_button_record->show ();
    m_LABSoft_GUI->oscilloscope_fl_button_record_config->show ();
  }
  else 
  {
    m_LABSoft_GUI->oscilloscope_fl_light_button_run_stop->show ();
    m_LABSoft_GUI->oscilloscope_fl_button_single->show ();
    m_LABSoft_GUI->oscilloscope_fl_button_record->hide ();
    m_LABSoft_GUI->oscilloscope_fl_button_record_config->hide ();

    // Frontend
    update_gui_horizontal ();

    m_LABSoft_GUI->oscilloscope_labsoft_gui_oscilloscope_display-> 
      update_gui_time_per_division ();
  }
}

// EOF