#ifndef LAB_CIRCUIT_CHECKER_H
#define LAB_CIRCUIT_CHECKER_H

#include "LAB_Core.h"

// forward declare LAB, #include in .cpp file
class LAB;

class LAB_Circuit_Checker
{
  public:
    LAB*      m_LAB;
    LAB_Core* m_LAB_Core;

    LAB_Circuit_Checker (LAB_Core *_LAB_Core, LAB *_LAB);
};

#endif