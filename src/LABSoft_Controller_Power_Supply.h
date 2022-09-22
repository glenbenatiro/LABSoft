#ifndef LABSOFT_CONTROLLER_POWER_SUPPLY_H
#define LABSOFT_CONTROLLER_POWER_SUPPLY_H

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Power_Supply
{
  public:
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

    LABSoft_Controller_Power_Supply (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // callback functions
    void LABSoft_Controller_Power_Supply_cb_fl_light_button_positive_12_volts_run_stop (Fl_Light_Button *w, void *data);
    void LABSoft_Controller_Power_Supply_cb_fl_light_button_negative_12_volts_run_stop (Fl_Light_Button *w, void *data);
    void LABSoft_Controller_Power_Supply_cb_fl_light_button_positive_5_volts_run_stop  (Fl_Light_Button *w, void *data);
    void LABSoft_Controller_Power_Supply_cb_fl_light_button_master_enable_disable      (Fl_Light_Button *w, void *data);
};

#endif