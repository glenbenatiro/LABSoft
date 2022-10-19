#include "LAB_Core.h"
#include "LAB_Oscilloscope.h"
#include <unistd.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <string>

int main ()
{
  LAB_Core m_LAB_Core;

  LAB_Oscilloscope m_LAB_Oscilloscope (&m_LAB_Core);
  m_LAB_Oscilloscope.LAB_Oscilloscope_stream_start ();

  while (true) {
    m_LAB_Oscilloscope.LAB_Oscilloscope_stream_csv ();
    usleep(10);
  }

  std::cout << "Program end.";  

  return 0;
}