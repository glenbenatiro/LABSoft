#ifndef LABSOFT_CONTROLLER_MAIN_WINDOW_H
#define LABSOFT_CONTROLLER_MAIN_WINDOW_H

#include "../LAB/LAB.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"

// forward declare LABSoft_Controller, #include in .cpp file
class LABSoft_Controller;

class LABSoft_Controller_Main_Window
{
  private:
    LAB*                m_LAB;
    LABSoft_GUI*        m_LABSoft_GUI;
    LABSoft_Controller* m_LABSoft_Controller;

  public:
    LABSoft_Controller_Main_Window (LAB*                _LAB,
                                    LABSoft_GUI*        _LABSoft_GUI,
                                    LABSoft_Controller* _LABSoft_Controller);

    void cb_workspace_exit        (Fl_Widget* w, void* data);
    void cb_help_about            (Fl_Widget* w, void* data);
    void cb_help_about_win_close  (Fl_Button* w, void* data);
};

#endif
