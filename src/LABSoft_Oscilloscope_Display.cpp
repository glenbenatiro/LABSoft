#include "LABSoft_Oscilloscope_Display.h"

#include <vector>
#include <cmath>

#include <FL/Enumerations.H>

#include "Defaults.h"

LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display (int X,
                              int Y,
                              int W,
                              int H,
                              const char *label = 0) : Fl_Widget (X,                                        
                                                                  Y,
                                                                  W,
                                                                  H,
                                                                  label)
{ 
  // load defaults
  // for oscilloscope core
  m_number_of_rows              = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS;
  m_number_of_columns           = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS;
  m_number_of_channels          = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_CHANNELS;

  // for 

  // for function generator functionality
  m_function_amplitude          = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_AMPLITUDE;
  m_function_frequency          = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_FREQUENCY;
  m_function_phase              = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_PHASE;
  m_function_y_offset           = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_Y_OFFSET;

  
  m_grid_color                  = LABSOFT_OSCILLOSCOPE_DISPLAY_GRID_COLOR;
  m_background_color            = LABSOFT_OSCILLOSCOPE_DISPLAY_BACKGROUND_COLOR;
  m_default_color               = LABSOFT_OSCILLOSCOPE_DISPLAY_DEFAULT_COLOR;
  m_max_number_of_channels      = LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_NUMBER_OF_CHANNELS;
  m_max_number_of_samples       = LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_NUMBER_OF_SAMPLES;
  m_function_generator_channel  = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_GENERATOR_CHANNEL_NUMBER;
  m_channel_colors              = LABSOFT_OSCILLOSCOPE_DISPLAY_CHANNEL_COLORS;

  m_channel_signals = new Channel_Signals (m_number_of_channels,
                                        m_max_number_of_samples,
                                        m_enable_sample_sharing);

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
  
  if (m_is_enabled)
    draw_channel_signals ();
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
draw_channel_signals () const
{
  if (m_is_enabled)
    {
      for (int a = 0; a < m_channel_signals->number_of_channels (); a++)  
        {
          Channel_Signal *chan = m_channel_signals->channel_signal_vector (a);

          if (chan->is_enabled ())
            {
              std::vector<std::vector<int>> *pp = chan->pixel_points ();

              for (int b = 0; b < (w () - 1); b++)
                fl_line ((*pp)[b][0], (*pp)[b][1], (*pp)[b + 1][0], (*pp)[b + 1][1]);
            }
        } 
    }
}

void LABSoft_Oscilloscope_Display:: 
normalize_channel_signals ()
{
  if (m_is_enabled)
    {
      for (int a = 0; a < m_channel_signals->number_of_channels (); a++)  
        {
          // pointer to specific Channel_Signal (chan 1, chan 2, chan 3...)
          Channel_Signal *chan = m_channel_signals->channel_signal_vector (a);

          if (chan->is_enabled ())
            {
              // pointer to the specific Channel_Signal's pixel points
              std::vector<std::vector<int>>* pp = chan->pixel_points ();

              // pointer to the specific Channel_Signal's sample values
              std::vector<float> *val = chan->samples ();
              
              // calculate y mid pixel, for zero
              float y_mid_pixel =  y () + ((float)h () / 2.0);
              float scaled_max_amplitude = m_number_of_rows * m_volts_per_division;

              for (int b = 0; (b < chan->samples ()->size ()) && (b < pp->size ()); b++)
                {
                  (*pp)[b][0] = b;

                  if ((*val)[b] == 0.0)
                    (*pp)[b][1] = y_mid_pixel;
                  else
                    (*pp)[b][1] = y_mid_pixel - ((*val)[b] * (((float)h () / 2.0) / scaled_max_amplitude));
                }
            }
        } 
    }
}

// getters
Channel_Signals* LABSoft_Oscilloscope_Display:: 
channel_signals ()
{
  return m_channel_signals;
}

// setters
void LABSoft_Oscilloscope_Display:: 
is_enabled (bool value)
{
  m_is_enabled = value;
}

void LABSoft_Oscilloscope_Display:: 
number_of_channels (int value)
{
  // change internal number of channels count
  m_number_of_channels = value;
  
  // change actual number of channels in channel_signals vector
  // for (int a = 0; a < m_channel_signals)
}

void LABSoft_Oscilloscope_Display:: 
max_number_of_samples (int value)
{
  m_max_number_of_samples = value;
}

void LABSoft_Oscilloscope_Display:: 
rows_columns (int number_of_rows,
              int number_of_columns)
{
  m_number_of_rows = number_of_rows;
  m_number_of_columns = number_of_columns;
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

// --- FUNCTION GENERATOR SECTION --- 

void LABSoft_Oscilloscope_Display:: 
enable_function_generator_mode ()
{
  // change number of channels to 1
  number_of_channels (1);

  // already default set to use channel 1
  
  // change pixel buffer size to widget width
  for (int a = 0; a < m_channel_signals->number_of_channels (); a++)
    {
      m_channel_signals->channel_signal_vector (a)->pixel_points ()->
        resize (w (), std::vector<int> (2));
    }


  // update channel signals
  //m_channel_signals->number_of_channels_all (1);
}

int LABSoft_Oscilloscope_Display:: 
generate_waveform (WaveType wave_type, int channel)
{
  std::vector<float>  values;  
  float               x_scaler = 1 / (static_cast<float>(w ()) /
                        static_cast<float>(m_number_of_columns));

  // error out if channel argument is greater than number of channels
  if (channel > m_channel_signals->number_of_channels ())
    return -1;
  else
    Channel_Signal *channel_signal = m_channel_signals->channel_signal_vector 
      (LABSOFT_FUNCTION_GENERATOR_SIGNAL_CHANNEL_NUMBER);
  
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

void LABSoft_Oscilloscope_Display:: 
regendraw ()
{
  if (m_is_enabled)  
    {
      normalize_channel_signals ();
    }
}

// setters
void LABSoft_Oscilloscope_Display::
function_wave_type (WaveType wave_type, int channel)
{
  m_wave_type = wave_type;
}

void LABSoft_Oscilloscope_Display::
function_amplitude (float amplitude)
{
  m_function_amplitude = amplitude;
}

void LABSoft_Oscilloscope_Display::
function_frequency (float frequency)
{
  m_function_frequency = frequency;
}

void LABSoft_Oscilloscope_Display::
function_phase (float phase)
{
  m_function_phase = phase;
}

void LABSoft_Oscilloscope_Display::
function_y_offset (float y_offset)
{
  m_function_y_offset = y_offset;
}

void LABSoft_Oscilloscope_Display::
function_time_per_division (float time_per_division)
{
  m_time_per_division = time_per_division;
}

void LABSoft_Oscilloscope_Display::
function_volts_per_division (float volts_per_division)
{
  m_volts_per_division = volts_per_division;
}

// getters
WaveType LABSoft_Oscilloscope_Display::
function_wave_type ()
{
  return m_wave_type;
}

float LABSoft_Oscilloscope_Display::
function_amplitude ()
{
  return m_function_amplitude;
}

float LABSoft_Oscilloscope_Display::
function_frequency ()
{
  return m_function_frequency;
}

float LABSoft_Oscilloscope_Display::
function_phase ()
{
  return m_function_phase;
}

float LABSoft_Oscilloscope_Display::
function_y_offset ()
{
  return m_function_y_offset;
}

float LABSoft_Oscilloscope_Display::
function_time_per_division ()
{
  return m_time_per_division;
}

float LABSoft_Oscilloscope_Display::
function_volts_per_division ()
{
  return m_volts_per_division;
}

// EOF

