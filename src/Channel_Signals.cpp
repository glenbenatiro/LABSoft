#include "Channel_Signals.h"

Channel_Signal:: 
Channel_Signal (unsigned number_of_samples, 
                unsigned display_width)
: m_values        (number_of_samples, 0.0),
  m_pixel_points  (number_of_samples, std::vector<int> (display_width, 0))
{
 
}

void Channel_Signal:: 
enable ()
{
  m_is_enabled = true;
}

void Channel_Signal:: 
disable ()
{
  m_is_enabled = false;
}

bool Channel_Signal:: 
is_enabled ()
{
  return m_is_enabled;
}

void Channel_Signal:: 
volts_per_division (double value)
{
  m_voltage_per_division = value;
}

void Channel_Signal:: 
vertical_offset (double value)
{
  m_vertical_offset = value;
}

double Channel_Signal:: 
voltage_per_division ()
{
  return m_voltage_per_division;
}

double Channel_Signal:: 
vertical_offset ()
{
  return m_vertical_offset;
}

double Channel_Signal:: 
time_per_division ()
{
  return m_time_per_division;
}

double Channel_Signal::
horizontal_offset ()
{
  return m_horizontal_offset;
}

unsigned Channel_Signal:: 
working_samples ()
{
  return m_working_samples;
}

// --- Channel_Signals ---

Channel_Signals:: 
Channel_Signals (unsigned number_of_channels,
                 unsigned number_of_samples,
                 unsigned display_width)
: m_chans (number_of_channels, Channel_Signal (number_of_samples, display_width))
{
  m_working_samples = number_of_samples;
}

Channel_Signals:: 
~Channel_Signals ()
{

}

int Channel_Signals:: 
size ()
{
  return m_chans.size ();
}

