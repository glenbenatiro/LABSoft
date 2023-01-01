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

    double m_time_per_division    = 0.001;  // 1 ms/div 
    double m_position             = 0.0;    // 0 s horizontal offset

    // for UI
    int m_channel_menu_button_width = 120;        // in pixels
    int m_graph_background_color    = 0xFFFFFF00; // white
    int m_graph_grid_color          = 0x00000000;
    int m_x_label_strip_height      = 30;         
    int m_x_label_top_padding       = 10;
    int m_x_label_size              = 10;
    int m_x_label_last_offset       = 18;

  public:
    LABSoft_Logic_Analyzer_Display_Group (int X, int Y, int W, int H, const char *label = 0);

    void draw ();
    void draw_grid ();

    void time_per_division (double value);
    void update_time_per_division_labels ();
    void update_time_per_division_labels (double value);
    void position (double value);
};

#endif 