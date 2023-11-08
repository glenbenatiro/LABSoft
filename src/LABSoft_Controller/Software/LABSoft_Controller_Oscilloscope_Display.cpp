#include "LABSoft_Controller_Oscilloscope_Display.h"

LABSoft_Controller_Oscilloscope_Display:: 
LABSoft_Controller_Oscilloscope_Display (LABSoft_Controller& _LABSoft_Controller)
  : LABSoft_Controller_Unit (_LABSoft_Controller),
    m_osc (_LABSoft_Controller.m_LAB.m_Oscilloscope)
{

}

void LABSoft_Controller_Oscilloscope_Display:: 
calc_pixel_points () const
{
  for (unsigned chan = 0; chan < lab ().m_Oscilloscope.channels (); chan++)
  {

  }
}

void LABSoft_Controller_Oscilloscope_Display:: 
calc_cached_values_drawing ()
{
  m_time_per_division_scaler  = calc_time_per_division_scaler ();
  m_samples_to_display        = calc_samples_to_display       (); 
  m_pixel_points_to_display   = calc_pixel_points_to_display  ();
}

void LABSoft_Controller_Oscilloscope_Display::
calc_time_per_division_scaler ()
{
  return (m_osc.time_per_division ())
}