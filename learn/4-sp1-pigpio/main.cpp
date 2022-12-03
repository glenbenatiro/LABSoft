#include <iostream>
#include <pigpio.h>

// compile using
// g++ -Wall -pthread -o main main.c -lpigpio -lrt


constexpr int AUXILIARY = 1 << 8;

int main ()
{
 if (gpioInitialise () < 0) 
 {
    printf ("gpio init fail. exiting.");
    return -1;
 } 
 else
 {
    printf ("gpio init OK\n");
 }

  int fd = spiOpen (2, 1'000'000, AUXILIARY);
  
  char txbuff[2], rxbuff[2];
  txbuff[0] = 0x10;
  txbuff[1] = 0x00;
  spiWrite (fd, txbuff, 2);
  txbuff[0] = 0x40;
  txbuff[1] = 0xFF;
  spiWrite (fd, txbuff, 2);

  gpioTerminate ();
  printf ("program terminated.\n");

}