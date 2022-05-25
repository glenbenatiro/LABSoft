#include <iostream>
#include <chrono>

#include "Worker.h"
#include "LABSoftAppWindow.h"

Worker::Worker ()
: m_shall_stop  {false}, 
  m_has_stopped {false}
{
}

void
Worker::do_work (LABSoftAppWindow *caller, LabInABox *_LabInABox)
{ 
 {
   std::lock_guard<std::mutex> lock (m_mutex);
   m_has_stopped = false;
   // m_fraction_done = 0.0;
   // m_message = "":
 } // The mutex is unlocked here by lock's destructor.
 
  for (int i = 0; ; ++i) // do until break
    {
      // std::this_thread::sleep_for(std::chrono::milliseconds(10));
      
      // Do function here
      {
        std::lock_guard<std::mutex> lock (m_mutex);
         
        _LabInABox->do_streaming ();
      
        if (m_shall_stop)
          {
            break;
          }
      }
       
      caller->notify ();     
    }

  {
    std::lock_guard<std::mutex> lock (m_mutex);
    m_shall_stop  = false;
    m_has_stopped = true;
  }
  
  caller->notify ();
}

void 
Worker::stop_work () 
{
  std::lock_guard<std::mutex> lock (m_mutex);
  
  m_shall_stop = true;
}

bool
Worker::has_stopped () const
{
  std::lock_guard<std::mutex> lock (m_mutex);
  
  return m_has_stopped;
}
// EOF
