/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2005
//
// Filename: font_color.c    (Historical note: LcdPrintString.c)
// Description: Routines for display of text
/////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "project.h"
#include "resource.h"
#include "sysmem.h"
//#include "sysequ.h"
#include "SysResourceApi.h"
#include "stringlib.h"
#include "displaydrv_color.h"
#include "unicode.h"
#include "dbcs.h"

#define MBC_FLAG 0x010000
#define SGM_MASK 0x00ffff

#define MAX_STRING 30

#pragma optimize 1


extern INT g_iPDM;
extern INT g_iSDMS;
extern WORD S_currentFont_PGM;
extern WORD S_currentFont_SGMs;
extern WORD S_currentFont_Scripts;
extern WORD S_currentFont_Script_00;

extern BYTE g_btHandlePGM;
extern BYTE g_btHandleSGMs;
extern BYTE g_btHandleScript00;

_reentrant BITMAP * InternalGetUnicodeCharacterBitmap(WORD c);
_reentrant WORD InternalGetTextWidthAddressUnicode(WORD *pUnicodeString);

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm    
#endif    

void FontDummy(void)
    {
    }

/////////////////////////////////////////////////////////////////////////////////
//> Name: CLcdSetFont
//
//  Type: Function
//  Description: 		Set the current font.
//  Prototype:			void _reentrant CLcdSetFont(WORD fontNum);
//  Inputs: 				A = Font number (index of one of one or more possible fonts.  
//                                           Indexes start at ZERO)
//  Outputs: 
//  Notes: 		The FONT_TABLE resource has the following structure:
//					numberOfFonts
//					font1_PGM_resourceNumber  (or label)
//					font1_SGMs_resourceNumber (or label)
//                  font1_Scripts_rscNumber   (or label)
//                  font2_PGM_rscNumber       (if present)
//                  ...
//<
/////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        FLCDSetFont:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_SET_FONT

        org p,"SYSFUNCLET_SET_FONT_P":
        dc      RSRC_FUNCLET_SET_FONT
    #pragma endasm
    #define LCDSetFont LCDSetFontFunclet
