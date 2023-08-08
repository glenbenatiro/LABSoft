#include "LABSoft_Controller_Logic_Analyzer.h"

#include "LABSoft_Controller.h"
#include "../Utility/LAB_Constants.h"
#include "../Utility/LAB_LabelValue.h"
#include "../Utility/LABSoft_GUI_Label_Values.h"

LABSoft_Controller_Logic_Analyzer:: 
LABSoft_Controller_Logic_Analyzer (LAB*                 _LAB, 
                                   LABSoft_GUI*         _LABSoft_GUI,
                                   LABSoft_Controller*  _LABSoft_Controller)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI),
    m_LABSoft_Controller  (_LABSoft_Controller)
{
  init ();
  init_gui_values ();
}

void LABSoft_Controller_Logic_Analyzer:: 
init ()
{
  m_LABSoft_GUI->logic_analyzer_labsoft_gui_logic_analyzer_display->
    load_logic_analyzer_parent_data (m_LAB->m_Logic_Analyzer.parent_data ());
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

  // Display
  gui.logic_analyzer_labsoft_gui_logic_analyzer_add_channel_signal_window->
    number_of_channels (LABC::LOGAN::NUMBER_OF_CHANNELS);

  for (unsigned chan = 0; chan < LABC::LOGAN::NUMBER_OF_CHANNELS; chan++)
  {
    gui.logic_analyzer_labsoft_gui_logic_analyzer_display->add_channel (chan);
  }
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

  m_LABSoft_GUI->logic_analyzer_labsoft_gui_logic_analyzer_display->
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
  std::string choice (w->text ());

  m_LAB->m_Logic_Analyzer.trigger_mode 
    (LABS_GUI_VALUES::LOGAN::TRIG_MODE_s[choice]);
}

void LABSoft_Controller_Logic_Analyzer:: 
cb_trigger_condition (Fl_Menu_Button* w,
                      void*           data)
{
  ChanWidget* chan    = static_cast<ChanWidget*>(data);
  unsigned    channel = chan->channel ();

  LABE::LOGAN::TRIG::CND trig_cnd = 
    LABS_GUI_VALUES::LOGAN_DISPLAY::TRIG_CND_s.at (std::string (w->text ()));

  m_LAB->m_Logic_Analyzer.trigger_condition (channel, trig_cnd);
}

void LABSoft_Controller_Logic_Analyzer::
display_update_cycle ()
{
  if (m_LAB->m_Logic_Analyzer.is_running ())
  {
    m_LAB->m_Logic_Analyzer.load_data_samples ();
  }

  m_LABSoft_GUI->logic_analyzer_labsoft_gui_logic_analyzer_display->
    update_display ();
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
  m_LABSoft_GUI->logic_analyzer_labsoft_gui_logic_analyzer_display->clear_all_channels ();
}

// EOF