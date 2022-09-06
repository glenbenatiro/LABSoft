#include "LABSoft_Oscilloscope_Display.h"

LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display (int X,
                              int Y,
                              int W,
                              int H)
                              : Fl_Widget (X,                                        
                                           Y,
                                           W,
                                           H,
                                           0)
{ 
  // load in default values
  m_number_of_rows        = OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS;
  m_number_of_columns     = OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS;
  m_grid_color            = OSCILLOSCOPE_DISPLAY_GRID_COLOR;
  m_max_amplitude         = OSCILLOSCOPE_DISPLAY_MAX_AMPLITUDE;
  m_channel_1_line_color  = OSCILLOSCOPE_DISPLAY_CHANNEL_1_LINE_COLOR;
  m_channel_2_line_color  = OSCILLOSCOPE_DISPLAY_CHANNEL_2_LINE_COLOR;
  m_background_color      = OSCILLOSCOPE_DISPLAY_BACKGROUND_COLOR;

  m_x_zoom = 1;
  m_y_zoom = 1;
  m_x_offset = 0;
  m_y_offset = 0;

  //
  m_display_relative_x_position = X;
  m_display_relative_y_position = Y;

  // set default display appearance
  this->box   (FL_FLAT_BOX);
  this->color (m_background_color);

  // resize values and points vector to display width, and initialize to zero
  m_values_buffer_channel_1.resize (w (), 0.0);
  m_values_buffer_channel_2.resize (w (), 0.0);
 
  // resize 2D vectors
  m_display_buffer_channel_1.resize (w (), std::vector<int> (2));
  m_display_buffer_channel_2.resize (w (), std::vector<int> (2));

  // generate sample sine wave
  LABSoft_Oscilloscope_Display_generate_sine_sample (1, 4.0, 0.025);
  LABSoft_Oscilloscope_Display_generate_sine_sample (2, 3.0, 0.004);

  // normalize values to screen
  LABSoft_Oscilloscope_Display_normalize_values_to_display (1);
  LABSoft_Oscilloscope_Display_normalize_values_to_display (2);
}

void LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display_generate_sine_sample (int channel,
                                                   float amplitude,
                                                   float frequency)
{
  // hard code pi, depends how accurate you want to be
  float pi_value = 3.141592653589;

  std::vector<float> *vals = (channel == 2) ? &m_values_buffer_channel_2 : &m_values_buffer_channel_1;

  for (int X = 0; X < vals->size (); X++)
    {
      vals->at (X) = amplitude * sin (2 * pi_value * frequency * X);
    }
}

void LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display_normalize_values_to_display (int channel)
{
  std::vector<float>            *vals   = (channel == 2) ? &m_values_buffer_channel_2 : &m_values_buffer_channel_1;
  std::vector<std::vector<int>> *points = (channel == 2) ? &m_display_buffer_channel_2 : &m_display_buffer_channel_1;

  for (int X = 0; X < vals->size (); X++)
    {
      points->at(X).at(0) = X + x ();

      if (vals->at(X) >= m_max_amplitude)  
        {
          points->at(X).at(1) = X + x ();
        }
      else if (vals->at(X)  <= (m_max_amplitude * -1))
        {
          points->at(X).at(1) = X + x ();
        }
      else
        {
          points->at(X).at(1) = (y () + (h () / 2)) - (((h () / 2) / m_max_amplitude) * (vals->at(X)));
        }
    }
}


// draw a line between two points, affected by zoom and offset
void 
LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display_draw_line (float x1,
                                        float y1,
                                        float x2,
                                        float y2)
{
  x1 = (x1 * m_x_zoom) + m_x_offset;
  x2 = (x2 * m_x_zoom) + m_x_offset;

  y1 = (y1 * m_y_zoom) + m_y_offset;
  y1 = y () + h () - y1; // inverted y

  y2 = (y2 * m_y_zoom) + m_y_offset;
  y2 = y () + h () - y2; // inverted y

  // set line color
  fl_color (m_line_color);

  fl_line ((int)x1, (int)y1, (int)x2, (int)y2);
  fl_line ((int)x1 + 1, (int)y1 +1, (int)x2 + 1, (int)y2 + 1);

  // tells draw() to stop drawing, we hit the right edge of the scope
  if (x2 > x () + w ())
    m_stop = 1;
}

// trace relative to current position
void
LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display_trace_relative (float xo,
                                             float yo)
{
  LABSoft_Oscilloscope_Display_draw_line (m_beam_x, m_beam_y, m_beam_x + xo, m_beam_y + yo);
  m_beam_x = m_beam_x + xo;
  m_beam_y = m_beam_y + yo;
}

// trace relative to absolute position
void
LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display_trace_to (float x2,
                                       float y2)
{
  LABSoft_Oscilloscope_Display_draw_line (m_beam_x, m_beam_y, x2, y2);
  m_beam_x = x2;
  m_beam_y = y2;
}

// void
// LABSoft_Oscilloscope_Display::
// LABSoft_Oscilloscope_Display (float amp,
//                               float usecs,
//                               int   cycles)
// {
//   int samps = 15;
//   float y_save = m_beam_y; // save ypos
//   float x_save = m_beam_x; // save xpos
//   unsigned int c_save = m_line_color // save line color
//   float cycle_width = (usecs / cycles); // cycle width

