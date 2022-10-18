#include "LAB_Core.h"
#include <unistd.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <string>

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

bool is_running = false;
uint8_t txbuff[2] = {0xBB, 0x0F};
  uint8_t rxbuff[2] = {0xc0 | 0<<5 , 0x00};

void stream (LAB_Core *core)
{
  while (is_running)
    {
      core->spi_cs(1);
      core->spi_xfer (txbuff, rxbuff, 2);
      core->spi_cs(0);

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  
  std::cout << "Thread exit.";
}

int main ()
{
  LAB_Core m_LAB_Core;

  
  std::string str;
  int freq;


  printf ("program start\n");

  is_running = true;
  std::thread thread1 (stream, &m_LAB_Core);


  while (str != "exit") {
    // std::cout << "Enter freq: ";
    // std::cin >> str;
    // std::cout << "\n";
    
    // freq = std::stoi (str);

    // if (freq >= 1 && freq <= 250'000'000) {
    //   m_LAB_Core.spi_set_clock_rate (freq);
    // }

    uint16_t buff = (rxbuff[0] << 6) | (rxbuff[1] >> 2);
    printf ("val: %d\n", buff);
    printf ("Val: " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "\n\n",
      BYTE_TO_BINARY ((char)(buff>>8)),
      BYTE_TO_BINARY ((char)buff));
    
    usleep(100);
      
  }

  is_running = false;
  thread1.join();

  std::cout << "Program end.";  

  return 0;
}