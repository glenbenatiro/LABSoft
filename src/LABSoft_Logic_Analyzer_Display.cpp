#include "LABSoft_Logic_Analyzer_Display.h"

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

#include <iostream>

LABSoft_Logic_Analyzer_Display::
LABSoft_Logic_Analyzer_Display (int X,
                                int Y,
                                int W,
                                int H,
                                const char *label = 0) 
  : Fl_Widget (X, Y, W, H, label) 
{
 

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
    static_cast<double>(LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS);

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
  for (int row = 1; row <= LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS; row++)
  {
    if (row == LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS)
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
  if (!m_logan_parent_data)
  {
    return -1;
  }

  if (!(m_logan_parent_data->is_enabled))
  {
    return -2;
  }

  //

  fl_push_clip (x (), y (), w (), h ());

  for (int a = 0; a < (m_logan_parent_data->channel_data.size ()); a++)
  {
    LAB_Channel_Data_Logic_Analyzer &chan = m_logan_parent_data->channel_data[a];

    std::cout << chan.pixel_points.size ();
    std::cout << "\n" << chan.pixel_points[0].size ();
    std::cout << "\n" << chan.samples[1];

    if (chan.is_enabled)
    {
      fl_color (LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_COLOR);

      fl_line_style (
        LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_STYLE,
        LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_WIDTH,
        LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_DASHES
      );

      for (int b = 0; b < (chan.pixel_points.size ()) - 1; b++)
      {
        fl_line (
          chan.pixel_points[b][0],
          chan.pixel_points[b][1],
          chan.pixel_points[b + 1][0],
          chan.pixel_points[b + 1][1]
        );
      }

      // Set fl line style to default
      fl_line_style (0);
    }
  }

  fl_pop_clip();

  return 1;
}

void LABSoft_Logic_Analyzer_Display::
load_logan_parent_data (LAB_Parent_Data_Logic_Analyzer *parent_data)
{
  m_logan_parent_data = parent_data;
}

int LABSoft_Logic_Analyzer_Display::
reserve_pixel_points ()
{
  if (!m_logan_parent_data)
  {
    return -1;
  }
  else 
  {
    for (int a = 0; a < (m_logan_parent_data->channel_data.size ()); a++)
    {
      m_logan_parent_data->channel_data[a].pixel_points.reserve (
        LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * 2
      );
    }

    return 1;
  }
}

int LABSoft_Logic_Analyzer_Display::
fill_pixel_points ()
{
  return 1;
}

// EOF