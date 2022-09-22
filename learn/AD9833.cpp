#include <iostream>
#include <thread>
#include <chrono>

#include <pigpio.h>

#define ZERO  0x0000
#define RESET 0x0100

void sleep (int milliseconds)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// compile using
//  g++ -Wall -pthread -o AD9833 AD9833.cpp -lpigpio -lrt


int main()
{
  char data[2];

  // initialize pigpio
  if (gpioInitialise() < 0)  {
    printf ("gpioinitialise fail\n");
    return 1;
  }

  // open spi
  int handle = spiOpen (0, 100000, 1);

  if (handle < 0) {
    printf ("open spi fail\n");
    return 1;
  }


  data[1] = 0b00000000;
  data[0] = 0b00000001;
  spiWrite (handle, data, 2);


  // data[1] = 0b00000000;
  // data[0] = 0b00100001;
  // spiWrite (handle, data, 2);

  // data[1] = 0b11000111;
  // data[0] = 0b01010000;
  // spiWrite (handle, data, 2);

  // data[1] = 0b00000000;
  // data[0] = 0b01000000;
  // spiWrite (handle, data, 2);

  // data[1] = 0b00000000;
  // data[0] = 0b11000000;
  // spiWrite (handle, data, 2);

  // data[1] = 0b00000000;
  // data[0] = 0b00100000;
  // spiWrite (handle, data, 2);



  spiClose (handle);

  printf("program end\n");

  return 0;
}