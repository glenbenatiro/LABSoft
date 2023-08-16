#ifndef LAB_H
#define LAB_H

#include "LAB_Core.h"
#include "LAB_Oscilloscope.h"
#include "LAB_Voltmeter.h"
#include "LAB_Function_Generator.h"
#include "LAB_Logic_Analyzer.h"
#include "LAB_Digital_Circuit_Checker.h"
#include "LAB_LABChecker_Digital.h"

class LAB
{
  public:
    LAB ();
   ~LAB ();
   
    LAB_Core                    m_LAB_Core;
    LAB_Oscilloscope            m_Oscilloscope;
    LAB_Voltmeter               m_Voltmeter;
    LAB_Function_Generator      m_Function_Generator;
    LAB_Logic_Analyzer          m_Logic_Analyzer;  
    LAB_Digital_Circuit_Checker m_Digital_Circuit_Checker;
    LAB_LABChecker_Digital      m_LABChecker_Digital;
};

#endif