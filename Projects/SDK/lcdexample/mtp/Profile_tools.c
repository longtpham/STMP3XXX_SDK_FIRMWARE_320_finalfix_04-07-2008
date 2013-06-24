#if 0
#include "types.h"
#include "profile_tools.h"
void _do_not_care(void);

#define NUMBER_TRACES	1000

TraceRecord _X Trace[NUMBER_TRACES];
int TraceCounter = 0;

void _reentrant ClearCounter(void)
{
	#pragma asm
	clr a
	move a1,x:$ffea
	move a1,x:$ffeb
	#pragma endasm

}

void _do_not_care(void)
{
#pragma asm
FGetCounter
	clr a
	move x:$ffea,a0
	move x:$ffeb,a1
	rts
#pragma endasm
}


void _reentrant ClearTrace(void)
{
	for(TraceCounter = 0 ; TraceCounter < NUMBER_TRACES ; TraceCounter++)
	{
		Trace[TraceCounter].Start_Time = 0;
		Trace[TraceCounter].End_Time = 0;
	}

	TraceCounter = 0;
}

void _reentrant Save_Trace_Start(WORD Data)
{

	if(TraceCounter < NUMBER_TRACES)
	{
		TraceCounter++;
		Trace[TraceCounter].Start_Time = GetCounter();
		Trace[TraceCounter].Data = (WORD) Data;
	}
}

void _reentrant Save_Trace_End(void)
{

	if(TraceCounter < NUMBER_TRACES)
	{
		Trace[TraceCounter].End_Time = GetCounter();
		Trace[TraceCounter].Delta_Time = 
			Trace[TraceCounter].End_Time - Trace[TraceCounter].Start_Time;

	}
}

#endif