#ifndef LABSOFT_CONTROLLER_H
#define LABSOFT_CONTROLLER_H

#include <chrono>

#include "LAB.h"
#include "LABSoft_GUI.h"

#include "LABSoft_Controller_Oscilloscope.h"
#include "LABSoft_Controller_Voltmeter.h"
#include "LABSoft_Controller_Function_Generator.h"
#include "LABSoft_Controller_Logic_Analyzer.h"
#include "LABSoft_Controller_Circuit_Checker.h"

struct LAB_PACK 
{
  LAB         *_LAB;
  LABSoft_GUI *_LABSoft_GUI;
};

class LABSoft_Controller
{
  private: 
    LAB_PACK _LAB_PACK;
    
  public:
    // --- Variables
    static std::chrono::time_point<std::chrono::steady_clock> post;
    static std::chrono::time_point<std::chrono::steady_clock> pre;

    LAB                                   *m_LAB;
    LABSoft_GUI                           *m_LABSoft_GUI;
    LABSoft_Controller_Oscilloscope        m_Oscilloscope;             
    LABSoft_Controller_Voltmeter           m_Voltmeter;
    LABSoft_Controller_Function_Generator  m_Function_Generator;
    LABSoft_Controller_Logic_Analyzer      m_Logic_Analyzer;
    LABSoft_Controller_Circuit_Checker     m_Circuit_Checker;

    // --- Functions ---
                LABSoft_Controller  (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);
    static void update_display      (void *data);
};

#endif