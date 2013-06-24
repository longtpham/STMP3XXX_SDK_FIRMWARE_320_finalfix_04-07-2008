////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// File : PackDesc.c
// Description : 
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "usb_api.h"
#include "..\usb_internal.h"
#include "regsrevision.h"

BYTE _X * _reentrant GetDescriptor(BYTE btDescType, BYTE btDevSpeed, BYTE btIndex);
void _reentrant BuildConfigDescriptor(BYTE _USB_MEM *pConfigRule[], WORD _X *pPackedDescriptor);
void _reentrant BuildStringDescriptor(BYTE _X * _X pbtBuffer);

//#define MAX_DESC_SIZE       86      // (255/3)+1 //fam:moved to usb_internal.h
WORD _X Descriptor[MAX_DESC_SIZE];
extern Struct_Standard_Dev_Desc _USB_MEM StDescDevice;
extern Struct_Dev_Qualifier_Desc _USB_MEM StDescDeviceQualFs;
extern Struct_Dev_Qualifier_Desc _USB_MEM StDescDeviceQualHs;
extern BYTE _USB_MEM *ConfigStruct[];
extern _circ _X WORD *lcl_StringDescriptor[MAX_USB_STRINGS+1];
_reentrant void usb_PackDescriptor(void* pDescriptor,void _X * pDestination, WORD startIndex);

#ifdef MTP_BUILD
extern _reentrant void usb_PackDescriptor_From_X(Struct_String_Desc _X * pDescriptor,void _X * pDestination, WORD startIndex);
#endif


#ifdef MTP_BUILD
extern Struct_String_Desc _X StStringDescriptor[MAX_USB_STRINGS];
#else
extern Struct_String_Desc StStringDescriptor[MAX_USB_STRINGS];
#endif

