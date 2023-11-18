#ifndef LABSOFT_PRESENTER_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_PRESENTER_OSCILLOSCOPE_DISPLAY_H

#include "../LABSoft_Controller_Unit.h"

class LABSoft_Presenter_Oscilloscope_Display : public LABSoft_Controller_Unit
{
  private:
    void load_gui ();

  public:
    LABSoft_Presenter_Oscilloscope_Display (LABSoft_Controller& _LABSoft_Controller);

    void cb_mouse_wheel         (int direction);
    void update_display         ();
};

#endif