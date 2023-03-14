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
  m_display = new LABSoft_Oscilloscope_Display (
    X + m_left_padding, 
    Y + m_upper_padding, 
    W - m_left_padding - m_right_padding,
    H - m_upper_padding - m_lower_padding,
    0
  );
  
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

  // create widget for m_upper_osc_disp_info
  m_upper_osc_disp_info = new Fl_Box 
  ( 
    X + m_left_padding + 10,
    Y + m_upper_padding - m_y_label_unit_padding,
    0,
    0,
    "2000 samples at 200 kHz"
  );

  m_upper_osc_disp_info->align      (FL_ALIGN_RIGHT);
  m_upper_osc_disp_info->labelsize  (m_label_size);
  m_upper_osc_disp_info->labelcolor (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_COLOR_WHITE);
  
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
draw_axes_labels ()
{

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
    if (m_channel_signals->m_chans[channel].is_enabled ())
    {
      char label[15];
      Channel_Signal *chn = &(m_channel_signals->m_chans[channel]);

      for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS + 1); a++)
      {
        float value_label = (-1 * chn->osc.volts_per_division) *
          (a - (m_number_of_rows / 2)) - chn->osc.vertical_offset;
        
        if (a == 0)
        {
          LabelValue _LabelValue (value_label);
          sprintf (label, "%cV", _LabelValue.unit_prefix ());
          m_y_label_units[channel]->copy_label (label);
        }
                  
        sprintf (label, "%3.2f", value_label);

        m_y_labels[channel][a]->copy_label (label);
        m_y_labels[channel][a]->labelcolor(m_channels_graph_color[channel]);
        m_y_labels[channel][a]->show ();
      }
    }
    else 
    {
      for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS + 1); a++)
      {
        m_y_labels[channel][a]->hide ();
      }
    }
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_time_per_division_labels ()
{
  if (m_channel_signals)
  {
    Channel_Signal_Oscilloscope *chan = &(m_channel_signals->m_chans[0].osc);
    
    for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS + 1);
      a++)
    {
      double col_time_per_div_value = (a + 
        ((LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS / 2) * -1)) * 
          chan->time_per_division;
    
      LabelValue _LabelValue (col_time_per_div_value + chan->horizontal_offset,
        LE_UNIT_SEC);
       
      m_x_labels[a]->copy_label (_LabelValue.to_label_text ().c_str ());
      m_x_labels[a]->labelcolor (m_default_label_color);
    }
  }
}

void LABSoft_Oscilloscope_Display_Group::  
hide_volts_per_division_labels (unsigned channel)
{
  for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS + 1); a++)
  {
    m_y_labels[channel][a]->hide ();
  }
}

void LABSoft_Oscilloscope_Display_Group::  
show_volts_per_division_labels (unsigned channel)
{
  for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS + 1); a++)
  {
    m_y_labels[channel][a]->show ();
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_upper_osc_disp_info (const char *text)
{
  m_upper_osc_disp_info->copy_label (text);
}


// EOF