// global flag created from hardware capability bit
extern BOOL g_bUsbHsEnabled;

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetDescriptor
//
//   Type:          Function
//
//   Description:   Get Descriptor
//
//   Inputs:        
//                  btDescType : Descriptor Type
//                  btDevSpeed : Actual speed of the device
//                  btIndex    : Depends on the type of descriptor
//                               For strings it is the strin Index
//                               For Config, it is the config number    
//   Outputs:       None
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _X * _reentrant GetDescriptor(BYTE btDescType, BYTE btDevSpeed, BYTE btIndex)
{ /* Body */
    
    INT     i, j;
    WORD    wReg;
    void _USB_MEM*pDescriptor;
    BOOL    bDescTypeOtherSpeed = FALSE;
    
    switch (btDescType)
    {

        case DESCRIPTOR_TYPE_DEVICE:

            // Copy speed dependent information
            
            if (btDevSpeed==USB_SPEED_HIGH)
            {
                StDescDevice.btDeviceClass    = StDescDeviceQualHs.btDeviceClass;
                StDescDevice.btDeviceSubclass = StDescDeviceQualHs.btDeviceSubclass;
                StDescDevice.btDeviceProtocol = StDescDeviceQualHs.btDeviceProtocol;
                StDescDevice.btMaxPacketSize0 = StDescDeviceQualHs.btMaxPacketSize0;
            }
            else
            {
                StDescDevice.btDeviceClass    = StDescDeviceQualFs.btDeviceClass;
                StDescDevice.btDeviceSubclass = StDescDeviceQualFs.btDeviceSubclass;
                StDescDevice.btDeviceProtocol = StDescDeviceQualFs.btDeviceProtocol;
                StDescDevice.btMaxPacketSize0 = StDescDeviceQualFs.btMaxPacketSize0;
            }
            

			// For devices that do not support High Speed USB such as STMP3501/02/03
			if(g_bUsbHsEnabled == FALSE)
			{
	            StDescDevice.btBcdUsbLsb = 0x10;
	            StDescDevice.btBcdUsbMsb = 0x01;
			}

            usb_PackDescriptor(&StDescDevice,Descriptor,0);

            break;

        
        case DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG:
            //in this case, lets change report the device speed we're not
            if(btDevSpeed == USB_SPEED_HIGH)
                btDevSpeed = USB_SPEED_FULL;
            else
                btDevSpeed = USB_SPEED_HIGH;

            // Set flag to modify the descriptor type after building the 
            // configuration descriptor
            bDescTypeOtherSpeed = TRUE;    
            
                
        case DESCRIPTOR_TYPE_CONFIG:
        // Prepare Configuration Descriptor transfer.
        // A configuration descriptor includes all related interfaces descriptor (1 or more per configuration)
        // and all related endpoints (1 or more per interface)
            {
                BYTE _USB_MEM **pConfig = ConfigStruct;
                int iConfigs=0;
                while(*pConfig)
                {
                    pConfig++;
                    pConfig++;
                    iConfigs++;
                }
            if((btIndex < 0) || (btIndex >= iConfigs))
                return(NULL);

            btIndex = 2 * btIndex;

            if(btDevSpeed==USB_SPEED_HIGH)
                btIndex ++;
           
            BuildConfigDescriptor((void _USB_MEM*)*(ConfigStruct+btIndex),Descriptor);
            
            if(bDescTypeOtherSpeed == TRUE)
            {
                // Modify the descriptor byte to DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG
                Descriptor[0] &= 0xff00ff;
                Descriptor[0] |= (DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG << 8);
            }

            }break;
                        
        case DESCRIPTOR_TYPE_STRING:
            {
                int iStrings = 0;
                WORD **pString = lcl_StringDescriptor;

                while(*pString)
                {
                    pString++;
                    iStrings++;
                }

				if( btIndex == MS_OS_STRING_DESCRIPTOR_INDEX )
				{
#ifdef MTP_BUILD
					usb_PackDescriptor_From_X(&StStringDescriptor[MS_OS_STRING_DESCRIPTOR_INDEX_IN_ARRAY],Descriptor,0);
#else				
					usb_PackDescriptor(&StStringDescriptor[MS_OS_STRING_DESCRIPTOR_INDEX_IN_ARRAY],Descriptor,0);
#endif
					break;
				}

                if(btIndex < iStrings)
                {
#ifdef MTP_BUILD
                    usb_PackDescriptor_From_X(&StStringDescriptor[btIndex],Descriptor,0);
#else				
                    usb_PackDescriptor(&StStringDescriptor[btIndex],Descriptor,0);
#endif
                }
                else
                {
                    // Return the last string descriptor by default
#ifdef MTP_BUILD
                    usb_PackDescriptor_From_X(&StStringDescriptor[iStrings-1],Descriptor,0);
#else				
                    usb_PackDescriptor(&StStringDescriptor[iStrings-1],Descriptor,0);
#endif
                }
            }break;

        case DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
            // Copy speed dependent information
            if(btDevSpeed==USB_SPEED_HIGH)
                pDescriptor = &StDescDeviceQualHs;
            else 
                pDescriptor = &StDescDeviceQualFs;

            usb_PackDescriptor(pDescriptor,Descriptor,0);
                
            break;

        default:
            return(NULL);

    } /* Endswitch */

    return((BYTE _X *)Descriptor);
} /* Endbody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BuildConfigDescriptor
//
//   Type:          Function
//
//   Description:   Build the string descriptor
//
//   Inputs:        BYTE _USB_MEM *pConfigRule[]        The rule to build a descriptor for
//
//   Outputs:       None
//
//   Notes:
//      This function is not really reentrant. It uses 1 global variable
//      that is not protected.         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant BuildConfigDescriptor(BYTE _USB_MEM *pConfigRule[], WORD _X *pPackedDescriptor)
{
    INT     i;
    WORD    wSizeMask;
    BYTE    btByteOffset, btCurElem;
    USHORT  usTotalBytes;    
    BYTE    _USB_MEM *pDescriptor;


    btByteOffset = 0;
    btCurElem = 0;                      //  Current element descriptor array
    usTotalBytes = 0;
    
    pDescriptor= *(pConfigRule);    // Points Current Descriptor

    while(pDescriptor != NULL)          // Process all descriptors from the Config Rule
    {                                   // NULL pointer demarks end of config
        usb_PackDescriptor(pDescriptor,pPackedDescriptor,usTotalBytes);
        usTotalBytes += *pDescriptor;   // Update total descriptor length
         

        pDescriptor = *(++pConfigRule);      // Points Current Descriptor
    
    };
    
    // Update the descriptor size
    *pPackedDescriptor &= 0x00ffff;
    wSizeMask = usTotalBytes & 0x0000ff;
    *pPackedDescriptor |= (wSizeMask << 16);
     
    *(pPackedDescriptor+1) &= 0xffff00;
    wSizeMask = usTotalBytes & 0x00ff00;
    *(pPackedDescriptor+1) |= (wSizeMask >> 8);

}
