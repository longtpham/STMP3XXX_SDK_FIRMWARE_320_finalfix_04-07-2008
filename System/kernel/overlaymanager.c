#include "overlay.h"
#include "overlay_internal.h"
#include "SysResourceApi.h"

#ifndef OVERLAY_MANAGER_STACK_SIZE
#define OVERLAY_MANAGER_STACK_SIZE 500
#endif

_circ WORD g_OverlayManagerTaskMessageQueue[8];
CircularBufferDescriptor g_OverlayTaskQueueDescriptor={ g_OverlayManagerTaskMessageQueue,g_OverlayManagerTaskMessageQueue};

WORD g_OverlayManagerTaskStack[OVERLAY_MANAGER_STACK_SIZE];

void _reentrant OverlayManagerTask(void);


TaskEntry g_OverlayManagerTaskEntry = {&g_OverlayTaskQueueDescriptor,   //Message queue
                            NULL,                                       //Waiting on ptr
                            0,                                          //waiting on value
                            EVENT_TIMEOUT,                              //Initial State
                            g_OverlayManagerTaskStack,                  //current stack ptr
                            0,                                          //Task ID
                            TIMEOUT_IMMEDIATE,                          //Initial timeout value
                            {0,0},                                      //Required regions (none-required)
                            255,                                        //Priority (make it higher than any others)
                            g_OverlayManagerTaskStack,                  //initial stack pointer
                            OVERLAY_MANAGER_STACK_SIZE,                 //stack size
                            OverlayManagerTask};                        //initial entry point


int glb_iLoadCount = 0;


void _reentrant LoadCode(WORD wResource, MemoryRegion* pRegion)
{
    if(SysLoadResource(wResource,RS_ROOT_HANDLE,RSRC_TYPE_DATA,(void*)pRegion->m_StartAddress,TARGET_MEM_P,0xffff))
        SystemHalt();
    pRegion->m_wResourceLoaded = wResource;

}

void _reentrant OverlayManagerTask(void)
{
    while(1)
    {
        SysWaitOnSignal(-1,&g_OverlaySignal,1);
        if(g_pTaskToLoad)
        {
            MemoryRegion **pRegionTable= g_RegionTable;
            WORD          *wRequiredResource = g_pTaskToLoad->m_wRegionResources;
            while(*pRegionTable)
            {
                if(*wRequiredResource)
                {
                    if(*wRequiredResource != (*pRegionTable)->m_wResourceLoaded)
                    {
                        glb_iLoadCount++;
                        LoadCode(*wRequiredResource, *pRegionTable);
                    }
                }
                pRegionTable++;
                wRequiredResource ++;
            }
            g_pTaskToLoad = NULL;
			glb_iLoadCount = 0;
        }
    }
}



void _reentrant OverlayManager(void)
{

    TaskEntry * Save_Task;

    if(g_pCurrentTask)
    {
        MemoryRegion **pRegionTable= g_RegionTable;
        WORD          *wRequiredResource = g_pCurrentTask->m_wRegionResources;
        
        while(*pRegionTable)
        {
            if(*wRequiredResource)
			{
                if(*wRequiredResource != (*pRegionTable)->m_wResourceLoaded)
				{
                    Save_Task = g_pCurrentTask;
					g_pCurrentTask = NULL;
                    LoadCode(*wRequiredResource, *pRegionTable);
					g_pCurrentTask = Save_Task; 
				}
			}

            pRegionTable++;
            wRequiredResource ++;
        }
    }
}

