  #include "LABSoft_Oscilloscope_Display.h"

#include <vector>
#include <cmath>
#include <iostream>

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl.H>

#include "Defaults.h"

LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display (int X,
                              int Y,
                              int W,
                              int H,
                              const char *label) 
  : Fl_Widget (X, Y, W, H, label)                                               
{
  calc_cached_drawing_values ();
}

LABSoft_Oscilloscope_Display:: 
~LABSoft_Oscilloscope_Display ()
{

}

int LABSoft_Oscilloscope_Display::
handle (int event)
{
  switch (event)
  {
    case (FL_MOUSEWHEEL):
    {
      if (Fl::belowmouse () == this)
      {
        do_callback ();
      }

      return (1);
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

void LABSoft_Oscilloscope_Display:: 
draw ()
{
  draw_box      (FL_FLAT_BOX, LABSOFT_OSCILLOSCOPE_DISPLAY::BACKGROUND_COLOR);
  draw_grid     ();
  draw_channels ();
}

void LABSoft_Oscilloscope_Display:: 
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

void LABSoft_Oscilloscope_Display:: 
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

  // ---

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

void LABSoft_Oscilloscope_Display:: 
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

void LABSoft_Oscilloscope_Display:: 
calc_cached_drawing_values ()
{
  m_column_width              = static_cast<float>(w ()) / LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS;
  m_row_height                = static_cast<float>(h ()) / LABC::OSC_DISPLAY::NUMBER_OF_ROWS;
  m_x_axis_minor_ticks_width  = m_column_width / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;
  m_y_axis_minor_ticks_height = m_row_height / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;
  m_display_half_height       = static_cast<float>(h ()) / 2.0;
  m_display_height_midline    = std::round (y () + m_display_half_height);
}

void LABSoft_Oscilloscope_Display::
load_osc_parent_data (LAB_Parent_Data_Oscilloscope& parent_data)
{
  m_parent_data = &parent_data;
}

void LABSoft_Oscilloscope_Display::
reserve_pixel_points ()
{
  LAB_Parent_Data_Oscilloscope& pdata = *m_parent_data;

  for (int a = 0; a < pdata.channel_data.size (); a++)
  {
    pdata.channel_data[a].pixel_points.reserve (LABC::OSC::NUMBER_OF_SAMPLES);
  }
}

void LABSoft_Oscilloscope_Display::
fill_pixel_points ()
{
  LAB_Parent_Data_Oscilloscope& pdata = *m_parent_data;

  for (int chan = 0; chan < pdata.channel_data.size (); chan++)
  {
    LAB_Channel_Data_Oscilloscope&    cdata = pdata.channel_data[chan];
    std::vector<std::array<int, 2>>&  pp    = cdata.pixel_points;

    double vertical_scaler = (m_display_half_height) / 
      (LABC::OSC_DISPLAY::NUMBER_OF_ROWS_HALF * cdata.voltage_per_division);
    
    double x_offset = calc_x_offset (chan);

    if (pdata.samples >= w ())
    {
      double skipper = (static_cast<double>(pdata.samples) - 1.0) / w ();
    
      for (int samp = 0; samp <= w (); samp++)
      {
        pp[samp][0] = std::round (x () + samp + x_offset);

        double curr_samp = (cdata.samples[std::round (skipper * samp)]) + 
          cdata.vertical_offset;
        
        if (LABF::compare_double (curr_samp, 0.0))
        {
          pp[samp][1] = m_display_height_midline;
        }
        else 
        {
          pp[samp][1] = std::round (m_display_height_midline - 
            (curr_samp * vertical_scaler));
        }
      }
    }
    else 
    {
      double scaler = w () / (static_cast<double>(pdata.samples) - 1);

      unsigned first_samp_index = ((cdata.samples.size () / 2.0) - 
        (pdata.samples / 2.0)) - 1.0;

      for (int samp = 0; samp < pdata.samples; samp++)
      {
        pp[samp][0] = std::round (x () + (samp * scaler) + x_offset);

        double curr_samp = (cdata.samples[first_samp_index + samp]) + 
          cdata.vertical_offset;
        
        if (LABF::compare_double (curr_samp, 0.0))
        {
          pp[samp][1] = m_display_height_midline;
        }
        else 
        {
          pp[samp][1] = std::round (m_display_height_midline - 
            (curr_samp * vertical_scaler));
        }
      }

      // double first_samp_index = (static_cast<double>(cdata.samples.size ()) - 
      //   static_cast<double>(pdata.samples)) / 2.0;
      
      // for (int samp = 0; samp < pdata.samples; samp++)
      // {
      //   pp[samp][0] = std::round (x () + (samp * scaler) + x_offset);

      //   double curr_samp = (cdata.samples[first_samp_index + samp]) + 
      //     cdata.vertical_offset;
        
      //   if (curr_samp == 0.0)
      //   {
      //     pp[samp][1] = m_display_height_midline;
      //   }
      //   else 
      //   {
      //     pp[samp][1] = std::round (m_display_height_midline - 
      //       (curr_samp * vertical_scaler));
      //   }
      // }
    }
  }
}

double LABSoft_Oscilloscope_Display:: 
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

void LABSoft_Oscilloscope_Display:: 
mouse_down_start (int X, int Y)
{
  m_mouse_down_start_x = X;
  m_mouse_down_start_y = Y;
} 

int LABSoft_Oscilloscope_Display:: 
mouse_down_start_x ()
{
  return (m_mouse_down_start_x);
}

int LABSoft_Oscilloscope_Display:: 
mouse_down_start_y ()
{
  return (m_mouse_down_start_y);
}

int LABSoft_Oscilloscope_Display::
calc_time_per_div_scaler (int drag_x)
{
  return ((m_mouse_down_start_x - drag_x) / m_x_axis_minor_ticks_width);
}

void LABSoft_Oscilloscope_Display:: 
pre_drag_horizontal_offset (double value)
{
  m_pre_drag_horizontal_offset = value;
}

double LABSoft_Oscilloscope_Display:: 
pre_drag_horizontal_offset ()
{
  return (m_pre_drag_horizontal_offset);
}

// EOF