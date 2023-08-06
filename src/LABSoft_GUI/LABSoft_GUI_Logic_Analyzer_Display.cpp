#include "LABSoft_GUI_Logic_Analyzer_Display.h"

#include <cmath>
#include <iostream>

#include "../Utility/LAB_Defaults.h"

Fl_Menu_Item ChanWidget::menu_m_fl_menu_button_trigger_mode[] = {
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

}

LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph::
~LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph:: 
draw ()
{
  draw_box    (LABC::LOGAN_DISPLAY::GRAPH_BOX, LABC::LOGAN_DISPLAY::GRAPH_BG_COLOR);
  draw_signal ();
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph:: 
draw_signal ()
{
  std::vector<std::array<int, 2>>& pp = m_display_data->pp[m_channel];

  if (m_display_data == nullptr || m_channel < 0 || pp.size () == 0)
  {
    return;
  }

  // =====

  fl_color (LABC::LOGAN_DISPLAY::GRAPH_LINE_COLOR);

  fl_line_style (
    LABC::LOGAN_DISPLAY::GRAPH_LINE_STYLE, 
    LABC::LOGAN_DISPLAY::GRAPH_LINE_WIDTH, 
    LABC::LOGAN_DISPLAY::GRAPH_LINE_DASHES
  );

  for (unsigned a = 0; a < (pp.size () - 1); a++)
  {
    fl_line (
      pp[a][0],
      pp[a][1] + m_graph_offset,
      pp[a + 1][0],
      pp[a + 1][1] + m_graph_offset
    );
  }
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph:: 
load_display_data (DisplayData* display_data)
{
  m_display_data = display_data;
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph:: 
channel (int channel)
{
  m_channel = channel;
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph::
graph_offset (int offset)
{
  m_graph_offset = offset;
}

// ========== LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget ==========

LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget (int X, int Y, int W, int H, const char* label)
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
  m_fl_group_channel_info = new Fl_Group (
    0, 
    0, 
    LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH, 
    LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT
  );

  m_fl_group_channel_info->box    (FL_THIN_DOWN_BOX);
  m_fl_group_channel_info->color  (53);
  {
    {
      m_fl_box_dragger = new Fl_Button (
        0,
        0,
        LABC::LOGAN_DISPLAY::CHANNEL_DRAGGER_WIDTH,
        LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT
      );

      m_fl_box_dragger->box (FL_GTK_UP_BOX);
      m_fl_box_dragger->color (53);
    }
    {
      m_fl_output_name = new Fl_Output (
        LABC::LOGAN_DISPLAY::CHANNEL_DRAGGER_WIDTH, 
        0, 
        LABC::LOGAN_DISPLAY::CHANNEL_NAME_WIDTH, 
        LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT
      );
    }
    {
      m_fl_button_channel_info_setting = new Fl_Button  (
        LABC::LOGAN_DISPLAY::CHANNEL_DRAGGER_WIDTH +
        LABC::LOGAN_DISPLAY::CHANNEL_NAME_WIDTH, 
        0, 
        LABC::LOGAN_DISPLAY::CHANNEL_SETTING_WIDTH,
        LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT
      );
      
      m_fl_button_channel_info_setting->box             (FL_GTK_UP_BOX);
      m_fl_button_channel_info_setting->color           (53);
    }
    {
      m_fl_output_dio_pin = new Fl_Output (
        LABC::LOGAN_DISPLAY::CHANNEL_DRAGGER_WIDTH +
        LABC::LOGAN_DISPLAY::CHANNEL_NAME_WIDTH + 
        LABC::LOGAN_DISPLAY::CHANNEL_SETTING_WIDTH,
        0, 
        LABC::LOGAN_DISPLAY::CHANNEL_DIO_WIDTH, 
        LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT
      );

      m_fl_output_dio_pin->color          (79);
    }
    {
      m_fl_menu_button_trigger_mode = new Fl_Menu_Button  (
        LABC::LOGAN_DISPLAY::CHANNEL_DRAGGER_WIDTH +
        LABC::LOGAN_DISPLAY::CHANNEL_NAME_WIDTH + 
        LABC::LOGAN_DISPLAY::CHANNEL_SETTING_WIDTH + 
        LABC::LOGAN_DISPLAY::CHANNEL_DIO_WIDTH, 
        0, 
        LABC::LOGAN_DISPLAY::CHANNEL_TRIGGER_WIDTH, 
        LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT, 
        "x"
      );

      m_fl_menu_button_trigger_mode->box      (FL_GTK_UP_BOX);
      m_fl_menu_button_trigger_mode->color    (53);
      m_fl_menu_button_trigger_mode->menu     (menu_m_fl_menu_button_trigger_mode);
      m_fl_menu_button_trigger_mode->callback ((Fl_Callback*)(cb_fl_menu_button_trigger_mode_static));
    }
  }

  m_fl_group_channel_info->end ();  

  m_fl_widget_channel_graph = new LABSoft_GUI_Logic_Analyzer_Display_Channel_Graph (
    LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH,
    0, 
    LABD::LOGAN_DISPLAY::CHANNEL_GRAPH_WIDTH, 
    LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT
  );

  m_fl_widget_channel_graph->box                (FL_THIN_DOWN_BOX);
  m_fl_widget_channel_graph->color              ((Fl_Color)53);
  m_fl_widget_channel_graph->load_display_data  (m_display_data);
  m_fl_widget_channel_graph->channel            (m_channel);

  end ();
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
draw ()
{
  draw_box ();
  draw_children ();
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
cb_fl_menu_button_trigger_mode (Fl_Menu_Button* w, void* data)
{
  const char* new_label;
  const char* text = w->text ();

  if (text == "X Ignore")
  {
    new_label = "X";
  }
  else if (text == "0 Low")
  {
    new_label = "0";
  }
  else if (text == "1 High")
  {
    new_label = "1";
  }
  else if (text == "\342\226\262 Rise")
  {
    new_label = "\342\226\262";
  }
  else if (text == "\342\226\274 Fall")
  {
    new_label = "\342\226\274";
  }
  else if (text == "\342\206\225 Edge")
  {
    new_label = "\342\206\225";
  }

  w->copy_label (new_label);
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
cb_fl_menu_button_trigger_mode_static (Fl_Menu_Button* w, void* data)
{
  ((LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget*)(w->parent ()))->
    cb_fl_menu_button_trigger_mode (w, data);
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
select ()
{
  m_is_selected = true;
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
deselect ()
{
  m_is_selected = false;
}

bool LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
selected ()
{
  return (m_is_selected);
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
load_display_data (DisplayData& display_data)
{
  m_display_data = &display_data;

  m_fl_widget_channel_graph->load_display_data (m_display_data);
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget::
channel (int channel)
{
  m_channel = channel;

  m_fl_widget_channel_graph->channel (m_channel);
}

int LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget:: 
channel () const
{
  return (m_channel);
}

void LABSoft_GUI_Logic_Analyzer_Display_Channel_Widget::
graph_offset (int offset)
{
  m_fl_widget_channel_graph->graph_offset (offset);
}

// ========== LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay ==========

LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay:: 
LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay (int X, int Y, int W, int H, const char* label)
  : Fl_Widget (X, Y, W, H, label)
{

}

LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay::
~LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay:: 
draw ()
{
  draw_grid ();
}

void LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay:: 
draw_grid ()
{
  double col_width = static_cast<double>(w ()) / LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS;

  fl_color (LABC::LOGAN_DISPLAY::GRID_COLOR);

  // Columns
  for (unsigned col = 0; col <= LABC::LOGAN_DISPLAY::NUMBER_OF_COLUMNS; col++)
  {
    if (col == (LABC::LOGAN_DISPLAY::NUMBER_OF_COLUMNS / 2))
    {
      fl_line_style (FL_DASH);
    }
    else if (col == 0 || col == LABC::LOGAN_DISPLAY::NUMBER_OF_COLUMNS)
    {
      fl_line_style (FL_SOLID);
    }
    else 
    {
      fl_line_style (FL_DOT);
    }

    fl_line (
      x () + (col * col_width), 
      y (), 
      x () + (col * col_width), 
      y () + h ()
    );
  }
}

void LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay:: 
display_data (DisplayData& display_data)
{
  m_display_data = &display_data;
}

// ========== LABSoft_GUI_Logic_Analyzer_Display ==========

LABSoft_GUI_Logic_Analyzer_Display::
LABSoft_GUI_Logic_Analyzer_Display (int X, int Y, int W, int H, const char* label)
  : Fl_Group (X, Y, W, H, label)
{
  init_child_widgets          ();
  reserve_pixel_points        ();
  calc_graph_base_line_coords ();
}

LABSoft_GUI_Logic_Analyzer_Display::
~LABSoft_GUI_Logic_Analyzer_Display ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display:: 
init_child_widgets ()
{
  Fl_Scroll* m_scroll = new Fl_Scroll (
    x (),
    y () + LABC::LOGAN_DISPLAY::TOP_INFO_STRIP_HEIGHT,
    w (),
    h () - LABC::LOGAN_DISPLAY::TOP_INFO_STRIP_HEIGHT - 
      LABC::LOGAN_DISPLAY::TIME_PER_DIVISION_LABELS_STRIP_HEIGHT
  );
  
  m_scroll->type (Fl_Scroll::VERTICAL);

  {
    m_pack = new Fl_Pack (
      x (),
      y () + LABC::LOGAN_DISPLAY::TOP_INFO_STRIP_HEIGHT,
      w (),
      h () - LABC::LOGAN_DISPLAY::TOP_INFO_STRIP_HEIGHT - 
        LABC::LOGAN_DISPLAY::TIME_PER_DIVISION_LABELS_STRIP_HEIGHT
    );

    m_pack->type (Fl_Pack::VERTICAL);
  }

  m_scroll->end ();

  m_overlay = new LABSoft_GUI_Logic_Analyzer_Display_Graph_Overlay (
    x () + LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH,
    y () + LABC::LOGAN_DISPLAY::TOP_INFO_STRIP_HEIGHT,
    LABD::LOGAN_DISPLAY::CHANNEL_GRAPH_WIDTH,
    h () - LABC::LOGAN_DISPLAY::TOP_INFO_STRIP_HEIGHT - 
      LABC::LOGAN_DISPLAY::TIME_PER_DIVISION_LABELS_STRIP_HEIGHT
  );

  m_overlay->display_data (m_display_data);

  init_child_widgets_time_per_division_labels ();

  this->end ();
}

void LABSoft_GUI_Logic_Analyzer_Display::
init_child_widgets_time_per_division_labels ()
{
  unsigned  disp_internal_width = w () - LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH;
  double    col_width           = static_cast<double>(disp_internal_width) / 
                                  LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS;

  for (unsigned col = 0; col <= m_time_per_division_labels.size (); col++)
  {
    double x_coord = x () + (LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH) + 
      (col * col_width);
    
    if (col == LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS)
    {
      x_coord -= LABC::LOGAN_DISPLAY::TIME_PER_DIVSION_LABELS_LAST_OFFSET;
    }

    double y_coord = y () + h () - 
      LABC::LOGAN_DISPLAY::TIME_PER_DIVISION_LABELS_STRIP_HEIGHT + 
      LABC::LOGAN_DISPLAY::TIME_PER_DIVISION_LABELS_TOP_MARGIN;

    Fl_Box* box = new Fl_Box (
      x_coord,
      y_coord,
      5,
      5,
      "0.00 s"
    );

    box->labelcolor (LABC::LOGAN_DISPLAY::TIME_PER_DIVISION_LABELS_COLOR);
    box->labelsize  (LABC::LOGAN_DISPLAY::TIME_PER_DIVISION_LABELS_SIZE);
    box->align      (FL_ALIGN_TEXT_OVER_IMAGE);

    m_time_per_division_labels[col] = box;
  }
}

ChanWidget* LABSoft_GUI_Logic_Analyzer_Display::
create_channel_widget (unsigned channel, const char* name)
{
  ChanWidget* widget = new ChanWidget (
    0, 
    0, 
    LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH + m_display_data.graph_width,
    LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT
  );

  char label[20];
  std::snprintf (label, sizeof (label), "DIO %d", channel);

  if (name == "\0" || name == nullptr)
  {
    widget->m_fl_output_name->value (label);
  }
  else 
  {
    widget->m_fl_output_name->value (name);
  }

  widget->m_fl_output_dio_pin->value  (label);
  widget->m_fl_output_dio_pin->color  (LABC::LOGAN_DISPLAY::CHANNEL_COLORS[channel]);
  widget->load_display_data           (m_display_data);
  widget->channel                     (channel);

  return (widget);
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
fill_pixel_points_backend_running ()
{
  LAB_Parent_Data_Logic_Analyzer& pdata = *m_parent_data;

  for (unsigned chan = 0; chan < pdata.channel_data.size (); chan++)
  {
    if (is_chan_present_in_chan_widget_array (chan))
    {
      LAB_Channel_Data_Logic_Analyzer& cdata  = pdata.channel_data[chan];
      std::vector<std::array<int, 2>>& pp     = m_display_data.pp[chan];

      pp.clear ();

      bool  curr_samp, next_samp;
      int   next_x, i;

      if (pdata.samples >= m_display_data.graph_width)
      {
        double samp_skipper = (pdata.samples - 1) /
          static_cast<double>(m_display_data.graph_width - 1);
        
        for (i = 0; i < (m_display_data.graph_width - 1); i++)
        {
          curr_samp = cdata.samples[std::round (i * samp_skipper)];
          next_samp = cdata.samples[std::round ((i + 1) * samp_skipper)];
          next_x    = x () + LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH + i;

          calc_pp_coords (curr_samp, next_samp, next_x, i, pp);
        }
      }     
      else 
      {
        double pxl_skipper = static_cast<double>(m_display_data.graph_width - 1) / 
          (pdata.samples - 1);

        for (i = 0; i < (pdata.samples - 1); i++)
        {
          curr_samp = cdata.samples[i];
          next_samp = cdata.samples[i + 1];
          next_x    = x () + LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH + std::round ((i + 1) * pxl_skipper);

          calc_pp_coords (curr_samp, next_samp, next_x, i, pp);
        }
      }
    }
  }
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
fill_pixel_points_backend_stopped ()
{
  LAB_Parent_Data_Logic_Analyzer& pdata = *m_parent_data;
}

void LABSoft_GUI_Logic_Analyzer_Display::
calc_pp_coords (bool      curr_samp, 
                bool      next_samp,
                int       next_x, 
                int       curr_index,
                std::vector<std::array<int, 2>>& pp)
{
  if (curr_index == 0)
  {
    pp.emplace_back (
      std::array<int, 2> {x () + LABC::LOGAN_DISPLAY::CHANNEL_INFO_WIDTH, 
        m_graph_base_line_coords[curr_samp]}
    );
  }
  else 
  {
    if (curr_samp == next_samp)
    {
      pp.emplace_back (
        std::array<int, 2> {next_x, m_graph_base_line_coords[next_samp]}
      );
    }
    else 
    {
      pp.emplace_back (
        std::array<int, 2> {next_x, m_graph_base_line_coords[next_samp ^ 1]}
      );

      pp.emplace_back (
        std::array<int, 2> {next_x, m_graph_base_line_coords[next_samp]}
      );
    }
  }
}

bool LABSoft_GUI_Logic_Analyzer_Display:: 
is_chan_present_in_chan_widget_array (unsigned channel) const
{
  for (const ChanWidget* w : m_channel_widgets)
  {
    if (channel == w->channel ())
    {
      return (true);
    }
  }

  return (false);
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
calc_graph_base_line_coords ()
{
  int main_y_offset = y () + LABC::LOGAN_DISPLAY::TOP_INFO_STRIP_HEIGHT + 
    (LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT / 2);

  int graph_y_offset = ((LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT * 
    LABC::LOGAN_DISPLAY::CHANNEL_GRAPH_PEAK_TO_PEAK_SPREAD) / (100.0)) / 2;

  m_graph_base_line_coords[0] = main_y_offset + graph_y_offset;
  m_graph_base_line_coords[1] = main_y_offset - graph_y_offset;
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
calc_chan_widget_graph_offset_last ()
{
  int graph_offset = (m_channel_widgets.size () - 1) * 
    LABC::LOGAN_DISPLAY::CHANNEL_HEIGHT;

  m_channel_widgets.back ()->graph_offset (graph_offset);
}

void LABSoft_GUI_Logic_Analyzer_Display::
reserve_pixel_points ()
{
  for (std::vector<std::array<int, 2>>& pp : m_display_data.pp)
  {
    pp.reserve (LABD::LOGAN_DISPLAY::CHANNEL_GRAPH_WIDTH * 2);
  }
}

void LABSoft_GUI_Logic_Analyzer_Display::
draw ()
{
  draw_box      (FL_FLAT_BOX, LABC::LOGAN_DISPLAY::BG_COLOR);
  draw_children ();
  draw_box      (FL_BORDER_FRAME, 0);
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
load_logic_analyzer_parent_data (LAB_Parent_Data_Logic_Analyzer& pdata)
{
  m_parent_data = &pdata;
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
fill_pixel_points ()
{
  if (m_parent_data->is_backend_running)
  {
    fill_pixel_points_backend_running ();
  }
  else 
  {
    fill_pixel_points_backend_stopped ();
  }
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
add_channel (unsigned channel, const char* name)
{
  ChanWidget* w = create_channel_widget (channel, name);
  
  m_pack->add (w);
  m_channel_widgets.push_back (w);

  calc_chan_widget_graph_offset_last ();

  redraw ();
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
clear_all_channels ()
{
  for (ChanWidget* ptr : m_channel_widgets)
  {
    delete ptr;
  }

  m_channel_widgets.clear ();

  redraw ();
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
update_gui_time_per_division_labels ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display:: 
update_display ()
{
  fill_pixel_points ();

  redraw ();
}

// EOF