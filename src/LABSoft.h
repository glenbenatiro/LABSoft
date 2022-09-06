#ifndef LABSOFT_H
#define LABSOFT_H

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft 
{
  private:
    LAB   *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

  public:
    LABSoft (int argc, char** argv);
   ~LABSoft ();
};

#endif