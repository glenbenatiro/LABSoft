#include <iostream>

#include "pigpio.h"

// compile with
// g++ main.cpp -o 6-lab-problem1 -lpigpio -lrt

constexpr int SPI_CHANNEL = 0;
constexpr int SPI_BAUD    = 100'000;
constexpr int SPI_FLAGS   = 0;

int main ()
{
  char txbuf[3200] = {}, rxbuf[3200] = {};

  gpioInitialise ();

  printf ("Program start\n");

  int fd = spiOpen (SPI_CHANNEL, SPI_BAUD, SPI_FLAGS);
  spiXfer (fd, txbuf, rxbuf, 3200);

  printf ("Read data: %x%x", rxbuf[1], rxbuf[0]);

  printf ("Program end\n");

  gpioTerminate ();

  return 0;
}