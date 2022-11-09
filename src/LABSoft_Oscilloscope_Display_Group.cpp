#include "LABSoft_Oscilloscope_Display_Group.h"

#include <iostream>
#include <cmath>

#include <FL/fl_draw.H>

#include "Auxiliary.h"
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
    lbl->labelcolor (m_label_color);
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
          Fl_Box *lbl = new Fl_Box (X + m_left_padding - ((a + 1) * m_y_label_padding), 
                                    Y + m_upper_padding + y_coord,
                                    0,
                                    0,
                                    ".");

          lbl->labelcolor (m_label_color);
          lbl->labelsize (m_label_size);
          lbl->align (FL_ALIGN_TEXT_OVER_IMAGE);
          m_y_labels[a].emplace_back (lbl);
        }
      
    }      
  
  end ();

  update_x_axis_labels ();
 
  update_volts_per_division ();
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

      sprintf (label, "%3.4f %cs", label_value, g_get_unit_prefix 
      (m_time_per_division_unit_scaler));

      m_x_labels[a]->copy_label (label);
    }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_y_axis_labels ()
{
  char label[15];

  for (int a = 0; a < m_number_of_channels; a++)
    {
      for (int b = 0; b < (m_number_of_rows + 1); b++)
        {
          Channel_Signal *chn = &(m_display->m_channel_signals.
            m_channel_signal_vector[a]);

          int power_scaler          = chn->m_volts_per_division_unit_scaler;
          double volts_per_division = chn->m_volts_per_division;
          double y_offset           = chn->m_vertical_offset;

          float label_value = (((a - (m_number_of_rows / 2)) * volts_per_division) + 
          y_offset) * pow (10, (power_scaler * -1));

          sprintf (label, "%3.3f %cV", label_value, g_get_unit_prefix 
          (power_scaler));

          m_y_labels[a][b]->copy_label (label);
        }
    }
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
  printf ("DEBUG: display updated\n");
}



void LABSoft_Oscilloscope_Display_Group:: 
update_fg ()
{
  // regenerate wave
  //m_display->regendraw ();
  // normalize to display
  // draw
}

// void LABSoft_Oscilloscope_Display_Group:: 
// update_volts_per_division (double volts_per_division,
//                           int    volts_per_division_unit_scaler,
//                           int    channel)
// {
//   Channel_Signal *chn = &(m_display->m_channel_signals.
//     m_channel_signal_vector[channel]);

//   chn->m_volts_per_division = volts_per_division;
//   chn->m_volts_per_division_unit_scaler = volts_per_division_unit_scaler;
  
//   update_y_axis_labels ();
//   update ();
// }
                        




// setters
void LABSoft_Oscilloscope_Display_Group:: 
update_volts_per_division ()
{
  for (int a = 0; a < m_number_of_channels; a++) 
  {
    ValueStruct _ValueStruct (m_display->m_channel_signals.
      m_channel_signal_vector[a].m_volts_per_division);

    update_volts_per_division (a, _ValueStruct);
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_volts_per_division (int channel, ValueStruct _ValueStruct)
{
  char label[15];

  m_display->volts_per_division (channel, _ValueStruct.actual_value ());

  for (int a = 0; a < (m_number_of_rows + 1); a++)
  {
    int value_label = ((a + ((m_number_of_rows / 2) * -1)) * -1) * 
      _ValueStruct.coefficient ();
        
    sprintf (label, "%3d %cV", value_label, _ValueStruct.unit_prefix ());

    m_y_labels[channel][a]->copy_label (label);
    m_y_labels[channel][a]->labelcolor(m_channels_graph_color[channel]);
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
vertical_offset  (int         channel,
                  ValueStruct _ValueStruct)
{
  m_display->vertical_offset (channel, _ValueStruct.actual_value ());
}

// EOF