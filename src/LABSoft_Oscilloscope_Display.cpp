#include "LABSoft_Oscilloscope_Display.h"

LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display (int X,
                              int Y,
                              int W,
                              int H) : Fl_Widget (X,                                        
                                                              Y,
                                                              W,
                                                              H,
                                                              0)
{ 
  // load defaults
  m_number_of_rows          = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS;
  m_number_of_columns       = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS;
  m_grid_color              = LABSOFT_OSCILLOSCOPE_DISPLAY_GRID_COLOR;
  m_background_color        = LABSOFT_OSCILLOSCOPE_DISPLAY_BACKGROUND_COLOR;
  m_default_color           = LABSOFT_OSCILLOSCOPE_DISPLAY_DEFAULT_COLOR;
  m_number_of_channels      = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_CHANNELS; 
  m_max_number_of_channels  = LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_NUMBER_OF_CHANNELS;
  m_volts_per_division      = LABSOFT_OSCILLOSCOPE_DISPLAY_VOLTS_PER_DIVISION;
}

void LABSoft_Oscilloscope_Display:: 
draw ()
{
  draw_box (FL_FLAT_BOX, m_background_color);
  draw_grid ();
  
  if (m_flag_is_display_enabled)
    draw_channel_signals ();
}

void LABSoft_Oscilloscope_Display:: 
draw_grid ()
{
  // set color
  fl_color (m_grid_color);

  // draw grid outer box
  fl_line_style (FL_SOLID, 0, NULL);
  fl_line (x (), y(), x () + w (), y ());                 // up
  fl_line (x () + w (), y (), x () + w (), y () + h ());  // right
  fl_line (x () + w (), y () + h (), x (), y () + h ());  // down
  fl_line (x (), y () + h (), x (), y ());                // left

  // draw rows
  for (int a = 0; a < (m_number_of_rows - 1); a++)
    {
      if (a == ((m_number_of_rows / 2) - 1))
        fl_line_style (FL_DASH, 0, NULL);
      else 
        fl_line_style (FL_DOT, 0, NULL);

      int Y = round ((a + 1) * ((float)h () / (float)m_number_of_rows)) + y ();
      fl_line (x (), Y, x () + w (), Y);
    }

  // draw columns 
  for (int a = 0; a < (m_number_of_columns - 1); a++)
    {
      if (a == ((m_number_of_columns / 2) - 1))
        fl_line_style (FL_DASH, 0, NULL);
      else 
        fl_line_style (FL_DOT, 0, NULL);

      int X = round ((a + 1) * ((float)w () / (float)m_number_of_columns)) + x ();
      fl_line (X, y (), X, y () + h ());
    }
  
  // reset color
  fl_line_style (0);
  fl_color (m_default_color);
}

void LABSoft_Oscilloscope_Display:: 
draw_channel_signals ()
{
  // for (int a = 0; a < m_channel_signals.size (); a++)
  //   {
  //     if (m_channel_signal[a].m_flag_is_enabled)
  //       {
  //         std::vector<std::vector<int>> *p = &(m_channel_signal[a].m_pixel_points);

  //         for (int b = 0; b < (p->size ()); b++)
  //           {
  //             fl_line ((*p)[b][0], (*p)[b][1], (*p)[b + 1][0], (*p)[b + 1][1]);
  //           }
  //       }
  //   }
}

void LABSoft_Oscilloscope_Display:: 
normalize_channel_signals ()
{
  // for (int a = 0; a < m_channel_signals.size (); a++)
  // {
  //   if (m_channel_signal[a].m_flag_is_enabled)
  //     {
  //       std::vector<std::vector<int>> *p = &(m_channel_signal[a].m_pixel_points);

  //       for (int b = 0; b < (p->size ()); b++)
  //         {
  //           fl_line ((*p)[b][0], (*p)[b][1], (*p)[b + 1][0], (*p)[b + 1][1]);
  //         }
  //     }
  }

// setters
void LABSoft_Oscilloscope_Display:: 
flag_is_display_enabled (bool value)
{
  m_flag_is_display_enabled = value;
}

void LABSoft_Oscilloscope_Display:: 
number_of_channels (int value)
{
  m_number_of_channels = value;
}

void LABSoft_Oscilloscope_Display:: 
max_number_of_samples (int value)
{
  m_max_number_of_samples = value;
}

void LABSoft_Oscilloscope_Display:: 
rows_columns (int number_of_rows,
              int number_of_columns)
{
  m_number_of_rows = number_of_rows;
  m_number_of_columns = number_of_columns;
}