//   for (int cycle = 0; cycle < cycles; cycle++)
//     {

//     }
// }


void 
LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display_draw_signal (int channel)
{
  std::vector<std::vector<int>> *points = (channel == 2) ? &m_display_buffer_channel_2 : &m_display_buffer_channel_1;
  int color = (channel == 2) ? m_channel_2_line_color : m_channel_1_line_color;

  fl_color (color);

  for (int X = 0; X < (points->size() - 1); X++)
    {
      fl_line (points->at(X).at(0), points->at(X).at(1), points->at(X + 1).at(0), points->at(X + 1).at(1));
    }
  
  // reset color to black
  fl_color (0);
}

// draw the scope's non-scaling graticule
void
LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display_draw_grid ()
{
  fl_color (m_grid_color);

  // draw rows
  for (int Y = y (); Y <=  (y () + h ()); Y += (h () / m_number_of_rows))
    {
      if (Y == y () || Y == h ())
        fl_line_style (FL_SOLID, 0, NULL);
      else if (Y == (y () + (h () / 2)))
        fl_line_style (FL_DASH, 0, NULL);
      else 
        fl_line_style (FL_DOT, 0, NULL);

      fl_line (x (), Y, x () + w (), Y);
    }
  
  // draw columns
  for (int X = x (); X <= (x () + w ()); X += (w () / m_number_of_columns))
    {
      if (X == x () || X == w ())
        fl_line_style (FL_SOLID, 0, NULL);
      else if (X == (x () + (w () / 2)))
        fl_line_style (FL_DASH, 0, NULL);
      else 
        fl_line_style (FL_DOT, 0, NULL);

      fl_line (X, y (), X, y () + h ());
    }

  fl_color (0);
  fl_line_style (0);
}

// FLTK calls this to draw the Scope widget
// this is the top level for handling drawing
void
LABSoft_Oscilloscope_Display::
draw () 
{
  draw_box (FL_FLAT_BOX, m_background_color);
  
  // draw grid
  LABSoft_Oscilloscope_Display_draw_grid ();

  if (m_flag_global_enable)  
    {
      if (m_flag_channel_1_enable)
        LABSoft_Oscilloscope_Display_draw_signal (1);

      if (m_flag_channel_2_enable)
        LABSoft_Oscilloscope_Display_draw_signal (2); 
    }
}

float 
LABSoft_Oscilloscope_Display::LABSoft_Oscilloscope_Display_get_x_zoom ()
{
  return m_x_zoom;
}

float 
LABSoft_Oscilloscope_Display::LABSoft_Oscilloscope_Display_get_y_zoom ()
{
  return m_y_zoom;
}

void
LABSoft_Oscilloscope_Display::LABSoft_Oscilloscope_Display_set_x_zoom (float val)
{
  m_x_zoom = val;

  redraw ();
}

void
LABSoft_Oscilloscope_Display::LABSoft_Oscilloscope_Display_set_y_zoom (float val)
{
  m_y_zoom = val;

  redraw ();
}

void
LABSoft_Oscilloscope_Display::LABSoft_Oscilloscope_Display_set_x_offset (float val)
{
  m_x_offset = val;

  redraw ();
}

void
LABSoft_Oscilloscope_Display::LABSoft_Oscilloscope_Display_set_y_offset (float val)
{
  m_y_offset = val;

  redraw ();
}

void 
LABSoft_Oscilloscope_Display::LABSoft_Oscilloscope_Display_save ()
{
  m_save[0] = m_x_zoom;
  m_save[1] = m_y_zoom;
  m_save[2] = m_x_offset; 
  m_save[3] = m_y_offset;
}

void 
LABSoft_Oscilloscope_Display::LABSoft_Oscilloscope_Display_load ()
{
  m_x_zoom = m_save[0];
  m_y_zoom = m_save[1];
  m_x_offset  = m_save[2];
  m_y_offset  = m_save[3];

  redraw ();
}

void 
LABSoft_Oscilloscope_Display::LABSoft_Oscilloscope_Display_set_test_pattern (char flag)
{
  m_test_pattern = flag;

  redraw ();
}



void LABSoft_Oscilloscope_Display:: 
LABSoft_Oscilloscope_Display_set_time_per_division (float value) 
{
  m_time_per_division = value;

  printf ("New time per div is: %f", m_time_per_division);
}
                                        
void LABSoft_Oscilloscope_Display:: 
LABSoft_Oscilloscope_Display_display_enable ()
{
  m_flag_global_enable = true;
}

void LABSoft_Oscilloscope_Display:: 
LABSoft_Oscilloscope_Display_display_disable ()
{
  m_flag_global_enable = false;
}

void LABSoft_Oscilloscope_Display:: 
LABSoft_Oscilloscope_Display_reload_and_draw ()
{
  if (m_flag_global_enable)
    {
      if (m_flag_channel_1_enable)
        LABSoft_Oscilloscope_Display_normalize_values_to_display (1);

      if (m_flag_channel_2_enable)
        LABSoft_Oscilloscope_Display_normalize_values_to_display (2);

      redraw ();
    }
}

// used in waveform generator
void LABSoft_Oscilloscope_Display:: 
LABSoft_Oscilloscope_Display_frequency_set (float frequency)
{
  m_frequency = frequency;
}

