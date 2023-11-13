#ifndef LABSOFT_CONTROLLER_H
#define LABSOFT_CONTROLLER_H

#include <FL/Fl_Tabs.H>

#include "LABSoft_Controller_Oscilloscope.h"
#include "LABSoft_Controller_Voltmeter.h"
#include "LABSoft_Controller_Function_Generator.h"
#include "LABSoft_Controller_Logic_Analyzer.h"
#include "LABSoft_Controller_Digital_Circuit_Checker.h"
#include "LABSoft_Controller_Main_Window.h"

#include "Software/LABSoft_Controller_Exporter.h"
#include "Software/LABSoft_Controller_Calibration.h"
#include "Software/LABSoft_Controller_LABChecker_Digital.h"
#include "Software/LABSoft_Presenter_Oscilloscope_Display.h"

class LAB;
class LABSoft_GUI;

class LABSoft_Controller
{
  private:
    LAB&          m_LAB;
    LABSoft_GUI&  m_LABSoft_GUI;
  
  public:
    // Hardware
    LABSoft_Controller_Oscilloscope             m_Oscilloscope;             
    LABSoft_Controller_Voltmeter                m_Voltmeter;
    LABSoft_Controller_Function_Generator       m_Function_Generator;
    LABSoft_Controller_Logic_Analyzer           m_Logic_Analyzer;
    LABSoft_Controller_Digital_Circuit_Checker  m_Digital_Circuit_Checker;
    LABSoft_Controller_LABChecker_Digital       m_LABChecker_Digital;

    // Software/GUI
    LABSoft_Controller_Main_Window              m_Main_Window;
    LABSoft_Controller_Exporter                 m_Exporter;
    LABSoft_Controller_Calibration              m_Calibration;
    LABSoft_Presenter_Oscilloscope_Display      m_Oscilloscope_Display;

  private:      
    void load_controller_to_gui ();
    void update_gui_tab_colors  ();

    static void update_display (void *data);     
  
  public: 
    LABSoft_Controller (LAB& _LAB, LABSoft_GUI& _LABSoft_GUI);

    void cb_tabs (Fl_Group* w, void* data);

    LAB&          lab () const;
    LABSoft_GUI&  gui () const;
};

#endif