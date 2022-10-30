#include "LAB_Function_Generator.h"

#include "Defaults.h"
#include "Globals.h"

LAB_Function_Generator::
LAB_Function_Generator () 
  : m_channel_signals (LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS)
{

}

void LAB_Function_Generator:: 
update ()
{
  if (m_is_running)
    printf ("LAB Function Generator updated.\n");
}

// EOFs