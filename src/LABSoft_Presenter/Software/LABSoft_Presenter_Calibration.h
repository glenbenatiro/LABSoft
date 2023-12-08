#ifndef LABSOFT_PRESENTER_CALIBRATION
#define LABSOFT_PRESENTER_CALIBRATION

#include "../LABSoft_Presenter_Unit.h"

#include <FL/Fl_Input.H>

class LABSoft_Presenter_Calibration : public LABSoft_Presenter_Unit 
{
  private: 
    void update_data ();

  public:
    LABSoft_Presenter_Calibration (LABSoft_Presenter& _LABSoft_Presenter);

    void cb_show_window ();
    void cb_cancel      ();
    void cb_save        ();

    void cb_osc_adc_ref_voltage (Fl_Input* w, void* data);
};

#endif 