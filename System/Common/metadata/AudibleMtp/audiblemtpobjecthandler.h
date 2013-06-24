////////////////////////////////////////////////////////////////////////////////
///
/// \file audiblemtpobjecthandler.h
/// \brief This is a header for ST-Store API, functions, enums, defines, typedefs and data structures definitions.
///
/// Copyright (c) SigmaTel, Inc. Unpublished
///
/// SigmaTel, Inc.
/// Proprietary  Confidential
///
/// This source code and the algorithms implemented therein constitute
/// confidential information and may comprise trade secrets of SigmaTel, Inc.
/// or its associates, and any use thereof is subject to the terms and
/// conditions of the Confidential Disclosure Agreement pursuant to which this
/// source code was originally received.
///
////////////////////////////////////////////////////////////////////////////////
#ifndef __AUDIBLEMTPOBJECTHANDLER_H_
#define __AUDIBLEMTPOBJECTHANDLER_H_

#include <kernel.h>

INT _reentrant DeleteAudiblePosFile(WORD ObjectHandle, int PackedPathNameAddress, _packed BYTE *pTempUserBuf, StString *pTempStringBuf);
INT _reentrant StorAudibleCodecObjectProp(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue);
INT _reentrant StorAudibleProductID(EnGetSet OPType, WORD ObjectHandle, StString *rValue);
INT _reentrant StorAudibleTitleSegment(EnGetSet OPType, WORD ObjectHandle, UINT128 *rValue);
INT _reentrant StorAudiblePlaybackPosition(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue);
INT _reentrant StorAudiblePlayedThrough(EnGetSet OPType, WORD ObjectHandle, UINT16 *rValue);

int _inline SysCallStorAudibleCodecObjectProp(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue)
{                                                       
__asm ("    extern RSRC_AUDIBLE_OBJECT_PROP_HANDLER         \n"     
       "    extern FStorAudibleCodecObjectProp              \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_AUDIBLE_OBJECT_PROP_HANDLER,r0    \n"     
       "    move    #FStorAudibleCodecObjectProp,r4" : : "A"(rValue));                     
    return exSysCallFunction_ii(OPType, ObjectHandle);
}

int _inline SysCallStorAudibleProductID(EnGetSet OPType, WORD ObjectHandle, StString *rValue)
{                                                       
__asm ("    extern RSRC_AUDIBLE_OBJECT_PROP_HANDLER         \n"     
       "    extern FStorAudibleProductID                    \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_AUDIBLE_OBJECT_PROP_HANDLER,r0    \n"     
       "    move    #FStorAudibleProductID,r4" : : "A"(rValue));                     
    return exSysCallFunction_ii(OPType, ObjectHandle);
}

int _inline SysCallStorAudibleTitleSegment(EnGetSet OPType, WORD ObjectHandle, UINT128 *rValue)
{                                                       
__asm ("    extern RSRC_AUDIBLE_OBJECT_PROP_HANDLER         \n"     
       "    extern FStorAudibleTitleSegment                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_AUDIBLE_OBJECT_PROP_HANDLER,r0    \n"     
       "    move    #FStorAudibleTitleSegment,r4" : : "A"(rValue));                     
    return exSysCallFunction_ii(OPType, ObjectHandle);
}

int _inline SysCallStorAudiblePlaybackPosition(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue)
{                                                       
__asm ("    extern RSRC_AUDIBLE_OBJECT_PROP_HANDLER         \n"     
       "    extern FStorAudiblePlaybackPosition             \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_AUDIBLE_OBJECT_PROP_HANDLER,r0    \n"     
       "    move    #FStorAudiblePlaybackPosition,r4" : : "A"(rValue));                     
    return exSysCallFunction_ii(OPType, ObjectHandle);
}

int _inline SysCallStorAudiblePlayedThrough(EnGetSet OPType, WORD ObjectHandle, UINT16 *rValue)
{                                                       
__asm ("    extern RSRC_AUDIBLE_OBJECT_PROP_HANDLER         \n"     
       "    extern FStorAudiblePlayedThrough                \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_AUDIBLE_OBJECT_PROP_HANDLER,r0    \n"     
       "    move    #FStorAudiblePlayedThrough,r4" : : "A"(rValue));                     
    return exSysCallFunction_ii(OPType, ObjectHandle);
}
int _inline SysCallDeleteAudiblePosFile(WORD ObjectHandle, int PackedPathNameAddress,  _packed BYTE *pTempUserBuf, StString *pTempStringBuf)
{                                                       
__asm ("    extern RSRC_AUDIBLE_OBJECT_PROP_HANDLER         \n"     
       "    extern FDeleteAudiblePosFile                    \n"     
       "    move    %0,r1                                   \n"
       "    move    %1,r5                                   \n"
       "    move    #RSRC_AUDIBLE_OBJECT_PROP_HANDLER,r0    \n"     
       "    move    #FDeleteAudiblePosFile,r4" : : "A"(pTempUserBuf), "A"(pTempStringBuf));                     
    return exSysCallFunction_ii(ObjectHandle, PackedPathNameAddress);
}

#endif //!__AUDIBLEMTPOBJECTHANDLER_H_