#include "Channel_Signals.h"

Channel_Signal:: 
Channel_Signal (int number_of_samples, 
                int display_width)
: m_values (number_of_samples, 0.0),
  m_pixel_points (number_of_samples, std::vector<int> (display_width, 0))
{
 
}

void Channel_Signal:: 
enable ()
{
  m_is_enabled = true;
}

void Channel_Signal:: 
disable ()
{
  m_is_enabled = false;
}

bool Channel_Signal:: 
is_enabled ()
{
  return m_is_enabled;
}

// --- Channel_Signals ---

Channel_Signals:: 
Channel_Signals (int number_of_channels,
                 int number_of_samples,
                 int display_width)
: m_channel_signal_vector (number_of_channels, Channel_Signal (number_of_samples, display_width))
{

}

int Channel_Signals:: 
size ()
{
  return m_channel_signal_vector.size ();
}