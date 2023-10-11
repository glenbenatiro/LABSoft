#ifndef LABSOFT_CONTROLLER_CALIBRATION
#define LABSOFT_CONTROLLER_CALIBRATION

#include "../LABSoft_Controller_Unit.h"

#include <FL/Fl_Input.H>

class LABSoft_Controller_Calibration : public LABSoft_Controller_Unit 
{
  private: 
    void update_data ();

  public:
    LABSoft_Controller_Calibration (LABSoft_Controller& _LABSoft_Controller);

    void cb_show_window ();
    void cb_cancel      ();
    void cb_save        ();

    void cb_osc_adc_ref_voltage (Fl_Input* w, void* data);
};

#endif 