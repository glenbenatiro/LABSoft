#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_File_Chooser.H>

// compile using fltk-config --compile filename.cpp

int main ()
{
  Fl_Double_Window win (300, 300,  "File Chooser");
  win.begin ();

  Fl_File_Chooser chooser (".", "*", 0, "Open Circuit Checker File");

  win.end ();
  win.show ();
  return Fl::run ();
}