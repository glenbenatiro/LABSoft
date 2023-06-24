#include "LAB_Circuit_Checker.h"

#include <chrono>
#include <thread>

#include "LAB.h"

LAB_Circuit_Checker:: 
LAB_Circuit_Checker (LAB_Core* _LAB_Core, LAB* _LAB)
{
  m_LAB_Core = _LAB_Core;
  m_LAB = _LAB;
}

void LAB_Circuit_Checker:: 
parse_circuit_checker_file ()
{

}

void LAB_Circuit_Checker:: 
perform_check ()
{
  double wait_time = 0.0;

  std::array<uint8_t, 10> output;
  std::array<uint8_t, 10> input;

  for (unsigned a = 0; a < output.size (); a++)
  {
    std::this_thread::sleep_for (std::chrono::seconds (wait_time));

    input[a] = m_expander.read_port ()
  }
}