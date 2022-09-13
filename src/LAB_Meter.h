#ifndef LAB_METER
#define LAB_METER

#include <cstdio>
#include <cstdlib>
#include <thread>

#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Output.H>

#include "LAB_Core.h"

class LAB_Meter
{
  public:
    LAB_Core *m_LAB_Core;

    float m_value = 0.0;
    float m_unit_scaler = 0.0;
    
    bool m_flag_is_meter_running = false;
    bool m_flag_is_generate_random_values_running = false;

    // functions  
    LAB_Meter (LAB_Core *_LAB_Core);

    //
    void LAB_Meter_update_value (Fl_Output *w);

    // callback functions
    void LAB_Meter_cb_fl_light_button_run_stop (Fl_Light_Button *w, void *data);
    void LAB_Meter_cb_fl_choice_unit             (Fl_Choice       *w, void *data);

    // test function
    void LAB_Meter_DEBUG_cb_meter_fl_light_button_generate_random_values (Fl_Light_Button *w, void *data);
    void LAB_Meter_random_values_generator ();
};

#endif
