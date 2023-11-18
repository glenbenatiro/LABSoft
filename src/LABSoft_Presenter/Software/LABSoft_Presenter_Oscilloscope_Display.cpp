#include "LABSoft_Presenter_Oscilloscope_Display.h"

#include "../../LAB/LAB.h"
#include "../LABSoft_Controller.h"
#include "../../LABSoft_GUI/LABSoft_GUI.h"

LABSoft_Presenter_Oscilloscope_Display:: 
LABSoft_Presenter_Oscilloscope_Display (LABSoft_Controller& _LABSoft_Controller)
  : LABSoft_Controller_Unit (_LABSoft_Controller)
{
  load_gui ();
}

void LABSoft_Presenter_Oscilloscope_Display:: 
load_gui ()
{
  LAB_Oscilloscope&                 osc           = lab ().m_Oscilloscope;
  LAB_Oscilloscope_Display&         osc_disp      = lab ().m_Oscilloscope_Display;
  LABSoft_GUI_Oscilloscope_Display& osc_disp_gui  = *(gui ().oscilloscope_labsoft_gui_oscilloscope_display);

  osc_disp_gui.load_presenter (m_controller);

  osc_disp.dimensions (osc_disp_gui.display_width (), osc_disp_gui.display_height ());

  osc_disp_gui.voltage_per_division (0, osc.voltage_per_division (0));
  osc_disp_gui.voltage_per_division (1, osc.voltage_per_division (1));
  osc_disp_gui.vertical_offset      (1, osc.vertical_offset (1));
  osc_disp_gui.vertical_offset      (1, osc.vertical_offset (1));
  osc_disp_gui.horizontal_offset    (osc.horizontal_offset ());
  osc_disp_gui.time_per_division    (osc.time_per_division ());
}

void LABSoft_Presenter_Oscilloscope_Display:: 
cb_mouse_wheel (int direction)
{

}

void LABSoft_Presenter_Oscilloscope_Display:: 
update_display ()
{
  LAB_Oscilloscope_Display&         osc_disp      = lab ().m_Oscilloscope_Display; 
  LABSoft_GUI_Oscilloscope_Display& osc_disp_gui  = *(gui ().oscilloscope_labsoft_gui_oscilloscope_display);

  osc_disp.load_pixel_points ();

  osc_disp_gui.draw_channels (osc_disp.pixel_points ());
}