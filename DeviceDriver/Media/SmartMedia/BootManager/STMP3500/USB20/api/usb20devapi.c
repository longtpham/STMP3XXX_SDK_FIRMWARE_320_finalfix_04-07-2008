////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2001-2004
//
// File : usb20devapi.c
// Description : USB2.0 device API functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "usb20devapi.h"
#include "usb20.h"

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

extern xd_struct _X *_X dXD_Base_Address[];
extern volatile BOOL  bInIsr;
////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////
Dev_State_Struct _X stDeviceState;
Service_Struct      Services[MAX_NUMBER_SERVICES];       // USB 2.0 Services 
xd_struct _X stTemp;
static BYTE btIsrDisableCount;
static WORD wIsrMask;

////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USB_unlock
//
//   Type:          Function
//
//   Description:   Disable interrupts if in usb isr and saves the mask
//                  for re-enabling later
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant USB_lock(void)
{

    if ((!btIsrDisableCount) && (!bInIsr)) 
    {

        // If I am not in the usb isr and the count is 0
        wIsrMask = SysMaskAllInterrupts();          // Disable all Interrupts
        
    } /* Endif */

    if (!bInIsr) 
    {
        btIsrDisableCount++;
    } /* Endif */

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          USB_unlock
//
//   Type:          Function
//
//   Description:   Enable interrupts that were disabled by USB_lock()
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant USB_unlock(void)
{
    if (!bInIsr)
    {
        btIsrDisableCount--;
    } /* Endif */
   
    if ((!btIsrDisableCount) && (!bInIsr))
    {
        // If we are not in the usb isr and the count is 0 
        SysUnMaskAllInterrupts(wIsrMask);
    } /* Endif */
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_init
//
//   Type:          Function
//
//   Description:   Initializes the USB device specific data structures
//                  and calls the low level device controller chip
//                  initialization routine.
//
//   Inputs:        none
//
//   Outputs:       USB_OK or error code
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_init(void)
{
    BYTE     btError;
   
    /* Initialize the device state structure */
    stDeviceState.btSpeed = USB_SPEED_FULL;
    stDeviceState.btCurConfig = 0;
    stDeviceState.btAddress = 0;
    stDeviceState.usUsbState = USB_STATE_UNKNOWN;
    stDeviceState.usUsbDeviceState = USB_SELF_POWERED;
    stDeviceState.usSofCount = 0;
    stDeviceState.btBusResetting = 0;
    stDeviceState.btServicesRegistered = 0;          // No service registered yet
    stDeviceState.wError = 0;

    // Used to disable the Isr
    bInIsr = FALSE;  
    btIsrDisableCount = 0;

    /* Initialize the USB controller chip */
    btError = usb_dci_init(&stDeviceState);

    if (btError) {
        return USBERR_INIT_FAILED;
    } /* Endif */

    return USB_OK;

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_register_service
//
//   Type:          Function
//
//   Description:   Registers a function service for a specified endpoint
//                  or event
//
//   Inputs:        
//                  type    : Service type
//                  service : Pointer to the service function
//   Outputs:       USB_OK or error code
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_register_service(BYTE btType, void (* Service)(BOOL bSetup, BYTE btDirection, BYTE _X * pbtBuffer, WORD wLength)) 
{ /* Body */
   INT          i;
   BOOL         bNewService;
   
   USB_lock();
   
   bNewService = TRUE;
       
    for(i = 0 ; i < stDeviceState.btServicesRegistered ; i++)
    {
        if(Services[i].btType == btType)
        {
            bNewService = FALSE;            // We want to replace a service
            break;
        }            
    }            
            
    if(bNewService == TRUE)
    {
        // Case of installing a new service
        
        // Check if number of service registered maxed
        if(stDeviceState.btServicesRegistered >= (MAX_NUMBER_SERVICES - 1))
        {
            USB_unlock();
            return USBERR_REGISTER_SERVICE;
        }
        
        // Register new service
        Services[stDeviceState.btServicesRegistered].btType = btType;
        Services[stDeviceState.btServicesRegistered].Service = Service;
        
        // Increment number of registered services
        stDeviceState.btServicesRegistered++;       
    }
    else
    {
        // Case of service replacement
        Services[i].Service = Service;
    }    

   USB_unlock();
   
   return USB_OK;
} /* EndBody */

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_call_service
//
//   Type:          Function
//
//   Description:   Calls the appropriate service for the specified type.
//
//   Inputs:        
//                  btType      : Service type
//                       Parameters to pass to the service function
//                  bSetup      : Set if setup transfer
//                  btDirection : Direction of transmission
//                  ptbBuffer      : Pointer to a buffer
//                  wLength     : Number of bytes in the transmission
//
//   Outputs:       USB_OK or error code
//
//   Notes:         
//                  If no service registered, returns an error code 
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_call_service(BYTE btType, BOOL bSetup, BYTE btDirection, BYTE _X *ptbBuffer, WORD wLength)
{ /* Body */
   INT          i;
   
   /* Needs mutual exclusion */
   USB_lock();
   

    // Search for the existing service
   for(i = 0 ; i < stDeviceState.btServicesRegistered ; i++)
   {
       if(Services[i].btType == btType)
       {
            Services[i].Service(bSetup, btDirection, ptbBuffer, wLength);
            USB_unlock();
            return USB_OK;
       }            
   }            

    // No service found
    USB_unlock();
    return USBERR_NO_SERVICE;
} /* EndBody */

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_get_service
//
//   Type:          Function
//
//   Description:   Gets the appropriate service for the specified type.
//
//   Inputs:        
//                  btType  : Service type
//                  service : Pointer to the service function registered for the specified type
//
//   Outputs:       USB_OK or error code
//
//   Notes:         
//                  If no service registered, returns an error code 
//<
////////////////////////////////////////////////////////////////////////////////
#if 0 //Remove unused function usb_device_get_service() to reclain _P memroy.
BYTE _reentrant usb_device_get_service(BYTE btType, void (* Service)(BOOL bSetup, BYTE btDirection, BYTE _X * pbtBuffer, WORD wLength)) 
{ /* Body */
   INT          i;
   
    // Search for the existing service
   for(i = 0 ; i < stDeviceState.btServicesRegistered ; i++)
   {
       if(Services[i].btType == btType)
       {
            Service = Services[i].Service;
            return USB_OK;
       }            
   }            

    // No service found
    return USBERR_NO_SERVICE;
} /* EndBody */
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_init_endpoint
//
//   Type:          Function
//
//   Description:   Initializes the endpoint and data structures associated
//
//   Inputs:        
//                  btEndpoint  : Endpoint to be initialized
//                  usMaxPacketSize: Maximum packet size for endpoint
//                  btDirection     : Direction Endpoint 
//                  btType          : Type Endpoint
//                  bFlag           : TRUE = Enable zero length packet to terminate
//                                          transfers equal to a multiple of the
//                                          maximum packet size.
//                                    FALSE = Disables zero length packet to terminate
//                                          transfers equal to a multiple of the
//                                          maximum packet size.
//   Outputs:       USB_OK or error code
//
//   Notes:         
//                  If no service registered, returns an error code 
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_init_endpoint(BYTE btEndpoint, USHORT usMaxPacketSize, BYTE btDirection, BYTE btType, BOOL bFlag)
{ /* Body */
   BYTE         btError;
   
    /* Initialize the transfer descriptor */
    stTemp.btEndpointNum = btEndpoint;
    stTemp.btDirection = btDirection;
    stTemp.btEndpointType = btType;
    stTemp.usMaxPacketSize = usMaxPacketSize;
    if(bFlag == FALSE)
        stTemp.btDontZeroTerminate = (BYTE)0x01;
    else
        stTemp.btDontZeroTerminate = (BYTE)0x00;
       
   btError = usb_dci_init_endpoint(&stTemp);
   
   return btError;
   
} /* EndBody */


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_read_setup_data
//
//   Type:          Function
//
//   Description:   Reads the setup data from the dQH
//
//   Inputs:        
//                  btEndpoint  : Endpoint number
//
//   Outputs:       
//                  a pointer to the setup structure
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
dQH_Struct_Setup _X * _reentrant usb_device_read_setup_data(BYTE btEndpoint)
{ /* Body */

   return(usb_dci_get_setup_data(btEndpoint));

} /* EndBody */




////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_get_status
//
//   Type:          Function
//
//   Description:   Returns the USB internal state
//
//   Inputs:        
//                  btType          : Status type
//                  pusStatus      : Status
//
//   Outputs:       
//                  none
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_get_status(BYTE btType, USHORT * pusStatus)
{ /* Body */

    switch (btType) {

        case USB_STATUS_DEVICE_STATE:
            *pusStatus = stDeviceState.usUsbDeviceState;
            break;

        case USB_STATUS:
            *pusStatus = stDeviceState.usUsbState;
            break;

        case USB_STATUS_INTERFACE:
            break;
         
        case USB_STATUS_ADDRESS:
            *pusStatus = (USHORT)(stDeviceState.btAddress);
            break;
         
        case USB_STATUS_CURRENT_CONFIG:
            *pusStatus = (USHORT)(stDeviceState.btCurConfig);
            break;

        case USB_STATUS_SOF_COUNT:
            *pusStatus = stDeviceState.usSofCount;
            break;
         
        default:
            if (btType & USB_STATUS_ENDPOINT)
                *pusStatus = usb_dci_get_endpoint_status(btType & (BYTE)USB_STATUS_ENDPOINT_NUMBER_MASK);
            break;    

    } /* Endswitch */
} /* EndBody */  
 
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_set_status
//
//   Type:          Function
//
//   Description:   Sets the USB internal state
//
//   Inputs:        
//                  btType          : Status type
//                  pusStatus      : Status
//
//   Outputs:       
//                  none
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_set_status(BYTE btType, USHORT usStatus)
{ /* Body */
    USB_lock();

    switch (btType)
    {
        case USB_STATUS_DEVICE_STATE:
            stDeviceState.usUsbDeviceState= usStatus;
            break;

        case USB_STATUS:
            stDeviceState.usUsbState = usStatus;
            break;

        case USB_STATUS_INTERFACE:
            break;

        case USB_STATUS_ADDRESS:
            stDeviceState.btAddress = (BYTE)usStatus;
            usb_dci_set_address(usStatus);
            break;

        case USB_STATUS_CURRENT_CONFIG:
            stDeviceState.btCurConfig = (BYTE)usStatus;
            break;
         
        case USB_STATUS_SOF_COUNT:
            stDeviceState.usSofCount = usStatus;
            break;

        case USB_STATUS_TEST_MODE:

#ifndef BOOT_ROM
#ifndef USE_EEPROM
            usb_dci_set_test_mode(usStatus);
#endif
#endif            

            break;
         
        default:
            if (btType & USB_STATUS_ENDPOINT)
                usb_dci_set_endpoint_status(btType & (BYTE)USB_STATUS_ENDPOINT_NUMBER_MASK, usStatus);

            break;
    } /* Endswitch */
    
    USB_unlock();

} /* EndBody */

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_get_transfer_status
//
//   Type:          Functiorn
//
//   Description:   Gets the status of the last transfer on the specified
//                  couple endpoint/direction
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  btDirection      : Direction
//
//   Outputs:       
//                  Transfer status
//
//   Notes:
//                  The status returned is the btStatus entry from the XD
//                  associated with the couple endpoint/direction         
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_get_transfer_status(BYTE btEndpoint, BYTE btDirection)
{ /* Body */

    return(usb_dci_get_transfer_status(btEndpoint, btDirection));

} /* EndBody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_stall_endpoint
//
//   Type:          Functiorn
//
//   Description:   Stalls the specified endpoint for the specified direction
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  btDirection      : Direction
//
//   Outputs:       
//                  None
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_stall_endpoint(BYTE btEndpoint, BYTE btDirection)
{ /* Body */

   usb_dci_stall_endpoint(btEndpoint, btDirection);

} /* EndBody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_init_data_toggle_endpoint
//
//   Type:          Functiorn
//
//   Description:   Reinitialize data toggle to DATA0
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//
//   Outputs:       
//                  None
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_init_data_toggle_endpoint(BYTE btEndpoint)
{ /* Body */

   usb_dci_init_data_toggle_endpoint(btEndpoint);

} /* EndBody */


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_unstall_endpoint
//
//   Type:          Functiorn
//
//   Description:   Unstalls the specified endpoint for the specified direction
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  btDirection      : Direction
//
//   Outputs:       
//                  None
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_unstall_endpoint(BYTE btEndpoint, BYTE btDirection)
{ /* Body */

   usb_dci_unstall_endpoint(btEndpoint, btDirection);

} /* EndBody */

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_setup_data
//
//   Type:          Function
//
//   Description:   Setup data on the specified endpoint
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  usStmpWordAdd   : Buffer Start address (STMP space)
//                  btByteOffset    : Byte offset in the buffer start address (0, 1, 2)
//                  usSize          : Number byte to transfer 
//                  bIsDataPhase    : TRUE if data phase during setup transaction
//
//   Outputs:       
//                  Error or USB_OK if no error
//
//   Notes:         This function derived from a common section in send & recv funcs.
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_setup_data(BYTE btEndpoint, BYTE _X * pbtStmpWordAdd, 
                                      BYTE btByteOffset, USHORT usSize, BOOL bIsDataPhase,
                                      xd_struct _X * pStXd, BYTE btDirection)
{ /* Body */
    BYTE        btError;

    if(usSize > USB_MAX_SIZE_PER_TRANSFER)
        return(USBERR_SIZE_TOO_LARGE);

    USB_lock();

    if(pStXd->btStatus == USB_STATUS_TRANSFER_IN_PROGRESS)
    {
        USB_unlock();
        return USB_STATUS_TRANSFER_IN_PROGRESS;
    } /* Endif */

    // Patch for ARC problem to not generate interrupt transfer complete
    // for 0-byte transfers
    if(usSize != 0)
        pStXd->btStatus = USB_STATUS_TRANSFER_IN_PROGRESS;
     
    // Initialize the transfer descriptor
    pStXd->btEndpointNum = btEndpoint;
    pStXd->btDirection = btDirection;
    pStXd->pbtStartAddressWord = pbtStmpWordAdd;
    pStXd->btStartOffsetByte = btByteOffset;
    pStXd->usTotalLength = usSize;
    pStXd->bIsDataPhase = bIsDataPhase; 
   
    btError = usb_dci_transfer_data(pStXd);

    USB_unlock();
   
    if(btError)
        return USBERR_TX_FAILED;
   
   return USB_OK;

} /* EndBody */


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_send_data
//
//   Type:          Function
//
//   Description:   Sends data on the specified endpoint
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  usStmpWordAdd   : Buffer Start address (STMP space)
//                  btByteOffset    : Byte offset in the buffer start address (0, 1, 2)
//                  usSize          : Number byte to transfer 
//                  bIsDataPhase    : TRUE if data phase during setup transaction
//
//   Outputs:       
//                  Error or USB_OK if no error
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_send_data(BYTE btEndpoint, BYTE _X * pbtStmpWordAdd, 
                                     BYTE btByteOffset, USHORT usSize, BOOL bIsDataPhase)
{ // Body 
    xd_struct _X * pStXd;
    BYTE        btError;

    pStXd = dXD_Base_Address[(btEndpoint*2)+1]; // Get the specif Xd structure
                                                // assign to the endpoint
    btError = usb_device_setup_data(btEndpoint, pbtStmpWordAdd, btByteOffset,
                                    usSize, bIsDataPhase, pStXd, USB_SEND);
   
   return btError;

} // EndBody


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_recv_data
//
//   Type:          Function
//
//   Description:   Receives data on the specified endpoint
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  usStmpWordAdd   : Buffer Start address (STMP space)
//                  btByteOffset    : Byte offset in the buffer start address (0, 1, 2)
//                  usSize          : Number byte to transfer 
//                  bIsDataPhase    : TRUE if data phase during setup transaction
//
//   Outputs:       
//                  Error or USB_OK if no error
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_recv_data(BYTE btEndpoint, BYTE _X * pbtStmpWordAdd, 
                                     BYTE btByteOffset, USHORT usSize, BOOL bIsDataPhase)
{ // Body 
    xd_struct _X * pStXd;
    BYTE        btError;

    pStXd = dXD_Base_Address[btEndpoint*2]; // Get the specif Xd structure
                                                // assign to the endpoint
    
   
    btError = usb_device_setup_data(btEndpoint, pbtStmpWordAdd, btByteOffset,
                                    usSize, bIsDataPhase, pStXd, USB_RECV);
   
    return btError;

} // EndBody



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_wait_configuration
//
//   Type:          Function
//
//   Description:   Waits for the device to be configured
//
//   Inputs:        
//
//   Outputs:       
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_wait_configuration(void)
{
    USHORT  usStatus;
    
    do
    {
        usb_device_get_status(USB_STATUS, &usStatus);
    }while(usStatus != USB_STATE_CONFIG);

}
