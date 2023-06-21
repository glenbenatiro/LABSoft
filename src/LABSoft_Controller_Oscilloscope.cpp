#include "LABSoft_Controller_Oscilloscope.h"

#include <cmath>
#include <string>
#include <iomanip>

#include <FL/Fl.H>
#include <FL/Fl_Menu_Item.H>

#include "Defaults.h"
#include "LabelValue.h"
#include "LABSoft_Controller.h"

LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LAB*                 _LAB, 
                                 LABSoft_GUI*         _LABSoft_GUI,
                                 LABSoft_Controller*  _LABSoft_Controller)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI), 
    m_LABSoft_Controller  (_LABSoft_Controller)
{
  /**
   * 1. Link the LABSoft_Oscilloscope_Display_Group widgets in the FLUID file
   *    to the LABSoft_Oscilloscope_Display_Group.h/cpp file class. Please 
   *    see the comment I placed for the function below.
   */
  oscilloscope_display_group_init ();

  /**
   * 2. Link the LAB_Oscilloscope_Parent_Data class to the
   *    LABSoft_Oscilloscope_Display_Group. This also links the parent data
   *    class to the LABSoft_Oscilloscope_Display widget.
   */
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    load_osc_parent_data (m_LAB->m_Oscilloscope.m_parent_data);

  /**
   * 3. Now that we already linked the LAB_Oscilloscope_Parent_Data class,
   *    call reserve_pixel_points () so that the pixel point array in the 
   *    data class is reserved to the width of the LABSoft_Oscilloscope_Display
   *    widget width. 
   */
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    reserve_pixel_points ();

  /**
   * 4. In connection with 3, now that the parent data class is linked,
   *    update all display information on the LABSoft_Oscilloscope_Display_Group
   *    widget (voltage per division labels, time per division labels, etc.).
   */
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_all_display_information ();

  /**
   * 5. Call time_per_division () on the LAB_Oscilloscope class to 
   *    update the time per division, now given the number of columns of
   *    the LABSoft_Oscilloscope_Display widget.
   */
  m_LAB->m_Oscilloscope.time_per_division (LABC::OSC::TIME_PER_DIVISION);

  /**
   * 6. Init all GUI values 
   */
   init_gui_values ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_run_stop (Fl_Light_Button* w, 
             void*            data)
{
  bool value = w->value ();

  if (value)
  {
    m_LAB->m_Oscilloscope.run ();

    m_LABSoft_Controller->m_Voltmeter.update_gui_main         (false);
    m_LABSoft_GUI->voltmeter_fl_light_button_run_stop->clear  ();
  }
  else 
  {
    m_LAB->m_Oscilloscope.stop ();
  }

  update_gui_main (value);
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

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_gui_vertical_elements ();
}

void LABSoft_Controller_Oscilloscope::
cb_ac_coupling (Fl_Light_Button*  w, 
                long              channel)
{
  bool value = w->value ();

  m_LAB->m_Oscilloscope.coupling (channel, value ? 
    LABE::OSC::COUPLING::AC : LABE::OSC::COUPLING::DC);
}

void LABSoft_Controller_Oscilloscope::
cb_voltage_per_division (Fl_Input_Choice* w, 
                        long              channel)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.voltage_per_division (channel),
    LabelValue::UNIT::VOLT
  );

  if (lv.is_valid ())
  {
    if (LABF::is_within_range (lv.actual_value (), 
      LABC::OSC::MIN_VOLTAGE_PER_DIVISION,
      LABC::OSC::MAX_VOLTAGE_PER_DIVISION))
    {
      m_LAB->m_Oscilloscope.voltage_per_division (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Oscilloscope.voltage_per_division (channel)).
    to_label_text (LabelValue::UNIT::VOLT).c_str ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_gui_vertical_elements ();
}

void LABSoft_Controller_Oscilloscope::
cb_vertical_offset (Fl_Input_Choice* w, 
                    long             channel)
{ 
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.vertical_offset (channel),
    LabelValue::UNIT::VOLT
  );

  if (lv.is_valid ())
  {
    if (LABF::is_within_range (lv.actual_value (),
      LABC::OSC::MIN_VERTICAL_OFFSET,
      LABC::OSC::MAX_VERTICAL_OFFSET))
    {
      m_LAB->m_Oscilloscope.vertical_offset (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Oscilloscope.vertical_offset (channel)).
    to_label_text (LabelValue::UNIT::VOLT).c_str ());
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
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
cb_scaling (Fl_Choice *w,
            long       channel)
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

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_gui_voltage_per_division ();
}

// Horizontal
void LABSoft_Controller_Oscilloscope::
cb_horizontal_offset (Fl_Input_Choice* w, 
                      void*            data)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.horizontal_offset (),
    LabelValue::UNIT::SECOND
  );

  if (lv.is_valid ())
  {
    if (LABF::is_within_range (lv.actual_value (), 
      LABC::OSC::MIN_HORIZONTAL_OFFSET,
      LABC::OSC::MAX_HORIZONTAL_OFFSET))
    {
      m_LAB->m_Oscilloscope.horizontal_offset (lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Oscilloscope.horizontal_offset ()).
    to_label_text (LabelValue::UNIT::SECOND).c_str ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_gui_time_per_division ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_samples (Fl_Input_Choice*  w,
            void*             data)
{
  LabelValue lv (w->value (), LabelValue::UNIT::NONE);

  if (lv.is_valid ())
  {
    m_LAB->m_Oscilloscope.samples (std::round (lv.actual_value ()));
  }

  update_gui_horizontal ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division (Fl_Input_Choice* w,
                      void*            data)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.time_per_division (),
    LabelValue::UNIT::SECOND_PER_DIVISION
  );

  if (lv.is_valid ())
  {
    m_LAB->m_Oscilloscope.time_per_division (lv.actual_value ());
  }

  update_gui_horizontal ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_sampling_rate (Fl_Input_Choice*  w,
                  void*             data)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.sampling_rate (),
    LabelValue::UNIT::HERTZ
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

    m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
      update_gui_time_per_division ();
    
    m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
      update_gui_upper_left_info ();
  }
}

