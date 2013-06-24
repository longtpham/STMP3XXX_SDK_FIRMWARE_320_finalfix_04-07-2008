/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: LcdPrintString.c
// Description: Routines for display of text
/////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "displaydrv.h"
#include "unicode.h"
#include "resource.h"
#include "project.h"
#include "stringlib.h"
#include "dbcs.h"
#include "sysresourceapi.h"

#ifdef STMP_BUILD_PLAYER
#include "sysmem.h"
#include "sysequ.h"
#else
#include "kernel.h"
#include "system.h"
#endif

#define MBC_FLAG 0x010000
#define SGM_MASK 0x00ffff

#pragma optimize 1

/////////////////////////////////////////////////////////////////////////////////
// Private permanent storage - these should not be in overlay space!

 WORD S_currentFont_PGM;
 WORD S_currentFont_SGMs;
 WORD S_currentFont_Scripts;
 WORD S_currentFont_Script_00;


    BYTE g_btHandlePGM = 0;
    BYTE g_btHandleSGMs = 0;
    BYTE g_btHandleScript00 = 0;

/////////////////////////////////////////////////////////////////////////////////
//> Name: LookupPrimaryGlyphMapEntry
//
//  Type: Function
//  Description:
//  Prototype:		WORD _reentrant LookupPrimaryGlyphMapEntry(WORD c); 
//  Inputs: 
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
static WORD _reentrant LookupPrimaryGlyphMapEntry(WORD c) {
  WORD highByte = (c>>8) & 0x0000ff;
  WORD GlyphMapEntry;
  int offset;

  offset = highByte + 3;                            // 1 word per entry + skip over PGM RSRC header

  if (g_btHandlePGM > 0) {
    // do nothing
  }
  else {

    g_btHandlePGM = (BYTE) SysOpenResource(S_currentFont_PGM, RS_ROOT_HANDLE);

  }
    
  SysResourceFileSeek(g_btHandlePGM, 0);
  SysResourceFileSeek(g_btHandlePGM, offset);
  SysResourceFileRead(g_btHandlePGM, 3, TARGET_MEM_Y, &GlyphMapEntry);

  return(GlyphMapEntry);
}

/////////////////////////////////////////////////////////////////////////////////
//> Name: LookupSecondaryGlyphMapEntry
//
//  Type: Function
//  Description:
//  Prototype:		WORD _reentrant LookupSecondaryGlyphMapEntry(WORD SecondaryMapIndex, WORD c); 
//  Inputs: 
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
static WORD _reentrant LookupSecondaryGlyphMapEntry(WORD SecondaryMapIndex, WORD c) {
  WORD GlyphMapEntry;
  WORD glyph_index = c & 0x0000ff;
  WORD rscNum = glyph_index + 1;
  WORD SecondaryMapNumber = SecondaryMapIndex + 1;
  int SGM_size;
  int offset;
  BYTE btHandle;

  SGM_size = 3 + 256;                                   // 3 words for each SGM RSRC hdr +
                                                        // 1 word for each of 256 SGM entries in RSRC

  offset = 3 +                                        // 3 words for SGM header
    SecondaryMapIndex * SGM_size +                      // Skip over other SGMs
    glyph_index;                                    // Skip to desired glyph entry


 if (g_btHandleSGMs > 0) {
    // nothing to do

 }
 else 
 {
   btHandle = (BYTE)SysOpenResource(S_currentFont_SGMs, RS_ROOT_HANDLE); 
   g_btHandleSGMs = (BYTE)SysOpenResource(1, btHandle);
   SysCloseResource(btHandle);

 }

 SysResourceFileSeek(g_btHandleSGMs, 0);
 SysResourceFileSeek(g_btHandleSGMs, offset);
 if (SysResourceFileRead(g_btHandleSGMs,3, TARGET_MEM_Y, &GlyphMapEntry) != SUCCESS)
   return(0);
 else
   return(GlyphMapEntry);
     
}

/////////////////////////////////////////////////////////////////////////////////
//> Name: LoadGlyph
//
//  Type: Function
//  Description: 
//  Prototype:		void _reentrant LoadGlyph(WORD ScriptIndex, WORD GlyphIndex, BITMAP_STRUCT *pBitmap);
//  Inputs: 
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
static void _reentrant LoadGlyph(WORD ScriptIndex, WORD GlyphIndex, BITMAP *pBitmap, WORD maxSize) {

  BYTE btHandle;
  BYTE btHandleScript;
  
  if (ScriptIndex == 0)
  {                                 // Script_00 is separate, for speed
        if (g_btHandleScript00 > 0)
        {
          // do nothing
        }
        else 
        {
          g_btHandleScript00 = (BYTE) SysOpenResource(S_currentFont_Script_00, RS_ROOT_HANDLE);
        }

        SysLoadResource(GlyphIndex+1, g_btHandleScript00, RSRC_TYPE_BITMAP, (WORD *) pBitmap, 
                           TARGET_MEM_Y, maxSize);

  }
  else 
  {
        btHandle = (BYTE)SysOpenResource(S_currentFont_Scripts, RS_ROOT_HANDLE); 

        btHandleScript = (BYTE)SysOpenResource(ScriptIndex+1, btHandle);

      SysLoadResource(GlyphIndex+1, btHandleScript, RSRC_TYPE_BITMAP, (WORD *) pBitmap, 
                           TARGET_MEM_Y, maxSize);
                    
      SysCloseResource(btHandle);                           
      SysCloseResource(btHandleScript);                           
  }
  
}


/////////////////////////////////////////////////////////////////////////////////
//> Name: GetUnicodeCharacterFramebuffer
//
//  Type: Function
//  Description: Display the specified Unicode character at the specified location
//               on the LCD.
//  Prototype:   WORD _reentrant LcdPrintUnicodeChar(WORD c);
//  Inputs:      A  = Unicode character value
//  Outputs:     r0 = new FRAMEBUFFER pointer.
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////

_reentrant BITMAP *  InternalGetUnicodeCharacterBitmap(WORD c) {

    INT iSize;
    BITMAP *pBitmap;
    
    WORD GlyphMapEntry;
    WORD ScriptIndex;
    WORD GlyphIndex;
    WORD nextX;

    if (c > 255) 
    {
        GlyphMapEntry = LookupPrimaryGlyphMapEntry(c);  // SGMs > 00
    }
    else
    {
        GlyphMapEntry = MBC_FLAG;            // SGM_00 is ALWAYS present
    }


    if (GlyphMapEntry & MBC_FLAG) 
    {
        GlyphMapEntry = LookupSecondaryGlyphMapEntry(GlyphMapEntry & SGM_MASK, c);
    }

    ScriptIndex = (GlyphMapEntry >> 8) & 0x0000ff;
    GlyphIndex = GlyphMapEntry & 0x0000ff;

    pBitmap = (BITMAP *)&g_wUserScratchY;
    iSize = SCRATCH_USER_Y_SIZE;

    LoadGlyph(ScriptIndex, GlyphIndex, pBitmap, iSize);

    return pBitmap;
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

_reentrant WORD InternalGetTextWidthAddressUnicode(WORD *pUnicodeString)
{
    INT x=0;
    BITMAP *pBITMAP;
    while(1)
    {
        if(*pUnicodeString)
        {
            pBITMAP = InternalGetUnicodeCharacterBitmap(*pUnicodeString++);
            x+= pBITMAP->m_iWidth;
        }
        else
        {
            break;
        }
    }
    return x;
}

