#include "Channel_Signal.h"

#include <iostream>

Channel_Signal:: 
Channel_Signal (int number_of_samples, 
                int display_width)
: m_values (number_of_samples, 0.0),
  m_pixel_points (number_of_samples, std::vector<int> (display_width, 0))
{
 
}