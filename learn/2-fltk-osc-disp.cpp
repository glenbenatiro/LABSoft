#include <iostream>
#include <cmath>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Enumerations.H>

// should be even!
#define LABSOFT_OSCILLOSCOPE_BACKGROUND_COLOR           0x00000000
#define LABSOFT_OSCILLOSCOPE_DEFAULT_COLOR              0x00000000
#define LABSOFT_OSCILLOSCOPE_GRID_COLOR                 0xFFFFFF00
#define LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS  5
#define LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS     7
#define LABSOFT_OSCILLOSCOPE_DISPLAY_BOXTYPE            FL_FLAT_BOX

class LABSoft_Oscilloscope_Display : public Fl_Widget 
{
  public:
    // variables   
    int         m_background_color,
                m_number_of_rows,
                m_number_of_columns,
                m_grid_color,
                m_default_color;

    Fl_Boxtype  m_boxtype;  


    // functions
    LABSoft_Oscilloscope_Display (int X, int Y, int W, int H);

    void draw ();
    void draw_grid ();
};

LABSoft_Oscilloscope_Display::
LABSoft_Oscilloscope_Display (int X, int Y, int W, int H) : Fl_Widget (X, Y, W, H, 0)
{
  m_background_color  = LABSOFT_OSCILLOSCOPE_BACKGROUND_COLOR;
  m_number_of_columns = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS;
  m_number_of_rows    = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS;
  m_boxtype           = LABSOFT_OSCILLOSCOPE_DISPLAY_BOXTYPE;
  m_grid_color        = LABSOFT_OSCILLOSCOPE_GRID_COLOR;
  m_default_color     = LABSOFT_OSCILLOSCOPE_DEFAULT_COLOR;
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

// ---

int main ()
{
  Fl_Double_Window win (0, 0, 1366, 768, "osc disp");
  win.begin ();

  LABSoft_Oscilloscope_Display disp (20, 20, 600, 600);

  win.end ();
  win.show ();

  return Fl::run ();
}