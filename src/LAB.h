#ifndef LAB_H
#define LAB_H

#include "LAB_Core.h"
#include "LAB_Oscilloscope.h"
#include "LAB_Voltmeter.h"
#include "LAB_Function_Generator.h"
#include "LAB_Logic_Analyzer.h"
#include "LAB_Circuit_Checker.h"

class LAB
{
  private: 
    
  public:
    AP_MemoryMap m_uncached_pwm_dma_data;
    
    LAB ();
   ~LAB ();
   
    LAB_Core                m_LAB_Core;
    LAB_Oscilloscope        m_Oscilloscope;
    LAB_Voltmeter           m_Voltmeter;
    LAB_Function_Generator  m_Function_Generator;
    LAB_Logic_Analyzer      m_Logic_Analyzer;
    LAB_Circuit_Checker     m_Circuit_Checker;   
};

#endif