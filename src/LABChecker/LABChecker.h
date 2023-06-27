#ifndef LABCHECKER_H
#define LABCHECKER_H

#include "LABChecker_Main.h"
#include "LABChecker_GUI.h"
#include "LABChecker_Controller.h"

class LABChecker
{
  private:
    LABChecker_Main       m_LABChecker_Main;
    LABChecker_GUI        m_LABChecker_GUI;
    LABChecker_Controller m_LABChecker_Controller;

  public:
    LABChecker ();
   ~LABChecker ();
};

#endif