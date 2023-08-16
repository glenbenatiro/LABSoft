#ifndef LABSOFT_CONTROLLER_LOGIC_ANALYZER_H
#define LABSOFT_CONTROLLER_LOGIC_ANALYZER_H

#include <FL/Fl_Light_Button.H>

#include <thread>

#include "../LAB/LAB.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"

// forward declare LABSoft_Controller, #include in .cpp file
class LABSoft_Controller;

class LABSoft_Controller_Logic_Analyzer
{
  private:
    LAB*                m_LAB;
    LABSoft_GUI*        m_LABSoft_GUI;
    LABSoft_Controller* m_LABSoft_Controller;
    std::thread*        m_thread_update_display;
  
  private:
    void init             ();
    void init_gui_values  ();
    
  public:
    LABSoft_Controller_Logic_Analyzer (LAB*                 _LAB, 
                                       LABSoft_GUI*         _LABSoft_GUI,
                                       LABSoft_Controller*  _LABSoft_Controller);

    // master controls
    void cb_run_stop              (Fl_Light_Button* w, void* data);
    void cb_single                (Fl_Button*       w, void* data);

    // horizontal
    void cb_horizontal_offset     (Fl_Input_Choice* w, void* data);
    void cb_time_per_division     (Fl_Input_Choice* w, void* data);
    void cb_samples               (Fl_Input_Choice* w, void* data);
    void cb_sampling_rate         (Fl_Input_Choice* w, void* data);
    
    // Trigger
    void cb_trigger_mode          (Fl_Choice* w, void* data);
    void cb_trigger_condition     (Fl_Menu_Button* w, void *data);

    // Mode
    void cb_mode                  (Fl_Choice* w, void* data);

    // Display update cycle
    void display_update_cycle     ();
    void update_gui_horizontal    ();
    void update_gui_mode          ();

    void cb_add_channel_selection (Fl_Menu_* w, void* data);
    void cb_add_channel_signal    (LABSoft_GUI_Logic_Analyzer_Add_Channel_Signal_Window* w, void* data);
    void cb_clear_channels        (Fl_Menu_* w, void* data);
};

#endif