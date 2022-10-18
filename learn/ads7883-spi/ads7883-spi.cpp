#include <iostream>
#include <pigpio.h>
#include <unistd.h>
#include <cstdlib>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

// compile with g++ -Wall -pthread -o ads7883-spi ads7883-spi.cpp -lpigpio -lrt

unsigned int SPI_SPEED = 1'000'000;
unsigned int SPI_CHAN = 0;
unsigned int SPI_FLAGS = 0;

char rxbuff[2];
char txbuff[2] {0xBB, 0xBB};

int fd;

int main()
{
  gpioTerminate ();
  printf ("Program start\n");
  if (gpioInitialise() < 0 ) {
    printf ("Error gpioInitialise\n");
    exit (0);
  }

  if ((fd = spiOpen (SPI_CHAN, SPI_SPEED, SPI_SPEED)) < 0) {
    printf ("Error opening SPI fd");
    exit (0);
  } else {
    printf ("opening SPI fd OK\n");
  }

  while (1) {
    int ret = spiXfer (fd, txbuff, rxbuff, 2);
    rxbuff[0] = rxbuff[1] = 0x00;

    usleep(10000);
  
    if (ret < 0) {
      printf ("Error in spiXfer\n");
      exit (0);
    } else {
      //printf ("Bytes xferred: %d\n", ret);
      printf ("Value: " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "\n", 
        BYTE_TO_BINARY (rxbuff[1]), BYTE_TO_BINARY (rxbuff[0]));
    }
  }
  
  spiClose (fd);
  gpioTerminate ();
  printf ("Program end\n");
  
  return 0;
}