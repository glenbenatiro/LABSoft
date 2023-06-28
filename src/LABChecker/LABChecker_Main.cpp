#include "LABChecker_Main.h"

#include <chrono>
#include <string>
#include <iomanip>
#include <iostream>

std::string LABChecker_Main:: 
getCurrentDateTimeString ()
{
  // // Get the current system time
  // auto now = std::chrono::system_clock::now();

  // // Convert the system time to a time point
  // std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

  // // Convert the time point to a local time struct
  // std::tm* localTime = std::localtime(&currentTime);

  // // Format the date and time
  // std::ostringstream oss;
  // oss << std::put_time(localTime, "%m/%d/%y %H:%M");
  // std::string dateTimeStr = oss.str();

  // return dateTimeStr;
}

unsigned LABChecker_Main:: 
binary_vector_to_decimal (const std::vector<int>& bin_vec)
{
  unsigned decimal  = 0;
  unsigned base     = 1;

  for (std::vector<int>::const_iterator it = bin_vec.begin ();
    it != bin_vec.end (); ++it)
  {
    decimal += (*it) * base;
    base *= 2;
  }
  
  return (decimal);
}

void LABChecker_Main:: 
parse_char_vec_row_to_binary (const std::vector<char>& char_vec,
                                    std::vector<int>&  bin_vec)
{
  bin_vec.resize (char_vec.size ());

  for (size_t col = 0; col < char_vec.size (); col++)
  {
    char c = char_vec[col];

    if (c == 'X' || c == '0')
    {
      bin_vec[col] = 0;
    }
    else if (c == '1')
    {
      bin_vec[col] = 1;
    }
    else 
    {
      throw (std::domain_error ("Invalid value found in char to binary conversion."));
    }
  }
}

unsigned LABChecker_Main:: 
parse_char_vec_row_to_int (const std::vector<char>& char_vec_row)
{
  std::vector<int> bin_vec;

  parse_char_vec_row_to_binary (char_vec_row, bin_vec);

  return (binary_vector_to_decimal (bin_vec));
}

void LABChecker_Main:: 
save_xml_file (       pugi::xml_document& doc,
               const  std::string&        save_path)
{
  std::string extension = ".xml";
  std::string path      = save_path;

  if (save_path.length () < extension.length () || save_path.compare (
    save_path.length () - extension.length (), extension.length (),
    extension) != 0)
  {
    path += extension;
  }

  doc.save_file (path.c_str ());
}

void LABChecker_Main:: 
create_file_digital (const std::vector<std::vector<char>>& inputs,
                     const std::vector<std::vector<char>>& outputs,
                     const std::string&                    save_path)
{
  pugi::xml_document  doc;
  pugi::xml_node      root = doc.append_child ("root");
  pugi::xml_node      data = root.append_child ("data");

  for (size_t row = 0; row < inputs.size (); ++row)
  {
    // 1. Node
    pugi::xml_node data_pair = data.append_child ("data_pair");
  
    // 2. Input
    unsigned input_val = parse_char_vec_row_to_int (inputs[row]);
    
    pugi::xml_node input_node = data_pair.append_child ("input");

    input_node.append_child (pugi::node_pcdata).set_value (
      std::to_string (input_val).c_str ());

    // 3. Output
    unsigned output_val = parse_char_vec_row_to_int (outputs[row]);

    pugi::xml_node output_node = data_pair.append_child ("output");

    output_node.append_child (pugi::node_pcdata).set_value (
      std::to_string (output_val).c_str ());  
  }  

  save_xml_file (doc, save_path);
}

// EOF
