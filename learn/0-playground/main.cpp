#include <pigpio.h>
#include <iostream>
#include <unistd.h>

// compile with
// g++ -Wall -pthread main.cpp -o main -lpigpio -lrt

int main ()
{
  printf ("program start\n");

  gpioInitialise ();

  char txbuff[2] = {0xCA, 0x4F};
  char rxbuff[2] = {0x00, 0x00};

  int fd = spiOpen (2, 1000000, (1 << 8));

  spiXfer (fd, txbuff, rxbuff, 2);

  printf ("data: %2X %2X\n", rxbuff[0], rxbuff[1]);


  gpioTerminate ();

  printf ("program end\n");
}