#include "kernel.h"

//Place this code in Region 2.  This will help demonstrate a task executing code from two regions.

_reentrant int Task0Region2Bank1(int a, int b, int *c)
{
    return a+b+*c;
}