#ifndef LABSOFT_CONTROLLER_MAIN
#define LABSOFT_CONTROLLER_MAIN

#include <iostream>

#include <FL/Fl.H>

#include "LAB_Globals.h"
#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Main
{
  public:
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

    LABSoft_Controller_Main (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    static void LABSoft_Controller_Main_cb_update_display (void *data);
};

#endif