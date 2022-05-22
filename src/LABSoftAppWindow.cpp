#include <stdexcept>
#include <iostream>

#include "LABSoftAppWindow.h"

LABSoftAppWindow::LABSoftAppWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
: Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder)
{
  m_refBuilder = Gtk::Builder::create_from_file("../ui/lab.glade");
  
  m_refBuilder->get_widget("toggle_btn_panel_osc_enable", toggle_btn_panel_osc_enable);
  m_refBuilder->get_widget<Gtk::Button>("btn_panel_menu1", btn_panel_menu1);
  
  btn_panel_menu1->signal_clicked().connect             (sigc::mem_fun(*this, &LABSoftAppWindow::on_btn_panel_menu1_clicked));  
  toggle_btn_panel_osc_enable->signal_toggled().connect (sigc::mem_fun(*this, &LABSoftAppWindow::on_toggle_btn_panel_osc_enable_toggle));
}

// Static
LABSoftAppWindow*
LABSoftAppWindow::create (Glib::RefPtr<Gtk::Builder> builder)
{
  LABSoftAppWindow* window = nullptr;
  builder->get_widget_derived("window", window);
  
  if (!window)
    throw std::runtime_error ("No \"app_window\" object in glade.ui\n");
    
  return window;
}

// --- SIGNAL HANDLERS ---
void 
LABSoftAppWindow::on_toggle_btn_panel_osc_enable_toggle ()
{
  std::cout << "button clicked!\n";
}
// EOF

void
LABSoftAppWindow::on_btn_panel_menu1_clicked ()
{
  std::cout << "Button clicked!\n";
  exit (0);
}
