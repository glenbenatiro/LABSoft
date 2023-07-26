#include "LABSoft_GUI_Logic_Analyzer_Display.h"

#include <cmath>

// delete soon
#include <iostream>

LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
LABSoft_GUI_Logic_Analyzer_Display_Internal (int          X,
                                             int          Y,
                                             int          W,
                                             int          H,
                                             const char*  label)
  : Fl_Widget (X, Y, W, H, label)
{

}

LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
~LABSoft_GUI_Logic_Analyzer_Display_Internal ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display_Internal::
draw ()
{
  draw_grid     ();
  draw_channels ();
}

void LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
draw_grid ()
{
  double col_width = static_cast<double>(w ()) / 
    LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS;

  fl_color (LABC::LOGAN_DISPLAY::GRID_COLOR);

  for (unsigned col = 1; col < LABC::LOGAN_DISPLAY::NUMBER_OF_COLUMNS; col++)
  {
    if (col == (LABC::LOGAN_DISPLAY::NUMBER_OF_COLUMNS / 2))
    {
      fl_line_style (FL_DASH);
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

  for (unsigned row = 1; row <= m_channel_list.size (); row++)
  {
    fl_line (
      x (),
      y () + (row * m_row_height),
      x () + w (),
      y () + (row * m_row_height)
    );
  }
}

void LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
draw_channels ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
add_channel (unsigned channel)
{

}

void LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
load_logic_analyzer_parent_data (LAB_Parent_Data_Logic_Analyzer& parent_data)
{
  m_parent_data = &parent_data;
}

void LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
reserve_pixel_points ()
{
  LAB_Parent_Data_Logic_Analyzer& pdata = *m_parent_data;

  for (unsigned a = 0; a < (pdata.channel_data.size ()); a++)
  {
    pdata.channel_data[a].pixel_points.reserve (LABC::LOGAN::NUMBER_OF_SAMPLES * 2);
  }
}

void LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
fill_pixel_points ()
{
  if (m_parent_data->is_logan_core_running)
  {
    fill_pixel_points_logan_running ();
  }
  else 
  {
    fill_pixel_points_logan_stopped ();
  }
}

void LABSoft_GUI_Logic_Analyzer_Display_Internal::
fill_pixel_points_logan_running ()
{
  LAB_Parent_Data_Logic_Analyzer& pdata = *m_parent_data;

  for (unsigned chan = 0; chan < (pdata.channel_data.size ()); chan++)
  {
    if (pdata.channel_data[chan].is_enabled)
    {
      LAB_Channel_Data_Logic_Analyzer&  cdata = pdata.channel_data[chan];
      std::vector<std::array<int, 2>>&  pp    = cdata.pixel_points;

      pp.clear ();

      if (pdata.samples >= w ())
      {
        double samp_skipper = (pdata.samples - 1) / static_cast<double>(w ());

        for (int a = 0; a <= w (); a++)
        {
          bool curr = cdata.samples[samp_skipper * a];
          bool next = cdata.samples[samp_skipper * (a + 1)];

          calc_next_pp (curr, next, x () + a + 1, chan, a, pp);
        }
      }
      else
      {
        double x_skipper = static_cast<double>(w ()) / (pdata.samples - 1);

        for (unsigned a = 0; a < pdata.samples; a++)
        {
          bool curr = cdata.samples[a];
          bool next = cdata.samples[a + 1];

          calc_next_pp (curr, next, x () + (x_skipper * a), chan, a, pp);
        }
      }
    }
  }
}

void LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
fill_pixel_points_logan_stopped ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
calc_graph_line_coords ()
{
  double row_height = static_cast<double>(h ()) / (LABC::LOGAN::NUMBER_OF_CHANNELS);

  double row_height_half = row_height / 2.0;

  double row_graph_amp = row_height * 
    ((std::fmod (LABC::LOGAN_DISPLAY::GRAPH_LINE_P2P_SPREAD, 101) / 
    2.0) / 100.0);
  
  for (int chan = 0; chan < (LABC::LOGAN::NUMBER_OF_CHANNELS); chan++)
  {
    m_graph_line_coords[chan][0] = y () + (chan * row_height) + 
      row_height_half + row_graph_amp;

    m_graph_line_coords[chan][1] = y () + (chan * row_height) + 
      row_height_half - row_graph_amp;
  }
}

void LABSoft_GUI_Logic_Analyzer_Display_Internal:: 
calc_next_pp (bool  curr,                           // current logic value
              bool  next,                           // next logic value
              int   next_x_coord,                   // x-coord of next logic value
              int   chan,                           // current logan channel to draw
              int   curr_index,                     // index of the current sample (0 to sample_count)
              std::vector<std::array<int, 2>> &pp)  // pixel point of the current logan channel                               
{
  if (curr_index == 0)
  {
    pp.emplace_back (
      std::array<int, 2> {x (), m_graph_line_coords[chan][curr]}
    );
  }
  else 
  {
    if (curr == next)
    {
      pp.emplace_back (
        std::array<int, 2> {next_x_coord, m_graph_line_coords[chan][next]}
      );
    }
    else 
    {
      pp.emplace_back (
        std::array<int, 2> {next_x_coord, m_graph_line_coords[chan][next ^ 1]}
      );

      pp.emplace_back (
        std::array<int, 2> {next_x_coord, m_graph_line_coords[chan][next]}
      );
    }
  }
}

// ==========

LABSoft_GUI_Logic_Analyzer_Display:: 
LABSoft_GUI_Logic_Analyzer_Display (int         X,
                                    int         Y,
                                    int         W,
                                    int         H,
                                    const char* label)
  : Fl_Group (X, Y, W, H, label)
{
  create_child_widgets ();
}

LABSoft_GUI_Logic_Analyzer_Display:: 
~LABSoft_GUI_Logic_Analyzer_Display ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display:: 
create_child_widgets ()
{
  // 1. internal display
  m_display = new LABSoft_GUI_Logic_Analyzer_Display_Internal (
    x () + LABC::LOGAN_DISPLAY::CHANNEL_BUTTON_WIDTH,
    y () + LABC::LOGAN_DISPLAY::TOP_INFO_STRIP_HEIGHT,
    w () - LABC::LOGAN_DISPLAY::CHANNEL_BUTTON_WIDTH,
    h () - LABC::LOGAN_DISPLAY::TOP_INFO_STRIP_HEIGHT - 
      LABC::LOGAN_DISPLAY::TIME_PER_DIVISION_LABELS_STRIP_HEIGHT
  );

  // 2. time per division labels
  create_child_widgets_time_per_division_labels ();

  end ();
}

void LABSoft_GUI_Logic_Analyzer_Display::
create_child_widgets_time_per_division_labels ()
{
  unsigned  disp_internal_width = w () - LABC::LOGAN_DISPLAY::CHANNEL_BUTTON_WIDTH;
  double    col_width           = static_cast<double>(disp_internal_width) / 
                                  LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS;

  for (unsigned col = 0; col <= m_time_per_division_labels.size (); col++)
  {
    double x_coord = x () + (LABC::LOGAN_DISPLAY::CHANNEL_BUTTON_WIDTH) + 
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

void LABSoft_GUI_Logic_Analyzer_Display:: 
add_channel_button_widget (unsigned channel)
{

}

void LABSoft_GUI_Logic_Analyzer_Display::
draw ()
{
  draw_box      (FL_FLAT_BOX, LABC::LOGAN_DISPLAY::BG_COLOR);
  draw_children ();
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
add_channel (unsigned channel, const char* name)
{
  std::cout << "Channel added: " << channel << "\n";
  std::cout << "String name: " << name << "\n";
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
load_logic_analyzer_parent_data (LAB_Parent_Data_Logic_Analyzer& parent_data)
{
  m_parent_data = &parent_data;

  m_display->load_logic_analyzer_parent_data (parent_data);
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
reserve_pixel_points ()
{
  m_display->reserve_pixel_points ();
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
fill_pixel_points ()
{
  m_display->fill_pixel_points ();
}


void LABSoft_GUI_Logic_Analyzer_Display:: 
update_gui_time_per_division_labels ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display:: 
clear_channels ()
{

}

// EOF