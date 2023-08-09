  #include "LABSoft_GUI_Oscilloscope_Display.h"

#include <vector>
#include <cmath>
#include <iostream>

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl.H>

#include "../Utility/LAB_Constants.h"
#include "../Utility/LAB_Utility_Functions.h"

LABSoft_GUI_Oscilloscope_Display::
LABSoft_GUI_Oscilloscope_Display (int X,
                              int Y,
                              int W,
                              int H,
                              const char *label) 
  : Fl_Widget (X, Y, W, H, label)                                               
{
  calc_cached_drawing_values ();
}

LABSoft_GUI_Oscilloscope_Display:: 
~LABSoft_GUI_Oscilloscope_Display ()
{

}

int LABSoft_GUI_Oscilloscope_Display::
handle (int event)
{
  switch (event)
  {
    case (FL_MOUSEWHEEL):
    {
      if (Fl::belowmouse () == this)
      {
        do_callback ();

        return (1);
      }
      else 
      {
        return (0);
      }
    }

    case (FL_PUSH):
    {
      do_callback ();

      return (1);
    }

    case (FL_DRAG):
    {
      do_callback ();

      return (1);
    }
  }
}

int LABSoft_GUI_Oscilloscope_Display::
calc_samp_y_coord (double sample, 
                   double y_scaler)
{
  if (LABF::is_equal (sample, 0.0))
  {
    return (m_display_height_midline);
  }
  else
  {
    return (std::round (m_display_height_midline - (sample * y_scaler)));
  }
}

double LABSoft_GUI_Oscilloscope_Display:: 
calc_y_scaler (double voltage_per_division)
{
  double y_scaler = m_display_half_height / 
    (LABC::OSC_DISPLAY::NUMBER_OF_ROWS_HALF * voltage_per_division);

  return (y_scaler);
}

