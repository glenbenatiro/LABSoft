#ifndef LABSOFT_PRESENTER_CALIBRATION
#define LABSOFT_PRESENTER_CALIBRATION

#include "../LABSoft_Presenter_Unit.h"
#include "../../LAB/Software/LAB_Calibration.h"
#include "../../LABSoft_GUI/LABSoft_GUI_Fl_Input.h"

#include <FL/Fl_Input.H>

class LABSoft_Presenter_Calibration : public LABSoft_Presenter_Unit 
{
  private: 
    void  init_gui_oscilloscope_calibration_table ();
    void  apply_calibration_data_to_gui           (const LAB_Calibration_Data& data);

    LAB_Calibration_Data get_calibration_data_from_gui () const;

  public:
    LABSoft_Presenter_Calibration (LABSoft_Presenter& _LABSoft_Presenter);

    void cb_open            ();
    void cb_save_to_file    ();
    void cb_save_as_default ();
    void cb_apply           ();
    void cb_discard_changes ();
    void cb_load_factory    ();
    void cb_show_window     (); 
    void cb_close           ();
};

#endif 