#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_H

#include <vector>
#include <cmath>

#include <FL/Fl_Widget.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>

#include "LAB_Globals.h"
#include "ChannelSignals.h"

class LABSoft_Oscilloscope_Display : public Fl_Widget
{ 
  protected:
    bool  m_flag_is_display_enabled = false;

    int   m_number_of_rows          = 10,
          m_number_of_columns       = 10,
          m_background_color        = FL_BLACK,
          m_grid_color              = FL_LIGHT3,
          m_default_color           = FL_BLACK,
          m_number_of_channels      = 2,
          m_max_number_of_channels  = 10,
          m_max_number_of_samples   = 16384;

    
    int   m_time_per_division,
          m_volts_per_division;
      
    ChannelSignals *m_channel_signals;


    //std::vector<std::vector<int>> m_channel_pixel_point_buffer (m_number_of_channels, 
    //  std::vector<int>(m_max_number_of_samples / m_number_of_channels, 0));

  public:
    // functions
          LABSoft_Oscilloscope_Display (int X, int Y, int W, int H);

    void  draw ();
    void  draw_grid ();
    void  draw_channel_signals ();
    void  normalize_channel_signals ();

    // setters
    void flag_is_display_enabled (bool value);
    void number_of_channels (int value);
    void max_number_of_samples (int value);
    void rows_columns (int number_of_rows, int number_of_columns);

    // getters
};

#endif