////////////////////////////////////////////////////////////////////////////////
///
/// \file audiblemtpdevicehandler.h
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
#ifndef __AUDIBLEMTPDEVICEHANDLER_H_
#define __AUDIBLEMTPDEVICEHANDLER_H_

#include <kernel.h>
#include <types.h>

int _reentrant SetAudibleActivationPropValue(SMtpHandler *pHandle);
int _reentrant SetMtpAudibleActivationDataPropDesc(SMtpHandler *pHandle);
int _reentrant MtpAudibleGetPropValueHandler(SMtpHandler *pHandle);

int _inline SysCallSetAudibleActivationPropValue(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_AUDIBLE_DEVICE_PROP_HANDLER        \n"     
       "    extern  FSetAudibleActivationPropValue          \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_AUDIBLE_DEVICE_PROP_HANDLER,r0    \n"     
       "    move    #FSetAudibleActivationPropValue,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}
    
int _inline SysCallMtpAudibleActivationDataPropDesc(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_AUDIBLE_DEVICE_PROP_HANDLER        \n"     
       "    extern  FMtpAudibleActivationDataPropDesc       \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_AUDIBLE_DEVICE_PROP_HANDLER,r0    \n"     
       "    move    #FMtpAudibleActivationDataPropDesc,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}
    
int _inline SysCallMtpAudibleGetPropValueHandler(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_AUDIBLE_DEVICE_PROP_HANDLER        \n"     
       "    extern  FMtpAudibleGetPropValueHandler          \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_AUDIBLE_DEVICE_PROP_HANDLER,r0    \n"     
       "    move    #FMtpAudibleGetPropValueHandler,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

#endif //!__AUDIBLEMTPDEVICEHANDLER_H_