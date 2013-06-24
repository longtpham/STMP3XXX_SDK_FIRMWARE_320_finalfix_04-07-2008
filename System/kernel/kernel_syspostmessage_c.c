#include "kernel.h"
#include "kernel_internal.h"

#pragma optimize 3
_reentrant WORD Kernel_SysPostMessage(Message *pMessage)
{
    void        *pTester;
    RoutingEntry *pEntry= g_RoutingTable;
    WORD wMessageType = pMessage->m_wMessage & 0xff0000;
    //pTester is here because if you use pEntry in the while() evaluator, the compiler barfs for unknown reasons
    pTester = pEntry;
    while(pTester)
    {   
        if(pEntry->m_wMessageType == wMessageType)
            break;
        pEntry++;
        pTester = pEntry;
    }
    assert(pEntry);
    return Kernel_AddMessageToQueue(pEntry->m_pDestination,pMessage);
}


