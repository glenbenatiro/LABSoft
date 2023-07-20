#include "LABSoft_GUI_Logic_Analyzer_Display.h"

#include <cmath>
#include <iostream>

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

LABSoft_GUI_Logic_Analyzer_Display::
LABSoft_GUI_Logic_Analyzer_Display (int X,
                                int Y,
                                int W,
                                int H,
                                const char *label = 0) 
  : Fl_Widget (X, Y, W, H, label) 
{
  // fill up graph line coords array
  calc_graph_line_coords ();
}

LABSoft_GUI_Logic_Analyzer_Display::
~LABSoft_GUI_Logic_Analyzer_Display ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display:: 
draw ()
{
  draw_box      (FL_FLAT_BOX, LABC::LOGAN_DISPLAY::BG_COLOR);
  draw_grid     ();
  draw_channels ();
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
draw_grid ()
{
  double column_width = static_cast<double>(w ()) / 
    LABC::LOGAN::DISPLAY_NUMBER_OF_COLUMNS;

  fl_color      (LABC::LOGAN_DISPLAY::GRID_COLOR);
  fl_line_style (FL_DOT);

  for (unsigned col = 1; col < LABC::LOGAN_DISPLAY::NUMBER_OF_COLUMNS; col++)
  {
    fl_line (
      x () + (col * column_width),
      y (),
      x () + (col * column_width),
      y () + h ()
    );
  }

  for (unsigned row = 1; row <= m_channel_list.size (); row++)
  {
    if (row == LABC::LOGAN::NUMBER_OF_CHANNELS)
    {
      fl_line_style (FL_SOLID);
    }
    else 
    {
      fl_line_style (FL_DOT);
    }

    fl_line (
      x (),
      y () + (row * m_row_height),
      x () + w (),
      y () + (row * m_row_height)
    );
  }
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
draw_channels ()
{
  if (m_parent_data == nullptr)
  {
    return;
  }

  if (!m_parent_data->has_enabled_channels ())
  {
    return;
  }

  // ----------

  fl_push_clip (x (), y (), w (), h ());

  LAB_Parent_Data_Logic_Analyzer &pdata = *m_parent_data;

  unsigned samp_count = (pdata.samples < w () ? pdata.samples : w ()) - 1;

  // Because of how line styles are implemented on Win32 systems, 
  // you must set the line style after setting the drawing color. 
  // If you set the color after the style, style settings will be lost    
  fl_color (LABC::LOGAN_DISPLAY::GRAPH_LINE_COLOR);
        
  fl_line_style (LABC::LOGAN_DISPLAY::GRAPH_LINE_STYLE,
    LABC::LOGAN_DISPLAY::GRAPH_LINE_WIDTH, 0);

  for (unsigned chan = 0; chan < (pdata.channel_data.size ()); chan++)
  {
    if (pdata.channel_data[chan].is_enabled)
    {
      std::vector<std::array<int, 2>> &pp = pdata.channel_data[chan].pixel_points;

      for (unsigned a = 0; a < samp_count; a++)
      {
        fl_line (pp[a][0], pp[a][1], pp[a + 1][0], pp[a + 1][1]);
      }
    }
  }

  fl_color      (0);
  fl_line_style (0);
  fl_pop_clip   ();
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
resize ()
{
  calc_graph_line_coords ();
}

/**
 * Calculate and cache the y-coordinates of the logic 0 and 1 values 
 * of all channels to avoid costly calculations per redraw
 */ 
void LABSoft_GUI_Logic_Analyzer_Display:: 
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

void LABSoft_GUI_Logic_Analyzer_Display:: 
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

void LABSoft_GUI_Logic_Analyzer_Display::
load_logan_parent_data (LAB_Parent_Data_Logic_Analyzer& parent_data)
{
  m_parent_data = &parent_data;
}

void LABSoft_GUI_Logic_Analyzer_Display::
reserve_pixel_points ()
{
  LAB_Parent_Data_Logic_Analyzer& pdata = *m_parent_data;

  for (int a = 0; a < pdata.channel_data.size (); a++)
  {
    pdata.channel_data[a].pixel_points.reserve (LABC::LOGAN::NUMBER_OF_SAMPLES * 2);
  }
}

void LABSoft_GUI_Logic_Analyzer_Display::
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

void LABSoft_GUI_Logic_Analyzer_Display::
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

void LABSoft_GUI_Logic_Analyzer_Display:: 
fill_pixel_points_logan_stopped ()
{

}

void LABSoft_GUI_Logic_Analyzer_Display:: 
add_channel (unsigned channel)
{
  m_channel_list.push_back (channel);

  resize_rows ();
}

/**
 * Set the height of each row. Should be called from 
 * display group to mach the height of each channel button
 */
void LABSoft_GUI_Logic_Analyzer_Display:: 
row_height (unsigned value)
{
  m_row_height = value;
}

void LABSoft_GUI_Logic_Analyzer_Display:: 
resize_rows ()
{

}

// EOF