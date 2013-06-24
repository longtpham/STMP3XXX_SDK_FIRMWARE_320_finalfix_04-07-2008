////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2001-2005
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
#include "project.h"
#include "usb_api.h"
#include "usb_internal.h"
#include "usb_dci.h"
#include "sysirq.h"

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm
#endif // ifdef FUNCLET

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////
xd_struct _USB_MEM * g_dXD_Queues[MAX_NUM_EP][2];
extern BYTE _USB_MEM * ConfigStruct[];

////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
xd_struct _USB_MEM * GetEmptyXd(void);
void FreeXd(xd_struct _USB_MEM * );
void AddXdToQueue(xd_struct _USB_MEM *,xd_struct _USB_MEM *);

////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////
Dev_State_Struct _USB_MEM stDeviceState;
Service_Struct      _X Services[MAX_NUMBER_SERVICES];       
 
xd_struct _USB_MEM stTemp;

static BYTE s_btHoldOffEP0 = TRUE;
static BYTE s_btStatusPhase = FALSE;
static BOOL s_bUsbFirstInit = TRUE;



////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////
#ifndef MAX_USB_TRANSFERS_QUEUED
#define MAX_USB_TRANSFERS_QUEUED 48
#endif
xd_struct _USB_MEM g_XdPool[MAX_USB_TRANSFERS_QUEUED];

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////


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
BYTE _reentrant usb_device_register_service(BYTE btType, void (* Service)(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM  * pbtBuffer, WORD wLength,WORD wPrivateData)) 
{ /* Body */
   INT          i;
   BOOL         bNewService;
   WORD			wOldInterrupt;
   wOldInterrupt = SysMaskAllInterrupts();
   
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
			SysUnMaskAllInterrupts(wOldInterrupt);
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

   SysUnMaskAllInterrupts(wOldInterrupt);
   
   return USB_OK;
} /* EndBody */

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
extern Struct_Standard_Dev_Desc _USB_MEM StDescDevice;

#ifdef FUNCLET
    #pragma asm
        Fusb_device_init:

        nolist
        include "sysmacro.asm"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_USB_DEVICE_INIT
        CallFunclet RSRC_FUNCLET_USB_DEVICE_INIT

        org p,"SYSFUNCLET_USB_DEVICE_INIT_P":
        dc      RSRC_FUNCLET_USB_DEVICE_INIT
    #pragma endasm
BYTE _reentrant usb_device_init_Funclet(void)
#else
BYTE _reentrant usb_device_init(void)
#endif
{
    BYTE     btError;
   
    _memset(&stDeviceState,0,sizeof(stDeviceState));
    
    _memset(&g_XdPool,0,sizeof(g_XdPool));
    
    
    /* Initialize the device state structure */
    stDeviceState.btSpeed = USB_SPEED_HIGH;
    
    stDeviceState.usUsbState = USB_STATE_UNKNOWN;

    StDescDevice.btIdVendorLsb = g_wUSBVendorID & 0xff;
    StDescDevice.btIdVendorMsb = (g_wUSBVendorID>>8) & 0xff;

    StDescDevice.btIdProductLsb= g_wUSBProductID & 0xff;
    StDescDevice.btIdProductMsb= (g_wUSBProductID>>8) & 0xff;

    StDescDevice.btBcdReleaseLsb= g_wUSBReleaseID & 0xff;
    StDescDevice.btBcdReleaseMsb= (g_wUSBReleaseID>>8) & 0xff;

    usb_device_register_service(USB_SERVICE_EP0, ch9ServiceEp0); 

    /* Initialize the USB controller chip */
    btError = usb_dci_init(&stDeviceState);

    if (btError) {
        return USBERR_INIT_FAILED;
    } /* Endif */
    

    return USB_OK;

}
//
// Reset the section.
//
#ifdef FUNCLET
    #pragma asm
        org p,".ptextusb_api":
    #pragma endasm
