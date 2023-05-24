#include "LABSoft_Controller_Logic_Analyzer.h"

#include "Defaults.h"
#include "LabelValue.h"
#include "LABSoft_Controller.h"

LABSoft_Controller_Logic_Analyzer:: 
LABSoft_Controller_Logic_Analyzer (LAB*                 _LAB, 
                                   LABSoft_GUI*         _LABSoft_GUI,
                                   LABSoft_Controller*  _LABSoft_Controller)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI),
    m_LABSoft_Controller  (_LABSoft_Controller)
{
  // Link the LAB_Logic_Analyzer_Parent_Data struct from LAB_Logic_Analyzer
  // to the LABSoft_Logic_Analyzer_Display_Group class in the GUI
  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display-> 
    load_logan_parent_data (&(m_LAB->m_Logic_Analyzer.m_parent_data));
  
  // reserve() the pixel point vectors in the LAB_Logic_Analyzer_Parent_Data
  // struct 
  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display-> 
    reserve_pixel_points ();
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_run_stop (Fl_Light_Button* w,
            void*             data)
{
  if (w->value () == 0)
  {
    m_LAB->m_Logic_Analyzer.stop ();
  }
  else 
  {
    m_LAB->m_Logic_Analyzer.run ();
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
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Logic_Analyzer.time_per_division (_LabelValue.actual_value (),
    LABSOFT_LOGIC_ANALYZER_DISPLAY_NUMBER_OF_COLUMNS);

  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display->
    update_time_per_division_labels ();
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_horizontal_offset (Fl_Input_Choice *w,
                      void            *data)
{
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Logic_Analyzer.horizontal_offset (_LabelValue.actual_value ());

  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display->
    update_time_per_division_labels ();
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

// EOF