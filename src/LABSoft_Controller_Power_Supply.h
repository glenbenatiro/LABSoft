#ifndef LABSOFT_CONTROLLER_POWER_SUPPLY
#define LABSOFT_CONTROLLER_POWER_SUPPLY

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Power_Supply
{
  private:
    LAB *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

  public:
          LABSoft_Controller_Power_Supply (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // callback functions



    
    void  cb_fl_light_button_run_stop_power_channel (Fl_Light_Button *w, void *data);
    void  cb_fl_light_button_master_run_stop        (Fl_Light_Button *w, void *data);
};

#endif

// EOF