////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : usb20ch9.c
// Description : USB2.0 Chapter Application skeleton
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "usb_api.h"
#include "..\..\usb_internal.h"
#include "..\..\usb_dci.h"
#include "..\usb20ch9.h"

#pragma optimize 1

SHORT _X usDataToSend;
BYTE _X btStatus;

// Global variable definitions
volatile USHORT usTestModeIndex;

extern USB_SetupData g_SetupData[];

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9GetStatus
//
//   Type:          Function
//
//   Description:   Chapter 9 get status command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9GetStatus(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */
    
    USHORT usStatus;
    BYTE btIndex;
    BOOL bRequestValid;

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup)
    {
        switch (pSetup->RequestType.B.Recipient)
        {

            case USB_REQUEST_RECIPIENT_DEVICE:
                // Device status request
                usb_device_get_status((BYTE)USB_STATUS_DEVICE_STATE, &usStatus);

                // Clear any undesired bits
                usDataToSend = usStatus;
                
                usDataToSend &= 0x0003;
                
                // Send the data
                usb_device_send_data(0, (WORD _X *)&usDataToSend, 0, (USHORT)2, TRUE,0); 
                break;

            case USB_REQUEST_RECIPIENT_INTERFACE:
                // Interace status request
                usDataToSend = 0x0000;

                // Send the data
                usb_device_send_data(0, (WORD _X *)&usDataToSend, 0, (USHORT)2, TRUE,0); 
                break;

            case USB_REQUEST_RECIPIENT_ENDPOINT:
                // Endpoint status request
                usb_device_get_status(USB_STATUS, &usStatus);
                
                btIndex = pSetup->wIndex & 0x0f; 
                bRequestValid = FALSE;

                if(btIndex == 0)
                    bRequestValid = TRUE;
                else if((btIndex < MAX_NUM_EP) && (usStatus == USB_STATE_CONFIGURED))
                    bRequestValid = TRUE;

                if(bRequestValid == TRUE)
                {
                    // Get endpoint status
                    usb_device_get_status((USB_STATUS_ENDPOINT + btIndex), &usStatus);

                    // Clear undesried bits
                    usDataToSend = usStatus;
                    
                    usDataToSend &= 0x0001;

                    // Prepare to send data
                    usb_device_send_data(0, (WORD _X *)&usDataToSend, 0, (USHORT)2, TRUE,0);
                }
                else
                {
                    usb_device_stall_endpoint(0, USB_IN);
                    return;
                }

                break;  

            default:
                // Unknown request
                usb_device_stall_endpoint(0, USB_IN);
                return;
      } /* Endswitch */
      
        // Prepare dTD for the status phase
        usb_device_recv_data(0, 0, 0, 0, FALSE,0);
    } /* Endif */

    return;
} /* Endbody */





