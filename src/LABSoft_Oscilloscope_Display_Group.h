#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H

#include <vector>

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>

#include "LABSoft_Oscilloscope_Display.h"

class LABSoft_Oscilloscope_Display_Group : public Fl_Group
{
  public:
    LABSoft_Oscilloscope_Display *m_disp;
    
    // for core oscilloscope display operation
    int m_number_of_rows,
        m_number_of_columns,
        m_number_of_channels;

    float m_x_offset,
          m_time_per_div;

    // for drawing UI
    int m_upper_padding,
        m_lower_padding,
        m_left_padding,
        m_right_padding,
        m_x_label_padding,
        m_y_label_padding,
        m_label_size,
        m_label_color,
        m_background_color;

    // widgets
    LABSoft_Oscilloscope_Display     *m_display;
    std::vector<Fl_Box*>              m_x_labels;   // label for x axis
    std::vector<std::vector<Fl_Box*>> m_y_labels;  // label for y axes
    
    // functions
          LABSoft_Oscilloscope_Display_Group (int X, int Y, int W, int H, const char *label = 0);
    void  draw ();
    void  update_x_axis_labels ();

    // getters
    LABSoft_Oscilloscope_Display* display ();

    // setters
    void  rows_columns (int rows, int columns);
};

#endif