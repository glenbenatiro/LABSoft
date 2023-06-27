#include "LABChecker.h"
 
#include <FL/Fl.H>

#include <iostream>

LABChecker:: 
LABChecker () 
  : m_LABChecker_Controller (&m_LABChecker_Main, &m_LABChecker_GUI)
{  
  m_LABChecker_GUI.m_LABChecker_Controller = &m_LABChecker_Controller;

  m_LABChecker_GUI.main_fl_window->show ();

  Fl::lock ();

  Fl::run ();
}

LABChecker:: 
~LABChecker ()
{

}