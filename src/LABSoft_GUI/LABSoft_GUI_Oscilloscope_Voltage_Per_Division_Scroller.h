#ifndef LABSOFT_GUI_OSCILLOSCOPE_VOLTAGE_PER_DIVISION_SCROLLER
#define LABSOFT_GUI_OSCILLOSCOPE_VOLTAGE_PER_DIVISION_SCROLLER

#include <FL/Fl_Widget.H>

class LABSoft_GUI_Oscilloscope_Voltage_Per_Division_Scroller : public Fl_Widget
{
  private:
    int   handle  (int event);
    void  draw    ();

  public:
    LABSoft_GUI_Oscilloscope_Voltage_Per_Division_Scroller (int X,
                                                            int Y,
                                                            int W,
                                                            int H,
                                                            const char* label = 0);
};

#endif