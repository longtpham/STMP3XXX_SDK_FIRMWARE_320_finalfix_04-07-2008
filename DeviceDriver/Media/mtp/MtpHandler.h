/**
***
*** Filename: mtphandler.h
*** Description: Contains the function prototypes for the MTP handler and structure.
*** The MTP handler processes the MTP commands and calls the lower level functions
*** of the MTP stack for processing those commands.
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


#ifndef __MTPHANDLER_H_INCLUDED
#define __MTPHANDLER_H_INCLUDED


#include "mtp.h"

#define GET_REFERENCES      1
#define SET_REFERENCES      0

//
// These variable indicate if the database or store have been modified without
// having been written out to the storage media.
//
extern volatile INT g_DB_Dirty;
extern volatile INT g_stor_dirty;

/**
 * \struct DATA4SENDOBJECT
 *
 * A structure for containing the object infomation.  It is used for saving 
 * information in between paired operations like sendObjectInfoMtpHandle 
 * and sendObjectMtpHandle.
 *
 * @see SObjectInfo
 */
typedef struct tagData4SendObject
{
  UINT32         StoreID;            /**< The storage ID. */
  UINT32         hParentHandle;      /**< Parent object handle for the incoming object. */
  UINT32         hObjectHandle;      /**< Reserved Object Handle for the incoming object. */
  BYTE           bValid;             /**< Indicates if the ObjectInfo dataset is valid. */
} DATA4SENDOBJECT, *PDATA4SENDOBJECT;


/**
 * \struct USBMTP_STATUS
 *
 * Contains response data for an MTP operation including status and
 * additional parameters, if any.
 *
**/
typedef struct tagUsbMtpStatus
{
    UINT32        Status;                 // MTP response code
    UINT32        ObjectHandle;           // Object handle of failed object transfer
    UINT32        NumParams;              // Number of parameters being returned
    UINT32        Params[MAX_MTP_PARAMS]; // Additional parameters
} USBMTP_STATUS, * PUSBMTP_STATUS;


//typedef struct MtpDevice SMtpDevice;

/**
 * \struct SMtpHandler
 *
 * A structure for contain for handling the operations of the MTP protocol.
 * It is the main driving structure behind the MTP protocol handling.  The
 * MTP device contains device information and other data buffers.
 *
 * @see SMtpDevice
 * @see SUsbCmdBlock
 * @see DATA4SENDOBJECT
 */
typedef struct MtpHandler
{
  USBMTP_DEVICE*     pDevice;          // A pointer to the device that the MTP handler utilizes. */
  DATA4SENDOBJECT Data4SendObject;     // Data saved during paired operations: SendObjectInfo/SendObject. */
  DWORD           SessionId;           // Current session ID. */
  DWORD           TransactionId;       // Current transaction ID. */
  BOOL            bMTPReset;           // Device requires reset
  USBMTP_STATUS   MtpResponse;         // Status and parameters
} SMtpHandler;

/**
 * Initializes the MTP handler structure.
 * 
 * @param pHandle A pointer to the SMtpHandler structure.
 * @param pDevice A pointer to the SMtpDevice structure to be used with the MTP handler.
 * @see allocMtpHandle()
 * @see destroyMtpHandle()
 */
void initMtpHandle(SMtpHandler *pHandle, USBMTP_DEVICE *pDevice);



/**
 * Processes the MTP command.
 *
 * @param pHandle Points to the MTP handler structure.
 * @param pCmd Points to the USB command structure. 
 */
void _reentrant processCommandMtpHandle(SMtpHandler *pHandle, SUsbCmdBlock *pCmd);

/** Original Protected Functions **/
int _reentrant MtpResponseHandler (SMtpHandler *pHandle);
BOOL _reentrant sendResponseMtpHandle (SMtpHandler *pHandle, UINT32 ResponseCode, UINT32 NumParams, UINT32 * pParams);
BOOL sendEventPtp(USBMTP_DEVICE *pDevice, WORD eventCode, DWORD transID, DWORD Param);

