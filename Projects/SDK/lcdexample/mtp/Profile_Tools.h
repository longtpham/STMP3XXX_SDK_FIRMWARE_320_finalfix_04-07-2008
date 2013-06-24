#ifndef __PROFILE_TOOLS_H

#define __PROFILE_TOOLS_H

#include "types.h"

void _reentrant ClearCounter(void);
long _reentrant GetCounter(void);
void _reentrant ClearTrace(void);
void _reentrant Save_Trace_Start(WORD Data);
void _reentrant Save_Trace_End(void);

typedef struct _TraceRecord
{
    long	Start_Time;
	long	End_Time;
	long	Delta_Time;
    WORD    Data;
} TraceRecord;

#endif
