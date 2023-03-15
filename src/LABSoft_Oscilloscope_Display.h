#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_H

#include <FL/Fl_Widget.H>

#include "Defaults.h"

class LABSoft_Oscilloscope_Display : public Fl_Widget
{ 
  private:
    LAB_Parent_Data_Oscilloscope *m_osc_parent_data = nullptr;

  public:
     // --- Functions ---
     LABSoft_Oscilloscope_Display (int X, int Y, int W, int H, const char *label);
    ~LABSoft_Oscilloscope_Display ();

    // Draw
    void  draw           ();
    void  draw_grid      ();
    int   draw_channels  ();

    // Data load/init
    void  load_osc_parent_data  (LAB_Parent_Data_Oscilloscope *parent_data);
    int   reserve_pixel_points  ();
    int   fill_pixel_points     ();
};

#endif

// EOF