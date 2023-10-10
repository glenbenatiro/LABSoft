#ifndef LABSOFT_CONTROLLER_CALIBRATION
#define LABSOFT_CONTROLLER_CALIBRATION

#include "../LABSoft_Controller_Unit.h"

class LABSoft_Controller_Calibration : public LABSoft_Controller_Unit 
{
  private:

  public:
    LABSoft_Controller_Calibration (LABSoft_Controller& _LABSoft_Controller);

    void cb_show_window ();
    void cb_cancel      ();
};

#endif 