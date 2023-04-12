#include "LABSoft_Oscilloscope_Display_Group.h"

#include <sstream>

#include <FL/Fl.H>

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
      1, // THIS IS IMPORTANT! Weird FLTK redraw bug if width and height set to 0
      1, // THIS IS IMPORTANT! Weird FLTK redraw bug if width and height set to 0
      ""
    );

    box->labelcolor (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_COLOR);
    box->labelsize  (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_SIZE);
    box->align      (FL_ALIGN_TEXT_OVER_IMAGE);

    m_x_labels[a] = box;
  }

  // Add y-axis labels
  for (int b = 0; b < (LAB_OSCILLOSCOPE::NUMBER_OF_CHANNELS); b++)
  {
    for (int c = 0; c < (m_y_labels[0].size ()); c++)
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
        1, // THIS IS IMPORTANT! Weird FLTK redraw bug if width and height set to 0
        1, // THIS IS IMPORTANT! Weird FLTK redraw bug if width and height set to 0
        ""
      );

      box->labelcolor (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNEL_COLORS[b]);
      box->labelsize  (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_SIZE);
      box->align      (FL_ALIGN_INSIDE | FL_ALIGN_LEFT);

      m_y_labels[b][c] = box;
    }
  }      

  // Add a small label at the top of each y-axis label column  
  // denoting the voltage per division unit
  for (int a = 0; a < LAB_OSCILLOSCOPE::NUMBER_OF_CHANNELS; a++) 
  {
    // create new label instance using fl_box
    Fl_Box *box = new Fl_Box (
      X + LABSOFT_OSCILLOSCOPE_DISPLAY_LEFT_MARGIN - 
        ((a * LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_INTERSPACE) + 
        LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_PADDING), 
      Y + LABSOFT_OSCILLOSCOPE_DISPLAY_TOP_MARGIN - 
        LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_UNIT_MARGIN,
      1, // THIS IS IMPORTANT! Weird FLTK redraw bug if width and height set to 0
      1, // THIS IS IMPORTANT! Weird FLTK redraw bug if width and height set to 0
      ""
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
      1,
      1,
      ""
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
}

void LABSoft_Oscilloscope_Display_Group:: 
draw ()
{
  draw_box      (FL_FLAT_BOX, LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_BACKGROUND_COLOR);
  draw_children ();
}

void LABSoft_Oscilloscope_Display_Group:: 
load_osc_parent_data  (LAB_Parent_Data_Oscilloscope *parent_data)
{
  m_parent_data_osc = parent_data;

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

void LABSoft_Oscilloscope_Display_Group::
fill_pixel_points ()
{
  m_display->fill_pixel_points ();
}

void LABSoft_Oscilloscope_Display_Group:: 
update_voltage_per_division_labels ()
{
  for (int chan = 0; chan < m_parent_data_osc->channel_data.size (); chan++)
  {
    update_voltage_per_division_labels (chan);
  }
}

double LABSoft_Oscilloscope_Display_Group:: 
calc_row_voltage_per_division (int                            row, 
                               unsigned                       number_of_rows,
                               LAB_Channel_Data_Oscilloscope& chan)
{
  double row_vpd = (
    (-1 * chan.voltage_per_division) * 
    (row - (static_cast<double>(number_of_rows) / 2.0)) - 
    (chan.vertical_offset)
  );

  if (is_approx_zero (row_vpd))
  {
    row_vpd = 0.0;
  }

  return (row_vpd);
}


void LABSoft_Oscilloscope_Display_Group:: 
update_voltage_per_division_labels (unsigned channel)
{
  if (m_parent_data_osc->channel_data[channel].is_enabled)
  {
    LabelValue unit_ref;
    LAB_Channel_Data_Oscilloscope &chan = m_parent_data_osc->channel_data[channel];

    for (int a = 0; a < m_y_labels[channel].size (); a++)
    {
      double row_vpd = calc_row_voltage_per_division (
        a, 
        LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_ROWS,
        chan
      );

      LabelValue lv (row_vpd, LABELVALUE_TYPE::VOLTS);

      if (a == 0)
      {
        unit_ref = lv;

        std::stringstream ss;
        ss << unit_ref.unit_prefix () << unit_ref.label_for ();

        m_y_label_units[channel]->copy_label (ss.str ().c_str ());
        m_y_label_units[channel]->show ();
      }
      
      m_y_labels[channel][a]->copy_label (lv.short_value ().c_str ());
      m_y_labels[channel][a]->show ();
    }
  }
  else 
  {
    m_y_label_units[channel]->hide ();

    for (int a = 0; a < m_y_labels[channel].size (); a++)
    {
      m_y_labels[channel][a]->hide ();
    }
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_time_per_division_labels ()
{
  for (int a = 0; a < m_x_labels.size (); a++)
  {
    double col_tpd = (a + ((LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS / 2) *
      -1)) * (m_parent_data_osc->time_per_division);
    
    LabelValue lv (col_tpd + m_parent_data_osc->horizontal_offset,
      LABELVALUE_TYPE::SECONDS);
    
    m_x_labels[a]->copy_label (lv.to_label_text ().c_str ());
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_upper_osc_disp_info ()
{
  LabelValue lv (m_parent_data_osc->sampling_rate, LABELVALUE_TYPE::HERTZ);

  std::stringstream ss;

  ss  << m_parent_data_osc->w_samp_count
      << " samples at "
      << lv.to_label_text ();

  m_upper_osc_disp_info->copy_label (ss.str ().c_str ());
}

void LABSoft_Oscilloscope_Display_Group:: 
update_y_label_unit (unsigned channel)
{

}

bool LABSoft_Oscilloscope_Display_Group::   
is_approx_zero (double x, double epsilon)
{
  return (std::abs (x) < epsilon);
}

// EOF