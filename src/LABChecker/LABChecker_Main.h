#ifndef LABCHECKER_MAIN
#define LABCHECKER_MAIN

#include <string>
#include <vector>

#include "pugixml.hpp"

class LABChecker_Main
{
  private:
    // FOR EXAMPLE ONLY! TECHNICALLY UNSECURE.
    std::string KEY = "thequickbrownfox"; 

  private:
    unsigned  binary_vector_to_decimal      (const std::vector<int>& bin_vec);
    void      save_xml_file                 (pugi::xml_document& doc, const std::string& save_path);
    void      encrypt_and_save              (pugi::xml_document& doc, const std::string& save_path);

  public:
    void create_file_digital (const std::vector<std::vector<char>>& inputs,
                              const std::vector<std::vector<char>>& outputs,
                              const std::string&                    file_path);
};

#endif