/**
***
*** Filename: MtpJanusPropertyHandler.c
*** Description: Handles MTP Janus Device Properties.
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

////////////////////////////////////////////////////////////////////////////////
//   Includes 
///////////////////////////////////////////////////////////////////////////////
#include "support.h"
#include "stringlib.h"
#include "MtpHandler.h"
#include "mtp.h"
#ifdef AUDIBLE
#include "AAActivationRecords.h"
#endif

#define FORM_FLAG_SIZE_BYTES 1

//   external references
///////////////////////////////////////////////////////////////////////////////
extern SPtpString      g_TempPtpString;
extern BOOL  g_bHostCancelling;

////////////////////////////////////////////////////////////////////////////////
//   Prototypes of local functions
///////////////////////////////////////////////////////////////////////////////

//If it's not an audible build, send back an error message.
#ifndef AUDIBLE
int _reentrant SetAudibleActivationPropValue(SMtpHandler *pHandle)
{
    pHandle->pDevice->Phase = DEVICE_PHASE_IDLE;
    pHandle->MtpResponse.Status = PTP_RESPONSECODE_PARAMETERNOTSUPPORTED;
    return TRUE;
}

int MtpAudibleActivationDataPropDesc( SMtpHandler *pHandle)
{
    pHandle->pDevice->Phase = DEVICE_PHASE_IDLE;
    pHandle->MtpResponse.Status = PTP_RESPONSECODE_PARAMETERNOTSUPPORTED;
    return TRUE;
}

int MtpAudibleGetPropValueHandler( SMtpHandler *pHandle)
{
    pHandle->pDevice->Phase = DEVICE_PHASE_IDLE;
    pHandle->MtpResponse.Status = PTP_RESPONSECODE_PARAMETERNOTSUPPORTED;
    return TRUE;
}
#else 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : INT MtpAudiblePropertyDescHandler (SMtpHandler *pHandle)
//! This function will be called by getDevicePropDescMtpHandle. 
//!
//! \param [in][out] pHandle
//!     a pointer to SMtpHandler.
//!
//! \returns TRUE but saves the result in pHandle->MtpResponse.Status
//!
int MtpAudibleActivationDataPropDesc( SMtpHandler *pHandle)
{
    WORD wVal=0;
    INT i=0;
	WORD byte;
    WORD PropId;
    WORD _X *pBuf = (WORD _X *)(pHandle->pDevice->CommandInBuffer);
    UINT32 dwBytesToWrite=0;
    WORD bytes_to_send=0;
    WORD header_offset=0;
    WORD pBuf_offset=0;
    WORD activation_offset=0;
    BOOL bLastPacket = FALSE;
    BOOL bValidActivationData = TRUE;

    
    pHandle->MtpResponse.Status = PTP_RESPONSECODE_OK;
    PropId = (WORD)pHandle->pDevice->PtpCmd.Params[0];
    
    // Clear the return buffer
    for(i=0; i<TRANSFER_BUFFER_SIZE;i++)
        pBuf[i] = 0;
    
    // leave the first 4 bytes for the length put in at the end
    MtpPutLittleEndianINT16InX(pBuf, 4, DataBlock);
    MtpPutLittleEndianINT16InX(pBuf, 6, pHandle->pDevice->PtpCmd.Code);
    MtpPutLittleEndianINT32InX(pBuf, 8, pHandle->pDevice->PtpCmd.TransactionID);
    pBuf_offset = SIZE_IN_BYTES_Usb_Container_Header;


    // Property Code
    MtpPutLittleEndianINT16InX(pBuf, pBuf_offset, PropId);
    pBuf_offset += 2;

    // Data Type 
    wVal = PTP_DATATYPE_AUINT8;
    MtpPutLittleEndianINT16InX(pBuf, pBuf_offset, wVal);
    pBuf_offset += 2;

    // Get/Set value.
    PutByteInArrayInX(pBuf, pBuf_offset, PTP_PROPGETSET_GETSET);
    pBuf_offset += 1;

    // Create an array of 1 zero elemenrt for default value
    MtpPutLittleEndianINT32InX(pBuf, pBuf_offset, 1);
    pBuf_offset += 4;
    // Default Value;
    MtpPutLittleEndianINT16InX(pBuf, pBuf_offset, 0);
    pBuf_offset += 1;

    // set the size of current value array
    MtpPutLittleEndianINT32InX(pBuf, pBuf_offset, AA_ACTIVATION_RECORD_SIZE_BYTES);   
    pBuf_offset += 4;

    //Set how many bytes are already added to pBuf
    header_offset = pBuf_offset;
    
    //Set the total number of bytes to transfer = Header + Activaction + Form Flag (added at the end)
    dwBytesToWrite = pBuf_offset + AA_ACTIVATION_RECORD_SIZE_BYTES + FORM_FLAG_SIZE_BYTES;
    
    // get the property value and set it as current value in the property descriptor
    if( AAGetActivationRecords(0, AA_ACTIVATION_RECORD_SIZE_BYTES, (int *)&g_TempPtpString) != SUCCESS )
    {
        //Invalid Activation Data; set flag
        bValidActivationData = FALSE;
    }

    // set the size of packet, must be located at pBuf[0] and only the 1st time through the loop.
    MtpPutLittleEndianINT32InX(pBuf, 0, dwBytesToWrite);

done:

    if( pHandle->MtpResponse.Status != PTP_RESPONSECODE_OK )
    {
        // we got an error already no need to do anything simply return;
        return TRUE;
    }

    //The activation data is larger than 512 bytes so multiple USB sends are needed.
    while(dwBytesToWrite > 0)
    {       
        //Calculate the number of bytes to send for the transaction
        if(dwBytesToWrite >= TRANSFER_BUFFER_SIZE)
        {
            bytes_to_send = TRANSFER_BUFFER_SIZE;
        }
        else
        {
            bytes_to_send = dwBytesToWrite;
            bLastPacket = TRUE;
        }    
        //transfer activation record data to pBuf.  Data may already exist in 
        //the buffer if the 1st time through so subtract the header information.      
	    for(i=0; i<(bytes_to_send - header_offset); i++)
	    {
	        byte = GetByteFromArrayInY((void _Y *)&g_TempPtpString, activation_offset);
            //Fill buffer with Activation data, but if it is Invalid, fill buffer with 0's.  
            //P4S v1.2 expects data to be sent, not an error.
            if(bValidActivationData)
            {
                PutByteInArrayInX(pBuf, pBuf_offset, byte);
            }
            else
            {
                byte = 0;
                PutByteInArrayInX(pBuf, pBuf_offset, byte);
            }
            pBuf_offset += 1;
            activation_offset += 1;
        }	

        if(bLastPacket)
        {
           PutByteInArrayInX(pBuf, pBuf_offset, PTP_FORMFLAGS_NONE);
        }
        
        SysAssignObject(&(pHandle->pDevice->TransferObject),ISR_OWNER);
        // only wait for object if we think we successfully sent the data.
        if (usb_device_send_data(pHandle->pDevice->wBulkInEndpointNum,
                        pHandle->pDevice->CommandInBuffer, 0,
                        (USHORT)(bytes_to_send & 0x00FFFF),
                        COMMAND_HANDLER|((WORD)pHandle->pDevice), FALSE) == USB_OK)
        {

            while(!g_bHostCancelling)
		    {
			    if(SysWaitOnObject(10, &(pHandle->pDevice->TransferObject)))
    			    break;	
		    }
            if (g_bHostCancelling) // have we been cancelled or reset? 
            {
                //SystemHalt();
                pHandle->MtpResponse.Status = PTP_RESPONSECODE_TRANSACTIONCANCELLED;
                goto done;
            }            

        }
        else
        {
            //
            // Host Cancelled data-in transfer
            //
            pHandle->pDevice->Phase = DEVICE_PHASE_NOTREADY;
            // set the response
            pHandle->MtpResponse.Status = PTP_RESPONSECODE_INCOMPLETETRANSFER;
            goto done;
        }
        
        header_offset = 0;   //reset the header offset
        pBuf_offset = 0;     //reset the output buffer offset
        dwBytesToWrite -= bytes_to_send;  //subtract the number of bytes that were sent
        
        //If all the bytes have been sent, send out a null packet.
        if( dwBytesToWrite <= 0 )
        {
            // if this is the last packet
            MtpLastPacketHandler(pHandle->pDevice->wBulkInEndpointNum, USB_IN, 
                bytes_to_send, COMMAND_HANDLER|((WORD)pHandle->pDevice), &(pHandle->pDevice->TransferObject));
        }

        // set the response
        pHandle->MtpResponse.Status = PTP_RESPONSECODE_OK;
    }

    return TRUE;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : INT MtpAudibleGetPropValueHandler (SMtpHandler *pHandle)
//! This function will be called by getDevicePropValueMtpHandle 
//! 
//! \param [in][out] pHandle
//!     a pointer to SMtpHandler.
//!
//! \returns nothing but saves the result in pHandle->MtpResponse.Status
//!
int MtpAudibleGetPropValueHandler( SMtpHandler *pHandle)
{
    WORD wVal=0;
    INT i=0;
	WORD byte=0;
    WORD total_bytes_to_send=0;
    WORD bytes_to_send=0;
    WORD header_offset=0;
    WORD _X *pBuf = (WORD _X *)(pHandle->pDevice->CommandInBuffer);
    WORD pBuf_offset=0;
    WORD activation_offset=0;
    
    pHandle->MtpResponse.Status = PTP_RESPONSECODE_OK;

    // Clear the return buffer
    for(i=0; i<TRANSFER_BUFFER_SIZE;i++)
        pBuf[i] = 0;

    // leave the first 4 bytes for the length put in at the end
    MtpPutLittleEndianINT16InX(pBuf, 4, DataBlock);
    MtpPutLittleEndianINT16InX(pBuf, 6, pHandle->pDevice->PtpCmd.Code);
    MtpPutLittleEndianINT32InX(pBuf, 8, pHandle->pDevice->PtpCmd.TransactionID);
    pBuf_offset = SIZE_IN_BYTES_Usb_Container_Header;

    // set the size of current value array
    MtpPutLittleEndianINT32InX(pBuf, pBuf_offset, AA_ACTIVATION_RECORD_SIZE_BYTES);
    pBuf_offset += 4;

    //Calculate the total number of bytes that will be sent.
    total_bytes_to_send = pBuf_offset + AA_ACTIVATION_RECORD_SIZE_BYTES;
    
    //Calculate the offset that are already in the transfer buffer
    header_offset = pBuf_offset;
    
    // get the property value.  Use &g_TempPtpString as a temporary storage container for the data.
    if( AAGetActivationRecords(0, AA_ACTIVATION_RECORD_SIZE_BYTES, (int *)&g_TempPtpString) != SUCCESS )
    {
        //pHandle->MtpResponse.Status = PTP_RESPONSECODE_GENERALERROR;
        //goto done;
    }

    // set the size of packet, must be located at pBuf[0] and only the 1st time through the loop.
    MtpPutLittleEndianINT32InX(pBuf, 0, total_bytes_to_send);

    //The activation data is larger than 512 bytes so multiple USB sends are needed.
    while(total_bytes_to_send > 0)
    {       
        //Calculate the number of bytes to send for the transaction
        if(total_bytes_to_send > TRANSFER_BUFFER_SIZE)
            bytes_to_send = TRANSFER_BUFFER_SIZE;
        else
            bytes_to_send = total_bytes_to_send;
            
        //transfer activation record data to pBuf.  Data may already exist in 
        //the buffer if the 1st time through so subtract the header information.      
	    for(i=0; i<(bytes_to_send - header_offset); i++)
	    {
	        byte = GetByteFromArrayInY((void _Y *)&g_TempPtpString, activation_offset);
            PutByteInArrayInX(pBuf, pBuf_offset, byte);
            pBuf_offset += 1;
            activation_offset += 1;
        }	
        
        SysAssignObject(&(pHandle->pDevice->TransferObject),ISR_OWNER);
        // only wait for object if we think we successfully sent the data.
        if (usb_device_send_data(pHandle->pDevice->wBulkInEndpointNum,
                        pHandle->pDevice->CommandInBuffer, 0,
                        (USHORT)(bytes_to_send & 0x00FFFF),
                        COMMAND_HANDLER|((WORD)pHandle->pDevice), FALSE) == USB_OK)
        {

            while(!g_bHostCancelling)
		    {
			    if(SysWaitOnObject(10, &(pHandle->pDevice->TransferObject)))
    			    break;	
		    }
            if (g_bHostCancelling) // have we been cancelled or reset? 
            {
                //SystemHalt();
                pHandle->MtpResponse.Status = PTP_RESPONSECODE_TRANSACTIONCANCELLED;
                goto done;
            }            

        }
        else
        {
            //
            // Host Cancelled data-in transfer
            //
            pHandle->pDevice->Phase = DEVICE_PHASE_NOTREADY;
            // set the response
            pHandle->MtpResponse.Status = PTP_RESPONSECODE_INCOMPLETETRANSFER;
            goto done;
        }
        
        header_offset = 0;   //reset the header offset
        pBuf_offset = 0;     //reset the output buffer offset
        total_bytes_to_send -= bytes_to_send;  //subtract the number of bytes that were sent
        
        //If all the bytes have been sent, send out a null packet.
        if( total_bytes_to_send <= 0 )
        {
            // if this is the last packet
            MtpLastPacketHandler(pHandle->pDevice->wBulkInEndpointNum, USB_IN, 
                bytes_to_send, COMMAND_HANDLER|((WORD)pHandle->pDevice), &(pHandle->pDevice->TransferObject));
        }

        // set the response
        pHandle->MtpResponse.Status = PTP_RESPONSECODE_OK;
    }
done:
        //response should already be set inside pHandle->MtpResponse.Status
        return TRUE;

}
#endif //AUDIBLE

