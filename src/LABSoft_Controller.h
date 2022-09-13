#ifndef LABSOFT_CONTROLLER_H
#define LABSOFT_CONTROLLER_H

#include "LAB.h"
#include "LABSoft_GUI.h"

#include <FL/Fl.H>

#include "LAB_Globals.h"
#include "LABSoft_Controller_Voltmeter.h"
#include "LABSoft_Controller_Main.h"

class LABSoft_Controller
{
  public:
    LAB                          *m_LAB;
    LABSoft_GUI                  *m_LABSoft_GUI;
    LABSoft_Controller_Voltmeter *m_LABSoft_Controller_Voltmeter;
    LABSoft_Controller_Main      *m_LABSoft_Controller_Main;

    LABSoft_Controller (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);
};

#endif