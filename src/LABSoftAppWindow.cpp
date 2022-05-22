#include <stdexcept>
#include <iostream>

#include "LABSoftAppWindow.h"

LABSoftAppWindow::LABSoftAppWindow (BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::ApplicationWindow(cobject)
//: Gtk::ApplicationWindow(cobject), m_builder(builder)
{
  std::cout << "labsoftappwindow construcotr\n";
  
  //m_builder = Gtk::Builder::create_from_file ("../ui/lab.glade");
  
  builder->get_widget("toggle_btn_panel_osc_enable", toggle_btn_panel_osc_enable);
  
  //m_refBuilder->get_widget<Gtk::Button>("btn_panel_menu1", btn_panel_menu1);
  
  // Connect signals
  //btn_panel_menu1->signal_clicked().connect             (sigc::mem_fun (*this, &LABSoftAppWindow::on_btn_panel_menu1_clicked));  
  toggle_btn_panel_osc_enable->signal_toggled().connect (sigc::mem_fun(*this, &LABSoftAppWindow::on_toggle_btn_panel_osc_enable_toggled));
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
LABSoftAppWindow::on_toggle_btn_panel_osc_enable_toggled ()
{
  if (toggle_btn_panel_osc_enable->get_active ())
    {
      toggle_btn_panel_osc_enable->set_label ("Enabled");
      
      if (worker_thread)
        {
          std::cout << "Can't start a worker thread while another one is running.\n";
        }
      else
        {
          // Start a new worker thread
          //worker_thread = new std::thread ([this]{worker.do_work(this);});
        }
    }
  else
    {
      toggle_btn_panel_osc_enable->set_label ("Disabled");
    }
}


//void
//LABSoftAppWindow::on_btn_panel_menu1_clicked ()
//{
  //std::cout << "Button clicked!\n";
  //exit (0);
//}
