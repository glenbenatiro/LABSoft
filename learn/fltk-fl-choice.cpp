#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Choice.H>

// compile using fltk-config --compile filename.cpp

int main() {
  Fl_Double_Window win (300, 300, "FL Choice"); 
  win.begin ();

  Fl_Choice choice (0, 0, 60, 120);
  choice.add ("One");
  choice.add ("Two");
  choice.add ("Three");

  choice.value (choice.find_item ("Two"));
  
  win.end ();
  win.show ();

  return Fl::run ();
}