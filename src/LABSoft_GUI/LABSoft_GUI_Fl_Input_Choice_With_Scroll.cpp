#include "LABSoft_GUI_Fl_Input_Choice_With_Scroll.h"

#include <iostream>

LABSoft_GUI_Fl_Input_Choice_With_Scroll::
LABSoft_GUI_Fl_Input_Choice_With_Scroll (int         X, 
                                      int         Y, 
                                      int         W, 
                                      int         H, 
                                      const char* label)
  : Fl_Input_Choice (X, Y, W, H, label)
{

}

void LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
mouse_wheel_cb (int scroll_amount)
{
  std::cout << "mouse wheel cb: " << label () << "\n";
  
  int input_index = menubutton ()->find_index (input ()->value ());

  if (input_index < 0)
  {
    std::cout << "Item not found!" << "\n";
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

int LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
handle (int e)
{
  switch (e)
  {    
    case (FL_ENTER):
    {
      return (1);
    }

    case (FL_MOUSEWHEEL):
    {
      if (Fl::belowmouse () == this)
      {
        mouse_wheel_cb (Fl::event_dy ());  

        return (1);
      }
    }

    default:
    {
      return (LABSoft_GUI_Fl_Input_Choice_With_Scroll::Fl_Input_Choice::handle (e));
    }
  }
}