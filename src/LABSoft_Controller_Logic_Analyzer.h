#ifndef LABSOFT_CONTROLLER_LOGIC_ANALYZER_H
#define LABSOFT_CONTROLLER_LOGIC_ANALYZER_H

#include <FL/Fl_Light_Button.H>

#include <thread>

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Logic_Analyzer
{
  private:
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;
    std::thread *m_thread_update_display;

  public:
    LABSoft_Controller_Logic_Analyzer (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    void reserve_pixel_points ();

    // callback functions
    void cb_master_run_stop   (Fl_Button       *w, void *data);
    void cb_memory_depth      (Fl_Input_Choice *w, void *data);
    void cb_time_per_division (Fl_Input_Choice *w, void *data);
    void cb_position          (Fl_Input_Choice *w, void *data);
    void update_display ();
};

#endif