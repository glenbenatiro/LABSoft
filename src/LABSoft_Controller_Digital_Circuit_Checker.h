#ifndef LABSOFT_CONTROLLER_DIGITAL_CIRCUIT_CHECKER_H
#define LABSOFT_CONTROLLER_DIGITAL_CIRCUIT_CHECKER_H

#include "LAB.h"
#include "LABSoft_GUI.h"

#include <string>

#include <FL/Fl_Button.H>
#include <FL/Fl_Native_File_Chooser.H>

// forward declare LABSoft_Controller
class LABSoft_Controller;

class LABSoft_Controller_Digital_Circuit_Checker
{
  private: 
    LAB*                m_LAB;
    LABSoft_GUI*        m_LABSoft_GUI;
    LABSoft_Controller* m_LABSoft_Controller;

  private:
    std::string get_filename_from_path  (const std::string& path);
    void        display_results         (const LAB_Digital_Circuit_Checker::ScoreData score_data);
    void        update_gui_reset        (); 

  public:
    LABSoft_Controller_Digital_Circuit_Checker (LAB*                _LAB,
                                                LABSoft_GUI*        _LABSoft_GUI,
                                                LABSoft_Controller* _LABSoft_Controller);

    void cb_load_file   (Fl_Button* w, void* data);
    void cb_unload_file (Fl_Button* w, void* data);
    void cb_run_checker (Fl_Button* w, void* data);
};

#endif

// EOF