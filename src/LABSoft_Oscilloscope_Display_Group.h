#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H

#include <vector>

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>

#include "LabelValue.h"
#include "LABSoft_Oscilloscope_Display.h"
#include "Defaults.h"

class LABSoft_Oscilloscope_Display_Group : public Fl_Group
{
  public:    
    // for display operation
    bool    m_is_enabled = false,
            m_is_function_generator_mode_enabled = false;

    int     m_number_of_channels      = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_CHANNELS,
            m_max_number_of_channels  = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_MAX_NUMBER_OF_CHANNELS,
            m_number_of_rows          = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS,  
            m_number_of_columns       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_COLUMNS,
            m_time_per_division_unit_scaler = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION_UNIT_SCALER;

    double  m_x_offset            = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_OFFSET,
            m_time_per_division   = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION,
            m_voltage_per_division  = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_VOLTAGE_PER_DIVISION;

    // for display (CSS-ish)
    int     m_upper_padding       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_UPPER_PADDING,         // upper padding of grid from display group background
            m_lower_padding       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LOWER_PADDING,         // lower padding
            m_left_padding        = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LEFT_PADDING,          // left padding
            m_right_padding       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_RIGHT_PADDING,         // right padding
            m_x_label_padding     = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_PADDING,       // padding of x-axis labels from bottom of grid
            m_y_label_padding     = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_PADDING,       // padding of first column of y-axis labels from left of grid
            m_y_label_interspace  = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_INTERSPACE,    // spacing between columns of y-axis labels
            m_y_label_unit_padding = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_UNIT_PADDING, // padding of voltage unit of y-axis labels from top of grid
            m_label_size          = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LABEL_SIZE,            // size of all x and y axis labels
            m_default_label_color = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_DEFAULT_LABEL_COLOR,
            m_background_color    = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_BACKGROUND_COLOR;
    
    std::array<int, LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_MAX_NUMBER_OF_CHANNELS> m_channels_graph_color = 
      LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_CHANNELS_GRAPH_COLOR;

    // widgets 
    Fl_Box* m_upper_osc_disp_info;
    std::vector<Fl_Box*>              m_x_labels; 
    std::vector<std::vector<Fl_Box*>> m_y_labels;  
    std::vector<Fl_Box*>              m_y_label_units;
    LABSoft_Oscilloscope_Display     *m_display;
    Channel_Signals                   *m_channel_signals; // this is the Channel_Signals from LAB_Oscilloscope

    // --- functions --- 
          LABSoft_Oscilloscope_Display_Group (int X, int Y, int W, int H, const char *label = 0);

    void  enable ();
    void  disable ();
    
    void  update ();
    void  update_fg ();

    // draw functions
    void  draw ();
    void  draw_axes_labels ();
    void  update_volts_per_division_labels ();
    void  update_volts_per_division_labels (int channel);
    void  update_time_per_division_labels ();
    void  hide_volts_per_division_labels (unsigned channel);
    void  show_volts_per_division_labels (unsigned channel);
    void  update_upper_osc_disp_info (const char *text); 
    void  volts_per_division (int channel, double value);
    void  vertical_offset (int channel, LabelValue _LabelValue);
    void  time_per_division (int channel, double value);
    

    LABSoft_Oscilloscope_Display* display ()
    {
      return m_display;
    }

    // forwarding functions to actual oscilloscope display
    void load_channel_signals (Channel_Signals *_Channel_Signals)
    {
      m_display->load_channel_signals (_Channel_Signals);
    }

  void channel_enable (unsigned channel)
  {
    update_volts_per_division_labels ();
  }

  void channel_disable (unsigned channel)
  {
    update_volts_per_division_labels ();
  }

  void channel_signals (Channel_Signals *_Channel_Signals)
  {
    m_channel_signals = m_display->m_channel_signals = _Channel_Signals;
  }
};

#endif