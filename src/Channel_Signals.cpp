#include "Channel_Signals.h"

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