#include "LABSoft_GUI_Fl_Input_Choice_With_Scroll.h"

#include <iostream>

#include "../Utility/LABSoft_GUI_Label.h"

LABSoft_GUI_Fl_Input_Choice_With_Scroll::
LABSoft_GUI_Fl_Input_Choice_With_Scroll (int        X, 
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
  // scroll down is positive
  // scroll up is negative

  int next_index = find_possible_next_index (scroll_amount);

  if (next_index >= 0)
  {
    value (next_index);

    do_callback ();
  }
}

int LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
find_possible_next_index  (int scroll_amount)
{
  int curr_index = menubutton ()->find_index (input ()->value ());

  if (curr_index >= 0)
  {
    int next_index = curr_index + scroll_amount;

    if ((next_index < menubutton ()->size ()) && next_index >= 0)
    {
      return (next_index);
    }
    else 
    {
      return (-1);
    }
  }

}