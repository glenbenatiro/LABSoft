#include "LABSoft_GUI_Oscilloscope_Internal_Display.h"

LABSoft_GUI_Oscilloscope_Internal_Display::
Internal_Display (int         X,
                  int         Y,
                  int         W,
                  int         H,
                  const char* label)
  : Fl_Widget (X, Y, W, H, label)
{
  calc_cached_values_all  ();
}

void LABSoft_GUI_Oscilloscope_Internal_Display:: 
draw ()
{
  draw_box      (FL_FLAT_BOX, m_background_color);
  draw_grid     ();
}

int LABSoft_GUI_Oscilloscope_Internal_Display::
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
        m_controller->m_Oscilloscope_Display.cb_mouse_wheel (Fl::event_dy ());

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

void LABSoft_GUI_Oscilloscope_Internal_Display::
resize (int x,
        int y,
        int w,
        int h)
{
  Fl_Widget::resize (x, y, w, h);

  reserve_pixel_points ();
  calc_cached_values_all ();
}

void LABSoft_GUI_Oscilloscope_Internal_Display::
draw_grid ()
{
  // 1. set line color
  fl_color (m_grid_color);

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

void LABSoft_GUI_Oscilloscope_Internal_Display::
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

void LABSoft_GUI_Oscilloscope_Internal_Display::
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

void LABSoft_GUI_Oscilloscope_Internal_Display::
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

void LABSoft_GUI_Oscilloscope_Internal_Display::
calc_cached_values_display ()
{
  m_row_height                = static_cast<double>(h ()) / LABC::OSC_DISPLAY::NUMBER_OF_ROWS;
  m_column_width              = static_cast<double>(w ()) / LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS;

  m_x_axis_minor_ticks_width  = m_column_width / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;
  m_y_axis_minor_ticks_width  = m_row_height / LABC::OSC_DISPLAY::NUMBER_OF_MINOR_TICKS;

  m_display_half_height       = h () / 2.0;
  m_display_height_midline    = std::round (y () + m_display_half_height);
}

void LABSoft_GUI_Oscilloscope_Internal_Display:: 
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







bool LABSoft_GUI_Oscilloscope_Internal_Display:: 
calc_mark_samples (double   time_per_division_delta_scaler,
                   unsigned samples_to_display) const
{
  return ((samples_to_display <= LABC::OSC_DISPLAY::SAMPLE_MARKING_THRESHOLD) &&
    (LABF::is_greater_than (time_per_division_delta_scaler, 0.5, LABC::LABSOFT::EPSILON)));
}

void LABSoft_GUI_Oscilloscope_Internal_Display::
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

void LABSoft_GUI_Oscilloscope_Internal_Display::
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

void LABSoft_GUI_Oscilloscope_Internal_Display::
calc_cached_values_all ()
{
  calc_cached_values_display          ();
  calc_cached_values_sample_y_scaler  ();

  calc_cached_values_drawing          ();
  calc_cached_values_sample_x_offset  ();
}

void LABSoft_GUI_Oscilloscope_Internal_Display::
calc_cached_values_horizontal ()
{
  calc_cached_values_drawing          ();
  calc_cached_values_sample_x_offset  ();
}

int LABSoft_GUI_Oscilloscope_Internal_Display::
calc_sample_y_coord (double   sample, 
                     unsigned channel) const
{
  return (std::round (m_display_height_midline - (sample * m_sample_y_scaler[channel])));
}

int LABSoft_GUI_Oscilloscope_Internal_Display::
calc_mouse_drag_time_per_div_scaler (int drag_x) const
{
  return ((m_mouse_down_start_x - drag_x) / m_x_axis_minor_ticks_width);
}

void LABSoft_GUI_Oscilloscope_Internal_Display::
cb_mouse_wheel (int direction)
{
  m_controller->m_Oscilloscope.cb_display_time_per_division (direction);
}

void LABSoft_GUI_Oscilloscope_Internal_Display::
cb_mouse_click (int x, int y)
{
  m_mouse_down_start_x = x;
  m_mouse_down_start_y = y;
  
  m_pre_drag_horizontal_offset = m_osc->horizontal_offset ();
}

void LABSoft_GUI_Oscilloscope_Internal_Display::
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

void LABSoft_GUI_Oscilloscope_Internal_Display::
load_oscilloscope (const LAB_Oscilloscope& oscilloscope)
{
  m_osc = &oscilloscope;

  calc_cached_values_all ();
}

void LABSoft_GUI_Oscilloscope_Internal_Display::
load_presenter (const LABSoft_Controller& presenter)
{
  m_presenter = &presenter;
}

void LABSoft_GUI_Oscilloscope_Internal_Display::
update_voltage_per_division ()
{
  calc_cached_values_sample_y_scaler ();
}

void LABSoft_GUI_Oscilloscope_Internal_Display:: 
update_horizontal_offset ()
{
  calc_cached_values_horizontal ();
}

void LABSoft_GUI_Oscilloscope_Internal_Display:: 
update_time_per_division ()
{
  calc_cached_values_horizontal ();
}

void LABSoft_GUI_Oscilloscope_Internal_Display:: 
update_samples ()
{
  calc_cached_values_horizontal ();
}

void LABSoft_GUI_Oscilloscope_Internal_Display:: 
update_sampling_rate ()
{
  calc_cached_values_horizontal ();
}

double LABSoft_GUI_Oscilloscope_Internal_Display:: 
row_height () const
{
  return (m_row_height);
}

double LABSoft_GUI_Oscilloscope_Internal_Display:: 
column_width () const 
{
  return (m_column_width);
}
     