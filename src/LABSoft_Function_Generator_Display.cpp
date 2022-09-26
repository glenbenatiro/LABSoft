#include "LABSoft_Function_Generator_Display"

LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display (int X, 
                                    int Y, 
                                    int W, 
                                    int H) : LABSoft_Oscilloscope_Display (X,
                                                                           Y,
                                                                           W,
                                                                           H)
{
  // set channel number to 1
  LABSoft_Oscilloscope_Display_set_number_of_channels (1);
}

int LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_generate_waveform (WaveType wave_type, int channel)
{
  // error out if channel argument is greataer than number of channels
  if (channel > m_channel_signals.size ())
    return -1;
  
  std::vector<float> values;
  values.resize (m_channel_signals[channel - 1].size (), 0.0);

  float x_scaler = 1 / (static_cast<float>(w ()) / static_cast<float>(m_number_of_columns));

  for (int a = 0; a < values.size (); a++)
    {
      switch (m_wave_type)
        {
          case (SINE):
            values[a] = (m_amplitude / m_volts_per_division) * 
              sin ((2 * m_pi * m_frequency * a * m_time_per_division * x_scaler) 
                + (m_phase * m_pi / 180.0)) + m_y_offset;
            break;

          case (SQUARE):
            values[a] = copysign ((m_amplitude / m_volts_per_division), sin ((2 * m_pi * m_frequency * 
              a * m_time_per_division * x_scaler) + (m_phase * m_pi / 180.0))) + m_y_offset;
            break;

          case (TRIANGLE):
            values[a] = ((2 * (m_amplitude / m_volts_per_division)) /m_pi) * asin (sin ((2 * m_pi / 
              (1.0 / m_frequency)) * a * x_scaler + (m_phase * m_pi / 180.0))) + m_y_offset;        
            break;

          case (DC):
            values[a] = (m_amplitude / m_volts_per_division) + m_y_offset;
            break;

          default:
            values[a] = 0.0;
            break;
        } 
    }  
  
  return 1;
}

// setters
void LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_amplitude (float amplitude)
{
  m_amplitude = amplitude;
}

void LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_frequency (float frequency)
{
  m_frequency = frequency;
}

void LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_phase (float phase)
{
  m_phase = phase;
}

void LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_y_offset (float y_offset)
{
  m_y_offset = y_offset;
}

void LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_time_per_division (float time_per_division)
{
  m_time_per_division = time_per_division;
}

void LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_volts_per_division (float volts_per_division)
{
  m_volts_per_division = volts_per_division;
}

// getters
float LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_amplitude ()
{
  return m_amplitude;
}

float LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_frequency ()
{
  return m_frequency;
}

float LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_phase ()
{
  return m_phase;
}

float LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_y_offset ()
{
  return m_y_offset;
}

float LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_time_per_division ()
{
  return m_time_per_division;
}

float LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_volts_per_division ()
{
  return m_volts_per_division;
}

