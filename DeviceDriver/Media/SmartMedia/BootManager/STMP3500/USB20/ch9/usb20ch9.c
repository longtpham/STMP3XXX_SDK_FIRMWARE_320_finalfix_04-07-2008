////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : usb20ch9.c
// Description : USB2.0 Chapter Application skeleton
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "usb20.h"
#include "usb20devapi.h"
#include "usb20ch9.h"
#include "usb20project.h"
#include "descriptor.h"


// Macro definitions
#define MIN(a,b) ((a) < (b) ? (a) : (b))      

USHORT _X usDataToSend;
dQH_Struct_Setup _X * _X pStLocalSetup;

// Global variable definitions
volatile BYTE   _X btSpeed;
volatile BOOL   _X bEnterTestMode;
volatile USHORT _X usTestModeIndex;

#ifdef  SIM_USB20

// Structure defining all ARC registers for simulation only
extern arc_reg_struct  stArcSoftRegs;

///////////////////////////////////////////////////////////////////////////////////
// DCLK Count Register Upper
typedef union               
{
    struct {
        int             :24;
    } B;

    int I;

} dclkcntu_type;
#define HW_DCLKCNTU         (*(volatile dclkcntu_type _X*) (0xFFEA+1))

// DCLK Count Register Low
typedef union               
{
    struct {
        int             :24;
    } B;

    int I;

} dclkcntl_type;
#define HW_DCLKCNTL         (*(volatile dclkcntl_type _X*) (0xFFEA))

void InitTime(void);
DWORD GetTime(void);

DWORD   dwTimeInIsr;

