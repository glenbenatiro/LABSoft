#ifndef LABSOFT_CONTROLLER_METER
#define LABSOFT_CONTROLLER_METER

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Meter 
{
  private:

  public:
    LABSoft_Controller_Meter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // callback functions
    void cb_fl_choice_unit (Fl_Choice *w, void *data);
};

#endif