#include "LABSoft_Controller_Oscilloscope.h"

#include <cmath>
// #include <string>
// #include <iomanip>

// #include "LABSoft_Controller.h"

// #include "../Utility/LAB_Utility_Functions.h"

#include "../Utility/LABSoft_GUI_Label.h"
#include "../Utility/LABSoft_GUI_Label_Values.h"

// // delete soon
// #include <iostream>

LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LABSoft_Controller& _LABSoft_Controller)
  : LABSoft_Controller_Unit  (_LABSoft_Controller)
{
  init_gui_values ();
}

void LABSoft_Controller_Oscilloscope:: 
init_gui_values ()
{
  LAB_Oscilloscope& osc = lab ().m_Oscilloscope;

  // 0.
  gui ().oscilloscope_labsoft_gui_oscilloscope_display->load_oscilloscope (osc);

  // ========== 1. vertical ==========

  // 1.1. channel enable disable
  osc.is_channel_enabled (0) ? 
    gui ().oscilloscope_fl_light_button_channel_0_enable->set    ():
    gui ().oscilloscope_fl_light_button_channel_0_enable->clear  ();
  
  osc.is_channel_enabled (1) ? 
    gui ().oscilloscope_fl_light_button_channel_1_enable->set    ():
    gui ().oscilloscope_fl_light_button_channel_1_enable->clear  ();

  // 1.2. coupling
  osc.coupling (0) == LABE::OSC::COUPLING::AC ? 
    gui ().oscilloscope_fl_light_button_channel_0_ac_coupling->set   (): 
    gui ().oscilloscope_fl_light_button_channel_0_ac_coupling->clear ();

  osc.coupling (1) == LABE::OSC::COUPLING::AC ? 
    gui ().oscilloscope_fl_light_button_channel_1_ac_coupling->set   (): 
    gui ().oscilloscope_fl_light_button_channel_1_ac_coupling->clear ();

  // 1.3. scaling
  {
    LABSoft_GUI_Fl_Choice_With_Scroll* w;

    w = gui ().oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_0_scaling;

    w->value (w->find_index (LABS_GUI_VALUES::OSC::SCALING_s.at (osc.scaling (0)).c_str ()));
  }
  {
    LABSoft_GUI_Fl_Choice_With_Scroll* w;

    w = gui ().oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_1_scaling;
  
    w->value (w->find_index (LABS_GUI_VALUES::OSC::SCALING_s.at (osc.scaling (1)).c_str ()));
  } 
  
  // 1.4. voltage per division
  {
    LABSoft_GUI_Fl_Input_Choice_With_Scroll* w;

    w = gui ().oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_voltage_per_division;

    w->value (LABSoft_GUI_Label (osc.voltage_per_division (0)).
      to_label_text (LABSoft_GUI_Label::UNIT::VOLT_PER_DIVISION).c_str ());
  }
  {
    LABSoft_GUI_Fl_Input_Choice_With_Scroll* w;
    
    w = gui ().oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_voltage_per_division;

    w->value (LABSoft_GUI_Label (osc.voltage_per_division (1)).
      to_label_text (LABSoft_GUI_Label::UNIT::VOLT_PER_DIVISION).c_str ());
  }
  
  // 1.5. vertical offset
  {
    LABSoft_GUI_Fl_Input_Choice_With_Scroll* w;

    w = gui ().oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_vertical_offset;

    w->value (LABSoft_GUI_Label (osc.vertical_offset (0)).
      to_label_text (LABSoft_GUI_Label::UNIT::VOLT).c_str ());
  }
  {
    LABSoft_GUI_Fl_Input_Choice_With_Scroll* w;

    w = gui ().oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_vertical_offset;

    w->value (LABSoft_GUI_Label (osc.vertical_offset (1)).
      to_label_text (LABSoft_GUI_Label::UNIT::VOLT).c_str ());
  }
  
  // ========== 2. horizontal ==========
  update_gui_horizontal_elements ();

  // ========== 3. trigger ==========
  {
    LABSoft_GUI_Fl_Choice_With_Scroll* w;

    w = gui ().oscilloscope_fl_choice_trigger_mode;

    w->value (w->find_index (LABS_GUI_VALUES::OSC::TRIGGER_MODE_s.at
      (osc.trigger_mode ()).c_str ()));
  }
  {
    LABSoft_GUI_Fl_Choice_With_Scroll* w;

    w = gui ().oscilloscope_fl_choice_trigger_source;

    w->value (w->find_index (LABS_GUI_VALUES::OSC::TRIGGER_SOURCE_s.at
      (osc.trigger_source ()).c_str ()));
  } 
  {
    LABSoft_GUI_Fl_Choice_With_Scroll* w;

    w = gui ().oscilloscope_fl_choice_trigger_type;

    w->value (w->find_index (LABS_GUI_VALUES::OSC::TRIGGER_TYPE_s.at
      (osc.trigger_type ()).c_str ()));
  }
  {
    LABSoft_GUI_Fl_Choice_With_Scroll* w;

    w = gui ().oscilloscope_fl_choice_trigger_condition;

    w->value (w->find_index (LABS_GUI_VALUES::OSC::TRIGGER_CONDITION_s.at
      (osc.trigger_condition ()).c_str ()));
  }

  gui ().oscilloscope_fl_input_choice_trigger_level->value (
    LABSoft_GUI_Label (osc.trigger_level ()).to_label_text (
    LABSoft_GUI_Label::UNIT::VOLT).c_str ()
  );

  // ========== 4. mode ==========
  {
    LABSoft_GUI_Fl_Choice_With_Scroll* w;

    w = gui ().oscilloscope_fl_choice_mode;

    w->value (w->find_index (LABS_GUI_VALUES::OSC::MODE_s.at 
      (osc.mode ()).c_str ())); 
  }
}

