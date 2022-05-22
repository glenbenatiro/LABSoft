#ifndef LABSoftApp_H
#define LABSoftApp_H

#include <gtkmm/application.h>
#include <gtkmm/builder.h>

#include "LabInABox.h"
#include "LABSoftOscDisplay.h"
#include "LABSoftAppWindow.h"

class LABSoftApp : public Gtk::Application
{
private:
  LabInABox*          m_LabInABox;
  LABSoftOscDisplay*  m_LABSoftOscDisplay;
  LABSoftAppWindow*   appwindow;
  Glib::RefPtr<Gtk::Builder> builder;
  
protected: 
  // Override default signal handlers:
  void on_activate() override;
  
public:
  LABSoftApp ();
  ~LABSoftApp ();
  
  LABSoftAppWindow* create_appwindow (Glib::RefPtr<Gtk::Builder> builder);
  static Glib::RefPtr<LABSoftApp> create ();  
  void on_hide_window (Gtk::Window *window);
};

#endif 
