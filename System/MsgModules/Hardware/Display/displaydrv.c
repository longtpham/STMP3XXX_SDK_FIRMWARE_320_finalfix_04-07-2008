#include "types.h"
#include "project.h"
#include "stringlib.h"
#include "SysResourceApi.h"


#ifdef STMP_BUILD_PLAYER
#include "sysmem.h"
#include "sysequ.h"
#else
#include "kernel.h"
#include "system.h"
#endif

#pragma optimize 1

#include "displaydrv.h"
#include "sal\display_sal.h"
#include "displaydriver.h"
#include "unicode.h"
#include "dbcs.h"

#define MAX_MASKS 5
#define MAX_DIGITS 10

INT g_iMaskLeft;
INT g_iMaskRight;
INT g_iMaskTop;
INT g_iMaskBottom;

_reentrant void PushMask(INT left, INT top, INT right, INT bottom);
_reentrant void PopMask(void);



MASK g_MaskStack[MAX_MASKS]={{0,0,LCD_X_SIZE,LCD_Y_SIZE}};
g_iMaskStack=0;
MASK *g_pMaskStack = g_MaskStack;

_reentrant BITMAP *DisplayLoadBitmap(WORD iResource);


_reentrant void EvaluateMask(void)
{
    int i;
    g_iMaskLeft = 0;
    g_iMaskRight = SALDisplayGetWidth();
    g_iMaskTop = 0;
    g_iMaskBottom = SALDisplayGetHeight();
    for(i=0;i<g_iMaskStack;i++)
    {
        if(g_iMaskLeft < g_MaskStack[i].m_iMaskLeft)
            g_iMaskLeft = g_MaskStack[i].m_iMaskLeft;
        if(g_iMaskTop < g_MaskStack[i].m_iMaskTop)
            g_iMaskTop = g_MaskStack[i].m_iMaskTop;
        if(g_iMaskRight > g_MaskStack[i].m_iMaskRight)
            g_iMaskRight = g_MaskStack[i].m_iMaskRight;
        if(g_iMaskBottom > g_MaskStack[i].m_iMaskBottom)
            g_iMaskBottom = g_MaskStack[i].m_iMaskBottom;
        if(g_iMaskBottom < g_iMaskTop)
            g_iMaskBottom = g_iMaskTop;
        if(g_iMaskRight < g_iMaskLeft)
            g_iMaskRight = g_iMaskLeft;
    }
}

_reentrant void PushMask(INT left, INT top, INT right, INT bottom)
{
    if(g_iMaskStack>MAX_MASKS-1)
        SystemHalt();
    g_iMaskStack++;
    g_pMaskStack->m_iMaskLeft  = left;
    g_pMaskStack->m_iMaskTop   = top;
    g_pMaskStack->m_iMaskRight = right;
    g_pMaskStack->m_iMaskBottom= bottom;
    g_pMaskStack++;
    EvaluateMask();
}

_reentrant void PopMask(void)
{
    if(!g_iMaskStack)
        SystemHalt();
    g_iMaskStack--;
    g_pMaskStack--;
    EvaluateMask();
}

_reentrant void DisplayUpdateDisplay(void)
{
    SALDisplayUpdate();
}

_reentrant void DisplayClearRange           (INT  x,INT  y,WORD cx         ,WORD cy    )
{
    SALDisplayClearRange(x,y,cx,cy);
}

_reentrant void DisplaySetFont(INT iFont)
{
    LCDSetFont(iFont);
}

_reentrant void DisplayPrintBitmapAddress(INT x,INT y,BITMAP *pBitmap,INT bInvert)
{
    SALDisplayPrintBitmapAddress(x,y,(void *)pBitmap,bInvert);
}

_reentrant void DisplayPrintBitmapResource  (INT x,INT y,WORD iResource  ,INT bInvert)
{
    BITMAP  *pBitmap = NULL;
    pBitmap = DisplayLoadBitmap(iResource);
    if(pBitmap)
        SALDisplayPrintBitmapAddress(x,y,(void *)pBitmap,bInvert);
}

