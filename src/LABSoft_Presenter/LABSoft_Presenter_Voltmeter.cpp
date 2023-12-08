#include "LABSoft_Presenter_Voltmeter.h"

#include "../LAB/LAB.h"
#include "LABSoft_Presenter.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"
#include "../Utility/LABSoft_GUI_Label.h"

LABSoft_Presenter_Voltmeter:: 
LABSoft_Presenter_Voltmeter (LABSoft_Presenter& _LABSoft_Presenter)
  : LABSoft_Presenter_Unit (_LABSoft_Presenter)
{
  init_gui_values ();
}

void LABSoft_Presenter_Voltmeter:: 
init_gui_values ()
{
  LABSoft_GUI& gui = m_presenter.gui ();

  gui.voltmeter_fl_output_chan_0_value->value ("- . - -");
  gui.voltmeter_fl_output_chan_1_value->value ("- . - -");
}

void LABSoft_Presenter_Voltmeter:: 
cb_run_stop  (Fl_Light_Button* w, 
              void*            data)
{
  if (w->value ())
  {
    gui ().oscilloscope_fl_light_button_run_stop->clear ();

    lab ().m_Voltmeter.run ();
  }
  else 
  {
    lab ().m_Voltmeter.stop ();
  }
}

void LABSoft_Presenter_Voltmeter:: 
display_update_cycle ()
{
  if (lab ().m_Voltmeter.is_running ())
  {
    LABSoft_GUI_Label chan0 (lab ().m_Voltmeter.read_voltage (0), LABSoft_GUI_Label::UNIT::VOLT);
    gui ().voltmeter_fl_output_chan_0_value->value (chan0.to_text ().c_str ());

    LABSoft_GUI_Label chan1 (lab ().m_Voltmeter.read_voltage (1), LABSoft_GUI_Label::UNIT::VOLT);
    gui ().voltmeter_fl_output_chan_1_value->value (chan1.to_text ().c_str ());
  } 
}

// EOF