#ifndef LABSOFT_GUI_LOGIC_ANALYZER_DISPLAY_H
#define LABSOFT_GUI_LOGIC_ANALYZER_DISPLAY_H

#include <array>

#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Widget.H>

#include "../Utility/LAB_Definitions.h"

class LABSoft_GUI_Logic_Analyzer_Display_Internal : Fl_Widget
{
  private:
    unsigned m_row_height = 0;
    std::vector<unsigned> m_channel_list;
    LAB_Parent_Data_Logic_Analyzer* m_parent_data = nullptr;
    std::array<std::array<int, 2>, LABC::LOGAN::NUMBER_OF_CHANNELS> m_graph_line_coords;

  private:
    void draw ();
    void draw_grid ();
    void draw_channels ();

  public:
    LABSoft_GUI_Logic_Analyzer_Display_Internal (int X, int Y, int W, int H, const char* label = 0);
   ~LABSoft_GUI_Logic_Analyzer_Display_Internal ();
    
    void  add_channel                     (unsigned channel);
    void  load_logic_analyzer_parent_data (LAB_Parent_Data_Logic_Analyzer& parent_data);
    void  reserve_pixel_points            ();
    void  fill_pixel_points               ();
    void  fill_pixel_points_logan_running ();
    void  fill_pixel_points_logan_stopped ();
    void  calc_graph_line_coords          ();
    void  calc_next_pp                    (bool curr, bool next, int next_x_coord, 
                                           int chan, int curr_index, std::vector<std::array<int, 2>> &pp);
};

class LABSoft_GUI_Logic_Analyzer_Display : public Fl_Group
{
  private:
    Fl_Box* m_display_status = nullptr;
    LABSoft_GUI_Logic_Analyzer_Display_Internal* m_display = nullptr;
    LAB_Parent_Data_Logic_Analyzer* m_parent_data;

    std::array<
      Fl_Box*,
      LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS
    > m_time_per_division_labels;    

  private:
    void create_child_widgets ();  
    void create_child_widgets_time_per_division_labels ();
    void add_channel_button_widget (unsigned channel);
    void draw ();

  public:
    LABSoft_GUI_Logic_Analyzer_Display (int X, int Y, int W, int H, const char* label = 0);
   ~LABSoft_GUI_Logic_Analyzer_Display ();

    void add_channel (unsigned channel, const char* name);
    void load_logic_analyzer_parent_data (LAB_Parent_Data_Logic_Analyzer& parent_data);
    void reserve_pixel_points ();
    void fill_pixel_points ();
    void update_gui_time_per_division_labels ();
    void clear_channels ();
};

#endif