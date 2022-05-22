#ifndef LABSOFTAPPWINDOW_H
#define LABSOFTAPPWINDOW_H

#include <gtkmm/builder.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/button.h>
#include <glibmm/dispatcher.h>
#include <thread>

#include "LABSoftOscDisplay.h"
//#include "Worker.h"

class LABSoftAppWindow : public Gtk::ApplicationWindow
{
private: 
  LABSoftOscDisplay *glarea_disp_graph;
  Gtk::Button 			*btn_panel_menu1;
  Gtk::ToggleButton *toggle_btn_panel_osc_enable;
  
  std::thread       *worker_thread;
  //Worker            worker;
  Glib::Dispatcher  dispatcher;
  
protected:
  
public:
  LABSoftAppWindow (BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
  static LABSoftAppWindow *create (Glib::RefPtr<Gtk::Builder> builder);
  
  // --- Signal Handlers ---
  void on_toggle_btn_panel_osc_enable_toggled ();
  
  //void on_btn_panel_menu1_clicked ();
};

#endif

// EOF
