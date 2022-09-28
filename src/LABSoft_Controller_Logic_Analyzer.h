#ifndef LABSOFT_CONTROLLER_LOGIC_ANALYZER_H
#define LABSOFT_CONTROLLER_LOGIC_ANALYZER_H

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Logic_Analyzer
{
  public:
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

    LABSoft_Controller_Logic_Analyzer (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);
};

#endif