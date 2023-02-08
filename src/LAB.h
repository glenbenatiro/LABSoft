#ifndef LAB_H
#define LAB_H

#include "LAB_Core.h"
#include "LAB_Oscilloscope.h"
#include "LAB_Multimeter.h"
#include "LAB_Function_Generator.h"
#include "LAB_Logic_Analyzer.h"
#include "LAB_Circuit_Checker.h"

// 


// 

class LAB
{
  public:
    // functions
    LAB ();
   ~LAB ();

    LAB_Core                m_LAB_Core;
    LAB_Oscilloscope       *m_Oscilloscope;
    LAB_Function_Generator *m_Function_Generator;
    LAB_Multimeter         *m_Multimeter;   
    LAB_Logic_Analyzer     *m_Logic_Analyzer;
    LAB_Circuit_Checker    m_LAB_Circuit_Checker;   
};

#endif