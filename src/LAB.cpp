#include "LAB.h"

LAB::LAB ()
{
  m_LAB_Core               = new LAB_Core ();
  m_LAB_Oscilloscope       = new LAB_Oscilloscope ();
  m_LAB_Function_Generator = new LAB_Function_Generator ();
}

LAB::~LAB ()
{

}