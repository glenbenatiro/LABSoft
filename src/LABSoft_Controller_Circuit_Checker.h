#ifndef LABSOFT_CONTROLLER_CIRCUIT_CHECKER_H
#define LABSOFT_CONTROLLER_CIRCUIT_CHECKER_H

#include <iostream>
#include <ctime>
#include <vector>
#include "pugixml.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Display.H>

#include "LAB.h"
#include "LAB_Globals.h"
#include "LABSoft_GUI.h"

struct Signal
{
  const char *type;
  float amplitude,
        frequency;
};

struct SignalPair
{
  Signal input, output;
  float duration;
};

struct SignalChain
{
  float sample_rate;
  std::vector<SignalPair> signal_pairs;
};

class LABSoft_Controller_Circuit_Checker
{
  public:
    LAB             *m_LAB;
    LABSoft_GUI     *m_LABSoft_GUI;
    Fl_Text_Buffer  *m_text_buffer;
    Fl_Text_Display *m_text_display;
    Fl_File_Chooser *m_file_chooser;
    const char      *m_circuit_checker_filename;
    bool            m_flag_is_circuit_checker_running = false,
                    m_flag_is_file_valid = false;
    SignalChain     m_signal_chain;

    pugi::xml_document m_xml_document;


    LABSoft_Controller_Circuit_Checker (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // functions
    void LABSoft_Controller_Circuit_Checker_log (const char *text);
    void LABSoft_Controller_Circuit_Checker_parse_xml_file ();
    void LABSoft_Controller_Circuit_Checker_start_circuit_checking ();
    void LABSoft_Controller_Circuit_Checker_log_all_signals ();

    // callback functions
    void LABSoft_Controller_Circuit_Checker_cb_fl_button_choose_circuit_checker_file (Fl_Button *w, void *data);
    void LABSoft_Controller_Circuit_Checker_cb_fl_button_start_circuit_checking (Fl_Button *w, void *data);
};

#endif