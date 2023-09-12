#include "LABSoft_Controller_LABChecker_Digital.h"

#include <string>
#include <iostream>

#include "LABSoft_Controller.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"

LABSoft_Controller_LABChecker_Digital:: 
LABSoft_Controller_LABChecker_Digital (LABSoft_Controller& _LABSoft_Controller)
  : LABSoft_Controller_Unit (_LABSoft_Controller)
{
  init_gui ();
}

void LABSoft_Controller_LABChecker_Digital:: 
init_gui ()
{
  LABSoft_GUI& gui = m_controller.gui ();
  
  LABSoft_GUI_LABChecker_Digital_Input_Table& table = *(m_controller.gui ().
    labchecker_digital_labsoft_gui_labchecker_digital_input_table_table);

  update_gui_digital_output_count (table.output_count ());
}

void LABSoft_Controller_LABChecker_Digital:: 
update_gui_digital_output_count (unsigned value)
{
  char c[30];

  Fl_Input& w = *(m_controller.gui ().digital_fl_input_output_count);

  std::snprintf (c, sizeof (c), "%d", value);
  w.value (c);

  w.copy_label  (" ");

  std::snprintf (c, sizeof (c), "Outputs (Max %d)", m_controller.gui ().
    labchecker_digital_labsoft_gui_labchecker_digital_input_table_table->max_output_count ());
    
  w.copy_label  (c);
}

void LABSoft_Controller_LABChecker_Digital::
cb_digital_input_bits (Fl_Choice*  w, 
                        void*       data)
{
  const char* text  = w->text ();
  unsigned bits     = std::atoi (text);

  m_controller.gui ().labchecker_digital_labsoft_gui_labchecker_digital_input_table_table->
    input_bits (bits);

  update_gui_digital_output_count (m_controller.gui ().
    labchecker_digital_labsoft_gui_labchecker_digital_input_table_table->output_count ());
}

void LABSoft_Controller_LABChecker_Digital::
cb_digital_output_bits (Fl_Choice*  w, 
                        void*       data)
{
  const char* text  = w->text ();
  unsigned bits     = std::atoi (text);

  m_controller.gui ().labchecker_digital_labsoft_gui_labchecker_digital_input_table_table->
    output_bits (bits);
}

void LABSoft_Controller_LABChecker_Digital::
cb_digital_output_count (Fl_Input*  w, 
                         void*      data)
{
  LABSoft_GUI_LABChecker_Digital_Input_Table& table = *(m_controller.gui ().
    labchecker_digital_labsoft_gui_labchecker_digital_input_table_table);

  const char* input = w->value ();
  unsigned output_count;

  try 
  {
    output_count = std::atoi (input);

    table.output_count (output_count);
  }
  catch (const std::exception& err)
  {

  }

  update_gui_digital_output_count (table.output_count ());
}

void LABSoft_Controller_LABChecker_Digital:: 
cb_digital_create_file (Fl_Button*  w, 
                        void*       data)
{
  std::string file_name ("checker.labcc");

  Fl_Native_File_Chooser chooser;

  chooser.title       ("Save Circuit Checker File...");
  chooser.type        (Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
  chooser.directory   ("/");
  chooser.preset_file (file_name.c_str ());
  chooser.options     (Fl_Native_File_Chooser::NEW_FOLDER | 
                        Fl_Native_File_Chooser::SAVEAS_CONFIRM);
  
  switch (chooser.show ())
  {
    case (1): // cancel
    {

      break;
    }

    default: 
    {
      LABSoft_GUI_LABChecker_Digital_Input_Table& table = *(m_controller.gui ().
        labchecker_digital_labsoft_gui_labchecker_digital_input_table_table);

      m_controller.lab ().m_LABChecker_Digital.create_file_digital 
        (table.inputs (), table.outputs (), chooser.filename ());

      break;
    }
  }  
}

// EOF