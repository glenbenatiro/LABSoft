#include <iostream>
#include <bitset>
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

  char txd[4] {0xFF, 0xFF, 0x00, 0x00};
  char rxd[4] {0x00, 0x00, 0x00, 0x00};

  spiXfer (fd, txd, rxd, sizeof (txd));

  std::cout << "txd: "; printCharArr (txd, sizeof (txd));
  std::cout << "rxd: "; printCharArr (rxd, sizeof (rxd));

  std::cout << "\nPress any key to exit.";
  
  std::cin.get ();

  spiClose (fd);

  gpioTerminate ();
}
