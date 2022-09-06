#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_H

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <stdio.h>
#include <cmath>
#include <vector>

#include "LAB_Globals.h"


#define GREEN     0xb0			// brightness
#define GREENBURN (0xff-GREEN)		// burn brightness

// NTSC color bar scope simulator
class LABSoft_Oscilloscope_Display : public Fl_Widget 
{
  private:
  int m_number_of_rows,
      m_number_of_columns,
      m_channel_1_line_color,
      m_channel_2_line_color,
      m_display_relative_x_position,
      m_display_relative_y_position,
      m_background_color;

  bool m_flag_global_enable = false,
       m_flag_channel_1_enable = true,
       m_flag_channel_2_enable = true;

  protected:
    // these vectors store the converted voltage values which are converted to x and y coordinates to be drawn on screen
    // index 0 is the x coordinate, index 1 is the y coordinate
    std::vector<std::vector<int>> m_display_buffer_channel_1,
                                  m_display_buffer_channel_2;

    // these vectors store the converted voltage values (from min amplitude to max amplitude)
    std::vector<float>  m_values_buffer_channel_1,  // converted voltage values
                        m_values_buffer_channel_2;

    // to be used in wave generator
    float m_frequency = 1.0; 

  float const m_pi = 3.141592653589; // 12 decimal places
  
  int m_grid_color;

  char m_stop;	         // 'stop drawing' flag

  float m_x_zoom,        // zoom, x/y offset
        m_y_zoom, 
        m_x_offset, 
        m_y_offset,
        m_max_amplitude,
        m_time_per_division = 1.0; // default to 1 second

  
                       
  float m_save[4];			 // saved zoom, x/y offset

  unsigned int m_line_color; // line color
  
  char m_test_pattern;		 // 0=black, 1=colorbars..

  float m_beam_x, 
        m_beam_y;			   // current beam position

    

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
  float LABSoft_Oscilloscope_Display_get_x_zoom   ();
  float LABSoft_Oscilloscope_Display_get_y_zoom   ();
  float LABSoft_Oscilloscope_Display_get_x_offset ();
  float LABSoft_Oscilloscope_Display_get_y_offset ();

  void  LABSoft_Oscilloscope_Display_set_x_zoom   (float val);
  void  LABSoft_Oscilloscope_Display_set_y_zoom   (float val);
  void  LABSoft_Oscilloscope_Display_set_x_offset (float val);
  void  LABSoft_Oscilloscope_Display_set_y_offset (float val);

  void  LABSoft_Oscilloscope_Display_save         ();
  void  LABSoft_Oscilloscope_Display_load         ();

  void  LABSoft_Oscilloscope_Display_set_test_pattern (char flag);

  void  LABSoft_Oscilloscope_Display_black_pattern ();

  void  LABSoft_Oscilloscope_Display_normalize_values_to_display (int channel);

  void  LABSoft_Oscilloscope_Display_generate_sine_sample (int channel, float amplitude, float frequency);

  

  void LABSoft_Oscilloscope_Display_set_time_per_division (float value);

  void LABSoft_Oscilloscope_Display_display_enable ();
  void LABSoft_Oscilloscope_Display_display_disable ();

  void LABSoft_Oscilloscope_Display_reload_and_draw ();
};

#endif