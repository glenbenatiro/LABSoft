#include "LABSoft_GUI_Oscilloscope_Display_Group.h"

#include <sstream>

#include "../Utility/LabelValue.h"

LABSoft_GUI_Oscilloscope_Display_Group::
LABSoft_GUI_Oscilloscope_Display_Group (int X, 
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

  // 1. Add x-labels
  for (int a = 0; a < m_x_labels.size (); a++)
  {
    // This will be repositioned in LABSoft_Controller_Oscilloscope
    Fl_Box* box = new Fl_Box (0, 0, 1, 1, "");

    box->labelcolor (LABC::OSC_DISPLAY_GROUP::X_LABEL_COLOR);
    box->labelsize  (LABC::OSC_DISPLAY_GROUP::AXIS_LABEL_SIZE);
    box->align      ();

    m_x_labels[a] = box;
  }

  // 2. Add y-labels 
  for (int chan = 0; chan < m_y_labels.size (); chan++)
  {
    for (int a = 0; a < m_y_labels[0].size (); a++)
    {
      // This will be repositioned in LABSoft_Controller_Oscilloscope
      Fl_Box* box = new Fl_Box (0, 0, 1, 1, "");

      box->labelcolor (LABC::DISPLAY::CHAN_COLORS[chan]);
      box->labelsize  (LABC::OSC_DISPLAY_GROUP::AXIS_LABEL_SIZE);
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
    box->labelsize  (LABC::OSC_DISPLAY_GROUP::AXIS_LABEL_SIZE);
    box->labelcolor (LABC::DISPLAY::CHAN_COLORS[chan]);

    m_y_label_units[chan] = box;
  }
  
  // I am not 101% sure if this should be commented out, as there are still 
  // some widgets of LABSoft_GUI_Oscilloscope_Display_Group which are defined in
  // the FLUID file.

  //end ();
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
draw ()
{
  update_display_status ();

  draw_box      (FL_FLAT_BOX, LABC::OSC_DISPLAY_GROUP::BACKGROUND_COLOR);
  draw_children ();

  m_display->redraw ();
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
load_osc_parent_data (LAB_Parent_Data_Oscilloscope& parent_data)
{
  m_parent_data = &parent_data;

  m_display->load_osc_parent_data (parent_data);
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
reserve_pixel_points ()
{
  m_display->reserve_pixel_points ();
}

void LABSoft_GUI_Oscilloscope_Display_Group::
fill_pixel_points ()
{
  m_display->fill_pixel_points ();
}

double LABSoft_GUI_Oscilloscope_Display_Group:: 
calc_row_voltage_per_division (int                            row, 
                               unsigned                       number_of_rows,
                               LAB_Channel_Data_Oscilloscope& chan)
{
  double row_vpd = ((-1 * chan.voltage_per_division) * 
    (row - (static_cast<double>(number_of_rows) / 2.0)) - 
    (chan.vertical_offset));

  if (is_approx_zero (row_vpd))
  {
    row_vpd = 0.0;
  }

  return (row_vpd);
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
update_gui_time_per_division ()
{
  double col_half = (LABC::OSC::DISPLAY_NUMBER_OF_COLUMNS / 2.0) * -1;

  for (int a = 0; a < m_x_labels.size (); a++)
  {
    double col_tpd = (a + col_half) * (m_parent_data->time_per_division) + 
      (m_parent_data->horizontal_offset);
    
    LabelValue lv (col_tpd, LabelValue::UNIT::SECOND);
    
    m_x_labels[a]->copy_label (lv.to_label_text ().c_str ());
  }
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
update_gui_upper_left_info ()
{
  LabelValue lv (m_parent_data->sampling_rate);

  std::stringstream ss;

  ss  << m_parent_data->samples 
      << " samples at "
      << lv.to_label_text ();

  m_upper_info_display->copy_label (ss.str ().c_str ());
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
update_all_display_information ()
{
  update_gui_voltage_per_division  ();
  update_gui_time_per_division     ();
  update_gui_upper_left_info          ();
}

bool LABSoft_GUI_Oscilloscope_Display_Group::   
is_approx_zero (double x, double epsilon)
{
  return (std::abs (x) < epsilon);
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
update_display_status ()
{
  switch (m_parent_data->status)
  {
    case (LABE::OSC::STATUS::READY):
    {
      m_display_status->label ("Ready");
      m_display_status->color (static_cast<uint32_t>(LABE::DISPLAY::COLOR::RED));

      break;
    }

    case (LABE::OSC::STATUS::STOP):
    {
      m_display_status->label ("Stop");
      m_display_status->color (static_cast<uint32_t>(LABE::DISPLAY::COLOR::RED));

      break;
    }

    case (LABE::OSC::STATUS::AUTO):
    {
      m_display_status->label ("Auto");
      m_display_status->color (static_cast<uint32_t>(LABE::DISPLAY::COLOR::GREEN));

      break;
    }

    case (LABE::OSC::STATUS::DONE):
    {
      m_display_status->label ("Done");
      m_display_status->color (static_cast<uint32_t>(LABE::DISPLAY::COLOR::RED));
    }
  }
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
channel_selector (unsigned channel)
{
  m_channel_selected = channel;

  for (int a = 0; a < m_channel_selectors.size (); a++)
  {
    if (a == channel)
    {
      m_channel_selectors[a]->color       (LABC::DISPLAY::CHAN_COLORS[a]);
      m_channel_selectors[a]->labelcolor  (0);
      m_channel_selectors[a]->set         ();
    }
    else 
    {
      m_channel_selectors[a]->color       (0);
      m_channel_selectors[a]->labelcolor  (LABC::DISPLAY::CHAN_COLORS[a]);
      m_channel_selectors[a]->clear       ();
    }
  }

  update_gui_vertical_offset ();
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
update_gui_vertical_offset ()
{
  m_vertical_offset->selection_color (LABC::DISPLAY::CHAN_COLORS[m_channel_selected]);

  if (m_parent_data->has_enabled_channels ())
  {
    m_vertical_offset->show ();
  }
  else 
  {
    m_vertical_offset->hide ();
  }
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
update_gui_voltage_per_division (unsigned channel)
{
  if (m_parent_data->channel_data[channel].is_enabled)
  {
    LAB_Channel_Data_Oscilloscope& chan = m_parent_data->channel_data[channel];

    for (int a = 0; a < m_y_labels[channel].size (); a++)
    {
      double row_vpd = calc_row_voltage_per_division (a,
        LABC::OSC::DISPLAY_NUMBER_OF_ROWS, chan);
      
      LabelValue lv (row_vpd, LabelValue::UNIT::VOLT);

      if (a == 0)
      {
        LabelValue unit_ref = lv; 

        std::stringstream ss;
        ss << unit_ref.unit_prefix () << unit_ref.unit_string ();

        m_y_label_units[channel]->copy_label (ss.str ().c_str ());
        m_y_label_units[channel]->show ();
      }

      m_y_labels[channel][a]->copy_label (lv.to_label_text ().c_str ());
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

void LABSoft_GUI_Oscilloscope_Display_Group:: 
update_gui_voltage_per_division ()
{
  for (int chan = 0; chan < m_parent_data->channel_data.size (); chan++)
  {
    update_gui_voltage_per_division (chan);
  }
}

void LABSoft_GUI_Oscilloscope_Display_Group:: 
update_gui_vertical_elements ()
{
  update_gui_vertical_offset      ();
  update_gui_voltage_per_division ();
}

// Setter
void LABSoft_GUI_Oscilloscope_Display_Group:: 
display (LABSoft_GUI_Oscilloscope_Display* _LABSoft_GUI_Oscilloscope_Display)
{
  m_display = _LABSoft_GUI_Oscilloscope_Display;
}

// EOF