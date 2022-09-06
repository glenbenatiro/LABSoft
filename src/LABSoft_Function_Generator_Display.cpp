#include "LABSoft_Function_Generator_Display.h"

LABSoft_Function_Generator_Display::
LABSoft_Function_Generator_Display ()
{
  m_flag_channel_1_enable = true;
}

// setters
void LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_set_wave_type (WaveType wave_type)
{
  m_wave_type = wave_type;

  LABSoft_Oscilloscope_Display_reload_and_draw ();
}

//
void LABSoft_Function_Generator_Display:: 
LABSoft_Function_Generator_Display_generate_wave (WaveType wave_type)
{
  std::vector<float> values (m_values_buffer_channel_1.size (), 0.0);
  
  // cache value of X (time) scaler, which depends on time per div setting and width width
  float x_scaler = m_time_per_division / (w () / m_number_of_columns);

  for (int a = 0; a < value.size (); a++)
    {
      switch (wave_type)
        {
          case (SINE):
            values[a] = m_amplitude * sin (2 * m_pi * m_frequency * (a * x_scaler));
            break;
          case (SQUARE):
            values[a] = (sin ((a * x_scaler) / (1 / m_frequency) * 2.0 * m_pi) >= 0.0) ? m_amplitude : (m_amplitude * -1);
            break:
          case (TRIANGLE):
            break:
          default:
            break;
        }
    }

  m_values_buffer_channel_1 = values;
}