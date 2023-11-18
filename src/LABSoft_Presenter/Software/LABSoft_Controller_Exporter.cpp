#include "LABSoft_Controller_Exporter.h"

#include <cstdio>
#include <sstream>
#include <fstream>

#include <FL/Fl_Widget.H>
#include <FL/Fl_Native_File_Chooser.H>

#include "../../LAB/LAB.h"
#include "../LABSoft_Controller.h"
#include "../../LABSoft_GUI/LABSoft_GUI.h"
#include "../../Utility/LABSoft_GUI_Label_Values.h"

LABSoft_Controller_Exporter::
LABSoft_Controller_Exporter (LABSoft_Controller& _LABSoft_Controller)
  : LABSoft_Controller_Unit  (_LABSoft_Controller)
{

}

void LABSoft_Controller_Exporter:: 
show_window_as_modal ()
{
  gui ().main_fl_window_exporter->set_modal ();
  gui ().main_fl_window_exporter->show ();
}

void LABSoft_Controller_Exporter:: 
hide_window_as_modal ()
{
  gui ().main_fl_window_exporter->hide ();
  gui ().main_fl_window_exporter->clear_modal_states ();
}

void LABSoft_Controller_Exporter:: 
load_window_label ()
{
  char label[50];

  std::snprintf (label, sizeof (label), "%s - Export", 
    gui ().main_fl_tabs->value ()->label ());

  gui ().main_fl_window_exporter->label (label);
}

void LABSoft_Controller_Exporter:: 
load_table_data ()
{
  lab ().m_Exporter.update_data_table ();
}

void LABSoft_Controller_Exporter:: 
load_image_data ()
{

}

void LABSoft_Controller_Exporter:: 
store_displayed_instrument ()
{
  const char* lbl = gui ().main_fl_tabs->value ()->label ();

  std::unordered_map<std::string, LABE::LAB::INSTRUMENT>::iterator it = 
    LABS_GUI_VALUES::MAIN::INSTRUMENT.find (std::string (lbl));

  if (it != LABS_GUI_VALUES::MAIN::INSTRUMENT.end ())
  {
     lab ().m_Exporter.instrument (it->second);
  }
  else 
  {
    throw (std::runtime_error ("LABSoft_Controller_Exporter::store_displayed_instrument ()"));
  }
}

void LABSoft_Controller_Exporter:: 
update_data_display ()
{
  std::string label = std::string (gui ().exporter_fl_tabs->value ()->label ());

  if (label == "Data")
  {
    load_table_data ();
  }
  else if (label == "Image")
  {
    load_image_data ();
  }
  else 
  {
    throw (std::runtime_error ("LABSoft_Controller_Exporter::update_data_display ()"));
  }
}

void LABSoft_Controller_Exporter:: 
show_file_chooser_export_window ()
{
  Fl_Native_File_Chooser chooser;

  chooser.title       (gui ().main_fl_window_exporter->label ());
  chooser.type        (Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
  chooser.preset_file ("default.csv");
  chooser.filter      ("*.csv");
  chooser.options     (Fl_Native_File_Chooser::NEW_FOLDER | 
                       Fl_Native_File_Chooser::SAVEAS_CONFIRM);
  
  switch (chooser.show ())
  {
    case (1): // user cancelled
    {
      break;
    }

    default:
    {
      lab ().m_Exporter.export_data_to_csv_file (std::string (chooser.filename ()));

      hide_window_as_modal ();

      break;
    }
  }
}

void LABSoft_Controller_Exporter::
cb_show_window ()
{
  show_window_as_modal        ();
  store_displayed_instrument  ();
  load_window_label           ();
  update_data_display         ();
}

void LABSoft_Controller_Exporter::
cb_cancel ()
{
  hide_window_as_modal ();
}

void LABSoft_Controller_Exporter:: 
cb_save ()
{
  show_file_chooser_export_window ();
}

void LABSoft_Controller_Exporter:: 
cb_comments (Fl_Light_Button* w)
{
  lab ().m_Exporter.comments (w->value ());
}

void LABSoft_Controller_Exporter:: 
cb_headers (Fl_Light_Button* w)
{
  lab ().m_Exporter.headers (w->value ());
}

void LABSoft_Controller_Exporter:: 
cb_labels (Fl_Light_Button* w)
{
  lab ().m_Exporter.labels (w->value ());
}
