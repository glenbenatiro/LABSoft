#ifndef LABS_GUI_VALUES_H
#define LABS_GUI_VALUES_H

#include <map>
#include <string>

#include "LAB_Enumerations.h"

namespace LABS_GUI_VALUES
{
  namespace OSC
  {
    static std::map<LABE::OSC::SCALING, std::string> SCALING = 
    {
      {LABE::OSC::SCALING::QUADRUPLE, "x4"},
      {LABE::OSC::SCALING::UNITY,     "x1"},
      {LABE::OSC::SCALING::HALF,      "x0.5"},
      {LABE::OSC::SCALING::FOURTH,    "x0.25"}
    };

    static std::map<LABE::OSC::MODE, std::string> MODE = 
    {
      {LABE::OSC::MODE::SCREEN,   "Screen"},
      {LABE::OSC::MODE::REPEATED, "Repeated"}
    };
  }

  namespace FUNC_GEN
  {
    static std::map<LABE::FUNC_GEN::WAVE_TYPE, std::string> WAVE_TYPE = 
    {
      {LABE::FUNC_GEN::WAVE_TYPE::DC,       "DC"},
      {LABE::FUNC_GEN::WAVE_TYPE::SINE,     "Sine"},
      {LABE::FUNC_GEN::WAVE_TYPE::SQUARE,   "Square"},
      {LABE::FUNC_GEN::WAVE_TYPE::TRIANGLE, "Triangle"}
    };
  };

  namespace LOGAN
  {
    static std::map<std::string, LABE::LOGAN::TRIG::MODE> TRIG_MODE_s = 
    {
      {"None",    LABE::LOGAN::TRIG::MODE::NONE},
      {"Normal",  LABE::LOGAN::TRIG::MODE::NORMAL},
      {"Auto",    LABE::LOGAN::TRIG::MODE::AUTO}
    };
  }
}

#endif