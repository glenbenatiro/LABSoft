#ifndef LABSOFT_PRESENTER_CALIBRATION
#define LABSOFT_PRESENTER_CALIBRATION

#include "../LABSoft_Presenter_Unit.h"

#include <FL/Fl_Input.H>

class LABSoft_Presenter_Calibration : public LABSoft_Presenter_Unit 
{
  private: 
    void update_data_display ();

  public:
    LABSoft_Presenter_Calibration (LABSoft_Presenter& _LABSoft_Presenter);

    void cb_show_window   ();
    void cb_cancel        ();
    void cb_save_to_file  ();

    // oscilloscope
    void cb_osc_adc_vref                        (Fl_Input* w, long data);
    void cb_osc_scaling_corrector_half_to_unity (Fl_Input* w, long data);

    // ohmmeter
    void cb_ohm_vref  (Fl_Input* w, long data);
    void cb_ohm_r1    (Fl_Input* w, long data);
};

#endif 