#include "kernel.h"
#include "messages.h"

SyncObject g_SyncObject3;
int     g_i3=0;
_reentrant void Task3EntryPoint(void)
{
    while(1)
    {
        SysWaitOnObject(-1,&g_SyncObject3);
        g_i3++;
        SysWait(100);
        SysReleaseObject(&g_SyncObject3);
        SysPostMessage(7,LCD_PRINT_NUMBER,0,24,g_i3,5,'0');

    }
}