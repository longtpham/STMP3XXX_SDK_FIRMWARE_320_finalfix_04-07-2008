////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : usb20.h
// Description : USB2.0 defines and structures definition for USB2.0 device
//                  driver API
////////////////////////////////////////////////////////////////////////////////

#ifndef __usb20devapi_h__
#define __usb20devapi_h__ 1

#include "types.h"
#include "usb20.h"
#include "usb20ddprv.h"

/* Endpoint types */
#define  USB_CONTROL_ENDPOINT             (0)
#define  USB_ISOCHRONOUS_ENDPOINT         (1)
#define  USB_BULK_ENDPOINT                (2)
#define  USB_INTERRUPT_ENDPOINT           (3)

/* Informational Request/Set Types */
#define  USB_STATUS_DEVICE_STATE          (0x01)
#define  USB_STATUS_INTERFACE             (0x02)
#define  USB_STATUS_ADDRESS               (0x03)
#define  USB_STATUS_CURRENT_CONFIG        (0x04)
#define  USB_STATUS_SOF_COUNT             (0x05)
#define  USB_STATUS_DEVICE                (0x06)
#define  USB_STATUS_TEST_MODE             (0x07)
#define  USB_STATUS_ENDPOINT              (0x10)
#define  USB_STATUS_ENDPOINT_NUMBER_MASK  (0x0F)

#define  USB_TEST_MODE_TEST_PACKET        (0x04)

/* Available service types */
/* Services 0 through 15 are reserved for endpoints */
#define  USB_SERVICE_EP0                  (0x00)
#define  USB_SERVICE_EP1                  (0x01)
#define  USB_SERVICE_EP2                  (0x02)
#define  USB_SERVICE_EP3                  (0x03)
#define  USB_SERVICE_EP4                  (0x04)
#define  USB_SERVICE_EP5                  (0x05)
#define  USB_SERVICE_EP6                  (0x06)
#define  USB_SERVICE_EP7                  (0x07)
#define  USB_SERVICE_EP8                  (0x08)
#define  USB_SERVICE_EP9                  (0x09)
#define  USB_SERVICE_EP10                 (0x0a)
#define  USB_SERVICE_EP11                 (0x0b)
#define  USB_SERVICE_EP12                 (0x0c)
#define  USB_SERVICE_EP13                 (0x0d)
#define  USB_SERVICE_EP14                 (0x0e)
#define  USB_SERVICE_EP15                 (0x0f)
#define  USB_SERVICE_BUS_RESET            (0x10)
#define  USB_SERVICE_SUSPEND              (0x11)
#define  USB_SERVICE_SOF                  (0x12)
#define  USB_SERVICE_RESUME               (0x13)
#define  USB_SERVICE_SLEEP                (0x14)
#define  USB_SERVICE_SPEED_DETECTION      (0x15)
#define  NON_EP_SERVICES                  (USB_SERVICE_SPEED_DETECTION - USB_SERVICE_EP15)

// This is the maximum service structures allocated 
// One for each End point + the common services (non endpoint services) 
#define MAX_NUMBER_SERVICES               (MAX_NUM_EP + NON_EP_SERVICES)  

#define NUMBER_BYTES_IN_SETUP               (8)

// possible values of btStatus on the xd_struct
#define  USB_STATUS_TRANSFER_IDLE           (0)
#define  USB_STATUS_TRANSFER_SUCCESS        (1)            
#define  USB_STATUS_TRANSFER_ACCEPTED       (2)
#define  USB_STATUS_TRANSFER_IN_PROGRESS    (3)
#define  USB_STATUS_TRANSFER_ERROR          (4)

// types of status set/returned by set_status() and get_status()
#define  USB_STATUS_DEVICE_STATE          (0x01)
#define  USB_STATUS_INTERFACE             (0x02)
#define  USB_STATUS_ADDRESS               (0x03)
#define  USB_STATUS_CURRENT_CONFIG        (0x04)
#define  USB_STATUS_SOF_COUNT             (0x05)
#define  USB_STATUS                       (0x06)
#define  USB_STATUS_TEST_MODE             (0x07)
// For status endpoint, the endpoint number is coded in the less significant nibble
#define  USB_STATUS_ENDPOINT              (0x10)
#define  USB_STATUS_ENDPOINT_NUMBER_MASK  (0x0F)

#define USB_MAX_SIZE_PER_TRANSFER       (0x4000)    // This is the maximum size that
                                                    // we allow per transfer
    // Service structures.
    // Those are the structures storing the function handler for a specific
    // USB service.
    // There is a service per Endpoint + Reset + Suspend + Start Of Frame
    //                                 + Resume + Sleep + Speed Detection  
    // The service structs are setup by the USB application using the device driver
typedef struct {
   BYTE             btType;     // Which service type
                                // Pointer to the service function
   void (* Service)(BOOL bSetup, BYTE btDirection, BYTE _X * pbtBuffer, WORD wLength);
} Service_Struct;

void _reentrant USB_lock(void);
void _reentrant USB_unlock(void);
BYTE _reentrant usb_device_init(void);
BYTE _reentrant usb_device_register_service(BYTE btType, void (* Service)(BOOL bSetup, BYTE btDirection, BYTE _X * pbtBuffer, WORD wLength)); 
BYTE _reentrant usb_device_call_service(BYTE btType, BOOL bSetup, BYTE btDirection, BYTE _X *pbBuffer, WORD wLength);
//Remove unused function usb_device_get_service() to reclaim _P memory.
//BYTE _reentrant usb_device_get_service(BYTE btType, void (* Service)(BOOL bSetup, BYTE btDirection, BYTE _X * pbtBuffer, WORD wLength)); 
BYTE _reentrant usb_device_init_endpoint(BYTE btEndpoint, USHORT usMaxPacketSize, BYTE btDirection, BYTE btType, BOOL bFlag);
dQH_Struct_Setup _X * _reentrant usb_device_read_setup_data(BYTE btEndpoint);
void _reentrant usb_device_get_status(BYTE btType, USHORT * pusStatus);
void _reentrant usb_device_set_status(BYTE btType, USHORT usStatus);
BYTE _reentrant usb_device_get_transfer_status(BYTE btEndpoint, BYTE btDirection);
void _reentrant usb_device_stall_endpoint(BYTE btEndpoint, BYTE btDirection);
void _reentrant usb_device_unstall_endpoint(BYTE btEndpoint, BYTE btDirection);
void _reentrant usb_device_init_data0_endpoint(BYTE btEndpoint);
BYTE _reentrant usb_device_send_data(BYTE btEndpoint, BYTE _X * pbtStmpWordAdd, BYTE btByteOffset, USHORT usSize, BOOL bIsDataPhase);
BYTE _reentrant usb_device_recv_data(BYTE btEndpoint, BYTE _X * pbtStmpWordAdd, BYTE btByteOffset, USHORT usSize, BOOL bIsDataPhase);
void _reentrant usb_device_wait_configuration(void);

extern WORD _reentrant SysMaskAllInterrupts(void);
extern void _reentrant SysUnMaskAllInterrupts(WORD wLevel);

#endif
/* EOF */

