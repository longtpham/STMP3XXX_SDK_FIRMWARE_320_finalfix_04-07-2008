////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : SysResourceApi.h
// Description : Structures and Functions Definitions for System Resource API
////////////////////////////////////////////////////////////////////////////////

#ifndef __SysResourceApi_h__
#define __SysResourceApi_h__ 1

#include "types.h"

#define RSRC_TYPE_STRING                1
#define RSRC_TYPE_DATA                  2
#define RSRC_TYPE_BITMAP                3
#define RSRC_TYPE_NESTED_RESOURCE       9

#define MAX_NESTED_RSC      10           // Included Root Resource
#define FREE_RSC_HANDLE     0xffffff    // Code for free handle

#define RS_ROOT_HANDLE                      0           // Pass this handle to open
                                                        // from the root.

#define RSC_MAX_HANDLE_REACHED              0x800001    // No more Nested Handles free
                                                        // Maximum allocated nested resource handle reached
                                                        // To correct error increase the number of MAX_NESTED_RSC
#define RSC_INVALID_NESTED_HANDLE           0x800002    // The Nested Resource Handle is Invalid
                                                        // Handle >= MAX_NESTED_RSC 
#define RSC_INVALID_NESTED_NOT_ITIALIZED    0x800003    // The Nested Resource Handle is not initialized
                                                        // element RscNumber of Struct_Handle_Nested_Rsc 
                                                        // is FREE_RSC_HANDLE in 
#define RSC_INVALID_RSC                     0x800004    // Resource is invalid. Resource Header does not
#define RSC_INVALID_MEM_SPACE               0x800005    // Memory Space for resource is invalid. Valid spaces are X, Y or P.
#define RSC_ERROR_READING_RSC               0x800006

#define TARGET_MEM_X                        0x800000
#define TARGET_MEM_Y                        0x400000
#define TARGET_MEM_L                        0x200000
#define TARGET_MEM_P                        0x100000

typedef struct {
    WORD    wRscNumber;             // Nested resource number
                                    // 0x00 = Root Resource
                                    // 0xffff = Free Handle
    WORD    wStartPosition;         // Start position data section (in bytes)
    WORD    wCurrentPosition;       // Current position (in bytes)
} Struct_Handle_Nested_Rsc, * pStruct_Handle_Nested_Rsc;

RETCODE _reentrant SysOpenResource(WORD wRscNum, BYTE btHandleNestedParentRsc);
RETCODE _reentrant SysCloseResource(BYTE btHandleNestedRsc);
RETCODE _reentrant SysLoadResource(WORD wRscNum, BYTE btHandleNestedRsc, WORD wRscType,
                        WORD *pwTargetBuffer, WORD wTargetSpace, WORD wMaxSize);
RETCODE _reentrant SysResourceFileRead(BYTE btHandleNestedRsc, WORD wNumBytes, 
                            WORD wTargetSpace, WORD *pwTargetBuffer);
RETCODE _reentrant SysResourceFileSeek(BYTE btHandleNestedRsc, WORD wNumWords);                        
WORD _reentrant SysResourceFileTell(BYTE btHandleNestedRsc);
void _reentrant SysSetResourceTag(WORD wTag);
		   
extern WORD wResourceTag;
#define SysGetResourceTag(void) (wResourceTag)

extern void EnterNonReentrantSection(void);
extern void LeaveNonReentrantSection(void);

#endif
