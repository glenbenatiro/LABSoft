#ifndef LABSOFT_GUI_FL_INPUT_CHOICE_WITH_SCROLL
#define LABSOFT_GUI_FL_INPUT_CHOICE_WITH_SCROLL

#include "FL/Fl_Input_Choice.H"

class LABSoft_GUI_Fl_Input_Choice_With_Scroll : public Fl_Input_Choice
{
  private:
    void mouse_wheel_cb (int scroll_amount);

  protected:
    int handle (int e);

  public: 
    LABSoft_GUI_Fl_Input_Choice_With_Scroll (int X, int Y, int W, int H, const char* label = 0);
};

#endif