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
  draw_box (FL_FLAT_BOX, m_background_color);
  draw_grid ();
  draw_channels ();
}

void LABSoft_Oscilloscope_Display:: 
draw_grid ()
{
  // set color
  fl_color (m_grid_color);

  // draw grid outer box
  fl_line_style (FL_SOLID, 0, NULL);
  fl_line (x (), y(), x () + w (), y ());                 // up
  fl_line (x () + w (), y (), x () + w (), y () + h ());  // right
  fl_line (x () + w (), y () + h (), x (), y () + h ());  // down
  fl_line (x (), y () + h (), x (), y ());                // left

  // draw rows
  for (int a = 0; a < (m_number_of_rows - 1); a++)
    {
      if (a == ((m_number_of_rows / 2) - 1))
        fl_line_style (FL_DASH, 0, NULL);
      else 
        fl_line_style (FL_DOT, 0, NULL);

      int Y = round ((a + 1) * ((float)h () / (float)m_number_of_rows)) + y ();
      fl_line (x (), Y, x () + w (), Y);
    }

  // draw columns 
  for (int a = 0; a < (m_number_of_columns - 1); a++)
    {
      if (a == ((m_number_of_columns / 2) - 1))
        fl_line_style (FL_DASH, 0, NULL);
      else 
        fl_line_style (FL_DOT, 0, NULL);

      int X = round ((a + 1) * ((float)w () / (float)m_number_of_columns)) + x ();
      fl_line (X, y (), X, y () + h ());
    }
  
  // reset color
  fl_line_style (0);
  fl_color (m_default_color);
}


void LABSoft_Oscilloscope_Display:: 
load_channel_signals (Channel_Signals *_Channel_Signals)
{
  double osc_disp_vert_half     = h () / 2;
  double osc_disp_vert_midline  = y () + osc_disp_vert_half;
  
  for (int a = 0; a < _Channel_Signals->m_chans.size (); a++)
  {
    if (_Channel_Signals->m_chans[a].is_enabled ())
    {
      Channel_Signal_Oscilloscope *osc  = &(_Channel_Signals->m_chans[a].osc);
      std::vector<std::vector<int>> *pp = &(_Channel_Signals->m_chans[a].osc.
        pixel_points);
      
      double vert_scaler = (osc_disp_vert_half) / 
        ((LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS / 2) *
          osc->volts_per_division);

      printf ("hello!");

      if (osc->time_per_division < LAB_OSCILLOSCOPE_MAX_TIME_PER_DIV_ZOOM)
      {
        double volt_samp_x_off = (LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES -
          osc->samples) / 2;

        if (osc->samples > w ())
        {
          // if (pp->size () != w ())
          // {
          //   pp->resize (w (), std::vector<int>(2));
          // }

          double sample_skip = static_cast<double>(osc->samples - 1) / 
          static_cast<double>(w () - 1);
                  
          for (int b = 0; b < w (); b++)
          {
            (*pp)[b][0] = x () + b;

            double samp_value = (osc->voltage_samples[volt_samp_x_off + 
              (sample_skip * b)]) + (osc->vertical_offset);

            if (samp_value == 0.0)
            {
              (*pp)[b][1] = osc_disp_vert_midline;
            }
            else
            {
              (*pp)[b][1] = osc_disp_vert_midline - (samp_value * vert_scaler);
            }
          }
        }
        else if (osc->samples == w ())
        {
          // if (pp->size () != w ())
          // {
          //   pp->resize (w (), std::vector<int>(2));
          // }

          for (int b = 0; b < w (); b++)
          {
            (*pp)[b][0] = x () + b;

            double samp_value = (osc->voltage_samples[volt_samp_x_off + b]) +
              (osc->vertical_offset);

            if (samp_value == 0.0)
            {
              (*pp)[b][1] = osc_disp_vert_midline;
            }
            else
            {
              (*pp)[b][1] = osc_disp_vert_midline - (samp_value * vert_scaler);
            }
          }
        }
        else // osc->samples < w ()
        { 
          // if (pp->size () != osc->samples)
          // {
          //   pp->resize (osc->samples, std::vector<int>(2));
          // }

          double x_pixel_scaler = ((w () - 1) / (osc->samples -1 ));

          for (int b = 0; b < osc->samples; b++)
          {
            (*pp)[b][0] = x () + (b * x_pixel_scaler);

            double samp_value = (osc->voltage_samples[volt_samp_x_off + b]) +
              (osc->vertical_offset);
            
            if (samp_value == 0.0)
            {
              (*pp)[b][1] = osc_disp_vert_midline;
            }
            else
            {
              (*pp)[b][1] = osc_disp_vert_midline - (samp_value * vert_scaler);
            }
          }
        }
      }
      else 
      {
        // if (pp->size () != w ())
        // {
        //   pp->resize (w (), std::vector<int>(2));
        // }

        double sample_skip = static_cast<double>(osc->voltage_samples.size () - 1) / 
        static_cast<double>(w () - 1);
                
        for (int b = 0; b < w (); b++)
        {
          (*pp)[b][0] = x () + b;

          double samp_value = (osc->voltage_samples[sample_skip * b]) + 
            (osc->vertical_offset);

          if (samp_value == 0.0)
          {
            (*pp)[b][1] = osc_disp_vert_midline;
          }
          else
          {
            (*pp)[b][1] = osc_disp_vert_midline - (samp_value * vert_scaler);
          }
        }
      }
    }
  }
}

void LABSoft_Oscilloscope_Display:: 
draw_channels () 
{
  if (m_channel_signals)
  {
    fl_push_clip (x (), y (), w (), h ());

    for (int a = 0; a < m_channel_signals->m_chans.size (); a++)
    {
      if (m_channel_signals->m_chans[a].is_enabled ())
      {
        std::vector<std::vector<int>> *pp = &(m_channel_signals->m_chans[a].
          osc.pixel_points);

        fl_color (m_channels_graph_color[a]);

        for (int b = 0; b < (w () - 1); b++)
        {
          fl_line ((*pp)[b][0], (*pp)[b][1], (*pp)[b + 1][0], (*pp)[b + 1][1]);
        }
      }
    }

    fl_pop_clip();
  }
}

// EOF