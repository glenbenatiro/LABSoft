#include "LABSoft.h"

#include <iostream>

#include <FL/Fl.H>

// FLTK: Hitting the 'Escape' key closes windows - how do I prevent this?
// https://www.fltk.org/doc-1.3/FAQ.html

LABSoft::
LABSoft (int    argc,
         char **argv) 
  : m_LABSoft_Controller (&m_LAB, &m_LABSoft_GUI)
{
  std::ios_base::sync_with_stdio(false);
  
  // link
  m_LABSoft_GUI.m_LABSoft_Controller = &m_LABSoft_Controller;

  // show main window
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
