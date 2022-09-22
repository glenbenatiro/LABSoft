#ifndef LAB_FUNCTION_GENERATOR
#define LAB_FUNCTION_GENERATOR

#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Light_Button.H>

#include "LAB_Globals.h"
#include "LAB_Core.h"
#include "LABSoft_Oscilloscope_Display.h"
#include "LABSoft_Oscilloscope_Display_Group.h"

class LAB_Function_Generator : public LAB_Core
{
  public:
    float m_frequency,
          m_period,
          m_amplitude,
          m_y_offset, 
          m_duty_cycle,
          m_phase;

    WaveType m_wave_type;

    bool m_flag_is_running = false;
  
    LAB_Function_Generator ();
};

#endif
