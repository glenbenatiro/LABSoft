#include "LABSoft_GUI_Oscilloscope_Display.h"

#include <cmath>
#include <cstdio>
#include <sstream>

#include <FL/Fl.H>
#include <FL/Enumerations.H>

#include "../Utility/LAB_Constants.h"

#include "../Utility/LABSoft_GUI_Label.h"
#include "../Utility/LAB_Utility_Functions.h"
#include "../LABSoft_Controller/LABSoft_Controller.h"

// ========== LABSoft_GUI_Oscilloscope_Display_Internal ==========

LABSoft_GUI_Oscilloscope_Display_Internal:: 
LABSoft_GUI_Oscilloscope_Display_Internal (int X, int Y, int W, int H, const char* label)
  : Fl_Widget (X, Y, W, H, label)
{
  calc_cached_drawing_values  ();
  reserve_pixel_points        ();
}

LABSoft_GUI_Oscilloscope_Display_Internal:: 
~LABSoft_GUI_Oscilloscope_Display_Internal ()
{
  
}

int LABSoft_GUI_Oscilloscope_Display_Internal:: 
handle (int event)
{
  switch (event)
  {
    case (FL_ENTER):
    {
      return (1);
    }

    case (FL_MOUSEWHEEL):
    {
      if (Fl::belowmouse () == this)
      {
        int direction = Fl::event_dy ();

        double time_per_division = m_parent_data->time_per_division * 
          ((direction > 0) ? 2.0 : 0.5);

        ((LABSoft_GUI_Oscilloscope_Display*)(parent ()))->controller ().   
          m_Oscilloscope.cb_display_time_per_division (this, static_cast<void*>(&time_per_division));

        return (1);
      }
    }

    case (FL_PUSH):
    {
      m_mouse_down_start_x          = Fl::event_x ();
      m_mouse_down_start_y          = Fl::event_y ();
      m_pre_drag_horizontal_offset  = m_parent_data->horizontal_offset;

      return (1);
    }

    case (FL_DRAG):
    {
      int     scaler                = calc_mouse_drag_time_per_div_scaler (Fl::event_x ());
      double  scaler_time           = scaler * (m_parent_data->time_per_division / 
                                      OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);
      double  new_horizontal_offset = m_pre_drag_horizontal_offset + scaler_time;

      if (std::abs (new_horizontal_offset) < 1e-12)
      {
        new_horizontal_offset = 0.0;
      }

      ((LABSoft_GUI_Oscilloscope_Display*)(parent ()))->controller ().
        m_Oscilloscope.cb_display_horizontal_offset (this, static_cast<void*>(&new_horizontal_offset));

      return (1);
    }
  }
}
void LABSoft_GUI_Oscilloscope_Display_Internal:: 
draw ()
{
  draw_box      (FL_FLAT_BOX, m_background_color);
  draw_grid     ();
  draw_channels ();
}

