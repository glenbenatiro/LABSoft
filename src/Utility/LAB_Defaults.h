#ifndef LAB_DEFAULTS_H
#define LAB_DEFAULTS_H

#include "LAB_Constants.h"

namespace LABD
{
  namespace LOGAN 
  {
    constexpr double    SAMPLING_RATE     = 40'000; // Hz
    constexpr unsigned  SAMPLES           = LABC::LOGAN::MAX_SAMPLES;
    constexpr double    TIME_PER_DIVISION = SAMPLES / (SAMPLING_RATE * LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS);  
    constexpr double    HORIZONTAL_OFFSET = 0.0;
    
    constexpr LABE::LOGAN::MODE MODE      = (TIME_PER_DIVISION < LABC::LOGAN::MIN_TIME_PER_DIVISION_SCREEN) ? 
                                              LABE::LOGAN::MODE::REPEATED : LABE::LOGAN::MODE::SCREEN;
  };

  namespace LOGAN_DISPLAY
  {
    constexpr int DISPLAY_WIDTH = 1300;
    constexpr int CHANNEL_WIDTH = DISPLAY_WIDTH;
    constexpr int CHANNEL_GRAPH_WIDTH = CHANNEL_WIDTH - LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH;
  };
};

#endif