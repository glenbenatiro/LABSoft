#include <stdexcept>
#include <iostream>
#include <unistd.h>


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
  
  m_thread_1 = nullptr;
     
  builder->get_widget("toggle_btn_panel_osc_enable", toggle_btn_panel_osc_enable);
  
  // Connect signals
  //btn_panel_menu1->signal_clicked().connect           (sigc::mem_fun (*this, &LABSoftAppWindow::on_btn_panel_menu1_clicked));  
  toggle_btn_panel_osc_enable->signal_toggled().connect (sigc::mem_fun(*this, &LABSoftAppWindow::on_toggle_btn_panel_osc_enable_toggled));
  
  // Connect idle signal
  // Glib::signal_idle().connect(sigc::mem_fun(_LABSoftOscDisplay, &LABSoftOscDisplay::run_osc_display));
  
  // Connect the handler to the dispatcher
 //  dispatcher.connect (sigc::mem_fun (*this, &LABSoftAppWindow::on_notification_from_worker_thread));
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
      // create fifo
      _LabInABox->fifo_create ();
      
      // IT IS IMPORTANT TO OPEN FIFO FOR READING FIRST BEFORE WRITING
      // open fifo for osc graphing
      _LABSoftOscDisplay->fifo_open_read ();
            
      // open fifo for adc writing
      //_LabInABox->fifo_open_write ();
      
      // spawn thread 1, set adc_streaming flag to true
      _LabInABox->m_flag_run_adc_streaming = true;
      m_thread_1 = new std::thread (&LabInABox::run_adc_streaming, _LabInABox);
      
      // spawn thread 2 and start reading from osc
      //_LABSoftOscDisplay->m_flag_run_osc_display = true;
      //m_thread_2 = new std::thread (&LABSoftOscDisplay::run_osc_display, _LABSoftOscDisplay);
                
      toggle_btn_panel_osc_enable->set_label ("Enabled");
      std::cout << "DEBUG: Button enabled.\n";
    }
  else
    {
      _LABSoftOscDisplay->m_flag_run_osc_display = false;
      _LabInABox->m_flag_run_adc_streaming       = false;
      
      // wait for threads to finish
      m_thread_1->join ();
      //m_thread_2->join ();
      
      delete m_thread_1;
      //delete m_thread_2;
      
      std::cout << "Threads joined\n";
            
      _LABSoftOscDisplay->fifo_close ();
      std::cout << "HELLO\n";
      _LabInABox->fifo_destroy ();
      std::cout << "HELLO2\n";
      
      toggle_btn_panel_osc_enable->set_label ("Disabled");
      std::cout << "DEBUG: Button disabled.\n";
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
  if (m_thread_1 && worker.has_stopped ())
    {
      // Work is done
      if (m_thread_1->joinable ())
        m_thread_1->join ();
      
      delete m_thread_1;
      
      m_thread_1 = nullptr;
    }
}

//void
//LABSoftAppWindow::on_btn_panel_menu1_clicked ()
//{
  //std::cout << "Button clicked!\n";
  //exit (0);
//}
