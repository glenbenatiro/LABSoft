#ifndef LABSOFT_LOGIC_ANALYZER_DISPLAY
#define LABSOFT_LOGIC_ANALYZER_DISPLAY

#include <vector>

#include <FL/Fl_Widget.H>

#include "Defaults.h"

class LABSoft_Logic_Analyzer_Display : public Fl_Widget
{
  private: 
    LAB_Parent_Data_Logic_Analyzer *m_parent_data;

  public:
    LABSoft_Logic_Analyzer_Display (int X, int Y, int W, int H, const char *label);
   ~LABSoft_Logic_Analyzer_Display ();

   void draw          ();
   void draw_grid     ();
   int  draw_channels ();

   //

   void load_channel_signals (LAB_Parent_Data_Logic_Analyzer *parent_data);
};

#endif