#ifndef LAB_H
#define LAB_H

#include "LAB_Core.h"
#include "LAB_Oscilloscope.h"
#include "LAB_Meter.h"
#include "LAB_Voltmeter.h"
#include "LAB_Ammeter.h"
#include "LAB_Ohmmeter.h"
#include "LAB_Function_Generator.h"
#include "LAB_Logic_Analyzer.h"
#include "LAB_Power_Supply.h"
#include "LAB_Circuit_Checker.h"

class LAB
{
  public:
    // functions
    LAB ();
   ~LAB ();

    LAB_Core               m_LAB_Core;

    LAB_Oscilloscope       *m_Oscilloscope;
    LAB_Function_Generator *m_Function_Generator;
    
    LAB_Voltmeter          m_Voltmeter;
    LAB_Ammeter            m_Ammeter;
    LAB_Ohmmeter           m_Ohmmeter;
    
    LAB_Logic_Analyzer     m_LAB_Logic_Analyzer;
    LAB_Power_Supply       m_Power_Supply;
    LAB_Circuit_Checker    m_LAB_Circuit_Checker;   
};

#endif