#endif
#ifndef USBMSC_BUILD
void _reentrant LCDSetFont(WORD fontNum) {
#else
void LCDSetFont(WORD fontNum) {
#endif
  // Currently allowing for up to three fonts in the table
#ifndef MAX_FONTS
#define MAX_FONTS 3
#endif

  struct {
    WORD numFonts;
    struct {
      WORD PDM_rscNum;			//DBCS Map (Maps DBCS->Unicode)
      WORD SDMs_rscNum;
      WORD PGM_rscNum;			//Glyph Map (Maps Unicode->Glyphs)
      WORD SGMs_rscNum;
      WORD Script_00_rscNum;   //Glyphs	(these are the images)
      WORD Scripts_rscNum;
    } font[MAX_FONTS];
  } fontTable;


  // Read the FONT_TABLE resource

  SysLoadResource(RSRC_FONT_TABLE, RS_ROOT_HANDLE, RSRC_TYPE_DATA,
                    (WORD *) &fontTable, TARGET_MEM_Y, sizeof(fontTable)*3);

  if((fontNum < 0) || (fontNum > fontTable.numFonts))
    fontNum = 0;	//handle errors gracefully

  g_iPDM = fontTable.font[fontNum].PDM_rscNum;
  g_iSDMS =	fontTable.font[fontNum].SDMs_rscNum;
  S_currentFont_PGM = fontTable.font[fontNum].PGM_rscNum;
  S_currentFont_SGMs = fontTable.font[fontNum].SGMs_rscNum;
  S_currentFont_Script_00 = fontTable.font[fontNum].Script_00_rscNum;
  S_currentFont_Scripts = fontTable.font[fontNum].Scripts_rscNum;

    if(g_btHandlePGM)
    {
        SysCloseResource(g_btHandlePGM);
        g_btHandlePGM = 0;
    }

    if(g_btHandleSGMs)
    {
        SysCloseResource(g_btHandleSGMs);
        g_btHandleSGMs = 0;
    }
    
    if(g_btHandleScript00)
    {
        SysCloseResource(g_btHandleScript00);
        g_btHandleScript00 = 0;
    }
    
}

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextfont":
    #pragma endasm
#endif


#ifdef FUNCLET
    #pragma asm
        FGetTextWidthAddressUnicode:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_UNICODE

        org p,"SYSFUNCLET_GET_TEXT_WIDTH_ADDRESS_UNICODE_P":
        dc      RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_UNICODE
    #pragma endasm
    #define GetTextWidthAddressUnicode GetTextWidthAddressUnicodeFunclet
#endif
_reentrant INT GetTextWidthAddressUnicode(WORD *pUnicodeString)
{
    return InternalGetTextWidthAddressUnicode(pUnicodeString);
}

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextfont":
    #pragma endasm
#endif

#ifdef FUNCLET
    #pragma asm
        FGetTextWidthResourceUnicode:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_UNICODE

        org p,"SYSFUNCLET_GET_TEXT_WIDTH_RESOURCE_UNICODE_P":
        dc      RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_UNICODE
    #pragma endasm
    #define GetTextWidthResourceUnicode GetTextWidthResourceUnicodeFunclet
#endif
_reentrant INT GetTextWidthResourceUnicode(WORD wResource)
{
    WORD StringBuffer[MAX_STRING+1];
    WORD *pUnicodeString = StringBuffer;
    INT x=0;
    BITMAP *pBITMAP;
    //load up the unicode string

    SysLoadResource(wResource, RS_ROOT_HANDLE, RSRC_TYPE_STRING, StringBuffer,
                        TARGET_MEM_Y, MAX_STRING);

    StringBuffer[MAX_STRING]=0;
    return InternalGetTextWidthAddressUnicode(pUnicodeString);
}

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextfont":
    #pragma endasm
#endif




#ifdef FUNCLET
    #pragma asm
        FGetTextWidthAddressDBCS:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_DBCS

        org p,"SYSFUNCLET_GET_TEXT_WIDTH_ADDRESS_DBCS_P":
        dc      RSRC_FUNCLET_GET_TEXT_WIDTH_ADDRESS_DBCS
    #pragma endasm
    #define GetTextWidthAddressDBCS GetTextWidthAddressDBCSFunclet
#endif
_reentrant INT GetTextWidthAddressDBCS(_packed unsigned char *pDBCSString)
{
    WORD StringBuffer[MAX_STRING+1];
    WORD *pUnicodeString=StringBuffer;
    BITMAP *pBITMAP;
    INT x=0;
    DBCSToUnicode(pDBCSString,StringBuffer,MAX_STRING);
    StringBuffer[MAX_STRING]=0;
    return InternalGetTextWidthAddressUnicode(pUnicodeString);
}

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextfont":
    #pragma endasm
#endif

#ifdef FUNCLET
    #pragma asm
        FGetTextWidthResourceDBCS:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_DBCS

        org p,"SYSFUNCLET_GET_TEXT_WIDTH_RESOURCE_DBCS_P":
        dc      RSRC_FUNCLET_GET_TEXT_WIDTH_RESOURCE_DBCS
    #pragma endasm
    #define GetTextWidthResourceDBCS GetTextWidthResourceDBCSFunclet
#endif
_reentrant INT GetTextWidthResourceDBCS(WORD wResource)
{
    WORD StringBuffer[MAX_STRING+1];
    WORD *pUnicodeString= StringBuffer;
    INT x=0;
    BITMAP *pBITMAP;

    //load up the unicode string

    SysLoadResource(wResource, RS_ROOT_HANDLE, RSRC_TYPE_STRING,&g_wUserScratchY,
                        TARGET_MEM_Y, MAX_STRING);

    DBCSToUnicode((_packed unsigned char*)&g_wUserScratchY,StringBuffer,MAX_STRING);
    StringBuffer[MAX_STRING]=0;
    return InternalGetTextWidthAddressUnicode(pUnicodeString);
}

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextfont":
    #pragma endasm
#endif


#ifdef FUNCLET
    #pragma asm
        FGetUnicodeCharacterBitmap:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_GET_UNICODE_CHARACTER_BITMAP

        org p,"SYSFUNCLET_GetUnicodeCharacterBitmap_P":
        dc      RSRC_FUNCLET_GET_UNICODE_CHARACTER_BITMAP
    #pragma endasm
    #define GetUnicodeCharacterBitmap GetUnicodeCharacterBitmapFunclet
#endif
_reentrant BITMAP *  GetUnicodeCharacterBitmap(WORD c) 
{
    return InternalGetUnicodeCharacterBitmap(c);
}

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextfont":
    #pragma endasm
#endif
