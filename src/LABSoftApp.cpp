#include <exception>
#include <iostream>

#include "LABSoftApp.h"

Glib::RefPtr<LABSoftApp> 
LABSoftApp::create ()
{
  return (Glib::RefPtr<LABSoftApp> (new LABSoftApp ()));
}

LABSoftApp::LABSoftApp ()
: Gtk::Application ("com.github.glenbenatiro.labsoft", Gio::APPLICATION_FLAGS_NONE)
{

}

void 
LABSoftApp::on_activate ()
{ 
  builder             = Gtk::Builder::create_from_file ("../ui/lab.glade");
  
  m_LabInABox         = new LabInABox;
  m_LABSoftOscDisplay = new LABSoftOscDisplay (builder);
  
  Glib::signal_idle().connect(sigc::mem_fun(m_LABSoftOscDisplay, &LABSoftOscDisplay::idle_handler));
  
  try
  {
    // The application has been started, so let's show a window.
    appwindow = create_appwindow (builder);
  }
  // If create_appwindow() throws an exception (perhaps from Gtk::Builder),
  // no window has been created, no window has been added to the application,
  // and therefore the application will stop running.
  catch (const Glib::Error& ex)
  {
    std::cerr << "LABSoftApp::on_activate(): " << ex.what() << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cerr << "LABSoftApp::on_activate(): " << ex.what() << std::endl;
  }
  
  appwindow->present();
}

LABSoftAppWindow* 
LABSoftApp::create_appwindow (Glib::RefPtr<Gtk::Builder> builder)
{
  LABSoftAppWindow* appwindow = LABSoftAppWindow::create (builder);
  
  // Make sure that the application runs for as long this window is still open.
  add_window(*appwindow);

  // Gtk::Application::add_window() connects a signal handler to the window's
  // signal_hide(). That handler removes the window from the application.
  // If it's the last window to be removed, the application stops running.
  // Gtk::Window::set_application() does not connect a signal handler, but is
  // otherwise equivalent to Gtk::Application::add_window().

  // Delete the window when it is hidden.
  appwindow->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this,
    &LABSoftApp::on_hide_window), appwindow));

  return appwindow;
}

LABSoftApp::~LABSoftApp ()
{
  m_LABSoftOscDisplay->graph_free ();
}

void 
LABSoftApp::on_hide_window (Gtk::Window *window)
{
  delete window;
}
