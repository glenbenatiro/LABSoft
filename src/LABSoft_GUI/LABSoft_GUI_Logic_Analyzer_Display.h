#ifndef LABSOFT_GUI_LOGIC_ANALYZER_DISPLAY_H
#define LABSOFT_GUI_LOGIC_ANALYZER_DISPLAY_H

#include <array>
#include <vector>

#include <FL/Fl_Pack.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Button.H>

#include "../Utility/LAB_Definitions.h"
#include "../Utility/LAB_Defaults.h"

class LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph;
class LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget;
class LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay;

using PixelPoints = std::array<std::vector<std::array<int, 2>>, 
                      LABC::LOGAN::NUMBER_OF_CHANNELS>;
using ChanGraph   = LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph;
using ChanWidget  = LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget;
using Overlay     = LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay;

struct DisplayData
{
  PixelPoints pp;
  unsigned    graph_width = LABD::LOGAN_DISPLAY::CHANNEL_GRAPH_WIDTH;
};

// ========== LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph ==========

class LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph : public Fl_Widget
{
  private:
    int           m_channel       = -1;
    int           m_graph_offset  = 0;
    DisplayData*  m_display_data  = nullptr;
    
  private:
    void draw               ();
    void draw_signal        ();
    void calc_graph_offset  ();
  
  public:
    LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph (int X, int Y, int W, int H, const char* label = 0);
   ~LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph ();
   
    void load_display_data  (DisplayData* display_data);
    void channel            (int channel);
    void graph_offset       (int offset);
};

// ========== LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget ==========

class LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget : public Fl_Group
{
  private:
    bool            m_is_selected   = false;
    int             m_channel       = -1;
    DisplayData*    m_display_data  = nullptr;

  public:
    Fl_Group*                                         m_fl_group_channel_info;
    Fl_Button*                                        m_fl_box_dragger;
    Fl_Output*                                        m_fl_output_name;
    Fl_Button*                                        m_fl_button_channel_info_setting;
    Fl_Output*                                        m_fl_output_dio_pin;
    Fl_Menu_Button*                                   m_fl_menu_button_trigger_mode;
    LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph* m_fl_widget_channel_graph;
    
    static Fl_Menu_Item menu_m_fl_menu_button_trigger_mode[];

  private:
    void init_child_widgets ();
    void draw ();
    void cb_fl_menu_button_trigger_mode (Fl_Menu_Button* w, void* data);

    static void cb_fl_menu_button_trigger_mode_static (Fl_Menu_Button* w, void* data);

  public:
    LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget (int X, int Y, int W, int H, const char* label = 0);
   ~LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget ();

    void  select            ();
    void  deselect          ();
    bool  selected          ();
    void  load_display_data (DisplayData& display_data);
    void  channel           (int channel);
    int   channel           () const;
    void  graph_offset      (int offset);
};

// ========== LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay ==========

class LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay : public Fl_Widget 
{
  private:
    DisplayData* m_display_data;

  private:
    void draw ();
    void draw_grid ();

  public:
    LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay (int X, int Y, int W, int H, const char* label = 0);
   ~LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay ();

   void display_data (DisplayData& display_data);
};

// ========== LABSoft_GUI_Logic_Analyzer_Display ==========

class LABSoft_GUI_Logic_Analyzer_Display : public Fl_Group
{
  private:
    DisplayData                     m_display_data;
    LAB_Parent_Data_Logic_Analyzer* m_parent_data;

    Fl_Pack*                  m_pack;           
    Overlay*                  m_overlay;
    Fl_Scroll*                m_scroll;  
    std::vector<ChanWidget*>  m_channel_widgets;
    std::array<int, 2>        m_graph_base_line_coords;
    
    std::array<
      Fl_Box*,
      LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS
    > m_time_per_division_labels;

  private:
    void        create_channel_widget                       ();
    ChanWidget* create_channel_widget                       (unsigned channel, const char* name);
    void        draw                                        ();
    void        calc_graph_base_line_coords                 ();
    void        calc_pp_coords                              (bool curr_samp, bool next_samp, int next_x, int curr_index, std::vector<std::array<int, 2>>& pixel_points);
    void        calc_chan_widget_graph_offset_last          ();
    void        fill_pixel_points                           ();
    void        fill_pixel_points_backend_running           ();
    void        fill_pixel_points_backend_stopped           ();
    void        init_child_widgets                          ();
    void        init_child_widgets_time_per_division_labels ();
    bool        is_chan_present_in_chan_widget_array        (unsigned channel) const;
    void        reserve_pixel_points                        ();

  public:
    LABSoft_GUI_Logic_Analyzer_Display (int X, int Y, int W, int H, const char* label = 0);
   ~LABSoft_GUI_Logic_Analyzer_Display ();

    void load_logic_analyzer_parent_data      (LAB_Parent_Data_Logic_Analyzer& pdata);
    void add_channel                          (unsigned channel, const char* name = 0);
    void clear_all_channels                   ();
    void update_gui_time_per_division_labels  ();
    void update_display                       ();
};

#endif