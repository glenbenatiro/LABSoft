#ifndef LAB_POWER_SUPPLY
#define LAB_POWER_SUPPLY

#include <cstdio>

#include <FL/Fl_Light_Button.H>

class LAB_Power_Supply
{
  private:
    bool m_flag_master_enable = false;

  public:

    // cb
    void LAB_Power_Supply_cb_power_supply_fl_light_button_positive_12_volts (Fl_Light_Button *w, void *data);
    void LAB_Power_Supply_cb_power_supply_fl_light_button_negative_12_volts (Fl_Light_Button *w, void *data);
    void LAB_Power_Supply_cb_power_supply_fl_light_button_positive_5_volts (Fl_Light_Button *w, void *data);
    void LAB_Power_Supply_cb_power_supply_fl_light_button_master_enable_disable (Fl_Light_Button *w, void *data);
};

#endif