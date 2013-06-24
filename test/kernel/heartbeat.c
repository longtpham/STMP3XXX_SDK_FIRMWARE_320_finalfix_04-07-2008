// filename: \test\kernel\heartbeat.c

#include "kernel.h"

extern _L LONG g_lTimerTick;

//This is to get the timer tick ticking without using interrupts.
//This function will override Kernel_Heartbeat that is defined in kernellib.a

_P extern _lc_u_Region1_b;
_P extern _lc_u_Region1_e;
_P extern _lc_u_Region2_b;
_P extern _lc_u_Region2_e;

_reentrant void Kernel_Heartbeat(void)
{
    int  a = _lc_u_Region1_e - _lc_u_Region1_b;
    int  b = _lc_u_Region2_e - _lc_u_Region2_b;
    g_lTimerTick++;
}


