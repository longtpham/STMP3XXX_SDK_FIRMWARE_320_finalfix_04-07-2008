#ifndef _DISPLAY_MODULE_H
#define _DISPLAY_MODULE_H


#define SIMPLE_RESOURCE 0
#define NESTED_RESOURCE 1

#include "project.h"
#include "sysresourceapi.h"

//comment this out to put the frame buffer in Y

#ifndef BITMAP_MEMORY_SPACE
#define BITMAP_MEMORY_SPACE TARGET_MEM_Y
#endif


#if  BITMAP_MEMORY_SPACE==TARGET_MEM_X
#define BITMAP_MEMORY_C _X
#else 
#define BITMAP_MEMORY_C _Y
#endif 

#ifndef LCD_X_SIZE     
#define LCD_X_SIZE          98
#endif

#ifndef LCD_Y_SIZE
#define LCD_Y_SIZE          64
#endif

#define MAX_BITMAP_SIZE 10000

#define DISPLAY_NORMAL  0
#define DISPLAY_INVERT  1
#define DISPLAY_CLEAR   2 

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
} BITMAP_TYPEDEF;

#define BITMAP BITMAP_TYPEDEF BITMAP_MEMORY_C

typedef struct 
{
    INT m_iWidth;
    INT m_iHeight;
} BITMAPHEADER_TYPEDEF;

#define BITMAPHEADER BITMAPHEADER_TYPEDEF BITMAP_MEMORY_C

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


extern INT g_iMaskLeft;
extern INT g_iMaskRight;
extern INT g_iMaskTop;
extern INT g_iMaskBottom;


_reentrant void SALDisplayClearRange(INT x,INT y,WORD iSourceWidth,WORD iSourceHeight);
_reentrant void SALDisplayPrintBitmapAddress   (INT x,INT y,void _Y *pAddress ,INT bInvert);
_reentrant void SALHandleMessage(Message  *pMsg);
_reentrant void SALUpdateDisplay(void);
_reentrant void SALDisplayInit(void );

_reentrant INT SALDisplayGetWidth(void);
_reentrant INT SALDisplayGetHeight(void);

_reentrant void PushMask(INT left, INT top, INT right, INT bottom);
_reentrant void PopMask(void);


#endif
