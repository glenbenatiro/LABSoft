#include "LAB_Multimeter.h"

LAB_Multimeter:: 
LAB_Multimeter (LAB_Core *_LAB_Core)
{
  m_LAB_Core = _LAB_Core;
  // Channel_Signals m_channel_signals (1);
}

void LAB_Multimeter:: 
run ()
{
  m_is_running = true;
}

void LAB_Multimeter:: 
stop ()
{
  m_is_running = false;
}

bool LAB_Multimeter:: 
is_running ()
{
  return m_is_running;
}

bool LAB_Multimeter:: 
is_random_value_generator_running ()
{
  return m_is_random_value_generator_running;
}

void LAB_Multimeter:: 
run_random_value_generator ()
{
  m_is_random_value_generator_running = true;
}

void LAB_Multimeter:: 
stop_random_value_generator ()
{
  m_is_random_value_generator_running = false;
}

unsigned LAB_Multimeter:: 
get_sample ()
{
  return 0;
}


// #include "LAB_Meter.h"

// #include <thread>
// #include <chrono>
// #include <cstdlib>

// #include "Defaults.h"


// LAB_Meter:: 
// LAB_Meter () 
// {
//   m_value = LAB_METER_VALUE;
// }

// void LAB_Meter:: 
// random_value_generator ()
// {
//   while (m_is_random_value_generator_running)
//     {
//       m_value = 1.500 + ((rand () % 100) / 1000.0);

//       // std::this_thread::sleep_for (std::chrono::milliseconds (RANDOM_VALUE_GENERATOR_WAIT_MS));
//     }
// }

// // getters
// bool LAB_Meter::
// is_running ()
// {
//   return m_is_running;
// }

// bool LAB_Meter:: 
// is_random_value_generator_running ()
// {
//   return m_is_random_value_generator_running;
// }

// float LAB_Meter::
// value ()
// {
//   return m_value;
// }

// // setters
// void LAB_Meter::
// is_running (bool value)
// {
//   m_is_running = value;

//   // reset value every start
//   LAB_Meter::value (0.0);
// }

// void LAB_Meter:: 
// is_random_value_generator_running (bool value)
// {
//   m_is_random_value_generator_running = value;
// }

// void LAB_Meter:: 
// value (float value)
// {
//   m_value = value; 
// }