void LABSoft_Controller_Oscilloscope:: 
cb_run_stop (Fl_Light_Button* w, 
             void*            data)
{
  if (w->value ())
  {
    gui ().voltmeter_fl_light_button_run_stop->clear ();
    
    lab ().m_Oscilloscope.run ();
  }
  else 
  {
    lab ().m_Oscilloscope.stop ();
  }
}

void LABSoft_Controller_Oscilloscope:: 
cb_single (Fl_Button* w,
           void*      data)
{
  gui ().oscilloscope_fl_light_button_run_stop->clear ();

  lab ().m_Oscilloscope.single ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_channel_enable_disable (Fl_Light_Button* w,
                           long             channel)
{
  lab ().m_Oscilloscope.channel_enable_disable (channel, w->value ());

  gui ().oscilloscope_labsoft_gui_oscilloscope_display->
    update_channel_enable_disable ();
}

void LABSoft_Controller_Oscilloscope::
cb_ac_coupling (Fl_Light_Button*  w, 
                long              channel)
{
  lab ().m_Oscilloscope.coupling (channel, w->value () ? 
    LABE::OSC::COUPLING::AC : LABE::OSC::COUPLING::DC);
}

void LABSoft_Controller_Oscilloscope:: 
cb_scaling (LABSoft_GUI_Fl_Choice_With_Scroll*  w,
            long                                channel)
{
  lab ().m_Oscilloscope.scaling (
    channel,
    LABS_GUI_VALUES::OSC::SCALING[std::string (w->text ())]
  );
}

void LABSoft_Controller_Oscilloscope::
cb_voltage_per_division (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, 
                        long                                      channel)
{
  LABSoft_GUI_Label label (
    w->value (),
    lab ().m_Oscilloscope.voltage_per_division (channel),
    LABSoft_GUI_Label::UNIT::VOLT_PER_DIVISION
  );

  if (label.is_valid ())
  {
    lab ().m_Oscilloscope.voltage_per_division (channel, label.actual_value ());

    gui ().oscilloscope_labsoft_gui_oscilloscope_display->
      update_voltage_per_division ();
  }

  w->value (
    LABSoft_GUI_Label (lab ().m_Oscilloscope.voltage_per_division (channel)).
    to_label_text (LABSoft_GUI_Label::UNIT::VOLT_PER_DIVISION).c_str ()
  );
}

void LABSoft_Controller_Oscilloscope::
cb_vertical_offset (LABSoft_GUI_Fl_Input_Choice_With_Scroll*  w, 
                    long                                      channel)
{ 
  LABSoft_GUI_Label label (
    w->value (),
    lab ().m_Oscilloscope.vertical_offset (channel),
    LABSoft_GUI_Label::UNIT::VOLT
  );

  if (label.is_valid ())
  {
    lab ().m_Oscilloscope.vertical_offset (channel, label.actual_value ());

    gui ().oscilloscope_labsoft_gui_oscilloscope_display->
      update_vertical_offset ();
  }

  w->value (
    LABSoft_GUI_Label (lab ().m_Oscilloscope.vertical_offset (channel)).
    to_label_text (LABSoft_GUI_Label::UNIT::VOLT).c_str ()
  );
}

void LABSoft_Controller_Oscilloscope:: 
cb_vertical_offset_slider (LABSoft_GUI_Fl_Slider* w,
                           void*                  data)
{
  unsigned channel = reinterpret_cast<unsigned>(data);

  // lab ().m_Oscilloscope.vertical_offset (channel, w->value ());

  // LABSoft_GUI_Label lbl (lab ().m_Oscilloscope.vertical_offset (channel));

  // // gui ().oscilloscope_fl_input_choice_vertical_offset->value (
  // //   lbl.to_label_text (LABSoft_GUI_Label::UNIT::VOLT).c_str ()
  // // );
}

// Horizontal
void LABSoft_Controller_Oscilloscope::
cb_horizontal_offset (Fl_Input_Choice* w, 
                      void*            data)
{
  LABSoft_GUI_Label label (
    w->value (),
    lab ().m_Oscilloscope.horizontal_offset (),
    LABSoft_GUI_Label::UNIT::SECOND
  );

  if (label.is_valid ())
  {
    lab ().m_Oscilloscope.horizontal_offset (label.actual_value ());

    gui ().oscilloscope_labsoft_gui_oscilloscope_display->
      update_horizontal_offset ();
  }

  w->value (
    LABSoft_GUI_Label (lab ().m_Oscilloscope.horizontal_offset ()).
    to_label_text (LABSoft_GUI_Label::UNIT::SECOND).c_str ()
  );
}

void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division (Fl_Input_Choice* w,
                      void*            data)
{
  LABSoft_GUI_Label label (
    w->value (),
    lab ().m_Oscilloscope.time_per_division (),
    LABSoft_GUI_Label::UNIT::SECOND_PER_DIVISION
  );

  if (label.is_valid ())
  {
    lab ().m_Oscilloscope.time_per_division (label.actual_value ());

    gui ().oscilloscope_labsoft_gui_oscilloscope_display->
      update_time_per_division ();
  }

  update_gui_horizontal_elements ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division_steps (int steps)
{

}

void LABSoft_Controller_Oscilloscope:: 
cb_samples (Fl_Input_Choice*  w,
            void*             data)
{
  LABSoft_GUI_Label label (
    w->value (), 
    0, // 0 as reference kay whole numbers raman sad ang samples
    LABSoft_GUI_Label::UNIT::NONE
  );

  if (label.is_valid ())
  {
    lab ().m_Oscilloscope.samples (std::round (label.actual_value ()));

    gui ().oscilloscope_labsoft_gui_oscilloscope_display->
      update_samples ();
  }

  update_gui_horizontal_elements ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_sampling_rate (Fl_Input_Choice*  w,
                  void*             data)
{
  LABSoft_GUI_Label label (
    w->value (),
    lab ().m_Oscilloscope.sampling_rate (),
    LABSoft_GUI_Label::UNIT::HERTZ
  );
  
  if (label.is_valid ())
  {
    lab ().m_Oscilloscope.sampling_rate (label.actual_value ());
  }

  update_gui_horizontal_elements ();
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

  lab ().m_Oscilloscope.mode (mode);

  update_gui_mode ();
}

// void LABSoft_Controller_Oscilloscope::
// cb_display_time_per_division (LABSoft_GUI_Oscilloscope_Display_Internal*  w, 
//                               void*                                       data)
// {
//   double time_per_division = *(static_cast<double*>(data));

//   lab ().m_Oscilloscope.time_per_division (time_per_division);

//   update_gui_horizontal ();
// }

// void LABSoft_Controller_Oscilloscope::
// cb_display_horizontal_offset (LABSoft_GUI_Oscilloscope_Display_Internal*  w, 
//                               void*                                       data)
// {
//   double horizontal_offset = *(static_cast<double*>(data));

//   lab ().m_Oscilloscope.horizontal_offset (horizontal_offset);

//   update_gui_horizontal ();
// }

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
  lab ().m_Oscilloscope.trigger_mode (mode);

  // frontend
  update_trigger_panel_gui ();
}

void LABSoft_Controller_Oscilloscope::
cb_trigger_source (Fl_Choice* w, 
                   void*      data)
{
  unsigned trigger_source = LABS_GUI_VALUES::OSC::TRIGGER_SOURCE.at (std::string (w->text ()));

  lab ().m_Oscilloscope.trigger_source (trigger_source);

  gui ().oscilloscope_labsoft_gui_oscilloscope_display->update_trigger_source ();
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
  
  lab ().m_Oscilloscope.trigger_type (type);
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

  lab ().m_Oscilloscope.trigger_condition (cnd);
}

void LABSoft_Controller_Oscilloscope::
cb_trigger_level (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, 
                  void*            data)
{
  LABSoft_GUI_Label label (
    w->value (),
    lab ().m_Oscilloscope.trigger_level (),
    LABSoft_GUI_Label::UNIT::VOLT
  );

  if (label.is_valid ())
  {
    lab ().m_Oscilloscope.trigger_level (label.actual_value ());

    gui ().oscilloscope_labsoft_gui_oscilloscope_display->
      update_trigger_level ();
  }

  w->value (LABSoft_GUI_Label (lab ().m_Oscilloscope.trigger_level ()). 
    to_label_text (LABSoft_GUI_Label::UNIT::VOLT).c_str ());
}

void LABSoft_Controller_Oscilloscope:: 
cb_trigger_level_slider (LABSoft_GUI_Fl_Slider* w, 
                         void*                  data)
{
  lab ().m_Oscilloscope.trigger_level (w->value ());

  LABSoft_GUI_Label lbl (lab ().m_Oscilloscope.trigger_level ());

  gui ().oscilloscope_fl_input_choice_trigger_level->value (
    lbl.to_label_text (LABSoft_GUI_Label::UNIT::VOLT).c_str ()
  );
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
  gui ().oscilloscope_fl_window_record_config->show ();
  gui ().main_fl_tabs->deactivate ();
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
  gui ().oscilloscope_fl_window_record_config->hide ();
  gui ().main_fl_tabs->activate ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_export (Fl_Menu_Item*  w,
           void*          data)
{

}
    
void LABSoft_Controller_Oscilloscope:: 
display_update_cycle ()
{ 
  LAB_Oscilloscope& osc = lab ().m_Oscilloscope;

  if (osc.is_frontend_running () || osc.is_backend_running ())
  {
    osc.load_data_samples ();

    gui ().oscilloscope_labsoft_gui_oscilloscope_display->update_display ();

    // TO-DO
    // this is not ideal as the trigger_found flag should not be changed
    // outside of LAB_Oscilloscope as much as possible.
    // but in the meantime, i am putting this here for possible performance
    // improvement. with this, the LAB_Oscilloscope does not have to find
    // for the next trigger as long as the current trigger has not yet been displayed

    if (lab ().m_Oscilloscope.trigger_found ())
    {
      lab ().m_Oscilloscope.trigger_serviced ();
    }  
  }
}

void LABSoft_Controller_Oscilloscope:: 
update_trigger_panel_gui ()
{
  if (lab ().m_Oscilloscope.trigger_mode () == LABE::OSC::TRIG::MODE::NONE)
  {
    gui ().oscilloscope_fl_choice_trigger_source->deactivate ();
    gui ().oscilloscope_fl_choice_trigger_type->deactivate ();
    gui ().oscilloscope_fl_choice_trigger_condition->deactivate ();
    gui ().oscilloscope_fl_input_choice_trigger_level->deactivate ();
  }
  else 
  {
    gui ().oscilloscope_fl_choice_trigger_source->activate ();
    gui ().oscilloscope_fl_choice_trigger_type->activate ();
    gui ().oscilloscope_fl_choice_trigger_condition->activate ();
    gui ().oscilloscope_fl_input_choice_trigger_level->activate ();
  }
}

void LABSoft_Controller_Oscilloscope:: 
update_gui_horizontal_elements ()
{
  LABSoft_GUI_Label horizontal_offset  (lab ().m_Oscilloscope.horizontal_offset ());
  LABSoft_GUI_Label time_per_division  (lab ().m_Oscilloscope.time_per_division ());
  LABSoft_GUI_Label samples            (lab ().m_Oscilloscope.samples ());
  LABSoft_GUI_Label sampling_rate      (lab ().m_Oscilloscope.sampling_rate ());

  // 1. horizontal offset
  gui ().oscilloscope_fl_input_choice_with_scroll_horizontal_offset->value (
    horizontal_offset.to_label_text (LABSoft_GUI_Label::UNIT::SECOND).c_str ()
  );

  // 2. time per division
  gui ().oscilloscope_fl_input_choice_with_scroll_time_per_division->value (
    time_per_division.to_label_text (LABSoft_GUI_Label::UNIT::SECOND_PER_DIVISION).c_str ()
  );

  // 3. samples
  char label[20];

  std::snprintf (label, sizeof (label), "%.0f", samples.actual_value ());

  gui ().oscilloscope_fl_input_choice_with_scroll_samples->value (label);
  
  // 4. sampling rate
  gui ().oscilloscope_fl_input_choice_with_scroll_sampling_rate->value (
    sampling_rate.to_label_text (LABSoft_GUI_Label::UNIT::HERTZ).c_str ()
  );

  // 5. display mode
  gui ().oscilloscope_fl_choice_mode->value (
    gui ().oscilloscope_fl_choice_mode->find_index (
      (LABS_GUI_VALUES::OSC::MODE_s.at (lab ().m_Oscilloscope.mode ())).c_str ()
    )
  );
}

void LABSoft_Controller_Oscilloscope:: 
update_gui_mode ()
{
  LABE::OSC::MODE mode = lab ().m_Oscilloscope.mode ();

  if (mode == LABE::OSC::MODE::RECORD)
  {
    gui ().oscilloscope_fl_light_button_run_stop->hide  ();
    gui ().oscilloscope_fl_button_single->hide          ();

    gui ().oscilloscope_fl_button_record->show          ();
    gui ().oscilloscope_fl_button_record_config->show   ();
  }
  else 
  {
    gui ().oscilloscope_fl_light_button_run_stop->show  ();
    gui ().oscilloscope_fl_button_single->show          ();

    gui ().oscilloscope_fl_button_record->hide          ();
    gui ().oscilloscope_fl_button_record_config->hide   ();
  }

  update_gui_horizontal_elements ();

  gui ().oscilloscope_labsoft_gui_oscilloscope_display->
    update_time_per_division ();
}

// EOF