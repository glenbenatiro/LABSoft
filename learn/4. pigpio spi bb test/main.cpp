#include <iostream>
#include <pigpio.h>

// compile using
// g++ -Wall -pthread main.cpp -o main -lpigpio -lrt

constexpr int SPI_BB_CE0 = 5;
constexpr int SPI_BB_MOSI = 15;
constexpr int SPI_BB_MISO = 26;
constexpr int SPI_BB_CLK = 14;
constexpr int SPI_BB_FREQ = 1'000;

int main ()
{
  char txbuff[2] = {0x0F, 0xAC};
  char rxbuff[2] = {0x00, 0x00};
  
  if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed.\n");
      return 1;
   }

  bbSPIOpen (SPI_BB_CE0, SPI_BB_MISO, SPI_BB_MOSI, SPI_BB_CLK, SPI_BB_FREQ, 0);

  txbuff[0] = 0xAE;
  txbuff[1] = 0x48;
  bbSPIXfer (SPI_BB_CE0, txbuff, rxbuff, 2);

  txbuff[0] = 0xFF;
  txbuff[1] = 0x23;
  bbSPIXfer (SPI_BB_CE0, txbuff, rxbuff, 2);

  txbuff[0] = 0xC2;
  txbuff[1] = 0x96;
  bbSPIXfer (SPI_BB_CE0, txbuff, rxbuff, 2);


  bbSPIClose(SPI_BB_CE0);

  gpioTerminate();

  return 0;
}