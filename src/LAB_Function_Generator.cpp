#include "LAB_Function_Generator.h"

#include <cstdio>

#include "Defaults.h"
#include "Auxiliary.h"

LAB_Function_Generator::
LAB_Function_Generator (LAB_Core *_LAB_Core) 
{
  m_LAB_Core = _LAB_Core;

  
}

void LAB_Function_Generator:: 
update ()
{
  if (m_is_running)
    printf ("LAB Function Generator updated.\n");
}

void LAB_Function_Generator::
wave_type  (int channel, WaveType value)
{

}

void LAB_Function_Generator::
frequency  (int channel, double value)
{
  printf ("frequency: %f\n", value);
}

void LAB_Function_Generator::
period      (int channel, double value)
{

}

void LAB_Function_Generator::
amplitude  (int channel, double value)
{

}

void LAB_Function_Generator::
offfset    (int channel, double value)
{

}


// EOFs