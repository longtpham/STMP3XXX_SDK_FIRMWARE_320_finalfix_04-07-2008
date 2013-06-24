#include "usb_api.h"
#include "usb_internal.h"

_reentrant void usb_PackDescriptor(void* pDescriptor,void _X * pDestination, WORD startIndex)
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

_reentrant void usb_CreateStringDescriptor(UCS3* pString,Struct_String_Desc * pDestination)
{
    BYTE *pStringBuf=(BYTE*)pDestination;
    INT iLength;
    INT i=0;
    WORD wValue;
    iLength = strlen(pString);
    *pStringBuf++ = iLength*2+2;
    *pStringBuf++ = DESCRIPTOR_TYPE_STRING;
    while((wValue = *pString++))
    {
        *pStringBuf++ = (wValue)&0xff;
        *pStringBuf++ = (wValue)>>8;
    }
}

extern void *g_pStrings[];
_reentrant void parse_strings(void)
{
    int i;
    for(i=0;i<MAX_STRINGS;i++)
    {
        StStringDescriptor[i].btLength = 0;
    }
    i=0;
    while(g_pStrings[i])
    {
        usb_CreateStringDescriptor(g_pStrings[i], &StStringDescriptor[i]);
        usb_PackDescriptor(&StStringDescriptor[i],lcl_StringDescriptor[i],0);
        i++;
    }
}


_reentrant void usb_CreateStringDescriptor(UCS3* pString,Struct_String_Desc * pDestination)
{
    BYTE *pStringBuf=(BYTE*)pDestination;
    INT iLength;
    INT i=0;
    WORD wValue;
    iLength = strlen(pString);
    *pStringBuf++ = iLength*2+2;
    *pStringBuf++ = DESCRIPTOR_TYPE_STRING;
    while((wValue = *pString++))
    {
        *pStringBuf++ = (wValue)&0xff;
        *pStringBuf++ = (wValue)>>8;
    }
}

extern void *g_pStrings[];
_reentrant void parse_strings(void)
{
    int i;
    for(i=0;i<MAX_STRINGS;i++)
    {
        StStringDescriptor[i].btLength = 0;
    }
    i=0;
    while(g_pStrings[i])
    {
        usb_CreateStringDescriptor(g_pStrings[i], &StStringDescriptor[i]);
        usb_PackDescriptor(&StStringDescriptor[i],lcl_StringDescriptor[i],0);
        i++;
    }
}
