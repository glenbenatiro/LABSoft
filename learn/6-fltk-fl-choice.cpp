#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Double_Window.H>

int main ()
{
  Fl_Double_Window win (0, 0, 300, 300, "fltk-fl-choice");

  win.begin ();

  Fl_Choice choice (25, 25, 120, 60);
  choice.add ("Choice 1");
  choice.add ("Choice 2");
  choice.add ("Choice 3");

  // walay function nga mu return ug Fl_Menu_Item, 
  //given the menu item's text. so ani nalang. use defaults.h
  choice.value (1);

  win.end ();
  win.show ();


  return Fl::run ();
}