//BOOL sendDataHeaderMtpHandle (SMtpHandler *pHandle, DWORD dataSize );

// begin overlaid functions
int _reentrant getDeviceInfoMtpHandle (SMtpHandler *pHandle);
int _reentrant openSessionMtpHandle (SMtpHandler *pHandle);

int _reentrant closeSessionMtpHandle(SMtpHandler *pHandle);

int _reentrant getStorageIdsMtpHandle (SMtpHandler *pHandle);
int _reentrant getStorageInfoMtpHandle (SMtpHandler *pHandle);
int _reentrant getNumObjectsMtpHandle (SMtpHandler *pHandle);
int _reentrant getObjectHandlesMtpHandle (SMtpHandler *pHandle);
int _reentrant getObjectInfoMtpHandle (SMtpHandler *pHandle);
int _reentrant getObjectMtpHandle (SMtpHandler *pHandle);
int _reentrant sendObjectInfoMtpHandle (SMtpHandler *pHandle);
int _reentrant sendObjectMtpHandle (SMtpHandler *pHandle);
int _reentrant deleteObjectMtpHandle (SMtpHandler *pHandle);
int _reentrant formatStoreMtpHandle (SMtpHandler *pHandle);
int _reentrant getDevicePropDescMtpHandle (SMtpHandler *pHandle);
int _reentrant getDevicePropValueMtpHandle (SMtpHandler *pHandle);
int _reentrant setDevicePropValueMtpHandle (SMtpHandler *pHandle);
int _reentrant resetDevicePropValueMtpHandle (SMtpHandler *pHandle);
int _reentrant moveObjectMtpHandle (int _x, int _y, int *_p);
int _reentrant copyObjectMtpHandle (int _x, int _y, int *_p);
int _reentrant powerDownMtpHandle(SMtpHandler *pHandle);
int _reentrant resetDeviceMtpHandle (SMtpHandler *pHandle);
int _reentrant selfTestMtpHandle (SMtpHandler *pHandle);
int _reentrant setObjectProtectionMtpHandle (SMtpHandler *pHandle);
int _reentrant getObjectPropDescMtpHandle(SMtpHandler *pHandle);
int _reentrant getObjectPropValueMtpHandle(SMtpHandler *pHandle);
int _reentrant setObjectPropValueMtpHandle(SMtpHandler *pHandle);
int _reentrant getObjectPropListMtpHandle(SMtpHandler *pHandle);
int _reentrant setObjectPropListMtpHandle(SMtpHandler *pHandle);
int _reentrant sendObjectPropListMtpHandle(SMtpHandler *pHandle);
int _reentrant getObjectPropSupportedMtpHandle(SMtpHandler *pHandle);
int _reentrant GetSetObjectReferences(int GetSetReferences, int _y, int *_p);

int _reentrant JanusGetDevCertChallengeMtpHandle(int unused1, int unused2, int *piHandle);
int _reentrant JanusSetDevCertResponseMtpHandle(int unused1, int unused2, int *piHandle);
int _reentrant JanusGetSecureTimeChallengeMtpHandle(SMtpHandler *pHandle);
int _reentrant JanusSetSecureTimeResponseMtpHandle(SMtpHandler *pHandle);
int _reentrant JanusSetLicenseResponseMtpHandle(SMtpHandler *pHandle);
int _reentrant JanusGetSyncListMtpHandle(SMtpHandler *pHandle);
int _reentrant JanusSendMeterChallengeQueryHandle(SMtpHandler *pHandle);
int _reentrant JanusGetMeterChallengeHandle(SMtpHandler *pHandle);
int _reentrant JanusSetMeterResponseHandle(SMtpHandler *pHandle);
int _reentrant JanusCleanDataStoreMtpHandle(SMtpHandler *pHandle);

int _reentrant JanusGetLicenseStateMtpHandle(SMtpHandler *pHandle);
int _reentrant JanusSendWMDRMPDCommandMtpHandle(SMtpHandler *pHandle);
int _reentrant JanusSendWMDRMPDRequestMtpHandle(SMtpHandler *pHandle);

