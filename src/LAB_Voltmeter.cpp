#include "LAB_Voltmeter.h"
#include "LAB.h"

#include <iostream>

LAB_Voltmeter::LAB_Voltmeter (LAB_Core *_LAB_Core, LAB *_LAB)
{
  m_LAB = _LAB;
  m_LAB_Core = _LAB_Core;
}

void LAB_Voltmeter:: 
run ()
{
  if (m_LAB->m_Oscilloscope.m_is_master_running)
  {
    printf ("I cant run, oscilloscope is running!\n");
  }
  else 
  {
    printf ("I can run, oscilloscope is not running!\n");
  }
}

void LAB_Voltmeter:: 
stop ()
{
  
}

double LAB_Voltmeter:: 
get_data_sample ()
{
  double data;

  return data;
}