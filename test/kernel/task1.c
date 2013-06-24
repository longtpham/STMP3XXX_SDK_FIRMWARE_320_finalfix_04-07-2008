#include "kernel.h"
#include "messages.h"

SyncObject g_SyncObject1;
int g_i1=0;

_reentrant void Task1EntryPoint(void)
{
    while(1)
    {
        SysWait(100);
        SysPostMessage(5,0x010001,0x010002,0x010003,0x010004);
        SysPostMessage(4,0x010001,0x010002,0x010003);
        g_i1++;
        SysPostMessage(7,LCD_PRINT_NUMBER,0,8,g_i1,5,'0');

    }
}