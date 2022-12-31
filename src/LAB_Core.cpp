#include "LAB_Core.h"

LAB_Core:: 
LAB_Core () : AikaPi ()
{
  map_uncached_mem (&m_vc_mem, VC_MEM_SIZE);
}