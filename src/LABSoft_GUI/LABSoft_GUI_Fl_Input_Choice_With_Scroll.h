#ifndef LABSOFT_GUI_FL_INPUT_CHOICE_WITH_SCROLL
#define LABSOFT_GUI_FL_INPUT_CHOICE_WITH_SCROLL

#include "FL/Fl_Input_Choice.H"

class LABSoft_GUI_Fl_Input_Choice_With_Scroll : public Fl_Input_Choice
{
  public:
    enum class SCROLL_DIRECTION
    {
      UP    = -1,
      DOWN
    };

  protected:
    int handle (int e);


  public: 
    LABSoft_GUI_Fl_Input_Choice_With_Scroll (int X, int Y, int W, int H, const char* label = 0);

    void scroll_menu (SCROLL_DIRECTION direction);
    void scroll_cb ();
};

#endif