////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9ClearFeature
//
//   Type:          Function
//
//   Description:   Chapter 9 clear feature command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
//#define USBTEST_FOR_USBCV_1_3
#ifdef USBTEST_FOR_USBCV_1_3
#include "usbmsc.h"
extern BOOL Need_Stall_Buck_EP_For_Waiting_MSC_Reset;
#endif
void _reentrant ch9ClearFeature(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */
    BYTE btIndex, btDirection = USB_OUT, i;
    BOOL bRequestValid;
    USHORT usStatus;

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup) 
    {
        switch (pSetup->RequestType.B.Recipient)
        {
            case USB_REQUEST_RECIPIENT_DEVICE:
                // Clear feature device
                if (pSetup->wValue == 1)
                {
                    // clear remote wakeup 
                    
                    // Get the state of the device
                    usb_device_get_status((BYTE)USB_STATUS_DEVICE_STATE, &usStatus);

                    // Set the remote feature
                    usStatus &= ~USB_REMOTE_WAKEUP;
                    usb_device_set_status((BYTE)USB_STATUS_DEVICE_STATE, usStatus);
                } /* Endif */
                else
                {
                    usb_device_stall_endpoint(0, USB_IN);
                    return;
                }
                break;
         
            case USB_REQUEST_RECIPIENT_ENDPOINT:
                usb_device_get_status(USB_STATUS, &usStatus);

                if(pSetup->wIndex & 0x80)
                {
                    btDirection = USB_IN;
                }
                
                btIndex = pSetup->wIndex & 0x0f; 

                bRequestValid = FALSE;

                if(btIndex == 0)
                    bRequestValid = TRUE;
                else if((btIndex < MAX_NUM_EP) && (usStatus == USB_STATE_CONFIGURED))
                    bRequestValid = TRUE;


                if(bRequestValid == TRUE)
                {
                    // STMP3747 - latest FW hangs Mac - an error would stall the endpoint.  Instead of
                    // clearing only the stalled endpoint, the Mac is sending a clear Endpoint 0 command.
                    // We'll assume that 0 means all devices so we'll cycle through all the endpoints and 
                    // if an endpoint is stalled, we'll set the appropriate bClearFeature flag.
                    if(btIndex == 0)
                    {
                        for(i=0; i<MAX_NUM_EP; i++)
                        {
                            usb_device_unstall_endpoint(i, USB_IN); 
                            usb_device_unstall_endpoint(i, USB_OUT); 
                        }
                    }
                    else
                    {
#ifdef USBTEST_FOR_USBCV_1_3
                        if(TRUE == Need_Stall_Buck_EP_For_Waiting_MSC_Reset)
                        {
                            if((btIndex == UsbMscDevice[0].wBulkOutEndpointNum) || (btIndex == UsbMscDevice[0].wBulkInEndpointNum))
                                break;
                        }
#endif
                        usb_device_unstall_endpoint(btIndex, btDirection); 
                    }
                }    
                else
                {
                    usb_device_stall_endpoint(0, USB_IN);
                    return;
                }

                break;

            default:
                // Unknown request
                usb_device_stall_endpoint(0, USB_IN);
                return;

        } /* Endswitch */
      
        // Prepare dTD for the status phase
        usb_device_send_data(0, 0, 0, 0, FALSE,0);
    } /* Endif */

    return;
} /* Endbody */





