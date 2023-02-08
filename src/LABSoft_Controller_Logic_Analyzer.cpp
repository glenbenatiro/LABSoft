#include "LABSoft_Controller_Logic_Analyzer.h"

#include "Defaults.h"

LABSoft_Controller_Logic_Analyzer:: 
LABSoft_Controller_Logic_Analyzer (LAB         *_LAB, 
                                   LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

  // Load default values
  m_LABSoft_GUI->logic_analyzer_fl_input_choice_memory_depth->
    value (LABSOFT_LOGIC_ANALYZER_MEMORY_DEPTH);
  m_LABSoft_GUI->logic_analyzer_fl_input_choice_sample_rate->
    value (LABSOFT_LOGIC_ANALYZER_SAMPLE_RATE);
  m_LABSoft_GUI->logic_analyzer_fl_input_choice_time_per_division->
    value (LABSOFT_LOGIC_ANALYZER_TIME_PER_DIVISON);
  m_LABSoft_GUI->logic_analyzer_fl_input_choice_position->
    value (LABSOFT_LOGIC_ANALYZER_POSITION);

}

void LABSoft_Controller_Logic_Analyzer:: 
run_display ()
{

}

void LABSoft_Controller_Logic_Analyzer:: 
cb_master_run_stop (Fl_Light_Button *w,
                    void            *data)
{
  if (w->value () == 1)
  {
    // backend
    m_LAB->m_Logic_Analyzer->master_run ();

    // frontend
  }
  else 
  {
    // backend
    m_LAB->m_Logic_Analyzer->master_stop ();

    // frontend
  }
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_memory_depth (Fl_Input_Choice *w,
                 void            *data)
{
  LabelValue _LabelValue (w->value ());
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_time_per_division (Fl_Input_Choice *w,
                      void            *data)
{
  LabelValue _LabelValue (w->value ());

  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display->
    update_time_per_division_labels (_LabelValue.actual_value ());
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_position (Fl_Input_Choice *w,
             void            *data)
{
  LabelValue _LabelValue (w->value ());

  m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display->
    position (_LabelValue.actual_value ());

    m_LABSoft_GUI->logic_analyzer_labsoft_logic_analyzer_display_group_display->
    update_time_per_division_labels ();
}

