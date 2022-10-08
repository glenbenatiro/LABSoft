#include "LABSoft_Oscilloscope_Display_Group.h"

#include "Defaults.h"

LABSoft_Oscilloscope_Display_Group::
LABSoft_Oscilloscope_Display_Group (int X, 
                                    int Y, 
                                    int W, 
                                    int H,
                                    const char *label) : Fl_Group (X, 
                                                                       Y, 
                                                                       W, 
                                                                       H, 
                                                                       label)
{
  // load defaults
  m_upper_padding       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_UPPER_PADDING;
  m_lower_padding       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LOWER_PADDING;
  m_left_padding        = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LEFT_PADDING;
  m_right_padding       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_RIGHT_PADDING;
  m_x_label_padding     = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_PADDING;
  m_y_label_padding     = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_PADDING;
  m_label_size          = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LABEL_SIZE;
  m_label_color         = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LABEL_COLOR;
  m_x_offset            = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_OFFSET;
  m_time_per_div        = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION;
  m_background_color    = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_BACKGROUND_COLOR;
  m_number_of_channels  = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_CHANNELS;
  m_number_of_rows      = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS;  
  m_number_of_columns   = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_COLUMNS;

  // resize y labels
  m_y_labels.resize (m_number_of_rows, std::vector<Fl_Box*> (m_number_of_channels));
  
  // create new labsoft oscilloscope display instance, with paddings
  m_display = new LABSoft_Oscilloscope_Display (X + m_left_padding, 
                                                Y + m_upper_padding, 
                                                W - m_left_padding - m_right_padding,
                                                H - m_upper_padding - m_lower_padding,
                                                0);

  // override number of rows and columns in osc disp widget
  m_display->rows_columns (m_number_of_rows, m_number_of_columns);

  // dynamically add x axis labels
  for (int a = 0; a < m_number_of_columns + 1; a++)
  {
    // calculate x_coord of label within the osc disp widget
    int x_coord =  a * ((float)(W - m_left_padding - m_right_padding) 
      / (float)(m_number_of_columns));

    // create new label instance using fl_box
    Fl_Box *lbl = new Fl_Box (X + m_left_padding + x_coord, 
                              H + Y - m_lower_padding + m_x_label_padding, 
                              0, 
                              0,
                              ".");
    lbl->labelcolor (m_label_color);
    m_x_labels.push_back (lbl);
  }

  // dynamically add y axis labels per channel
  for (int a = 0; a < m_number_of_channels; a++) 
    {
      for (int b = 0; b < (m_number_of_rows + 1); b++) 
        {
          // calculate x_coord of label within the osc disp widget
          int y_coord =  b * ((float)(H - m_upper_padding - m_lower_padding) 
            / (float)(m_number_of_rows));

          // create new label instance using fl_box
          Fl_Box *lbl = new Fl_Box (X + m_left_padding - ((a + 1) * m_y_label_padding), 
                                    Y + m_upper_padding + y_coord,
                                    0,
                                    0,
                                    "0 v");

          lbl->labelcolor (m_label_color);
          m_y_labels[a].push_back (lbl);
        }
    }      
  
  end ();

  update_x_axis_labels ();
}

void LABSoft_Oscilloscope_Display_Group:: 
draw ()
{
  draw_box (FL_FLAT_BOX, m_background_color);
  draw_children ();
}


void LABSoft_Oscilloscope_Display_Group:: 
update_x_axis_labels ()
{
  char s[50];

  // update x labels
  for (int a = 0; a < m_number_of_columns + 1; a++) 
  { 
    sprintf(s, "%.1f s", ((a + m_x_offset - ((float)m_number_of_columns / 2.0)) * m_time_per_div));
    m_x_labels[a]->copy_label (s);
    m_x_labels[a]->labelcolor (m_label_color);
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_y_axis_labels ()
{

}

void LABSoft_Oscilloscope_Display_Group:: 
enable_function_generator_mode ()
{
  m_is_function_generator_mode_enabled = true;
  m_display->enable_function_generator_mode ();
}

void LABSoft_Oscilloscope_Display_Group::
update ()
{
  // if (m_is_function_generator_mode_enabled)
  //   // {
  //   //   // 
  //   //   for (int a = 0)
  //   // }
}

// setters
void LABSoft_Oscilloscope_Display_Group:: 
rows_columns (int rows, 
              int columns)
{
 m_number_of_rows = rows;
 m_number_of_columns = columns;
}

void LABSoft_Oscilloscope_Display_Group:: 
is_enabled (bool value)
{
  m_is_enabled = value;
  m_display->is_enabled (value);
}

// getters
LABSoft_Oscilloscope_Display* LABSoft_Oscilloscope_Display_Group::
display ()
{
  return m_display;
}

bool LABSoft_Oscilloscope_Display_Group:: 
is_enabled ()
{
  return m_is_enabled;
}

// EOF