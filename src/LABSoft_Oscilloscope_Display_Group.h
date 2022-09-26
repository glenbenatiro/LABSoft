#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H

#include <vector>

#include <FL/Fl_Group.H>

#include "LABSoft_Oscilloscope_Display.h"

class LABSoft_Oscilloscope_Display_Group : public Fl_Group
{
  protected:
    // for oscilloscope display
    int m_number_of_rows,
        m_number_of_columns,
        m_number_of_channels;

    LABSoft_Oscilloscope_Display *m_oscilloscope_disp;
    LABSoft_Function_Generator_Display *m_function_gen_disp;

  public: 
    LABSoft_Oscilloscope_Display_Group (int X, int Y, int W, int H);
};

#endif