_reentrant BITMAP *DisplayLoadBitmap(WORD wResource)
{
    INT iSize;
    BITMAP *pBITMAP;
    COMPLEX_RESOURCE Resource;
    BYTE btRscHandle;
    Resource.I = wResource;

    pBITMAP = (BITMAP *)&g_wUserScratchY;
    iSize = SCRATCH_USER_Y_SIZE*3;

    switch(Resource.Complex.m_Type)
    {
    case RESOURCE_TYPE_SIMPLE:
        SysLoadResource(wResource, RS_ROOT_HANDLE, RSRC_TYPE_BITMAP, (WORD*)pBITMAP, BITMAP_MEMORY_SPACE, iSize);
        break;
    case RESOURCE_TYPE_UNICODE:
        pBITMAP = GetUnicodeCharacterBitmap(Resource.Complex.m_Resource);
        break;
    case RESOURCE_TYPE_NESTED:
        btRscHandle = (BYTE) SysOpenResource(Resource.Complex.m_Resource&0x03f, RS_ROOT_HANDLE);                
        SysLoadResource(Resource.Complex.m_Resource>>10, btRscHandle, RSRC_TYPE_BITMAP,(WORD *)pBITMAP,BITMAP_MEMORY_SPACE, iSize);
        SysCloseResource(btRscHandle);
        break;
    }
    return pBITMAP;
}


_reentrant void DisplayPrintStringResource  (INT x,INT y,WORD wResource,INT bInvert)
{
    BYTE btRscHandle;
    INT iSize = SCRATCH_USER_Y_SIZE*3;
    WORD wStringResourceMemorySpace = TARGET_MEM_Y;
    WORD *pResourceString = (WORD*)&g_wUserScratchY;

    COMPLEX_RESOURCE Resource;
    Resource.I = wResource;

    switch(Resource.Complex.m_Type)
    {
    case RESOURCE_TYPE_SIMPLE:
        SysLoadResource(wResource, RS_ROOT_HANDLE, RSRC_TYPE_STRING,pResourceString, wStringResourceMemorySpace , iSize);
        break;
    case RESOURCE_TYPE_NESTED:
        btRscHandle = (BYTE) SysOpenResource(Resource.Complex.m_Resource&0x03f, RS_ROOT_HANDLE);                
        SysLoadResource(Resource.Complex.m_Resource>>10, btRscHandle, RSRC_TYPE_STRING,pResourceString, wStringResourceMemorySpace , iSize);
        SysCloseResource(btRscHandle);
        break;
    }
    DisplayPrintStringAddress(x,y,pResourceString,bInvert);
}


_reentrant void DisplayPrintStringResourceUnicode  (INT x,INT y,WORD wResource,INT bInvert)
{
    INT iSize = SCRATCH_USER_Y_SIZE*3;
    WORD wStringResourceMemorySpace = TARGET_MEM_Y;
    WORD *pResourceString = ((WORD*)&g_wUserScratchY)+50;//give it some space to avoid where the bitmap will be loaded
    COMPLEX_RESOURCE Resource;
    BYTE btRscHandle;

    Resource.I = wResource;

    switch(Resource.Complex.m_Type)
    {
    case RESOURCE_TYPE_SIMPLE:
        SysLoadResource(wResource, RS_ROOT_HANDLE, RSRC_TYPE_STRING,pResourceString, wStringResourceMemorySpace , iSize);
        break;
    case RESOURCE_TYPE_NESTED:
        btRscHandle = (BYTE) SysOpenResource(Resource.Complex.m_Resource&0x03f, RS_ROOT_HANDLE);                
        SysLoadResource(Resource.Complex.m_Resource>>10, btRscHandle, RSRC_TYPE_STRING,pResourceString, wStringResourceMemorySpace , iSize);
        SysCloseResource(btRscHandle);
        break;
    }
    DisplayPrintStringAddressUnicode(x,y,pResourceString,bInvert);
}


#ifdef FAT32TEST
WORD Fat32TestString[64];
#endif
_reentrant void DisplayPrintStringAddress   (INT x,INT y,void *pAddress ,INT bInvert)
{
    INT     iLength;
    INT     iSize = SCRATCH_USER_Y_SIZE;
    WORD    *pUnicodeString;

    iLength = packed_strlen((_packed unsigned char *)pAddress)+1;
#ifdef FAT32TEST
    // If same -3 is used, the first two words of pUnicodeString 
    // become corrupted resulting in garbage being displayed on
    // screen.  This seems to only affect fat32test
    pUnicodeString = Fat32TestString;
#else
    pUnicodeString = ((WORD*)&g_wUserScratchY)+iSize-iLength-3;
#endif
    //convert the string to unicode.
    DBCSToUnicode((_packed unsigned char *)pAddress,pUnicodeString,iLength);
    //display the string to unicode.
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert);


}

