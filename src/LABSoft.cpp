#include "LABSoft.h"

LABSoft::
LABSoft (int    argc,
         char **argv) 
{
  m_LAB                = new LAB ();
  m_LABSoft_GUI        = new LABSoft_GUI ();
  m_LABSoft_Controller = new LABSoft_Controller (m_LAB, m_LABSoft_GUI);

  // link
  m_LABSoft_GUI->m_LABSoft_Controller = m_LABSoft_Controller;

  // show main window
  m_LABSoft_GUI->main_fl_window->show ();

  // run main fltk loop
  Fl::run();
}

LABSoft::
~LABSoft() 
{

}
