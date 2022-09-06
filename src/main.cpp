#include <FL/Fl.H>

#include "LABSoft.h"

// compile with
// g++ `fltk-config --cxxflags` main.cpp LABSoft.cpp LABSoft_GUI.cpp 
// LAB.cpp LAB_Core.cpp LAB_Oscilloscope.cpp `fltk-config --ldflags` -lpthread -o LABSoft

// GNOME coding standards
// https://developer.gnome.org/documentation/guidelines/programming/coding-style.html

// regarding motty x11 problem, use:
// sudo xauth add $(xauth -f ~glen/.Xauthority list|tail -1)

int main(int argc, char **argv) 
{
  LABSoft _LABSoft (argc, argv);

  return Fl::run ();
}
