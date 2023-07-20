#include "LABSoft_Controller_Logic_Analyzer.h"

#include "LABSoft_Controller.h"
#include "../Utility/LAB_Constants.h"
#include "../Utility/LAB_LabelValue.h"

LABSoft_Controller_Logic_Analyzer:: 
LABSoft_Controller_Logic_Analyzer (LAB*                 _LAB, 
                                   LABSoft_GUI*         _LABSoft_GUI,
                                   LABSoft_Controller*  _LABSoft_Controller)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI),
    m_LABSoft_Controller  (_LABSoft_Controller)
{
  init ();
}

void LABSoft_Controller_Logic_Analyzer:: 
init ()
{
  LABSoft_GUI& gui = *m_LABSoft_GUI;

  LABSoft_GUI_Logic_Analyzer_Display_Group& group = 
    *(gui.logic_analyzer_labsoft_logic_analyzer_display_group_display);
  
  LAB_Logic_Analyzer& logan = m_LAB->m_Logic_Analyzer;

  // 1.
  group.load_logan_parent_data (logan.m_parent_data);

  // 2. 
  group.reserve_pixel_points ();

  // 3. 
  init_gui_values ();  
}

void LABSoft_Controller_Logic_Analyzer:: 
init_gui_values ()
{
  LAB_Logic_Analyzer& logan = m_LAB->m_Logic_Analyzer;
  LABSoft_GUI&        gui   = *m_LABSoft_GUI;  

  // Horizontal
  gui.logic_analyzer_fl_input_choice_horizontal_offset->value (
    LAB_LabelValue (logan.horizontal_offset ()).to_label_text (
    LAB_LabelValue::UNIT::SECOND).c_str ()
  );

  gui.logic_analyzer_fl_input_choice_time_per_division->value (
    LAB_LabelValue (logan.time_per_division ()).to_label_text (
    LAB_LabelValue::UNIT::SECOND_PER_DIVISION).c_str ()
  );

  gui.logic_analyzer_fl_input_choice_samples->value (
    LAB_LabelValue (logan.samples ()).to_label_text (
    LAB_LabelValue::UNIT::NONE).c_str ()
  );

  gui.logic_analyzer_fl_input_choice_sampling_rate->value (
    LAB_LabelValue (logan.sampling_rate ()).to_label_text (
    LAB_LabelValue::UNIT::HERTZ).c_str ()
  );
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_run_stop (Fl_Light_Button* w,
             void*            data)
{
  if (w->value () == 0)
  {
    m_LAB->m_Logic_Analyzer.stop ();
    update_gui_main (false);
  }
  else 
  {
    m_LAB->m_Logic_Analyzer.run ();
    update_gui_main (true);
  }
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_single (Fl_Button* w,
           void*      data)
{
  
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_samples (Fl_Input_Choice*  w,
            void*             data)
{

}

void LABSoft_Controller_Logic_Analyzer:: 
cb_sampling_rate (Fl_Input_Choice *w,
                 void            *data)
{
  
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_time_per_division (Fl_Input_Choice *w,
                      void            *data)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Logic_Analyzer.time_per_division (),
    LAB_LabelValue::UNIT::SECOND_PER_DIVISION
  );

  m_LAB->m_Logic_Analyzer.time_per_division (lv.actual_value ());

  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display->
    update_gui_time_per_division ();
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_horizontal_offset (Fl_Input_Choice *w,
                      void            *data)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Logic_Analyzer.horizontal_offset (),
    LAB_LabelValue::UNIT::SECOND
  );

  m_LAB->m_Logic_Analyzer.horizontal_offset (lv.actual_value ());

  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display->
    update_gui_time_per_division ();
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_display_mode (Fl_Choice  *w,
                 void       *data)
{

}

void LABSoft_Controller_Logic_Analyzer:: 
cb_trigger_mode (Fl_Choice* w, 
                 void*      data)
{
  LABE::LOGAN::TRIG::MODE mode;
  std::string choice (w->text ());

  if (choice == "None")
  {
    mode = LABE::LOGAN::TRIG::MODE::NONE;
  }
  else if (choice == "Auto")
  { 
    mode = LABE::LOGAN::TRIG::MODE::AUTO;
  }
  else if (choice == "Normal")
  {
    mode = LABE::LOGAN::TRIG::MODE::NORMAL;
  }
  else 
  {
    throw (std::runtime_error ("Invalid trigger mode input."));
  }

  m_LAB->m_Logic_Analyzer.trigger_mode (mode);
}

void LABSoft_Controller_Logic_Analyzer::
display_update_cycle ()
{
  // 1. 
  m_LAB->m_Logic_Analyzer.fill_channel_samples_buffer ();

  // 2.
  LABSoft_GUI_Logic_Analyzer_Display_Group& display = *(m_LABSoft_GUI->
    logic_analyzer_labsoft_logic_analyzer_display_group_display);

  display.fill_pixel_points ();

  // 3.
  display.redraw ();
}

void LABSoft_Controller_Logic_Analyzer:: 
update_gui_main (bool value)
{
  m_LABSoft_Controller->tab_selection_color_toggle (
    m_LABSoft_GUI->main_fl_group_logic_analyzer_tab,
    value
  );
}

// EOF