////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9SetFeature
//
//   Type:          Function
//
//   Description:   Chapter 9 set feature command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SetFeature(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */
    USHORT usStatus;
    USHORT usDeviceState;
    BYTE btIndex, btDirection = USB_OUT;
    BOOL bRequestValid;
    
    // Get the device status
    usDeviceState = 0;
    usb_device_get_status(USB_STATUS, &usDeviceState);

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup)
    {
        switch (pSetup->RequestType.B.Recipient)
        {
            case USB_REQUEST_RECIPIENT_DEVICE:
                // Set feature device
                switch (pSetup->wValue)
                {
                    case 1:
                    // Set remote wakeup                    

                    // Get the state of the device
                    usb_device_get_status((BYTE)USB_STATUS_DEVICE_STATE, &usStatus);

                    // Set the remote feature
                    usStatus |= USB_REMOTE_WAKEUP;
                    usb_device_set_status((BYTE)USB_STATUS_DEVICE_STATE, usStatus);

                    break;
                    
                    case 2:
                    // Test mode
                    
                        usb_device_get_status((BYTE)USB_STATUS_DEVICE_SPEED, &usStatus);
               
                        
                        if(((pSetup->wIndex) & 0xFF) || (usStatus != (USHORT)USB_SPEED_HIGH)) 
                        { // Test mode incorrect
                            usb_device_stall_endpoint(0, USB_IN);
                            return;
                        } /* Endif */

                        if ((usDeviceState == USB_STATE_CONFIGURED) || 
                            (usDeviceState == USB_STATE_ADDRESSED) || 
                            (usDeviceState == USB_STATE_DEFAULT)) 
                        {
                            // Do not switch to test mode yet.
                            // The switch must happen after the status phase.
                            usb_device_set_status(USB_STATUS_DEVICE_TEST, (USHORT)TRUE);
                            usTestModeIndex = ((pSetup->wIndex)>>8) & 0xFF;
                        }
                        else
                        {
                            usb_device_stall_endpoint(0, USB_IN);
                            return;
                        } /* Endif */
                        break;

                default:
                    usb_device_stall_endpoint(0, USB_IN);
                    return;
            } /* Endswitch */
            break;
            
            case USB_REQUEST_RECIPIENT_ENDPOINT:

                if(pSetup->wIndex & 0x80)
                {
                    btDirection = USB_IN;
                }

                btIndex = pSetup->wIndex & 0x0f; 

                bRequestValid = FALSE;

                if((btIndex < MAX_NUM_EP) && (usDeviceState == USB_STATE_CONFIGURED))
                    bRequestValid = TRUE;

                if(bRequestValid == TRUE)
                {
                    usb_device_stall_endpoint(btIndex, btDirection); 
                }                    
                else
                {
                    usb_device_stall_endpoint(0, USB_IN);
                    return;
                }

                break;

            default:
                usb_device_stall_endpoint(0, USB_IN);
                return;
        } /* Endswitch */
      
        // Prepare dTD for the status phase
        usb_device_send_data(0, 0, 0, 0, FALSE,0);
    }
    else 
    {
        usb_device_get_status(USB_STATUS_DEVICE_TEST, &usDeviceState);
        
        if (usDeviceState == (USHORT)TRUE) 
        {
            // Now enter test mode
            usb_device_set_status(USB_STATUS_TEST_MODE, usTestModeIndex);
            
            // Inform Application
            usb_device_call_service(USB_SERVICE_TEST, FALSE, 0, 0, 0,0);
            
        } 
    } 
    
    return;
} /* Endbody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9SetAddress
//
//   Type:          Function
//
//   Description:   Chapter 9 set address command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//      The 1st time we enter in this function, the bSetup flag should be TRUE.
//      Therefore we save the address locally and prepare a dTD for the status phase
//      (TX packet of 0 length).
//      The 2nd time we enter the function, due to the status phase OUT packet completed
//      the bSetup flag should be clear. Therefore we save the new address, and so on ...
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SetAddress(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */
    static BYTE btNewAddress;

    if (bSetup)
    {   // Setup phase

        // Get the address from the setup buffer 
        btNewAddress = pSetup->wValue;
      
        // Prepare the status phase
        usDataToSend = 0x00;
        
        usb_device_send_data((BYTE)0, (WORD _X *)&usDataToSend, (BYTE)0, 0, FALSE,0);
    } 
    else
    {   // Status phase
        
        // Program the new address
        usb_device_set_status((BYTE)USB_STATUS_ADDRESS, (USHORT)btNewAddress);
        
        if(btNewAddress != 0)       // if address not 0, then set device state to addressed
            usb_device_set_status((BYTE)USB_STATUS, (USHORT)USB_STATE_ADDRESSED);
        else                        // if address is 0 then set device state to default
                                    // per USB specs   
            usb_device_set_status((BYTE)USB_STATUS, USB_STATE_DEFAULT);
    }   // Endif            
} /* Endbody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9GetDescription
//
//   Type:          Function
//
//   Description:   Chapter 9 get descriptor command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//     The Device Request can ask for Device/Config/string/interface/endpoint
//     descriptors (via wValue). We then post an IN response to return the
//     requested descriptor.
//     And then wait for the OUT which terminates the control transfer.
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9GetDescription(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */
   USHORT   usLength, usDescLength;
   USHORT   usSpeed;
   BYTE _X * pbtDesc;
   WORD wDescriptorType;
   WORD wDescriptorIndex;
   

   wDescriptorType = (pSetup->wValue&0xff00)>>8;
   wDescriptorIndex= (pSetup->wValue&0xff);
    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup) 
    {        
        // Load the appropriate string depending on the descriptor requested.
        switch (wDescriptorType)
        {

            case DESCRIPTOR_TYPE_STRING:

            case DESCRIPTOR_TYPE_DEVICE:
            // Prepare Device descriptor transfer
            
            case DESCRIPTOR_TYPE_CONFIG:
            // Prepare Configuration Descriptor transfer.
            // A configuration descriptor includes all related interfaces descriptor (1 or more per configuration)
            // and all related endpoints (1 or more per interface)
                            
            case DESCRIPTOR_TYPE_DEVICE_QUALIFIER:

            case DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG:      

                usb_device_get_status(USB_STATUS_DEVICE_SPEED, &usSpeed);
                
                pbtDesc = GetDescriptor(wDescriptorType, (BYTE)usSpeed, wDescriptorIndex);
               
                if(pbtDesc)     // If Descriptor exists
                {                
                    usLength = pSetup->wLength;
                    if(!usLength)
                    {
                        usb_device_stall_endpoint(0, USB_IN);
                    }
                    else
                    {
                        if( (wDescriptorType == DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG) ||
                            (wDescriptorType == DESCRIPTOR_TYPE_CONFIG) )
                        {
                            usDescLength = ((pbtDesc[0] & 0xff0000) >> 16)  + ((pbtDesc[1] & 0x0000ff) >> 16);

                            if(usLength > usDescLength)
                                usLength = usDescLength;
                        }

                        else

                        {                          
                            if(usLength > (pbtDesc[0] & 0x0000ff))
                                usLength = (BYTE)(pbtDesc[0] & 0x0000ff);  
                        }                            

                        usb_device_send_data(0, (void _X* ) pbtDesc, 0, usLength, TRUE,0);
                    }
                }

                break;

            default:
                usb_device_stall_endpoint(0, USB_IN);
                return;

        } /* Endswitch */


        // Sometimes, we need to physically hold the usb device 
        // we do this by not sending the data phase here.
        // EP0 will NAK until we release the device again.
        if(usb_device_is_hold() == FALSE)
        {
            // Prepare dTD for the status phase
            usb_device_recv_data(0, 0, 0, 0, FALSE,0);
            
        }
        else
        {
            // We hold off a data phase, therefore we flagged it
            // the usb_device_release() function will take care of sending it
            usb_device_data_phase_required();
        }
            
    } /* Endif */
    
    return;
} /* Endbody */





