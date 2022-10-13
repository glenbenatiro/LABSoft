#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_H

#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>

#include "LAB_Globals.h"
#include "Channel_Signals.h"

class LABSoft_Oscilloscope_Display : public Fl_Widget
{ 
  public:
    bool  m_is_enabled = false,
          m_enable_sample_sharing = false,
          m_is_function_generator_mode = false;

    int   m_grid_color                  = LABSOFT_OSCILLOSCOPE_DISPLAY_GRID_COLOR,
          m_background_color            = LABSOFT_OSCILLOSCOPE_DISPLAY_BACKGROUND_COLOR,
          m_default_color               = LABSOFT_OSCILLOSCOPE_DISPLAY_DEFAULT_COLOR,
          m_function_generator_channel  = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_GENERATOR_CHANNEL_NUMBER;
     // int     m_channel_colors              = LABSOFT_OSCILLOSCOPE_DISPLAY_CHANNEL_COLORS;

    int   m_number_of_rows     = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS,
          m_number_of_columns  = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS,
          m_number_of_channels = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_CHANNELS;    

    float m_time_per_division,
          m_volts_per_division;

    // --- FUNCTION GENERATOR SECTION ---
    const double m_pi = 3.1415926535897932384626433832795028841971693993751058209;

    float m_function_time_per_division,
          m_function_volts_per_division;

    double  m_function_amplitude = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_AMPLITUDE,
            m_function_frequency = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_FREQUENCY,
            m_function_phase     = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_PHASE,
            m_function_y_offset  = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_Y_OFFSET;
    
    WaveType m_wave_type;
      
    Channel_Signals m_channel_signals;

    // --- OSCILLOSCOPE SECTION ---
    // functions
          LABSoft_Oscilloscope_Display (int X, int Y, int W, int H, const char *label);
         ~LABSoft_Oscilloscope_Display ();

    void  draw ();
    void  draw_grid ();
    void  draw_channel_signals ();
    void  normalize_channel_signals ();
    void  regendraw ();    


    void update ();

    // -- FUNCTION GENERATOR SECTION ---
    int  generate_waveform (WaveType wave_type, int channel);
    void enable_function_generator_mode ();
};

#endif