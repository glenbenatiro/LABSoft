#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H

#include <vector>

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>

#include "LABSoft_Oscilloscope_Display.h"
#include "Defaults.h"

class LABSoft_Oscilloscope_Display_Group : public Fl_Group
{
  public:    
    // for display
    bool    m_is_enabled = false,
            m_is_function_generator_mode_enabled = false;

    int     m_number_of_channels  = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_CHANNELS,
            m_number_of_rows      = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_ROWS,  
            m_number_of_columns   = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_NUMBER_OF_COLUMNS,
            m_time_per_division_unit_scaler = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION_UNIT_SCALER;

    double  m_x_offset            = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_OFFSET,
            m_time_per_division   = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_TIME_PER_DIVISION;

    // for display (CSS-ish)
    int     m_upper_padding       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_UPPER_PADDING,
            m_lower_padding       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LOWER_PADDING,
            m_left_padding        = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LEFT_PADDING,
            m_right_padding       = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_RIGHT_PADDING,
            m_x_label_padding     = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_X_LABEL_PADDING,
            m_y_label_padding     = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_Y_LABEL_PADDING,
            m_label_size          = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LABEL_SIZE,
            m_label_color         = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_LABEL_COLOR,
            m_background_color    = LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_BACKGROUND_COLOR;

    // widgets 
    std::vector<Fl_Box*>              m_x_labels; 
    std::vector<std::vector<Fl_Box*>> m_y_labels;  
    LABSoft_Oscilloscope_Display     *m_display;

    // --- functions --- 
          LABSoft_Oscilloscope_Display_Group (int X, int Y, int W, int H, const char *label = 0);

    void  draw ();
    void  update_x_axis_labels ();
    void  update_y_axis_labels ();
    void  enable_function_generator_mode ();
    void  update ();

    void  update_time_per_division (double time_per_division, 
      int time_per_division_unit_prefix_power_scaler);
    void  update_volts_per_division (double volts_per_division, 
      int volts_per_division_unit_prefix_power_scaler, int channel);
};

#endif