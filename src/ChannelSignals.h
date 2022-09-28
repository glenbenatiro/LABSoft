#ifndef CHANNEL_SIGNAL_H
#define CHANNEL_SIGNAL_H

#include <vector>

class ChannelSignal
{
  public:
    // variables
    bool  m_flag_is_enabled = true;

    int   number_of_samples;
    float m_sample_rate;  // hz

    std::vector<float>            m_samples;
    std::vector<std::vector<int>> m_pixel_points;

    // functions
    ChannelSignal (int number_of_samples)
    {
      m_samples.resize (number_of_samples, 0.0);
      // m_pixel_points.resize (length, std::vector<int> (2));
    }

    void resize (int length)
    {
      m_samples.resize (number_of_samples, 0.0);
      // m_pixel_points.resize (length, std::vector<int> (2));
    }
};

class ChannelSignals
{
  public:
    // variables
    int m_number_of_channels, 
        m_max_number_of_samples;

    std::vector<ChannelSignal> m_channel_signals;

    // functions
    ChannelSignals (int number_of_channels, 
                    int max_number_of_samples)
    {
      // load defaults
      m_number_of_channels    = number_of_channels;
      m_max_number_of_samples = max_number_of_samples;
    }

    ChannelSignals (int number_of_channels)
    {
      m_channel_signals.resize (number_of_channels, ChannelSignal (0));
    }
};

#endif