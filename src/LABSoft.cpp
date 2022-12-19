#include "LABSoft.h"

#include <iostream>

#include <FL/Fl.H>

LABSoft::
LABSoft (int    argc,
         char **argv) 
  : m_LABSoft_Controller (&m_LAB, &m_LABSoft_GUI)
{
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