void LABSoft_Controller_Oscilloscope:: 
cb_channel_selector (Fl_Toggle_Button*  w,
                     long               channel)
{
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    channel_selector (channel);
}

void LABSoft_Controller_Oscilloscope::
cb_display (LABSoft_Oscilloscope_Display* w, 
            void*                         data)
{
  switch (Fl::event ())
  {
    case (FL_MOUSEWHEEL):
    {
      int direction = Fl::event_dy ();

      double time_per_division = m_LAB->m_Oscilloscope.time_per_division () * 
        ((direction > 0) ? 2.0 : 0.5);

      m_LAB->m_Oscilloscope.time_per_division (time_per_division);
  

      break;
    }

    case (FL_PUSH):
    {
      LABSoft_Oscilloscope_Display& disp = *(m_LABSoft_GUI->
        oscilloscope_labsoft_oscilloscope_display_display);
      
      disp.mouse_down_start (Fl::event_x (), Fl::event_y ());

      disp.pre_drag_horizontal_offset (m_LAB->m_Oscilloscope.horizontal_offset ());
    }

    case (FL_DRAG):
    {
      LABSoft_Oscilloscope_Display& disp = *(m_LABSoft_GUI->
        oscilloscope_labsoft_oscilloscope_display_display);

      int     scaler      = disp.calc_time_per_div_scaler (Fl::event_x ());

      double  scaler_time = scaler * (m_LAB->m_Oscilloscope.time_per_division () / 
                              LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);

      double new_horizontal_offset = disp.pre_drag_horizontal_offset () + scaler_time;

      if (LABF::compare_double (new_horizontal_offset, 0.0))
      {
        new_horizontal_offset = 0.0;
      }

      m_LAB->m_Oscilloscope.horizontal_offset (new_horizontal_offset);

      break;
    }
  }

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
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.trigger_level (),
    LabelValue::UNIT::VOLT
  );

  if (lv.is_valid ())
  {
    if (LABF::is_within_range (lv.actual_value (),
      LABC::OSC::MIN_TRIGGER_LEVEL,
      LABC::OSC::MAX_TRIGGER_LEVEL))
    {
      m_LAB->m_Oscilloscope.trigger_level (lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Oscilloscope.trigger_level ()).
    to_label_text (LabelValue::UNIT::VOLT).c_str ());
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

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    fill_pixel_points ();

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    redraw ();

  if (m_LAB->m_Oscilloscope.m_parent_data.trig_found)
  {
    m_LAB->m_Oscilloscope.m_parent_data.trig_found = false;
  }
}

