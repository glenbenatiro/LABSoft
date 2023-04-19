#ifndef LABSOFT_CONTROLLER_H
#define LABSOFT_CONTROLLER_H

#include <chrono>

#include "LAB.h"
#include "LABSoft_GUI.h"
#include "Defaults.h"
#include "LABSoft_Controller_Oscilloscope.h"
#include "LABSoft_Controller_Voltmeter.h"
#include "LABSoft_Controller_Function_Generator.h"
#include "LABSoft_Controller_Logic_Analyzer.h"
#include "LABSoft_Controller_Circuit_Checker.h"

class LABSoft_Controller
{
  private:
    // --- Variables
    std::chrono::time_point<std::chrono::steady_clock> post;
    std::chrono::time_point<std::chrono::steady_clock> pre;

    LAB*          m_LAB;
    LABSoft_GUI*  m_LABSoft_GUI;
   
    // --- Functions ---           
    static void update_display      (void *data);
  
  public:
    LABSoft_Controller (LAB* _LAB, LABSoft_GUI* _LABSoft_GUI);

    LABSoft_Controller_Oscilloscope        m_Oscilloscope;             
    LABSoft_Controller_Voltmeter           m_Voltmeter;
    LABSoft_Controller_Function_Generator  m_Function_Generator;
    LABSoft_Controller_Logic_Analyzer      m_Logic_Analyzer;
    LABSoft_Controller_Circuit_Checker     m_Circuit_Checker;
};

#endif