#include "LAB.h"

LAB::LAB ()
{
  m_Oscilloscope        = new LAB_Oscilloscope        (&(m_LAB_Core));
  m_Function_Generator  = new LAB_Function_Generator  (&(m_LAB_Core));
  m_Multimeter          = new LAB_Multimeter          (&(m_LAB_Core));
}

LAB::~LAB ()
{

}