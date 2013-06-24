
#include "kernel.h"
#include "kernel_internal.h"

//This function is a bit wierd.  It assumes that we're in an ISR, so it does not return to the kernel

_reentrant void ISRReleaseObject(SyncObject *pObject)
{
    // assert(pObject->m_pOwner==ISR_OWNER);
    if(pObject->m_pOwner==ISR_OWNER)
    	pObject->m_pOwner = NULL;
}
