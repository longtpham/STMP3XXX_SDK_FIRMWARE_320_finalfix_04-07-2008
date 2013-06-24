/*
 *  File: sRand.c 
 *
 *  Mix multiple hardware sources with sequences for Random Number Generation
 */

#include "types.h"
#include "rand_internal.h"
#pragma optimize 1

/*//////////////////////////////////////////////////////////////////////////////
//> Name:    void _reentrant srand( WORD seed );
//  Type:    Function
//  Input:   Seed word should be random
//  Returns: none
//  Globals: g_wRandNum written with seed word
//<
///////////////////////////////////////////////////////////////////////////////*/
void _reentrant srand( WORD seed )
{   
    g_RandWorkingValue = seed;
}
