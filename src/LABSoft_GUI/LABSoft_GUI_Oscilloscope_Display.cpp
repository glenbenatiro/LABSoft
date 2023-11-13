#include "LABSoft_GUI_Oscilloscope_Display.h"

#include <cmath>
#include <chrono>
#include <iomanip>
#include <sstream>

// delete soon
#include <iostream>

#include <FL/Enumerations.H>

#include "../Utility/LABSoft_GUI_Label.h"
#include "../Utility/LAB_Utility_Functions.h"
#include "../LABSoft_Presenter/LABSoft_Controller.h"

LABSoft_GUI_Oscilloscope_Display::Internal_Display::
Internal_Display (int         X,
                  int         Y,
                  int         W,
                  int         H,
                  const char* label)
  : Fl_Widget (X, Y, W, H, label)
{
  reserve_pixel_points    ();
  calc_cached_values_all  ();
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
draw ()
{
  draw_box  (FL_FLAT_BOX, LABC::OSC_DISPLAY::BACKGROUND_COLOR);
  draw_grid ();

  if (m_osc)
  {
    calc_pixel_points ();
    draw_channels     ();
  }
}

int LABSoft_GUI_Oscilloscope_Display::Internal_Display::
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
        cb_mouse_wheel (Fl::event_dy ());

        return (1);
      }
      else 
      {
        return (0);
      }
    }

    case (FL_PUSH):
    {
      cb_mouse_click (Fl::event_x (), Fl::event_y ());

      return (1);
    }

    case (FL_DRAG):
    {
      cb_mouse_drag (Fl::event_x ());

      return (1);
    }

    default:
    {
      return (0);
    }
  }
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
resize (int x,
        int y,
        int w,
        int h)
{
  Fl_Widget::resize (x, y, w, h);

  reserve_pixel_points ();
  calc_cached_values_all ();
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
draw_grid ()
{
  // 1. set line color
  fl_color (LABC::OSC_DISPLAY::GRID_COLOR);

  // 2. draw rows
  for (unsigned row = 0; row < (LABC::OSC_DISPLAY::NUMBER_OF_ROWS - 1); row++)
  {
    (row == LABC::OSC_DISPLAY::MID_ROW_INDEX) ? fl_line_style (FL_DASH) : 
                                                fl_line_style (FL_DOT);

    int Y = std::round ((row + 1) * m_row_height) + y ();

    fl_line (x (), Y, x () + w (), Y);
  }

  // 3. draw columns
  for (unsigned col = 0; col < (LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS - 1); col++)
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
        
        int y = Y + (i * m_y_axis_minor_ticks_width);

        (a == 0) ?  fl_line (x (), y, x () + length, y) :
                    fl_line (x () + w () - length, y, x () + w (), y);
      }

      int length = LABC::OSC_DISPLAY::Y_AXIS_MAJOR_TICK_LENGTH;

      (a == 0) ?  fl_line (x (), Y, x () + length, Y) :
                  fl_line (x () + w () - length, Y, x () + w (), Y);
    }
  }

  // 6. Draw border box
  draw_box (FL_BORDER_FRAME, LABC::OSC_DISPLAY::GRID_COLOR);

  // 7. Reset line color and style
  fl_color      (0);
  fl_line_style (FL_SOLID, 0, NULL);
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
draw_channels ()
{
  fl_push_clip (x (), y (), w (), h ());

  for (unsigned chan = 0; chan < m_pixel_points.size (); chan++)
  {
    if (m_osc->is_channel_enabled (chan))
    {
      std::vector<std::array<int, 2>>& pp = m_pixel_points[chan];

      fl_color (LABC::OSC_DISPLAY::CHANNEL_COLORS[chan]);

      if (m_mark_samples)
      {
        for (unsigned i = 0; i < (m_draw_window_width - 1); i++)
        {
          fl_line             (pp[i][0], pp[i][1], pp[i + 1][0], pp[i + 1][1]);
          draw_sample_marker  (pp[i][0], pp[i][1]);
        }
      }
      else 
      {
        for (unsigned i = 0; i < (m_draw_window_width - 1); i++)
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

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
draw_sample_marker (int x,
                    int y)
{
  // vertical
  fl_line (
    x,
    y + LABC::OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
    x,
    y - LABC::OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE
  );

  // horizontal
  fl_line (
    x + LABC::OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
    y,
    x - LABC::OSC_DISPLAY::SAMPLE_MARKING_AMPLITUDE,
    y
  );
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
calc_pixel_points ()
{
  for (unsigned chan = 0; chan < m_osc->channels (); chan++)
  {
    if (m_osc->is_channel_enabled (chan))
    {
      std::vector<std::array<int, 2>>& pp = m_pixel_points[chan];
      
      if (m_samples_to_display >= m_draw_window_width)
      {
        for (unsigned i = 0; i < m_draw_window_width; i++)
        {
          double sample = m_osc->chan_samples (chan)[std::round (m_samples_start_index
            + (i * m_samp_skipper))] + m_osc->vertical_offset (chan);

          pp[i][0] = m_x_coord_draw_start + i;
          pp[i][1] = calc_sample_y_coord (sample, chan);
        }
      }
      else 
      {
        for (unsigned i = 0; i < m_samples_to_display; i++)
        {
          double sample = m_osc->chan_samples (chan)[i] + m_osc->vertical_offset (chan);

          pp[i][0] = std::round (m_x_coord_draw_start + (i * m_x_skipper));
          pp[i][1] = calc_sample_y_coord (sample, chan);
        }
      }
    }
  }
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
calc_cached_values_display ()
{
  m_row_height                = static_cast<double>(h ()) / LABC::OSC_DISPLAY::NUMBER_OF_ROWS;
  m_column_width              = static_cast<double>(w ()) / LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS;

  m_x_axis_minor_ticks_width  = m_column_width / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;
  m_y_axis_minor_ticks_width  = m_row_height / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;

  m_display_half_height       = h () / 2.0;
  m_display_height_midline    = std::round (y () + m_display_half_height);
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
calc_cached_values_drawing ()
{
  if (m_osc)
  {
    m_time_per_division_delta_scaler  = calc_time_per_division_delta_scaler ();
    m_samples_to_display              = calc_samples_to_display             (m_time_per_division_delta_scaler);
    m_draw_window_width               = calc_draw_window_width              (m_time_per_division_delta_scaler);
    m_x_coord_draw_start              = calc_x_coord_draw_start             (m_time_per_division_delta_scaler);
    m_samples_start_index             = calc_samples_start_index            (m_time_per_division_delta_scaler, m_samples_to_display);
    m_samp_skipper                    = calc_samp_skipper                   (m_samples_to_display, m_draw_window_width);
    m_x_skipper                       = calc_x_skipper                      (m_samples_to_display, m_draw_window_width);
    m_mark_samples                    = calc_mark_samples                   (m_time_per_division_delta_scaler, m_samples_to_display);
  }
  else 
  {
    m_time_per_division_delta_scaler  = 0;
    m_samples_to_display              = 0;
    m_draw_window_width         = 0;
    m_x_coord_draw_start              = 0;
    m_samples_start_index             = 0;
    m_samp_skipper                    = 0;
    m_x_skipper                       = 0;
    m_mark_samples                    = 0;
  }

  {
    std::cout << "m_time_per_division_delta_scaler: " << m_time_per_division_delta_scaler << "\n";
    std::cout << "m_samples_to_display: "             << m_samples_to_display << "\n";
    std::cout << "m_draw_window_width: "              << m_draw_window_width << "\n";
    std::cout << "m_x_coord_draw_start: "             << m_x_coord_draw_start << "\n";
    std::cout << "m_samples_start_index: "            << m_samples_start_index << "\n";
    std::cout << "m_samp_skipper: "                   << m_samp_skipper << "\n";
    std::cout << "m_x_skipper: "                      << m_x_skipper << "\n";
    std::cout << "m_mark_samples: "                   << m_mark_samples << "\n\n";
  }
}



double LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
calc_time_per_division_delta_scaler () const
{
  



  return (m_osc->raw_buffer_time_per_division () / m_osc->time_per_division ());                                     
}

unsigned LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
calc_samples_to_display (double time_per_division_delta_scaler) const
{
  if (LABF::is_less_than (time_per_division_delta_scaler, 1.0, LABC::LABSOFT::EPSILON))
  {
    return (m_osc->samples ());
  }
  else 
  {
    // double samples =  m_osc->raw_buffer_sampling_rate () * m_osc->time_per_division () * 
    //                 LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS;

    double samples = m_osc->samples () * (1 / time_per_division_delta_scaler);

    if (LABF::is_less_than (time_per_division_delta_scaler, 1.0, LABC::LABSOFT::EPSILON))
    {
      return (1);
    }
    else 
    {
      return (std::round (samples));
    }
  }
}

unsigned LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
calc_draw_window_width (double time_per_division_delta_scaler) const
{
  if (LABF::is_less_than (time_per_division_delta_scaler, 1.0, LABC::LABSOFT::EPSILON))
  {
    double pixel_points = w () * time_per_division_delta_scaler;

    if (LABF::is_less_than (pixel_points, 1.0, LABC::LABSOFT::EPSILON))
    {
      return (1);
    }
    else 
    {
      return (std::round (pixel_points));
    }
  }
  else 
  {
    return (w ());
  }
}

unsigned LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
calc_x_coord_draw_start (double time_per_division_delta_scaler) const 
{
  if (LABF::is_less_than (time_per_division_delta_scaler, 1.0, LABC::LABSOFT::EPSILON))
  {
    double draw_window_width      = w () * time_per_division_delta_scaler; 
    double free_space_width       = w () - draw_window_width;
    double free_space_width_half  = free_space_width / 2.0;
    double x_coord_draw_start     = x () + free_space_width_half;

    return (std::round (x_coord_draw_start));
  }
  else 
  {
    return (x ());
  }    
}

unsigned LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
calc_samples_start_index (double    time_per_division_delta_scaler,
                          unsigned  samples_to_display) const
{
  if (samples_to_display < m_osc->samples ())
  {
    unsigned  undisplayed_samples       = m_osc->samples () - samples_to_display;
    double    undisplayed_samples_half  = undisplayed_samples / 2.0;

    return (std::round (undisplayed_samples_half)); 
  }
  else 
  {
    return (0);
  }
}

double LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
calc_samp_skipper (unsigned samples_to_display,
                   unsigned draw_window_width) const
{
  return ((samples_to_display - 1.0) / (draw_window_width - 1.0));
}

double LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
calc_x_skipper (unsigned samples_to_display,
                unsigned draw_window_width) const
{
  return ((m_draw_window_width - 1.0) / (samples_to_display - 1.0));
}

bool LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
calc_mark_samples (double   time_per_division_delta_scaler,
                   unsigned samples_to_display) const
{
  return ((samples_to_display <= LABC::OSC_DISPLAY::SAMPLE_MARKING_THRESHOLD) &&
    (LABF::is_greater_than (time_per_division_delta_scaler, 0.5, LABC::LABSOFT::EPSILON)));
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
calc_cached_values_sample_y_scaler ()
{
  if (m_osc)
  {
    for (unsigned chan = 0; chan < m_sample_y_scaler.size (); chan++)
    {
      m_sample_y_scaler[chan] = m_display_half_height / 
        (LABC::OSC_DISPLAY::NUMBER_OF_ROWS_HALF * m_osc->chan_voltage_per_division (chan));
    }
  }
  else 
  {
    for (unsigned chan = 0; chan < m_sample_y_scaler.size (); chan++)
    {
      m_sample_y_scaler[chan] = 0.0;
    }
  }  
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
calc_cached_values_sample_x_offset ()
{
  if (m_osc)
  {
    double horizontal_offset_delta  = m_osc->raw_buffer_horizontal_offset () - 
                                      m_osc->horizontal_offset ();
    
    double time_per_division        = m_osc->time_per_division (); 

    m_sample_x_offset = (-m_column_width * m_time_per_division_delta_scaler) * 
                        (horizontal_offset_delta / time_per_division);
  }
  else 
  {
    m_sample_x_offset = 0;
  }  
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
calc_cached_values_all ()
{
  calc_cached_values_display          ();
  calc_cached_values_sample_y_scaler  ();

  calc_cached_values_drawing          ();
  calc_cached_values_sample_x_offset  ();
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
calc_cached_values_horizontal ()
{
  calc_cached_values_drawing          ();
  calc_cached_values_sample_x_offset  ();
}

int LABSoft_GUI_Oscilloscope_Display::Internal_Display::
calc_sample_y_coord (double   sample, 
                     unsigned channel) const
{
  return (std::round (m_display_height_midline - (sample * m_sample_y_scaler[channel])));
}

int LABSoft_GUI_Oscilloscope_Display::Internal_Display::
calc_mouse_drag_time_per_div_scaler (int drag_x) const
{
  return ((m_mouse_down_start_x - drag_x) / m_x_axis_minor_ticks_width);
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
reserve_pixel_points ()
{
  for (std::vector<std::array<int, 2>>& pp : m_pixel_points)
  {
    pp.reserve (w ());
  }
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
cb_mouse_wheel (int direction)
{
  m_controller->m_Oscilloscope.cb_display_time_per_division (direction);
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
cb_mouse_click (int x, int y)
{
  m_mouse_down_start_x = x;
  m_mouse_down_start_y = y;
  
  m_pre_drag_horizontal_offset = m_osc->horizontal_offset ();
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
cb_mouse_drag (int x)
{
  int     scaler      = calc_mouse_drag_time_per_div_scaler (x);
  double  scaler_time = scaler * (m_osc->time_per_division () / 
                        LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);
  double  horiz_off   = m_pre_drag_horizontal_offset + scaler_time;

  if (LABF::is_equal (horiz_off, 0.0, LABC::LABSOFT::EPSILON))
  {
    horiz_off = 0.0;
  }

  m_controller->m_Oscilloscope.display_cb_horizontal_offset (
    static_cast<void*>(&horiz_off));
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
load_oscilloscope (const LAB_Oscilloscope& oscilloscope)
{
  m_osc = &oscilloscope;

  calc_cached_values_all ();
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
load_controller (const LABSoft_Controller& controller)
{
  m_controller = &controller;
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display::
update_voltage_per_division ()
{
  calc_cached_values_sample_y_scaler ();
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
update_horizontal_offset ()
{
  calc_cached_values_horizontal ();
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
update_time_per_division ()
{
  calc_cached_values_horizontal ();
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
update_samples ()
{
  calc_cached_values_horizontal ();
}

void LABSoft_GUI_Oscilloscope_Display::Internal_Display:: 
update_sampling_rate ()
{
  calc_cached_values_horizontal ();
}
        
// ==========

LABSoft_GUI_Oscilloscope_Display:: 
LABSoft_GUI_Oscilloscope_Display (int         X, 
                                  int         Y, 
                                  int         W, 
                                  int         H, 
                                  const char* label)
  : Fl_Group (X, Y, W, H, label)
{
  init_child_widgets  ();
}

LABSoft_GUI_Oscilloscope_Display:: 
~LABSoft_GUI_Oscilloscope_Display ()
{

}

void LABSoft_GUI_Oscilloscope_Display:: 
load_oscilloscope (const LAB_Oscilloscope& oscilloscope)
{
  m_osc = &oscilloscope;

  m_internal_display->load_oscilloscope (oscilloscope);

  update_gui_vertical_elements    ();
  update_gui_horizontal_elements  ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
load_controller (const LABSoft_Controller& controller)
{
  m_controller = &controller;

  m_internal_display->load_controller (controller);
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets ()
{
  // ! The order of initialization here is important!
  // ! The order of initialization here is important!
  // ! The order of initialization here is important!

  init_child_widgets_internal_display             ();
  init_child_widgets_sliders                      ();  
  init_child_widgets_status                       ();
  init_child_widgets_voltage_per_division_labels  ();
  init_child_widgets_time_per_division_labels     ();
  init_child_widgets_channel_selectors            ();
  init_child_widgets_top_info                     ();

  end ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
draw ()
{
  if (m_osc->is_backend_running ())
  {
    update_gui_top_info ();
  }
  
  update_gui_status ();

  draw_box      (FL_THIN_DOWN_BOX, LABC::OSC_DISPLAY::BACKGROUND_COLOR);
  draw_children ();
}

// update gui functions
void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_status ()
{
  switch (m_osc->status ())
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
update_channel_enable_disable ()
{
  update_gui_vertical_elements ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_vertical_offset ()
{
  update_gui_vertical_elements ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_horizontal_offset ()
{
  m_internal_display->update_horizontal_offset ();

  update_gui_time_per_division ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_time_per_division ()
{
  m_internal_display->update_time_per_division ();

  update_gui_time_per_division ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_samples ()
{
  m_internal_display->update_samples ();

  update_gui_time_per_division ();
}

void LABSoft_GUI_Oscilloscope_Display::
update_sampling_rate ()
{
  m_internal_display->update_sampling_rate ();

  update_gui_time_per_division ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_voltage_per_division ()
{
  m_internal_display->update_voltage_per_division ();

  update_gui_voltage_per_division ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_trigger_source ()
{
  update_gui_trigger_level_slider ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_trigger_level ()
{
  update_gui_trigger_level_slider ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_display ()
{
  redraw ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
select_channel (unsigned channel)
{
  m_selected_channel = channel;

  for (unsigned a = 0; a < m_channel_selectors.size (); a++)
  {
    (a == channel) ?  m_channel_selectors[a]->set   () : 
                      m_channel_selectors[a]->clear ();
  }

  update_gui_vertical_offset_slider ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_internal_display ()
{
  m_internal_display = new Internal_Display (
    x () + LABC::OSC_DISPLAY::INTERNAL_DISPLAY_LEFT_MARGIN,
    y () + LABC::OSC_DISPLAY::INTERNAL_DISPLAY_TOP_MARGIN, 
    w () - LABC::OSC_DISPLAY::INTERNAL_DISPLAY_LEFT_MARGIN - 
      LABC::OSC_DISPLAY::INTERNAL_DISPLAY_RIGHT_MARGIN,
    h () - LABC::OSC_DISPLAY::INTERNAL_DISPLAY_TOP_MARGIN - 
      LABC::OSC_DISPLAY::INTERNAL_DISPLAY_BOTTOM_MARGIN
  );
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_sliders ()
{
  const Internal_Display& disp = *m_internal_display;
   
  // 2.1
  m_vertical_offset_slider = new LABSoft_GUI_Fl_Slider (disp.x (), disp.y (), LABC::OSC_DISPLAY::SLIDER_WIDTH, disp.h ());
  m_vertical_offset_slider->box    (FL_NO_BOX);
  m_vertical_offset_slider->color  (3);
  m_vertical_offset_slider->bounds (0, disp.h ());
  m_vertical_offset_slider->value  (disp. h () / 2.0);
  m_vertical_offset_slider->step   (disp.h (), LABC::OSC_DISPLAY::NUMBER_OF_ROWS * LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);

  // 2.2 trigger level
  m_trigger_level_slider = new LABSoft_GUI_Fl_Slider (disp.x () + disp.w () - LABC::OSC_DISPLAY::SLIDER_WIDTH, 
    disp.y (), LABC::OSC_DISPLAY::SLIDER_WIDTH, disp.h ());
  m_trigger_level_slider->box       (FL_NO_BOX);
  m_trigger_level_slider->color     (3);
  m_trigger_level_slider->bounds    (0, disp.h ());
  m_trigger_level_slider->value     (disp.h () / 2.0);
  m_trigger_level_slider->step      (disp.h (), LABC::OSC_DISPLAY::NUMBER_OF_ROWS * LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);
  m_trigger_level_slider->callback  (cb_trigger_level_static);  

  // 2.3 horizontal offset
  m_horizontal_offset_slider = new LABSoft_GUI_Fl_Slider (disp.x (), disp.y (), disp.w (), LABC::OSC_DISPLAY::SLIDER_WIDTH);
  m_horizontal_offset_slider->type   (1); // horizontal slider
  m_horizontal_offset_slider->box    (FL_NO_BOX);
  m_horizontal_offset_slider->color  (3);
  m_horizontal_offset_slider->bounds (0, disp.w ());
  m_horizontal_offset_slider->value  (disp. w () / 2.0);
  m_horizontal_offset_slider->step   (disp.w (), LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS * LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS);

  // remove for now para di mag conflict sa v offset ug trig level sliders
  m_horizontal_offset_slider->deactivate ();
  m_horizontal_offset_slider->hide ();
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_status ()
{
  m_status = new Fl_Box (
    m_internal_display->x (),
    m_internal_display->y () - LABC::OSC_DISPLAY::STATUS_HEIGHT,
    LABC::OSC_DISPLAY::STATUS_WIDTH,
    LABC::OSC_DISPLAY::STATUS_HEIGHT,
    "Ready"
  );

  m_status->box         (FL_BORDER_FRAME);
  m_status->color       (1);
  m_status->labelfont   (1);
  m_status->labelsize   (LABC::OSC_DISPLAY::TOP_INFO_LABEL_SIZE);
  m_status->labelcolor  (FL_WHITE);
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_voltage_per_division_labels ()
{
  for (unsigned chan = 0; chan < m_voltage_per_division_labels.size (); chan++)
  {
    int X = 
      m_internal_display->x () - 
      (LABC::OSC_DISPLAY::Y_AXIS_LABEL_HORIZONTAL_INTRASPACE *
      m_voltage_per_division_labels.size ()) + 
      (LABC::OSC_DISPLAY::Y_AXIS_LABEL_HORIZONTAL_INTRASPACE *
      chan);
    
    for (unsigned row = 0; row < m_voltage_per_division_labels[chan].size (); row++)
    {
      int Y = std::round (m_internal_display->y () + (row * m_internal_display->m_row_height));

      Fl_Box* box = new Fl_Box (X, Y, 3, 20, "0");

      box->labelcolor (LABC::OSC_DISPLAY::CHANNEL_COLORS[chan]);
      box->labelsize  (LABC::OSC_DISPLAY::AXIS_LABEL_SIZE);
      box->align      (FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE);

      m_voltage_per_division_labels[chan][row] = box;
    }

    m_voltage_per_division_labels[chan].front ()->align (FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE);
    m_voltage_per_division_labels[chan].back ()->align  (FL_ALIGN_TOP_LEFT);

    init_child_widgets_voltage_per_division_units (X, chan);
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_voltage_per_division_units (int      X,
                                               unsigned chan)
{
  char label[20];
  std::snprintf (label, sizeof (label), "C%d mV", chan + 1);

  Fl_Box* box = new Fl_Box (X, m_internal_display->y (), 3, 20, "C0 mV");

  box->copy_label (label);
  box->labelcolor (LABC::OSC_DISPLAY::CHANNEL_COLORS[chan]);
  box->labelsize  (LABC::OSC_DISPLAY::AXIS_LABEL_SIZE);
  box->align      (FL_ALIGN_TOP_LEFT);

  m_voltage_per_division_units[chan] = box;
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_time_per_division_labels ()
{
  for (unsigned a = 0; a < m_time_per_division_labels.size (); a++)
  {
    Fl_Box* box = new Fl_Box (
      m_internal_display->x () + (a * m_internal_display->m_column_width),
      m_internal_display->y () + m_internal_display->h (),
      3,
      20
    );

    box->labelcolor (LABC::OSC_DISPLAY::X_AXIS_LABEL_COLOR);
    box->labelsize  (LABC::OSC_DISPLAY::AXIS_LABEL_SIZE);

    m_time_per_division_labels[a] = box;
  }

  m_time_per_division_labels.back ()->align (FL_ALIGN_LEFT);
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_channel_selectors ()
{
  for (unsigned a = 0; a < m_channel_selectors.size (); a++)
  {
    Fl_Button* butt = new Fl_Button (
      m_status->x () + m_status->w () + (a * LABC::OSC_DISPLAY::STATUS_HEIGHT),
      m_status->y (),
      LABC::OSC_DISPLAY::STATUS_HEIGHT,
      LABC::OSC_DISPLAY::STATUS_HEIGHT
    );

    char label[20];
    std::snprintf (label, sizeof (label), "C%d", a + 1);

    butt->box              (FL_THIN_UP_BOX);
    butt->color            (FL_BLACK);
    butt->selection_color  (FL_BLACK);
    butt->labelsize        (LABC::OSC_DISPLAY::AXIS_LABEL_SIZE);
    butt->labelcolor       (LABC::OSC_DISPLAY::CHANNEL_COLORS[a]);
    butt->copy_label       (label);
    butt->user_data        (reinterpret_cast<void*>(a));
    butt->callback         (cb_channel_selector_static);

    m_channel_selectors[a] = butt;
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
init_child_widgets_top_info ()
{
  m_top_info = new Fl_Box (
    m_status->x () + m_status->w () + 10 + 
      (LABC::OSC_DISPLAY::STATUS_HEIGHT * m_channel_selectors.size ()),
    m_status->y (),
    3,
    m_status->h ()
  );

  m_top_info->box         (FL_NO_BOX);
  m_top_info->align       (FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
  m_top_info->labelsize   (LABC::OSC_DISPLAY::TOP_INFO_LABEL_SIZE);
  m_top_info->labelcolor  (FL_WHITE);
}

void LABSoft_GUI_Oscilloscope_Display:: 
cb_trigger_level_static (Fl_Widget* w,
                         void*      data)
{
  LABSoft_GUI_Oscilloscope_Display* disp = 
    static_cast<LABSoft_GUI_Oscilloscope_Display*>(w->parent ());
}

void LABSoft_GUI_Oscilloscope_Display:: 
cb_channel_selector_static (Fl_Widget* w,
                            void*      data)
{
  LABSoft_GUI_Oscilloscope_Display& disp = 
    *(static_cast<LABSoft_GUI_Oscilloscope_Display*>(w->parent ()));

  unsigned channel = reinterpret_cast<int>(data);

  disp.select_channel (channel);
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_voltage_per_division (unsigned channel)
{
  std::array<Fl_Box*, LABC::OSC_DISPLAY::NUMBER_OF_ROWS + 1>& labels = 
      m_voltage_per_division_labels[channel];

  if (m_osc->is_channel_enabled (channel))
  {
    for (unsigned row = 0; row< labels.size (); row++)
    {
      double row_vpd = calc_row_voltage_per_division (channel, row);

      LABSoft_GUI_Label label (row_vpd);

      char w_label[20];
      std::snprintf (w_label, sizeof (w_label), "%3.3f", label.actual_value ());      

      labels[row]->copy_label (w_label);
      labels[row]->show ();

      if (row == 0)
      {
        std::snprintf (w_label, sizeof (w_label), "C%d %sV", channel + 1,
          label.unit_prefix ().c_str ());

        m_voltage_per_division_units[channel]->copy_label (w_label);
        m_voltage_per_division_units[channel]->show ();
      }
    }
  }
  else 
  {
    m_voltage_per_division_units[channel]->hide ();

    for (unsigned row = 0; row< labels.size (); row++)
    {
      labels[row]->hide ();
    }
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_voltage_per_division ()
{
  for (unsigned chan = 0; chan < m_voltage_per_division_labels.size (); chan++)
  {
    update_gui_voltage_per_division (chan); 
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_time_per_division ()
{
  for (int col = 0; col < m_time_per_division_labels.size (); col++)
  {
    double col_tpd = calc_col_time_per_division (col);

    LABSoft_GUI_Label label (col_tpd, LABSoft_GUI_Label::UNIT::SECOND);
    
    m_time_per_division_labels[col]->copy_label (label.to_text ().c_str ());
  }
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_top_info ()
{
  LABSoft_GUI_Label label (m_osc->sampling_rate (), LABSoft_GUI_Label::UNIT::HERTZ);

  std::stringstream ss;

  ss  << m_osc->samples ()
      << " samples at "
      << label.to_text ()
      << " | "
      << LABF::get_now_timestamp ();
  
  m_top_info->copy_label (ss.str ().c_str ());
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
  unsigned trigger_source = m_osc->trigger_source ();

  // 1. update color
  m_trigger_level_slider->selection_color 
    (LABC::OSC_DISPLAY::CHANNEL_COLORS[trigger_source]);
  
  // 2. calc values
  double max_tpd_raw  = m_osc->chan_voltage_per_division (trigger_source) * 
    LABC::OSC_DISPLAY::NUMBER_OF_ROWS_HALF;
  double min_tpd_raw  = -max_tpd_raw;
  double max_bounds   = max_tpd_raw - m_osc->vertical_offset (trigger_source);
  double min_bounds   = min_tpd_raw - m_osc->vertical_offset (trigger_source);

  // 3. set values
  // if fl_slider is vertical, minimum is above, max is below
  m_trigger_level_slider->bounds  (max_bounds, min_bounds);
  m_trigger_level_slider->value   (m_osc->trigger_level ());
}

void LABSoft_GUI_Oscilloscope_Display:: 
update_gui_vertical_offset_slider ()
{
  if (m_osc->has_enabled_channels ())
  {
    unsigned trigger_source = m_osc->trigger_source ();
    
    m_vertical_offset_slider->show ();
    m_vertical_offset_slider->selection_color 
      (LABC::OSC_DISPLAY::CHANNEL_COLORS[m_selected_channel]);
    
    // 2. calc values
    double max_tpd_raw  = m_osc->chan_voltage_per_division (trigger_source) * 
      LABC::OSC_DISPLAY::NUMBER_OF_ROWS_HALF;
    double min_tpd_raw  = -max_tpd_raw;
    double max_bounds   = max_tpd_raw - m_osc->vertical_offset (trigger_source);
    double min_bounds   = min_tpd_raw - m_osc->vertical_offset (trigger_source);

    // 3. set values
    // if fl_slider is vertical, minimum is above, max is below
    m_vertical_offset_slider->bounds  (max_bounds, min_bounds);
    m_vertical_offset_slider->value   (m_osc->trigger_level ());
  }
}

double LABSoft_GUI_Oscilloscope_Display:: 
calc_row_voltage_per_division (unsigned channel,
                               unsigned row)
{
  int index = static_cast<int>(row) - 
    static_cast<int>(LABC::OSC_DISPLAY::NUMBER_OF_ROWS_HALF);

  double row_vpd = (-index * m_osc->chan_voltage_per_division (channel)) - 
    m_osc->chan_vertical_offset (channel);
  
  return (row_vpd);
}

double LABSoft_GUI_Oscilloscope_Display:: 
calc_col_time_per_division (unsigned col)
{
  int index = static_cast<int>(col) -
   static_cast<int>(LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS_HALF);
  
  double col_tpd = (index * m_osc->time_per_division ()) + 
    m_osc->horizontal_offset ();

  return (col_tpd);
}

// eof