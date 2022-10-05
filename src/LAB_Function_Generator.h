#ifndef LAB_FUNCTION_GENERATOR
#define LAB_FUNCTION_GENERATOR

#include "LAB_Core.h"

#include "Channel_Signals.h"

class LAB_Function_Generator : public LAB_Core
{
  private:
    bool  m_is_running = false;
    int   m_number_of_channels;

    Channel_Signals *m_channel_signals;

  public:
    // --- functions ---
          LAB_Function_Generator ();
    void  update ();

    // getters
    bool  is_running ();
    
    Channel_Signals* channel_signals ();

    // setters
    void  is_running (bool value);
};

#endif
