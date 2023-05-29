#include "LABSoft_Logic_Analyzer_Display.h"

#include <cmath>
#include <iostream>

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

LABSoft_Logic_Analyzer_Display::
LABSoft_Logic_Analyzer_Display (int X,
                                int Y,
                                int W,
                                int H,
                                const char *label = 0) 
  : Fl_Widget (X, Y, W, H, label) 
{
  // fill up graph line coords array
  calc_graph_line_coords ();
}

LABSoft_Logic_Analyzer_Display::
~LABSoft_Logic_Analyzer_Display ()
{

}

void LABSoft_Logic_Analyzer_Display:: 
draw ()
{
  draw_box (FL_FLAT_BOX, LABSOFT_LOGIC_ANALYZER_DISPLAY_BG_COLOR);
  draw_grid ();
  draw_channels ();
}

void LABSoft_Logic_Analyzer_Display:: 
draw_grid ()
{
  double column_width = static_cast<double>(w ()) / 
    static_cast<double>(LABSOFT_LOGIC_ANALYZER_DISPLAY_NUMBER_OF_COLUMNS);

  double row_height   = static_cast<double>(h ()) / 
    static_cast<double>(LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS);

  fl_color      (LABSOFT_LOGIC_ANALYZER_DISPLAY_GRID_COLOR);
  fl_line_style (FL_DOT);

  // draw columns
  for (int col = 1; col < LABSOFT_LOGIC_ANALYZER_DISPLAY_NUMBER_OF_COLUMNS; col++)
  {
    fl_line (
      x () + (col * column_width),
      y (),
      x () + (col * column_width),
      y () + h ()
    );
  }

  // draw rows
  for (int row = 1; row <= LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS; row++)
  {
    if (row == LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS)
    {
      fl_line_style (FL_SOLID);
    }
    else 
    {
      fl_line_style (FL_DOT);
    }

    fl_line (
      x (),
      y () + (row * row_height),
      x () + w (),
      y () + (row * row_height)
    );
  }
}

int LABSoft_Logic_Analyzer_Display:: 
draw_channels ()
{
  if (!m_parent_data_logan)
  {
    return -1;
  }

  if (!m_parent_data_logan->has_enabled_channels ())
  {
    return -2;
  }

  //

  if (m_parent_data_logan->is_enabled)
  {
    
    fl_push_clip (x (), y (), w (), h ());

    LAB_Parent_Data_Logic_Analyzer &pdat  = *m_parent_data_logan;

    fl_color (LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_COLOR);
        
    fl_line_style (
      LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_STYLE,
      LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_WIDTH,
      0
    );

    for (int chan = 0; chan < (pdat.channel_data.size ()); chan++)
    {
      if (pdat.channel_data[chan].is_enabled)
      {
        std::vector<std::array<int, 2>> &pp = pdat.channel_data[chan].pixel_points;
  
        for (int a = 0; a < (pp.size () - 1); a++)
        {
          fl_line (pp[a][0], pp[a][1], pp[a + 1][0], pp[a + 1][1]);
        }
      }
    }

    fl_line_style (0);
    fl_pop_clip();
  }
  
  return 1;
}

void LABSoft_Logic_Analyzer_Display:: 
resize ()
{
  calc_graph_line_coords ();
}

void LABSoft_Logic_Analyzer_Display::
load_logan_parent_data (LAB_Parent_Data_Logic_Analyzer& parent_data)
{
  m_parent_data_logan = &parent_data;
}

int LABSoft_Logic_Analyzer_Display::
reserve_pixel_points ()
{
  if (!m_parent_data_logan)
  {
    return -1;
  }

  // 

  for (int a = 0; a < (m_parent_data_logan->channel_data.size ()); a++)
  {
    m_parent_data_logan->channel_data[a].pixel_points.reserve (
      LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES * 2
    );
  }

  return 1;
}

int LABSoft_Logic_Analyzer_Display::
fill_pixel_points ()
{
  if (!m_parent_data_logan)
  {
    return -1;
  }

  if (!m_parent_data_logan->has_enabled_channels ())
  {
    return -2;
  }

  //

  if (m_parent_data_logan->is_enabled)
  {
    LAB_Parent_Data_Logic_Analyzer &pdat = *m_parent_data_logan;

    for (int chan = 0; chan < (pdat.channel_data.size ()); chan++)
    {
      if (pdat.channel_data[chan].is_enabled)
      {
        LAB_Channel_Data_Logic_Analyzer &cdat = pdat.channel_data[chan];
        std::vector<std::array<int, 2>> &pp   = cdat.pixel_points;

        pp.clear ();

        if (pdat.w_samp_count >= w ())
        {
          double samp_skip = static_cast<double>(cdat.samples.size () - 1) / 
            static_cast<double>(w () - 1);

          for (int a = 0; a < (w () - 1); a++)
          {
            bool curr = cdat.samples[samp_skip * a];
            bool next = cdat.samples[samp_skip * (a + 1)];

            calc_next_pp (
              curr,
              next,
              x () + a + 1,
              chan,
              pp,
              a
            );
          }
        }
        else // pdat.w_samp_count < w () 
        {
          double pix_scale = static_cast<double>(w () - 1) /
            static_cast<double>(pdat.w_samp_count - 1);

          double samp_start_index = (static_cast<double>(cdat.samples.size ()) /
            2.0) - (static_cast<double>(pdat.w_samp_count) / 2.0);

          for (int a = 0; a < (pdat.w_samp_count - 1); a++)
          {
            bool curr = cdat.samples[samp_start_index + a];
            bool next = cdat.samples[samp_start_index + a + 1];

            calc_next_pp (
              curr,
              next,
              x () + (pix_scale * a),
              chan,
              pp,
              a
            );
          }
        }
      }
    }
  }
  
  return 1;
}

int LABSoft_Logic_Analyzer_Display:: 
calc_graph_line_coords ()
{
  double row_height = static_cast<double>(h ()) /
    static_cast<double>(LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS);
  
  double row_height_half = row_height / 2.0;

  double row_graph_amp = row_height * 
    ((std::fmod (LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_P2P_SPREAD, 101) / 
    2.0) / 100.0);
  
  for (int chan = 0; chan < (LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS); chan++)
  {
    m_graph_line_coords[chan][0] = y () + (chan * row_height) + 
      row_height_half + row_graph_amp;

    m_graph_line_coords[chan][1] = y () + (chan * row_height) + 
      row_height_half - row_graph_amp;
  }

  return 1;
}

int LABSoft_Logic_Analyzer_Display:: 
calc_next_pp (bool  curr,
              bool  next,
              int   next_x_coord,
              int   chan,
              std::vector<std::array<int, 2>> &pp,
              int   index)
{
  if (index == 0)
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

  return 1;
}

// EOF