int _reentrant sysErrorResponseMtpHandle(int _x, int _y, int *_piHandle);

//
// MTP operations
//
void resetObjectPropValueMtpHandle(SMtpHandler *pHandle);
// Janus extensions
void setLicenseResponseMtpHandle(SMtpHandler *pHandle); 
//
//Vendor-specificoperations
//
void vendorCommand_1MtpHandle(SMtpHandler *pHandle);

//
// Currently unsupported
//
void MtpInitiateCapture (SMtpHandler *pHandle);
void MtpInitiateOpenCapture (SMtpHandler *pHandle);
void MtpTerminateOpenCapture (SMtpHandler *pHandle);


int _inline SysCallresetDeviceMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_RESETDEVICE        \n"     
       "    extern  FresetDeviceMtpHandle       \n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_MTP_RESETDEVICE,r0    \n"     
       "    move    #FresetDeviceMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallcloseSessionMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_CLOSESESSION        \n"     
       "    extern  FcloseSessionMtpHandle       \n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_MTP_CLOSESESSION,r0    \n"     
       "    move    #FcloseSessionMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallopenSessionMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_OPENSESSION        \n"     
       "    extern  FopenSessionMtpHandle       \n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_MTP_OPENSESSION,r0    \n"     
       "    move    #FopenSessionMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetDeviceInfoMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETDEVICEINFO        \n"     
       "    extern  FgetDeviceInfoMtpHandle       \n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_MTP_GETDEVICEINFO,r0    \n"     
       "    move    #FgetDeviceInfoMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetStorageIdsMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETSTORAGEIDSMTPHANDLE         \n"     
       "    extern  FgetStorageIdsMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_GETSTORAGEIDSMTPHANDLE,r0     \n"     
       "    move    #FgetStorageIdsMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetStorageInfoMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETSTORAGEINFOMTPHANDLE        \n"     
       "    extern  FgetStorageInfoMtpHandle                \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_GETSTORAGEINFOMTPHANDLE,r0    \n"     
       "    move    #FgetStorageInfoMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetObjectHandlesMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETOBJECTHANDLES               \n"     
       "    extern  FgetObjectHandlesMtpHandle              \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_GETOBJECTHANDLES,r0           \n"     
       "    move    #FgetObjectHandlesMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetObjectInfoMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETOBJECTINFO                  \n"     
       "    extern  FgetObjectInfoMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_GETOBJECTINFO,r0              \n"     
       "    move    #FgetObjectInfoMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}
        
