#include "LABSoft_GUI_Oscilloscope_Voltage_Per_Division_Scroller.h"

#include <FL/Fl.H>
#include <FL/Enumerations.H>

LABSoft_GUI_Oscilloscope_Voltage_Per_Division_Scroller:: 
LABSoft_GUI_Oscilloscope_Voltage_Per_Division_Scroller (int X,
                                                        int Y,
                                                        int W,
                                                        int H,
                                                        const char* label)
  : Fl_Widget (X, Y, W, H, label)
{

}

int LABSoft_GUI_Oscilloscope_Voltage_Per_Division_Scroller:: 
handle (int event)
{
  switch (event)
  {
    case (FL_MOUSEWHEEL):
    {
      if (Fl::belowmouse () == this)
      {
        do_callback ();

        return (1);
      }
    }
  }
}

void LABSoft_GUI_Oscilloscope_Voltage_Per_Division_Scroller:: 
draw ()
{

}