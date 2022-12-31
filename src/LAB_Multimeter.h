
#ifndef LAB_MULTIMETER
#define LAB_MULTIMETER

#include "LAB_Core.h"
#include "Channel_Signals.h"

class LAB_Multimeter
{
  private:
    LAB_Core *m_LAB_Core;

    bool m_is_running = false;
    bool m_is_random_value_generator_running = false;
    Channel_Signals m_channel_signals;
 
  public:
      LAB_Multimeter (LAB_Core *_LAB_Core);

    // 
    void run ();
    void stop ();
    bool is_running ();
    bool is_random_value_generator_running ();
    void run_random_value_generator ();
    void stop_random_value_generator ();
    unsigned get_sample ();

  // public:
  //   bool  m_is_running = false,
  //         m_is_random_value_generator_running = false;

  //   float m_value;

  
  //   // functions  
  //         LAB_Meter ();
  //   void  random_value_generator ();

  //   // getters
  //   bool  is_running ();
  //   bool  is_random_value_generator_running ();
  //   float value ();

  //   // setters
  //   void  is_running (bool value);
  //   void  is_random_value_generator_running (bool value);
  //   void  value (float value);
};

#endif