void LABSoft_GUI_Oscilloscope_Display_Internal:: 
draw_grid ()
{
  // 1. set line color 
  fl_color (m_grid_color);

  // 2. draw rows
  for (int row = 0; row < (LABC::OSC_DISPLAY::NUMBER_OF_ROWS - 1); row++)
  {
    (row == LABC::OSC_DISPLAY::MID_ROW_INDEX) ? fl_line_style (FL_DASH) : 
                                                fl_line_style (FL_DOT);

    int Y = std::round ((row + 1) * m_row_height) + y ();

    fl_line (x (), Y, x () + w (), Y);
  }

  // 3. draw columns
  for (int col = 0; col < (LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS - 1); col++)
  {
    (col == LABC::OSC_DISPLAY::MID_COLUMN_INDEX) ?  fl_line_style (FL_DASH) : 
                                                    fl_line_style (FL_DOT);

    int X = std::round ((col + 1) * m_column_width) + x ();

    fl_line (X, y (), X, y () + h ());
  }

  // 4. draw upper and lower x-axis ticks
  fl_line_style (FL_SOLID, 0, NULL);
 
  for (int a = 0; a < 2; a++)
  {
    for (int X = x (); X < (x () + w ()); X += m_column_width)
    {
      for (int i = 0; i < LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS; i++)
      {
        int length = (i == LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS_HALF) ?
          LABC::OSC_DISPLAY::X_AXIS_SEMI_MAJOR_TICK_LENGTH :
          LABC::OSC_DISPLAY::X_AXIS_MINOR_TICK_LENGTH;
        
        int x = X + (i * m_x_axis_minor_ticks_width);

        (a == 0) ?  fl_line (x, y (), x, y () + length) : 
                    fl_line (x, y () + h () - length, x, y () + h ());
      }

      int length = LABC::OSC_DISPLAY::X_AXIS_MAJOR_TICK_LENGTH;

      (a == 0) ?  fl_line (X, y (), X, y () + length) : 
                  fl_line (X, y () + h () - length, X, y () + h ());
    }
  }

  // 5. draw left and right y-axis ticks
  fl_line_style (FL_SOLID, 0, NULL);
 
  for (int a = 0; a < 2; a++)
  {
    for (int Y = y (); Y < (y () + h ()); Y += m_row_height)
    {
      for (int i = 0; i < LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS; i++)
      {
        int length = (i == LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS_HALF) ?
          LABC::OSC_DISPLAY::Y_AXIS_SEMI_MAJOR_TICK_LENGTH :
          LABC::OSC_DISPLAY::Y_AXIS_MINOR_TICK_LENGTH;
        
        int y = Y + (i * m_y_axis_minor_ticks_height);

        (a == 0) ?  fl_line (x (), y, x () + length, y) :
                    fl_line (x () + w () - length, y, x () + w (), y);
      }

      int length = LABC::OSC_DISPLAY::Y_AXIS_MAJOR_TICK_LENGTH;

      (a == 0) ?  fl_line (x (), Y, x () + length, Y) :
                  fl_line (x () + w () - length, Y, x () + w (), Y);
    }
  }

  // 6. Draw border box
  draw_box (FL_BORDER_FRAME, m_grid_color);

  // 7. Reset line color and style
  fl_color      (0);
  fl_line_style (FL_SOLID, 0, NULL);
}

void LABSoft_GUI_Oscilloscope_Display_Internal:: 
draw_channels ()
{
  if (m_parent_data == nullptr)
  {
    return;
  }

  if (!m_parent_data->has_enabled_channels ())
  {
    return;
  }

  // ----------

  fl_push_clip  (x (), y (), w (), h ());
  
  const LAB_Parent_Data_Oscilloscope &pdata = *m_parent_data;

  unsigned samp_count = (pdata.samples < w () ? pdata.samples : w ()) - 1;

  for (unsigned chan = 0; chan < (pdata.channel_data.size ()); chan++)
  {
    if (pdata.channel_data[chan].is_enabled)
    {
      std::vector<std::array<int, 2>> &pp = m_pixel_points[chan];

      // Because of how line styles are implemented on Win32 systems, 
      // you must set the line style after setting the drawing color. 
      // If you set the color after the style, style settings will be lost.
      fl_color      (OSC_DISPLAY::CHANNEL_COLORS[chan]);
      fl_line_style (FL_SOLID, 0, 0);

      if (pdata.samples <= OSC_DISPLAY::SAMPLE_MARKING_THRESHOLD)
      {
        for (unsigned i = 0; i < samp_count; i++)
        {
          fl_line             (pp[i][0], pp[i][1], pp[i + 1][0], pp[i + 1][1]);
          draw_sample_marker  (pp[i][0], pp[i][1]);
        }
      }
      else 
      {
        for (unsigned i = 0; i < samp_count; i++)
        {
          fl_line (pp[i][0], pp[i][1], pp[i + 1][0], pp[i + 1][1]);
        }
      }
    }
  }

  fl_color      (0);
  fl_line_style (0);
  fl_pop_clip   ();
}

void LABSoft_GUI_Oscilloscope_Display_Internal:: 
draw_sample_marker (int x, 
                    int y)
{
  // Vertical
  fl_line (
    x,
    y + OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
    x,
    y - OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE
  );

  // Horizontal
  fl_line (
    x + OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
    y,
    x - OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
    y
  );
}

