#ifndef LABSOFT_CONTROLLER_MAIN_WINDOW_H
#define LABSOFT_CONTROLLER_MAIN_WINDOW_H

#include <FL/Fl_Button.H>
#include <FL/Fl_Widget.H>

#include "LABSoft_Controller_Unit.h"

class LABSoft_Controller_Main_Window: public LABSoft_Controller_Unit
{
  public:
    LABSoft_Controller_Main_Window (LABSoft_Controller& _LABSoft_Controller);

    void cb_workspace_exit        (Fl_Widget* w, void* data);
    void cb_help_about            (Fl_Widget* w, void* data);
    void cb_help_about_win_close  (Fl_Button* w, void* data);
};

#endif
