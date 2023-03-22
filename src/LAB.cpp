#include "LAB.h"

#include <cstring>

LAB::LAB ()
: m_Oscilloscope        (&(m_LAB_Core), this),
  m_Voltmeter           (&(m_LAB_Core), this),
  m_Function_Generator  (&(m_LAB_Core)),
  m_Logic_Analyzer      (&(m_LAB_Core), this),
  m_Circuit_Checker     (&(m_LAB_Core), this)
{

}

LAB::~LAB ()
{

}
