#include "LABSoft_Oscilloscope_Display_Group.h"

LABSoft_Oscilloscope_Display_Group::
LABSoft_Oscilloscope_Display_Group (int X, 
                                    int Y, 
                                    int W, 
                                    int H) 
: Fl_Group (X, Y, W, H, 0), m_LABSoft_Oscilloscope_Display (X + 40 , Y + 60, m_disp_w, m_disp_h)
{
  end ();
}

LABSoft_Oscilloscope_Display* LABSoft_Oscilloscope_Display_Group::
disp ()
{
  return &m_LABSoft_Oscilloscope_Display;
}

void LABSoft_Oscilloscope_Display_Group::
draw ()
{
  // outer box
  draw_box (FL_FLAT_BOX, 0x00000000);

  draw_children ();
}
