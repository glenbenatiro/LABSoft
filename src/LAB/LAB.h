#ifndef LAB_H
#define LAB_H

#include "../../lib/AikaPi/AikaPi.h"

#include "LAB_Oscilloscope.h"
#include "LAB_Voltmeter.h"
#include "LAB_Function_Generator.h"
#include "LAB_Logic_Analyzer.h"
#include "LAB_Digital_Circuit_Checker.h"
#include "LAB_LABChecker_Digital.h"

#include "Software/LAB_Exporter.h"

class LAB
{
  private:
    AikaPi& m_LAB_Core;

  public:
    LAB ();
   ~LAB ();
   
    LAB_Oscilloscope            m_Oscilloscope;
    LAB_Voltmeter               m_Voltmeter;
    LAB_Function_Generator      m_Function_Generator;
    LAB_Logic_Analyzer          m_Logic_Analyzer;  
    LAB_Digital_Circuit_Checker m_Digital_Circuit_Checker;
    LAB_LABChecker_Digital      m_LABChecker_Digital;

    LAB_Exporter                m_Exporter;

    AikaPi& rpi () const;
};

#endif