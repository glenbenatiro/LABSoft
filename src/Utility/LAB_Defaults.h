#ifndef LAB_DEFAULTS_H
#define LAB_DEFAULTS_H

#include "LAB_Constants.h"

namespace LABD
{
  namespace LOGAN 
  {
    constexpr double    HORIZONTAL_OFFSET = 0.0;
    constexpr double    TIME_PER_DIVISION = LABC::LOGAN::SAMPLES / 
                                            (LABC::LOGAN::SAMPLING_RATE * 
                                            LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS);  
    constexpr unsigned  samples            

  };

  namespace LOGAN_DISPLAY
  {
    constexpr int DISPLAY_WIDTH = 1300;
    constexpr int CHANNEL_WIDTH = DISPLAY_WIDTH;
    constexpr int CHANNEL_GRAPH_WIDTH = CHANNEL_WIDTH - LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH;
  };
};

#endif