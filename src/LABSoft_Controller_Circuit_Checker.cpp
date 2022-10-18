#include "LABSoft_Controller_Circuit_Checker.h"

LABSoft_Controller_Circuit_Checker:: 
LABSoft_Controller_Circuit_Checker (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

  m_file_chooser = new Fl_File_Chooser (".",
                                        "LABSoft Circuit Checker File (*.labcc)",
                                        Fl_File_Chooser::SINGLE,
                                        "Open LABSoft Circuit Checker File");
  m_file_chooser->preview (0);
  m_text_buffer = new Fl_Text_Buffer ();
  
  m_text_display = m_LABSoft_GUI->circuit_checker_fl_text_display_logger;
  m_text_display->buffer (m_text_buffer);

  m_LABSoft_GUI->circuit_checker_fl_output_selected_circuit_checker_file->value ("No file selected.");
  m_LABSoft_GUI->circuit_checker_fl_output_similarity_score->value ("--");
  
  log ("Circuit checker tool ready.");
  log ("Connect input and outputs to your circuit and select a circuit checker file");  
}

void LABSoft_Controller_Circuit_Checker:: 
log (const char *text)
{
  char a[50], b[100];
  std::time_t time = std::time (nullptr);
  std::strftime (a, sizeof (a), "[%F | %T]", std::localtime (&time));

  // create new text
  sprintf (b, "%s: %s\n", a, text);
  m_text_buffer->append (b);
}

 
void LABSoft_Controller_Circuit_Checker:: 
parse_xml_file ()
{
  char s[100];
  sprintf (s, "Selected file %s", m_circuit_checker_filename);

  log (s);
  log ("Checking circuit checker file integrity...");


  // load circuit checker file to xml parser
  pugi::xml_parse_result xml_parse_result = m_xml_document.load_file (m_circuit_checker_filename);
  
  if (xml_parse_result)
    {
      log ("Selected circuit checker file OK.");
      m_is_file_valid = true;
      m_LABSoft_GUI->circuit_checker_fl_output_selected_circuit_checker_file->value (m_circuit_checker_filename);
    }
  else
    {
      m_is_file_valid = false;
      log ("Invalid circuit checker file. Please select another file.");
      m_LABSoft_GUI->circuit_checker_fl_output_selected_circuit_checker_file->value ("Invalid file.");

      return;
    } 

  // load contents of valid xml file
  pugi::xml_node xml_root = m_xml_document.child ("CircuitCheckerFile");
  m_signal_chain.sample_rate = xml_root.attribute ("sample_rate").as_float ();
  
  // traverse through all signal pairs
  for (pugi::xml_node node = xml_root.child ("SignalChain").child ("SignalPair"); node; node = node.next_sibling ("SignalPair"))
    {
      SignalPair signal_pair;
      signal_pair.duration = node.attribute ("duration").as_float ();

      pugi::xml_node input = node.child ("InputSignal");
      signal_pair.input.type      = input.attribute ("type").value ();
      signal_pair.input.amplitude = input.attribute ("amplitude").as_float ();
      signal_pair.input.frequency = input.attribute ("frequency").as_float ();

      pugi::xml_node output = node.child ("ExpectedOutputSignal");
      signal_pair.output.type      = output.attribute ("type").value ();
      signal_pair.output.amplitude = output.attribute ("amplitude").as_float ();
      signal_pair.output.frequency = output.attribute ("frequency").as_float ();

      m_signal_chain.signal_pairs.push_back (signal_pair);
    }
}

void LABSoft_Controller_Circuit_Checker:: 
log_all_signals ()
{
  for (int i = 0; i < m_signal_chain.signal_pairs.size (); i++)
    {
      char text[100];

      sprintf (text, "Signal Pair %d", i + 1);
      log (text);
      sprintf (text, "Duration:  %f", m_signal_chain.signal_pairs[i].duration);
      log (text);

      sprintf (text, "Input Signal");
      log (text);
      sprintf (text, "Type: %s", m_signal_chain.signal_pairs[i].input.type);
      log (text);
      sprintf (text, "Amplitude: %f", m_signal_chain.signal_pairs[i].input.amplitude);
      log (text);
      sprintf (text, "Frequency: %f", m_signal_chain.signal_pairs[i].input.frequency);
      log (text);

      sprintf (text, "Expected Output Signal");
      log (text);
      sprintf (text, "Type: %s", m_signal_chain.signal_pairs[i].output.type);
      log (text);
      sprintf (text, "Amplitude: %f", m_signal_chain.signal_pairs[i].output.amplitude);
      log (text);
      sprintf (text, "Frequency: %f", m_signal_chain.signal_pairs[i].output.frequency);
      log (text);
    }
}

void LABSoft_Controller_Circuit_Checker:: 
start_circuit_checking ()
{
  // do circuit checking operations here
  log_all_signals ();  
}

// callback functions
void LABSoft_Controller_Circuit_Checker:: 
cb_fl_button_choose_circuit_checker_file (Fl_Button *w,
                                                                             void      *data)
{
  if (!m_is_circuit_checker_running)
    {
      // show file chooser open
      m_file_chooser->show ();

      // wait while file chooser window is open
      while (m_file_chooser->shown ())
        Fl::wait ();

      if (m_file_chooser->value () == NULL)
        return;
      
      m_circuit_checker_filename = m_file_chooser->value ();
      parse_xml_file ();    
    }
  else  
    {
      log ("Cannot select new file. Wait for checking operation to finish.");
    }
}

void LABSoft_Controller_Circuit_Checker:: 
 cb_fl_button_start_circuit_checking (Fl_Button *w, 
                                                                         void      *data)
{
  if (!m_is_circuit_checker_running)
    {
      if (m_is_file_valid)
        {
          m_is_circuit_checker_running = true;
          log ("Circuit checking started.");
          start_circuit_checking ();

          m_is_circuit_checker_running = false;
          log ("Circuit checking ended.");
        }
      else 
        {
          log ("Cannot start circuit checking. Select a valid circuit checker file.");
        }
    }
}