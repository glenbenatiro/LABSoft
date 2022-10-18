

#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>

int main ()
{

  Fl_Double_Window win (0, 0, 400, 400, "fltk-fl-choice-unit-scake");

  win.begin ();



  win.end ();
  win.show ();

  Fl::lock ();
  return Fl::run ();
}