#ifndef LAB_H
#define LAB_H

#include "LAB_Core.h"
#include "LAB_Oscilloscope.h"
#include "LAB_Function_Generator.h"

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
};

#endif