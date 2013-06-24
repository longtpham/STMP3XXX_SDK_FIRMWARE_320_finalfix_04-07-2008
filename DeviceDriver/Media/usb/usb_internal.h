#ifndef _USB_INTERNAL_H
#define _USB_INTERNAL_H

#include "project.h"

#ifndef MAX_USB_STRINGS
#define MAX_USB_STRINGS 9
#endif


#define  USB_SELF_POWERED                    (0x01)
#define  USB_REMOTE_WAKEUP                   (0x02)

#define MAX_DESC_SIZE       86      // (255/3)+1

#define MS_OS_STRING_DESCRIPTOR_INDEX 0xEE
#define MS_OS_STRING_DESCRIPTOR_INDEX_IN_ARRAY 7

/* The USB Device State Structure */
typedef struct {
   BYTE         btSpeed;                // Speed of device when connected
                                        // High Speed or Full Speed
   BYTE         btCurConfig;            // Current configuration number (0 means not configured yet)
                                        // or default interface
   BYTE         btCurInterface;         // Current Alternate Interface (0 means not configured yet)                                                         
   BYTE         btAddress;              // USB device address
   USHORT       usUsbState;             // State USB (UNKNOWN, POWERED, DEFAULT, ADDRESSED, CONFIGURED, SUSPENDED)
   USHORT       usUsbStatePriorSusp;    // Ste USB prior a SUSPEND. This state needs to be restore
                                        // once the USB resumes
   USHORT       usUsbDeviceState;       // State of the device (SelfPowered, Remote Wakeup)
   USHORT       usSofCount;
   BYTE         btBusResetting;         // Set during a reset phase
   BYTE         btServicesRegistered;   // Number of services registered
   WORD         wError;                 // Increments each time a USB Error Interrupt occurs
   BOOL         bEnterTestMode;         // Set when entered in test mode. To clear the device
                                        // must be powered down
   USHORT       usConnectionState;      // State of the USB connection (CONNECTED, DISCONNECTED, UNKNOWN)                                         
} Dev_State_Struct;

typedef struct xd_struct_type{
   BYTE         btEndpointNum;          // Endpoint number 
   BYTE         btDirection;            // Direction : Send/Receive
   BYTE         btEndpointType;         // Type of the endpoint: Ctrl, Isoch, Bulk,Int 
   BYTE         btStatus;               // Current transfer status 
   WORD _X *    pbtStartAddressWord;    // Address of first byte (DSP Word address in STMP space)
   BYTE         btStartOffsetByte;      // Address of first byte (Word Byte offset in STMP space)
   USHORT       usTotalLength;          // Number of bytes to send/recv 
   USHORT       usMaxPacketSize;        // Max Packet size 
   BYTE         btDontZeroTerminate;
   BOOL         bIsDataPhase;           // Set to TRUE if the transfer is data phase
                                        // of a setup transfer
   struct xd_struct_type _USB_MEM*pNext;     // pointer to the next xd_struct
   WORD         wPrivateData;           //data to be passed to the service handler
   USHORT       usBytesCopied;          //number of bytes copied so far
   WORD         wCurrentOffsetByte;
   WORD _X*    pbtCurrentAddressWord;
} xd_struct;

// If MAX_NUM_EP is not defined, default to 4
// *** NOTE: if you change this definition, change the equate for MAX_NUM_EP in usb20ddmem.asm as well.
// The two definitions must be equivalent for proper operation.
#define MAX_NUM_EP  4

extern Dev_State_Struct _USB_MEM stDeviceState;

extern xd_struct _USB_MEM * g_dXD_Queues[MAX_NUM_EP][2];

BYTE _reentrant usb_device_call_service(BYTE btType, BOOL bSetup, BYTE btDirection, WORD _X*ptbBuffer, WORD wLength, WORD wPrivateData);
xd_struct _USB_MEM * GetEmptyXd(void);
void FreeXd(xd_struct _USB_MEM * pTemp);
void AddXdToQueue(xd_struct _USB_MEM *pHead,xd_struct _USB_MEM *pElement);
void InitXdPool(void);
_reentrant void usb_PackDescriptor(void* pDescriptor,void _X * pDestination, WORD startIndex);
_reentrant void parse_strings(void);


    // Service structures.
    // Those are the structures storing the function handler for a specific
    // USB service.
    // There is a service per Endpoint + Reset + Suspend + Start Of Frame
    //                                 + Resume + Sleep + Speed Detection  
    // The service structs are setup by the USB application using the device driver
typedef struct {
   BYTE             btType;     // Which service type
                                // Pointer to the service function
   void (* Service)(BOOL bSetup, BYTE btDirection, WORD _X* pbtBuffer, WORD wLength, WORD wPrivateData);
} Service_Struct;


#endif
