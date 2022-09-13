#include "LABSoft_Controller.h"

LABSoft_Controller:: 
LABSoft_Controller (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

  m_LABSoft_Controller_Voltmeter = new LABSoft_Controller_Voltmeter (m_LAB, m_LABSoft_GUI);
  m_LABSoft_Controller_Main      = new LABSoft_Controller_Main      (m_LAB, m_LABSoft_GUI); 

  // add timeout
  Fl::add_timeout (DISPLAY_UPDATE_RATE, 
    m_LABSoft_Controller_Main->LABSoft_Controller_Main_cb_update_display, (void *)m_LABSoft_GUI);
}