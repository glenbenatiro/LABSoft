#include "LABSoft_Controller_Unit.h"

#include "LABSoft_Controller.h"

LABSoft_Controller_Unit:: 
LABSoft_Controller_Unit (LABSoft_Controller& _LABSoft_Controller)
  : m_controller (_LABSoft_Controller)
{

}

LAB& LABSoft_Controller_Unit:: 
lab () const
{
  return (m_controller.lab ());
}

LABSoft_GUI& LABSoft_Controller_Unit:: 
gui () const
{
  return (m_controller.gui ());
}
