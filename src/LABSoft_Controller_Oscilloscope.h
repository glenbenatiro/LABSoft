#ifndef LABSOFT_CONTROLLER_OSCILLOSCOPE_H
#define LABSOFT_CONTROLLER_OSCILLOSCOPE_H

#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Toggle_Button.H>

#include "LAB.h"
#include "LABSoft_GUI.h"

// forward declare LABSoft_Controller
class LABSoft_Controller;

class LABSoft_Controller_Oscilloscope
{
  private:
    LAB*                m_LAB;
    LABSoft_GUI*        m_LABSoft_GUI;
    LABSoft_Controller* m_LABSoft_Controller;

  public:
    LABSoft_Controller_Oscilloscope (LAB*                 _LAB, 
                                     LABSoft_GUI*         _LABSoft_GUI, 
                                     LABSoft_Controller*  _LABSoft_Controller);
    
    // Master controls
    void cb_run_stop                      (Fl_Light_Button*   w, void* data);

    // Vertical
    void cb_channel_enable_disable        (Fl_Light_Button*   w, long channel);
    void cb_ac_coupling                   (Fl_Light_Button*   w, long channel);
    void cb_voltage_per_division          (Fl_Input_Choice*   w, long channel);
    void cb_vertical_offset               (Fl_Input_Choice*   w, long channel);
    void cb_scaling                       (Fl_Choice*         w, long channel);

    // Horizontal
    void cb_time_per_division             (Fl_Input_Choice*   w, void* data);
    void cb_sampling_rate                 (Fl_Input_Choice*   w, void* data);
    void cb_horizontal_offset             (Fl_Input_Choice*   w, void* data);

    // Trigger
    void cb_trigger_mode                  (Fl_Choice*         w, void* data);
    void cb_trigger_source                (Fl_Choice*         w, void* data);
    void cb_trigger_type                  (Fl_Choice*         w, void* data);
    void cb_trigger_condition             (Fl_Choice*         w, void* data);
    void cb_trigger_level                 (Fl_Input_Choice*   w, void* data);

    // Display
    void cb_display_mode                  (Fl_Choice*         w, void* data);
    void cb_channel_selector              (Fl_Toggle_Button*  w, long channel);

    // GUI update
    void display_update_cycle             ();
    void update_gui_main                  ();
    void update_trigger_panel_gui         ();
    void update_horizontal_widgets_gui    ();
    
    // Extra
    void oscilloscope_display_group_init  ();
};

#endif