void LABSoft_GUI_Oscilloscope_Display:: 
draw ()
{
  draw_box      (FL_FLAT_BOX, LABC::OSC_DISPLAY::BACKGROUND_COLOR);
  draw_grid     ();
  draw_channels ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
draw_grid ()
{
  // 1. Set line color and style
  fl_color      (LABC::OSC_DISPLAY::GRID_COLOR);
  fl_line_style (FL_SOLID, 0, NULL);

  // 2. Draw outer box
  fl_line (x ()       , y ()       , x () + w (), y ()       ); // up
  fl_line (x () + w (), y ()       , x () + w (), y () + h ()); // right
  fl_line (x () + w (), y () + h (), x ()       , y () + h ()); // down
  fl_line (x ()       , y () + h (), x ()       , y ()       ); // left

  // 3. Draw rows
  for (int row = 0; row < (LABC::OSC_DISPLAY::NUMBER_OF_ROWS - 1); row++)
  {
    if (row == LABC::OSC_DISPLAY::MID_ROW_INDEX)
    {
      fl_line_style (FL_DASH, 0, NULL);
    }
    else 
    {
      fl_line_style (FL_DOT, 0, NULL);
    }

    int Y = std::round ((row + 1) * m_row_height) + y ();
    
    fl_line (x (), Y, x () + w (), Y);
  }

  // 4. Draw columns
  for (int col = 0; col < (LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS - 1); col++)
  {
    if (col == LABC::OSC_DISPLAY::MID_COLUMN_INDEX)
    {
      fl_line_style (FL_DASH, 0, NULL);
    }
    else 
    {
      fl_line_style (FL_DOT, 0, NULL);
    }

    int X = std::round ((col + 1) * m_column_width) + x ();

    fl_line (X, y (), X, y () + h ());
  }

  // 5. Draw upper and lower x-axis ticks
  fl_line_style (FL_SOLID, 0, NULL);

  for (int Y = y (); Y <= (y () + h ()); Y += h ())
  {
    for (int X = x (); X < (x () + w ()); X += m_column_width)
    {
      for (int i = 0; i < LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS; i++)
      {
        if (i == (LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS / 2))
        {
          fl_line (
            X + (i * m_x_axis_minor_ticks_width),
            Y,
            X + (i * m_x_axis_minor_ticks_width),
            Y == y () ? Y + LABC::OSC_DISPLAY::X_AXIS_SEMI_MAJOR_TICK_LENGTH :
              Y - LABC::OSC_DISPLAY::X_AXIS_SEMI_MAJOR_TICK_LENGTH
          );
        }
        else 
        {
          fl_line (
            X + (i * m_x_axis_minor_ticks_width),
            Y,
            X + (i * m_x_axis_minor_ticks_width),
            Y == y () ? Y + LABC::OSC_DISPLAY::X_AXIS_MINOR_TICK_LENGTH :
              Y - LABC::OSC_DISPLAY::X_AXIS_MINOR_TICK_LENGTH
          );
        }
      }

      fl_line (
        X,
        Y, 
        X, 
        Y == y () ? Y + LABC::OSC_DISPLAY::X_AXIS_MAJOR_TICK_LENGTH : 
          Y - LABC::OSC_DISPLAY::X_AXIS_MAJOR_TICK_LENGTH 
      );
    }
  }

  // 6. Draw left and right y-axis ticks
  fl_line_style (FL_SOLID, 0, NULL);

  for (int X = x (); X <= (x () + w ()); X += w ())
  {
    for (int Y = y (); Y < (y () + h ()); Y += m_row_height)
    {
      for (int i = 0; i < LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS; i++)
      {
        if (i == (LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS / 2))
        {
          fl_line (
            X,
            Y + (i * m_y_axis_minor_ticks_height),
            X == x () ? X + LABC::OSC_DISPLAY::Y_AXIS_SEMI_MAJOR_TICK_LENGTH :
              X - LABC::OSC_DISPLAY::Y_AXIS_SEMI_MAJOR_TICK_LENGTH,
            Y + (i * m_y_axis_minor_ticks_height)
          );
        }
        else 
        {
          fl_line (
            X,
            Y + (i * m_y_axis_minor_ticks_height),
            X == x () ? X + LABC::OSC_DISPLAY::Y_AXIS_MINOR_TICK_LENGTH :
              X - LABC::OSC_DISPLAY::Y_AXIS_MINOR_TICK_LENGTH,
            Y + (i * m_y_axis_minor_ticks_height)
          );
        }
      }

      fl_line (
        X,
        Y,
        X == x () ? X + LABC::OSC_DISPLAY::Y_AXIS_MAJOR_TICK_LENGTH :
          X - LABC::OSC_DISPLAY::Y_AXIS_MAJOR_TICK_LENGTH,
        Y
      );
    }
  }

  // 7. Reset line color and style
  fl_color      (0);
  fl_line_style (FL_SOLID, 0, NULL);
}

void LABSoft_GUI_Oscilloscope_Display:: 
draw_channels () 
{
  if (m_parent_data == nullptr)
  {
    return;
  }

  if (!m_parent_data->has_enabled_channels ())
  {
    return;
  }

  // ----------

  fl_push_clip  (x (), y (), w (), h ());
  
  LAB_Parent_Data_Oscilloscope &pdata = *m_parent_data;

  unsigned samp_count = (pdata.samples < w () ? pdata.samples : w ()) - 1;

  for (unsigned chan = 0; chan < (pdata.channel_data.size ()); chan++)
  {
    if (pdata.channel_data[chan].is_enabled)
    {
      std::vector<std::array<int, 2>> &pp = pdata.channel_data[chan].pixel_points;

      // Because of how line styles are implemented on Win32 systems, 
      // you must set the line style after setting the drawing color. 
      // If you set the color after the style, style settings will be lost.
      fl_color      (LABC::DISPLAY::CHAN_COLORS[chan]);
      fl_line_style (FL_SOLID, 0, 0);

      if (pdata.samples <= LABC::OSC_DISPLAY::SAMPLE_MARKING_THRESHOLD)
      {
        for (unsigned i = 0; i < samp_count; i++)
        {
          fl_line             (pp[i][0], pp[i][1], pp[i + 1][0], pp[i + 1][1]);
          draw_sample_marker  (pp[i][0], pp[i][1]);
        }
      }
      else 
      {
        for (unsigned i = 0; i < samp_count; i++)
        {
          fl_line (pp[i][0], pp[i][1], pp[i + 1][0], pp[i + 1][1]);
        }
      }
    }
  }

  fl_color      (0);
  fl_line_style (0);
  fl_pop_clip   ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
fill_pixel_points ()
{
  if (m_parent_data->is_osc_core_running)
  {
    fill_pixel_points_osc_running ();
  }
  else 
  {
    fill_pixel_points_osc_stopped ();
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
fill_pixel_points_osc_running ()
{
  LAB_Parent_Data_Oscilloscope& pdata = *m_parent_data;

  for (unsigned chan = 0; chan < pdata.channel_data.size (); chan++)
  {
    if (pdata.channel_data[chan].is_enabled)
    {
      LAB_Channel_Data_Oscilloscope&    cdata = pdata.channel_data[chan];
      std::vector<std::array<int, 2>>&  pp    = cdata.pixel_points;

      double y_scaler = calc_y_scaler (cdata.voltage_per_division);
      double x_offset = calc_x_offset (chan);

      if (pdata.samples >= w ())
      {
        double samp_skipper = (pdata.samples - 1) / static_cast<double>(w ());

        for (unsigned a = 0; a <= w (); a++)
        {
          double sample = cdata.samples[std::round (a * samp_skipper)] + cdata.vertical_offset;

          pp[a][0] = std::round   (x () + a + x_offset);
          pp[a][1] = calc_samp_y_coord (sample, y_scaler);
        }
      }
      else 
      {
        double x_skipper = static_cast<double>(w ()) / (pdata.samples - 1);

        for (unsigned a = 0; a < pdata.samples; a++)
        {
          double sample = cdata.samples[a] + cdata.vertical_offset;

          pp[a][0] = std::round   (x () + (a * x_skipper) + x_offset);
          pp[a][1] = calc_samp_y_coord (sample, y_scaler);
        }
      }
    }
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
fill_pixel_points_osc_stopped ()
{
  LAB_Parent_Data_Oscilloscope& pdata = *m_parent_data;

  double  tpd     = pdata.time_per_division;
  double  tpd_raw = pdata.time_per_division_raw_buffer;

  fill_pixel_points_osc_running ();
  
  // if (LABF::is_equal (tpd, tpd_raw))
  // {
  //   fill_pixel_points_osc_running ();
  // }
  // else 
  // {
  //   double scaler = tpd_raw / tpd;

  //   if (tpd > tpd_raw) // zoom out
  //   {
  //     unsigned  pix_count   = w () * scaler;
  //     double    samp_scaler = pdata.samples / scaler;

  //     for (unsigned chan = 0; chan < pdata.channel_data.size (); chan++)
  //     {


  //       LAB_Channel_Data_Oscilloscope&    cdata = pdata.channel_data[chan];
  //       std::vector<std::array<int, 2>>&  pp    = cdata.pixel_points;

  //       for (unsigned a = 0; a < pix_count; a++)
  //       {
  //         double samp = cdata.samples[std::round (a * samp_scaler)];

  //         pp[a][0] = (x () + x_offset);
  //         pp[a][1] = 
  //       }
  //     }
  //   }
  //   else // (tpd < tpd_raw) 
  //   {
  //     unsigned samp_count = 

  //     for ()
  //     {

  //     }
  //   }
  // }
}

void LABSoft_GUI_Oscilloscope_Display:: 
draw_sample_marker (int x, int y)
{
  // Vertical
  fl_line (
    x,
    y + LABC::OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
    x,
    y - LABC::OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE
  );

  // Horizontal
  fl_line (
    x + LABC::OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
    y,
    x - LABC::OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
    y
  );
}

void LABSoft_GUI_Oscilloscope_Display:: 
calc_cached_drawing_values ()
{
  m_column_width              = static_cast<float>(w ()) / LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS;
  m_row_height                = static_cast<float>(h ()) / LABC::OSC_DISPLAY::NUMBER_OF_ROWS;
  m_x_axis_minor_ticks_width  = m_column_width / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;
  m_y_axis_minor_ticks_height = m_row_height / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;
  m_display_half_height       = static_cast<float>(h ()) / 2.0;
  m_display_height_midline    = std::round (y () + m_display_half_height);
}

void LABSoft_GUI_Oscilloscope_Display::
load_osc_parent_data (LAB_Parent_Data_Oscilloscope& parent_data)
{
  m_parent_data = &parent_data;
}

void LABSoft_GUI_Oscilloscope_Display::
reserve_pixel_points ()
{
  LAB_Parent_Data_Oscilloscope& pdata = *m_parent_data;

  for (int a = 0; a < pdata.channel_data.size (); a++)
  {
    pdata.channel_data[a].pixel_points.reserve (LABC::OSC::NUMBER_OF_SAMPLES);
  }
}

double LABSoft_GUI_Oscilloscope_Display:: 
calc_x_offset (unsigned channel)
{
  double curr_off = m_parent_data->horizontal_offset;
  double curr_tpd = m_parent_data->time_per_division;

  if (curr_off == 0.0)
  { 
    return (0.0);
  }
  else 
  {
    double offset_pix = (-1.0 * m_column_width * (curr_off / curr_tpd));

    return (offset_pix);
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
mouse_down_start (int X, int Y)
{
  m_mouse_down_start_x = X;
  m_mouse_down_start_y = Y;
} 

int LABSoft_GUI_Oscilloscope_Display:: 
mouse_down_start_x ()
{
  return (m_mouse_down_start_x);
}

int LABSoft_GUI_Oscilloscope_Display:: 
mouse_down_start_y ()
{
  return (m_mouse_down_start_y);
}

int LABSoft_GUI_Oscilloscope_Display::
calc_time_per_div_scaler (int drag_x)
{
  return ((m_mouse_down_start_x - drag_x) / m_x_axis_minor_ticks_width);
}

void LABSoft_GUI_Oscilloscope_Display:: 
pre_drag_horizontal_offset (double value)
{
  m_pre_drag_horizontal_offset = value;
}

double LABSoft_GUI_Oscilloscope_Display:: 
pre_drag_horizontal_offset ()
{
  return (m_pre_drag_horizontal_offset);
}

// EOF