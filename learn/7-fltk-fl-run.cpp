#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Output.H>

#include <cstdlib>
#include <thread>
#include <cstdio>

bool is_running = false;
std::thread *thread1;
float val = 0;

void genrand (Fl_Output *w)
{
  while (is_running)
    {
      val =  1500 + (rand () % 100);
    }
}

void update 

void randcb (Fl_Light_Button *w, void *data)
{
  if (w->value () == 1)
    {
      is_running = true;

      thread1 = new std::thread (genrand, (Fl_Output*)data);
    }
  else 
    {
      is_running = false; 
      //thread1->join();
    }
}

void runstopcb (Fl_Light_Button *w, void *data)
{

}

int main ()
{
  Fl_Double_Window win (0, 0, 400, 400, "fltk-fl-choice");

  win.begin ();

  Fl_Output out (25, 25, 120, 60);
  out.value ("Hello");

  Fl_Choice unit (225, 25, 120, 60);
  unit.add ("V");
  unit.add ("mV");
  unit.value (0);

  Fl_Light_Button butt (225, 105, 120, 60, "Rand Start/Stop");
  butt.callback ((Fl_Callback*) randcb, &out);

  Fl_Light_Button runstop (25, 105, 120, 60, "Start/Stop");
  runstop.callback ((Fl_Callback*) runstopcb);

  win.end ();
  win.show ();

  Fl::lock ();

  return Fl::run ();
}