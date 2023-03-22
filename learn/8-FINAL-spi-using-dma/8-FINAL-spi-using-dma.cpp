#include <iostream>

#include "../../lib/AikaPi/AikaPi.h"

// compile using
// g++ 8-FINAL-spi-using-dma.cpp ../../lib/AikaPi/AikaPi.cpp -o main

constexpr LAB_PIN_PWM_OUTPUT  = 12;
constexpr PWM_CHAN            = 0;

int main ()
{
  AikaPi AP;

  AP.pwm_init (100'000);
  AP.pwm_algo (PWM_CHAN, AP_PWM_ALGO_MARKSPACE);

  std::string user_input;

  while (true)
  {
    // Get user input
    std::cout << "Input: ";
    std::getline (std::cin, user_input);

    if (user_input == "exit")
    {
      std::cout << "Program will exit...\n";
      return 0;
    }
    else 
    {
      std::cout << "Program will continue...\n";
    }
  }
}