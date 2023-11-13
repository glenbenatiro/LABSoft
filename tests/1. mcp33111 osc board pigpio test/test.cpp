#include <iostream>
#include <bitset>
#include <pigpio.h>

// compile with
// g++ -Wall -pthread -o test test.cpp -lpigpio -lrt

void printCharArr (const char* arr, int size)
{
  for (int i = 0; i < size; i++)
  {
    std::cout << std::bitset<8> (arr + i) << " ";
  }

  std::cout << "\n";
}

int main ()
{
  gpioInitialise ();

  int fd = spiOpen (0, 50'000, 0);

  char txd[4] {0};
  char rxd[4] {};

  spiXfer (fd, txd, rxd, sizeof (txd));

  std::cout << "rxd: "; printCharArr (rxd, sizeof (rxd));
  std::cout << "txd: "; printCharArr (txd, sizeof (txd));

  std::cin.get ();

  spiClose (fd);

  gpioTerminate ();
}
