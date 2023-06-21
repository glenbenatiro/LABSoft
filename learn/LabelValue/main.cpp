#include <FL/Fl.H>

#include "LabelValue_Test.h"
#include "LabelValue_GUI.h"

// compile by
/**

g++ `fltk-config --cxxflags` main.cpp ../../src/LabelValue.cpp LabelValue_Test.cpp LabelValue_GUI.cpp LabelValue_Controller.cpp LAB.cpp -o LabelValue_Test `fltk-config --ldflags`

*/

int main (int argc, char** argv)
{
  LabelValue_Test prog (argc, argv);

  return (0);
}