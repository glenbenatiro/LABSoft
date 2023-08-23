#ifndef LAB_DEFAULTS_H
#define LAB_DEFAULTS_H

#include "LAB_Constants.h"

namespace LABD
{
  namespace OSC
  {
    constexpr bool  IS_ENABLED = true;
  };

  namespace LOGAN 
  {
    // Horizontal
    constexpr double    SAMPLING_RATE                   = 50; // Hz
    constexpr double    SAMPLING_PERIOD                 = 1.0 / SAMPLING_RATE;
    constexpr unsigned  SAMPLES                         = LABC::LOGAN::MAX_SAMPLES;
    constexpr double    TIME_PER_DIVISION               = SAMPLES / (SAMPLING_RATE * LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS);  
    constexpr double    HORIZONTAL_OFFSET               = 0.0;
    
    // mode
    constexpr LABE::LOGAN::MODE MODE                    = (TIME_PER_DIVISION < LABC::LOGAN::MIN_TIME_PER_DIVISION_SCREEN) ? 
                                                          LABE::LOGAN::MODE::REPEATED : LABE::LOGAN::MODE::SCREEN;
    
    // trigger
    constexpr LABE::LOGAN::TRIG::CND TRIGGER_CONDITION  = LABE::LOGAN::TRIG::CND::IGNORE;
    constexpr LABE::LOGAN::TRIG::MODE TRIGGER_MODE      = LABE::LOGAN::TRIG::MODE::NONE;
    constexpr double CHECK_TRIGGER_SLEEP_PERIOD         = SAMPLING_RATE / 5.0;
  };
};

#endif