#ifndef LABSOFT_CONTROLLER_H
#define LABSOFT_CONTROLLER_H

#include <FL/Fl_Tabs.H>

#include "../LAB/LAB.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"

#include "LABSoft_Controller_Oscilloscope.h"
#include "LABSoft_Controller_Voltmeter.h"
#include "LABSoft_Controller_Function_Generator.h"
#include "LABSoft_Controller_Logic_Analyzer.h"
#include "LABSoft_Controller_Circuit_Checker.h"
#include "LABSoft_Controller_Digital_Circuit_Checker.h"
#include "LABSoft_Controller_Main_Window.h"
#include "LABSoft_Controller_LABChecker_Digital.h"

class LABSoft_Controller
{
  private:
    LAB*          m_LAB;
    LABSoft_GUI*  m_LABSoft_GUI;
  
  public:
    LABSoft_Controller_Oscilloscope             m_Oscilloscope;             
    LABSoft_Controller_Voltmeter                m_Voltmeter;
    LABSoft_Controller_Function_Generator       m_Function_Generator;
    LABSoft_Controller_Logic_Analyzer           m_Logic_Analyzer;
    LABSoft_Controller_Circuit_Checker          m_Circuit_Checker;
    LABSoft_Controller_Digital_Circuit_Checker  m_Digital_Circuit_Checker;
    LABSoft_Controller_Main_Window              m_Main_Window;
    LABSoft_Controller_LABChecker_Digital       m_LABChecker_Digital;

  private:      
    static void update_display (void *data);     
  
  public: 
    LABSoft_Controller (LAB* _LAB, LABSoft_GUI* _LABSoft_GUI);

    void tab_selection_color_toggle (Fl_Group* tab, bool value);
    void main_tab_selection_color   ();

    void cb_main_fl_tabs (Fl_Tabs* w, void* data); 
};

#endif