#include "LABSoft_GUI_Fl_Input_Choice_With_Scroll.h"

LABSoft_GUI_Fl_Input_Choice_With_Scroll::
LABSoft_GUI_Fl_Input_Choice_With_Scroll (int         X, 
                                      int         Y, 
                                      int         W, 
                                      int         H, 
                                      const char* label)
  : Fl_Input_Choice (X, Y, W, H, label)
{

}

int LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
handle (int e)
{
  switch (e)
  {    
    case (FL_MOUSEWHEEL):
    {
      if (Fl::belowmouse () == this ||
          Fl::belowmouse () == this->input () || 
          Fl::belowmouse () == this->menubutton ())
      {
        cb_mouse_wheel (Fl::event_dy ());  

        return (1);
      }
    }

    default:
    {
      return (LABSoft_GUI_Fl_Input_Choice_With_Scroll::Fl_Input_Choice::handle (e));
    }
  }
}

void LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
cb_mouse_wheel (int scroll_amount)
{
  int input_index = menubutton ()->find_index (input ()->value ());

  if (input_index < 0)
  {

  }
  else 
  {
    int new_index = input_index + scroll_amount;

    if (!(new_index < 0 || new_index >= menubutton()->size ()))
    {
      value (new_index);

      do_callback ();
    }
  }
}