////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9SetDescription
//
//   Type:          Function
//
//   Description:   Chapter 9 set descriptor command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//     The Device Request can ask for Device/Config/string/interface/endpoint
//     descriptors (via wValue). We then post an IN response to return the
//     requested descriptor.
//     And then wait for the OUT which terminates the control transfer.
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SetDescription(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */
    usb_device_stall_endpoint(0, USB_IN);
    return;
} /* Endbody */





////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9GetConfig
//
//   Type:          Function
//
//   Description:   Chapter 9 get configuration command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9GetConfig(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */
    USHORT usStatus;

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup) 
    {    
        // Get the device status
        usb_device_get_status(USB_STATUS, &usStatus);
        
        if(usStatus != USB_STATE_CONFIGURED)
            usDataToSend = 0x00;                // Return config 0
        else
        {        
            usb_device_get_status(USB_STATUS_CURRENT_CONFIG, &usStatus);
            usDataToSend = usStatus;
        }
        
        // Send the data
        usb_device_send_data(0, (WORD _X *)&usDataToSend, 0, (USHORT)1, TRUE,0); 

        // Prepare dTD for the status phase
        usb_device_recv_data(0, 0, 0, 0, FALSE,0);
    } /* Endif */
    
    return;
} /* Endbody */






////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9SetConfig
//
//   Type:          Function
//
//   Description:   Chapter 9 set configuration command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////

BOOL g_bUsbConfigured = FALSE;

