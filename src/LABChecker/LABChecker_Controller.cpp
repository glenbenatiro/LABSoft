#include "LABChecker_Controller.h"

#include <string>
#include <iostream>

LABChecker_Controller:: 
LABChecker_Controller (LABChecker_Main* main, LABChecker_GUI* gui)
{
  m_LABChecker_Main = main;
  m_LABChecker_GUI  = gui;

  init_gui ();
}

void LABChecker_Controller:: 
init_gui ()
{
  LABChecker_GUI& gui = *m_LABChecker_GUI;
  
  LABChecker_GUI_Digital_Input_Table& table = *(m_LABChecker_GUI->
    digital_labchecker_gui_digital_input_table_table);

  update_gui_digital_output_count (table.output_count ());
}

void LABChecker_Controller:: 
update_gui_digital_output_count (unsigned value)
{
  char c[30];

  Fl_Input& w = *(m_LABChecker_GUI->digital_fl_input_output_count);

  std::snprintf (c, sizeof (c), "%d", value);
  w.value (c);

  w.copy_label  (" ");

  std::snprintf (c, sizeof (c), "Outputs (Max %d)", m_LABChecker_GUI->
    digital_labchecker_gui_digital_input_table_table->max_output_count ());
    
  w.copy_label  (c);
}

void LABChecker_Controller::
cb_digital_input_bits (Fl_Choice*  w, 
                        void*       data)
{
  const char* text  = w->text ();
  unsigned bits     = std::atoi (text);

  m_LABChecker_GUI->digital_labchecker_gui_digital_input_table_table->
    input_bits (bits);

  update_gui_digital_output_count (m_LABChecker_GUI->
    digital_labchecker_gui_digital_input_table_table->output_count ());
}

void LABChecker_Controller::
cb_digital_output_bits (Fl_Choice*  w, 
                        void*       data)
{
  const char* text  = w->text ();
  unsigned bits     = std::atoi (text);

  m_LABChecker_GUI->digital_labchecker_gui_digital_input_table_table->
    output_bits (bits);
}

void LABChecker_Controller::
cb_digital_output_count (Fl_Input*  w, 
                         void*      data)
{
  LABChecker_GUI_Digital_Input_Table& table = *(m_LABChecker_GUI->
    digital_labchecker_gui_digital_input_table_table);

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

void LABChecker_Controller:: 
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
      LABChecker_GUI_Digital_Input_Table& table = *(m_LABChecker_GUI->
        digital_labchecker_gui_digital_input_table_table);

      m_LABChecker_Main->create_file_digital (table.inputs (), table.outputs (), chooser.filename ());

      break;
    }
  }  
}

// EOF