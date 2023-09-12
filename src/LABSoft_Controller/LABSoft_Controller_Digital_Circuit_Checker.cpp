#include "LABSoft_Controller_Digital_Circuit_Checker.h"

#include <cstdio>
#include <string>

#include <FL/fl_ask.H>

#include "LABSoft_Controller.h"

LABSoft_Controller_Digital_Circuit_Checker:: 
LABSoft_Controller_Digital_Circuit_Checker (LABSoft_Controller& _LABSoft_Controller)
 : LABSoft_Controller_Unit (_LABSoft_Controller)
{

}

std::string LABSoft_Controller_Digital_Circuit_Checker:: 
get_filename_from_path (const std::string& path)
{
  // 1. Get position of the last separator slash
  size_t last_separator_pos = path.find_last_of ("/\\");

  // 2. Extract the substring starting from the last separator
  std::string filename = path.substr (last_separator_pos + 1);

  // 3. Get position of the last dot (before the extension)
  size_t last_dot_pos = filename.find_last_of (".");

  // 4. Extract the substring until before the last dot, to get 
  //    actual filename
  filename = filename.substr (0, last_dot_pos);

  return (filename);
}

void LABSoft_Controller_Digital_Circuit_Checker:: 
display_results (const LAB_Digital_Circuit_Checker::ScoreData score_data)
{
  char str[50];

  std::snprintf (str, sizeof (str), "%d/%d - %2.2f%%", score_data.score_current,
    score_data.score_total, score_data.score_percent);

  m_controller.gui ().digital_circuit_checker_fl_output_results->value (str);
}

void LABSoft_Controller_Digital_Circuit_Checker:: 
update_gui_reset ()
{
  LABSoft_GUI& gui = m_controller.gui ();

  gui.digital_circuit_checker_labchecker_gui_digital_output_table_table->reset ();
  gui.digital_circuit_checker_fl_output_selected_file->value ("");
  gui.digital_circuit_checker_fl_output_results->value ("");
}

void LABSoft_Controller_Digital_Circuit_Checker:: 
cb_load_file (Fl_Button*  w, 
              void*       data)
{
  Fl_Native_File_Chooser chooser;

  chooser.title   ("Open Circuit Checker File");
  chooser.type    (Fl_Native_File_Chooser::BROWSE_FILE);
  chooser.filter  ("LAB Circuit Checker\t*.labcc");

  //

  Fl_Output& selected_file = *(m_controller.gui ().digital_circuit_checker_fl_output_selected_file);

  //

  try 
  {
    switch (chooser.show ())
    {
      // user picked file
      case (0):
      {
        std::string path (chooser.filename ());

        size_t dot_pos = path.find_last_of (".");

        if (dot_pos != std::string::npos)
        {
          std::string file_extension = path.substr (dot_pos + 1);

          if (file_extension == "labcc")
          {
            update_gui_reset ();

            selected_file.value (get_filename_from_path (path).c_str ());

            m_controller.lab ().m_Digital_Circuit_Checker.load_file (path);
          }
          else 
          {
            throw (std::domain_error ("Selected file is not a LAB Circuit Checker file."));
          }
        }

        break;
      }

      // user cancelled
      case (1):
      {
        break;
      }

      // open failed
      case (-1):
      {
        break;
      }
    }
  }
  catch (const std::exception& e)
  {
    fl_message (e.what ());

    selected_file.value ("");
  }
}

void LABSoft_Controller_Digital_Circuit_Checker:: 
cb_unload_file (Fl_Button*  w, 
                void*       data)
{
  m_controller.lab ().m_Digital_Circuit_Checker.unload_file ();

  update_gui_reset ();
}

void LABSoft_Controller_Digital_Circuit_Checker:: 
cb_run_checker (Fl_Button*  w,
                void*       data)
{
  try
  {
    m_controller.lab ().m_Digital_Circuit_Checker.run_checker ();

    if (m_controller.lab ().m_Digital_Circuit_Checker.are_results_ready ())
    {
      LAB_Digital_Circuit_Checker::ScoreData score_data = m_controller.lab ().
        m_Digital_Circuit_Checker.get_score_data ();

      // Update results Fl_Output widget
      display_results (score_data);

      // Update table
      m_controller.gui ().digital_circuit_checker_labchecker_gui_digital_output_table_table->
        display_results (score_data);

      fl_message ("Checking done.");
    }    
  }
  catch (const std::exception& e)
  {
    fl_message (e.what ());
  }
}

// EOF