int _inline SysCallgetObjectMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETOBJECT                      \n"     
       "    extern  FgetObjectMtpHandle                     \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_GETOBJECT,r0                  \n"     
       "    move    #FgetObjectMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCalldeleteObjectMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_DELETEOBJECTMTPHANDLE          \n"     
       "    extern  FdeleteObjectMtpHandle                  \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_DELETEOBJECTMTPHANDLE,r0      \n"     
       "    move    #FdeleteObjectMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallsendObjectInfoMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_SENDOBJECTINFO                 \n"     
       "    extern  FsendObjectInfoMtpHandle                \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_SENDOBJECTINFO,r0             \n"     
       "    move    #FsendObjectInfoMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallsendObjectMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_SENDOBJECT                     \n"     
       "    extern  FsendObjectMtpHandle                    \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_SENDOBJECT,r0                 \n"     
       "    move    #FsendObjectMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetObjectPropDescMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETOBJECTPROPDESCMTPHANDLE     \n"     
       "    extern  FgetObjectPropDescMtpHandle             \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_GETOBJECTPROPDESCMTPHANDLE,r0 \n"     
       "    move    #FgetObjectPropDescMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetObjectPropValueMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETOBJECTPROPVALUEMTPHANDLE        \n"     
       "    extern  FgetObjectPropValueMtpHandle                \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_GETOBJECTPROPVALUEMTPHANDLE,r0    \n"     
       "    move    #FgetObjectPropValueMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetObjectPropSupportedMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETOBJECTPROPSUPPORTEDMTPHANDLE         \n"     
       "    extern  FgetObjectPropSupportedMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_GETOBJECTPROPSUPPORTEDMTPHANDLE,r0     \n"     
       "    move    #FgetObjectPropSupportedMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallsetObjectPropValueMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_SETOBJECTPROPVALUEMTPHANDLE        \n"     
       "    extern  FsetObjectPropValueMtpHandle                \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_SETOBJECTPROPVALUEMTPHANDLE,r0    \n"     
       "    move    #FsetObjectPropValueMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetObjectPropListMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETOBJECTPROPLISTMTPHANDLE         \n"     
       "    extern  FgetObjectPropListMtpHandle                 \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_GETOBJECTPROPLISTMTPHANDLE,r0     \n"     
       "    move    #FgetObjectPropListMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallsetObjectPropListMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_SETOBJECTPROPLISTMTPHANDLE         \n"     
       "    extern  FsetObjectPropListMtpHandle                 \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_SETOBJECTPROPLISTMTPHANDLE,r0     \n"     
       "    move    #FsetObjectPropListMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallsendObjectPropListMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_SENDOBJECTPROPLISTMTPHANDLE        \n"     
       "    extern  FsendObjectPropListMtpHandle                \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_SENDOBJECTPROPLISTMTPHANDLE,r0    \n"     
       "    move    #FsendObjectPropListMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetDevicePropDescMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETDEVICEPROPDESCMTPHANDLE         \n"     
       "    extern  FgetDevicePropDescMtpHandle                 \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_GETDEVICEPROPDESCMTPHANDLE,r0     \n"     
       "    move    #FgetDevicePropDescMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetDevicePropValueMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETDEVICEPROPVALUEMTPHANDLE        \n"     
       "    extern  FgetDevicePropValueMtpHandle                \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_GETDEVICEPROPVALUEMTPHANDLE,r0    \n"     
       "    move    #FgetDevicePropValueMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallsetDevicePropValueMtpHandle(SMtpHandler *pHandle) 
{
__asm ("    extern  RSRC_MTP_SETDEVICEPROPVALUEMTPHANDLE        \n"     
       "    extern  FsetDevicePropValueMtpHandle                \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_SETDEVICEPROPVALUEMTPHANDLE,r0    \n"     
       "    move    #FsetDevicePropValueMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallresetDevicePropValueMtpHandle(SMtpHandler *pHandle)
{
__asm ("    extern  RSRC_MTP_RESETDEVICEPROPVALUEMTPHANDLE      \n"     
       "    extern  FresetDevicePropValueMtpHandle              \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_RESETDEVICEPROPVALUEMTPHANDLE,r0  \n"     
       "    move    #FresetDevicePropValueMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusGetSecureTimeChallengeMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_GETSECURETIMECHALLENGE       \n"     
       "    extern  FJanusGetSecureTimeChallengeMtpHandle       \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_JANUS_GETSECURETIMECHALLENGE,r0   \n"     
       "    move    #FJanusGetSecureTimeChallengeMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusSetSecureTimeResponseMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_SETSECURETIMERESPONSE        \n"     
       "    extern  FJanusSetSecureTimeResponseMtpHandle        \n"     
       "    move    %0,r1                                       \n"
       "    move    #RSRC_MTP_JANUS_SETSECURETIMERESPONSE,r0    \n"     
       "    move    #FJanusSetSecureTimeResponseMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusSetLicenseResponseMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_SETLICENSERESPONSE       \n"     
       "    extern  FJanusSetLicenseResponseMtpHandle       \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_JANUS_SETLICENSERESPONSE,r0   \n"     
       "    move    #FJanusSetLicenseResponseMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusGetSyncListMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_GETSYNCLIST          \n"     
       "    extern  FJanusGetSyncListMtpHandle          \n"     
       "    move    %0,r1                               \n"
       "    move    #RSRC_MTP_JANUS_GETSYNCLIST,r0      \n"     
       "    move    #FJanusGetSyncListMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusSendMeterChallengeQueryHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_SEND_METER_CHALLENGE_QUERY       \n"     
       "    extern  FJanusSendMeterChallengeQueryHandle             \n"     
       "    move    %0,r1                                           \n"
       "    move    #RSRC_MTP_JANUS_SEND_METER_CHALLENGE_QUERY,r0   \n"     
       "    move    #FJanusSendMeterChallengeQueryHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusGetMeterChallengeHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_GETMETERCHALLENGE        \n"     
       "    extern  FJanusGetMeterChallengeHandle           \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_JANUS_GETMETERCHALLENGE,r0    \n"     
       "    move    #FJanusGetMeterChallengeHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusSetMeterResponseHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_SETMETERRESPONSE         \n"     
       "    extern  FJanusSetMeterResponseHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_JANUS_SETMETERRESPONSE,r0     \n"     
       "    move    #FJanusSetMeterResponseHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusGetLicenseStateMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_GET_LICENSE_STATE         \n"     
       "    extern  FJanusGetLicenseStateMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_JANUS_GET_LICENSE_STATE,r0     \n"     
       "    move    #FJanusGetLicenseStateMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusSendWMDRMPDCommandMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_SEND_WMDRMPD_COMMAND         \n"     
       "    extern  FJanusSendWMDRMPDCommandMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_JANUS_SEND_WMDRMPD_COMMAND,r0     \n"     
       "    move    #FJanusSendWMDRMPDCommandMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallJanusSendWMDRMPDRequestMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_JANUS_SEND_WMDRMPD_REQUEST         \n"     
       "    extern  FJanusSendWMDRMPDRequestMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_JANUS_SEND_WMDRMPD_REQUEST,r0     \n"     
       "    move    #FJanusSendWMDRMPDRequestMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallformatStoreMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_FORMATSTOREMTPHANDLE         \n"     
       "    extern  FformatStoreMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_FORMATSTOREMTPHANDLE,r0     \n"     
       "    move    #FformatStoreMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallselfTestMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_SELFTEST         \n"     
       "    extern  FselfTestMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_SELFTEST,r0     \n"     
       "    move    #FselfTestMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallsetObjectProtectionMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_SETOBJECTPROTECTION         \n"     
       "    extern  FsetObjectProtectionMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_SETOBJECTPROTECTION,r0     \n"     
       "    move    #FsetObjectProtectionMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallpowerDownMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_POWERDOWN         \n"     
       "    extern  FpowerDownMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_POWERDOWN,r0     \n"     
       "    move    #FpowerDownMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallgetNumObjectsMtpHandle(SMtpHandler *pHandle)               
{
__asm ("    extern  RSRC_MTP_GETNUMOBJECTS         \n"     
       "    extern  FgetNumObjectsMtpHandle                 \n"     
       "    move    %0,r1                                   \n"
       "    move    #RSRC_MTP_GETNUMOBJECTS,r0     \n"     
       "    move    #FgetNumObjectsMtpHandle,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallRestoreDriveFromBackupMTP(int i1, int i2)               
{
__asm ("    extern  RSRC_RESTORESYSDRIVE_P_CODE       \n"     
       "    extern  FRestoreDriveFromBackupMTP     \n"     
       "    move    #RSRC_RESTORESYSDRIVE_P_CODE,r0   \n"     
       "    move    #FRestoreDriveFromBackupMTP,r4" );
    return exSysCallFunction_ii(i1,i2);
}

#ifndef RESET_AFTER_FORMAT
int _inline SysCallstorInit(SMtpHandler *pHandle)
{
__asm ("    extern  RSRC_MTP_OPENSESSION        \n"     
       "    extern  FStorInit					\n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_MTP_OPENSESSION,r0    \n"     
       "    move    #FStorInit,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}

int _inline SysCallstorClose (SMtpHandler *pHandle)
{
__asm ("    extern  RSRC_MTP_CLOSESESSION        \n"     
       "    extern  FStorClose					\n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_MTP_CLOSESESSION,r0    \n"     
       "    move    #FStorClose,r4" : : "A"(pHandle));
    return exSysCallFunction_v();
}
#endif

#endif
