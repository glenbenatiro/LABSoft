#include "LAB_Oscilloscope_Display.h"

#include "../LAB.h"
#include "../LAB_Oscilloscope.h"
#include "../Utility/LAB_Utility_Functions.h"

using enum LAB_Oscilloscope_Display::DRAW_MODE;

void LAB_Oscilloscope_Display::
LAB_Oscilloscope_Display (LAB&              _LAB,
                          LAB_Oscilloscope& _LAB_Oscilloscope)
  : LAB_Module  (_LAB),
    m_osc       (_LAB_Oscilloscope)
{

}

void LAB_Oscilloscope_Display::
reserve_pixel_points (unsigned value)
{
  m_pixel_points.reserve (value);
}

void LAB_Oscilloscope_Display::
calc_cached_values ()
{
  m_time_per_division_delta_scaler  = calc_time_per_division_delta_scaler ();

  double& tpd_ds = m_time_per_division_delta_scaler;

  m_draw_mode           = calc_draw_mode          (tpd_ds);
  m_samples_to_display  = calc_samples_to_display (tpd_ds, m_draw_mode);
  m_draw_width_width    = calc_draw_window_width  (tpd_ds, m_draw_mode);
}

double LAB_Oscilloscope_Display::
calc_time_per_division_delta_scaler ()
{
  return (m_osc.raw_buffer_time_per_division () / m_osc.time_per_division ());
}

DRAW_MODE LAB_Oscilloscope_Display:: 
calc_draw_mode (double tpd_ds)
{
  if (LABF::is_less_than (tpd_ds, 1.0, LABC::LABSOFT::EPSILON))
  {
    return (DRAW_MODE::SHRINK);
  }
  else if (LABF::is_greater_than (tpd_ds, 1.0, LABC::LABSOFT::EPSILON))
  {
    return (DRAW_MODE::STRETCH);
  }
  else
  {
    return (DRAW_MODE::FIT);
  }
}

unsigned LAB_Oscilloscope_Display::
calc_samples_to_display (double    tpd_ds, 
                         DRAW_MODE draw_mode)
{
  if (draw_mode == DRAW_MODE::SHRINK)
  {
    return (m_osc.samples ());
  }
  else 
  {
    double samples = m_osc.samples () * (1 / tpd_ds);

    if (LABF::is_less_than (tpd_ds, 1.0, LABC::LABSOFT::EPSILON))
    {
      return (1);
    }
    else 
    {
      return (std::round (samples));
    }
  }
}

unsigned LAB_Oscilloscope_Display:: 
calc_draw_window_width (double    tpd_ds,
                        DRAW_MODE draw_mode)
{
  if (draw_mode == DRAW_MODE::SHRINK)
  {
    double pixel_points = m_width * tpd_ds;

    if (LABF::is_less_than (pixel_points, 1.0, LABC::LABSOFT::EPSILON))
    {
      return (1);
    }
    else 
    {
      return (std::round (pixel_points));
    }
  }
  else 
  {
    return (m_width);
  }
}

unsigned LAB_Oscilloscope_Display::
calc_x_coord_draw_start (double    tpd_ds,
                         DRAW_MODE draw_mode)
{
  if (draw_mode == DRAW_MODE::SHRINK)
  {
    double draw_window_width      = m_width * tpd_ds;
    double free_space_width       = m_width - draw_window_width;
    double free_space_width_half  = free_space_width / 2.0; 

    return (free_space_width_half);
  }
  else 
  {
    return (0);
  }
}

unsigned LAB_Oscilloscope_Display::
calc_samples_start_index (double   tpd_ds,
                          unsigned samples_to_display)
{
  if (samples_to_display < m_osc.samples ())
  {
    unsigned  undisplayed_samples       = m_osc.samples () - samples_to_display;
    unsigned  undisplayed_samples_half  = undisplayed_samples / 2.0;

    return (std::round (undisplayed_samples_half));
  }
  else 
  {
    return (0);
  }
}

double LAB_Oscilloscope_Display:: 
calc_samp_skipper (unsigned samples_to_display,
                   unsigned draw_window_width)
{
  return ((samples_to_display - 1.0) / (draw_window_width - 1.0));
}

double LAB_Oscilloscope_Display::
calc_x_skipper (unsigned samples_to_display,
                unsigned draw_window_width)
{
  return ((draw_window_width - 1.0) / (samples_to_display - 1.0));
}

void LAB_Oscilloscope_Display:: 
dimensions (unsigned width,
            unsigned height)
{
  m_width   = width;
  m_height  = height;

  calc_cached_values ();
}

void LAB_Oscilloscope_Display:: 
load_pixel_points ()
{

}

LAB_Oscilloscope_Display::PixelPoints& LAB_Oscilloscope_Display::
pixel_points () const
{
  return (m_pixel_points);
}