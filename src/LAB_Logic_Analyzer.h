#ifndef LAB_LOGIC_ANALYZER_H
#define LAB_LOGIC_ANALYZER_H

#include "LAB_Core.h"
#include "Channel_Signals.h"

class LAB_Logic_Analyzer
{
  private:
    bool m_is_running = false;
    unsigned m_memory_depth = 4096;

    int m_number_of_channels = 4;

    LAB_Core *m_LAB_Core;

    Channel_Signal m_channel_signals [LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS];

  public:
    LAB_Logic_Analyzer (LAB_Core *_LAB_Core);

    void master_run ();
    void master_stop ();
    void init_pins ();

    // setter

    // getter
    void memory_depth (unsigned value);

};

#endif