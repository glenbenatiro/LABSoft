#include "LABSoftApp.h"

// compile with
// g++ *.cpp -o LABSoft `pkg-config gtkmm-3.0 epoxy --cflags --libs`

int 
main (int   argc, 
      char *argv[])
{
  Glib::RefPtr<LABSoftApp> app = LABSoftApp::create ();

  return app->run(argc, argv);
}
