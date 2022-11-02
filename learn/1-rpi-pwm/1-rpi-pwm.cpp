// compile with
// g++ -o 1-rpi-pwm 1-rpi-pwm.cpp LAB_Core.cpp Auxiliary.cpp

#include <iostream>
#include <unistd.h>

#include "LAB_Core.h"

int main ()
{
  LAB_Core m_LAB_Core;

  int m_sample_rate = 800'000;
  uint32_t m_pwm_range = (PWM_FREQ * 2) / m_sample_rate;

  printf ("Hello world!\n");

  m_LAB_Core.pwm_init (5'000'000, m_pwm_range, 2);
  m_LAB_Core.pwm_start ();

  while (true);

  return 0;
}