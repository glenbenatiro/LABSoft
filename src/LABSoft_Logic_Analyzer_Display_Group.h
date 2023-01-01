#ifndef LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_H
#define LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_H

#include <vector>

#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Box.H>

#include "Defaults.h"

class LABSoft_Logic_Analyzer_Display_Group : public Fl_Group
{
  private: 
    std::vector<Fl_Menu_Button*> m_channel_menu_buttons;
    std::vector<Fl_Box*> m_x_labels;

    int m_number_of_channels      =  1;
    int m_graph_number_of_columns = 10;


    // for UI
    int m_channel_menu_button_width = 120;        // in pixels
    int m_graph_background_color    = 0xFFFFFF00; // white
    int m_graph_grid_color          = 0x00000000;
    int m_x_label_strip_height      = 30;         
    int m_x_label_top_padding       = 10;

  public:
    LABSoft_Logic_Analyzer_Display_Group (int X, int Y, int W, int H, const char *label = 0);

    void draw ();
    void draw_grid ();
};

#endif 