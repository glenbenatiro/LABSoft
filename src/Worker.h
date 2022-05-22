#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <mutex>

#include "LabInABox.h"

class LABSoftAppWindow;

class Worker
{
private:
  // Synchronizes access to member data
  mutable std::mutex m_mutex;
  
  // Data used by both GUI thread and worker thread
  bool m_shall_stop;
  bool m_has_stopped;
  // double m_fraction_done;
  // Glib::ustring m_message;
  
protected:
  
public:
  Worker ();
  
  // Thread function
  void do_work (LABSoftAppWindow *caller, LabInABox *labinabox);
  
  // void get_data (double* fraction_done, Glib::ustring *message) const;
  void stop_work ();
  bool has_stopped ();
};

#endif

// EOF
