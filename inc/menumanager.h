#ifndef _EXEC_H
#define _EXEC_H

#include "types.h"

int _asmfunc SysWaitOnEvent(unsigned int uEvent,struct CMessage *,int uLength);
int _asmfunc SysCallFunction(unsigned int RESOURCE,int _reentrant (int,int,int*),  int, int, int *);

#if !defined(NULL)
#define NULL 0
#endif 

#if !defined(FALSE)
#define FALSE 0
#endif
#if !defined(TRUE)
#define TRUE  !FALSE
#endif

// The same memory location contains either a menu message or button event. 
// The button info is stored in the first word or the entire message is stored.

// CMessage is kept for backards compatibility.
// The union and 2 new structures are added to aid in readability.


#include "messages.h"

#endif 