#ifndef LAB_DEFAULTS_H
#define LAB_DEFAULTS_H

#include "LAB_Constants.h"

namespace LABD
{
  namespace LOGAN 
  {
    // Horizontal
    constexpr double    SAMPLING_RATE               = 5'000; // Hz
    constexpr unsigned  SAMPLES                     = LABC::LOGAN::MAX_SAMPLES;
    constexpr double    TIME_PER_DIVISION           = SAMPLES / (SAMPLING_RATE * LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS);  
    constexpr double    HORIZONTAL_OFFSET           = 0.0;
    
    // Mode
    constexpr LABE::LOGAN::MODE MODE                = (TIME_PER_DIVISION < LABC::LOGAN::MIN_TIME_PER_DIVISION_SCREEN) ? 
                                                        LABE::LOGAN::MODE::REPEATED : LABE::LOGAN::MODE::SCREEN;
    
    // Mode
    constexpr LABE::LOGAN::TRIG::MODE TRIGGER_MODE  = LABE::LOGAN::TRIG::MODE::NONE;
    constexpr double CHECK_TRIGGER_SLEEP_PERIOD     = SAMPLING_RATE / 5.0;
  };

  namespace LOGAN_DISPLAY
  {
    constexpr int DISPLAY_WIDTH = 1300;
    constexpr int CHANNEL_WIDTH = DISPLAY_WIDTH;
    constexpr int CHANNEL_GRAPH_WIDTH = CHANNEL_WIDTH - LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH;
  };
};

#endif