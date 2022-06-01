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
  
  worker_thread = nullptr;
     
  builder->get_widget("toggle_btn_panel_osc_enable", toggle_btn_panel_osc_enable);
  
  // Connect signals
  //btn_panel_menu1->signal_clicked().connect           (sigc::mem_fun (*this, &LABSoftAppWindow::on_btn_panel_menu1_clicked));  
  toggle_btn_panel_osc_enable->signal_toggled().connect (sigc::mem_fun(*this, &LABSoftAppWindow::on_toggle_btn_panel_osc_enable_toggled));
  
  // Connect idle signal
  Glib::signal_idle().connect(sigc::mem_fun(_LABSoftOscDisplay, &LABSoftOscDisplay::idle_handler));
  
  // Connect the handler to the dispatcher
  dispatcher.connect (sigc::mem_fun (*this, &LABSoftAppWindow::on_notification_from_worker_thread));
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
      std::cout << "DEBUG: Button enabled.\n";
      
      _LABSoftOscDisplay->m_flag_osc_update = true;
      
      toggle_btn_panel_osc_enable->set_label ("Enabled");
      
      // IT IS IMPORTANT to open fifo first before reading!
      // open fifo for adc writing
      _LABSoftOscDisplay->fifo_open_read ();
      
      // open fifo for adc writing
      _LabInABox->fifo_open_write ();
      
      // spawn thread 1, set streaming flag to true, and start adc (pwm_start)
      _LabInABox->m_flag_adc_stream = true;
      worker_thread = new std::thread (&LabInABox::do_streaming, _LabInABox);
      
      // spawn thread 2 and start reading from osc, OR just enable idle flag
      
      //if (worker_thread)
        //{
          //std::cout << "Can't start a worker thread while another one is running.\n";
        //}
      //else
        //{
          //// Begin LabInABox ADC stream
          
          //// Start a new worker thread
          //worker_thread = new std::thread ([this]{worker.do_work(this, _LabInABox);});
      }
  else
    {
      std::cout << "DEBUG: Button disabled.\n";
      
      _LABSoftOscDisplay->m_flag_osc_update = false;
      _LabInABox->m_flag_adc_stream = false;
      
      toggle_btn_panel_osc_enable->set_label ("Disabled");
      
      //if (!worker_thread)
        //{
          //std::cout << "Can't stop a worker thread. None is running.\n";
        //}
      //else
        //{
          //// Order the worker thread to stop and wait for it to stop
          //worker.stop_work ();
        //}
    }
}

void
LABSoftAppWindow::notify ()
{
  dispatcher.emit ();
}

void 
LABSoftAppWindow::on_notification_from_worker_thread ()
{
  if (worker_thread && worker.has_stopped ())
    {
      // Work is done
      if (worker_thread->joinable ())
        worker_thread->join ();
      
      delete worker_thread;
      
      worker_thread = nullptr;
    }
}

//void
//LABSoftAppWindow::on_btn_panel_menu1_clicked ()
//{
  //std::cout << "Button clicked!\n";
  //exit (0);
//}
