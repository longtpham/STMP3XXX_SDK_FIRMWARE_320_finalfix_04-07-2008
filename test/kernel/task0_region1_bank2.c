#include "kernel.h"
#include "resource.h"

//This codebank is used to demonstrate a task calling code within the same region, and also calling code in a separate region.

_reentrant int Task0Region2Bank1(int a, int b, int *c);



_reentrant int Task0Region1Bank2(int a, int b, int *c)
{
    //Call into the other region.
    return SysCallFunction(RSRC_REGION2_TASK0_BANK1,Task0Region2Bank1,0,1,&a);

}