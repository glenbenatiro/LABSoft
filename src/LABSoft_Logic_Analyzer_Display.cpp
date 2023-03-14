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
  fl_push_clip (x (), y (), w (), h ());

  for (int a = 0; a < (m_parent_data->channel_data.size ()); a++)
  {
    LAB_Channel_Data_Logic_Analyzer *chan = &(m_parent_data->channel_data[a]);

    if (chan->is_enabled)
    {
      fl_color (LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_COLOR);

      fl_line_style (
        LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_STYLE,
        LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_WIDTH,
        LABSOFT_LOGIC_ANALYZER_DISPLAY_GRAPH_LINE_DASHES
      );

      for (int b = 0; b < (chan->pixel_point.size ()) - 1; b++)
      {
        fl_line (
          chan->pixel_point[b][0],
          chan->pixel_point[b][1],
          chan->pixel_point[b + 1][0],
          chan->pixel_point[b + 1][1]
        );
      }

      // Set fl line style to default
      fl_line_style (0);
    }
  }

  fl_pop_clip();

  return 0;
}

// 

void LABSoft_Logic_Analyzer_Display::
load_channel_signals (LAB_Parent_Data_Logic_Analyzer *parent_data)
{
  m_parent_data = parent_data;
}