void LABSoft_Controller_Oscilloscope:: 
update_gui_main (bool value)
{
  m_LABSoft_Controller->tab_selection_color_toggle (
    m_LABSoft_GUI->main_fl_group_oscilloscope_tab,
    value
  );
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
  LabelValue horizontal_offset  (m_LAB->m_Oscilloscope.horizontal_offset ());
  LabelValue time_per_division  (m_LAB->m_Oscilloscope.time_per_division ());
  LabelValue samples            (m_LAB->m_Oscilloscope.samples ());
  LabelValue sampling_rate      (m_LAB->m_Oscilloscope.sampling_rate ());

  // 1. Upper Display Info
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_gui_upper_left_info ();

  // 2. Horizontal Offset
  m_LABSoft_GUI->oscilloscope_fl_input_choice_horizontal_offset->value (
    horizontal_offset.to_label_text (LabelValue::UNIT::SECOND).c_str ()
  );

  // 3. Time per Division
  m_LABSoft_GUI->oscilloscope_fl_input_choice_time_per_division->value (
    time_per_division.to_label_text (LabelValue::UNIT::SECOND_PER_DIVISION).c_str ()
  );

  // 4. Samples
  m_LABSoft_GUI->oscilloscope_fl_input_choice_samples->value (
    samples.to_label_text (LabelValue::UNIT::NONE, 3).c_str ()
  );

  // 5. Sampling Rate
  m_LABSoft_GUI->oscilloscope_fl_input_choice_sampling_rate->value (
    sampling_rate.to_label_text (LabelValue::UNIT::HERTZ).c_str ()
  );

  // 6. Display Mode
  m_LABSoft_GUI->oscilloscope_fl_choice_mode->value (
    m_LABSoft_GUI->oscilloscope_fl_choice_mode->find_index (
      (GUI_LABEL::OSC::MODE.at (m_LAB->m_Oscilloscope.mode ())).c_str ()
    )
  );

  // 7. Time per Division Labels
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_gui_time_per_division ();
}

/**
 * This is needed as the whole definition of the
 * LABSoft_Oscilloscope_Display_Group class is split into two: inside the 
 * LABSoft_Oscilloscope_Display_Group.h/cpp file, and in the 
 * FLUID file. I placed the other half in FLUID because I want to
 * visualize how the overall look of the oscilloscope display and display group
 * would look like. I also wanted to make resizing and repositioning of the 
 * major widgets easier, instead of manually changing in code, compiling, 
 * then actually running LABSoft to see the changes.
 * 
 * Basically this function links and repositions the other widgets part of the 
 * LABSoft_Oscilloscope_Display_Group, which were defined in the FLUID file,
 * to the LABSoft_Oscilloscope_Display_Group.h/cpp file class.
*/
void LABSoft_Controller_Oscilloscope::
oscilloscope_display_group_init ()
{
  LABSoft_Oscilloscope_Display_Group& group = *(m_LABSoft_GUI->
    oscilloscope_labsoft_oscilloscope_display_group_display);
    
  // 1. Link widgets from the FLUID file to the 
  //    LABSoft_Oscilloscope_Display_Group class
  group.m_display               = m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_display;
  group.m_display_status        = m_LABSoft_GUI->oscilloscope_fl_box_display_status;
  group.m_upper_info_display    = m_LABSoft_GUI->oscilloscope_fl_box_upper_info_display;
  group.m_horizontal_offset     = m_LABSoft_GUI->oscilloscope_fl_slider_horizontal_offset;
  group.m_vertical_offset       = m_LABSoft_GUI->oscilloscope_fl_slider_vertical_offset;
  group.m_trigger_level         = m_LABSoft_GUI->oscilloscope_fl_slider_trigger_level;
  group.m_channel_selectors[0]  = m_LABSoft_GUI->oscilloscope_fl_toggle_button_c1_selector;
  group.m_channel_selectors[1]  = m_LABSoft_GUI->oscilloscope_fl_toggle_button_c2_selector;
  
  // 2. Initialize slider bounds and value
  group.m_horizontal_offset->bounds (0, group.m_display->w ());
  group.m_horizontal_offset->value  (group.m_display->w () / 2.0);
  group.m_horizontal_offset->step   (group.m_display->w (), 
                                      LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS * 
                                      LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);
  
  group.m_vertical_offset->bounds   (0, group.m_display->h ());
  group.m_vertical_offset->value    (group.m_display->h () / 2.0);
  group.m_vertical_offset->step     (group.m_display->h (),
                                      LABC::OSC_DISPLAY::NUMBER_OF_ROWS * 
                                      LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);

  group.m_trigger_level->bounds     (0, group.m_display->h ());
  group.m_trigger_level->value      (group.m_display->h () / 2.0);

  // 3. Reposition y-axis labels and label units
  LABSoft_Oscilloscope_Display& disp = *(group.m_display);
  
  float row_height = (static_cast<float>(disp.h ())) / 
    LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_ROWS;

  for (int chan = 0; chan < group.m_y_labels.size (); chan++)
  {
    int x = (disp.x ()) - 
      (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::Y_LABEL_INTERSPACE * (chan + 1));

    for (int row = 0; row < group.m_y_labels[chan].size (); row++)
    {
      int y = (disp.y ()) + (std::round (row_height * row));

      group.m_y_labels[chan][row]->position (x, y);
    }

    int x_label_unit = x + 
      LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::Y_LABEL_UNIT_LEFT_MARGIN;
    int y_label_unit = (disp.y ()) -
      LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::Y_LABEL_UNIT_BOTTOM_MARGIN;

    group.m_y_label_units[chan]->position (x_label_unit, y_label_unit);
  }

  // 4. Reposition x-axis labels 
  float column_width = (static_cast<float>(disp.w ())) / 
    LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS;
  
  int y = (disp.x () + disp.h ()) + 
    LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::X_LABEL_INTRASPACE;

  for (int i = 0; i < group.m_x_labels.size (); i++)
  {
    int x = (disp.x ()) + (column_width * i);

    // The rightmost x-label could be a bit to the left to avoid clipping
    if (i == group.m_x_labels.size () - 1)
      x -= 5;

    group.m_x_labels[i]->position (x, y);
  }  
}

