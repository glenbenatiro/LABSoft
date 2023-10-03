#ifndef LABSOFT_CONTROLLER_UNIT_H
#define LABSOFT_CONTROLLER_UNIT_H

#include "../LAB/LAB.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"

// 
class LABSoft_Controller;

class LABSoft_Controller_Unit
{
  protected:
    LABSoft_Controller& m_controller;
  
  public:
    LABSoft_Controller_Unit (LABSoft_Controller& _LABSoft_Controller);

    // helper accessors
    LAB&          lab () const;
    LABSoft_GUI&  gui () const;
};


#endif