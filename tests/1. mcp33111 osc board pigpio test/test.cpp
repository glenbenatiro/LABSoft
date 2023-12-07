#include <iostream>
#include <bitset>
#include <chrono>
#include <thread>
#include <cmath>

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

  int fd = spiOpen (0, 100'000, 0);

  char txd[4] {0xFF, 0xFF, 0x00, 0x00};
  char rxd[4] {0x00, 0x00, 0x00, 0x00};

  while (true)
  {
    spiXfer (fd, txd, rxd, sizeof (txd));

    // uint32_t raw_adc_data = (rxd[0] << 8) | rxd[1];

    // // make it 11 bits
    // raw_adc_data <<= 1;

    // uint32_t adc_value = (raw_adc_data >> 5) & 0x7FF;

    // // get actual value
    //double actual_value = (adc_value) * (5.0 / 2048);

    std::cout << "rxd         : "; printCharArr (rxd, sizeof (rxd));
    std::cout << "actual value: " << actual_value << "\n\n";

    std::this_thread::sleep_for (std::chrono::milliseconds (250));    
  }

  spiClose (fd);

  gpioTerminate ();
}
