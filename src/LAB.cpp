#include "LAB.h"

#include <iostream>

LAB::LAB ()
{
  m_Oscilloscope = new LAB_Oscilloscope (&(m_LAB_Core));
}

LAB::~LAB ()
{

}