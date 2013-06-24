/**
***
*** Filename: mtpdeviceinfodata.c
*** Description: MTP DeviceInfo data
***
***
*** Copyright (c) SigmaTel, Inc. Unpublished
***
*** SigmaTel, Inc.
*** Proprietary & Confidential
***
*** This source code and the algorithms implemented therein constitute
*** confidential information and may comprise trade secrets of SigmaTel, Inc.
*** or its associates, and any use thereof is subject to the terms and
*** conditions of the Confidential Disclosure Agreement pursuant to which this
*** source code was originally received.
**/
//////////////////////////////////////////////////////////////////
// Copyright © Microsoft Corporation, 2003
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//   Includes
//////////////////////////////////////////////////////////////////
#include "mtp.h"
#include "sysresourceapi.h"
#include "sysserialnumber.h"
#include "stringlib.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////
//   Globals
//////////////////////////////////////////////////////////////////
/*static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}
*/

void LoadMTPDeviceInfoData(WORD* pBufDevInfoData, WORD TargetMem, WORD BufSize)
{
    SysLoadResource(RSRC_MTPDEVICEINFODATA, RS_ROOT_HANDLE, RSRC_TYPE_DATA, 
        pBufDevInfoData, TargetMem, BufSize);
}
WORD _X g_sMtpDeviceInfo[] = {
0x060064, 0x000000, 0x420064, 0x69006D,
0x006300, 0x6F0072, 0x007300, 0x66006F,
0x007400, 0x63002E, 0x006F00, 0x3A006D,
0x002000, 0x2E0031, 0x003000, 0x20003B,
0x006D00, 0x630069, 0x007200, 0x73006F,
0x006F00, 0x740066, 0x002E00, 0x6F0063,
0x006D00, 0x57002F, 0x004D00, 0x520044,
0x004D00, 0x440050, 0x003A00, 0x310020,
0x003000, 0x31002E, 0x003B00, 0x610020,
0x007500, 0x690064, 0x006200, 0x65006C,
0x002E00, 0x6F0063, 0x006D00, 0x20003A,
0x003100, 0x30002E, 0x000000, 0x240000,
0x000000, 0x021001, 0x100310, 0x051004,
0x100610, 0x081007, 0x100910, 0x0C100B,
0x100D10, 0x151014, 0x101610, 0x019802,
0x980398, 0x059804, 0x980698, 0x019808,
0x910291, 0x049103, 0x910591, 0x079106,
0x910891, 0x0A9109, 0x910B91, 0xF1100F,
0x97F297, 0x000002, 0x400200, 0x034003,
0x000000, 0x01D100, 0xD102D1, 0x000000,
0x000600, 0x090000, 0x300830, 0x04B901,
0x3001B9, 0x093000, 0x690053, 0x006700,
0x61006D, 0x005400, 0x6C0065, 0x000000,
0x004D11, 0x500054, 0x002000, 0x6C0050,
0x006100, 0x650079, 0x007200, 0x200020,
0x002000, 0x200020, 0x002000, 0x180000,
0x2E0033, 0x003200, 0x300030, 0x002E00,
0x310039, 0x003000, 0x250020, 0x004F00,
0x540055, 0x005F00, 0x410050, 0x004400,
0x31003A, 0x003200, 0x250038, 0x000000
};