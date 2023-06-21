#ifndef LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_H
#define LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_H

#include <array>

#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Box.H>

#include "Defaults.h"
#include "LABSoft_Logic_Analyzer_Display.h"

class LABSoft_Logic_Analyzer_Display_Group : public Fl_Group
{
  private: 
    LAB_Parent_Data_Logic_Analyzer *m_parent_data_logan = nullptr;

    std::array<Fl_Menu_Button *, LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS>         
      m_channel_menu_buttons;

    std::array<Fl_Box *, LABSOFT_LOGIC_ANALYZER_DISPLAY_NUMBER_OF_COLUMNS + 1>  
      m_x_labels;

  public:
    LABSoft_Logic_Analyzer_Display *m_display = nullptr;

    // --- Functions ---
    LABSoft_Logic_Analyzer_Display_Group (int X, int Y, int W, int H, const char *label = 0);

    // Draw
    void draw ();

    // Load data
    void  load_logan_parent_data  (LAB_Parent_Data_Logic_Analyzer& parent_data);
    void   reserve_pixel_points    ();
    void  fill_pixel_points       ();

    // Update data
    void update_gui_time_per_division ();
};

#endif 