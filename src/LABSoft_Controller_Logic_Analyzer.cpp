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
    m_LAB->m_Logic_Analyzer.run ();
    
    w->label ("Stop");
  }
  else 
  {
    m_LAB->m_Logic_Analyzer.stop ();

    w->label ("Run");
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

void LABSoft_Controller_Logic_Analyzer:: 
update_display ()
{
  while (m_LAB->m_Logic_Analyzer.is_running ()) 
  {
    // auto start = std::chrono::steady_clock::now ();
    printf (".\n");

    m_LAB->m_Logic_Analyzer.load_data_samples ();

    // m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    //  load_channel_signals (&(m_LAB->m_Oscilloscope->m_channel_signals));

    // // draw signals
    // m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->display ()->redraw ();
    // Fl::awake ();

    // std::this_thread::sleep_for (std::chrono::milliseconds 
    //   (DISPLAY_UPDATE_SLEEP_TIME_MS));

    // auto end = std::chrono::steady_clock::now ();

    // std::chrono::duration<double, std::milli> elapsed = end - start;
    // std::cout << "Duration: " << elapsed.count () << " ms\n";
  }
}

// EOF