#include "LABSoft_Controller_Logic_Analyzer.h"

#include "LABSoft_Controller.h"
#include "../Utility/LAB_Constants.h"
#include "../Utility/LAB_LabelValue.h"

// delete soon
#include <iostream>

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
  LABSoft_GUI&                        gui     = *m_LABSoft_GUI;
  LAB_Logic_Analyzer&                 logan   = m_LAB->m_Logic_Analyzer;
  LABSoft_GUI_Logic_Analyzer_Display& display = 
    *(gui.logic_analyzer_labsoft_gui_logic_analyzer_display);  

  // 1.
  display.load_logic_analyzer_parent_data (logan.m_parent_data);

  // 2. 
  display.reserve_pixel_points ();

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

  gui.logic_analyzer_labsoft_gui_logic_analyzer_add_channel_signal_window->
    number_of_channels (LABC::LOGAN::NUMBER_OF_CHANNELS);
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

  m_LABSoft_GUI->logic_analyzer_labsoft_gui_logic_analyzer_display->
    update_gui_time_per_division_labels ();
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

  m_LABSoft_GUI->logic_analyzer_labsoft_gui_logic_analyzer_display->
    update_gui_time_per_division_labels ();
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
  LABSoft_GUI_Logic_Analyzer_Display& display = *(m_LABSoft_GUI->
    logic_analyzer_labsoft_gui_logic_analyzer_display);

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

void LABSoft_Controller_Logic_Analyzer:: 
cb_add_channel_selection (Fl_Menu_* w, void* data)
{
  m_LABSoft_GUI->logic_analyzer_labsoft_gui_logic_analyzer_add_channel_signal_window
    ->show_as_modal ();
}

void LABSoft_Controller_Logic_Analyzer::
cb_add_channel_signal (LABSoft_GUI_Logic_Analyzer_Add_Channel_Signal_Window* w, void* data)
{
  Fl_Input&         i = *(w->m_name);
  Fl_Multi_Browser& b = *(w->m_multi_browser);

  LABSoft_GUI_Logic_Analyzer_Display& disp = 
    *(m_LABSoft_GUI->logic_analyzer_labsoft_gui_logic_analyzer_display);

  char      label[20];
  unsigned  added_count = 0;

  for (unsigned line = b.size (); line > 0; line--)
  {
    if ((b.selected (line)) == 1)
    {
      // create label
      if (*(i.value ()) == '\0')
      {
        std::snprintf (label, sizeof (label), "DIO %d", b.data (line));
      }
      else 
      {
        if (added_count == 0)
        {
          std::snprintf (label, sizeof (label), "%s", i.value ());
        }
        else 
        {
          std::snprintf (label, sizeof (label), "%s%d", i.value (), added_count + 1);
        }
      }

      // add channel
      disp.add_channel (reinterpret_cast<unsigned>(b.data (line)), label);

      // increment
      added_count++;
    }
  }
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_clear_channels (Fl_Menu_* w, void* data)
{
  m_LABSoft_GUI->logic_analyzer_labsoft_gui_logic_analyzer_display->clear_channels ();
}

// EOF