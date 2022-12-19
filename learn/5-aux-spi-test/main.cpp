#include <iostream>
#include <chrono>
#include <thread>

#include "LAB_Core.h"

// compile with
// g++ main.cpp Utility.cpp LAB_Core.cpp LabelValue.cpp -o 3-aux-spi-test

int main ()
{
  LAB_Core *core = new LAB_Core ();
  
  char rxbuff[2], txbuff[2];

  txbuff[0] = 0xCA;
  txbuff[1] = 0xFF;

  core->aux_spi0_init ();

  core->aux_spi0_write (txbuff, 2);

  return 0;
}