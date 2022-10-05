#include "LAB.h"

LAB::LAB ()
{
  m_LAB_Core               = new LAB_Core               ();
  m_LAB_Oscilloscope       = new LAB_Oscilloscope       ();
  m_Voltmeter          = new LAB_Voltmeter          ();
  m_Ammeter            = new LAB_Ammeter            ();
  m_Ohmmeter           = new LAB_Ohmmeter           ();
  m_Function_Generator = new LAB_Function_Generator ();
  m_Power_Supply       = new LAB_Power_Supply       ();
  m_LAB_Circuit_Checker    = new LAB_Circuit_Checker    ();
}

LAB::~LAB ()
{

}