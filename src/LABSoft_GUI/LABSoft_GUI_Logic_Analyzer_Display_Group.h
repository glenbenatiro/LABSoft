#ifndef LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_H
#define LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_H

#include <array>
#include <vector>

#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>

#include "../Utility/LAB_Constants.h"
#include "LABSoft_GUI_Logic_Analyzer_Display.h"
#include "LABSoft_GUI_Logic_Analyzer_Channel_Button.h"

class LABSoft_GUI_Logic_Analyzer_Display_Group : public Fl_Group
{
  private: 
    LAB_Parent_Data_Logic_Analyzer *m_parent_data = nullptr;

    std::vector<LABSoft_GUI_Logic_Analyzer_Channel_Button*> m_channel_buttons;

    std::array<
      Fl_Box*, 
      LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS + 1
    > m_x_labels;
    
  private:
    void draw ();

  public:
    LABSoft_GUI_Logic_Analyzer_Display *m_display = nullptr;

    // --- Functions ---
    LABSoft_GUI_Logic_Analyzer_Display_Group (int X, int Y, int W, int H, const char *label = 0);

    // Load data
    void  load_logan_parent_data  (LAB_Parent_Data_Logic_Analyzer& parent_data);
    void  reserve_pixel_points    ();
    void  fill_pixel_points       ();

    // Update data
    void update_gui_time_per_division ();

    void add_channel (unsigned channel);
};

#endif 