#include "LABSoft_Oscilloscope_Display_Group.h"

LABSoft_Oscilloscope_Display_Group::
LABSoft_Oscilloscope_Display_Group (int X, 
                                    int Y, 
                                    int W, 
                                    int H,
                                    const char *label) 
: Fl_Group (X, 
            Y, 
            W, 
            H, 
            label)
{
  // Add the LABSoft_Oscilloscope_Display widget
  m_display = new LABSoft_Oscilloscope_Display (
    X + LABSOFT_OSCILLOSCOPE_DISPLAY_LEFT_MARGIN, 
    Y + LABSOFT_OSCILLOSCOPE_DISPLAY_TOP_MARGIN, 
    W - LABSOFT_OSCILLOSCOPE_DISPLAY_LEFT_MARGIN - LABSOFT_OSCILLOSCOPE_DISPLAY_RIGHT_MARGIN,
    H - LABSOFT_OSCILLOSCOPE_DISPLAY_TOP_MARGIN - LABSOFT_OSCILLOSCOPE_DISPLAY_BOTTOM_MARGIN,
    0
  );

  // Add x-axis labels 
  for (int a = 0; a < (m_x_labels.size ()); a++)
  {
    double x_coord = a * 
      static_cast<float>(W - LABSOFT_OSCILLOSCOPE_DISPLAY_LEFT_MARGIN - 
      LABSOFT_OSCILLOSCOPE_DISPLAY_RIGHT_MARGIN) / static_cast<float>
      (LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS);
    
    Fl_Box *box = new Fl_Box (
      X + LABSOFT_OSCILLOSCOPE_DISPLAY_LEFT_MARGIN + static_cast<int>(x_coord),
      H + Y - LABSOFT_OSCILLOSCOPE_DISPLAY_BOTTOM_MARGIN + 
        LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_INTRASPACE,
      0,
      0,
      "."
    );

    box->labelcolor (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_COLOR);
    box->labelsize  (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_SIZE);
    box->align      (FL_ALIGN_TEXT_OVER_IMAGE);

    m_x_labels[a] = box;
  }

  // Add y-axis labels
  for (int b = 0; b < (LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS); b++)
  {
    for (int c = 0; c < (m_y_labels.size ()); c++)
    {
      double y_coord = c * 
        (static_cast<float>(H - LABSOFT_OSCILLOSCOPE_DISPLAY_TOP_MARGIN - 
        LABSOFT_OSCILLOSCOPE_DISPLAY_BOTTOM_MARGIN)) / static_cast<float>
        (LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS);
      
      Fl_Box *box = new Fl_Box (
        X + LABSOFT_OSCILLOSCOPE_DISPLAY_LEFT_MARGIN - 
          ((b * LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_INTERSPACE) + 
          LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_PADDING), 
        Y + LABSOFT_OSCILLOSCOPE_DISPLAY_TOP_MARGIN + static_cast<int>(y_coord),
        0,
        0,
        "."
      );

      box->labelcolor (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_COLORS[b]);
      box->labelsize  (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_SIZE);
      box->align      (FL_ALIGN_TEXT_OVER_IMAGE);

      m_y_labels[b][c] = box;
    }
  }      

  // Add a small label at the top of each y-axis label column  
  // denoting the voltage per division unit
  for (int a = 0; a < LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS; a++) 
  {
    // create new label instance using fl_box
    Fl_Box *box = new Fl_Box (
      X + LABSOFT_OSCILLOSCOPE_DISPLAY_LEFT_MARGIN - 
        ((a * LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_INTERSPACE) + 
        LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_PADDING), 
      Y + LABSOFT_OSCILLOSCOPE_DISPLAY_TOP_MARGIN - 
        LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_UNIT_MARGIN,
      0,
      0,
      "."
    );

    box->align      (FL_ALIGN_TEXT_OVER_IMAGE);
    box->labelsize  (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_SIZE);
    box->labelcolor (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_COLORS[a]);

    m_y_label_units[a] = box;
  }

  // create widget for m_upper_osc_disp_info
  {
    m_upper_osc_disp_info = new Fl_Box 
    ( 
      X + LABSOFT_OSCILLOSCOPE_DISPLAY_LEFT_MARGIN + 10,
      Y + LABSOFT_OSCILLOSCOPE_DISPLAY_TOP_MARGIN 
        - LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_UNIT_MARGIN,
      0,
      0,
      "."
    );

    m_upper_osc_disp_info->align      (FL_ALIGN_RIGHT);

    m_upper_osc_disp_info->labelsize  (
      LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_SIZE
    );

    m_upper_osc_disp_info->labelcolor (
      LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_DEFAULT_LABEL_COLOR
    );
  }
  
  end ();
 
  update_voltage_per_division_labels ();
  update_time_per_division_labels ();
}