_reentrant int DisplayPrintCharUnicode   (INT  x,INT y,WORD wChar,INT bInvert)
{
    COMPLEX_RESOURCE Resource;
    BITMAP *pCharacter;
    WORD wWidth = 0;

    Resource.I = wChar;
    Resource.Complex.m_Type = RESOURCE_TYPE_UNICODE;
    pCharacter = DisplayLoadBitmap(Resource.I);
    if(pCharacter)
    {
        SALDisplayPrintBitmapAddress(x,y,pCharacter,bInvert);
        wWidth = pCharacter->m_iWidth;       
    }

    return wWidth;
}


_reentrant void DisplayPrintStringAddressUnicode   (INT  x,INT y,void *pAddress ,INT bInvert)
{
    COMPLEX_RESOURCE Resource;
    WORD *pString=pAddress;
    BITMAP *pCharacter;
    while(*pString)
    {
        x+=DisplayPrintCharUnicode(x,y,*pString++,bInvert);
        if(x > g_iMaskRight)
            break;//short cut to end print string quicker
    }
}

void StringPrintNumber(WORD *pUnicodeString,INT iNumber, INT iDigits, INT iFiller)
{
    int i;
    for(i=0;i<iDigits;i++)
    {
        if(iNumber||!i)
        {
            pUnicodeString[iDigits-i-1] = 0x30+ iNumber%10;
            iNumber /= 10;
        }
        else
        {
            pUnicodeString[iDigits-i-1] = iFiller;
        }

    }
    pUnicodeString[iDigits]=0;

}



_reentrant void DisplayPrintNumber(INT  x,INT y,INT iNumber,INT iDigits, INT iFiller, INT bInvert)
{
    WORD    pUnicodeString[MAX_DIGITS];
    if(iDigits < MAX_DIGITS)
    {
        StringPrintNumber(pUnicodeString, iNumber, iDigits,iFiller);
        DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert);
    }
}

#ifndef FAT32TEST
_reentrant void DisplayPrintTime    (INT x,INT y,INT iMinutes   ,INT iSeconds                   ,INT bInvert)
{
    WORD pUnicodeString[2+1+2+1+2];//2 for minutes, 1 for colon, 2 for seconds, 1 for terminator, and 2 for padding
    StringPrintNumber(pUnicodeString+0, iMinutes, 2,'0');
    pUnicodeString[2]=':';
    StringPrintNumber(pUnicodeString+3, iSeconds, 2,'0');
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert);
}

_reentrant void DisplayPrintLongTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert)
{
    WORD pUnicodeString[2+1+2+1+2+1+2];//2 for minutes, 1 for colon, 2 for seconds, 1 for terminator, and 2 for padding
    StringPrintNumber(pUnicodeString+0, iHours  , 2,'0');
    pUnicodeString[2]=':';
    StringPrintNumber(pUnicodeString+3, iMinutes, 2,'0');
    pUnicodeString[5]=':';
    StringPrintNumber(pUnicodeString+6, iSeconds, 2,'0');
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert);
}
#endif
_reentrant void DisplayHandleMessage(Message *pMsg)
{
    SALHandleMessage(pMsg);
}

// tt - added for Stmp00002965/2970 - now display hour if we're over an hour of play time.
_reentrant void DisplayPrint1DigitHourTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert)
{
    WORD pUnicodeString[2+1+2+1+2+1+2];// 2 for hours, 1 for colon, 2 for minutes, 1 for colon, 2 for seconds, 1 for terminator, and 2 for padding
    StringPrintNumber(pUnicodeString+0, iHours  , 2,'0');
    pUnicodeString[2]=':';
    StringPrintNumber(pUnicodeString+3, iMinutes, 2,'0');
    pUnicodeString[5]=':';
    StringPrintNumber(pUnicodeString+6, iSeconds, 2,'0');
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert);
}

