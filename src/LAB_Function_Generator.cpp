#include "LAB_Function_Generator.h"

#include "Defaults.h"

LAB_Function_Generator::
LAB_Function_Generator () : LAB_Core ()
{
  m_number_of_channels = LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS;

  // new Channel_Signals ()
  m_channel_signals = new Channel_Signals (); 
}

void LAB_Function_Generator:: 
update ()
{
  if (m_is_running)
    printf ("LAB Function Generator updated.\n");
}

// getters
bool LAB_Function_Generator:: 
is_running ()
{
  return m_is_running;
}

Channel_Signals* LAB_Function_Generator:: 
channel_signals ()
{
  return m_channel_signals;
}

// setters
void LAB_Function_Generator:: 
is_running (bool value)
{
  m_is_running = value;
}

// EOFs