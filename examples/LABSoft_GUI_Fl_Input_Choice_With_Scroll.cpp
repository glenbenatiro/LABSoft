#include "LABSoft_GUI_Fl_Input_Choice_With_Scroll.h"

#include <string>

#include <FL/Enumerations.H>

#include "../src/Utility/LAB_Constants.h"
#include "../src/Utility/LABSoft_GUI_Label.h"
#include "../src/Utility/LAB_Utility_Functions.h"

LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
LABSoft_GUI_Fl_Input_Choice_With_Scroll (int          X,
                                         int          Y,
                                         int          W,
                                         int          H,
                                         const char*  label)
  : Fl_Input_Choice (X, Y, W, H, label)
{
  set_when ();
}

void LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
set_when ()
{
  when (FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

  input ()->when (FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);
}

int LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
handle (int event)
{
  switch (event)
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
      return (LABSoft_GUI_Fl_Input_Choice_With_Scroll::Fl_Input_Choice::handle (event));
    }
  }
}

void LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
cb_mouse_wheel (int scroll_amount)
{
  int new_index = find_new_index_after_scroll (scroll_amount);

  if (new_index >= 0)
  {
    value (new_index);

    do_callback ();
  }
}

int LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
find_new_index_after_scroll (int scroll_amount)
{
  int current_index = menubutton ()->find_index (value ());

  if (current_index < 0)
  {
    return (get_next_index_after_scroll_with_custom_input (scroll_amount));
  }
  else 
  {
    return (get_next_index_after_scroll (scroll_amount));
  }
}

int LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
get_next_index_after_scroll (int scroll_amount)
{
  int index = menubutton ()->value () + scroll_amount;

  if (index >= (menubutton ()->size () - 1) || index < 0)
  {
    return (-1);
  }
  else 
  {
    return (index);
  }
}

int LABSoft_GUI_Fl_Input_Choice_With_Scroll:: 
get_next_index_after_scroll_with_custom_input (int scroll_amount)
{
  LABSoft_GUI_Label label (value ());

  if (!label.is_valid ())
  {
    return (-1);
  }
  else 
  {
    int menu_button_entries = menubutton ()->size () - 1;
    int index               = -1;

    for (int i = 0; i < (menu_button_entries - 1); i++)
    {
      try 
      {
        double prev = std::stod (static_cast<const char*>(menu ()[i].user_data ()));
        double curr = std::stod (static_cast<const char*>(menu ()[i + 1].user_data ()));
      }
      catch (const std::exception& e)
      {
        return (-1);
      }

      if (LABF::is_within_range (label.actual_value (), prev, curr, LABC::LABSOFT::EPSILON))
      {
        index = i + scroll_amount;

        if (index >= menu_button_entries)
        {
          index = menu_button_entries - 1;
        }
        else if (index < 0)
        {
          index = 0;
        }

        break;
      }
    }

    return (index);
  }
}