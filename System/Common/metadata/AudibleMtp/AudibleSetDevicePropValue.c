/**
***
*** Filename: AudibleSetDevicePropValue.c
*** Description: handles the device property value for audible command(s)
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
#include "support.h"
#include "MtpHandler.h"
#include "FileSpec.h"
#include "FileSystem.h"
#include "extern.h"
#include "stStore.h"
#include "stError.h"
#include "mtp.h"
#include "stringlib.h"
#include "AAActivationRecords.h"
#include "audiblemtpobjecthandler.h"

//////////////////////////////////////////////////////////////////
//   Extern declaration
//////////////////////////////////////////////////////////////////
extern SPtpString      g_TempPtpString;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : int _reentrant SetAudibleActivationPropValue(SMtpHandler *pHandle) 
//! This function takes the the activation record data from USB and writes it to a file.  The following 
//! responses are used as a return to the host:
//!
//! PTP_RESPONSECODE_OK                  - Record was written successfully 
//! PTP_RESPONSECODE_INCOMPLETETRANSFER	 - Data transfer size from host does not match 
//!										   the expected record size.
//! PTP_RESPONSECODE_GENERALERROR        - Error during file write.
//! 
//! \param [IN][out] _pSMtpHandler
//!     a pointer to SMtpHandler data structure
//! \return function always returns TRUE
//!
int _reentrant SetAudibleActivationPropValue(SMtpHandler *pHandle)
{
    WORD PropId; 
    DWORD dwTotalBytes;
	DWORD dwArraySize;
    UINT16 i;
	UINT16 byteOffset = 0;
	int byte=0;
	DWORD dwObjectDataLength;
	SyncObject  *pSyncObject;  
	WORD _X     *pBuf = pHandle->pDevice->CommandOutBuffer;

    //
    // Get the Property ID from the first paramter of the command block
    //
    PropId = (WORD)pHandle->pDevice->PtpCmd.Params[0];


    //
    // Proceed MTP device to DATA-out Phase
    //
    // Transfer in the data sent by the host.
    pHandle->pDevice->Phase = DEVICE_PHASE_DATAOUT;

    //Set the number of bytes to transfer from the host.
    dwTotalBytes = SIZE_IN_BYTES_Usb_Container_Header + MTP_AA_ACTIVATION_RECORD_TRANSFER_SIZE_BYTES;
	    
    pSyncObject = &pHandle->pDevice->TransferObject;

    SysAssignObject(pSyncObject,ISR_OWNER);

    //Get the data from the host.  
    if( usb_device_recv_data(pHandle->pDevice->wBulkOutEndpointNum, pBuf, 0, 
	    dwTotalBytes, COMMAND_HANDLER|((WORD)pHandle->pDevice),TRUE) == USB_OK )
    {
	    SysWaitOnObject(-1,pSyncObject);
        pHandle->pDevice->Phase = DEVICE_PHASE_IDLE;
    }
        
    // The first packet contains the data header.  Extract the object length which should match that
    // sent in the object info.  
    dwObjectDataLength = MtpGetLittleEndianINT32InX(pBuf, 0);

    if (dwObjectDataLength != dwTotalBytes)
    {   // expected size doesn't match.  Return error.  USB/MTP may stall.
        pHandle->MtpResponse.Status = PTP_RESPONSECODE_INCOMPLETETRANSFER;
        return TRUE;
    }

//  Do not have to worry about receiving Null packet since Audible Activation code is 580-bytes 
//  which is less than TRANSFER_BUFFER_SIZE * 8.
//    MtpLastPacketHandler(pHandle->pDevice->wBulkOutEndpointNum, 
//       USB_OUT, dwBytesReceived, COMMAND_HANDLER|((WORD)pHandle->pDevice), 
//       pSyncObject);

    // Set the buffer index to the begining of the data
    byteOffset = SIZE_IN_BYTES_Usb_Container_Header;
		
		
	// The first agrument is the number of elements in the Array
    dwArraySize = MtpGetLittleEndianINT32InX((void _X *)pBuf, byteOffset);
    byteOffset += 4;

    //transfer data from _X to a _Y location.  Use g_TempPtpString as a temporary storage
    //container as oppose to allocating memory for an array.    
	for(i=0; i<=AA_ACTIVATION_RECORD_SIZE_BYTES; i++)
	{
	    byte = GetByteFromArrayInX((void _X *)pBuf, byteOffset);
		byteOffset += 1;
		PutByteInArrayInY((void _Y *)&g_TempPtpString, i, byte);
    }	    
		
	//Write the Audible Activation Record
	if(AASetActivationRecords(dwArraySize, 0, (int *)&g_TempPtpString)== SUCCESS)
	{
			pHandle->MtpResponse.Status = PTP_RESPONSECODE_OK;
	}
	else
	{	//A problem has occurred during the write, return an error to the host.
		pHandle->MtpResponse.Status = PTP_RESPONSECODE_GENERALERROR;
	}

    return TRUE;
}
