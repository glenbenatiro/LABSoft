#ifndef LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_H
#define LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_H

#include <FL/Fl_Group.H>

class LABSoft_Logic_Analyzer_Display_Group : public Fl_Group
{
  private: 

  public:
    LABSoft_Logic_Analyzer_Display_Group (int X, int Y, int W, int H, const char *label = 0);
};

#endif 