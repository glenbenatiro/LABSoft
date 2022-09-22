#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Choice.H>

void choice_callback (Fl_Choice *w, void *data)
{

  printf ("scaler: %s\n", w->text());
}

int main ()
{
  printf ("hello alf\n");

  Fl_Window *win = new Fl_Window (20, 40, 400, 400, "test");

  Fl_Choice *choice = new Fl_Choice (20, 40, 120, 60);

  choice->add ("V");
  choice->add ("mV");

  choice->value (0);

  printf ("choice 1: %s\nchoice 2: %s\n", choice->text(0), choice->text(1));

  choice->callback((Fl_Callback*)choice_callback);

  win->end();
  win->show();

  return Fl::run();
}