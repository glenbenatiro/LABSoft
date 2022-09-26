#include "LABSoft_Oscilloscope_Display_Group.h"

LABSoft_Oscilloscope_Display_Group:: 
LABSoft_Oscilloscope_Display_Group (int X,
                                    int Y,
                                    int W,
                                    int H) : Fl_Group (X,
                                                       Y,
                                                       W,
                                                       H)
{
  disp = new LABSoft_Oscilloscope_Display ();
  
  end ();
}