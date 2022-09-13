#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP

#include "LABSoft_Oscilloscope_Display.h"

class LABSoft_Oscilloscope_Display_Group : public Fl_Group
{
  private:
    int m_disp_w = 720,
        m_disp_h = 540;

    LABSoft_Oscilloscope_Display m_LABSoft_Oscilloscope_Display;

  public:
    LABSoft_Oscilloscope_Display_Group (int X, int Y, int W, int H);

    // getter
    LABSoft_Oscilloscope_Display* disp ();

    // overriden functions
    void draw ();
};

#endif
