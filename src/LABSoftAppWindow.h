#ifndef LABSOFTAPPWINDOW_H
#define LABSOFTAPPWINDOW_H

#include <gtkmm/builder.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/button.h>

#include "LABSoftOscDisplay.h"

class LABSoftAppWindow : public Gtk::ApplicationWindow
{
private: 
  LABSoftOscDisplay          *glarea_disp_graph;
  Glib::RefPtr<Gtk::Builder>  m_refBuilder;
  
protected:
  
  

public:
  LABSoftAppWindow (BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
  static LABSoftAppWindow *create (Glib::RefPtr<Gtk::Builder> builder);
  Gtk::Button* btn_panel_menu1;
  
   Gtk::ToggleButton *toggle_btn_panel_osc_enable;
  
  // --- Signal Handlers ---
  void on_toggle_btn_panel_osc_enable_toggle ();
  void on_btn_panel_menu1_clicked ();
};

#endif
