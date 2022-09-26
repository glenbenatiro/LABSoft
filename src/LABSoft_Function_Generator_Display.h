#ifndef LABSOFT_FUNCTION_GENERATOR_DISPLAY_H
#define LABSOFT_FUNCTION_GENERATOR_DISPLAY_H

#include <cmath>

#include "LAB_Globals.h"
#include "LABSoft_Oscilloscope_Display.h"

class LABSoft_Function_Generator_Display : public LABSoft_Oscilloscope_Display
{
  private:
    const double m_pi = 3.1415926535897932384626433832795028841971693993751058209;

    float m_amplitude,
          m_frequency,
          m_phase,
          m_y_offset,
          m_time_per_division,
          m_volts_per_division;

  public:
         LABSoft_Function_Generator_Display (int X, int Y, int W, int H);
    int  generate_waveform (WaveType wave_type, int channel);

    // setters
    void amplitude (float amplitude);
    void frequency (float frequency);
    void phase (float phase);
    void y_offset (float y_offset);
    void time_per_division (float time_per_division);
    void volts_per_division (float volts_per_division);

    // getters
    float amplitude ();
    float frequency ();
    float phase ();
    float y_offset ();
    float time_per_division ();
    float volts_per_division ();

#endif