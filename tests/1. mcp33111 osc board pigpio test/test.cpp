#include <iostream>
#include <bitset>
#include <chrono>
#include <thread>

#include <pigpio.h>

// compile with
// g++ -Wall -pthread -o test test.cpp -lpigpio -lrt

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
  gpioInitialise ();

  int fd = spiOpen (0, 50'000, 0);

  char txd[2] {0x00, 0x00};
  char rxd[2] {0x00, 0x00};

  spiXfer (fd, txd, rxd, sizeof (txd));

  while (true)
  {
    std::cout << "rxd: "; printCharArr (rxd, sizeof (rxd));

    std::this_thread::sleep_for (std::chrono::milliseconds (10));
  }

  spiClose (fd);

  gpioTerminate ();
}
