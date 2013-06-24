#include "kernel.h"
#include "messages.h"

SyncObject g_SyncObject2={(void*)1};
int g_i2=0;

_reentrant void Task2EntryPoint(void)
{
    while(1)
    {
        if(EVENT_OBJECT == SysWaitOnObject(100,&g_SyncObject2))
        {
            g_i2++;

            SysWaitOnObject(-1,&g_SyncObject2);
            SysReleaseObject(&g_SyncObject2);
            SysReleaseObject(&g_SyncObject2);
        }
        SysPostMessage(7,LCD_PRINT_NUMBER,0,16,g_i2,5,'0');
    }
}