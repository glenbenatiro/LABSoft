#include "LABSoft_Controller_Main_Window.h"

#include <FL/Fl.H>

#include "LABSoft_Controller.h"

LABSoft_Controller_Main_Window:: 
LABSoft_Controller_Main_Window (LABSoft_Controller& _LABSoft_Controller)
  : LABSoft_Controller_Unit (_LABSoft_Controller)
{

}

void LABSoft_Controller_Main_Window:: 
cb_workspace_exit (Fl_Widget* w, 
                   void*      data)
{
  if (Fl::modal () == m_controller.gui ().main_fl_window)
  {
    m_controller.gui ().main_fl_window->hide ();
  }
}

void LABSoft_Controller_Main_Window:: 
cb_help_about (Fl_Widget* w, void* data)
{
  m_controller.gui ().main_fl_window->deactivate ();
  m_controller.gui ().main_fl_window_about->show ();
}

void LABSoft_Controller_Main_Window:: 
cb_help_about_win_close (Fl_Button* w, void* data)
{
  m_controller.gui ().main_fl_window_about->hide ();
  m_controller.gui ().main_fl_window->activate ();
}
