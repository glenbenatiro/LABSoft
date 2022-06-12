#ifndef LABSOFTAPPWINDOW_H
#define LABSOFTAPPWINDOW_H

#include <gtkmm/builder.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/button.h>
#include <glibmm/dispatcher.h>
#include <thread>

#include "Worker.h"
#include "LABSoftOscDisplay.h"
#include "LabInABox.h"

class LABSoftAppWindow : public Gtk::ApplicationWindow
{
private: 
  LabInABox         *_LabInABox;
  LABSoftOscDisplay *_LABSoftOscDisplay;
  
  Gtk::Button 			*btn_panel_menu1;
  Gtk::ToggleButton *toggle_btn_panel_osc_enable;
    
  Worker            worker;
  std::thread       *m_thread_1;
  std::thread       *m_thread_2;
  Glib::Dispatcher  dispatcher;
    
  // Dispatcher handler
  void on_notification_from_worker_thread ();
  
protected:
  
public:
  Glib::RefPtr<Gtk::Builder> builder;
  void init (Glib::RefPtr<Gtk::Builder> _builder);

  LABSoftAppWindow (BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
  static LABSoftAppWindow *create (Glib::RefPtr<Gtk::Builder> builder);
  
   // Called from the worker thread
  void notify ();
  
  // --- Signal Handlers ---
  void on_toggle_btn_panel_osc_enable_toggled ();
  
  //void on_btn_panel_menu1_clicked ();
};

#endif

// EOF
