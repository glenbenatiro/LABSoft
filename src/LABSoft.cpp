#include "LABSoft.h"

#include <FL/Fl.H>

// FLTK: Hitting the 'Escape' key closes windows - how do I prevent this?
// https://www.fltk.org/doc-1.3/FAQ.html

LABSoft::
LABSoft (int    argc,
         char** argv) 
  : m_LABSoft_Controller (&m_LAB, &m_LABSoft_GUI)
{
  link_controller_to_gui ();

  m_LABSoft_GUI.main_fl_window->show ();

  // important for multithreading!
  Fl::lock ();

  // run main fltk loop
  Fl::run();
}

LABSoft::
~LABSoft() 
{

}

void LABSoft:: 
link_controller_to_gui ()
{
  // 1. main LABSoft_GUI
  m_LABSoft_GUI.m_LABSoft_Controller = &m_LABSoft_Controller;

  // 2. LABSoft_GUI_Logic_Analyzer_Display
  m_LABSoft_GUI.logic_analyzer_labsoft_gui_logic_analyzer_display->
    controller (m_LABSoft_Controller);
}