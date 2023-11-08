#ifndef LABSOFT_CONTROLLER_OSCILLOSCOPE_DISPLAY
#define LABSOFT_CONTROLLER_OSCILLOSCOPE_DISPLAY

#include "../LABSoft_Controller_Unit.h"

class LABSoft_Controller_Oscilloscope_Display : public LABSoft_Controller_Unit
{
  private:
    LAB_Oscilloscope& m_osc;

  private:
    void calc_pixel_points ();
    
  public:
    LABSoft_Controller_Oscilloscope_Display (LABSoft_Controller& _LABSoft_Controller);

    void cb_update_display ();



};

#endif