void _reentrant ch9SetConfig(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */
    USHORT  usStatus;
    INT     i;
    
    // Get the device status
    usb_device_get_status(USB_STATUS, &usStatus);

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if(bSetup)
    {
        if((usStatus == USB_STATE_CONFIGURED) || (usStatus == USB_STATE_ADDRESSED))
        {
            // Unstall all stall endpoints per ch9 specs
            for(i=0 ; i < MAX_NUM_EP ; i++)
            {
                usb_device_get_status((USB_STATUS_ENDPOINT + i), &usStatus);
                if(usStatus == (USHORT)TRUE)
                    usb_device_set_status((USB_STATUS_ENDPOINT + i), FALSE);
            }                    
            
            if(pSetup->wValue == 0)
            { // If configuration value is 0, then go to ADDRESS State
#ifdef  USBTEST_FOR_USBCV_1_3
                //after this command  sould cancel the MSC bulk out request for CBW
                g_bUsbConfigured = FALSE;
#endif
                // Inform Application
                usb_device_call_service(USB_SERVICE_CHANGE_CONFIG, FALSE, 0, (WORD _X*)&btStatus, (USHORT)pSetup->wValue,0);
                
                // Clear the current config value
                usb_device_set_status(USB_STATUS_CURRENT_CONFIG, 0);
                
                // Set the device state to ADDRESS State
                usb_device_set_status(USB_STATUS, USB_STATE_ADDRESSED);
                
                // Prepare status phase
                usb_device_send_data(0, 0, 0, 0, FALSE,0);
#ifdef USBTEST_FOR_USBCV_1_3
                //release to UsbMscCbwProcessor() event
                ISRReleaseObject(&(UsbMscDevice[0].TransferObject));
#endif
            }
            else
            {
                
                // Inform Application
                btStatus = USB_OK;              // By default request is accepted
                usb_device_call_service(USB_SERVICE_CHANGE_CONFIG, FALSE, 0, (WORD _X*)&btStatus, (USHORT)pSetup->wValue,0);
                
                if(btStatus == USB_OK)
                {                
                
                    // Set the confing value
                    usb_device_set_status(USB_STATUS_CURRENT_CONFIG, (USHORT)pSetup->wValue);
            
                    g_bUsbConfigured = TRUE;
                    
                    // Set the device state to CONFIG State
                    usb_device_set_status(USB_STATUS, USB_STATE_CONFIGURED);

                    // Prepare status phase
                    usb_device_send_data(0, 0, 0, 0, FALSE,0);
                }                    
                else
                {                
                    // Invalid request
                    usb_device_stall_endpoint(0, USB_IN);
                }                    
            
            }
        }
        else
            // Invalid request
            usb_device_stall_endpoint(0, USB_IN);
    }    
    return;
} /* Endbody */






////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9GetInterface
//
//   Type:          Function
//
//   Description:   Chapter 9 get interface command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9GetInterface(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */
    USHORT usStatus;

    // Get the device status
    usb_device_get_status(USB_STATUS, &usStatus);

    if (usStatus != USB_STATE_CONFIGURED)
    {
        usb_device_stall_endpoint(0, USB_IN);
        return;
    } /* Endif */

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if(bSetup)
    {    
        usb_device_get_status(USB_STATUS_CURRENT_IFACE, &usStatus);

        usDataToSend = usStatus;

        usb_device_send_data(0, (WORD _X *)&usDataToSend, 0, (USHORT)1, TRUE,0); 

        // Prepare dTD for the status phase
        usb_device_recv_data(0, 0, 0, 0, FALSE,0);
    } /* Endif */

    return;
} /* Endbody */





