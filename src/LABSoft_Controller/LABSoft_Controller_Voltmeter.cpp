#include "LABSoft_Controller_Voltmeter.h"

#include "LABSoft_Controller.h"
#include "../Utility/LABSoft_GUI_Label.h"

LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter (LABSoft_Controller& _LABSoft_Controller)
  : LABSoft_Controller_Unit (_LABSoft_Controller)
{
  init_gui_values ();
}

void LABSoft_Controller_Voltmeter:: 
init_gui_values ()
{
  LABSoft_GUI& gui = m_controller.gui ();

  gui.voltmeter_fl_output_chan_0_value->value ("- . - -");
  gui.voltmeter_fl_output_chan_1_value->value ("- . - -");
}

void LABSoft_Controller_Voltmeter:: 
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

void LABSoft_Controller_Voltmeter:: 
display_update_cycle ()
{
  if (lab ().m_Voltmeter.is_running ())
  {
    LABSoft_GUI_Label chan0 (lab ().m_Voltmeter.read_voltage (0), LABSoft_GUI_Label::UNIT::VOLT);
    gui ().voltmeter_fl_output_chan_0_value->value (chan0.to_label_text ().c_str ());

    LABSoft_GUI_Label chan1 (lab ().m_Voltmeter.read_voltage (1), LABSoft_GUI_Label::UNIT::VOLT);
    gui ().voltmeter_fl_output_chan_1_value->value (chan1.to_label_text ().c_str ());
  } 
}

// EOF