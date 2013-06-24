////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : displaydrv_color.h
// Description : LCD driver Header file
////////////////////////////////////////////////////////////////////////////////
#ifndef __DISPLAY_H
#define __DISPLAY_H


#include "project.h"

#define MAX_BITMAP_SIZE 10000

//comment this out to put the frame buffer in Y

#ifndef LCD_X_SIZE     
#define LCD_X_SIZE          128
#endif

#ifndef LCD_Y_SIZE
#define LCD_Y_SIZE          159
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


#define MONOCHROME_BITMAP_RESOURCE 0
#define COLOR_BITMAP_RESOURCE 1
typedef struct 
{
    INT m_ColorType;	//used to differentiate between color and monochrome
    INT m_iWidth;
    INT m_iHeight;
    WORD m_wBitmap[MAX_BITMAP_SIZE];
} BITMAP;


typedef struct 
{
    INT m_ColorType;
    INT m_iWidth;
    INT m_iHeight;
} BITMAPHEADER;

typedef struct
{
	LONG      biSize;
	LONG       biWidth;
	LONG       biHeight;
	LONG       biPlanes;
	LONG       biBitCount;
	LONG      biCompression;
	LONG      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	LONG      biClrUsed;
	LONG      biClrImportant;
	///////////////////
	INT       iColorSize;
	LONG	  diHeight;
	BITMAP*   pBITMAP;
	DWORD     dwHeadSkipByte;
	DWORD     dwWidthByte;
	DWORD     dwSkipByte;
	WORD	  wBitMask[3];
	WORD	  wBitShift[3];


} _BITMAPINFOHEADER;

#define BITMAPFILEHEADER_STRUCT_SIZE            14
#define BITMAPFILEHEADER_TYPE                   ('B'<<8|'M')

#define BITMAPCOREHEADER_STRUCT_SIZE            12

#define BITMAPINFOHEADER_STRUCT_SIZE            40
#define BITMAPINFOHEADER_SIZE_OFFSET            0
#define BITMAPINFOHEADER_WIDTH_OFFSET           4
#define BITMAPINFOHEADER_HEIGHT_OFFSET          8
#define BITMAPINFOHEADER_PLANES_OFFSET          12
#define BITMAPINFOHEADER_BITCOUNT_OFFSET        14
#define BITMAPINFOHEADER_COMPRESSION_OFFSET     16
#define BITMAPINFOHEADER_SIZEIMAGE_OFFSET       20
#define BITMAPINFOHEADER_XPELSPERMETER_OFFSET   24
#define BITMAPINFOHEADER_YPELSPERMETER_OFFSET   28
#define BITMAPINFOHEADER_COLORUSED_OFFSET       32
#define BITMAPINFOHEADER_COLORIMPORTANT_OFFSET  36

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

#define DISPLAY_NORMAL  	0x00
#define DISPLAY_INVERT  	0x01
#define DISPLAY_CLEAR   	0x02
#define DISPLAY_MONOCHROME 	0x04
#define DISPLAY_COLOR		0x08
#define DISPLAY_TEXT    	0x10
#define DISPLAY_BUFFER		0x20 
#define DISPLAY_HORZ_FLIP   0x40
#define DISPLAY_VERT_FLIP   0x80
#define DISPLAY_ROTATION	0x100

#define DIRECT_TO_LCD_BUFFER -1

void _reentrant DisplayInit(void);


_reentrant void DisplayHandleMessage(Message *);

_reentrant void DisplayUpdateDisplay(void);


_reentrant void DisplayPrintBitmapResource  (INT  x,INT  y,WORD wResource  ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintBitmapFile(INT x, INT y, void* pFileName, INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintStringResource  (INT  x,INT  y,WORD wResource  ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintStringAddress   (INT  x,INT  y,void *pAddress  ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplaySetFont(INT);
_reentrant void DisplayPrintBitmapAddress(INT x,INT y,BITMAP *pBitmap,INT bInvert);
_reentrant void DisplayPrintStringAddressUnicode   (INT x,INT y,void *pAddress ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintStringResourceUnicode  (INT x,INT y,WORD wResource,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintNumber  (INT x,INT y,INT iNumber    ,INT iDigits    ,INT iFiller    ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintTime    (INT x,INT y,INT iMinutes   ,INT iSeconds                   ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplayPrintLongTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert, INT iFrameBufferID);
_reentrant void DisplaySetFont(INT iFont);
_reentrant int DisplayPrintCharUnicode   (INT  x,INT y,WORD wChar,INT bInvert, INT iFrameBufferID);


#endif




																	   