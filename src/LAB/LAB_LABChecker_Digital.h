#ifndef LAB_LABCHECKER_DIGITAL
#define LAB_LABCHECKER_DIGITAL

#include <string>
#include <vector>

#include "LAB_Core.h"
#include "../Utility/pugixml.hpp"

// forward declare LAB, #include in .cpp file
class LAB;

class LAB_LABChecker_Digital
{
  private:
    LAB*      m_LAB;
    LAB_Core* m_LAB_Core;

    // FOR EXAMPLE ONLY! TECHNICALLY UNSECURE.
    std::string KEY = "thequickbrownfox"; 

  private:
    unsigned  binary_vector_to_decimal      (const std::vector<int>& bin_vec);
    void      save_xml_file                 (pugi::xml_document& doc, const std::string& save_path);
    void      encrypt_and_save              (pugi::xml_document& doc, const std::string& save_path);

  public:
    LAB_LABChecker_Digital (LAB_Core* _LAB_Core, LAB* _LAB);

    void create_file_digital (const std::vector<std::vector<char>>& inputs,
                              const std::vector<std::vector<char>>& outputs,
                              const std::string&                    file_path);
};

#endif