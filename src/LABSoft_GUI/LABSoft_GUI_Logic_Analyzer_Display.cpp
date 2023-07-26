#include "LABSoft_GUI_Logic_Analyzer_Display.h"

// delete soon
#include <iostream>

Fl_Menu_Item LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget::menu_m_fl_menu_button_trigger_mode[] = {
 {"X Ignore", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"0 Low", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"1 High", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"\342\226\262 Rise", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"\342\226\274 Fall", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"\342\206\225 Edge", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

// ========== LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph ==========

LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph:: 
LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph (int X, int Y, int W, int H, const char* label)
  : Fl_Widget (X, Y, W, H, label)
{
  m_pixel_points.reserve (W * 2);
}

LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph::
~LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph:: 
draw ()
{

}

// ========== LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget ==========

LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget (int          X, 
                                                   int          Y, 
                                                   int          W, 
                                                   int          H,   
                                                   const char*  label)
  : Fl_Group (X, Y, W, H, label)
{
  init_child_widgets ();
}

LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
~LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
init_child_widgets ()
{
  m_fl_group_channel_info = new Fl_Group (0, 0, 240, 60);
  m_fl_group_channel_info->box (FL_THIN_DOWN_BOX);
  m_fl_group_channel_info->color (53);
  {
    {
      m_fl_output_name = new Fl_Output (20, 0, 90, 60);
    }
    {
      m_fl_button_channel_info_setting = new Fl_Button (110, 0, 30, 60, "\\");
      m_fl_button_channel_info_setting->box (FL_GTK_UP_BOX);
      m_fl_button_channel_info_setting->color (53);
    }
    {
      m_fl_output_dio_pin = new Fl_Output (140, 0, 60, 60);
      m_fl_output_dio_pin->color (79);
    }
    {
      m_fl_menu_button_trigger_mode = new Fl_Menu_Button (200, 0, 40, 60, "x");
      m_fl_menu_button_trigger_mode->box (FL_GTK_UP_BOX);
      m_fl_menu_button_trigger_mode->color (53);
      m_fl_menu_button_trigger_mode->menu (menu_m_fl_menu_button_trigger_mode);
    }
  }
  m_fl_group_channel_info->end ();  

  m_fl_widget_channel_graph = new LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph (240, 0, 1060, 60);
  m_fl_widget_channel_graph->box(FL_THIN_DOWN_BOX);
  m_fl_widget_channel_graph->color((Fl_Color)53);

  end ();
}

// ========== LABSoft_GUI_Logic_Analyzer_Display ==========

LABSoft_GUI_Logic_Analyzer_Display::
LABSoft_GUI_Logic_Analyzer_Display (int X, int Y, int W, int H, const char* label)
  : Fl_Group (X, Y, W, H, label)
{

}

LABSoft_GUI_Logic_Analyzer_Display::
~LABSoft_GUI_Logic_Analyzer_Display ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display::
draw ()
{
  draw_box      (FL_FLAT_BOX, LABC::LOGAN_DISPLAY::BG_COLOR);
  draw_children ();
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
load_logic_analyzer_parent_data (LAB_Parent_Data_Logic_Analyzer& pdata)
{
  m_parent_data = &pdata;

  // m_display->load_logic_analyzer_parent_data (parent_data);
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
fill_pixel_points ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display:: 
add_channel (unsigned channel, const char* name)
{
  std::cout << "Channel added: " << channel << "\n";
  std::cout << "String name: " << name << "\n";
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
clear_channels ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display:: 
update_gui_time_per_division_labels ()
{

}

// EOF