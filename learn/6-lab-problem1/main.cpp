#include <iostream>

#include "pigpio.h"

// compile with
// g++ main.cpp -o 6-lab-problem1

int main ()
{
  gpioInitialise ();

  printf ("Hello, world!");

  gpioTerminate ();
}