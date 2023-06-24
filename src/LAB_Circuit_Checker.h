#ifndef LAB_CIRCUIT_CHECKER_H
#define LAB_CIRCUIT_CHECKER_H

#include "LAB_Core.h"
#include "Defaults.h"
#include "LAB_MCP23S17.h"

// forward declare LAB, include LAB.h in cpp file
class LAB;

class LAB_Circuit_Checker
{
  public:
    enum class CIRCUIT_CHECKER_TYPE
    {
      ANALOG_SIMPLE_DC,
      DIGITAL
    };

  public:
    LAB*          m_LAB;
    LAB_Core*     m_LAB_Core;
    LAB_MCP23S17  m_expander;

    LAB_Circuit_Checker (LAB_Core *_LAB_Core, LAB *_LAB);

    void run_circuit_checker_file   ();
    void parse_circuit_checker_file (); 
    void perform_check              ();
};

#endif