#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_H

#include <stdio.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Box.H>

#include "LAB_Globals.h"

#define GREEN     0xb0			// brightness
#define GREENBURN (0xff-GREEN)		// burn brightness

// NTSC color bar scope simulator
class LABSoft_Oscilloscope_Display : public Fl_Widget
{ 
  public:
    const double m_pi = 3.1415926535897932384626433832795028841971693993751058209;

    // value buffers
    std::vector<std::vector<int>> m_channel_1_display_buffer,
                                  m_channel_2_display_buffer;

    std::vector<float>            m_channel_1_values_buffer,  
                                  m_channel_2_values_buffer;

    // for displaying
    float     m_x_zoom = 1,        // zoom, x/y offset
              m_y_zoom = 1,
              m_x_offset = 0,
              m_y_offset = 0,
              m_max_amplitude,
              m_time_per_division = 1.0,
              m_volts_per_division = 1.0; // default to 1 second
          
    // for function generator
    float m_frequency,
          m_period,
          m_amplitude,
          m_offset, 
          m_duty_cycle,
          m_phase;
          
    WaveType  m_wave_type;  

  

    bool m_flag_is_display_enabled = false,
         m_flag_channel_1_enable = true,
         m_flag_channel_2_enable = false;

         int m_number_of_rows,
      m_number_of_columns,
      m_channel_1_line_color,
      m_channel_2_line_color,
      m_display_relative_x_position,
      m_display_relative_y_position,
      m_background_color;
    



  
  int m_grid_color;

  char m_stop;	         // 'stop drawing' flag

  

  
                       
  float m_save[4];			 // saved zoom, x/y offset

  unsigned int m_line_color; // line color
  
  char m_test_pattern;		 // 0=black, 1=colorbars..

  float m_beam_x, 
        m_beam_y;			   // current beam position

  // draw commands
  void LABSoft_Oscilloscope_Display_draw_line (float x1, float y1, float x2, float y2);
  
  void LABSoft_Oscilloscope_Display_trace_relative (float xo, float yo);
  void LABSoft_Oscilloscope_Display_trace_to (float x2, float y2);

//   void LABSoft_Oscilloscope_Display_sin_cycle (float amplitude,
//                                                float usecs,
//                                                int   cycles);
  
  void  LABSoft_Oscilloscope_Display_draw_signal (int channel);
  void  LABSoft_Oscilloscope_Display_draw_grid ();
  void  draw ();

  public:
    LABSoft_Oscilloscope_Display (int x, int y, int w, int h);

    void LABSoft_Oscilloscope_Display_reload ();

    // getters
    float LABSoft_Oscilloscope_Display_get_x_zoom   ();
    float LABSoft_Oscilloscope_Display_get_y_zoom   ();
    float LABSoft_Oscilloscope_Display_get_x_offset ();
    float LABSoft_Oscilloscope_Display_get_y_offset ();

    // setters
    void  LABSoft_Oscilloscope_Display_set_x_zoom   (float value);
    void  LABSoft_Oscilloscope_Display_set_y_zoom   (float value);
    void  LABSoft_Oscilloscope_Display_set_x_offset (float value);
    void  LABSoft_Oscilloscope_Display_set_y_offset (float value);
    void  LABSoft_Oscilloscope_Display_set_volts_per_division (float value);

    //

    void  LABSoft_Oscilloscope_Display_save         ();
    void  LABSoft_Oscilloscope_Display_load         ();

    void  LABSoft_Oscilloscope_Display_set_test_pattern (char flag);

    void  LABSoft_Oscilloscope_Display_black_pattern ();

    void  LABSoft_Oscilloscope_Display_normalize_values_to_display (int channel);

    void  LABSoft_Oscilloscope_Display_generate_sine_sample (int channel, float amplitude, float frequency);

    

    void LABSoft_Oscilloscope_Display_set_time_per_division (float valueue);

    void LABSoft_Oscilloscope_Display_display_enable ();
    void LABSoft_Oscilloscope_Display_display_disable ();

    void LABSoft_Oscilloscope_Display_reload_and_draw ();

      // setters
      void LABSoft_Oscilloscope_Display_set_wave_type (WaveType wave_type);
      void LABSoft_Oscilloscope_Display_set_amplitude (float value);
      void LABSoft_Oscilloscope_Display_set_frequency (float value);
      void LABSoft_Oscilloscope_Display_set_phase (float value);

      void LABSoft_Oscilloscope_Display_generate_wave ();

      void LABSoft_Oscilloscope_Display_renerate_wave_and_draw ();

    // for debug
    void LABSoft_Oscilloscope_Display_generate_random_values ();

};

#endif