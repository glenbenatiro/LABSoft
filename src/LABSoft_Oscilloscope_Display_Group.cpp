#include "LABSoft_Oscilloscope_Display_Group.h"

#include <iostream>
#include <cmath>

#include <FL/fl_draw.H>


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
  // create new labsoft oscilloscope display instance, with paddings
  m_display = new LABSoft_Oscilloscope_Display (X + m_left_padding, 
                                                Y + m_upper_padding, 
                                                W - m_left_padding - m_right_padding,
                                                H - m_upper_padding - m_lower_padding,
                                                0);
  
  // m_display->m_channel_signals = m_channel_signals;

  // override number of rows and columns in osc disp widget
  // m_display->rows_columns (m_number_of_rows, m_number_of_columns);

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
    lbl->labelcolor (m_default_label_color);
    lbl->labelsize (m_label_size);
    lbl->align (FL_ALIGN_TEXT_OVER_IMAGE);
    m_x_labels.push_back (lbl);
  }

  // resize y labels to accommodate num of chans
  m_y_labels.resize (m_number_of_channels, std::vector<Fl_Box*> (0));

  // dynamically add y axis labels per channel
  for (int a = 0; a < m_number_of_channels; a++) 
  {
    for (int b = 0; b < (m_number_of_rows + 1); b++) 
    {
      // calculate x_coord of label within the osc disp widget
      int y_coord =  b * ((float)(H - m_upper_padding - m_lower_padding) 
        / (float)(m_number_of_rows));

      // create new label instance using fl_box
      Fl_Box *lbl = new Fl_Box (X + m_left_padding - ((a * m_y_label_interspace) + m_y_label_padding), 
                                Y + m_upper_padding + y_coord,
                                0,
                                0,
                                ".");

      lbl->labelcolor (m_default_label_color);
      lbl->labelsize (m_label_size);
      lbl->align (FL_ALIGN_TEXT_OVER_IMAGE);

      m_y_labels[a].emplace_back (lbl);
    }
  }      

  // add label for the channel voltage per div unit
  for (int a = 0; a < m_number_of_channels; a++) 
  {
    // create new label instance using fl_box
    Fl_Box *lbl = new Fl_Box (X + m_left_padding - ((a * m_y_label_interspace) + m_y_label_padding), 
                              Y + m_upper_padding - m_y_label_unit_padding,
                              0,
                              0,
                              ".");

    lbl->align      (FL_ALIGN_TEXT_OVER_IMAGE);
    lbl->labelsize  (m_label_size);
    lbl->labelcolor (m_channels_graph_color[a]);

    m_y_label_units.emplace_back (lbl);
  }
  
  end ();
 
  update_volts_per_division_labels ();
  update_time_per_division_labels ();
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
  char label[10];
  for (int a = 0; a < (m_number_of_columns + 1); a++)
    {
      float label_value = (((a - (m_number_of_columns / 2)) * m_time_per_division) + 
      m_x_offset) * pow (10, (m_time_per_division_unit_scaler * -1));

      // sprintf (label, "%3.4f %cs", label_value, g_get_unit_prefix 
      // (m_time_per_division_unit_scaler));

      m_x_labels[a]->copy_label (label);
    }
}

void LABSoft_Oscilloscope_Display_Group::
update ()
{
  printf ("DEBUG: display updated\n");
}

void LABSoft_Oscilloscope_Display_Group:: 
update_volts_per_division_labels ()
{
  if (m_channel_signals)
  {
    for (int a = 0; a < m_number_of_channels; a++)
    {
      update_volts_per_division_labels (a);
    }
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_volts_per_division_labels (int channel)
{
  if (m_channel_signals)
  {
    char label[15];

    Channel_Signal *chn = &(m_display->m_channel_signals->m_chans[channel]);

    if (chn->is_enabled ())
    {
      for (int a = 0; a < (m_number_of_columns + 1); a++)
      {
        float value_label = (-1 * chn->m_voltage_per_division) *
          (a - (m_number_of_rows / 2)) - chn->m_vertical_offset;
        
        if (a == 0)
        {
          LabelValue _LabelValue (value_label);
          sprintf (label, "%cV", _LabelValue.unit_prefix ());
          m_y_label_units[channel]->copy_label (label);
        }
                  
        sprintf (label, "%3.2f", value_label);

        m_y_labels[channel][a]->copy_label (label);
        m_y_labels[channel][a]->labelcolor(m_channels_graph_color[channel]);
      }
    }
    else 
    {
      sprintf (label, "");
      m_y_label_units[channel]->copy_label (label);

      for (int a = 0; a < (m_number_of_columns + 1); a++)
      {
        m_y_labels[channel][a]->copy_label (label);
      }
    }
  }
}


void LABSoft_Oscilloscope_Display_Group:: 
update_time_per_division_labels ()
{
  if (m_channel_signals)
  {
    for (int a = 0; a < m_number_of_channels; a++)
    {
      update_time_per_division_labels (a);
    }
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_time_per_division_labels (int channel)
{
  if (m_channel_signals)
  {
    char label[15];

    Channel_Signal *chn = &(m_display->m_channel_signals->m_chans[0]);

    LabelValue _LabelValue (chn->m_time_per_division + chn->m_horizontal_offset);

    for (int a = 0; a < (m_number_of_columns + 1); a++)
    {
      int value_label = (a + ((m_number_of_rows / 2) * -1)) * 
        _LabelValue.coefficient ();
          
      sprintf (label, "%3d %cs", value_label, _LabelValue.unit_prefix ());

      m_x_labels[a]->copy_label (label);
      m_x_labels[a]->labelcolor(m_default_label_color);
    }
  }
}


// EOF