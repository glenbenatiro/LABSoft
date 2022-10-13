#ifndef CHANNEL_SIGNALS_H
#define CHANNEL_SIGNALS_H

#include <vector>
#include "Channel_Signal.h"
#include "Defaults.h"

class Channel_Signals
{
  public:
    std::vector<Channel_Signal> m_channel_signal_vector;

    Channel_Signals (int number_of_channels = 1,
                     int number_of_samples  = 0,
                     int display_width = 0);
    
    int size ();
};

#endif