void LABSoft_GUI_Oscilloscope_Display_Internal:: 
calc_cached_drawing_values ()
{
  m_column_width              = static_cast<float>(w ()) / OSC_DISPLAY::NUMBER_OF_COLUMNS;
  m_row_height                = static_cast<float>(h ()) / OSC_DISPLAY::NUMBER_OF_ROWS;
  m_x_axis_minor_ticks_width  = m_column_width / OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;
  m_y_axis_minor_ticks_height = m_row_height / OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;
  m_display_half_height       = static_cast<float>(h ()) / 2.0;
  m_display_height_midline    = std::round (y () + m_display_half_height);
}

int LABSoft_GUI_Oscilloscope_Display_Internal:: 
calc_samp_y_coord (double sample, 
                   double y_scaler)
{
  return (std::round (m_display_height_midline - (sample * y_scaler)));
}

double LABSoft_GUI_Oscilloscope_Display_Internal:: 
calc_y_scaler (double voltage_per_division)
{
  double y_scaler = m_display_half_height / 
    (LABC::OSC_DISPLAY::NUMBER_OF_ROWS_HALF * voltage_per_division);
  
  return (y_scaler);
}

double LABSoft_GUI_Oscilloscope_Display_Internal:: 
calc_x_offset (unsigned channel)
{
  double curr_off = m_parent_data->horizontal_offset;
  double curr_tpd = m_parent_data->time_per_division;

  if (curr_off == 0.0)
  { 
    return (0.0);
  }
  else 
  {
    double offset_pix = (-1.0 * m_column_width * (curr_off / curr_tpd));

    return (offset_pix);
  }
}

void LABSoft_GUI_Oscilloscope_Display_Internal:: 
load_parent_data (const LAB_Parent_Data_Oscilloscope& parent_data)
{
  m_parent_data = &parent_data;
}

void LABSoft_GUI_Oscilloscope_Display_Internal:: 
fill_pixel_points_backend_running ()
{
  const LAB_Parent_Data_Oscilloscope& pdata = *m_parent_data;

  for (unsigned chan = 0; chan < pdata.channel_data.size (); chan++)
  {
    if (pdata.channel_data[chan].is_enabled)
    {
      const LAB_Channel_Data_Oscilloscope&    cdata = pdata.channel_data[chan];
            std::vector<std::array<int, 2>>&  pp    = m_pixel_points[chan];

      double y_scaler = calc_y_scaler (cdata.voltage_per_division);
      double x_offset = calc_x_offset (chan);
      
      if (pdata.samples >= w ())
      {
        double samp_skipper = (pdata.samples - 1) / static_cast<double>(w ());

        for (unsigned a = 0; a <= w (); a++)
        {
          double sample = cdata.samples[std::round (a * samp_skipper)] + cdata.vertical_offset;

          pp[a][0] = std::round         (x () + a + x_offset);
          pp[a][1] = calc_samp_y_coord  (sample, y_scaler);
        }
      }
      else 
      {
        double x_skipper = static_cast<double>(w ()) / (pdata.samples - 1);

        for (unsigned a = 0; a < pdata.samples; a++)
        {
          double sample = cdata.samples[a] + cdata.vertical_offset;

          pp[a][0] = std::round         (x () + (a * x_skipper) + x_offset);
          pp[a][1] = calc_samp_y_coord  (sample, y_scaler);
        }
      }
    }
  }
}

void LABSoft_GUI_Oscilloscope_Display_Internal:: 
fill_pixel_points_backend_stopped ()
{

}

void LABSoft_GUI_Oscilloscope_Display_Internal:: 
reserve_pixel_points ()
{
  for (std::vector<std::array<int, 2>>& pp : m_pixel_points)
  {
    pp.reserve (w ());
  }
}

int LABSoft_GUI_Oscilloscope_Display_Internal:: 
calc_mouse_drag_time_per_div_scaler (int drag_x)
{
  return ((m_mouse_down_start_x - drag_x) / m_x_axis_minor_ticks_width);
}

void LABSoft_GUI_Oscilloscope_Display_Internal:: 
fill_pixel_points ()
{
  if (m_parent_data->is_backend_running)
  {
    fill_pixel_points_backend_running ();
  }
  else 
  {
    fill_pixel_points_backend_stopped ();
  }
}

// ========== LABSoft_GUI_Oscilloscope_Display ==========

