#ifndef LABSOFT_FUNCTION_GENERATOR_DISPLAY_H
#define LABSOFT_FUNCTION_GENERATOR_DISPLAY_H

#include <FL/Fl_Input_Choice.H>

#include "LABSoft_Oscilloscope_Display.h"

class LABSoft_Function_Generator_Display : public LABSoft_Oscilloscope_Display
{
  private:
    const float m_pi = 3.141592653589; // hard coded, depende on how much accuracy you want
          
          float m_frequency = 1.0,
                m_amplitude = 1.0;
          
    WaveType m_wave_type = SINE;   

  public:
    LABSoft_Function_Generator_Display ();

    // setters
    void LABSoft_Function_Generator_Display_set_wave_type (WaveType wave_type);

    //
    void LABSoft_Function_Generator_Display_generate_wave (WaveType wave_type);
};

#endif