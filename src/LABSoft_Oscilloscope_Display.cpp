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
  : Fl_Widget (X, Y, W, H, label),
    m_channel_signals (LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS,
                       LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES,
                       w ())                                                    
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
  draw_channels_signals ();
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
load_and_process_samples (Channel_Signals *_Channel_Signals)
{
  if (m_is_enabled)
  {
    for (int channel = 0; channel < _Channel_Signals->m_chans.size (); channel++)
    {
      Channel_Signal *chn = &(_Channel_Signals->m_chans[channel]);

      if (chn->is_enabled ())
      {
        // double time_per_division    = chn->osc.time_per_division;
        // double sampling_rate        = chn->osc.sampling_rate;
        // double sampling_rate_period = 1 / sampling_rate;

        // if (time_per_division == sampling_rate_period)
        // {

        // }
        // else if (time_per_division < sampling_rate_period)
        // {

        // }


        // // calculate sample skip interval
        // float skip_interval = static_cast<float>(chn->m_values.size ()) / static_cast<float>(w ());
        // float y_mid_pixel = y () + (static_cast<float>(h ()) / 2.0);
        // float scaled_max_amplitude = m_number_of_rows * chn->m_voltage_per_division;

        // for (int b = 0; b < w (); b++) 
        // {
        //   // pixel point x coord
        //   chn->m_pixel_points[b][0] = x () + b;
      
        //   int index = static_cast<int>(b * skip_interval);
        //   double sample_value = chn->m_values[index] + (chn->m_vertical_offset * 2);

        //   if (sample_value == 0.0) 
        //   {
        //     chn->m_pixel_points[b][1] = y_mid_pixel;
        //   } else 
        //   {
        //     chn->m_pixel_points[b][1] = 
        //       y_mid_pixel - (sample_value * ((static_cast<float>(h ()) / 2.0) / 
        //         scaled_max_amplitude));
            
        //     //printf ("y: %d\n",chn->m_pixel_points[b][1]);
        //   }
      }
    }
  }
}

void LABSoft_Oscilloscope_Display:: 
draw_channels_signals () 
{
  for (int a = 0; a < m_channel_signals.size (); a++) {
    Channel_Signal *chan = &(m_channel_signals.m_chans[a]);

    if (chan->is_enabled ()) 
    {
      std::vector<std::vector<int>> *pp = &(chan->m_pixel_points);
      
      fl_color (m_channels_graph_color[a]);

      for (int b = 0; b < (w () - 1); b++) {
        fl_line ((*pp)[b][0], (*pp)[b][1], (*pp)[b + 1][0], (*pp)[b + 1][1]);
      }
    }
  }
}

// EOF