#endif // FUNCLET

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
//                  ptbBuffer   : Pointer to a buffer
//                  wLength     : Number of bytes in the transmission
//                  wPrivateData: Word passed in 
//
//   Outputs:       USB_OK or error code
//
//   Notes:         
//                  If no service registered, returns an error code 
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_call_service(BYTE btType, BOOL bSetup, BYTE btDirection, WORD _X*ptbBuffer, WORD wLength, WORD wPrivateData)
{ /* Body */
   INT          i;
   WORD         wOldInterrupt;   
   /* Needs mutual exclusion */
   wOldInterrupt = SysMaskAllInterrupts();
   

    // Search for the existing service
   for(i = 0 ; i < stDeviceState.btServicesRegistered ; i++)
   {
       if(Services[i].btType == btType)
       {
            Services[i].Service(bSetup, btDirection, ptbBuffer, wLength,wPrivateData);
            SysUnMaskAllInterrupts(wOldInterrupt);
            return USB_OK;
       }            
   }            

    // No service found
    SysUnMaskAllInterrupts(wOldInterrupt);
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
#if 0	// Remove unused function to reclain _P memory.
BYTE _reentrant usb_device_get_service(BYTE btType, void (* Service)(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM  * pbtBuffer, WORD wLength)) 
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

        case USB_STATUS_DEVICE_SPEED:
            *pusStatus = stDeviceState.btSpeed;
            break;

        case USB_STATUS_DEVICE_TEST:
            *pusStatus = (USHORT)stDeviceState.bEnterTestMode;
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

        case USB_STATUS_CURRENT_IFACE:
            *pusStatus = (USHORT)(stDeviceState.btCurInterface);
            break;

        case USB_STATUS_SOF_COUNT:
            *pusStatus = stDeviceState.usSofCount;
            break;
        
        case USB_STATUS_CONNECTION:
            usb_dci_update_connection_status();
            *pusStatus = stDeviceState.usConnectionState;
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
    WORD         wOldInterrupt;   
    wOldInterrupt = SysMaskAllInterrupts();

    switch (btType)
    {
        case USB_STATUS_DEVICE_STATE:
            stDeviceState.usUsbDeviceState = usStatus;
            break;

        case USB_STATUS_DEVICE_TEST:
            stDeviceState.bEnterTestMode = (BOOL)usStatus;
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
         
        case USB_STATUS_CURRENT_IFACE:
            stDeviceState.btCurInterface = (BYTE)usStatus;
            break;

        case USB_STATUS_SOF_COUNT:
            stDeviceState.usSofCount = usStatus;
            break;

        case USB_STATUS_TEST_MODE:

#if defined (DEVICE_3500)
#ifndef BOOT_ROM
            usb_dci_set_test_mode(usStatus);
#endif // !defined (BOOT_ROM)
#endif // defined (DEVICE_3500)

            break;
         
        default:
            if (btType & USB_STATUS_ENDPOINT)
                usb_dci_set_endpoint_status(btType & (BYTE)USB_STATUS_ENDPOINT_NUMBER_MASK, usStatus);

            break;
    } /* Endswitch */
    
    SysUnMaskAllInterrupts(wOldInterrupt);

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

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_transfer_data
//
//   Type:          Function
//
//   Description:   transfers data on the specified endpoint - space savings
//                  for usb_device_send_data and usb_device_recv_data
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  usStmpWordAdd   : Buffer Start address (STMP space)
//                  btByteOffset    : Byte offset in the buffer start address (0, 1, 2)
//                  usSize          : Number byte to transfer 
//                  bIsDataPhase    : TRUE if data phase during setup transaction
//                  wDirection      : USB_IN or USB_OUT
//
//   Outputs:       
//                  Error or USB_OK if no error
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_transfer_data(BYTE btEndpoint, WORD _X* pbtStmpWordAdd, 
                                         BYTE btByteOffset, USHORT usSize, WORD wPrivateData, 
                                         BOOL bIsDataPhase, WORD wDirection)
{ /* Body */
    WORD         wOldInterrupt;   

    xd_struct _USB_MEM  * pCurrentXd = NULL;
    xd_struct _USB_MEM  * pNewXd;
    BYTE    btError = USB_OK;
    USHORT      usStatus;

    if(usSize > USB_MAX_SIZE_PER_TRANSFER)
        return(USBERR_SIZE_TOO_LARGE);

    if (wDirection == USB_IN)
    {
        // Check if device is in the configuration state
        // Only endpoint 0 is allow to transfer data in state other than configured
        if(btEndpoint != 0)
        {
            usb_device_get_status(USB_STATUS, &usStatus);
            if(usStatus != USB_STATE_CONFIGURED)
                return USBERR_DEVICE_NOT_CONFIGURED;
        }            
    }
    
    wOldInterrupt = SysMaskAllInterrupts();

    if (wDirection == USB_OUT)
    {
        pCurrentXd = g_dXD_Queues[btEndpoint][USB_OUT];
    }


    pNewXd = GetEmptyXd();
    assert(pNewXd);
    // Initialize the transfer descriptor
    pNewXd->btEndpointNum = btEndpoint;
    pNewXd->btDirection = wDirection;
    pNewXd->btStatus = USB_STATUS_TRANSFER_IN_PROGRESS;
    pNewXd->pbtStartAddressWord = pbtStmpWordAdd;
    pNewXd->btStartOffsetByte = btByteOffset;
    pNewXd->usTotalLength = usSize;
    pNewXd->usBytesCopied = 0;
    pNewXd->bIsDataPhase = bIsDataPhase; 
    pNewXd->wPrivateData = wPrivateData;
    pNewXd->pbtCurrentAddressWord = pbtStmpWordAdd;
    pNewXd->wCurrentOffsetByte= btByteOffset;

    if (wDirection == USB_IN)
    {
        // Get the specif Xd structure
        // assign to the endpoint
        pCurrentXd = g_dXD_Queues[btEndpoint][USB_IN]; 
    }                                                

    if(pCurrentXd)
    {
        AddXdToQueue(pCurrentXd,pNewXd);
    } 
    else
    {
        btError = usb_dci_transfer_data(pNewXd);
        if(btError)
        {
            FreeXd(pNewXd);
        }
    }
   
    SysUnMaskAllInterrupts(wOldInterrupt);

    return btError;

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
BYTE _reentrant usb_device_send_data(BYTE btEndpoint, WORD _X* pbtStmpWordAdd, BYTE btByteOffset, USHORT usSize, WORD wPrivateData, BOOL bIsDataPhase)
{ /* Body */
  // I'd like to replace these at a future time, but it would change the API.
    return usb_device_transfer_data(btEndpoint, pbtStmpWordAdd, btByteOffset, 
                                    usSize, wPrivateData, bIsDataPhase, USB_IN);

} /* EndBody */


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
BYTE _reentrant usb_device_recv_data(BYTE btEndpoint, WORD _X* pbtStmpWordAdd, BYTE btByteOffset, USHORT usSize, WORD wPrivateData, BOOL bIsDataPhase)
{ /* Body */
  // I'd like to replace these at a future time, but it would change the API.
    return usb_device_transfer_data(btEndpoint, pbtStmpWordAdd, btByteOffset, 
                                    usSize, wPrivateData, bIsDataPhase, USB_OUT);


} /* EndBody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_release
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        
//   Outputs:       
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_release(void)
{

    s_btHoldOffEP0 = FALSE;

    if(s_btStatusPhase == TRUE)
    {
        // Prepare dTD for the status phase
        usb_device_recv_data(0, 0, 0, 0, FALSE,0);
        
        s_btStatusPhase = FALSE;
    }

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_hold
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        
//   Outputs:       
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_hold(void)
{

    if(!(usb_device_is_hold()))
    {
        s_btHoldOffEP0 = TRUE;
        s_btStatusPhase = FALSE;
    }
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_is_hold
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        
//   Outputs:       
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_device_is_hold(void)
{
    return s_btHoldOffEP0;
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_data_phase_required
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        
//   Outputs:       
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_data_phase_required(void)
{

    s_btStatusPhase = TRUE;

}

//
//
//
xd_struct _USB_MEM * GetEmptyXd(void)
{
    int i;
    xd_struct _USB_MEM * pTemp=NULL;

    for (i=0;i<MAX_USB_TRANSFERS_QUEUED;i++)
    {
        if(g_XdPool[i].btStatus == USB_STATUS_TRANSFER_IDLE)
        {
            pTemp = &g_XdPool[i];
            pTemp ->pNext = NULL;
            break;
        }
    }

    return pTemp;
}

//
//
//
void FreeXd(xd_struct _USB_MEM * pTemp)
{
    pTemp->btStatus = USB_STATUS_TRANSFER_IDLE;
    pTemp->pNext = NULL;

}

//
//
//
void AddXdToQueue(xd_struct _USB_MEM *pHead,xd_struct _USB_MEM *pElement)
{
    int i=0;
    while(pHead->pNext)
    {
        pHead = pHead->pNext;
        i++;
    }
    pHead->pNext = pElement;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_is_first_init
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
BOOL _reentrant usb_device_is_first_init(void)
{
    return s_bUsbFirstInit;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_set_first_init
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_set_first_init(void)
{

    s_bUsbFirstInit = TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_clear_first_init
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_device_clear_first_init(void)
{

    s_bUsbFirstInit = FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_get_current_limit
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       WORD current limit in the device descriptor 
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
WORD _reentrant usb_get_current_limit(void)
{
    //WORD wCurrent=0;
    return (((Struct_Standard_Config_Desc*)(((BYTE _USB_MEM*)ConfigStruct[0])[0]))->btMaxPower)*2;
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_set_current_limit
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        WORD wNewCurrent        new current limit
//
//   Outputs:       RETCODE 
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant usb_set_current_limit(WORD wNewCurrent)
{
    RETCODE rtn = !SUCCESS;
    if(wNewCurrent >= 100 && wNewCurrent <= 500)
    {
        rtn  = SUCCESS;
        ((Struct_Standard_Config_Desc*)(((BYTE _USB_MEM*)ConfigStruct[0])[0]))->btMaxPower = (wNewCurrent/2);
    }
    return rtn;
}