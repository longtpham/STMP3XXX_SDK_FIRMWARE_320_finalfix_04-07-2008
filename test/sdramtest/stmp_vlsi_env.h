/***********************************************************************
 *
 * $Id: stmp_vlsi_env.h,v 1.4 2000/02/17 00:15:46 thurson Exp $
 *
 * stmp_vlsi_env.h - definitions/function prototypes common to whole
 * stmp project, but specific to the VLSI simulation environment.
 *
 * Non-real-device-driver-type stuff goes here.
 *
 * Tony Hurson, 2/14/00
 *
 **********************************************************************
 *
 *            Copyright (c) SigmaTel Inc.
 *
 * This code is confidential and proprietary product of SigmaTel.
 * Any unauthorized use, reproduction or transfer of this
 * code is strictly prohibited.
 *
 *3456789012345678901234567890123456789012345678901234567890123456789012
***********************************************************************/

#ifndef _STMP_VLSI_ENV_INCLUDE_
#define _STMP_VLSI_ENV_INCLUDE_

/**********************************************************************
 * Device ID's of SmartMedia devices attached to STMP:
 **********************************************************************/

#define SMDeviceID 0x73ec

/**********************************************************************
 * TEST_PASSED - VLSI simulation (only) macro. Sets up "pass" 
 * signature recognised by VLSI simulation environment in the A and
 * B accumulators, then pulls a stop (instruction).
 **********************************************************************/

#define TEST_PASSED \
__asm("\
;/// set up test-passed simulation signature in accums and exit:\n\
        move    #$de,a2\n\
        move    #$789abc,a1\n\
        move    #$123456,a0\n\
        move    #$ed,b2\n\
        move    #$cba987,b1\n\
        move    #$654322,b0\n\
        debug\n\
");

/**********************************************************************
 * TEST_FAILED - VLSI simulation (only) macro. Sets up "fail" 
 * signature recognised by VLSI simulation environment in the A and
 * B accumulators, then pulls a stop (instruction).
 **********************************************************************/

#define TEST_FAILED \
__asm("\
;/// set up test-failed simulation signature in accums and exit:\n\
        move    #$de,a2\n\
        move    #$789abc,a1\n\
        move    #$123456,a0\n\
        move    #$ed,b2\n\
        move    #$cba987,b1\n\
        move    #$654323,b0\n\
        debug\n\
");

#endif
