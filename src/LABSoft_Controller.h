#ifndef LABSOFT_CONTROLLER_H
#define LABSOFT_CONTROLLER_H

#include "LAB.h"
#include "LABSoft_GUI.h"

#include <FL/Fl.H>

#include "LABSoft_Controller_Oscilloscope.h"
#include "LABSoft_Controller_Voltmeter.h"
#include "LABSoft_Controller_Ammeter.h"
#include "LABSoft_Controller_Ohmmeter.h"
#include "LABSoft_Controller_Function_Generator.h"
#include "LABSoft_Controller_Logic_Analyzer.h"
#include "LABSoft_Controller_Power_Supply.h"
#include "LABSoft_Controller_Circuit_Checker.h"
#include "LABSoft_Controller_Main.h"

class LABSoft_Controller
{
  public:
    // variables
    LAB                                   *m_LAB;
    LABSoft_GUI                           *m_LABSoft_GUI;
    
    LABSoft_Controller_Oscilloscope       m_Oscilloscope;             
    LABSoft_Controller_Voltmeter          m_Voltmeter;
    LABSoft_Controller_Ammeter            m_Ammeter;
    LABSoft_Controller_Ohmmeter           m_Ohmmeter;
    LABSoft_Controller_Function_Generator m_Function_Generator;
    LABSoft_Controller_Logic_Analyzer     m_Logic_Analyzer;
    LABSoft_Controller_Power_Supply       m_Power_Supply;
    LABSoft_Controller_Circuit_Checker    m_Circuit_Checker;
    LABSoft_Controller_Main               m_Main;

    // functions
    LABSoft_Controller (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);
};

#endif