#include "LAB_Ohmmeter.h"

LAB_Ohmmeter:: 
LAB_Ohmmeter (LAB& _LAB)
  : LAB_Module (_LAB)
{

}

bool LAB_Ohmmeter:: 
is_running () const 
{
  return (m_is_running);
}