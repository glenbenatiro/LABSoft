#ifndef LABSOFT_CONTROLLER_DIGITAL_CIRCUIT_CHECKER_H
#define LABSOFT_CONTROLLER_DIGITAL_CIRCUIT_CHECKER_H

#include <string>

#include <FL/Fl_Button.H>
#include <FL/Fl_Native_File_Chooser.H>

#include "LABSoft_Controller_Unit.h"
#include "../LAB/LAB_Digital_Circuit_Checker.h"

class LABSoft_Controller_Digital_Circuit_Checker : public LABSoft_Controller_Unit
{
  private:
    std::string get_filename_from_path  (const std::string& path);
    void        display_results         (const LAB_Digital_Circuit_Checker::ScoreData score_data);
    void        update_gui_reset        (); 

  public:
    LABSoft_Controller_Digital_Circuit_Checker (LABSoft_Controller& _LABSoft_Controller);

    void cb_load_file   (Fl_Button* w, void* data);
    void cb_unload_file (Fl_Button* w, void* data);
    void cb_run_checker (Fl_Button* w, void* data);
};

#endif

// EOF