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
    return (m_osc.samples_displayed ());
  }
  else 
  {
    double samples = m_osc.samples_displayed () * (1 / tpd_ds);

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
calc_draw_window_width (double                              tpd_ds,
                        LAB_Oscilloscope_Display::DRAW_MODE draw_mode) const
{
  if (draw_mode == LAB_Oscilloscope_Display::DRAW_MODE::SHRINK)
  {
    double draw_window_width = m_width * tpd_ds;

    if (LABF::is_less_than (draw_window_width, 1.0, LABC::LABSOFT::EPSILON))
    {
      return (1);
    }
    else 
    {
      return (std::round (draw_window_width));
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
    scalers[chan] = (m_height / 2.0) / ((m_rows / 2.0) * m_osc.voltage_per_division (chan));
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
calc_sample_x_offset (double tpd_ds) const
{
  double horizontal_offset_delta  = m_osc.raw_buffer_horizontal_offset () - 
                                    m_osc.horizontal_offset ();
                                  
  double sample_x_offset =  (-m_column_width * tpd_ds) * 
                            (horizontal_offset_delta / m_osc.time_per_division ());
  
  return (sample_x_offset);
}

void LAB_Oscilloscope_Display:: 
resize_pixel_points (PixelPoints& pixel_points, 
                     unsigned     samples_to_display, 
                     unsigned     draw_window_width)
{
  unsigned value = samples_to_display >= draw_window_width ? samples_to_display : draw_window_width;

  for (unsigned i = 0; i < pixel_points.size (); i++)
  {
    std::vector<std::array<int, 2>>& pp = pixel_points[i];

    pp.resize (value);
  }
}

void LAB_Oscilloscope_Display:: 
update_cached_display_values ()
{
  m_column_width            = m_width / m_columns;
  m_display_height_midline  = m_height / 2.0; 
}

void LAB_Oscilloscope_Display:: 
display_parameters (unsigned width,
                    unsigned height,
                    unsigned rows,
                    unsigned columns)
{
  m_width   = width;
  m_height  = height;
  m_rows    = rows;
  m_columns = columns;

  update_cached_display_values  ();
  update_cached_values          ();
}

void LAB_Oscilloscope_Display::
update_cached_values ()
{
  if (m_osc.is_backend_running ())
  {
    double tpd_ds                     = 1.0;
    m_time_per_division_delta_scaler  = tpd_ds;
    
    m_draw_mode                       = LAB_Oscilloscope_Display::DRAW_MODE::FIT;
    m_samples_to_display              = m_osc.samples_displayed   ();
    m_draw_window_width               = m_width;
    m_x_coord_draw_start              = 0;
    m_samples_start_index             = 0;
    m_samp_skipper                    = calc_samp_skipper         (tpd_ds, m_draw_window_width);
    m_x_skipper                       = calc_x_skipper            (tpd_ds, m_draw_window_width);
    m_sample_y_scaler                 = calc_sample_y_scaler      ();
    m_sample_x_offset                 = calc_sample_x_offset      (tpd_ds);
    m_mark_samples                    = calc_mark_samples         (tpd_ds, m_samples_to_display);

    resize_pixel_points (m_pixel_points, m_samples_to_display, m_draw_window_width);
  }
  else 
  {
    m_time_per_division_delta_scaler  = calc_time_per_division_delta_scaler ();
    double& tpd_ds                    = m_time_per_division_delta_scaler;

    m_draw_mode                       = calc_draw_mode            (tpd_ds);
    m_samples_to_display              = calc_samples_to_display   (tpd_ds, m_draw_mode);
    m_draw_window_width               = calc_draw_window_width    (tpd_ds, m_draw_mode);
    m_x_coord_draw_start              = calc_x_coord_draw_start   (tpd_ds, m_draw_mode);
    m_samples_start_index             = calc_samples_start_index  (tpd_ds, m_samples_to_display);
    m_samp_skipper                    = calc_samp_skipper         (m_samples_to_display, m_draw_window_width);
    m_x_skipper                       = calc_x_skipper            (m_samples_to_display, m_draw_window_width);
    m_sample_y_scaler                 = calc_sample_y_scaler      ();
    m_sample_x_offset                 = calc_sample_x_offset      (tpd_ds);
    m_mark_samples                    = calc_mark_samples         (tpd_ds, m_samples_to_display);

    resize_pixel_points (m_pixel_points, m_samples_to_display, m_draw_window_width);
  }

  // debug
  std::cout << "m_draw_mode: "                      << static_cast<int>(m_draw_mode) << "\n";
  std::cout << "m_time_per_division_delta_scaler: " << m_time_per_division_delta_scaler << "\n";
  std::cout << "m_samples_to_display: "             << m_samples_to_display << "\n";
  std::cout << "m_draw_window_width: "              << m_draw_window_width << "\n";
  std::cout << "m_x_coord_draw_start: "             << m_x_coord_draw_start << "\n";
  std::cout << "m_samples_start_index: "            << m_samples_start_index << "\n";
  std::cout << "m_samp_skipper: "                   << m_samp_skipper << "\n";
  std::cout << "m_x_skipper: "                      << m_x_skipper << "\n";
  std::cout << "m_pixel_points.size ():  "          << m_pixel_points.size () << "\n";
  std::cout << "m_sample_x_offset:  "               << m_sample_x_offset << "\n";
  std::cout << "m_mark_samples:  "                  << m_mark_samples << "\n\n";
}

void LAB_Oscilloscope_Display:: 
update_pixel_points ()
{
  for (unsigned chan = 0; chan < m_osc.channels (); chan++)
  {
    if (m_osc.is_channel_enabled (chan))
    {
      std::vector<std::array<int, 2>>& pp = m_pixel_points[chan];

      if (m_samples_to_display >= m_draw_window_width)
      {
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

const LAB_Oscilloscope_Display::PixelPoints& LAB_Oscilloscope_Display:: 
pixel_points () const
{
  return (m_pixel_points);
}

bool LAB_Oscilloscope_Display:: 
mark_samples () const
{
  return (m_mark_samples);
}