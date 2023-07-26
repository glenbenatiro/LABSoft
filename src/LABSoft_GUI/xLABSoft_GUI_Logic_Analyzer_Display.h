#ifndef LABSOFT_GUI_LOGIC_ANALYZER_DISPLAY_H
#define LABSOFT_GUI_LOGIC_ANALYZER_DISPLAY_H

#include <array>

#include <FL/Fl_Group.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Menu_Item.H>

#include "../Utility/LAB_Definitions.h"

class LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget : public Fl_Group
{
  public:
    Fl_Group*       m_fl_group_channel_info;
    Fl_Output*      m_fl_output_name;
    Fl_Button*      m_fl_button_channel_info_setting;
    Fl_Output*      m_fl_output_dio_pin;
    Fl_Menu_Button* m_fl_menu_button_trigger_mode;
    Fl_Widget*      m_fl_widget_channel_graph;

    static Fl_Menu_Item menu_m_fl_menu_button_trigger_mode[];

  private:
    void init_child_widgets ();

  public:
    LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget (int X, int Y, int W, int H, const char* label = 0);
   ~LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget ();
};

class LABSoft_GUI_Logic_Analyzer_Display_Browser_Widget : public Fl_Browser_
{
  private:
    std::list<LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget*> m_channel_widget_list;
};

class LABSoft_GUI_Logic_Analyzer_Display : public Fl_Group
{
  private:
    LAB_Parent_Data_Logic_Analyzer* m_parent_data;

    std::array<
      Fl_Box*,
      LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS
    > m_time_per_division_labels;


  private:
    void create_child_widgets ();
    void create_child_widgets_time_per_division_labels ();
    void draw ();

  public:
    LABSoft_GUI_Logic_Analyzer_Display (int X, int Y, int W, int H, const char* label = 0);
   ~LABSoft_GUI_Logic_Analyzer_Display ();

    void load_logic_analyzer_parent_data (LAB_Parent_Data_Logic_Analyzer& pdata);
    void reserve_pixel_points ();
    void fill_pixel_points ();

    void add_channel (unsigned channel, const char* name);
    void clear_channels ();
    void update_gui_time_per_division_labels ();
};

#endif