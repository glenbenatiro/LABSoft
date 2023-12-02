#include <iostream>
#include <bitset>
#include <chrono>
#include <thread>

#include "../../lib/AikaPi/AikaPi.h"

void printCharArr (char* arr, int size)
{
  for (int i = 0; i < size; i++)
  {
    std::cout << std::bitset<8> (*(arr + i)) << " ";
  }

  std::cout << "\n";
}

int main ()
{
  AikaPi AP;


  char txd[2] {0x00, 0x00};
  char rxd[2] {0x00, 0x00};

  while (true)
  {
    std::cout << "rxd: "; printCharArr (rxd, sizeof (rxd));

    spiXfer (fd, txd, rxd, sizeof (txd));

    AP.spi.

    std::this_thread::sleep_for (std::chrono::milliseconds (50));
  }

}
