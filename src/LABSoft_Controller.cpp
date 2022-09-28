#include "LABSoft_Controller.h"

LABSoft_Controller:: 
LABSoft_Controller (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

  m_LABSoft_Controller_Oscilloscope       = new LABSoft_Controller_Oscilloscope       (m_LAB, m_LABSoft_GUI);
  m_LABSoft_Controller_Voltmeter          = new LABSoft_Controller_Voltmeter          (m_LAB, m_LABSoft_GUI, m_LAB->m_LAB_Voltmeter, m_LABSoft_GUI->voltmeter_fl_output_value);
  m_LABSoft_Controller_Ammeter            = new LABSoft_Controller_Ammeter            (m_LAB, m_LABSoft_GUI, m_LAB->m_LAB_Ammeter, m_LABSoft_GUI->ammeter_fl_output_value);
  m_LABSoft_Controller_Ohmmeter           = new LABSoft_Controller_Ohmmeter           (m_LAB, m_LABSoft_GUI, m_LAB->m_LAB_Ohmmeter, m_LABSoft_GUI->ohmmeter_fl_output_value);
  m_LABSoft_Controller_Function_Generator = new LABSoft_Controller_Function_Generator (m_LAB, m_LABSoft_GUI);
  m_LABSoft_Controller_Logic_Analyzer     = new LABSoft_Controller_Logic_Analyzer     (m_LAB, m_LABSoft_GUI);
  m_LABSoft_Controller_Power_Supply       = new LABSoft_Controller_Power_Supply       (m_LAB, m_LABSoft_GUI);
  m_LABSoft_Controller_Circuit_Checker    = new LABSoft_Controller_Circuit_Checker    (m_LAB, m_LABSoft_GUI);
  m_LABSoft_Controller_Main               = new LABSoft_Controller_Main               (m_LAB, m_LABSoft_GUI); 

  // add timeout
  Fl::add_timeout (DISPLAY_UPDATE_RATE, 
    m_LABSoft_Controller_Main->LABSoft_Controller_Main_cb_update_display, (void *)m_LABSoft_GUI);
}