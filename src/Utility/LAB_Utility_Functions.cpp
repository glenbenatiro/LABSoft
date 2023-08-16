#include "LAB_Utility_Functions.h"

#include <cmath>
#include <chrono>
#include <iomanip>
#include <sstream>

bool LABF::
is_equal (double a,
          double b,
          double epsilon)
{
  return ((std::abs (a - b)) < epsilon);
}

bool LABF:: 
is_within_range (double value,
                 double min,
                 double max)
{
  if ((value <= max && value >= min) || is_equal (value, min) || 
      is_equal (value, max))
  {
    return (true);
  }
  else 
  {
    return (false);
  }
}

int LABF::
normalize (int input, 
           int min_input, 
           int max_input, 
           int min_output, 
           int max_output)
{
  return (min_output + (((input - min_input) * (max_output - min_output)) / 
    (max_input - min_input)));
}

std::string LABF::
get_now_timestamp ()
{
  // x.
  std::chrono::time_point<std::chrono::system_clock> current_time = 
    std::chrono::system_clock::now ();
  
  // x.
  std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>
      current_time_ms = std::chrono::time_point_cast<std::chrono::milliseconds> (current_time);
  
  // x.
  std::time_t time_time_t = std::chrono::system_clock::to_time_t (current_time);

  // x. 
  std::tm* time_tm = std::localtime (&time_time_t);
  
  // x. skip milliseconds for now
  // int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
  //   (current_time - current_time_ms).count ();
  
  std::stringstream ss;

  ss << std::put_time (time_tm, "%Y-%m-%d %H:%M:%S");
  // ss << "." << std::setw (3) << std::setfill ('0') << milliseconds;

  return (ss.str ());
}
