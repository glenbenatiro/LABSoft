#ifndef LABSOFT_CONTROLLER_OSCILLOSCOPE_H
#define LABSOFT_CONTROLLER_OSCILLOSCOPE_H

#include <FL/Fl_Toggle_Button.H>

#include "../LAB/LAB.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"

// forward declare LABSoft_Controller, #include in .cpp file
class LABSoft_Controller;

class LABSoft_Controller_Oscilloscope
{
  private:
    LAB*                m_LAB;
    LABSoft_GUI*        m_LABSoft_GUI;
    LABSoft_Controller* m_LABSoft_Controller;

  private: 
    void init                             ();
    void link_widgets                     ();
    void init_gui                         ();
    void oscilloscope_display_group_init  ();
    void init_gui_values                  ();

  public:
    LABSoft_Controller_Oscilloscope (LAB*                 _LAB, 
                                     LABSoft_GUI*         _LABSoft_GUI, 
                                     LABSoft_Controller*  _LABSoft_Controller);
    
    // Master controls
    void cb_run_stop                      (Fl_Light_Button*   w, void* data);
    void cb_single                        (Fl_Button*         w, void* data);

    // Vertical
    void cb_channel_enable_disable        (Fl_Light_Button*                         w, long   channel);
    void cb_ac_coupling                   (Fl_Light_Button*                         w, long   channel);
    void cb_scaling                       (LABSoft_GUI_Fl_Choice_With_Scroll*       w, long   channel);
    void cb_voltage_per_division          (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, long   channel);
    void cb_vertical_offset               (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, long   channel);
    void cb_fl_slider_vertical_offset     (Fl_Slider*                               w, void*  data);
    
    // Horizontal
    void cb_horizontal_offset             (Fl_Input_Choice*   w, void* data);
    void cb_time_per_division             (Fl_Input_Choice*   w, void* data);
    void cb_samples                       (Fl_Input_Choice*   w, void* data);
    void cb_sampling_rate                 (Fl_Input_Choice*   w, void* data);

    // Trigger
    void cb_trigger_mode                  (Fl_Choice*         w, void* data);
    void cb_trigger_source                (Fl_Choice*         w, void* data);
    void cb_trigger_type                  (Fl_Choice*         w, void* data);
    void cb_trigger_condition             (Fl_Choice*         w, void* data);
    void cb_trigger_level                 (Fl_Input_Choice*   w, void* data);

    // Record/Export
    void cb_record                        (Fl_Button*         w, void* data);
    void cb_record_config                 (Fl_Button*         w, void* data);
    void cb_record_config_start           (Fl_Button*         w, void* data);
    void cb_record_config_cancel          (Fl_Button*         w, void* data);
    void cb_export                        (Fl_Menu_Item*      w, void* data);

    // Mode
    void cb_mode                          (Fl_Choice*         w, void* data);
    void cb_channel_selector              (Fl_Toggle_Button*  w, long channel);

    // Display
    void cb_display                       (LABSoft_GUI_Oscilloscope_Display* w, void* data);

    // GUI update
    void display_update_cycle             ();
    void update_gui_main                  (bool value);
    void update_trigger_panel_gui         ();
    void update_gui_horizontal            ();
    void update_gui_mode                  ();
    
    // Extra
    void test (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, void* data);    
};

#endif