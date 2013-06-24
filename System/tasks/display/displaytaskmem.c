#include "kernel.h"
#include "resource.h"
#include <project.h>

_reentrant void DisplayTask(void);
//_circ WORD g_DisplayTaskMessageQueue[256]; // was 32
_circ WORD g_DisplayTaskMessageQueue[254]; // changed to 254 because with AUDIBLEDEC, sdkbuildall could not pass in some combination
CircularBufferDescriptor g_DisplayTaskQueueDescriptor={ g_DisplayTaskMessageQueue,g_DisplayTaskMessageQueue};

#ifndef DISPLAY_TASK_STACK_SIZE
//#define DISPLAY_TASK_STACK_SIZE 256
#define DISPLAY_TASK_STACK_SIZE 254 // changed to 254 because with AUDIBLEDEC, sdkbuildall could not pass in some combination
#endif 
extern WORD g_DisplayTaskStack[DISPLAY_TASK_STACK_SIZE];
#pragma asm
;Fg_DisplayTaskStack ds 256-1
Fg_DisplayTaskStack ds 254-1    ;changed to 254 because with AUDIBLEDEC, sdkbuildall could not pass in some combination
Fg_DisplayTaskStackEnd ds 1
#pragma endasm

//If the display is not being used in an overlay setting, we need to declare the codebank resource
#ifndef RSRC_DISPLAY_CODEBANK
#define RSRC_DISPLAY_CODEBANK 0
#endif


TaskEntry g_DisplayTaskEntry = {&g_DisplayTaskQueueDescriptor,      //Message Queue
                                0,                                  //pointer to wait on (meaning depends on state)
                                0,                                  //word to wait on (meaning depends on state)
                                EVENT_TIMEOUT,                      //initial state
                                g_DisplayTaskStack,                 //Current stack pointer
                                0xABCDEF,                           //task ID
                                TIMEOUT_IMMEDIATE,                  //Initial timeout
                                {RSRC_DISPLAY_CODEBANK,0,0,0,0},    //Region requirements map
                                0,                                  //Priority
                                g_DisplayTaskStack,                 //Initial stack pointer
                                DISPLAY_TASK_STACK_SIZE,            //Stack Size
                                DisplayTask};                       //Entry point
 