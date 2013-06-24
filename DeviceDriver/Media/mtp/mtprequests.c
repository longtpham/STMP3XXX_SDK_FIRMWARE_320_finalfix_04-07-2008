/**
***
*** Filename: mtprequests.c
*** Description: Handles MTP Requests
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
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel.h"
#include "filesystem.h"
#include "sysirq.h"
#include "icoll.h"
#include "usb_api.h"
#include "..\usb\usb_dci.h"
#include "..\usb\usb_internal.h"
#include "mtp.h"
#include "mtprequests.h"
#include "ptp.h"
#include "stringlib.h"
#include "MtpHandler.h"
#include "usbmscMtpSwitch.h"

////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////

    extern SMtpHandler MtpHandler;
extern USBMTP_DEVICE UsbMtpDevice;
extern USB_SetupData g_SetupData[];
extern DWORD g_dwTransactionIdToCancel;
extern BOOL g_bHostCancelling;
extern WORD _X Descriptor[];
extern MS_OS_EXTENDED_CONFIGURATION_DESCRIPTOR Transfer_MSOS_Extended_Configuration;
extern volatile WORD g_wActivityState;

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
void _reentrant UsbRequestHandlerMtpReset(BYTE bSetup, USB_SetupData *  pSetup) ;
_asmfunc void PutByteInArrayInX( void _X * p,int n, int newchar);
static void _reentrant MtpPutLittleEndianINT32IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue);
static void _reentrant MtpPutLittleEndianINT16IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

WORD g_CancelSetupCount = 0;
WORD g_CancelDataPhaseCount = 0;
BYTE bDataPhaseCancel = FALSE;
BYTE bStatusPhaseCancel = FALSE;
BYTE bDataPhaseStatus = FALSE;
BOOL g_bUsbPoll = FALSE;
WORD g_wCancelCRC = 0;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//  CRC8 Lookup Table.
//  This version is "packed", with three one-byte entries per WORD.
/////////////////////////////////////////////////////////////////////////////////
// TOVERIFY: 3.0 had this in X, but its moved to Y as in lbmlc
extern WORD _X crcvalues[];  // MOVED TO _X AT SDK 3.111. 3.110 had this in _Y. 
 

///////////////////////////////////////////////////////////////////////////////////
//> Name:        MTP_CRC8
//
//  Type:        Function
//  Description: returns the CRC-8 for a given sequence of bytes
//
//  Inputs:      wData - Points to start of byte stream (packed into words in _X)
//               wOffset - Byte offset into data buffer (may b>2, to skip over stuff)
//               wBcount - the number of bytes over which this routine should execute
//  Outputs:     an unsigned 8 bit CRC8 value
//  Notes:       
//               This is typically used to detect errors in bytes in the Redundant Area,
//               but nothing is assumed about layout.  The "data" is assumed to be 
//               contiguous packed bytes, but they may start at any point after the
//               WORD data pointer (wData).  So you can pass it a WHOLE SECTOR or just
//               a Redundant Area buffer, with wOffset set appropriately.
//               
//<
/////////////////////////////////////////////////////////////////////////////////

WORD _reentrant MTP_CRC8(WORD _X * wData, WORD wOffset, WORD wBcount)
{
    int
        i;
    WORD
      temp,
      crc,
      newindex;
    WORD wFFcnt = 0;

    for(crc=0, i=0; i < wBcount; i++)
    {
        temp = GetByteFromArrayInX(wData, i + wOffset);
        newindex = crc ^ temp;
        //crc = GetByteFromArrayInY(crcvalues, newindex); // crcvalues array alloc was changed to Y on LB proj in this file at top so get byte from array in Y now. TOVERIFY if that's ok.
           crc = GetByteFromArrayInX(crcvalues, newindex); // Now at 3.111 start getting it from X since Y is short. 3.110 used above line.

        if (temp == 0xFF)
          wFFcnt++;
    }



    /////////////////////////////////////////////////////////////////////////////////
    //  If the RA is all FFs, it's probably erased, so the CRC byte will contain 0xFF.
    //  To match that, we force the computation to 0xFF, here.
    /////////////////////////////////////////////////////////////////////////////////

    if (wFFcnt == wBcount)
      crc = 0xFF;

    return (crc);
}

// These functions have to be defined here because the non-static counter-parts are funclets
// and cannot be called from ISR

static void _reentrant MtpPutLittleEndianINT32IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue)
{
    int i;
    for(i=0;i<4;i++)
    {
//        PutByteInArrayInX(pBuffer,iStartIndex + 3 - i,lValue & 0xff);
        PutByteInArrayInX(pBuffer,iStartIndex + i,lValue & 0xff);
        lValue=lValue>>8;
    }
}


static void _reentrant MtpPutLittleEndianINT16IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue)
{
    int i;
    for(i=0;i<2;i++)
    {
//        PutByteInArrayInX(pBuffer,iStartIndex + 1 - i,lValue & 0xff);
        PutByteInArrayInX(pBuffer,iStartIndex + i,lValue & 0xff);
        lValue=lValue>>8;
    }
}

static long _reentrant LocalMtpGetLittleEndianINTInXCommon(void _X *pBuffer, int iStartIndex, int Size)
{
    long lValue=0;
    int i;
    for(i=(Size-1);i>=0;i--)
    {
        lValue = (lValue << 8) | GetByteFromArrayInX(pBuffer,iStartIndex+i);
    }
    
    return lValue;
}

#define LocalMtpGetLittleEndianINT16InX(pBuffer, iStartIndex) LocalMtpGetLittleEndianINTInXCommon((pBuffer), (iStartIndex), (2))
#define LocalMtpGetLittleEndianINT32InX(pBuffer, iStartIndex) LocalMtpGetLittleEndianINTInXCommon((pBuffer), (iStartIndex), (4))

void _reentrant MTPRequestHandler(BOOL bSetup, BYTE btDirection, WORD _X *pbtBuffer, WORD wLength,WORD wPrivateData)
{
    SyncObject *pObject;
    USB_SetupData * pSetup =  &g_SetupData[0];
    USB_PTP_STATUSREQ_DATA StatusData = {0};
    USB_PTP_CANCELREQ_DATA CancelData = {0};
    USBMTP_DEVICE * pDev = &UsbMtpDevice;
    WORD i, j;

    if(bSetup)
    {
        usb_device_read_setup_data(0);

    }

    switch(pSetup->RequestType.I)
    {
        case HOST_TO_DEVICE | VENDOR | RECIPIENT_DEVICE:
            usb_device_finish_transaction(0);
            usb_device_recv_data(0, 0, 0, pSetup->wLength, 0, FALSE);
               break;

        case DEVICE_TO_HOST | VENDOR | RECIPIENT_DEVICE:
				SystemHalt();
            break;

        case DEVICE_TO_HOST | VENDOR | RECIPIENT_INTERFACE:
            switch(pSetup->btRequest)
            {
                case MSOS_VENDORCODE_TO_GET_MS_DESCRIPTOR:
                    if (pSetup->wIndex == MSOS_FEATURE_INDEX_EXTENDED_PROP_DESC)
                         {
                            // Not implemented
                            usb_device_finish_transaction(0);
                           }
                    break;

                default:
                        //don't know how to handle
                        SystemHalt();
                    break;
            }
            break;
        
        case HOST_TO_DEVICE | CLASS | RECIPIENT_INTERFACE:
            
            switch(pSetup->btRequest)
            {
                
                //////////////////////////////////////////////////////////////////////////////////////
                //                             Cancel Request                                            //
                //////////////////////////////////////////////////////////////////////////////////////
                case USB_PTPREQUEST_CANCELIO:
                    
                    // Setup Phase
                    // Verify command format and start data phase 6-bytes
                    if(bSetup)
                    {
                        usb_device_finish_transaction(0);
                        
                        //
                        // Verify the contents in Setup data packet: wLength 
                        //
                        if ((pSetup->wValue == 0x00) && (pSetup->wIndex == 0x00) && (pSetup->wLength == 0x06))
                        {                     
                            // Do NOT remove this delay.  It seems superfluous, but it 
                            // fixes an unexplainable NACK'ing of Status IN transaction by
                            // ARC core.
                            for(i=0;i<1000;i++)
                            {
                                bDataPhaseCancel = TRUE;
                            }
                            
                            // Get the 6 byte data, the first 2 bytes must be USB_PTPCANCELIO_ID
                            usb_device_recv_data(0, Descriptor, 0, pSetup->wLength, 0, TRUE);
                            bDataPhaseCancel = TRUE;
                        }
                        else
                        {
                            //
                            // Stall End Point 0
                            //
                            SystemHalt();
                            usb_device_stall_endpoint(0, USB_IN);
                        }

                        break;
                    }
                    
                    if(bDataPhaseCancel)
                    {    // Data and Status Phase
                        
                        bDataPhaseCancel = FALSE;
                        //usb_device_finish_transaction(0);

                        CancelData.wCancelIOCode = LocalMtpGetLittleEndianINT16InX(Descriptor, 0);
                        CancelData.TransactionId = LocalMtpGetLittleEndianINT32InX(Descriptor, 2);
                        if ( CancelData.wCancelIOCode != USB_PTPCANCELIO_ID)
                        {
                            //
                            // Stall End Point 0
                            //
                            usb_device_stall_endpoint(0, USB_IN);
                        }
                        else
                        {
                            // if we are coming out of a stalled phase and MTP is cancelling
                            // the previous transaction then don't wait for this flag to be
                            // turned off for an OK device status request.
                            if (pDev->Phase != DEVICE_PHASE_STALLED)
                            {
                                g_bHostCancelling = TRUE;

                                // Set the flag to enable polling in forground
                                g_bUsbPoll = TRUE;
                            }
                            else
                            {
                                pDev->Status = DEVICE_STATUSOK;
                                pDev->Phase = DEVICE_PHASE_IDLE;
                            }

                            g_dwTransactionIdToCancel = (CancelData.TransactionId );
                            // Prepare status phase
                            usb_device_send_data(0, 0, 0, 0, FALSE,0);
                            bStatusPhaseCancel = TRUE;
                          
                            // Compute CRC
                            g_wCancelCRC = MTP_CRC8(Device1CommandOutBuffer,0,TRANSFER_BUFFER_SIZE*sizeof(Device1CommandOutBuffer[0]));
                             
                        } /* else USB_PTPCANCELIO_ID == CancelData.wCanelIOCode */

                        break;
                    }

                    if(bStatusPhaseCancel)
                    {
                        bStatusPhaseCancel = FALSE;
                    }

                break;    
                
                case USB_PTPREQUEST_RESET:
                    usb_device_finish_transaction(0);
                    // Set the flag to be cleared in call from MtpCmdProcessor
                    // It will also close the session
                    MtpHandler.bMTPReset = TRUE;
                    usb_device_send_data(0, 0, 0, 0, FALSE,0);
                    break;
                default:
                    break;
            }
            break;

        case DEVICE_TO_HOST | CLASS | RECIPIENT_INTERFACE:
            switch (pSetup->btRequest)
            {
                case USB_PTPREQUEST_GETEVENT:
                    usb_device_finish_transaction(0);
                    break;

                //////////////////////////////////////////////////////////////////////////////////////
                //                             Get Status Request                                         //
                //////////////////////////////////////////////////////////////////////////////////////
                case USB_PTPREQUEST_GETSTATUS:

                    if(bSetup)
                    {

                        usb_device_finish_transaction(0);
                        bDataPhaseStatus = TRUE;

                        //
                        // Verify the contents in Setup data packet
                        //
                        if (pSetup->wLength)
                        {                         
                            DWORD dwParams = 0;
                            //
                            // Prepare the Device Status Data Packet
                            //

                            // Check the bulk endpoints to see if they are stalled
                            if (usb_device_get_endpoint_status(UsbMtpDevice.wBulkInEndpointNum) == USB_ENDPOINT_STATUS_STALLED)
                            {
                                StatusData.Params[dwParams] = UsbMtpDevice.wBulkInEndpointNum | 0x80; 
                                dwParams ++;
                            }
                            if (usb_device_get_endpoint_status(UsbMtpDevice.wBulkOutEndpointNum) == USB_ENDPOINT_STATUS_STALLED)
                            {
                                StatusData.Params[dwParams] = UsbMtpDevice.wBulkOutEndpointNum; 
                                dwParams ++;
                            }

                            if (dwParams)
                            {

                                StatusData.wLength = 4 + (dwParams * 4);
                                StatusData.Code = PTP_RESPONSECODE_TRANSACTIONCANCELLED;
                            }
                            else
                            {
                                StatusData.wLength = 4;
                                if (g_bHostCancelling || (g_wActivityState != MTP_IDLE))
                                    StatusData.Code = PTP_RESPONSECODE_DEVICEBUSY;
                                else 
                                {
                                    if ( UsbMtpDevice.Status != DEVICE_STATUSOK )
                                        UsbMtpDevice.Status = DEVICE_STATUSOK;

                                    StatusData.Code = PTP_RESPONSECODE_OK;
                                }
                            }
                            
                            // Build the Data Response
                            MtpPutLittleEndianINT16IntoByteBufferInX( (void _X *)Descriptor, 0, (long)StatusData.wLength );
                            MtpPutLittleEndianINT16IntoByteBufferInX( (void _X *)Descriptor, 2, (long)StatusData.Code );
                            MtpPutLittleEndianINT32IntoByteBufferInX( (void _X *)Descriptor, 4, (long)StatusData.Params[0] );
                            MtpPutLittleEndianINT32IntoByteBufferInX( (void _X *)Descriptor, 8, (long)StatusData.Params[1] );

                            //
                            // Send the data packet
                            //
                            usb_device_send_data(0, Descriptor, 0, StatusData.wLength, 0, TRUE);

                        }  // end if verify length of request
                        else
                        {
                            usb_device_stall_endpoint(0, USB_IN);
                        }

                    } // if(bSetup)

                    else if(bDataPhaseStatus)
                    {
                        bDataPhaseStatus = FALSE;
                        // Prepare dTD for the status phase
                        usb_device_recv_data(0, 0, 0, 0, FALSE,0);
                    }

                    break;

                default:
                    SystemHalt();
                    break;
            }
            break;
        
        default:
            ch9ServiceEp0(bSetup, btDirection, pbtBuffer, wLength,wPrivateData);

    } // end of switch(pSetup->RequestType.I)

}


