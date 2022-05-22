#include <stdexcept>
#include <iostream>

#include "LABSoftAppWindow.h"

LABSoftAppWindow::LABSoftAppWindow (BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::ApplicationWindow(cobject)
{
  
}

void
LABSoftAppWindow::init (Glib::RefPtr<Gtk::Builder> _builder)
{
  builder = _builder;
  
  _LabInABox         = new LabInABox;
  _LABSoftOscDisplay = new LABSoftOscDisplay (builder);
  
  builder->get_widget("toggle_btn_panel_osc_enable", toggle_btn_panel_osc_enable);
  
  // Connect signals
  //btn_panel_menu1->signal_clicked().connect             (sigc::mem_fun (*this, &LABSoftAppWindow::on_btn_panel_menu1_clicked));  
  toggle_btn_panel_osc_enable->signal_toggled().connect (sigc::mem_fun(*this, &LABSoftAppWindow::on_toggle_btn_panel_osc_enable_toggled));
  
  // Connect idle signal
  Glib::signal_idle().connect(sigc::mem_fun(_LABSoftOscDisplay, &LABSoftOscDisplay::idle_handler));
  
  // Connect the handler to the dispatcher
  m_dispatcher.connect (sigc::mem_fun (*this, &LABSoftAppWindow::on_notification_from_worker_thread));
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
      
      if (m_worker_thread)
        {
          std::cout << "Can't start a worker thread while another one is running.\n";
        }
      else
        {
          // Start a new worker thread
          // m_worker_thread = new std::thread ([this]{m_worker.do_work(this);});
        }
    }
  else
    {
      toggle_btn_panel_osc_enable->set_label ("Disabled");
      
      if (!m_worker_thread)
        {
          std::cout << "Can't stop a worker thread. None is running.\n";
        }
      else
        {
          // Order the worker thread to stop working
          m_worker.stop_work ();
        }
    }
}

void
LABSoftAppWindow::notify ()
{
  m_dispatcher.emit ();
}

void 
LABSoftAppWindow::on_notification_from_worker_thread ()
{
  if (m_worker_thread && m_worker.has_stopped ())
    {
      // Work is done
      if (m_worker_thread->joinable ())
        m_worker_thread->join ();
      
      delete m_worker_thread;
      
      m_worker_thread = nullptr;
    }
}

//void
//LABSoftAppWindow::on_btn_panel_menu1_clicked ()
//{
  //std::cout << "Button clicked!\n";
  //exit (0);
//}