////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9SetInterface
//
//   Type:          Function
//
//   Description:   Chapter 9 set interface command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SetInterface(BYTE bSetup, USB_SetupData * pSetup) 
{ /* Body */

    USHORT usStatus;
    INT     i;
    
    // Get the device status
    usb_device_get_status(USB_STATUS, &usStatus);

    if(usStatus != USB_STATE_CONFIGURED)
    {
       usb_device_stall_endpoint(0, USB_IN);
       return;
    } /* Endif */

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if(bSetup) 
    {

        // Unstall all stall endpoints per ch9 specs
        for(i=0 ; i < MAX_NUM_EP ; i++)
        {
            usb_device_get_status((USB_STATUS_ENDPOINT + i), &usStatus);
            if(usStatus == (USHORT)TRUE)
                usb_device_set_status((USB_STATUS_ENDPOINT + i), FALSE);
        }                    
        
        // Informs the  Application
        btStatus = USB_OK;              // By default request is accepted
        usb_device_call_service(USB_SERVICE_CHANGE_IFACE, FALSE, 0, (void _X*)&btStatus, (USHORT)pSetup->wValue,0);
        
        if(btStatus == USB_OK)
        {                
            // Set the interface value
            usb_device_set_status(USB_STATUS_CURRENT_IFACE, (USHORT)pSetup->wValue);

            // Prepare status phase
            usb_device_send_data(0, 0, 0, 0, FALSE,0);
        }
        else
        {
            usb_device_stall_endpoint(0, USB_IN);
        }
                        

    } /* Endif */

    return;
} /* Endbody */




////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9SynchFrame
//
//   Type:          Function
//
//   Description:   Chapter 9 synch frame command
//
//   Inputs:        
//                  bSetup : TRUE if setup phase
//                  pSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SynchFrame(BYTE bSetup, USB_SetupData* pSetup) 
{ /* Body */
   
    // None of our endpoints support this type of request
    if (bSetup) 
    {
        usb_device_stall_endpoint(0, USB_IN);
        return;
    }        

    return;
} /* Endbody */


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ch9ServiceEp0
//
//   Type:          Function
//
//   Description:   Handles completed transfers on endpoint 0
//
//   Inputs:        
//                  Not used
//   Outputs:       None
//
//   Notes:         
//                  The input parameters are here only to respect the Service
//                  function declaration in the service structure
//                  
//<
////////////////////////////////////////////////////////////////////////////////

void _reentrant ch9ServiceEp0(BOOL bSetup, BYTE btDirection, WORD _X *pbtBuffer, WORD wLength, WORD wPrivateData)
{ /* Body */

    USB_SetupData * pSetup =  &g_SetupData[0];
    
    // Is this the setup phase
    if (bSetup)
    {
        usb_device_read_setup_data(0);
        
        // This is to accomodate USB1.1 Stack on the STMP3410. This stack has no
        // means to know that the phase is a SETUP phase. Therefore, every 1st
        // rx on endpoint 0 is considered to be a SETUP phase and after the 
        // status phase a this function is called to finish the request.
        // In our case we finich the request as soon as we copied the setup bytes
        // from the dQH into the setup buffer.
        usb_dci_finish_transaction(0);
    }

    // Process the setup request
    switch (pSetup->RequestType.B.Type)
    {
        // case standard request
        case USB_REQUEST_TYPE_STANDARD:
            switch (pSetup->btRequest) 
            {
                case USB_REQUEST_STANDARD_GET_STATUS:
                    ch9GetStatus(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_CLEAR_FEATURE:
                    ch9ClearFeature(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_SET_FEATURE:
                    ch9SetFeature(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_SET_ADDRESS:
                    ch9SetAddress(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_GET_DESCRIPTOR:
                    ch9GetDescription(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_SET_DESCRIPTOR:
                    ch9SetDescription(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_GET_CONFIGURATION:
                    ch9GetConfig(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_SET_CONFIGURATION:
                    ch9SetConfig(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_GET_INTERFACE:
                    ch9GetInterface(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_SET_INTERFACE:
                    ch9SetInterface(bSetup, pSetup);
                    break;

                case USB_REQUEST_STANDARD_SYNCH_FRAME:
                    ch9SynchFrame(bSetup, pSetup);
                    break;

                default:
                    usb_device_stall_endpoint(0, USB_OUT);
                    break;

            } /* Endswitch */
         
        break;

        default:
            usb_device_stall_endpoint(0, USB_OUT);
            break;
         
   } /* Endswitch */
   
   return;
} /* Endbody */






