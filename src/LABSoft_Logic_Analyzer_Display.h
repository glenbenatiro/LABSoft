#ifndef LABSOFT_LOGIC_ANALYZER_DISPLAY
#define LABSOFT_LOGIC_ANALYZER_DISPLAY

#include <vector>

#include <FL/Fl_Widget.H>

#include "Defaults.h"

class LABSoft_Logic_Analyzer_Display : public Fl_Widget
{
  private: 

  public:
    LABSoft_Logic_Analyzer_Display (int X, int Y, int W, int H, const char *label);
   ~LABSoft_Logic_Analyzer_Display ();

   void draw ();

    void  load_channel_signals (std::vector <Channel_Data_Logic_Analyzer> *chan_data);
};

#endif