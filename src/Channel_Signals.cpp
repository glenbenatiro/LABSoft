#include "Channel_Signals.h"

#include "Defaults.h"

// Channel_Signal
Channel_Signal:: 
Channel_Signal (int number_of_samples)
{
  // defaults
  m_function_wave_type  = CHANNEL_SIGNAL_FUNCTION_WAVE_TYPE;
  m_function_amplitude  = CHANNEL_SIGNAL_FUNCTION_AMPLITUDE;
  m_function_frequency  = CHANNEL_SIGNAL_FUNCTION_FREQUENCY;
  m_function_phase      = CHANNEL_SIGNAL_FUNCTION_PHASE; 
  m_function_y_offset   = CHANNEL_SIGNAL_FUNCTION_Y_OFFSET; 
  m_function_duty_cycle = CHANNEL_SIGNAL_FUNCTION_DUTY_CYCLE; 
  m_volts_per_division  = CHANNEL_SIGNAL_VOLTS_PER_DIVISION;
  m_time_per_division   = CHANNEL_SIGNAL_TIME_PER_DIVISION;

  // for oscilloscope display
  m_number_of_samples_to_use = number_of_samples;

  m_samples.resize (number_of_samples, 0.0);
  m_pixel_points.resize (number_of_samples, std::vector<int> (2));
}

// getters
bool Channel_Signal:: 
is_enabled ()
{
  return m_is_enabled;
}

int Channel_Signal:: 
number_of_samples_to_use ()
{
  return m_number_of_samples_to_use;
}

float Channel_Signal:: 
sample_rate ()
{
  return m_sample_rate;
}

float Channel_Signal:: 
function_duty_cycle ()
{
  return m_function_duty_cycle;
}

std::vector<float>* Channel_Signal:: 
samples ()
{
  return &m_samples;
}

std::vector<std::vector<int>>* Channel_Signal:: 
pixel_points ()
{
  return &m_pixel_points;
}

// setters
void Channel_Signal:: 
is_enabled (bool value)
{
  m_is_enabled = value;
}

void Channel_Signal:: 
function_wave_type (WaveType wave_type)
{
  m_function_wave_type = wave_type;
}


void Channel_Signal:: 
function_amplitude (float value)
{
  m_function_amplitude = value;
}

void Channel_Signal:: 
function_frequency (float value)
{
  m_function_frequency = value;
}

void Channel_Signal:: 
function_phase (float value)
{
  m_function_phase = value;
}

void Channel_Signal:: 
function_y_offset (float value)
{
  m_function_y_offset = value;
}

void Channel_Signal:: 
function_duty_cycle (float value)
{
  m_function_duty_cycle = value;
}



// ---



// Channel_Signals
Channel_Signals::
Channel_Signals (int  number_of_channels, 
                 int  max_number_of_samples,
                 bool is_sample_sharing_enabled)
{
  m_number_of_channels        = number_of_channels;
  m_max_number_of_samples     = max_number_of_samples;
  m_is_sample_sharing_enabled = is_sample_sharing_enabled;

  if (m_is_sample_sharing_enabled)
    {
      m_channel_signal_vector.resize (m_number_of_channels, 
        (m_max_number_of_samples / m_number_of_channels));
    }
  else 
    {
      m_channel_signal_vector.resize (m_number_of_channels, m_max_number_of_samples);
    }
}

// getters
int Channel_Signals:: 
number_of_channels ()
{
  if (m_channel_signal_vector.size () == m_number_of_channels)
    return m_number_of_channels;
  else 
    return -1;
}

int Channel_Signals:: 
max_number_of_samples ()
{
  return m_max_number_of_samples;
}

bool Channel_Signals:: 
is_sample_sharing_enabled ()
{
  return m_is_sample_sharing_enabled;
}

Channel_Signal* Channel_Signals:: 
channel_signal_vector (int channel)
{
  return &(m_channel_signal_vector[channel]);
}

// setters
void Channel_Signals:: 
number_of_channels (int value)
{
  m_number_of_channels = value;
  m_channel_signal_vector.resize (value, Channel_Signal (0));

  for (int a = 0; a < m_channel_signal_vector.size (); a++)
    {
      if (m_is_sample_sharing_enabled)
        {
          m_channel_signal_vector[a].samples()->
            resize (m_max_number_of_samples / m_number_of_channels, 0.0);
        }
      else 
        {
          m_channel_signal_vector[a].samples()->
            resize (m_max_number_of_samples, 0.0);
        }
    }
}

void Channel_Signals:: 
max_number_of_samples (int value)
{
  m_max_number_of_samples = value;

  for (int a = 0; a < m_channel_signal_vector.size (); a++)
    {
      if (m_is_sample_sharing_enabled)
        {
          m_channel_signal_vector[a].samples()->
            resize (m_max_number_of_samples / m_number_of_channels, 0.0);
        }
      else 
        {
          m_channel_signal_vector[a].samples()->
            resize (m_max_number_of_samples, 0.0);
        }
    }
}