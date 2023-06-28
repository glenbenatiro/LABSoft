#ifndef LABCHECKER_MAIN
#define LABCHECKER_MAIN

#include <string>
#include <vector>

#include "pugixml.hpp"

class LABChecker_Main
{
  private:
    std::string getCurrentDateTimeString      ();
    unsigned    binary_vector_to_decimal      (const std::vector<int>& bin_vec);
    void        parse_char_vec_row_to_binary  (const std::vector<char>& char_vec, std::vector<int>& vec_bin);
    unsigned    parse_char_vec_row_to_int     (const std::vector<char>& char_vec_row);
    void        save_xml_file                 (pugi::xml_document& doc, const std::string& save_path);

  public:
    void create_file_digital (const std::vector<std::vector<char>>& inputs,
                              const std::vector<std::vector<char>>& outputs,
                              const std::string&                    file_path);
};

#endif