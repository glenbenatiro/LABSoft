#include "LAB_Logic_Analyzer.h"

LAB_Logic_Analyzer::
LAB_Logic_Analyzer (LAB_Core *_LAB_Core)
{
  m_LAB_Core = _LAB_Core;

  // Load default values
  m_number_of_channels  = LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS;
  m_memory_depth        = LAB_LOGIC_ANALYZER_MEMORY_DEPTH;

  init_pins ();
}

void LAB_Logic_Analyzer:: 
master_run ()
{
  m_is_running = true;
}

void LAB_Logic_Analyzer:: 
master_stop ()
{
  m_is_running = false;
}

void LAB_Logic_Analyzer:: 
init_pins ()
{
  int arr_length = sizeof (LAB_LOGIC_ANALYZER_CHANNEL_GPIO_PINS) / sizeof (*LAB_LOGIC_ANALYZER_CHANNEL_GPIO_PINS);

  for (int a = 0; a < arr_length; a++)
  {
    m_LAB_Core->gpio_set (LAB_LOGIC_ANALYZER_CHANNEL_GPIO_PINS[a], GPIO_MODE_INPUT, GPIO_PULL_DOWN);
  }
}

// setter
void LAB_Logic_Analyzer:: 
memory_depth (unsigned value)
{
  m_memory_depth = value;

  // propagate values to channel signal array
  
  // m_channel_signals
}