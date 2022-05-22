#ifndef LABSoftApp_H
#define LABSoftApp_H

#include <gtkmm/application.h>
#include <gtkmm/builder.h>

#include "LABSoftAppWindow.h"

class LABSoftApp : public Gtk::Application
{
private:
  LABSoftAppWindow           *appwindow;
  Glib::RefPtr<Gtk::Builder>  builder;
  
protected: 
  // Override default signal handlers:
  void on_activate() override;
  
public:
  LABSoftApp ();
  LABSoftAppWindow* create_appwindow ();
  static Glib::RefPtr<LABSoftApp> create ();  
  void on_hide_window (Gtk::Window *window);
};

#endif 

// EOF
