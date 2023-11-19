#include "LAB_Oscilloscope_Display.h"

#include <cmath>
#include <iostream>

#include "../LAB.h"
#include "../LAB_Oscilloscope.h"
#include "../../Utility/LAB_Utility_Functions.h"

LAB_Oscilloscope_Display::
LAB_Oscilloscope_Display (LAB&              _LAB,
                          LAB_Oscilloscope& _LAB_Oscilloscope)
  : LAB_Module  (_LAB),
    m_osc       (_LAB_Oscilloscope)
{

}

void LAB_Oscilloscope_Display:: 
calc_pixel_points ()
{
  for (unsigned chan = 0; chan < m_osc.channels (); chan++)
  {
    if (m_osc.is_channel_enabled (chan))
    {
      std::vector<std::array<int, 2>>& pp = m_pixel_points[chan];

      if (m_samples_to_display >= m_draw_window_width)
      {
        pp.resize (m_draw_window_width);

        for (unsigned i = 0; i < pp.size (); i++)
        {
          double sample = m_osc.chan_samples (chan)[std::round (m_samples_start_index) + 
            (i * m_samp_skipper)] + m_osc.vertical_offset (chan);
          
          pp[i][0] = m_x_coord_draw_start + i;
          pp[i][1] = calc_sample_y_coord (sample, chan);
        }
      }
      else 
      {
        pp.resize (m_samples_to_display);

        for (unsigned i = 0; i < pp.size (); i++)
        {
          double sample = m_osc.chan_samples (chan)[i] + m_osc.vertical_offset (chan);

          pp[i][0] = std::round (m_x_coord_draw_start + (i * m_x_skipper));
          pp[i][1] = calc_sample_y_coord (sample, chan);
        }
      }
    }
  }
}

void LAB_Oscilloscope_Display::
calc_cached_values ()
{
  m_time_per_division_delta_scaler  = calc_time_per_division_delta_scaler ();

  double& tpd_ds = m_time_per_division_delta_scaler;

  m_draw_mode           = calc_draw_mode            (tpd_ds);
  m_samples_to_display  = calc_samples_to_display   (tpd_ds, m_draw_mode);
  m_draw_window_width   = calc_draw_window_width    (tpd_ds, m_draw_mode);
  m_x_coord_draw_start  = calc_x_coord_draw_start   (tpd_ds, m_draw_mode);
  m_samples_start_index = calc_samples_start_index  (tpd_ds, m_samples_to_display);
  m_samp_skipper        = calc_samp_skipper         (tpd_ds, m_draw_window_width);
  m_x_skipper           = calc_x_skipper            (tpd_ds, m_draw_window_width);
  m_sample_y_scaler     = calc_sample_y_scaler      ();
  m_sample_x_offset     = calc_sample_x_offset      ();

  // debug
  std::cout << "m_time_per_division_delta_scaler: " << m_time_per_division_delta_scaler << "\n";
  std::cout << "m_samples_to_display: "             << m_samples_to_display << "\n";
  std::cout << "m_draw_window_width: "              << m_draw_window_width << "\n";
  std::cout << "m_x_coord_draw_start: "             << m_x_coord_draw_start << "\n";
  std::cout << "m_samples_start_index: "            << m_samples_start_index << "\n";
  std::cout << "m_samp_skipper: "                   << m_samp_skipper << "\n";
  std::cout << "m_x_skipper: "                      << m_x_skipper << "\n";
}

double LAB_Oscilloscope_Display::
calc_time_per_division_delta_scaler () const
{
  return (m_osc.raw_buffer_time_per_division () / m_osc.time_per_division ());
}

LAB_Oscilloscope_Display::DRAW_MODE LAB_Oscilloscope_Display:: 
calc_draw_mode (double tpd_ds) const
{
  if (LABF::is_less_than (tpd_ds, 1.0, LABC::LABSOFT::EPSILON))
  {
    return (LAB_Oscilloscope_Display::DRAW_MODE::SHRINK);
  }
  else if (LABF::is_greater_than (tpd_ds, 1.0, LABC::LABSOFT::EPSILON))
  {
    return (LAB_Oscilloscope_Display::DRAW_MODE::STRETCH);
  }
  else
  {
    return (LAB_Oscilloscope_Display::DRAW_MODE::FIT);
  }
}

unsigned LAB_Oscilloscope_Display::
calc_samples_to_display (double    tpd_ds, 
                         LAB_Oscilloscope_Display::DRAW_MODE draw_mode) const
{
  if (draw_mode == LAB_Oscilloscope_Display::DRAW_MODE::SHRINK)
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
                        LAB_Oscilloscope_Display::DRAW_MODE draw_mode) const
{
  if (draw_mode == LAB_Oscilloscope_Display::DRAW_MODE::SHRINK)
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
                         LAB_Oscilloscope_Display::DRAW_MODE draw_mode) const
{
  if (draw_mode == LAB_Oscilloscope_Display::DRAW_MODE::SHRINK)
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
                          unsigned samples_to_display) const
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
                   unsigned draw_window_width) const
{
  return ((samples_to_display - 1.0) / (draw_window_width - 1.0));
}

double LAB_Oscilloscope_Display::
calc_x_skipper (unsigned samples_to_display,
                unsigned draw_window_width) const
{
  return ((draw_window_width - 1.0) / (samples_to_display - 1.0));
}

bool LAB_Oscilloscope_Display:: 
calc_mark_samples (double   tpd_ds, 
                   unsigned samples_to_display) const
{
  return ((samples_to_display <= LABC::OSC_DISPLAY::SAMPLE_MARKING_THRESHOLD) &&
    (LABF::is_greater_than (tpd_ds, 0.5, LABC::LABSOFT::EPSILON)));
}

LAB_Oscilloscope_Display::Scalers LAB_Oscilloscope_Display::
calc_sample_y_scaler () const
{
  LAB_Oscilloscope_Display::Scalers scalers;

  for (unsigned chan = 0; chan < scalers.size (); chan++)
  {
    scalers[chan] = (m_height / 2.0) / ((LABC::OSC::DISPLAY_NUMBER_OF_ROWS / 2.0) * 
                    m_osc.voltage_per_division (chan));
  }

  return (scalers);
}

int LAB_Oscilloscope_Display::
calc_sample_y_coord (double   sample, 
                     unsigned channel) const
{
  return (std::round (m_display_height_midline - (sample * m_sample_y_scaler[channel])));
}

double LAB_Oscilloscope_Display::
calc_sample_x_offset () const
{
  double horizontal_offset_delta  = m_osc.raw_buffer_horizontal_offset () - 
                                    m_osc.horizontal_offset ();
  
  double sample_x_offset  = (-m_column_width * m_time_per_division_delta_scaler) * 
                            (horizontal_offset_delta/ m_osc.time_per_division ());

  return (sample_x_offset);
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
pixel_points ()
{
  return (m_pixel_points);
}