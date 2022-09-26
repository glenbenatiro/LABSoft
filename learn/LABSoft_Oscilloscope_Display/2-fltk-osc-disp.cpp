#include <vector>
#include <iostream>


#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Enumerations.H>

// should be even!
#define LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS             2

#define LABSOFT_OSCILLOSCOPE_BACKGROUND_COLOR           0x00000000
#define LABSOFT_OSCILLOSCOPE_DEFAULT_COLOR              0x00000000
#define LABSOFT_OSCILLOSCOPE_GRID_COLOR                 0xFFFFFF00
#define LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS  8
#define LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS     10
#define LABSOFT_OSCILLOSCOPE_DISPLAY_BOXTYPE            FL_FLAT_BOX
#define LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_CHANNELS LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS

class ChannelSignal
{
  public:
    std::vector<float> values;
    
    int y_offset = 0,
        x_offset = 0;
    
    ChannelSignal (int length)
    {
      values.resize (length, 0.0); // initialize to zero
    }

    void ChannelSignal_resize (int length)
    {
      values.resize (length, 0.0);
    }
};

class LABSoft_Oscilloscope_Display : public Fl_Widget 
{
  public:
    // variables   
    int         m_background_color,
                m_number_of_rows,
                m_number_of_columns,
                m_grid_color,
                m_default_color;
                m_number_of_channels = 2, // default to 2
                ;
    
    std::vector<ChannelSignal> m_channel_signals;

    Fl_Boxtype  m_boxtype;  


    // functions
    LABSoft_Oscilloscope_Display (int X, int Y, int W, int H);

    void draw ();
    void draw_grid ();
};                               

LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display (int X, int Y, int W, int H) : Fl_Widget (X, Y, W, H, 0)
{
  // load in defaults
  m_background_color    = LABSOFT_OSCILLOSCOPE_BACKGROUND_COLOR;
  m_number_of_columns   = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS;
  m_number_of_rows      = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS;
  m_boxtype             = LABSOFT_OSCILLOSCOPE_DISPLAY_BOXTYPE;
  m_grid_color          = LABSOFT_OSCILLOSCOPE_GRID_COLOR;
  m_default_color       = LABSOFT_OSCILLOSCOPE_DEFAULT_COLOR;
  m_number_of_channels  = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_CHANNELS;
  
  // factory for channels
  LABSoft_Oscilloscope_Display_set_number_of_channels (int count);

}

void LABSoft_Oscilloscope_Display:: 
LABSoft_Oscilloscope_Display_set_number_of_channels (int count)
{
  m_channel_signals.resize (count, );
}

void LABSoft_Oscilloscope_Display::
draw ()
{
  draw_box (m_boxtype, m_background_color);
  draw_grid ();
}

void LABSoft_Oscilloscope_Display:: 
draw_grid ()
{
  // scalers
  int y_scaler = h () / m_number_of_rows,
      x_scaler = w () / m_number_of_columns;

  // set color
  fl_color (m_grid_color);
  fl_line_style (FL_DOT, 0, NULL);

  // draw rows
  for (int Y = y (); Y <= (y () + h ()); Y += y_scaler)
    { 
      // set line style
      if (Y == y () || Y == y () + h ()) 
        fl_line_style (FL_SOLID, 0, NULL);
      else if (Y == y () + (y_scaler * (m_number_of_rows / 2)) )
        fl_line_style (FL_DASH, 0, NULL);
      else 
        fl_line_style (FL_DOT, 0, NULL);

      // draw line
      fl_line (x (), Y, x () + w (), Y);

      // draw last row, for when sizing is not cleanly divisible
      if (Y + y_scaler > (y () + h()))
        fl_line (x (), y () + h (), x () + w (), y () + h());
    }
  
  // draw columns
  for (int X = x (); X <= (x () + w ()); X += x_scaler)
    { 
      // set line style
      if (X == x () || X == x () + w ()) 
        fl_line_style (FL_SOLID, 0, NULL);
      else if (X == x () + (x_scaler * (m_number_of_columns / 2))) 
        fl_line_style (FL_DASH, 0, NULL);
      else 
        fl_line_style (FL_DOT, 0, NULL);

      // draw line
      fl_line (X, y (), X, y () + h());
      
      // draw last column, for when sizing is not cleanly divisible
      if (X + x_scaler > (x () + w()))  
        fl_line (x () + w (), y (), x () + w (), y () + h ());
    }
  
  // reset color
  fl_color (m_default_color);
  fl_line_style (0);
}

// ---

int main ()
{
  Fl_Double_Window win (0, 0, 1366, 768, "osc disp");
  win.begin ();

  LABSoft_Oscilloscope_Display disp (20, 20, 800, 600);

  win.end ();
  win.show ();

  return Fl::run ();
} 