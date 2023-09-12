#ifndef LAB_MODULE_H
#define LAB_MODULE_H

#include "../../lib/AikaPi/AikaPi.h"

//
class LAB;

class LAB_Module
{
  protected:
    LAB& m_LAB;

  public:
    LAB_Module (LAB& _LAB);
   ~LAB_Module ();

   // shortcuts
   AikaPi& rpi () const;
   LAB& lab () const;
};

#endif