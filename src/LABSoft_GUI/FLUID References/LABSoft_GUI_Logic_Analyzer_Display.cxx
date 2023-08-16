// generated by Fast Light User Interface Designer (fluid) version 1.0305

#include "LABSoft_GUI_Logic_Analyzer_Display.h"

Fl_Menu_Item UserInterface::menu_m_fl_menu_button_trigger_mode[] = {
 {"X Ignore", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"0 Low", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"1 High", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"\342\226\262 Rise", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"\342\226\274 Fall", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"\342\206\225 Edge", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};
UserInterface::UserInterface(int X, int Y, int W, int H, const char *L)
  : Fl_Group(X, Y, W, H, L) {
this->color(FL_LIGHT3);
{ time_per_division_labels_strip = new Fl_Group(0, 510, 1300, 30);
  time_per_division_labels_strip->box(FL_FLAT_BOX);
  time_per_division_labels_strip->color(FL_YELLOW);
  time_per_division_labels_strip->end();
} // Fl_Group* time_per_division_labels_strip
{ top_info_strip = new Fl_Group(0, 0, 1300, 30);
  top_info_strip->box(FL_FLAT_BOX);
  top_info_strip->color(FL_YELLOW);
  top_info_strip->end();
} // Fl_Group* top_info_strip
{ new Fl_Browser(0, 30, 1300, 480);
} // Fl_Browser* o
{ channel widget = new Fl_Group(0, 30, 1300, 60);
  channel widget->box(FL_THIN_DOWN_BOX);
  channel widget->color((Fl_Color)87);
  { Fl_Menu_Button* o = new Fl_Menu_Button(200, 30, 40, 60, "x");
    o->box(FL_GTK_UP_BOX);
    o->color((Fl_Color)53);
  } // Fl_Menu_Button* o
  { m_fl_group_channel_info = new Fl_Group(0, 30, 240, 60);
    m_fl_group_channel_info->box(FL_THIN_DOWN_BOX);
    m_fl_group_channel_info->color((Fl_Color)94);
    { m_fl_output_name = new Fl_Output(20, 30, 90, 60);
    } // Fl_Output* m_fl_output_name
    { m_button_channel_info_setting = new Fl_Button(110, 30, 30, 60, "\\");
      m_button_channel_info_setting->box(FL_GTK_UP_BOX);
      m_button_channel_info_setting->color((Fl_Color)53);
    } // Fl_Button* m_button_channel_info_setting
    { m_fl_output_dio_pin = new Fl_Output(140, 30, 60, 60);
      m_fl_output_dio_pin->color((Fl_Color)79);
    } // Fl_Output* m_fl_output_dio_pin
    { m_fl_menu_button_trigger_mode = new Fl_Menu_Button(200, 30, 40, 60, "x");
      m_fl_menu_button_trigger_mode->box(FL_GTK_UP_BOX);
      m_fl_menu_button_trigger_mode->color((Fl_Color)53);
      m_fl_menu_button_trigger_mode->menu(menu_m_fl_menu_button_trigger_mode);
    } // Fl_Menu_Button* m_fl_menu_button_trigger_mode
    m_fl_group_channel_info->end();
  } // Fl_Group* m_fl_group_channel_info
  { m_fl_widget_channel_graph = new Fl_Group(240, 30, 1060, 60);
    m_fl_widget_channel_graph->box(FL_THIN_DOWN_BOX);
    m_fl_widget_channel_graph->color((Fl_Color)132);
    m_fl_widget_channel_graph->end();
  } // Fl_Group* m_fl_widget_channel_graph
  channel widget->end();
} // Fl_Group* channel widget
end();
}