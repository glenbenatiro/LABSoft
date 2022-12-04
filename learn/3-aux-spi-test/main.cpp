#include <iostream>
#include <chrono>
#include <thread>

#include "LAB_Core.h"

// compile with
// g++ main.cpp Auxiliary.cpp LAB_Core.cpp ValueStruct.cpp -o 3-aux-spi-test

int main ()
{
  std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now ();
  
  for (int x = 0; x < 100'000; x++);


  std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now ();

  std::chrono::duration<double, std::nano> elapsed = end - start;

  std::cout << "Waited " << std::fixed << elapsed.count () << " ns\n";


  // LAB_Core *core = new LAB_Core ();
  
  // char rxbuff[2], txbuff[2];

  // txbuff[0] = 0xCA;
  // txbuff[1] = 0xFF;

  // core->aux_spi0_init ();

  // // while (true)
  // // {
  // //   core->aux_spi0_write (txbuff, 2);
  // //   //g_reg32_peek (core->m_aux_regs, AUX_SPI0_PEEK_REG);
  // // }

  // core->aux_spi0_write (txbuff, 2);

  return 0;
}