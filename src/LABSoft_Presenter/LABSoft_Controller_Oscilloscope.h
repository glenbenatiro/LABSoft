#ifndef LABSOFT_CONTROLLER_OSCILLOSCOPE_H
#define LABSOFT_CONTROLLER_OSCILLOSCOPE_H

#include <FL/Fl_Choice.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Light_Button.H>

#include "../LABSoft_GUI/LABSoft_GUI_Fl_Slider.h"
#include "../LABSoft_GUI/LABSoft_GUI_Oscilloscope_Display.h"
#include "../LABSoft_GUI/LABSoft_GUI_Fl_Choice_With_Scroll.h"
#include "../LABSoft_GUI/LABSoft_GUI_Fl_Input_Choice_With_Scroll.h"

#include "LABSoft_Controller_Unit.h"

class LABSoft_Controller_Oscilloscope : public LABSoft_Controller_Unit
{
  private:
    void init_gui_values                        () const;

    //
    void update_gui_panel_vertical_offset       (unsigned channel) const;
    void update_gui_panel_voltage_per_division  (unsigned channel) const;
    void update_gui_panel_horizontal_offset     () const;
    void update_gui_panel_time_per_division     () const;
    void update_gui_panel_samples               () const;
    void update_gui_panel_sampling_rate         () const;
    void update_gui_panel_mode                  () const;
    void update_gui_panel_trigger_level         () const;

    // 
    void update_gui_vertical_offset             (unsigned channel) const;
    void update_gui_voltage_per_division        (unsigned channel) const;
    void update_gui_horizontal_offset           () const;
    void update_gui_time_per_division           ();
    void update_gui_samples                     ();
    void update_gui_sampling_rate               ();
    void update_gui_horizontal_elements         () const;
    void update_gui_mode                        () const;
    void update_gui_trigger_level               () const;
    void update_gui_trigger_panel               () const;

  public:
    LABSoft_Controller_Oscilloscope (LABSoft_Controller& _LABSoft_Controller);
    
    // Master controls
    void cb_run_stop                  (Fl_Light_Button*   w, void* data);
    void cb_single                    (Fl_Button*         w, void* data);

    // Vertical
    void cb_channel_enable_disable    (Fl_Light_Button*                         w, long   channel);
    void cb_ac_coupling               (Fl_Light_Button*                         w, long   channel);
    void cb_scaling                   (LABSoft_GUI_Fl_Choice_With_Scroll*       w, long   channel);
    void cb_voltage_per_division      (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, long   channel);
    void cb_vertical_offset           (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, long   channel);
    void cb_vertical_offset_slider    (LABSoft_GUI_Fl_Slider*                   w, void*  data);
    
    // horizontal
    void cb_horizontal_offset         (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, void* data) const;
    void cb_time_per_division         (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, void* data);
    void cb_time_per_division_steps   (int steps);
    void cb_samples                   (Fl_Input_Choice*   w, void* data);
    void cb_sampling_rate             (Fl_Input_Choice*   w, void* data);

    // Trigger
    void cb_trigger_mode              (Fl_Choice*             w, void* data);
    void cb_trigger_source            (Fl_Choice*             w, void* data);
    void cb_trigger_type              (Fl_Choice*             w, void* data);
    void cb_trigger_condition         (Fl_Choice*             w, void* data);
    void cb_trigger_level             (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, void* data);
    void cb_trigger_level_slider      (LABSoft_GUI_Fl_Slider* w, void* data);

    // Record/Export
    void cb_record                    (Fl_Button*         w, void* data);
    void cb_record_config             (Fl_Button*         w, void* data);
    void cb_record_config_start       (Fl_Button*         w, void* data);
    void cb_record_config_cancel      (Fl_Button*         w, void* data);
    void cb_export                    (Fl_Menu_Item*      w, void* data);

    // Mode
    void cb_mode                      (Fl_Choice*         w, void* data);

    // GUI update
    void display_update_cycle         ();
};

#endif