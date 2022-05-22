#include "LABSoftApp.h"

// compile with
// g++ main.cpp LABSoftApp.cpp LABSoftAppWindow.cpp LabInABox.cpp -o -Wno-narrowing LABSoft `pkg-config gtkmm-3.0 --cflags --libs`

int 
main (int   argc, 
      char *argv[])
{
  Glib::RefPtr<LABSoftApp> app = LABSoftApp::create ();

  return app->run(argc, argv);
}
