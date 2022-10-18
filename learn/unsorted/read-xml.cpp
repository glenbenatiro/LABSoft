#include <iostream>

#include "pugixml.hpp"

int main ()
{
  std::cout << "Program start.\n";


  const char *source = "Activity1.labcc";

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file (source);

  if (result) 
    {
      std::cout << "XML file " << source << " parsed without errors.\n"; 
    }
  else 
    {
      std::cout << "XML file " << source << " parsed with errors.\n";
      std::cout << "Error description: " << result.description () << "\n";
      std::cout << "Error offset: " << result.offset << " (error at [..." << (source + result.offset) << "]\n\n";

    }

  pugi::xml_node ccfile = doc.child ("CircuitCheckerFile");

  for (pugi::xml_node signal = ccfile.child("Signal"); signal; signal = signal.next_sibling ("Signal"))
    {
      std::cout << "Output signal: " << signal.child_value ("OutputSignal") << "\n";
    }

  std::cout << "Program end.\n";

  return 0;
}