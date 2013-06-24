#include "kernel.h"
#include "messages.h"

extern SyncObject g_SyncObject3;
int g_i4=0;

_reentrant void Task4EntryPoint(void)
{
    while(1)
    {
        SysWaitOnObject(-1,&g_SyncObject3);
        g_i4++;
        SysWait(100);
        SysReleaseObject(&g_SyncObject3);
        SysPostMessage(7,LCD_PRINT_NUMBER,0,32,g_i4,5,'0');

    }
}