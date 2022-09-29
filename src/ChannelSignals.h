#ifndef CHANNEL_SIGNAL_H
#define CHANNEL_SIGNAL_H

#include <vector>

class ChannelSignal
{
  private:
    // variables
    bool  m_is_enabled = true;
    int   m_number_of_samples;
    float m_sample_rate;  // hz
    int   m_screen_width = 1366; // 1366 pixels, just guessing

    std::vector<float>            *m_samples;
    std::vector<std::vector<int>> *m_pixel_points;
  
  public:
    // functions
    ChannelSignal (int number_of_samples)
    {
      //
      m_samples = new std::vector<float>;
      m_samples->reserve (number_of_samples);

      m_pixel_points = new std::vector<std::vector<int>>;
      m_pixel_points->resize (m_screen_width, std::vector<int> (2, 0));
    }

    // getters
    bool is_enabled ()
    {
      return m_is_enabled;
    }

    std::vector<std::vector<int>>* pixel_points ()
    {
      return m_pixel_points;
    }

    std::vector<float>* samples ()
    {
      return m_samples;
    }
};

class ChannelSignals
{
  private:
    // variables
    bool m_is_sample_sharing_enabled;
    int m_number_of_channels,
        m_max_number_of_samples;

    std::vector<ChannelSignal> *m_channel_signal_vector;

  public:
    // functions
    ChannelSignals (int number_of_channels, 
                    int max_number_of_samples,
                    bool enable_sample_sharing)
    {
      // load defaults
      m_number_of_channels        = number_of_channels;
      m_max_number_of_samples     = max_number_of_samples;
      m_is_sample_sharing_enabled = enable_sample_sharing;

      if (enable_sample_sharing)
        m_channel_signal_vector = new std::vector<ChannelSignal> (number_of_channels, ChannelSignal (max_number_of_samples / number_of_channels));
      else 
        m_channel_signal_vector = new std::vector<ChannelSignal> (number_of_channels, ChannelSignal (max_number_of_samples));
    }

  // getters
  int number_of_channels ()
  {
    if (m_number_of_channels == m_channel_signal_vector->size ())
      return m_number_of_channels;
    else
      return -1;
  }

  std::vector<ChannelSignal>* channel_signal_vector ()
  {
    return m_channel_signal_vector;
  }

  // setters
  void number_of_channels (int value) // does not change m_channel_signal_vector size
  {
    m_number_of_channels = value;
  }

  void number_of_channels_all (int value) // also changes m_channel_signal_vector size
  {
    m_number_of_channels = value;
    
    // ! expensive operation
    m_channel_signal_vector->resize (m_number_of_channels, m_is_sample_sharing_enabled ? 
        (m_max_number_of_samples / m_number_of_channels) : (m_max_number_of_samples));
  }
};

#endif