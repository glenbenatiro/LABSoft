#include <iostream>
#include <string>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Menu_Button.H>

#define LABSOFT_OSCILLOSCOPE_DISPLAY_LABEL_SIZE 5

class out : public Fl_Input_Choice
{
  public:
    out (int X, int Y, int W, int H, const char *l) : Fl_Input_Choice (X, Y, W, H, l)
    {

    }

    int handle (int e) {
      int key = Fl::event_key ();

      switch (e)
        {
          case FL_KEYDOWN:
            if (key == FL_Enter) {
              //printf ("HANDLE wow ok\n");
              
              char s[50];
              sprintf (s, "%s", this->label());
              
              printf ("%s pressed\n", s);
              this->label (this->value());
            }

            else if (key == 32) {
              printf ("spacebar?\n");
            }

            break;
          }         

      return (Fl_Input_Choice::handle(e));
    }
};

void cb_time_per_div (Fl_Input_Choice *w, void *data)
{
  static int x = 0;
  char s[10];

  // sprintf (s, "%d", x++);

  // w->label (s);

  int key = Fl::event_key ();

  if (key == FL_Enter)
    printf ("wow ok\n");
  else
    printf ("callback. last key pressed: %d, %s\n", Fl::event_key(), Fl::event_text());
}

float globals_get_actual_value_from_label (const char *label)
{
  float scaler = 1.0;
  float value = atof (strtok (strdup (label),  " "));
  char unit = strtok (NULL, "/")[0];

  switch (unit)
    {
      case 'G':
          scaler = 1000000000.0;
          break;
      case 'M':
          scaler = 1000000.0;
          break;
      case 'k':
          scaler = 1000.0;
          break;
      case 'm':
          scaler = 0.001;
          break;
      case 'u':
          scaler = 0.000001;
          break;
      case 'n':
          scaler = 0.000000001;
          break;
      default:
          scaler = 1.0;
          break;
    }
  
  return value * scaler;
}

int main ()
{
  Fl_Double_Window win (0, 0, 600, 600, "fltk widget");
  win.begin ();

  out time_per_div (130, 130, 120, 60,"Time per Div");
  time_per_div.callback ((Fl_Callback*)cb_time_per_div, 0);
  //time_per_div.when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
  //time_per_div.when(FL_WHEN_CHANGED);
  time_per_div.add ("1 s");
  time_per_div.add ("2 s");
  time_per_div.value("1s"); 


  win.end ();
  win.show ();

  return Fl::run ();
}

