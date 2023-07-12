#include "LABSoft_Controller_Main_Window.h"

LABSoft_Controller_Main_Window:: 
LABSoft_Controller_Main_Window (LAB*                _LAB, 
                                LABSoft_GUI*        _LABSoft_GUI,
                                LABSoft_Controller* _LABSoft_Controller)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI),
    m_LABSoft_Controller  (_LABSoft_Controller)
{

}

void LABSoft_Controller_Main_Window:: 
cb_workspace_exit (Fl_Widget* w, 
                   void*      data)
{
  if (Fl::modal () == m_LABSoft_GUI->main_fl_window)
  {
    m_LABSoft_GUI->main_fl_window->hide ();
  }
}

void LABSoft_Controller_Main_Window:: 
cb_help_about (Fl_Widget* w, void* data)
{
  m_LABSoft_GUI->main_fl_window->deactivate ();
  m_LABSoft_GUI->main_fl_window_about->show ();
}

void LABSoft_Controller_Main_Window:: 
cb_help_about_win_close (Fl_Button* w, void* data)
{
  m_LABSoft_GUI->main_fl_window_about->hide ();
   m_LABSoft_GUI->main_fl_window->activate ();
}
