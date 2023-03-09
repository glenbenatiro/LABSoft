#include "LABSoft_Logic_Analyzer_Display.h"

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

LABSoft_Logic_Analyzer_Display::
LABSoft_Logic_Analyzer_Display (int X,
                                int Y,
                                int W,
                                int H,
                                const char *label = 0) 
  : Fl_Widget (X, Y, W, H, label) 
{
 

}

LABSoft_Logic_Analyzer_Display::
~LABSoft_Logic_Analyzer_Display ()
{

}

void LABSoft_Logic_Analyzer_Display:: 
draw ()
{
  draw_box (FL_FLAT_BOX, 0x00000000);
}

void LABSoft_Logic_Analyzer_Display::
load_channel_signals (std::vector <Channel_Data_Logic_Analyzer> *chan_data)
{

}

