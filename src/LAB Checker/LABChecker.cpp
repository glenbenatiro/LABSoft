#include "LABChecker.h"
 
#include <FL/Fl.H>

LABChecker:: 
LABChecker () 
  : m_LABChecker_Controller (&m_LABChecker_Main, &m_LABChecker_GUI)
{
  m_LABChecker_GUI.m_LABChecker_Controller = &m_LABChecker_Controller;

  m_LABChecker_GUI->main_fl_window->show ();

  Fl::run ();
}

LABChecker:: 
~LABChecker ()
{

}