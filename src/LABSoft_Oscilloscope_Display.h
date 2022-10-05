#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_H

#include <vector>
#include <cmath>

#include <FL/Fl_Widget.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>

#include "LAB_Globals.h"
#include "Channel_Signals.h"

class LABSoft_Oscilloscope_Display : public Fl_Widget
{ 
  protected:
    bool  m_is_enabled = false,
          m_enable_sample_sharing = false,
          m_is_function_generator_mode = false;

    int   m_number_of_rows,
          m_number_of_columns,
          m_background_color,
          m_grid_color,
          m_graph_color,
          m_default_color,
          m_number_of_channels,
          m_max_number_of_channels,
          m_max_number_of_samples;
      
    int  *m_channel_colors;
      

    float m_time_per_division,
          m_volts_per_division;

    // --- FUNCTION GENERATOR SECTION ---
    int   m_function_generator_channel;
    const double m_pi = 3.1415926535897932384626433832795028841971693993751058209;

    float m_function_amplitude,
          m_function_frequency,
          m_function_phase,
          m_function_y_offset,
          m_function_time_per_division,
          m_function_volts_per_division;
    
    WaveType m_wave_type;
      
    Channel_Signals *m_channel_signals;


    //std::vector<std::vector<int>> m_channel_pixel_point_buffer (m_number_of_channels, 
    //  std::vector<int>(m_max_number_of_samples / m_number_of_channels, 0));

  public:
    // --- OSCILLOSCOPE SECTION ---
    // functions
          LABSoft_Oscilloscope_Display (int X, int Y, int W, int H, const char *label);

    void  draw ();
    void  draw_grid ();
    void  draw_channel_signals () const;
    void  normalize_channel_signals ();
    void  regendraw ();    

    // setters
    void is_enabled (bool value);
    void number_of_channels (int value);
    void max_number_of_samples (int value);
    void rows_columns (int number_of_rows, int number_of_columns);

    // getters
    Channel_Signals* channel_signals ();


    void update ();

    // -- FUNCTION GENERATOR SECTION ---
    int  generate_waveform (WaveType wave_type, int channel);
    void enable_function_generator_mode ();
    
    // setters
    void function_wave_type           (WaveType wave_type, int channel);
    void function_amplitude           (float    amplitude);
    void function_frequency           (float    frequency);
    void function_phase               (float    phase);
    void function_y_offset            (float    y_offset);
    void function_time_per_division   (float    time_per_division);
    void function_volts_per_division  (float    volts_per_division);

    // getters
    WaveType  function_wave_type          ();
    float     function_amplitude          ();
    float     function_frequency          ();
    float     function_phase              ();
    float     function_y_offset           ();
    float     function_time_per_division  ();
    float     function_volts_per_division ();
};

#endif