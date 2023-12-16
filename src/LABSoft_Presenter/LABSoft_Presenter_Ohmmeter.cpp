#include "LABSoft_Presenter_Ohmmeter.h"

#include "../LAB/LAB.h"
#include "LABSoft_Presenter.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"
#include "../Utility/LABSoft_GUI_Label.h"

LABSoft_Presenter_Ohmmeter:: 
LABSoft_Presenter_Ohmmeter (LABSoft_Presenter& _LABSoft_Presenter)
  : LABSoft_Presenter_Unit (_LABSoft_Presenter)
{
  init_gui_values ();
}

void LABSoft_Presenter_Ohmmeter:: 
init_gui_values ()
{
  gui ().ohmmeter_fl_output_resistance->value ("- . - -");
}

void LABSoft_Presenter_Ohmmeter:: 
cb_run_stop  (Fl_Light_Button* w, 
              void*            data)
{
  // if (w->value ())
  // {
  //   gui ().oscilloscope_fl_light_button_run_stop->clear ();

  //   lab ().m_Voltmeter.run ();
  // }
  // else 
  // {
  //   lab ().m_Voltmeter.stop ();
  // }
}

void LABSoft_Presenter_Ohmmeter:: 
display_update_cycle ()
{
  if (lab ().m_Ohmmeter.is_running ())
  {

  }
}

// EOF