#endif


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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9GetStatus(BYTE bSetup, dQH_Struct_Setup _X * _X pStLocalSetup) 
{ /* Body */
    
    USHORT usStatus;
    BYTE btIndex;
    BOOL bRequestValid;

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup)
    {
        switch (pStLocalSetup->btRequestType)
        {

            case GET_STATUS_DEVICE:
                // Device status request
                usb_device_get_status((BYTE)USB_STATUS_DEVICE_STATE, &usStatus);

                // Clear any undesired bits
                usDataToSend = usStatus;
                
                usDataToSend &= 0x0003;
                
                // Send the data
                usb_device_send_data(0, (BYTE _X *)&usDataToSend, 0, (USHORT)2, TRUE); 
                break;

            case GET_STATUS_INTERFACE:
                // Interace status request
                usDataToSend = 0x0000;

                // Send the data
                usb_device_send_data(0, (BYTE _X *)&usDataToSend, 0, (USHORT)2, TRUE); 
                break;

            case GET_STATUS_ENDPOINT:
                // Endpoint status request
                usb_device_get_status(USB_STATUS, &usStatus);
                
                btIndex = pStLocalSetup->btIndexLsb & 0x0f; 

                bRequestValid = FALSE;

                if(btIndex == 0)
                    bRequestValid = TRUE;
                else if((btIndex < MAX_NUM_EP) && (usStatus == USB_STATE_CONFIG))
                    bRequestValid = TRUE;

                if(bRequestValid == TRUE)
                {
                    // Get endpoint status
                    usb_device_get_status((USB_STATUS_ENDPOINT + btIndex), &usStatus);

                    // Clear undesried bits
                    usStatus = usDataToSend;
                    
                    usDataToSend &= 0x0001;

                    // Prepare to send data
                    usb_device_send_data(0, (BYTE _X *)&usDataToSend, 0, (USHORT)2, TRUE);
                }
                else
                {
                    usb_device_stall_endpoint(0, USB_SEND);
                    return;
                }

                break;  

            default:
                // Unknown request
                usb_device_stall_endpoint(0, USB_SEND);
                return;
      } /* Endswitch */
      
        // Prepare dTD for the status phase
        usb_device_recv_data(0, 0, 0, 0, FALSE);
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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9ClearFeature(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */
    BYTE btIndex;
    BOOL bRequestValid;
    USHORT usStatus;

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup) 
    {
        switch (pStLocalSetup->btRequestType)
        {
      
            case FEATURE_DEVICE:
                // Clear feature device
                if (pStLocalSetup->btValueLsb == 1)
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
                    usb_device_stall_endpoint(0, USB_SEND);
                    return;
                }
                break;
         
            case FEATURE_ENDPOINT:
                usb_device_get_status(USB_STATUS, &usStatus);

                btIndex = pStLocalSetup->btIndexLsb & 0x0f; 

                bRequestValid = FALSE;

                if(btIndex == 0)
                    bRequestValid = TRUE;
                else if((btIndex < MAX_NUM_EP) && (usStatus == USB_STATE_CONFIG))
                    bRequestValid = TRUE;


                if(bRequestValid == TRUE)
                    // Set endpoint status
                    usb_device_set_status((USB_STATUS_ENDPOINT + btIndex), FALSE);

                else
                {
                    usb_device_stall_endpoint(0, USB_SEND);
                    return;
                }

                break;

            default:
                // Unknown request
                usb_device_stall_endpoint(0, USB_SEND);
                return;

        } /* Endswitch */
      
        // Prepare dTD for the status phase
        usb_device_send_data(0, 0, 0, 0, FALSE);
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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SetFeature(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */
    USHORT usStatus;
    USHORT usDeviceState;
    BYTE btIndex;
    BOOL bRequestValid;
    
    // Get the device status
    usDeviceState = 0;
    usb_device_get_status(USB_STATUS, &usDeviceState);

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup)
    { 
        switch (pStLocalSetup->btRequestType)
        {

            case FEATURE_DEVICE:
                // Set feature device
                switch (pStLocalSetup->btValueLsb)
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
                    
                        if((pStLocalSetup->btValueLsb) || (btSpeed != USB_SPEED_HIGH)) 
                        { // Test mode incorrect
                            usb_device_stall_endpoint(0, USB_SEND);
                            return;
                        } /* Endif */

                        if ((usDeviceState == USB_STATE_CONFIG) || 
                            (usDeviceState == USB_STATE_ADDRESS) || 
                            (usDeviceState == USB_STATE_DEFAULT)) 
                        {
                            // Do not switch to test mode yet.
                            // The switch must happen after the status phase.
                            bEnterTestMode = TRUE;
                            usTestModeIndex = pStLocalSetup->btValueLsb;
                        }
                        else
                        {
                            usb_device_stall_endpoint(0, USB_SEND);
                            return;
                        } /* Endif */
                        break;

                default:
                    usb_device_stall_endpoint(0, USB_SEND);
                    return;
            } /* Endswitch */
            break;
            
            case FEATURE_ENDPOINT:
                btIndex = pStLocalSetup->btIndexLsb & 0x0f; 

                bRequestValid = FALSE;

                if((btIndex < MAX_NUM_EP) && (usDeviceState == USB_STATE_CONFIG))
                    bRequestValid = TRUE;

                if(bRequestValid == TRUE)
                    // Set endpoint status
                    usb_device_set_status((USB_STATUS_ENDPOINT + btIndex), TRUE);

                else
                {
                    usb_device_stall_endpoint(0, USB_SEND);
                    return;
                }

                break;

            default:
                usb_device_stall_endpoint(0, USB_SEND);
                return;
        } /* Endswitch */
      
        // Prepare dTD for the status phase
        usb_device_send_data(0, 0, 0, 0, FALSE);
    }
    else 
    {
        if (bEnterTestMode == TRUE) 
        {
            // Now enter test mode
            usb_device_set_status(USB_STATUS_TEST_MODE, usTestModeIndex);
        } /* Endif */
    } /* Endif */
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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
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
void _reentrant ch9SetAddress(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */
    static BYTE btNewAddress;

    if (bSetup)
    {   // Setup phase

        // Get the address from the setup buffer 
        btNewAddress = pStLocalSetup->btValueLsb;
      
        // Prepare the status phase
        usDataToSend = 0x00;
        
        usb_device_send_data((BYTE)0, (BYTE _X *)&usDataToSend, (BYTE)0, 0, FALSE);
    } 
    else
    {   // Status phase
        
        // Program the new address
        usb_device_set_status((BYTE)USB_STATUS_ADDRESS, (USHORT)btNewAddress);
        
        if(btNewAddress != 0)       // if address not 0, then set device state to addressed
            usb_device_set_status((BYTE)USB_STATUS, (USHORT)USB_STATE_ADDRESS);
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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//     The Device Request can ask for Device/Config/string/interface/endpoint
//     descriptors (via wValue). We then post an IN response to return the
//     requested descriptor.
//     And then wait for the OUT which terminates the control transfer.
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9GetDescription(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */
   USHORT   usMaxPacketSize;
   USHORT   Length, usLegthDesc;
   INT      i;
   BYTE _X * pbtDesc;
   
    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup) 
    {
        // Load the appropriate string depending on the descriptor requested.
        switch (pStLocalSetup->btValueMsb)
        {
            case DESCRIPTOR_TYPE_DEVICE:
            // Prepare Device descriptor transfer
            
                Length = ((USHORT)pStLocalSetup->btLengthMsb << 8 ) + (USHORT)pStLocalSetup->btLengthLsb;
            
                pbtDesc = GetDeviceDesc(&usLegthDesc);

                if(Length > usLegthDesc)
                    Length = usLegthDesc;  

                usb_device_send_data(0, pbtDesc, 0, Length, TRUE);
                break;

            case DESCRIPTOR_TYPE_CONFIG:
            // Prepare Configuration Descriptor transfer.
            // A configuration descriptor includes all related interfaces descriptor (1 or more per configuration)
            // and all related endpoints (1 or more per interface)

                Length = ((USHORT)pStLocalSetup->btLengthMsb << 8 ) + (USHORT)pStLocalSetup->btLengthLsb;

                pbtDesc = GetConfigDesc(&usLegthDesc);

                if(Length > usLegthDesc)
                    Length = usLegthDesc;  

                usb_device_send_data(0, pbtDesc, 0, Length, TRUE);
                break;

            case DESCRIPTOR_TYPE_STRING:
                // Check for valid string
                Length = ((USHORT)pStLocalSetup->btLengthMsb << 8 ) + (USHORT)pStLocalSetup->btLengthLsb;

                pbtDesc = GetStringDesc(&usLegthDesc, pStLocalSetup->btValueLsb);

                if(Length > usLegthDesc)
                    Length = usLegthDesc;  

                usb_device_send_data(0, pbtDesc, 0, Length, TRUE);

                break;
            
            case DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
                Length = ((USHORT)pStLocalSetup->btLengthMsb << 8 ) + (USHORT)pStLocalSetup->btLengthLsb;

                pbtDesc = GetDeviceQualifierDesc(&usLegthDesc);

                if(Length > usLegthDesc)
                    Length = usLegthDesc;  

                usb_device_send_data(0, pbtDesc, 0, Length, TRUE);
                break;
            
            case DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG:      
                Length = ((USHORT)pStLocalSetup->btLengthMsb << 8 ) + (USHORT)pStLocalSetup->btLengthLsb;
            
                pbtDesc = GetOtherSpeedConfigDesc(&usLegthDesc);

                if(Length > usLegthDesc)
                    Length = usLegthDesc;  

                usb_device_send_data(0, pbtDesc, 0, Length, TRUE);
                break;

            default:
                usb_device_stall_endpoint(0, USB_SEND);
                return;
        } /* Endswitch */

        // Prepare dTD for the status phase
        usb_device_recv_data(0, 0, 0, 0, FALSE);
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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//     The Device Request can ask for Device/Config/string/interface/endpoint
//     descriptors (via wValue). We then post an IN response to return the
//     requested descriptor.
//     And then wait for the OUT which terminates the control transfer.
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SetDescription(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */
    usb_device_stall_endpoint(0, USB_RECV);
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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9GetConfig(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */
    USHORT usStatus;

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if (bSetup) 
    {    
        // Get the device status
        usb_device_get_status(USB_STATUS, &usStatus);
        
        if(usStatus != USB_STATE_CONFIG)
            usDataToSend = 0x00;                // Return config 0
        else
        {        
            usb_device_get_status(USB_STATUS_CURRENT_CONFIG, &usStatus);
            usDataToSend = usStatus;
        }
        
        // Send the data
        usb_device_send_data(0, (BYTE _X *)&usDataToSend, 0, (USHORT)1, TRUE); 

        // Prepare dTD for the status phase
        usb_device_recv_data(0, 0, 0, 0, FALSE);
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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SetConfig(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */
    USHORT usStatus;
    INT     i;
    
    // Get the device status
    usb_device_get_status(USB_STATUS, &usStatus);

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if(bSetup)
    {
        if((usStatus == USB_STATE_CONFIG) || (usStatus == USB_STATE_ADDRESS))
        {
            // Unstall all stall endpoints per ch9 specs
            for(i=0 ; i < MAX_NUM_EP ; i++)
            {
                usb_device_get_status((USB_STATUS_ENDPOINT + i), &usStatus);
                if(usStatus == (USHORT)TRUE)
                    usb_device_set_status((USB_STATUS_ENDPOINT + i), FALSE);
            }                    
            
            if(pStLocalSetup->btValueLsb == 0)
            { // If configuration value is 0, then go to ADDRESS State
                
                // Clear the current config value
                usb_device_set_status(USB_STATUS_CURRENT_CONFIG, 0);
                
                // Set the device state to ADDRESS State
                usb_device_set_status(USB_STATUS, USB_STATE_ADDRESS);
                
                // Prepare status phase
                usb_device_send_data(0, 0, 0, 0, FALSE);
            }
            else
            {
                // Set the confing value
                usb_device_set_status(USB_STATUS_CURRENT_CONFIG, (USHORT)pStLocalSetup->btValueLsb);
            
                // Set the device state to CONFIG State
                usb_device_set_status(USB_STATUS, USB_STATE_CONFIG);

                // Prepare status phase
                usb_device_send_data(0, 0, 0, 0, FALSE);
            
            }
        }
        else
            // Invalid request
            usb_device_stall_endpoint(0, USB_SEND);
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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9GetInterface(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */
    USHORT usStatus;

    // Get the device status
    usb_device_get_status(USB_STATUS, &usStatus);

    if (usStatus != USB_STATE_CONFIG)
    {
        usb_device_stall_endpoint(0, USB_SEND);
        return;
    } /* Endif */

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if(bSetup)
    {    
        usDataToSend = 0x00;
        
        usb_device_send_data(0, (BYTE _X *)&usDataToSend, 0, (USHORT)1, TRUE); 

        // Prepare dTD for the status phase
        usb_device_recv_data(0, 0, 0, 0, FALSE);
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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SetInterface(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */

    USHORT usStatus;
    INT     i;
    
    // Get the device status
    usb_device_get_status(USB_STATUS, &usStatus);

    if(usStatus != USB_STATE_CONFIG)
    {
       usb_device_stall_endpoint(0, USB_SEND);
       return;
    } /* Endif */

    // If we are in the setup phase, return the descriptor
    // otherwise do nothing ....
    if(bSetup) 
    {

      /*
      ** If the alternate value (setup_ptr->VALUE & 0x00FF) differs
      ** from the current alternate value for the specified interface,
      ** then endpoints must be reconfigured to match the new alternate
      */
        if(pStLocalSetup->btValueLsb != 0)
        { // Here we only use one interface
            usb_device_stall_endpoint(0, USB_SEND);
            return;
        }            
        
        // Unstall all stall endpoints per ch9 specs
        for(i=0 ; i < MAX_NUM_EP ; i++)
        {
            usb_device_get_status((USB_STATUS_ENDPOINT + i), &usStatus);
            if(usStatus == (USHORT)TRUE)
                usb_device_set_status((USB_STATUS_ENDPOINT + i), FALSE);
        }                    
        
        // Prepare status phase
        usb_device_send_data(0, 0, 0, 0, FALSE);

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
//                  pStLocalSetup : pointer to the setup packet (8 bytes)
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ch9SynchFrame(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup) 
{ /* Body */
   
    // None of our endpoints support this type of request
    if (bSetup) 
    {
        usb_device_stall_endpoint(0, USB_SEND);
        return;
    }        

    return;
} /* Endbody */




////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          reset_ep0
//
//   Type:          Function
//
//   Description:   Initializes the control endpoint 0.
//
//   Inputs:        
//                  Not used
//   Outputs:       None
//
//   Notes:         
//                  The input parameters are here only to respect the Service
//                  function declaration in the service structure
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant reset_ep0(BOOL bSetup, BYTE btDirection, BYTE _X *pbtBuffer, WORD wLength)
{ /* Body */
    /* Initialize the endpoint 0 in both directions */
    usb_device_init_endpoint(0, APP_CONTROL_MAX_PKT_SIZE, USB_RECV, USB_CONTROL_ENDPOINT, TRUE);
    usb_device_init_endpoint(0, APP_CONTROL_MAX_PKT_SIZE, USB_SEND, USB_CONTROL_ENDPOINT, TRUE);
    usb_device_init_endpoint(1, EP_FS_MAX_PACKET_SIZE, USB_RECV, USB_BULK_ENDPOINT, FALSE);
    

#ifdef SIM_USB20
    // Measure overhead time to process ISR
    dwTimeInIsr = GetTime();
#endif    

} /* EndBody */   




////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_init_ep1
//
//   Type:          Function
//
//   Description:   Initializes the control endpoint 1.
//
//   Inputs:        None
//                  
//   Outputs:       None
//
//   Notes:         
//                  
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_init_ep1()
{ /* Body */

    /* Initialize the endpoint 1 in receive */
    usb_device_init_endpoint(1, EP_FS_MAX_PACKET_SIZE, USB_RECV, USB_BULK_ENDPOINT, FALSE);
    
} /* EndBody */   





////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          service_ep0
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
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant service_ep0(BOOL bSetup, BYTE btDirection, BYTE _X *pbtBuffer, WORD wLength)
{ /* Body */
   BOOL     bClassRequest;
   
   bClassRequest = FALSE;
   
    // Is this the setup phase
    if (bSetup)
    {
        pStLocalSetup = usb_device_read_setup_data(0);
    }
    else if(bClassRequest)
    {
        bClassRequest = FALSE;

        /* Finish your class or vendor request here */
        
        return;
    }     
            
    // Process the setup request
    switch (pStLocalSetup->btRequestType & (BYTE)(SETUP_REQUEST_TYPE_MASK))
    {
        // case standard request
        case SETUP_REQUEST_TYPE_STANDARD:
            switch (pStLocalSetup->btRequest) 
            {
                case STANDARD_REQUEST_GET_STATUS:
                    ch9GetStatus(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_CLEAR_FEATURE:
                    ch9ClearFeature(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_SET_FEATURE:
                    ch9SetFeature(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_SET_ADDRESS:
                    ch9SetAddress(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_GET_DESCRIPTOR:
                    ch9GetDescription(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_SET_DESCRIPTOR:
                    ch9SetDescription(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_GET_CONFIG:
                    ch9GetConfig(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_SET_CONFIG:
                    ch9SetConfig(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_GET_INTERFACE:
                    ch9GetInterface(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_SET_INTERFACE:
                    ch9SetInterface(bSetup, pStLocalSetup);
                    break;

                case STANDARD_REQUEST_SYNCH_FRAME:
                    ch9SynchFrame(bSetup, pStLocalSetup);
                    break;

                default:
                    usb_device_stall_endpoint(0, USB_RECV);
                    break;

            } /* Endswitch */
         
        break;

        // case class specific request
        case SETUP_REQUEST_TYPE_CLASS:
            bClassRequest = TRUE;
            /* Queue your own packet for class data here */
            return;

        // case vendor specific request
        case SETUP_REQUEST_TYPE_VENDOR:
            return;
      
        default:
            usb_device_stall_endpoint(0, USB_RECV);
            break;
         
   } /* Endswitch */
   
#ifdef SIM_USB20
    // Measure overhead time to process ISR
    dwTimeInIsr = GetTime();
#endif    

   return;
} /* Endbody */






////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          service_ep1
//
//   Type:          Function
//
//   Description:   Handles completed transfers on endpoint 1
//
//   Inputs:        
//                  bSetup = TRUE if this is a setup packet.
//                          By default this is always FALSE
//                  btDirection = Direction of the transfer. This application
//                          only supports Receive transfers, therefore if this byte
//                          differs from USB_RECV, we have a problem
//                  pbtBuffer = Pointer to the XD structure used for this endpoint
//                  wLength = Number of bytes actually transfered. 
//                  
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant service_ep1(BOOL bSetup, BYTE btDirection, BYTE _X *pbtBuffer, WORD wLength)
{ /* Body */

#ifdef SIM_USB20
    // Measure overhead time to process ISR
    dwTimeInIsr = GetTime();
#endif    
    
    if(btDirection != USB_RECV)
    {
        usb_device_stall_endpoint(1, USB_SEND);         // Stall the IN endpoint
        return;                 
    }

    ProcessUsbEp1Data(wLength);
   
    return;
} /* Endbody */






////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          service_speed
//
//   Type:          Function
//
//   Description:   Handles speed detection event
//
//   Inputs:        
//                  Not used
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant service_speed(BOOL bSetup, BYTE btDirection, BYTE _X *pbtBuffer, WORD wLength)
{
   btSpeed = wLength;

    // In systems where the endpoint behavior changes from full speed to high speed
    // (i.e. PACKET LOAD), you must re-initialize them here ...
    // so the dQH has the right payload setup

   return;
} /* EndBody */




#ifdef SIM_USB20

///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           InitTime
//
//   Type:           Function
//
//   Description:    Initializes the DCLK counter register
//                   
//
//   Inputs:         none
//
//   Outputs:        none
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
void InitTime(void)
{
    HW_DCLKCNTU.I = 0;
    HW_DCLKCNTL.I = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           GetTime
//
//   Type:           Function
//
//   Description:    Reads the DCLK counter register
//                   
//
//   Inputs:         none
//
//   Outputs:        DWORD : number of DCLKS cycles since last time 
//                      initialization
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
DWORD GetTime(void)
{
    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
}    

#endif


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InitUsb20          
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        None
//                  
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant InitUsb20(void)
{ /* Body */
    BYTE    btError;
    DWORD   dwStatus;


    // Initialize all descriptor for the application
    PrepareDesc();

    /* Initialize the USB interface */
    btError = usb_device_init();
    
    // Initialize global variables
    btSpeed = (BYTE) USB_SPEED_FULL;
    bEnterTestMode = FALSE;
    usTestModeIndex = 0;
       
    if (btError != USB_OK)
      UsbError();
   
    usb_device_register_service(USB_SERVICE_BUS_RESET, reset_ep0);
   
    usb_device_register_service(USB_SERVICE_EP0, service_ep0);        

    usb_device_register_service(USB_SERVICE_EP1, service_ep1);        

    usb_device_register_service(USB_SERVICE_SPEED_DETECTION, service_speed);


#ifdef SIM_USB20
    dwTimeInIsr = GetTime();
#endif    
   
#ifdef SIM_USB20
    while(1)
    {
        read_usb_reg(&(stArcSoftRegs.SoftArcUsbSts), &dwStatus);
        if(dwStatus)
        {
            InitTime();
            
            #pragma asm
                extern  Fusb_dci_isr
                jsr     Fusb_dci_isr
            #pragma endasm                

        }
    }
#endif

#ifndef BOOT_ROM
#ifndef USE_EEPROM
    while (1)
    {
        if (bEnterTestMode == TRUE)
        { // We are in test mode
            usb_device_set_status(USB_STATUS_TEST_MODE, usTestModeIndex); 
            return;
        } /* Endif */
    } /* Endwhile */
#endif
#endif
   
} /* Endbody */

/* EOF */
