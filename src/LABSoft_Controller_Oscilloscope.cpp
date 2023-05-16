#include "LABSoft_Controller_Oscilloscope.h"

#include <iostream>
#include <string>
#include <iomanip>

#include <FL/Fl.H>

#include "Defaults.h"

LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

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
   * 5. Lastly, call time_per_division () on the LAB_Oscilloscope class to 
   *    update the time per division, now given the number of columns of
   *    the LABSoft_Oscilloscope_Display widget.
   */
  m_LAB->m_Oscilloscope.time_per_division (LABC::OSC::TIME_PER_DIVISION,
    LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS);
}

void LABSoft_Controller_Oscilloscope:: 
cb_run_stop (Fl_Light_Button *w, 
             void            *data)
{
  if (w->value () == 0)
  {
    m_LAB->m_Oscilloscope.stop ();
  }
  else 
  {
    m_LAB->m_Oscilloscope.run ();

    // 
    m_LABSoft_GUI->voltmeter_fl_light_button_run_stop->clear ();
  }
}

void LABSoft_Controller_Oscilloscope:: 
cb_channel_enable_disable (Fl_Light_Button* w,
                           long             channel)
{
  bool value = w->value ();

  m_LAB->m_Oscilloscope.channel_enable_disable (channel, value);

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_voltage_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_horizontal_offset (Fl_Input_Choice* w, 
                      void*            data)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.horizontal_offset (),
    LabelValue::TYPE::SECONDS
  );

  if (lv.is_valid_label_text ())
  {
    if (LAB_DEFAULTS::isWithinRange (lv.actual_value (), 
      LABC::OSC::MIN_HORIZONTAL_OFFSET,
      LABC::OSC::MAX_HORIZONTAL_OFFSET))
    {
      m_LAB->m_Oscilloscope.horizontal_offset (lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Oscilloscope.horizontal_offset ()).
    to_label_text (LabelValue::TYPE::SECONDS).c_str ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_time_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_voltage_per_division (Fl_Input_Choice* w, 
                        long              channel)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.voltage_per_division (channel),
    LabelValue::TYPE::VOLTS
  );

  if (lv.is_valid_label_text ())
  {
    if (LAB_DEFAULTS::isWithinRange (lv.actual_value (), 
      LABC::OSC::MIN_VOLTAGE_PER_DIVISION,
      LABC::OSC::MAX_VOLTAGE_PER_DIVISION))
    {
      m_LAB->m_Oscilloscope.voltage_per_division (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Oscilloscope.voltage_per_division (channel)).
    to_label_text (LabelValue::TYPE::VOLTS).c_str ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_voltage_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_vertical_offset (Fl_Input_Choice* w, 
                    long             channel)
{ 
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.vertical_offset (channel),
    LabelValue::TYPE::VOLTS
  );

  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LABC::OSC::MIN_VERTICAL_OFFSET &&
      lv.actual_value () <= LABC::OSC::MAX_VERTICAL_OFFSET)
    {
      m_LAB->m_Oscilloscope.vertical_offset (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Oscilloscope.vertical_offset (channel)).
    to_label_text (LabelValue::TYPE::VOLTS).c_str ());
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_voltage_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_ac_coupling (Fl_Light_Button *w, 
                long             channel)
{
  m_LAB->m_Oscilloscope.coupling (static_cast<unsigned>(channel),
    (w->value () == 0) ? LABE::OSC::COUPLING::DC : LABE::OSC::COUPLING::AC);
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

  m_LAB->m_Oscilloscope.scaling (static_cast<unsigned>(channel), scaling);
}

void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division (Fl_Input_Choice* w,
                      void*            data)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.time_per_division (),
    LabelValue::TYPE::SECONDS
  );
  
  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LABC::OSC::MIN_TIME_PER_DIVISION &&
      lv.actual_value () <= LABC::OSC::MAX_TIME_PER_DIVISION)
    {
      // Backend
      m_LAB->m_Oscilloscope.time_per_division (
        lv.actual_value (), 
        LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS
      );
    }
  }

  // Frontend
  update_horizontal_widgets_gui ();

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_upper_osc_disp_info ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_sampling_rate (Fl_Input_Choice* w, 
                  void*            data)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Oscilloscope.sampling_rate (),
    LabelValue::TYPE::HERTZ
  );
  
  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LABC::OSC::MIN_SAMPLING_RATE &&
      lv.actual_value () <= LABC::OSC::MAX_SAMPLING_RATE)
    {
      // Backend
      m_LAB->m_Oscilloscope.sampling_rate (
        lv.actual_value (), 
        LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS
      );
    }
  }

  // Frontend
  update_horizontal_widgets_gui ();

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_upper_osc_disp_info ();
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
    LabelValue::TYPE::VOLTS
  );

  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LABC::OSC::MIN_TRIGGER_LEVEL &&
      lv.actual_value () <= LABC::OSC::MAX_TRIGGER_LEVEL)
    {
      m_LAB->m_Oscilloscope.trigger_level (lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Oscilloscope.trigger_level ()).
    to_label_text (LabelValue::TYPE::VOLTS).c_str ());
}

void LABSoft_Controller_Oscilloscope::
cb_display_mode (Fl_Choice* w,  
                 void*      data)
{
  std::string str (w->text ());
  LE::DISPLAY_MODE disp_mode;

  if (str == "Repeated")
  {
    disp_mode = LE::DISPLAY_MODE::REPEATED;
  }
  else if (str == "Screen")
  {
    disp_mode = LE::DISPLAY_MODE::SCREEN;
  }
  else 
  {
    return;
  }

  m_LAB->m_Oscilloscope.display_mode_frontend (disp_mode);

  // Frontend
  update_horizontal_widgets_gui ();

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_upper_osc_disp_info ();
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
update_horizontal_widgets_gui ()
{
  LabelValue time_per_division  (m_LAB->m_Oscilloscope.time_per_division ());
  LabelValue sampling_rate      (m_LAB->m_Oscilloscope.sampling_rate ());

  // Time per Division
  m_LABSoft_GUI->oscilloscope_fl_input_choice_time_per_division->value (
    time_per_division.to_label_text (LabelValue::TYPE::SECONDS).c_str ()
  );

  // Sampling Rate
  m_LABSoft_GUI->oscilloscope_fl_input_choice_sampling_rate->value (
    sampling_rate.to_label_text (LabelValue::TYPE::HERTZ).c_str ()
  );

  // Display Mode
  m_LABSoft_GUI->oscilloscope_fl_choice_display_mode->value (
    m_LABSoft_GUI->oscilloscope_fl_choice_display_mode->find_index (
      (GUI_LBL::DISPLAY_MODE.at (m_LAB->m_Oscilloscope.display_mode ())).c_str ()
    )
  );
}

void LABSoft_Controller_Oscilloscope:: 
display_update_cycle ()
{
  m_LAB->m_Oscilloscope.load_data_samples ();

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    fill_pixel_points ();

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    redraw ();
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
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    m_display_status = m_LABSoft_GUI->oscilloscope_fl_box_display_status;

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    m_upper_info_display = m_LABSoft_GUI->oscilloscope_fl_box_upper_info_display;

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    m_display = m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_display;


  // Reposition y-axis labels and label units
  LABSoft_Oscilloscope_Display_Group& group = *(m_LABSoft_GUI->
    oscilloscope_labsoft_oscilloscope_display_group_display);
  
  LABSoft_Oscilloscope_Display& disp = *(m_LABSoft_GUI->
    oscilloscope_labsoft_oscilloscope_display_display);
    
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

  // Reposition x-axis labels 
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

// EOF