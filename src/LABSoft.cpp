#include "LABSoft.h"

#include <FL/Fl.H>

// FLTK: Hitting the 'Escape' key closes windows - how do I prevent this?
// https://www.fltk.org/doc-1.3/FAQ.html

LABSoft::
LABSoft (int    argc,
         char** argv) 
  : m_LABSoft_Controller (&m_LAB, &m_LABSoft_GUI)
{
  // initialize threading support
  Fl::lock ();

  // show main window
  m_LABSoft_GUI.main_fl_window->show ();

  // run main fltk loop
  Fl::run();
}

LABSoft::
~LABSoft() 
{

}
