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
  // Link the LAB_Logic_Analyzer_Parent_Data struct from LAB_Logic_Analyzer
  // to the LABSoft_GUI_Logic_Analyzer_Display_Group class in the GUI
  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display-> 
    load_logan_parent_data (m_LAB->m_Logic_Analyzer.m_parent_data);
  
  // reserve() the pixel point vectors in the LAB_Logic_Analyzer_Parent_Data
  // struct 
  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display-> 
    reserve_pixel_points ();
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
cb_memory_depth (Fl_Input_Choice *w,
                 void            *data)
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

  m_LAB->m_Logic_Analyzer.time_per_division (lv.actual_value (),
    LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS);

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
display_update_cycle ()
{
  m_LAB->m_Logic_Analyzer.load_data_samples ();

  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display-> 
    fill_pixel_points ();

  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display-> 
    redraw ();
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