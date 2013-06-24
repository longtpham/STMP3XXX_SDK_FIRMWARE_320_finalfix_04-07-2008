#include "project.h"
#include "usb_api.h"
#include "usb_internal.h"
#include "stringlib.h"

#ifdef MTP_BUILD
#include "mtprequests.h"
#endif


#pragma optimize 1

#ifdef MTP_BUILD
#ifndef CUSTOMIZE_UNICODE
extern const BYTE g_MSOSDescriptorString[];
#else
extern const WORD g_MSOSDescriptorString[];
#endif
#endif

#ifdef MTP_BUILD
_reentrant void usb_CreateStringDescriptor(UCS3 * pString,Struct_String_Desc _X * pDestination);
#else
_reentrant void usb_CreateStringDescriptor(UCS3 * pString,Struct_String_Desc * pDestination);
#endif


//define the memory for the strings
_circ _X WORD lcl_String0[(64+2)/3];
_circ _X WORD lcl_String1[(64+2)/3];
_circ _X WORD lcl_String2[(64+2)/3];
_circ _X WORD lcl_String3[(64+2)/3];
_circ _X WORD lcl_String4[(64+2)/3];
_circ _X WORD lcl_String5[(64+2)/3];
_circ _X WORD lcl_String6[(64+2)/3];
_circ _X WORD lcl_String7[(64+2)/3];

//define the array of descriptor pointers
_circ _X WORD *lcl_StringDescriptor[MAX_USB_STRINGS+1]=
    {
        lcl_String0,
        lcl_String1,
        lcl_String2,
        lcl_String3,
        lcl_String4,
        lcl_String5,
        lcl_String6,
        lcl_String7,
        NULL
};


#ifdef MTP_BUILD
Struct_String_Desc _X StStringDescriptor[MAX_USB_STRINGS];
#else
Struct_String_Desc StStringDescriptor[MAX_USB_STRINGS];
#endif

_reentrant void usb_PackDescriptor(void _USB_MEM * pDescriptor,void _X * pDestination, WORD startIndex)
{
    BYTE *pDescriptorByteArray=pDescriptor;
    BYTE btData;
    INT iLength;
    INT i;
    iLength = *pDescriptorByteArray;
    for(i=0;i<iLength;i++)
    {
        btData = *pDescriptorByteArray++;
        PutByteInArrayInX(pDestination,startIndex+i,btData);
    }
}

#ifdef MTP_BUILD
_reentrant void usb_PackDescriptor_From_X(Struct_String_Desc _X * pDescriptor,void _X * pDestination, WORD startIndex)
{
    BYTE _X *pDescriptorByteArray=(BYTE _X *)pDescriptor;
    BYTE btData;
    INT iLength;
    INT i;

    iLength = *pDescriptorByteArray;

    for(i=0;i<iLength;i++)
    {
        btData = *pDescriptorByteArray++;
        PutByteInArrayInX(pDestination,startIndex+i,btData);
    }
}
#endif

#ifdef MTP_BUILD
    #ifdef JANUS_BUILD // DRM9 & DRM10
_reentrant void usb_CreateStringDescriptor(UCS3 * pString,Struct_String_Desc _X * pDestination)
{
    BYTE _X *pStringBuf=(BYTE _X *)pDestination;
    INT iLength;
    INT i=0;
    WORD wValue;
   	BYTE bMSOSDesc=TRUE;
    iLength = strlen(pString);

	for(i=0; i<iLength; i++ )
	{
		if( pString[i] != g_MSOSDescriptorString[i] )
		{
			bMSOSDesc = FALSE;
			break;
		}
	}

	if( bMSOSDesc )
        *pStringBuf++ = iLength*2+2+2;
	else
		*pStringBuf++ = iLength*2+2;

    *pStringBuf++ = DESCRIPTOR_TYPE_STRING;
    while((wValue = *pString++))
    {
        *pStringBuf++ = (wValue)&0xff;
        *pStringBuf++ = (wValue)>>8;
    }

    if( bMSOSDesc )
	{
		wValue=MSOS_VENDORCODE_TO_GET_MS_DESCRIPTOR;
        *pStringBuf++ = (wValue)&0xff;
        *pStringBuf++ = (wValue)>>8;
	}

}
    #else // DRM9 only, don't declare it as MTP device
_reentrant void usb_CreateStringDescriptor(UCS3 * pString,Struct_String_Desc _X * pDestination)
{
    BYTE _X *pStringBuf=(BYTE _X *)pDestination;
    INT iLength;
    INT i=0;
    WORD wValue;
   	BYTE bMSOSDesc=TRUE;
    iLength = strlen(pString);

	for(i=0; i<iLength; i++ )
	{
		if( pString[i] != g_MSOSDescriptorString[i] )
		{
			bMSOSDesc = FALSE;
			break;
		}
	}

	if( bMSOSDesc )
    {
        *pStringBuf++ = 2;
        *pStringBuf++ = DESCRIPTOR_TYPE_STRING;
        *pStringBuf++ = 0;
        *pStringBuf++ = 0;
	}
	else
	{
	    *pStringBuf++ = iLength*2+2;

        *pStringBuf++ = DESCRIPTOR_TYPE_STRING;
        while((wValue = *pString++))
        {
            *pStringBuf++ = (wValue)&0xff;
            *pStringBuf++ = (wValue)>>8;
        }
    }
}
    #endif
#else

_reentrant void usb_CreateStringDescriptor(UCS3* pString,Struct_String_Desc * pDestination)
{
    BYTE *pStringBuf=(BYTE*)pDestination;
    INT iLength;
    INT i=0;
    WORD wValue;
   	BYTE bMSOSDesc=TRUE;
    iLength = strlen(pString);

    *pStringBuf++ = iLength*2+2;
    *pStringBuf++ = DESCRIPTOR_TYPE_STRING;
    while((wValue = *pString++))
    {
        *pStringBuf++ = (wValue)&0xff;
        *pStringBuf++ = (wValue)>>8;
    }
}

#endif

extern void * g_pStrings[];

_reentrant void parse_strings(void)
{
    int i;
    for(i=0;i<MAX_USB_STRINGS;i++)
    {
        StStringDescriptor[i].btLength = 0;
    }

    i=0;
    while(g_pStrings[i])
    {
#ifdef MTP_BUILD
        usb_CreateStringDescriptor(g_pStrings[i], (void _X *)&StStringDescriptor[i]);
#else
        usb_CreateStringDescriptor(g_pStrings[i], (void *)&StStringDescriptor[i]);
#endif

#ifdef MTP_BUILD
		usb_PackDescriptor_From_X(&StStringDescriptor[i],(void _X *)lcl_StringDescriptor[i],0);
#else
        usb_PackDescriptor(&StStringDescriptor[i],(void _X *)lcl_StringDescriptor[i],0);
#endif
        i++;                                      //compiler assertion fails w/o cast
    }

    // NULL out the unused string descriptors
    for( ; i < MAX_USB_STRINGS ; i++)
    {
        lcl_StringDescriptor[i] = NULL;
    }


}

