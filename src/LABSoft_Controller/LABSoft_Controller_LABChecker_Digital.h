#ifndef LABSOFT_CONTROLLER_LABCHECKER_DIGITAL
#define LABSOFT_CONTROLLER_LABCHECKER_DIGITAL

#include <FL/Fl_Native_File_Chooser.H>

#include "../LAB/LAB.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"

// forward declare LABSoft_Controller
class LABSoft_Controller;

class LABSoft_Controller_LABChecker_Digital 
{
  private:
    LAB*                m_LAB;
    LABSoft_GUI*        m_LABSoft_GUI;
    LABSoft_Controller* m_LABSoft_Controller;

  private:
    void init_gui ();
    void update_gui_digital_output_count (unsigned value);

    static void update_display (void* data);
    
  public:
    LABSoft_Controller_LABChecker_Digital (LAB*                 _LAB, 
                                           LABSoft_GUI*         _LABSoft_GUI,
                                           LABSoft_Controller*  _LABSoft_Controller);

    void cb_digital_input_bits    (Fl_Choice* w, void* data);
    void cb_digital_output_bits   (Fl_Choice* w, void* data);
    void cb_digital_output_count  (Fl_Input*  w, void* data);
    void cb_digital_create_file   (Fl_Button* w, void* data);
};

#endif