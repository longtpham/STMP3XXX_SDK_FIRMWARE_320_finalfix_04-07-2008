/*
 *  File: Rand.c 
 *
 *  Mix multiple hardware sources with sequences for Random Number Generation
 */

#include "types.h"
#include "rand_internal.h"

#pragma optimize 1

WORD g_RandWorkingValue= 1;  


/*//////////////////////////////////////////////////////////////////////////////
//
//>  Name:          WORD _reentrant rand(void);
//
//   Type:          Function
//
//   Description:   Returns a positive pseudo random WORD from 0 to MAX_INT. 
//
//   Inputs:        none
//
//   Outputs:       a pseudo random WORD 
//
//   Notes:         this implementation is based on an ANSI definition.
//   Global usage:  g_RandWorkingValue; 
//<
///////////////////////////////////////////////////////////////////////////////*/
WORD _reentrant rand( void )
{
 	g_RandWorkingValue= g_RandWorkingValue* 1103515245 + 12345;
	return( (unsigned int)(g_RandWorkingValue/65536) & (32768 - 1));
}



