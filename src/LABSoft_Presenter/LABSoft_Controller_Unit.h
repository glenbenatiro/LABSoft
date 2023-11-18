#ifndef LABSOFT_CONTROLLER_UNIT_H
#define LABSOFT_CONTROLLER_UNIT_H

class LAB;
class LABSoft_GUI;
class LABSoft_Controller;

class LABSoft_Controller_Unit
{
  protected:
    LABSoft_Controller& m_controller;

  public:
    LABSoft_Controller_Unit (LABSoft_Controller& _LABSoft_Controller);

    LAB&                lab       () const;
    LABSoft_GUI&        gui       () const;
    LABSoft_Controller& presenter () const;
};

#endif