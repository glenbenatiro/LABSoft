#include "Channel_Signal.h"

Channel_Signal:: 
Channel_Signal (int number_of_samples, 
                int display_width) 
  : m_pixel_points (display_width, std::vector<int> (2, 0))
{

}