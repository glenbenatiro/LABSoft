#ifndef LABSOFT_GUI_FL_INPUT_CHOICE_WITH_SCROLL_H
#define LABSOFT_GUI_FL_INPUT_CHOICE_WITH_SCROLL_H

#include <FL/Fl_Input_Choice.H>

class LABSoft_GUI_Fl_Input_Choice_With_Scroll : public Fl_Input_Choice
{
  private:
    MENU_ITEM_ORDER m_menu_item_order = MENU_ITEM_ORDER::DECREASING;

  private:
    void  set_when                                      ();
    int   handle                                        (int event);
    void  cb_mouse_wheel                                (int scroll_amount);
    int   find_new_index_after_scroll                   (int scroll_amount);
    int   get_next_index_after_scroll                   (int scroll_amount);
    int   get_next_index_after_scroll_with_custom_input (int scroll_amount);
    
  public:
    LABSoft_GUI_Fl_Input_Choice_With_Scroll (int X, int Y, int W, int H, const char* label = 0);
};

#endif