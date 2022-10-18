#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>

class MyClass : public Fl_Widget
{
  public:
    MyClass (int X, int Y, int W, int H, const char *l = 0) : Fl_Widget (X, Y, W, H, l)
    {

    }

    void draw ()
    {
      draw_box (FL_FLAT_BOX, 0);
    }
};

int main ()
{
  MyClass _MyClass (0, 0, 200, 200, 0);

  Fl::run ();
}