void LABSoft_Oscilloscope_Display_Group:: 
draw ()
{
  draw_box (FL_FLAT_BOX, LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_BACKGROUND_COLOR);
  draw_children ();
}

void LABSoft_Oscilloscope_Display_Group:: 
load_osc_parent_data  (LAB_Parent_Data_Oscilloscope *parent_data)
{
  m_osc_parent_data = parent_data;

  m_display->load_osc_parent_data (parent_data);
}

int LABSoft_Oscilloscope_Display_Group:: 
reserve_pixel_points ()
{
  if (!m_display)
  {
    return -1;
  }
  else 
  {
    m_display->reserve_pixel_points ();

    return 1;
  }
}

int LABSoft_Oscilloscope_Display_Group::
fill_pixel_points ()
{
  if (!m_display)
  {
    return -1;
  }
  else 
  {
    m_display->fill_pixel_points ();

    return 1;
  }
}

int LABSoft_Oscilloscope_Display_Group:: 
update_voltage_per_division_labels ()
{
  if (!m_osc_parent_data)
  {
    return -1;
  }
  else 
  {
    for (int a = 0; a < m_osc_parent_data->channel_data.size (); a++)
    {
      update_voltage_per_division_labels (a);
    }

    return 1;
  }
}

int LABSoft_Oscilloscope_Display_Group:: 
update_voltage_per_division_labels (unsigned channel)
{
  if (!m_osc_parent_data)
  {
    return -1;
  }
  else
  {
    if (m_osc_parent_data->channel_data[channel].is_enabled)
    {
      char label[15];

      LAB_Channel_Data_Oscilloscope *chan = 
        &(m_osc_parent_data->channel_data[channel]);

      for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS + 1); a++)
      {
        double row_vpd = (-1 * chan->voltage_per_division) * 
          (a - (LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS / 2)) -
          (chan->vertical_offset);

        if (a == 0)
        {
          LabelValue _LabelValue (row_vpd);
          sprintf (label, "%cV", _LabelValue.unit_prefix ());
          m_y_label_units[channel]->copy_label (label);
        }
                  
        sprintf (label, "%3.2f", row_vpd);

        m_y_labels[channel][a]->copy_label (label);

        m_y_labels[channel][a]->labelcolor(
          LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_COLORS[channel]
          );

        m_y_labels[channel][a]->show ();
      }
    }
    else 
    {
      for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS + 1); a++)
      {
        m_y_labels[channel][a]->hide ();
      }
    }

    return 1;
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_time_per_division_labels ()
{
  if (m_osc_parent_data)
  {
    for (int a = 0; a < (LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS + 1); a++)
    {
      double col_tpd = (a + ((LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS / 2) *
        -1)) * (m_osc_parent_data->time_per_division);

      LabelValue _LabelValue (col_tpd + m_osc_parent_data->horizontal_offset, 
        LE_UNIT_SEC);

      m_x_labels[a]->copy_label (_LabelValue.to_label_text ().c_str ());
    }
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_upper_osc_disp_info (const char *text)
{
  m_upper_osc_disp_info->copy_label (text);
}

// EOF