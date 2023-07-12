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


int LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
handle (int e)
{
  switch (e)
  {    
    case FL_MOUSEWHEEL:
    {
      scroll_cb ();

      return 1;
    }

    default:
    {
      return (LABSoft_GUI_Fl_Input_Choice_With_Scroll::Fl_Input_Choice::handle (e));
    }
  }
}

void LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
scroll_menu (SCROLL_DIRECTION direction)
{
  int input_index = menubutton ()->find_index (input ()->value ());

  if (input_index < 0)
  {
    std::cout << "Item not found!" << "\n";
  }
  else 
  {
    std::cout << "Item found!" << "\n";


    // if (!((direction == SCROLL_DIRECTION::UP && input_index == 0) || 
    //   (direction == SCROLL_DIRECTION::DOWN && input_index == ((menubutton ()->size ()) - 1))))
    // {
    //   Fl_Menu_Item* new_item = menu ()[input_index + (direction == )]

    //   // set fl_input
    // }
  }
}

void LABSoft_GUI_Fl_Input_Choice_With_Scroll::
scroll_cb ()
{
  scroll_menu (Fl::event_dy () > 0 ? SCROLL_DIRECTION::DOWN : SCROLL_DIRECTION::UP);

  // do_callback ();
}

// EOF