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
  
}

LABSoft_Oscilloscope_Display:: 
~LABSoft_Oscilloscope_Display ()
{

}

void LABSoft_Oscilloscope_Display:: 
draw ()
{
  draw_box (FL_FLAT_BOX, LABSOFT_OSCILLOSCOPE_DISPLAY::BACKGROUND_COLOR);
  draw_grid ();
  draw_channels ();
}

void LABSoft_Oscilloscope_Display:: 
draw_grid ()
{
  // 1. Set line color and style
  fl_color      (LABC::OSC_DISPLAY::COLOR::GRID);
  fl_line_style (FL_SOLID, 0, NULL);

  // 2. Draw outer box
  fl_line (x ()       , y ()       , x () + w (), y ()       ); // up
  fl_line (x () + w (), y ()       , x () + w (), y () + h ()); // right
  fl_line (x () + w (), y () + h (), x ()       , y () + h ()); // down
  fl_line (x ()       , y () + h (), x ()       , y ()       ); // left

  // 3. Draw rows
  for (int row = 0; row < (LABC::OSC_DISPLAY::NUMBER_OF_ROWS - 1); row++)
  {
    if (row == ((LABC::OSC_DISPLAY::NUMBER_OF_ROWS / 2) - 1))
    {
      fl_line_style (FL_DASH, 0, NULL);
    }
    else 
    {
      fl_line_style (FL_DOT, 0, NULL);
    }

    int Y = std::round ((row + 1) * (static_cast<float>(h ()) / static_cast<float>
      (LABC::OSC_DISPLAY::NUMBER_OF_ROWS))) + y ();
    
    fl_line (x (), Y, x () + w (), Y);
  }

  // 4. Draw columns
  for (int col = 0; col < (LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS - 1); col++)
  {
    if (col == ((LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS / 2) - 1))
    {
      fl_line_style (FL_DASH, 0, NULL);
    }
    else 
    {
      fl_line_style (FL_DOT, 0, NULL);
    }

    int X = std::round ((col + 1) * (static_cast<float>(w ()) / static_cast<float>
      (LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS))) + x ();

    fl_line (X, y (), X, y () + h ());
  }

  // 5. Draw upper and lower x-axis ticks
  fl_line_style (FL_SOLID, 0, NULL);

  for (int Y = y (); Y <= (y () + h ()); Y += h ())
  {
    double major_width = static_cast<double>(w ()) / LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS;
    double minor_width = major_width / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;

    for (int X = x (); X < (x () + w ()); X += major_width)
    {
      for (int i = 0; i < LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS; i++)
      {
        if (i == (LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS / 2))
        {
          fl_line (
            X + (i * minor_width),
            Y,
            X + (i * minor_width),
            Y == y () ? Y + LABC::OSC_DISPLAY::X_AXIS_SEMI_MAJOR_TICK_LENGTH :
              Y - LABC::OSC_DISPLAY::X_AXIS_SEMI_MAJOR_TICK_LENGTH
          );
        }
        else 
        {
          fl_line (
            X + (i * minor_width),
            Y,
            X + (i * minor_width),
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
    double major_height = static_cast<double>(h ()) / LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS;
    double minor_height = major_height / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;

    for (int Y = y (); Y < (y () + h ()); Y += major_height)
    {
      for (int i = 0; i < LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS; i++)
      {
        if (i == (LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS / 2))
        {
          fl_line (
            X,
            Y + (i * minor_height),
            X == x () ? X + LABC::OSC_DISPLAY::Y_AXIS_SEMI_MAJOR_TICK_LENGTH :
              X - LABC::OSC_DISPLAY::Y_AXIS_SEMI_MAJOR_TICK_LENGTH,
            Y + (i * minor_height)
          );
        }
        else 
        {
          fl_line (
            X,
            Y + (i * minor_height),
            X == x () ? X + LABC::OSC_DISPLAY::Y_AXIS_MINOR_TICK_LENGTH :
              X - LABC::OSC_DISPLAY::Y_AXIS_MINOR_TICK_LENGTH,
            Y + (i * minor_height)
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

int LABSoft_Oscilloscope_Display:: 
draw_channels () 
{
  if (!m_parent_data_osc)
  {
    return -1;
  }

  if (!(m_parent_data_osc->has_enabled_channels ()))
  {
    return -2;
  }

  // ---
 
  fl_push_clip (x (), y (), w (), h ());

  LAB_Parent_Data_Oscilloscope  &pdata = *m_parent_data_osc;
  int samp_count = (pdata.w_samp_count < w () ? pdata.w_samp_count : w ()) - 1;

  for (int chan = 0; chan < (m_parent_data_osc->channel_data.size ()); chan++)
  {
    LAB_Channel_Data_Oscilloscope &c_data = pdata.channel_data[chan];
    std::vector<std::array<int, 2>> &pp   = c_data.pixel_points;

    if (c_data.is_enabled)
    {
      fl_color (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::CHANNEL_COLORS[chan]);

      if (pdata.w_samp_count <= LABSOFT_OSCILLOSCOPE_DISPLAY::SAMPLE_MARKING_THRESHOLD)
      {
        for (int samp_i = 0; samp_i < samp_count; samp_i++)
        {
          fl_line_style (
            FL_SOLID,
            0,
            0
          );

          fl_line (
            pp[samp_i][0], 
            pp[samp_i][1], 
            pp[samp_i + 1][0], 
            pp[samp_i + 1][1]
          );

          fl_line_style (
            FL_SOLID,
            LABSOFT_OSCILLOSCOPE_DISPLAY::SAMPLE_MARKING_THICKNESS,
            0
          );

          // Draw the sample markers
          fl_line (
            // vertical
            pp[samp_i][0],
            pp[samp_i][1] + LABSOFT_OSCILLOSCOPE_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
            pp[samp_i][0],
            pp[samp_i][1] - LABSOFT_OSCILLOSCOPE_DISPLAY::SAMPLE_MARKING_AMPLITUDE
          );

          fl_line (
            // horizontal
            pp[samp_i][0] + LABSOFT_OSCILLOSCOPE_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
            pp[samp_i][1],
            pp[samp_i][0] - LABSOFT_OSCILLOSCOPE_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
            pp[samp_i][1]
          );
        }
      }
      else 
      {
        for (int samp_i = 0; samp_i < samp_count; samp_i++)
        {
          fl_line (
            pp[samp_i][0], 
            pp[samp_i][1], 
            pp[samp_i + 1][0], 
            pp[samp_i + 1][1]
          );
        }
      }  
    }
  }

  fl_line_style (0);
  fl_pop_clip();

  return (1);
}

void LABSoft_Oscilloscope_Display::
load_osc_parent_data (LAB_Parent_Data_Oscilloscope& parent_data)
{
  m_parent_data_osc = &parent_data;
}

int LABSoft_Oscilloscope_Display::
reserve_pixel_points ()
{
  if (!m_parent_data_osc)
  {
    return -1;
  }
  else 
  {
    for (int a = 0; a < (m_parent_data_osc->channel_data.size ()); a++)
    {
      m_parent_data_osc->channel_data[a].pixel_points.reserve (
        LABC::OSC::NUMBER_OF_SAMPLES
      );
    }

    return 1;
  }
}


void LABSoft_Oscilloscope_Display::
fill_pixel_points ()
{
  LAB_Parent_Data_Oscilloscope& pdata = *(m_parent_data_osc);
  double  osc_disp_vert_half           = h () / 2.0;
  int     osc_disp_vert_midline        = std::round (y () + osc_disp_vert_half);

  for (int chan = 0; chan < pdata.channel_data.size (); chan++)
  {
    LAB_Channel_Data_Oscilloscope&    cdata = pdata.channel_data[chan];
    std::vector<std::array<int, 2>>&  pp    = cdata.pixel_points;

    double vertical_scaler = (osc_disp_vert_half) / 
      ((LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_ROWS / 2.0) * 
      cdata.voltage_per_division);
    
    double x_offset = calc_x_offset (chan);

    if (pdata.w_samp_count >= w ())
    {
      double skipper = (static_cast<double>(pdata.w_samp_count) - 1.0) / w ();
    
      for (int samp = 0; samp <= w (); samp++)
      {
        pp[samp][0] = std::round (x () + samp + x_offset);

        double curr_samp = (cdata.samples[std::round (skipper * samp)]) + 
          cdata.vertical_offset;
        
        if (curr_samp == 0.0)
        {
          pp[samp][1] = osc_disp_vert_midline;
        }
        else 
        {
          pp[samp][1] = std::round (osc_disp_vert_midline - 
            (curr_samp * vertical_scaler));
        }
      }
    }
    else 
    {
      double scaler = w () / (static_cast<double>(pdata.w_samp_count) - 1.0);

      double first_samp_index = (static_cast<double>(cdata.samples.size ()) - 
        static_cast<double>(pdata.w_samp_count)) / 2.0;
      
      for (int samp = 0; samp < pdata.w_samp_count; samp++)
      {
        pp[samp][0] = std::round (x () + (samp * scaler) + x_offset);

        double curr_samp = (cdata.samples[first_samp_index + samp]) + 
          cdata.vertical_offset;
        
        if (curr_samp == 0.0)
        {
          pp[samp][1] = osc_disp_vert_midline;
        }
        else 
        {
          pp[samp][1] = std::round (osc_disp_vert_midline - 
            (curr_samp * vertical_scaler));
        }
      }
    }
  }
}

double LABSoft_Oscilloscope_Display:: 
calc_x_offset (unsigned channel)
{
  double curr_off = m_parent_data_osc->horizontal_offset;
  double curr_tpd = m_parent_data_osc->time_per_division;

  if (curr_off == 0.0)
  { 
    return (0.0);
  }
  else 
  {
    double col_width = static_cast<double>(w ()) / 
      LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS;

    double offset_pix = (-1.0 * col_width * (curr_off / curr_tpd));

    return (offset_pix);
  }
}

// EOF