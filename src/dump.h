double LABSoft_Oscilloscope_Display::clockToMilliseconds(clock_t ticks){
    // units/(units/time) => time (seconds) * 1000 = milliseconds
    return (ticks/(double)CLOCKS_PER_SEC)*1000.0;
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
  //     &(m_channel_signals.m_chans[a])->m_pixel_points.
  //       resize (w (), std::vector<int> (2));
  //   }


  // update channel signals
  //m_channel_signals.number_of_channels_all (1);
}

// int LABSoft_Oscilloscope_Display:: 
// generate_waveform (LE_WAVE_TYPE wave_type, int channel)
// {
//   std::vector<float>  values;  
//   float               x_scaler = 1 / (static_cast<float>(w ()) /
//                         static_cast<float>(m_number_of_columns));

//   // error out if channel argument is greater than number of channels
//   if (channel > m_channel_signals.size ())
//     return -1;
//   else
//     Channel_Signal *chn = &(m_channel_signals.m_chans[channel]);
  
//   for (int a = 0; a < w (); a++)
//     {
//       switch (m_wave_type)
//         {
//           case (LE_WAVE_TYPE_SINE):
//             values[a] = (m_function_amplitude / m_function_voltage_per_division) * 
//               sin ((2 * m_pi * m_function_frequency * a * m_time_per_division * x_scaler) 
//                 + (m_function_phase * m_pi / 180.0)) + m_function_y_offset;
//             break;

//           case (LE_WAVE_TYPE_SQUARE):
//             values[a] = copysign ((m_function_amplitude / m_function_voltage_per_division), sin ((2 * m_pi * m_function_frequency * 
//               a * m_time_per_division * x_scaler) + (m_function_phase * m_pi / 180.0))) + m_function_y_offset;
//             break;

//           case (LE_WAVE_TYPE_TRIANGLE):
//             values[a] = ((2 * (m_function_amplitude / m_function_voltage_per_division)) /m_pi) * asin (sin ((2 * m_pi / 
//               (1.0 / m_function_frequency)) * a * x_scaler + (m_function_phase * m_pi / 180.0))) + m_function_y_offset;        
//             break;

//           case (DC):
//             values[a] = (m_function_amplitude / m_function_voltage_per_division) + m_function_y_offset;
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
  if (channel > LAB_FUNCTION_GENERATOR::NUMBER_OF_CHANNELS)
    return -1;
  else
    Channel_Signal *chn = &(m_channel_signals.m_chans[channel]);
  
  for (int a = 0; a < w (); a++)
    {
      switch (m_wave_type)
        {
          case (LE_WAVE_TYPE_SINE):
            values[a] = (m_function_amplitude / m_function_voltage_per_division) * 
              sin ((2 * m_pi * m_function_frequency * a * m_time_per_division * x_scaler) 
                + (m_function_phase * m_pi / 180.0)) + m_function_y_offset;
            break;

          case (LE_WAVE_TYPE_SQUARE_FULL):
            values[a] = copysign ((m_function_amplitude / m_function_voltage_per_division), sin ((2 * m_pi * m_function_frequency * 
              a * m_time_per_division * x_scaler) + (m_function_phase * m_pi / 180.0))) + m_function_y_offset;
            break;

          case (LE_WAVE_TYPE_TRIANGLE):
            values[a] = ((2 * (m_function_amplitude / m_function_voltage_per_division)) /m_pi) * asin (sin ((2 * m_pi / 
              (1.0 / m_function_frequency)) * a * x_scaler + (m_function_phase * m_pi / 180.0))) + m_function_y_offset;        
            break;

          case (DC):
            values[a] = (m_function_amplitude / m_function_voltage_per_division) + m_function_y_offset;
            break;

          default:
            values[a] = 0.0;
            break;
        } 
    }  
  
  return 1;
}