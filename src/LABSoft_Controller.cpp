#include "LABSoft_Controller.h"

#include <iostream>

LABSoft_Controller:: 
LABSoft_Controller (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
  : m_Oscilloscope        (m_LAB, m_LABSoft_GUI),
    m_Voltmeter           (m_LAB, m_LABSoft_GUI),
    m_Ammeter             (m_LAB, m_LABSoft_GUI),
    m_Ohmmeter            (m_LAB, m_LABSoft_GUI),
    m_Function_Generator  (m_LAB, m_LABSoft_GUI),
    m_Logic_Analyzer      (m_LAB, m_LABSoft_GUI),
    m_Power_Supply        (m_LAB, m_LABSoft_GUI),
    m_Circuit_Checker     (m_LAB, m_LABSoft_GUI),
    m_Main                (m_LAB, m_LABSoft_GUI)    
{
  std::cout << "start controller\n";

  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

  std::cout << "end controller\n";

  // add timeout
  //Fl::add_timeout (DISPLAY_UPDATE_RATE, 
  //  m_LABSoft_Controller_Main->LABSoft_Controller_Main_cb_update_display, (void *)m_LABSoft_GUI);
}