void LABSoft_Controller_Oscilloscope:: 
init_gui_values ()
{
  // --- Mode ---

  // 1. Mode
  Fl_Choice* w = m_LABSoft_GUI->oscilloscope_fl_choice_mode;

  w->value (w->find_index (GUI_LABEL::OSC::MODE.at (m_LAB->
    m_Oscilloscope.mode ()).c_str ()));

  // --- Vertical ---
  // Channel 0
  m_LAB->m_Oscilloscope.m_parent_data.channel_data[0].coupling == LABE::OSC::COUPLING::AC ? 
    m_LABSoft_GUI->oscilloscope_fl_light_button_channel_0_ac_coupling->set () :
    m_LABSoft_GUI->oscilloscope_fl_light_button_channel_0_ac_coupling->clear ();
  
  // Channel 1
  m_LAB->m_Oscilloscope.m_parent_data.channel_data[1].coupling == LABE::OSC::COUPLING::AC ? 
    m_LABSoft_GUI->oscilloscope_fl_light_button_channel_0_ac_coupling->set () :
    m_LABSoft_GUI->oscilloscope_fl_light_button_channel_0_ac_coupling->clear ();

  // --- Horizontal ---
  m_LABSoft_GUI->oscilloscope_fl_input_choice_horizontal_offset->value (
    LabelValue (m_LAB->m_Oscilloscope.m_parent_data.horizontal_offset,
      LabelValue::UNIT::SECOND).to_label_text ().c_str ()
  );

  m_LABSoft_GUI->oscilloscope_fl_input_choice_time_per_division->value (
    LabelValue (m_LAB->m_Oscilloscope.m_parent_data.time_per_division,
      LabelValue::UNIT::SECOND_PER_DIVISION).to_label_text ().c_str ()
  );

  m_LABSoft_GUI->oscilloscope_fl_input_choice_samples->value (
    LabelValue (m_LAB->m_Oscilloscope.m_parent_data.samples,
      LabelValue::UNIT::NONE).to_label_text ().c_str ()
  );

  m_LABSoft_GUI->oscilloscope_fl_input_choice_sampling_rate->value (
    LabelValue (m_LAB->m_Oscilloscope.m_parent_data.sampling_rate,
      LabelValue::UNIT::HERTZ).to_label_text ().c_str ()
  );
}

// EOF