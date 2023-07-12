#ifndef LABSOFT_GUI_FL_CHOICE_WITH_SCROLL_H
#define LABSOFT_GUI_FL_CHOICE_WITH_SCROLL_H

#include "FL/Fl_Choice.H"

class LABSoft_GUI_Fl_Choice_With_Scroll : public Fl_Choice
{
  private:
    enum class SCROLL_DIRECTION
    {
      UP,
      DOWN
    };

  private:
    void mouse_wheel_cb (int scroll_amount);

  protected:
    int handle (int e);

  public: 
    LABSoft_GUI_Fl_Choice_With_Scroll (int X, int Y, int W, int H, const char* label = 0);
};

#endif