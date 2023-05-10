#include "LABSoft_Oscilloscope_Display_Group.h"

#include <sstream>

#include <FL/Fl.H>

#include "LABSoft_GUI.h"

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
  // It is important to set the width and height of the Fl_Box widgets to at
  // least 1 to avoid weird FLTK bug.

  // Add x-labels
  for (int a = 0; a < m_x_labels.size (); a++)
  {
    // This will be repositioned in LABSoft_Controller_Oscilloscope
    Fl_Box* box = new Fl_Box (0, 0, 1, 1, "");

    box->labelcolor (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::X_LABEL_COLOR);
    box->labelsize  (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::AXIS_LABEL_SIZE);
    box->align      ();

    m_x_labels[a] = box;
  }

  // Add y-labels 
  for (int chan = 0; chan < m_y_labels.size (); chan++)
  {
    for (int a = 0; a < m_y_labels[0].size (); a++)
    {
      // This will be repositioned in LABSoft_Controller_Oscilloscope
      Fl_Box* box = new Fl_Box (0, 0, 1, 1, "");

      box->labelcolor (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::CHANNEL_COLORS[chan]);
      box->labelsize  (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::AXIS_LABEL_SIZE);
      box->align      (FL_ALIGN_INSIDE | FL_ALIGN_LEFT);

      m_y_labels[chan][a] = box;
    }
  }

  // Add a small label at the top of each y-axis label column, denoting the
  // voltage per division unit
  for (int chan = 0; chan < m_y_label_units.size (); chan++)
  {
    // This will be repositioned in LABSoft_Controller_Oscilloscope
    Fl_Box* box = new Fl_Box (0, 0, 1, 1, "");

    box->align      ();
    box->labelsize  (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::AXIS_LABEL_SIZE);
    box->labelcolor (LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::CHANNEL_COLORS[chan]);

    m_y_label_units[chan] = box;
  }
  
  // I am not 101% sure if this should be commented out, as there are still 
  // some widgets of LABSoft_Oscilloscope_Display_Group which are defined in
  // the FLUID file.

  //end ();
}

void LABSoft_Oscilloscope_Display_Group:: 
draw ()
{
  draw_box      (FL_FLAT_BOX, LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP::BACKGROUND_COLOR);
  draw_children ();

  m_display->redraw ();
}

void LABSoft_Oscilloscope_Display_Group:: 
load_osc_parent_data (LAB_Parent_Data_Oscilloscope& parent_data)
{
  m_parent_data_osc = &parent_data;

  m_display->load_osc_parent_data (parent_data);
}

void LABSoft_Oscilloscope_Display_Group:: 
reserve_pixel_points ()
{
  m_display->reserve_pixel_points ();
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
    LAB_Channel_Data_Oscilloscope& chan = m_parent_data_osc->channel_data[channel];

    for (int a = 0; a < m_y_labels[channel].size (); a++)
    {
      double row_vpd = calc_row_voltage_per_division (a,
        LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_ROWS, chan);
      
      LabelValue lv (row_vpd, LabelValue::TYPE::VOLTS);

      if (a == 0)
      {
        LabelValue unit_ref = lv; 

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
  double col_half = (LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS / 2.0) * -1;

  for (int a = 0; a < m_x_labels.size (); a++)
  {
    double col_tpd = (a + col_half) * (m_parent_data_osc->time_per_division) + 
      (m_parent_data_osc->horizontal_offset);
    
    LabelValue lv (col_tpd, LabelValue::TYPE::SECONDS);
    
    m_x_labels[a]->copy_label (lv.to_label_text ().c_str ());
  }
}

void LABSoft_Oscilloscope_Display_Group:: 
update_upper_osc_disp_info ()
{
  LabelValue lv (m_parent_data_osc->sampling_rate, LabelValue::TYPE::HERTZ);

  std::stringstream ss;

  ss  << m_parent_data_osc->w_samp_count
      << " samples at "
      << lv.to_label_text ();

  m_upper_info_display->copy_label (ss.str ().c_str ());
}

void LABSoft_Oscilloscope_Display_Group:: 
update_all_display_information ()
{
  update_voltage_per_division_labels  ();
  update_time_per_division_labels     ();
  update_upper_osc_disp_info          ();
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

// Setter
void LABSoft_Oscilloscope_Display_Group:: 
display (LABSoft_Oscilloscope_Display* _LABSoft_Oscilloscope_Display)
{
  m_display = _LABSoft_Oscilloscope_Display;
}

// EOF