LABSoft_GUI_Oscilloscope_Display:: 
LABSoft_GUI_Oscilloscope_Display (int X, int Y, int W, int H, const char* label)
  : Fl_Group (X, Y, W, H, label)
{
  init_child_widgets  ();
}

LABSoft_GUI_Oscilloscope_Display:: 
~LABSoft_GUI_Oscilloscope_Display ()
{

}

void LABSoft_GUI_Oscilloscope_Display:: 
draw ()
{
  if (m_parent_data->is_backend_running)
  {
    update_gui_top_info ();
  }

  update_gui_status ();

  draw_box      (FL_THIN_DOWN_BOX, OSC_DISPLAY::BACKGROUND_COLOR);
  draw_children ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets ()
{
  // ! The order of initialization here is important!
  // ! The order of initialization here is important!
  // ! The order of initialization here is important!

  // 1. internal display
  //    the positions of all widgets in the display are based
  //    nn the position of this internal display
  m_display_internal = new LABSoft_GUI_Oscilloscope_Display_Internal (x () + 80, 
    y () + 20, 610, 470); 

  // 2. fl_slider widgets
  init_child_widgets_sliders ();
  
  // 3. display status
  init_child_widgets_status ();

  // 4. axis labels
  init_child_widgets_voltage_per_division_labels ();
  init_child_widgets_time_per_division_labels ();

  // 5. channel selectors
  init_child_widgets_channel_selectors ();

  // 6. top info
  init_child_widgets_top_info ();
                      
  end ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_sliders ()
{
  const LABSoft_GUI_Oscilloscope_Display_Internal& disp = *m_display_internal;
   
  // 2.1
  m_vertical_offset_slider = new LABSoft_GUI_Fl_Slider (disp.x (), disp.y (), OSC_DISPLAY::SLIDER_WIDTH, disp.h ());
  m_vertical_offset_slider->box    (FL_NO_BOX);
  m_vertical_offset_slider->color  (3);
  m_vertical_offset_slider->bounds (0, disp.h ());
  m_vertical_offset_slider->value  (disp. h () / 2.0);
  m_vertical_offset_slider->step   (disp.h (), OSC_DISPLAY::NUMBER_OF_ROWS * OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);

  // 2.2 trigger level
  m_trigger_level_slider = new LABSoft_GUI_Fl_Slider (disp.x () + disp.w () - OSC_DISPLAY::SLIDER_WIDTH, 
    disp.y (), OSC_DISPLAY::SLIDER_WIDTH, disp.h ());
  m_trigger_level_slider->box       (FL_NO_BOX);
  m_trigger_level_slider->color     (3);
  m_trigger_level_slider->bounds    (0, disp.h ());
  m_trigger_level_slider->value     (disp.h () / 2.0);
  m_trigger_level_slider->step      (disp.h (), OSC_DISPLAY::NUMBER_OF_ROWS * OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);
  m_trigger_level_slider->callback  (cb_trigger_level_static);  

  // 2.3 horizontal offset
  m_horizontal_offset_slider = new LABSoft_GUI_Fl_Slider (disp.x (), disp.y (), disp.w (), OSC_DISPLAY::SLIDER_WIDTH);
  m_horizontal_offset_slider->type   (1); // horizontal slider
  m_horizontal_offset_slider->box    (FL_NO_BOX);
  m_horizontal_offset_slider->color  (3);
  m_horizontal_offset_slider->bounds (0, disp.w ());
  m_horizontal_offset_slider->value  (disp. w () / 2.0);
  m_horizontal_offset_slider->step   (disp.w (), OSC_DISPLAY::NUMBER_OF_COLUMNS *
                                OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);

  // remove for now para di mag conflict sa v offset ug trig level sliders
  m_horizontal_offset_slider->deactivate ();
  m_horizontal_offset_slider->hide ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_status ()
{
  m_status = new Fl_Box (
    m_display_internal->x (), 
    m_display_internal->y () - OSC_DISPLAY::STATUS_HEIGHT, 
    80, 
    OSC_DISPLAY::STATUS_HEIGHT, 
    "Ready"
  );

  m_status->box         (FL_BORDER_FRAME);
  m_status->color       (1);
  m_status->labelfont   (1);
  m_status->labelsize   (OSC_DISPLAY::AXIS_LABEL_SIZE);
  m_status->labelcolor  (FL_WHITE);
}

void LABSoft_GUI_Oscilloscope_Display::
init_child_widgets_time_per_division_labels ()
{
  float col_width = static_cast<float>(m_display_internal->w ()) / 
    OSC_DISPLAY::NUMBER_OF_COLUMNS;
  
  for (int a = 0; a < m_time_per_division_labels.size (); a++)
  {
    Fl_Box* box = new Fl_Box (
      m_display_internal->x () + (a * col_width), 
      m_display_internal->y () + m_display_internal->h (), 
      3, 
      20
    );
    
    box->labelcolor (OSC_DISPLAY::X_AXIS_LABEL_COLOR);
    box->labelsize  (9);

    m_time_per_division_labels[a] = box;
  }

  m_time_per_division_labels.back ()->align (FL_ALIGN_LEFT);
}

void LABSoft_GUI_Oscilloscope_Display::
init_child_widgets_voltage_per_division_labels ()
{
  float row_height = static_cast<float>(m_display_internal->h ()) / 
    OSC_DISPLAY::NUMBER_OF_ROWS;

  for (int chan = 0; chan < m_voltage_per_division_labels.size (); chan++)
  {
    unsigned X = m_display_internal->x () - (OSC_DISPLAY::Y_AXIS_LABEL_HORIZONTAL_INTRASPACE * 
      m_voltage_per_division_labels.size ()) + (OSC_DISPLAY::Y_AXIS_LABEL_HORIZONTAL_INTRASPACE * 
      chan);
    
    for (int row = 0; row < m_voltage_per_division_labels[chan].size (); row++)
    {
      unsigned Y = std::round (m_display_internal->y () + (row * row_height));

      Fl_Box* box = new Fl_Box (X, Y, 3, 20, "0");

      box->labelcolor (OSC_DISPLAY::CHANNEL_COLORS[chan]);
      box->labelsize  (9);
      box->align      (FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE);

      m_voltage_per_division_labels[chan][row] = box;
    }

    m_voltage_per_division_labels[chan].back ()->align (FL_ALIGN_TOP_LEFT);

    // create the voltage per division unit display
    char label[20];
    std::snprintf (label, sizeof (label), "C%d mV", chan + 1);

    Fl_Box* box = new Fl_Box (X, m_display_internal->y (), 3, 20, "C0 mV");

    box->copy_label (label);
    box->labelcolor (OSC_DISPLAY::CHANNEL_COLORS[chan]);
    box->labelsize  (9);
    box->align      (FL_ALIGN_TOP_LEFT);

    m_voltage_per_division_units[chan] = box;
  }
}

void LABSoft_GUI_Oscilloscope_Display::
init_child_widgets_channel_selectors ()
{
  for (int a = 0; a < m_channel_selectors.size (); a++)
  {
    Fl_Button* box = new Fl_Button (
      m_status->x () + m_status->w () + 
        (a * OSC_DISPLAY::STATUS_HEIGHT),
      m_status->y (), 
      OSC_DISPLAY::STATUS_HEIGHT, 
      OSC_DISPLAY::STATUS_HEIGHT
    );

    char label[20];
    std::snprintf (label, sizeof (label), "C%d", a + 1);
    
    box->box              (FL_THIN_UP_BOX);
    box->color            (FL_BLACK);
    box->selection_color  (FL_BLACK);
    box->labelsize        (OSC_DISPLAY::AXIS_LABEL_SIZE);
    box->labelcolor       (OSC_DISPLAY::CHANNEL_COLORS[a]);
    box->copy_label       (label);
    box->user_data        (reinterpret_cast<void*>(a));
    box->callback         (cb_channel_selector_static);

    m_channel_selectors[a] = box;
  }
}

void LABSoft_GUI_Oscilloscope_Display::
init_child_widgets_top_info ()
{
  m_top_info = new Fl_Box (
    m_status->x () + m_status->w () + 10 +
      (OSC_DISPLAY::STATUS_HEIGHT * m_channel_selectors.size ()), 
    m_status->y (), 
    3, 
    m_status->h ()
  );

  m_top_info->box         (FL_NO_BOX);
  m_top_info->align       (FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  m_top_info->labelsize   (OSC_DISPLAY::AXIS_LABEL_SIZE);
  m_top_info->labelcolor  (FL_WHITE);
}

double LABSoft_GUI_Oscilloscope_Display::
calc_row_voltage_per_division (       unsigned                       row,
                                      unsigned                       number_of_rows,
                               const  LAB_Channel_Data_Oscilloscope& cdata)
{
  double row_vpd = (-1 * cdata.voltage_per_division) *
    (row - (static_cast<double>(number_of_rows) / 2.0)) - cdata.vertical_offset;

  // is approx. equal to zero
  if (std::abs (row_vpd) < 1e-12)
  {
    row_vpd = 0.0;
  }

  return (row_vpd);
}

void LABSoft_GUI_Oscilloscope_Display:: 
cb_trigger_level_static (Fl_Widget* w, 
                         void*      data)
{
  (static_cast<LABSoft_GUI_Oscilloscope_Display*>(w->parent ()))->controller ().
    m_Oscilloscope.cb_trigger_level_slider (static_cast<LABSoft_GUI_Fl_Slider*>(w), data);
}

void LABSoft_GUI_Oscilloscope_Display:: 
cb_channel_selector_static (Fl_Widget*  w,
                            void*       data)
{
  LABSoft_GUI_Oscilloscope_Display& display = 
    *(static_cast<LABSoft_GUI_Oscilloscope_Display*>(w->parent ()));

  unsigned channel = reinterpret_cast<int>(data);

  display.select_channel (channel);
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_status ()
{
  switch (m_parent_data->status)
  {
    case (LABE::OSC::STATUS::READY):
    {
      m_status->copy_label ("Ready");
      m_status->color (1);

      break;
    }

    case (LABE::OSC::STATUS::STOP):
    {
      m_status->copy_label ("Stop");
      m_status->color (1);

      break;
    }

    case (LABE::OSC::STATUS::AUTO):
    {
      m_status->copy_label ("Auto");
      m_status->color (2);

      break;
    }

    case (LABE::OSC::STATUS::DONE):
    {
      m_status->copy_label ("Done");
      m_status->color (1);
    }

    case (LABE::OSC::STATUS::CONFIG):
    {
      m_status->copy_label ("Config");
      m_status->color (3);
    }
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
load_parent_data (const LAB_Parent_Data_Oscilloscope& pdata)
{
  m_parent_data = &pdata;

  m_display_internal->load_parent_data (pdata);

  update_gui_vertical_elements    ();
  update_gui_horizontal_elements  ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
load_controller (LABSoft_Controller& controller)
{
  m_LABSoft_Controller = &controller;
}

void LABSoft_GUI_Oscilloscope_Display::
update_display ()
{
  m_display_internal->fill_pixel_points ();
  
  redraw ();
}

void LABSoft_GUI_Oscilloscope_Display::
update_gui_voltage_per_division (unsigned channel)
{
  std::array<std::array<Fl_Box*, OSC_DISPLAY::NUMBER_OF_ROWS + 1>,
    LABC::OSC::NUMBER_OF_CHANNELS>& labels = m_voltage_per_division_labels;

  if (m_parent_data->channel_data[channel].is_enabled)
  {
    const LAB_Channel_Data_Oscilloscope& cdata = m_parent_data->channel_data[channel];
    
    for (int a = 0; a < labels[channel].size (); a++)
    {
      double row_vpd = calc_row_voltage_per_division (a,
        OSC_DISPLAY::NUMBER_OF_ROWS, cdata);
      
      LABSoft_GUI_Label lbl (row_vpd);

      char w_label[20];
      std::snprintf (w_label, sizeof (w_label), "%.3f", lbl.actual_value ());

      labels[channel][a]->copy_label (w_label);
      labels[channel][a]->show ();

      if (a == 0)
      {
        std::snprintf (w_label, sizeof (w_label), "C%d %sV", 
          channel + 1, lbl.unit_prefix ().c_str ());

        m_voltage_per_division_units[channel]->copy_label (w_label);
        m_voltage_per_division_units[channel]->show ();
      }
    }    
  }
  else 
  {
    m_voltage_per_division_units[channel]->hide ();

    for (int row = 0; row < labels[channel].size (); row++)
    {
      labels[channel][row]->hide ();
    }
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_voltage_per_division ()
{
  for (int chan = 0; chan < m_parent_data->channel_data.size (); chan++)
  {
    update_gui_voltage_per_division (chan);
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_time_per_division ()
{
  double col_half = (OSC_DISPLAY::NUMBER_OF_COLUMNS / 2.0) * -1;

  for (int a = 0; a < m_time_per_division_labels.size (); a++)
  {
    double col_tpd = (a + col_half) * (m_parent_data->time_per_division) + 
      (m_parent_data->horizontal_offset);
    
    LABSoft_GUI_Label lbl (col_tpd, LABSoft_GUI_Label::UNIT::SECOND);
    
    m_time_per_division_labels[a]->copy_label (lbl.to_label_text ().c_str ());
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_top_info ()
{
  LABSoft_GUI_Label lbl (m_parent_data->sampling_rate, LABSoft_GUI_Label::UNIT::HERTZ);

  std::stringstream ss;

  ss  << m_parent_data->samples 
      << " samples at "
      << lbl.to_label_text ()
      << " | "
      << LABF::get_now_timestamp ();

  m_top_info->copy_label (ss.str ().c_str ());
}

void LABSoft_GUI_Oscilloscope_Display:: 
select_channel (unsigned channel)
{
  m_selected_channel = channel;

  for (int a = 0; a < m_channel_selectors.size (); a++)
  {
    (a == channel) ? m_channel_selectors[a]->set () : m_channel_selectors[a]->clear ();
  }

  update_gui_vertical_offset_slider ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_vertical_elements ()
{
  update_gui_voltage_per_division   ();
  update_gui_trigger_level_slider   ();
  update_gui_vertical_offset_slider ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_horizontal_elements ()
{
  update_gui_time_per_division ();
}

void LABSoft_GUI_Oscilloscope_Display::
update_gui_trigger_level_slider ()
{
  const LAB_Parent_Data_Oscilloscope& pdata = *m_parent_data;

  // 1. update color
  m_trigger_level_slider->selection_color (OSC_DISPLAY::CHANNEL_COLORS[pdata.trigger_source]);

  // 2. update value
  const LAB_Channel_Data_Oscilloscope& cdata = pdata.channel_data[pdata.trigger_source];

  double max_tpd_raw  = cdata.voltage_per_division * (OSC_DISPLAY::NUMBER_OF_ROWS_HALF);
  double min_tpd_raw  = -1 * max_tpd_raw;
  double max_bounds   = max_tpd_raw - cdata.vertical_offset;
  double min_bounds   = min_tpd_raw - cdata.vertical_offset;  

  // if fl_slider is vertical, minimum is above, max is below
  m_trigger_level_slider->bounds (max_bounds, min_bounds); 
  m_trigger_level_slider->value  (pdata.trigger_level);
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_vertical_offset_slider ()
{
  if (m_parent_data->has_enabled_channels ())
  {
    m_vertical_offset_slider->show ();
    m_vertical_offset_slider->selection_color (OSC_DISPLAY::CHANNEL_COLORS[m_selected_channel]);

    const LAB_Parent_Data_Oscilloscope&   pdata = *m_parent_data;
    const LAB_Channel_Data_Oscilloscope&  cdata = pdata.channel_data[m_selected_channel];

    double max_tpd_raw  = cdata.voltage_per_division * (OSC_DISPLAY::NUMBER_OF_ROWS_HALF);
    double min_tpd_raw  = -1 * max_tpd_raw;
    double max_bounds   = max_tpd_raw - cdata.vertical_offset;
    double min_bounds   = min_tpd_raw - cdata.vertical_offset;   

    // if fl_slider is vertical, minimum is above, max is below
    m_vertical_offset_slider->bounds (max_bounds, min_bounds);
    m_vertical_offset_slider->value  (0);
  }
  else 
  {
    m_vertical_offset_slider->hide ();
  }
}

LABSoft_Controller& LABSoft_GUI_Oscilloscope_Display:: 
controller () const
{
  return (*m_LABSoft_Controller);
}

unsigned LABSoft_GUI_Oscilloscope_Display:: 
selected_channel () const
{
  return (m_selected_channel);
}

// EOF 