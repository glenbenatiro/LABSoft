#ifndef LABSOFT_H
#define LABSOFT_H

#include "Utility/Defaults.h"

#include "LAB/LAB.h"
#include "LABSoft_GUI/LABSoft_GUI.h"
#include "LABSoft_Controller/LABSoft_Controller.h"

class LABSoft 
{
  private:
    LAB                m_LAB;
    LABSoft_GUI        m_LABSoft_GUI;
    LABSoft_Controller m_LABSoft_Controller;
    
  public:
    LABSoft (int argc, char** argv);
   ~LABSoft ();
};

#endif