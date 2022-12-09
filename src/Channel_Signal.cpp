#include "Channel_Signal.h"

#include <iostream>

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

// eof