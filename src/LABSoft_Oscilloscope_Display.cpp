#include "LABSoft_Oscilloscope_Display.h"

#include <vector>
#include <cmath>

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
    m_channel_signals (m_number_of_channels,
                       m_number_of_samples,
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
update ()
{
  if (m_is_function_generator_mode)
    {
      // // generate waveforms of all channels
      // for (int a = 0; a < m_channel_signals; a++) 
      //   {
      //     ;
      //   }
    }
}

void LABSoft_Oscilloscope_Display:: 
normalize_all_channels_raw_data ()
{
  float scaler = m_adc_reference_voltage / (m_adc_resolution_in_integer >> 1);
  int16_t ref_half = m_adc_resolution_in_integer >> 1;

  for (int a = 0; a < m_channel_signals.size (); a++) 
  {
    Channel_Signal *chn = &(m_channel_signals.m_channel_signal_vector[a]);

    if (chn->m_is_enabled)
    {
      for (int b = 0; b < (chn->m_values.size ()); b++)
      {
        uint16_t temp;
        uint16_t data;

        if (a == 0)
        {
          temp = chn->m_raw_values[b] & 0x0000FFFF;
        }
        else if (a == 1)
        {
          temp = (chn->m_raw_values[b] & 0xFFFF0000) >> 16;
        }
        else 
        {

        }

        data = ((temp << 6) | (temp >> 10)) & 0x0FFF;
        chn->m_values[b] = (static_cast<int16_t>(data) - ref_half) * (scaler);

        //printf (BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "\n",
        //   BYTE_TO_BINARY (data >> 8), BYTE_TO_BINARY (data));
      }
    }
  }
}

void LABSoft_Oscilloscope_Display:: 
normalize_channels_data_to_display ()
{
  if (m_is_enabled) 
  {
    for (int a = 0; a < m_channel_signals.size (); a++ ) 
    {
      Channel_Signal *chn = &(m_channel_signals.m_channel_signal_vector[a]);

      if (chn->m_is_enabled) 
      {
        // calculate sample skip interval
        float skip_interval = static_cast<float>(chn->m_values.size ()) / static_cast<float>(w ());
        float y_mid_pixel = y () + (static_cast<float>(h ()) / 2.0);
        float scaled_max_amplitude = m_number_of_rows * chn->m_volts_per_division;

        for (int b = 0; b < w (); b++) 
        {
          // pixel point x coord
          chn->m_pixel_points[b][0] = x () + b;
      
          int index = static_cast<int>(b * skip_interval);
          double sample_value = chn->m_values[index] + (chn->m_vertical_offset * 2);

          if (sample_value == 0.0) 
          {
            chn->m_pixel_points[b][1] = y_mid_pixel;
          } else 
          {
            chn->m_pixel_points[b][1] = 
              y_mid_pixel - (sample_value * ((static_cast<float>(h ()) / 2.0) / 
                scaled_max_amplitude));
            
            //printf ("y: %d\n",chn->m_pixel_points[b][1]);
          }
        }
      }
    }
  }
}

void LABSoft_Oscilloscope_Display:: 
draw_channels_signals () 
{
  for (int a = 0; a < m_channel_signals.size (); a++) {
    Channel_Signal *chan = &(m_channel_signals.m_channel_signal_vector[a]);

    if (chan->m_is_enabled) 
    {
      std::vector<std::vector<int>> *pp = &(chan->m_pixel_points);
      
      fl_color (m_channels_graph_color[a]);

      for (int b = 0; b < (w () - 1); b++) {
        fl_line ((*pp)[b][0], (*pp)[b][1], (*pp)[b + 1][0], (*pp)[b + 1][1]);
      }
    }
  }
}

// --- FUNCTION GENERATOR SECTION --- 

void LABSoft_Oscilloscope_Display:: 
enable_function_generator_mode ()
{
  // change number of channels to 1
  //number_of_channels (1);

  // already default set to use channel 1
  
  // // change pixel buffer size to widget width
  // for (int a = 0; a < m_channel_signals.m_number_of_channels; a++)
  //   {
  //     &(m_channel_signals.m_channel_signal_vector[a])->m_pixel_points.
  //       resize (w (), std::vector<int> (2));
  //   }


  // update channel signals
  //m_channel_signals->number_of_channels_all (1);
}

// int LABSoft_Oscilloscope_Display:: 
// generate_waveform (WaveType wave_type, int channel)
// {
//   std::vector<float>  values;  
//   float               x_scaler = 1 / (static_cast<float>(w ()) /
//                         static_cast<float>(m_number_of_columns));

//   // error out if channel argument is greater than number of channels
//   if (channel > m_channel_signals.size ())
//     return -1;
//   else
//     Channel_Signal *chn = &(m_channel_signals.m_channel_signal_vector[channel]);
  
//   for (int a = 0; a < w (); a++)
//     {
//       switch (m_wave_type)
//         {
//           case (SINE):
//             values[a] = (m_function_amplitude / m_function_volts_per_division) * 
//               sin ((2 * m_pi * m_function_frequency * a * m_time_per_division * x_scaler) 
//                 + (m_function_phase * m_pi / 180.0)) + m_function_y_offset;
//             break;

//           case (SQUARE):
//             values[a] = copysign ((m_function_amplitude / m_function_volts_per_division), sin ((2 * m_pi * m_function_frequency * 
//               a * m_time_per_division * x_scaler) + (m_function_phase * m_pi / 180.0))) + m_function_y_offset;
//             break;

//           case (TRIANGLE):
//             values[a] = ((2 * (m_function_amplitude / m_function_volts_per_division)) /m_pi) * asin (sin ((2 * m_pi / 
//               (1.0 / m_function_frequency)) * a * x_scaler + (m_function_phase * m_pi / 180.0))) + m_function_y_offset;        
//             break;

//           case (DC):
//             values[a] = (m_function_amplitude / m_function_volts_per_division) + m_function_y_offset;
//             break;

//           default:
//             values[a] = 0.0;
//             break;
//         } 
//     }  
  
//   return 1;
// }

int LABSoft_Oscilloscope_Display:: 
generate_waveform (WaveType wave_type, int channel)
{
  std::vector<float>  values;  
  float               x_scaler = 1 / (static_cast<float>(w ()) /
                        static_cast<float>(m_number_of_columns));

  // error out if channel argument is greater than number of channels
  if (channel > m_channel_signals.size ())
    return -1;
  else
    Channel_Signal *chn = &(m_channel_signals.m_channel_signal_vector[channel]);
  
  for (int a = 0; a < w (); a++)
    {
      switch (m_wave_type)
        {
          case (SINE):
            values[a] = (m_function_amplitude / m_function_volts_per_division) * 
              sin ((2 * m_pi * m_function_frequency * a * m_time_per_division * x_scaler) 
                + (m_function_phase * m_pi / 180.0)) + m_function_y_offset;
            break;

          case (SQUARE):
            values[a] = copysign ((m_function_amplitude / m_function_volts_per_division), sin ((2 * m_pi * m_function_frequency * 
              a * m_time_per_division * x_scaler) + (m_function_phase * m_pi / 180.0))) + m_function_y_offset;
            break;

          case (TRIANGLE):
            values[a] = ((2 * (m_function_amplitude / m_function_volts_per_division)) /m_pi) * asin (sin ((2 * m_pi / 
              (1.0 / m_function_frequency)) * a * x_scaler + (m_function_phase * m_pi / 180.0))) + m_function_y_offset;        
            break;

          case (DC):
            values[a] = (m_function_amplitude / m_function_volts_per_division) + m_function_y_offset;
            break;

          default:
            values[a] = 0.0;
            break;
        } 
    }  
  
  return 1;
}

// getter
double LABSoft_Oscilloscope_Display:: 
volts_per_division (int channel)
{
  return (m_channel_signals.m_channel_signal_vector[channel].m_volts_per_division);
}

// setters
void LABSoft_Oscilloscope_Display:: 
volts_per_division (int channel, double value)
{
  m_channel_signals.m_channel_signal_vector[channel].m_volts_per_division = value;
}

void LABSoft_Oscilloscope_Display:: 
vertical_offset (int channel, double value)
{
  m_channel_signals.m_channel_signal_vector[channel].m_vertical_offset = value;
}
// EOF

