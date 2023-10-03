#ifndef LABSOFT_CONTROLLER_EXPORTER
#define LABSOFT_CONTROLLER_EXPORTER

#include "LABSoft_Controller_Unit.h"

#include <FL/Fl_Light_Button.H>

class LABSoft_Controller_Exporter : public LABSoft_Controller_Unit 
{
  private:
    void        show_window_as_modal ();
    void        hide_window_as_modal ();
    void        load_window_label ();
    void        load_table_data ();
    void        load_image_data ();
    void        store_displayed_instrument ();
    void        update_data_display ();
    void        show_file_chooser_export_window ();
    
  public:
    LABSoft_Controller_Exporter (LABSoft_Controller& _LABSoft_Controller);

    void cb_show_window ();
    void cb_cancel      ();
    void cb_save        ();
    void cb_comments    (Fl_Light_Button* w);
};

#endif 