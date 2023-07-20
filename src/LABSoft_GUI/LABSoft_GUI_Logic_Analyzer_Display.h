#ifndef LABSOFT_LOGIC_ANALYZER_DISPLAY
#define LABSOFT_LOGIC_ANALYZER_DISPLAY

#include <array>
#include <vector>

#include <FL/Fl_Widget.H>

#include "../Utility/LAB_Constants.h"
#include "../Utility/LAB_Definitions.h"

class LABSoft_GUI_Logic_Analyzer_Display : public Fl_Widget
{
  private: 
    LAB_Parent_Data_Logic_Analyzer *m_parent_data = nullptr;

    std::array<std::array<int, 2>, LABC::LOGAN::NUMBER_OF_CHANNELS> m_graph_line_coords;

    std::vector<unsigned> m_channel_list;

    unsigned m_row_height = 0;
  
  private:
    void  draw                            ();
    void  draw_grid                       ();
    void  draw_channels                   ();
    void  resize                          ();
    void  calc_graph_line_coords          ();
    void  calc_next_pp                    (bool curr, bool next, int next_x_coord,
                                            int chan, int curr_index, std::vector<std::array<int, 2>> &pp);
    void  fill_pixel_points_logan_running ();
    void  fill_pixel_points_logan_stopped ();

  public:
    LABSoft_GUI_Logic_Analyzer_Display (int X, int Y, int W, int H, const char *label);
   ~LABSoft_GUI_Logic_Analyzer_Display ();

    // Data load/init
    void  load_logan_parent_data  (LAB_Parent_Data_Logic_Analyzer& parent_data);
    void  reserve_pixel_points    ();
    void  fill_pixel_points       ();

    void add_channel  (unsigned channel);
    void row_height   (unsigned value);
    void resize_rows  ();
};

#endif