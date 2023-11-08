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
  // https://stackoverflow.com/a/17341

  return (std::abs (a - b) < epsilon);
}

bool LABF:: 
is_greater_than (double value,
                 double reference,
                 double epsilon)
{
  // https://stackoverflow.com/a/253874

  return ((value - reference) > ((std::abs (value) < std::abs (reference) ? 
    std::abs (reference) : std::abs (value)) * epsilon));
}


bool LABF:: 
is_less_than (double value,
              double reference,
              double epsilon)
{
  // https://stackoverflow.com/a/253874

  return ((reference - value) > ((std::abs (value) < std::abs (reference) ? 
    std::abs (reference) : std::abs (value)) * epsilon));
}

bool LABF:: 
is_greater_than_or_equal_to (double value,
                             double reference,
                             double epsilon)
{
  return (is_greater_than (value, reference, epsilon) && is_equal (value, reference, epsilon));
}

bool LABF:: 
is_less_than_or_equal_to (double value,
                          double reference,
                          double epsilon)
{
  return (is_less_than (value, reference, epsilon) && is_equal (value, reference, epsilon));
}

bool LABF:: 
is_within_range (double value,
                 double min,
                 double max,
                 double epsilon)
{
  return (
    (is_less_than (value, max, epsilon)     && 
    is_greater_than (value, min, epsilon))  || 
    is_equal (value, min, epsilon)          || 
    is_equal (value, max, epsilon)
  );
}

double LABF::
normalize (double input, 
           double min_input, 
           double max_input, 
           double min_output, 
           double max_output)
{
  return (((input - min_input) / (max_input - min_input)) * 
    (max_output - min_output) + min_output);
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