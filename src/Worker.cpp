#include "Worker.h"

Worker::Worker ()
{
}

void
Worker::do_work (LABSoftAppWindow *caller, LabInABox *labinabox)
{
 {
   std::lock_guard<std::mutex> lock (m_mutex);
   m_has_stopped = false;
   // m_fraction_done = 0.0;
   // m_message = "":
 } // The mutex is unlocked here by lock's destructor.
 
 for (int i = 0; ; ++i)
   {
     // Do function here
     {
       std::lock_guard<std::mutex> lock (m_mutex);
       
       labinabox->do_streaming ();
       
       if (m_shall_stop)
       {
         break;
       }
     }
     
    
    //caller->notify ();
   }

  //caller->notify ();
}

void 
Worker::stop_work ()
{
  std::lock_guard<std::mutex> lock (m_mutex);
}

bool
Worker::has_stopped ()
{
  std::lock_guard<std::mutex> lock (m_mutex);
  return m_has_stopped;
}
// EOF
