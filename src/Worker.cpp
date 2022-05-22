#include "Worker.h"

Worker::Worker ()
{
}

void
Worker::do_work (LABSoftAppWindow *caller)
{
 {
   std::lock_guard<std::mutex> lock (m_mutex);
   m_has_stopped = false;
   // m_fraction_done = 0.0;
   // m_message = "":
 } // The mutex is unlocked here by lock's destructor.
 
 // Do function here
 
 {
 }
 

}

void 
Worker::stop_work ()
{
  std::lock_guard<std::mutex> lock (m_mutex);
}
// EOF
