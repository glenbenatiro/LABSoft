#ifndef LAB_H
#define LAB_H

#include "LAB_Core.h"
#include "LAB_Oscilloscope.h"
#include "LAB_Function_Generator.h"
#include "LAB_Power_Supply.h"
#include "LAB_Meter.h"
#include "LAB_Voltmeter.h"
#include "LAB_Ammeter.h"
#include "LAB_Ohmmeter.h"

class LAB
{
  private:
    
  public:
    // functions
    LAB ();
   ~LAB ();

    LAB_Core               *m_LAB_Core;
    LAB_Oscilloscope       *m_LAB_Oscilloscope;
    LAB_Function_Generator *m_LAB_Function_Generator;
    LAB_Power_Supply       *m_LAB_Power_Supply;
    LAB_Voltmeter          *m_LAB_Voltmeter;
    LAB_Ammeter            *m_LAB_Ammeter;
    LAB_Ohmmeter           *m_LAB_Ohmmeter;
};

#endif