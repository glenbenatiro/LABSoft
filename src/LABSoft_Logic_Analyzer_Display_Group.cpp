#include "LABSoft_Logic_Analyzer_Display_Group.h"

#include <iostream>

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl.H>

#include "LabelValue.h"

LABSoft_Logic_Analyzer_Display_Group::
LABSoft_Logic_Analyzer_Display_Group 
  (int X, int Y, int W, int H, const char *label) 
    : Fl_Group (X, Y, W, H, label)
{
  // create new labsoft oscilloscope display instance, with paddings
  m_display = new LABSoft_Logic_Analyzer_Display (
    X + LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_CHANNEL_MENU_BUTTON_WIDTH,          
    Y,
    W - LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_CHANNEL_MENU_BUTTON_WIDTH,
    H - LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_STRIP_HEIGHT, 
    0
  );
  
  // Channel menu buttons
  for (int a = 0; a < LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS; a++)
  {
    char label[20];

    sprintf (label, "Channel %d", a + 1);

    double button_y_coord = Y + (a * ((H - 
      LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_STRIP_HEIGHT) / 
        (LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS)));
    
    double button_height = ((H - 
      LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_STRIP_HEIGHT) / 
        LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS);

    Fl_Menu_Button *button = new Fl_Menu_Button (
      X,       
      button_y_coord,
      LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_CHANNEL_MENU_BUTTON_WIDTH,
      button_height,
      0
    );                                

    button->copy_label (label);

    m_channel_menu_buttons[a] = button;
  }

  // X-axis labels
  unsigned actual_graph_disp_width = W - 
    LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_CHANNEL_MENU_BUTTON_WIDTH;
  
  double column_width = static_cast<double>(actual_graph_disp_width) /
    static_cast<double>(LABSOFT_LOGIC_ANALYZER_DISPLAY_NUMBER_OF_COLUMNS); 

  for (int a = 0; a < m_x_labels.size (); a++)
  {
    char label[20];
    sprintf (label, ".");

    double box_x_coord = (X + (LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_CHANNEL_MENU_BUTTON_WIDTH) + 
      (a * column_width)) - (a == LABSOFT_LOGIC_ANALYZER_DISPLAY_NUMBER_OF_COLUMNS ?
      LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LAST_LABEL_OFFSET : 0);

    double box_y_coord = Y + H - 
      LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_STRIP_HEIGHT + 
      LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_TOP_MARGIN;

    Fl_Box *box = new Fl_Box (
      box_x_coord,
      box_y_coord,
      5,
      5,
      "2.50 s"
    );

    box->labelcolor (LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_COLOR);
    box->labelsize  (LABSOFT_LOGIC_ANALYZER_DISPLAY_GROUP_X_LABEL_SIZE);
    box->align      (FL_ALIGN_TEXT_OVER_IMAGE);

    m_x_labels[a] = box; 
  }
  
  end ();

  // update_time_per_division_labels ();
}

void LABSoft_Logic_Analyzer_Display_Group::
draw ()
{
  draw_box (FL_FLAT_BOX, LABSOFT_LOGIC_ANALYZER_DISPLAY_BG_COLOR);
  draw_children ();
}

void LABSoft_Logic_Analyzer_Display_Group::
load_logan_parent_data (LAB_Parent_Data_Logic_Analyzer *parent_data)
{
  m_parent_data_logan = parent_data;

  m_display->load_logan_parent_data (parent_data);
}

int LABSoft_Logic_Analyzer_Display_Group::
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

int LABSoft_Logic_Analyzer_Display_Group::
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

void LABSoft_Logic_Analyzer_Display_Group:: 
update_time_per_division_labels ()
{

}

// eof