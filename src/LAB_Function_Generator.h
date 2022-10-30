#ifndef LAB_FUNCTION_GENERATOR
#define LAB_FUNCTION_GENERATOR

#include "LAB_Core.h"

#include "Channel_Signals.h"

class LAB_Function_Generator 
{
  public:
    bool  m_is_running = false;

    Channel_Signals m_channel_signals;

    // --- functions ---
          LAB_Function_Generator ();
    void  update ();
};

#endif
