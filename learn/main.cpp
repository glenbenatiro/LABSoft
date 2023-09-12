#include "../../lib/AikaPi/AikaPi.h"
#include "../src/LAB_Libraries/LAB_MCP23S17.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <bitset>

// compile with
// g++ main.cpp ../src/LAB_Libraries/LAB_MCP23S17.cpp ../lib/MCP23S17/MCP23S17.cpp ../lib/AikaPi/AikaPi.cpp -o main

constexpr unsigned MISO = 12;
constexpr unsigned MOSI = 3;
constexpr unsigned SCLK = 2;
constexpr unsigned CS   = 25;

int main ()
{
  AikaPi        AP;
  LAB_MCP23S17  xpander (CS, MISO, MOSI, SCLK, 100'000, 0);
  
  xpander.direction (LAB_MCP23S17::PORT::B, LAB_MCP23S17::DIRECTION::INPUT);
  xpander.direction (LAB_MCP23S17::PORT::A, LAB_MCP23S17::DIRECTION::OUTPUT);

  xpander.write (LAB_MCP23S17::PORT::A, 0xFF);

  uint8_t data = xpander.read (LAB_MCP23S17::PORT::B);

  std::cout << "data is: " << std::bitset<8>(data) << "\n";

  return (0);
}