#include "LABSoft_Logic_Analyzer_Display_Group.h"

#include <iostream>

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl.H>

#include "LabelValue.h"

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

    Fl_Box *box = new Fl_Box 
      (X + (m_channel_menu_button_width + (a * column_width)) - 
        ((a == m_graph_number_of_columns) ? m_x_label_last_offset : 0),
       Y + H - m_x_label_strip_height + m_x_label_top_padding,
       0,
       0,
       0);              

    box->copy_label (label);
    box->align (FL_ALIGN_TEXT_OVER_IMAGE);
    box->labelsize (m_x_label_size);

    m_x_labels.emplace_back (box);
  }

  end ();

  update_time_per_division_labels ();
}

void LABSoft_Logic_Analyzer_Display_Group::
draw ()
{
  draw_box (FL_FLAT_BOX, m_graph_background_color);
  draw_grid ();
  draw_children ();
  draw_signals ();
}

void LABSoft_Logic_Analyzer_Display_Group::
draw_grid ()
{
  int actual_graph_display_width = w () - m_channel_menu_button_width;
  int column_width = actual_graph_display_width / m_graph_number_of_columns;

  fl_color (m_graph_grid_color);
  fl_line_style (FL_DOT);

  // draw columns
  for (int col = 1; col <= m_graph_number_of_columns; col++)
  {
    fl_line (x () + (m_channel_menu_button_width + (col * column_width)),
             y (),
             x () + (m_channel_menu_button_width + (col * column_width)),
             y () + h () - m_x_label_strip_height);
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

void LABSoft_Logic_Analyzer_Display_Group:: 
draw_signals ()
{
  // int graph_width = w () - m_channel_menu_button_width;

  // if (m_channel_signals->m_working_samples >= graph_width)
  // {
    
  // }

  // for (int a = 0; a < m_number_of_channels; a++)
  // {
  //   for (int b = 0; b < m_channel_signals->m_working_samples; b++)
  //   {
  //     m_channel_signals->m_chans[a].logic_analyzer.m_converted_data[b];
  //   }
  // }
}

void LABSoft_Logic_Analyzer_Display_Group:: 
time_per_division (double value)
{
  m_time_per_division = value;
}

void LABSoft_Logic_Analyzer_Display_Group:: 
update_time_per_division_labels ()
{
  for (int a = 0; a <= m_x_labels.size (); a++)
  {
    double temp = m_position + (m_time_per_division * (a - (m_graph_number_of_columns / 2)));
    m_x_labels[a]->copy_label ((LabelValue::label_text (temp, TIME_PER_DIVISION)).c_str());
  }
}

void LABSoft_Logic_Analyzer_Display_Group:: 
update_time_per_division_labels (double value)
{
  time_per_division (value);
  update_time_per_division_labels ();
}

void LABSoft_Logic_Analyzer_Display_Group:: 
position (double value)
{
  m_position = value;
}

void LABSoft_Logic_Analyzer_Display_Group:: 
channel_signals (const Channel_Signals *_Channel_Signals)
{
  m_channel_signals = _Channel_Signals;
}

// eof