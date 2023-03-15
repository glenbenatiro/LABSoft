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
                              const char *label = 0) 
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
  draw_box (FL_FLAT_BOX, LABSOFT_OSCILLOSCOPE_DISPLAY_BACKGROUND_COLOR);
  draw_grid ();
  draw_channels ();
}

void LABSoft_Oscilloscope_Display:: 
draw_grid ()
{
  // set color
  fl_color (LABSOFT_OSCILLOSCOPE_DISPLAY_GRID_COLOR);

  // draw grid outer box
  fl_line_style (FL_SOLID, 0, NULL);
  fl_line (x (), y(), x () + w (), y ());                 // up
  fl_line (x () + w (), y (), x () + w (), y () + h ());  // right
  fl_line (x () + w (), y () + h (), x (), y () + h ());  // down
  fl_line (x (), y () + h (), x (), y ());                // left

  // Draw rows
  for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS - 1); a++)
  {
    if (a == ((LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS / 2) - 1))
      fl_line_style (FL_DASH, 0, NULL);
    else 
      fl_line_style (FL_DOT, 0, NULL);

    int Y = round ((a + 1) * ((float)h () / (float)LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS)) + y ();
    fl_line (x (), Y, x () + w (), Y);
  }

  // draw columns 
  for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS - 1); a++)
    {
      if (a == ((LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS / 2) - 1))
        fl_line_style (FL_DASH, 0, NULL);
      else 
        fl_line_style (FL_DOT, 0, NULL);

      int X = round ((a + 1) * ((float)w () / (float)LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS)) + x ();
      fl_line (X, y (), X, y () + h ());
    }
  
  // reset color
  fl_line_style (0);
}

int LABSoft_Oscilloscope_Display:: 
draw_channels () 
{
  if (!m_osc_parent_data)
  {
    return -1;
  }

  if (!(m_osc_parent_data->has_enabled_channels ()))
  {
    return -2;
  }

  // ---
 
  fl_push_clip (x (), y (), w (), h ());

  for (int a = 0; a < (m_osc_parent_data->channel_data.size ()); a++)
  {
    LAB_Parent_Data_Oscilloscope  &osc  = *m_osc_parent_data;
    LAB_Channel_Data_Oscilloscope &chn  = osc.channel_data[a];

    std::vector<std::array<int, 2>> &pp = chn.pixel_points;

    if (chn.is_enabled)
    {
      fl_color (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_COLORS[a]);

      int samp_count = ((osc.w_samp_count < w ()) ? osc.w_samp_count : w ()) - 1;

      for (int b = 0; b < samp_count; b++)
      {
        fl_line (pp[b][0], pp[b][1], pp[b + 1][0], pp[b + 1][1]);
      }
    }
  }

  fl_pop_clip();

  return 1;
}

void LABSoft_Oscilloscope_Display::
load_osc_parent_data (LAB_Parent_Data_Oscilloscope *parent_data)
{
  m_osc_parent_data = parent_data;
}

int LABSoft_Oscilloscope_Display::
reserve_pixel_points ()
{
  if (!m_osc_parent_data)
  {
    return -1;
  }
  else 
  {
    for (int a = 0; a < (m_osc_parent_data->channel_data.size ()); a++)
    {
      m_osc_parent_data->channel_data[a].pixel_points.reserve (
        LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES
      );
    }

    return 1;
  }
}

int LABSoft_Oscilloscope_Display::
fill_pixel_points ()
{
  if (!m_osc_parent_data)
  {
    return -1;
  }

  if (!(m_osc_parent_data->has_enabled_channels ()))
  {
    return -2;
  }

  //

  double osc_disp_vert_half     = h () / 2;
  double osc_disp_vert_midline  = y () + osc_disp_vert_half;

  for (int a = 0; a < (m_osc_parent_data->channel_data.size ()); a++)
  {
    if (m_osc_parent_data->channel_data[a].is_enabled)
    {
      LAB_Parent_Data_Oscilloscope    &osc  = *m_osc_parent_data;
      LAB_Channel_Data_Oscilloscope   &chn  = osc.channel_data[a];
      std::vector<std::array<int, 2>> &pp   = chn.pixel_points;

      double vert_scaler = (osc_disp_vert_half) / 
        ((LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS / 2) *
          chn.voltage_per_division);

      if (osc.time_per_division < LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_NO_ZOOM)
      {
        double volt_samp_x_off = 0;

        if (osc.w_samp_count > w ())
        {
          double sample_skip = static_cast<double>(osc.w_samp_count - 1.0) / 
            static_cast<double>(w () - 1.0);
                  
          for (int b = 0; b < w (); b++)
          {
            pp[b][0] = x () + b;

            double samp_value = (chn.samples[volt_samp_x_off + 
              (sample_skip * b)]) + (chn.vertical_offset);

            if (samp_value == 0.0)
            {
              pp[b][1] = osc_disp_vert_midline;
            }
            else
            {
              pp[b][1] = osc_disp_vert_midline - (samp_value * vert_scaler);
            }
          }
        }
        else if (osc.w_samp_count == w ())
        {
          for (int b = 0; b < w (); b++)
          {
            pp[b][0] = x () + b;

            double samp_value = (chn.samples[volt_samp_x_off + b]) +
              (chn.vertical_offset);

            if (samp_value == 0.0)
            {
              pp[b][1] = osc_disp_vert_midline;
            }
            else
            {
              pp[b][1] = osc_disp_vert_midline - (samp_value * vert_scaler);
            }
          }
        }
        else // osc.w_samp_count < w ()
        { 
          double x_pixel_scaler = ((static_cast<double>(w ()) - 1.0) / 
            (osc.w_samp_count - 1.0));

          for (int b = 0; b < osc.w_samp_count; b++)
          {
            pp[b][0] = x () + (b * x_pixel_scaler);

            double samp_value = (chn.samples[volt_samp_x_off + b]) +
              (chn.vertical_offset);
            
            if (samp_value == 0.0)
            {
              pp[b][1] = osc_disp_vert_midline;
            }
            else
            {
              pp[b][1] = osc_disp_vert_midline - (samp_value * vert_scaler);
            }
          }
        }
      }
      else 
      {
        double sample_skip = static_cast<double>(chn.samples.size () - 1) / 
          static_cast<double>(w () - 1);
                
        for (int b = 0; b < w (); b++)
        {
          pp[b][0] = x () + b;

          double samp_value = (chn.samples[sample_skip * b]) + 
            (chn.vertical_offset);

          if (samp_value == 0.0)
          {
            pp[b][1] = osc_disp_vert_midline;
          }
          else
          {
            pp[b][1] = osc_disp_vert_midline - (samp_value * vert_scaler);
          }
        }
      }
    }
  }

  return 1;
}

// EOF