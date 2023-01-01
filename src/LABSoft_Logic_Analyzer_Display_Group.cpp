#include "LABSoft_Logic_Analyzer_Display_Group.h"

#include <iostream>

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

LABSoft_Logic_Analyzer_Display_Group::
LABSoft_Logic_Analyzer_Display_Group 
  (int X, int Y, int W, int H, const char *label) 
    : Fl_Group (X, Y, W, H, label)
{
  // Load default values
  m_number_of_channels      = LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_NUMBER_OF_CHANNELS;
  // m_graph_number_of_columns = LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_GRAPH_NUMBER_OF_COLUMNS;
  // m_graph_background_color  = LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_GRAPH_BACKGROUND_COLOR;
  // m_graph_grid_color        = LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_GRAPH_GRID_COLOR;
  // m_x_label_strip_height    = LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_GRAPH_X_LABEL_STRIP_HEIGHT;

  // Generate child widgets
  // Channel menu buttons
  for (int a = 0; a < m_number_of_channels; a++)
  {
    char label[20];
    sprintf (label, "Channel %d", a + 1);

    Fl_Menu_Button *temp = new Fl_Menu_Button (X,                                     // x-coord
                                               Y + (a * ((H - m_x_label_strip_height) / m_number_of_channels)),  // y-coord
                                               m_channel_menu_button_width,           // width
                                               ((H - m_x_label_strip_height) / m_number_of_channels),            // height
                                               0);                                

    temp->copy_label (label);

    m_channel_menu_buttons.emplace_back (temp);
  }


  // x labels
  int actual_graph_display_width = W - m_channel_menu_button_width;
  int column_width = actual_graph_display_width / m_graph_number_of_columns;

  for (int a = 0; a < (m_graph_number_of_columns + 1); a++)
  {
    char label[20];
    sprintf (label, "%d", a + 1);

    Fl_Box *temp = new Fl_Box (X + (m_channel_menu_button_width + (a * column_width)),
                               Y + H - m_x_label_strip_height + m_x_label_top_padding,
                               0,
                               0,
                               0);              

    temp->copy_label (label);

    m_x_labels.emplace_back (temp);
  }

  end ();
}

void LABSoft_Logic_Analyzer_Display_Group::
draw ()
{
  draw_box (FL_FLAT_BOX, m_graph_background_color);
  draw_grid ();
  draw_children ();
}

void LABSoft_Logic_Analyzer_Display_Group::
draw_grid ()
{
  int actual_graph_display_width = w () - m_channel_menu_button_width;
  int column_width = actual_graph_display_width / m_graph_number_of_columns;

  fl_color (m_graph_grid_color);
  fl_line_style (FL_DOT);

  // draw columns
  for (int col = 1; col < m_graph_number_of_columns; col++)
  {
    fl_line (x () + (m_channel_menu_button_width + (col * column_width)),
             y (),
             x () + (m_channel_menu_button_width + (col * column_width)),
             (y () + h ()) - m_x_label_strip_height);
  }

  // draw rows
  for (int row = 1; row <= m_number_of_channels; row++)
  {
    fl_line (x (),
             y () + (row * ((h () - m_x_label_strip_height) / m_number_of_channels)),
             x () + w (),
             y () + (row * ((h () - m_x_label_strip_height) / m_number_of_channels)));
  }
}

