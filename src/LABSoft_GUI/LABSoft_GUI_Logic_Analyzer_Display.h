#ifndef LABSOFT_LOGIC_ANALYZER_DISPLAY
#define LABSOFT_LOGIC_ANALYZER_DISPLAY

#include <vector>
#include <array>

#include <FL/Fl_Widget.H>

#include "../Utility/Defaults.h"

class LABSoft_GUI_Logic_Analyzer_Display : public Fl_Widget
{
  private: 
    LAB_Parent_Data_Logic_Analyzer *m_parent_data_logan = nullptr;

    std::array<std::array<int, 2>, LABC::LOGAN::NUMBER_OF_CHANNELS> m_graph_line_coords;

  public:
    LABSoft_GUI_Logic_Analyzer_Display (int X, int Y, int W, int H, const char *label);
   ~LABSoft_GUI_Logic_Analyzer_Display ();

    // Draw
    void draw           ();
    void draw_grid      ();
    int  draw_channels  ();
    void resize         ();

    // Data load/init
    void  load_logan_parent_data  (LAB_Parent_Data_Logic_Analyzer& parent_data);
    int   reserve_pixel_points    ();
    int   fill_pixel_points       ();
    int   calc_graph_line_coords  ();
    int   calc_next_pp            (bool curr, bool next, int x_coord, int chan, std::vector<std::array<int, 2>> &pp, int index);
};

#endif