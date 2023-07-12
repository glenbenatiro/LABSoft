#include "LABSoft_GUI_Fl_Choice_With_Scroll.h"

#include "FL/Fl.H"

LABSoft_GUI_Fl_Choice_With_Scroll:: 
LABSoft_GUI_Fl_Choice_With_Scroll (int          X,
                                   int          Y,
                                   int          W,
                                   int          H,
                                   const char*  label)
  : Fl_Choice (X, Y, W, H, label)
{

}

// private
void LABSoft_GUI_Fl_Choice_With_Scroll:: 
mouse_wheel_cb (int scroll_amount)
{
  int new_index = value () + scroll_amount;

  if (!(new_index < 0 || new_index >= size ()))
  {
    value (new_index);
  }
}

// protected
int LABSoft_GUI_Fl_Choice_With_Scroll:: 
handle (int e)
{
  switch (e)
  {
    case (FL_MOUSEWHEEL):
    {
      mouse_wheel_cb (Fl::event_dy ());

      return (1);
    }

    default:
    {
      return (LABSoft_GUI_Fl_Choice_With_Scroll::Fl_Choice::handle (e));
    }
  }
}

// EOF 