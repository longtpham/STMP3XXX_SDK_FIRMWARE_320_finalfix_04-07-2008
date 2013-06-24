///////////////////////////////////////////////////////////////////////////////
///
/// Filename: displaydrv.h
/// Description: Prototypes for the display driver files.
///
///
/// Copyright (c) SigmaTel, Inc. Unpublished
///
/// SigmaTel, Inc.
/// Proprietary  Confidential
///
/// This source code and the algorithms implemented therein constitute
/// confidential information and may comprise trade secrets of SigmaTel, Inc.
/// or its associates, and any use thereof is subject to the terms and
/// conditions of the Confidential Disclosure Agreement pursuant to which this
/// source code was originally received.
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "project.h"

#define MAX_BITMAP_SIZE 10000

//comment this out to put the frame buffer in Y

#ifndef LCD_X_SIZE     
#define LCD_X_SIZE          98
#endif

#ifndef LCD_Y_SIZE
#define LCD_Y_SIZE          64
#endif

#define BITMAP_MEMORY_SPACE TARGET_MEM_Y

typedef union
{
    struct
    {
        INT m_Resource:21;
        INT m_Type :3;
    } Complex;
    WORD I;
} COMPLEX_RESOURCE;

#define RESOURCE_TYPE_SIMPLE    0
#define RESOURCE_TYPE_NESTED    1
#define RESOURCE_TYPE_UNICODE   2


typedef struct 
{
    INT m_iMaskLeft;
    INT m_iMaskTop;
    INT m_iMaskRight;
    INT m_iMaskBottom;
}   MASK;

typedef struct 
{
    INT m_iWidth;
    INT m_iHeight;
    WORD m_wBitmap[MAX_BITMAP_SIZE];
} BITMAP;


typedef struct 
{
    INT m_iWidth;
    INT m_iHeight;
} BITMAPHEADER;

#define DISPLAY_NORMAL  0
#define DISPLAY_INVERT  1
#define DISPLAY_CLEAR   2 

void _reentrant DisplayInit(void);


_reentrant void DisplayHandleMessage(Message *);

_reentrant void DisplayUpdateDisplay(void);

_reentrant void DisplayClearRange           (INT  x,INT  y,WORD cx         ,WORD cy    );
_reentrant void DisplayPrintBitmapResource  (INT  x,INT  y,WORD wResource  ,INT bInvert);
_reentrant void DisplayPrintStringResource  (INT  x,INT  y,WORD wResource  ,INT bInvert);
_reentrant void DisplayPrintStringAddress   (INT  x,INT  y,void *pAddress  ,INT bInvert);
_reentrant void DisplaySetFont(INT);
_reentrant void DisplayPrintBitmapAddress(INT x,INT y,BITMAP *pBitmap,INT bInvert);
_reentrant void DisplayPrintStringAddressUnicode   (INT x,INT y,void *pAddress ,INT bInvert);
_reentrant void DisplayPrintStringResourceUnicode  (INT x,INT y,WORD wResource,INT bInvert);
_reentrant void DisplayPrintNumber  (INT x,INT y,INT iNumber    ,INT iDigits    ,INT iFiller    ,INT bInvert);
_reentrant void DisplayPrintTime    (INT x,INT y,INT iMinutes   ,INT iSeconds                   ,INT bInvert);
_reentrant void DisplayPrintLongTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert);
_reentrant void DisplaySetFont(INT iFont);
_reentrant int DisplayPrintCharUnicode   (INT  x,INT y,WORD wChar,INT bInvert);


#endif