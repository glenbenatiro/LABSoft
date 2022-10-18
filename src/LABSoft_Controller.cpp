#include "LABSoft_Controller.h"

#include <iostream>

LABSoft_Controller:: 
LABSoft_Controller (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI),
    m_Oscilloscope        (_LAB, _LABSoft_GUI),
    m_Voltmeter           (_LAB, _LABSoft_GUI),
    m_Ammeter             (_LAB, _LABSoft_GUI),
    m_Ohmmeter            (_LAB, _LABSoft_GUI),
    m_Function_Generator  (_LAB, _LABSoft_GUI),
    m_Logic_Analyzer      (_LAB, _LABSoft_GUI),
    m_Power_Supply        (_LAB, _LABSoft_GUI),
    m_Circuit_Checker     (_LAB, _LABSoft_GUI),
    m_Main                (_LAB, _LABSoft_GUI)    
{
  // add timeout
  //Fl::add_timeout (DISPLAY_UPDATE_RATE, 
  //  m_LABSoft_Controller_Main->LABSoft_Controller_Main_cb_update_display, (void *)_LABSoft_GUI);
}