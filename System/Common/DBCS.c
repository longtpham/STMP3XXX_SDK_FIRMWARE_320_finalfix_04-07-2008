///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: DBCS.c
// Description: DBCS (Localized Fonts) routines.
///////////////////////////////////////////////////////////////////////////////


#include "types.h"
#include "resource.h"
#include "stringlib.h"
#include "sysresourceapi.h"

///////////////////////////////////////////////////////////////////////////////
//	External function prototypes

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm    
#endif    


WORD _reentrant LookupPrimaryDBCSMapEntry(WORD c);
WORD _reentrant LookupSecondaryDBCSMapEntry(WORD SecondaryMapNumber, WORD c);
//3200Merge+
#if !defined(USBMSC_BUILD) &&  defined(FUNCLET)
// Following functions are just duplication of the above, including function body
WORD _reentrant LookupPrimaryDBCSMapEntry_Private1(WORD c);
WORD _reentrant LookupSecondaryDBCSMapEntry_Private1(WORD SecondaryMapNumber, WORD c);
WORD _reentrant LookupPrimaryDBCSMapEntry_Private2(WORD c);
WORD _reentrant LookupSecondaryDBCSMapEntry_Private2(WORD SecondaryMapNumber, WORD c);
#endif
//3200Merge-

#define MBC_FLAG 0x010000

                                            // Note: Once set, these don't go bad, unless
                                            // we change to allow multiple Code Pages
INT g_iPDM = RSRC_PDM;
INT g_iSDMS = RSRC_SDMS;

BYTE s_btHandle_PDM = 0;
BYTE s_bthandle_SDM = 0;

//#if defined(USBMSC_BUILD) && defined(LIION) && defined(MMC)
#if defined(USBMSC_BUILD)
extern _P WORD * _P _lc_u_p_usbmsc_lcd_battery_start;
extern _Y WORD * _Y _lc_u_y_Usbmsc_lcd_OVLY_copy_start;
extern _Y WORD * _Y _lc_u_y_Usbmsc_lcd_OVLY_copy_end;
extern _Y WORD * _Y _lc_u_y_dbcs_usbmsc_OVLY_copy_start;
extern _Y WORD * _Y _lc_u_y_dbcs_usbmsc_OVLY_copy_end;

extern _reentrant void CopyCodeIntoBootmanagerArea(_Y WORD * _Y pYCopyStart, 
                _P WORD * _P pPCopyStart, WORD wYCopySize);

_reentrant void DBCSToUnicode_wrapper(_packed unsigned char *pDBCS, WORD *pUnicode,INT iLength);
_reentrant void DBCSToUnicodeDstXSrcY_wrapper(_packed BYTE *pDBCS, WORD _X * _X pUnicode, INT iLength);
#endif

void DBCSDummy (void)
{
}


#ifdef FUNCLET
    #pragma asm
        FDBCSToUnicode:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_DBCS_TO_UNICODE

        org p,"SYSFUNCLET_DBCS_TO_UNICODE_P":
        dc      RSRC_FUNCLET_DBCS_TO_UNICODE
    #pragma endasm
    #define DBCSToUnicode DBCSToUnicodeFunclet
#endif
_reentrant void DBCSToUnicode(_packed unsigned char *pDBCS, WORD *pUnicode,INT iLength)
{
//#if defined(USBMSC_BUILD) && defined(LIION) && defined(MMC)
#if defined(USBMSC_BUILD)
    unsigned int CopySize = (unsigned int)(&_lc_u_y_dbcs_usbmsc_OVLY_copy_end-&_lc_u_y_dbcs_usbmsc_OVLY_copy_start);
    
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_dbcs_usbmsc_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
  
    DBCSToUnicode_wrapper(pDBCS,pUnicode,iLength);

    CopySize = (unsigned int)(&_lc_u_y_Usbmsc_lcd_OVLY_copy_end-&_lc_u_y_Usbmsc_lcd_OVLY_copy_start);
        
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_Usbmsc_lcd_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
    
#else
    int iDBCSIndex=0;
    int iUnicodeIndex=0;
    WORD c;
    WORD DBCSMapEntry;
    while (1) 
    {
        c = packed_get(pDBCS,iDBCSIndex++);

        if (c == 0)
        {
            *pUnicode++ = 0;
            break;
        }
#if 1 //3200Merge
    #ifdef FUNCLET
        DBCSMapEntry = LookupPrimaryDBCSMapEntry_Private1(c);
    #else
        DBCSMapEntry = LookupPrimaryDBCSMapEntry(c);
    #endif
#else
        DBCSMapEntry = LookupPrimaryDBCSMapEntry(c);
#endif
        if (DBCSMapEntry & MBC_FLAG) 
        {

          c = packed_get(pDBCS, iDBCSIndex++);
#if 1 //3200Merge
    #ifdef FUNCLET
          c = LookupSecondaryDBCSMapEntry_Private1(DBCSMapEntry & (unsigned)~MBC_FLAG, c);
    #else          
          c = LookupSecondaryDBCSMapEntry(DBCSMapEntry & (unsigned)~MBC_FLAG, c);
    #endif
#else
          c = LookupSecondaryDBCSMapEntry(DBCSMapEntry & (unsigned)~MBC_FLAG, c);
#endif
        }
        else
        {
          c = DBCSMapEntry;
        }

        *pUnicode++ = c;
        iUnicodeIndex++;
        if(iUnicodeIndex > iLength -2 )
        {
            *pUnicode=0;
            break;
        }
    }
#endif
}

#if 1 //3200Merge
#if !defined(USBMSC_BUILD) &&  defined(FUNCLET)
/////////////////////////////////////////////////////////////////////////////////
//> Name: LookupPrimaryDBCSMapEntry
//
//  Type: Function
//  Description:
//  Prototype:		WORD _reentrant LookupPrimaryDBCSMapEntry(WORD c);
//  Inputs:
//  Outputs:
//  Notes:
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant LookupPrimaryDBCSMapEntry_Private1(WORD c) {
  WORD highByte = c & 0x0000ff;
  WORD DBCSMapEntry;
  int offset;
                                    // The PDM file is stored as a single resource with
                                    // 256 entries, each one data word.  You don't LOAD
                                    // the whole resource;  just OPEN it and then SEEK
                                    // to the deisred entry.

  offset =
    3 +                           // PDM Resource Header
    highByte;                     // 1 words per PDM entry

    if(s_btHandle_PDM > 0)
    {
        // do nothing
    }
    else
    {
        s_btHandle_PDM = (BYTE) SysOpenResource(g_iPDM, RS_ROOT_HANDLE);
    }

    SysResourceFileSeek(s_btHandle_PDM, 0);
    SysResourceFileSeek(s_btHandle_PDM, offset);
    SysResourceFileRead(s_btHandle_PDM, 3, TARGET_MEM_Y, &DBCSMapEntry);

  return(DBCSMapEntry);
}

/////////////////////////////////////////////////////////////////////////////////
//> Name: LookupSecondaryDBCSMapEntry
//
//  Type: Function
//  Description:
//  Prototype:		WORD _reentrant LookupSecondaryDBCSMapEntry(WORD SecondaryMapNumber, WORD c);
//  Inputs:
//  Outputs:
//  Notes:
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant LookupSecondaryDBCSMapEntry_Private1(WORD SecondaryMapIndex, WORD c) {
  WORD DBCSMapEntry;
  WORD charIdx = c & 0x0000ff;
  int SDM_header;
  int SDM_size;
  int offset;
  BYTE btHandle;
                                    // The SDMs file is stored as nested resources.  Each of
                                    // the N entries is a separate SDM resource.  Each SDM
                                    // resource contains 256 map entries.  You don't LOAD the
                                    // whole SDM resource;  you just OPEN it and SEEK to the
                                    // desired entry.


  SDM_size =                        // size of one SDM resource, in bytes = 3 byte per word *
                3 +                 // header +
                256;                // 256 SDM entries


  offset =
    SecondaryMapIndex*SDM_size +    // skip to desired SDM
    3 +                           // skip over its 3-word header
    charIdx;                      // skip to desired entry


  if (s_bthandle_SDM > 0) {
     // nothing to do

  }
  else
  {
    btHandle = (BYTE)SysOpenResource(g_iSDMS, RS_ROOT_HANDLE);
    s_bthandle_SDM = (BYTE)SysOpenResource(1,btHandle);     // Seek past the RSRC table and 1st RSRC header
    SysCloseResource(btHandle);

  }

  SysResourceFileSeek(s_bthandle_SDM, 0);
  SysResourceFileSeek(s_bthandle_SDM, offset);
  SysResourceFileRead(s_bthandle_SDM, 3, TARGET_MEM_Y, &DBCSMapEntry); // Read the map entry
  return(DBCSMapEntry);
}
#endif //#if defined(STMP_BUILD_PLAYER) &&  defined(FUNCLET)
#endif //3200Merge

#ifdef STMP_BUILD_PLAYER
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextdbcs":
    #pragma endasm
#endif

#ifdef FUNCLET
    #pragma asm
        FDBCSToUnicodeDstXSrcY:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_DBCS_TO_UNICODE_DSTX_SRCY

        org p,"SYSFUNCLET_DBCS_TO_UNICODE_DSTX_SRCY_P":
        dc      RSRC_FUNCLET_DBCS_TO_UNICODE_DSTX_SRCY
    #pragma endasm
    #define DBCSToUnicodeDstXSrcY DBCSToUnicodeDstXSrcYFunclet
#endif
_reentrant void DBCSToUnicodeDstXSrcY(_packed BYTE *pDBCS, WORD _X * _X pUnicode, INT iLength)
{
//#if defined(USBMSC_BUILD) && defined(LIION) && defined(MMC)
#if defined(USBMSC_BUILD)
    unsigned int CopySize = (unsigned int)(&_lc_u_y_dbcs_usbmsc_OVLY_copy_end-&_lc_u_y_dbcs_usbmsc_OVLY_copy_start);
    
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_dbcs_usbmsc_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
  
    DBCSToUnicodeDstXSrcY_wrapper(pDBCS,pUnicode,iLength);

    CopySize = (unsigned int)(&_lc_u_y_Usbmsc_lcd_OVLY_copy_end-&_lc_u_y_Usbmsc_lcd_OVLY_copy_start);
        
    CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_Usbmsc_lcd_OVLY_copy_start, 
                                (_P WORD * _P)&_lc_u_p_usbmsc_lcd_battery_start, CopySize);
    
#else
	INT	iDBCSIndex = 0;
    INT	iUnicodeIndex = 0;
    WORD	c;
    WORD	DBCSMapEntry;

    while (1) {
		c = packed_get(pDBCS, iDBCSIndex++);

        if (c == 0) {
            *pUnicode++ = 0;
            break;
        }
#if 1 //3200Merge
    #ifdef FUNCLET
        DBCSMapEntry = LookupPrimaryDBCSMapEntry_Private2(c);
    #else
        DBCSMapEntry = LookupPrimaryDBCSMapEntry(c);
    #endif
#else //3200Merge
        DBCSMapEntry = LookupPrimaryDBCSMapEntry(c);
#endif //3200Merge
        if (DBCSMapEntry & MBC_FLAG) {
          c = packed_get(pDBCS, iDBCSIndex++);
#if 1 //3200Merge
    #ifdef FUNCLET
          c = LookupSecondaryDBCSMapEntry_Private2(DBCSMapEntry & (unsigned)~MBC_FLAG, c);
    #else
          c = LookupSecondaryDBCSMapEntry(DBCSMapEntry & (unsigned)~MBC_FLAG, c);
    #endif
#else //3200Merge
          c = LookupSecondaryDBCSMapEntry(DBCSMapEntry & (unsigned)~MBC_FLAG, c);
#endif //3200Merge
        } else
          c = DBCSMapEntry;

        *pUnicode++ = c;
        iUnicodeIndex++;
        if(iUnicodeIndex > iLength -2) {
            *pUnicode=0;
            break;
        }
    }
#endif
}

#if 1 //3200Merge
#if !defined(USBMSC_BUILD) &&  defined(FUNCLET)
/////////////////////////////////////////////////////////////////////////////////
//> Name: LookupPrimaryDBCSMapEntry
//
//  Type: Function
//  Description:
//  Prototype:		WORD _reentrant LookupPrimaryDBCSMapEntry(WORD c);
//  Inputs:
//  Outputs:
//  Notes:
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant LookupPrimaryDBCSMapEntry_Private2(WORD c) {
  WORD highByte = c & 0x0000ff;
  WORD DBCSMapEntry;
  int offset;
                                    // The PDM file is stored as a single resource with
                                    // 256 entries, each one data word.  You don't LOAD
                                    // the whole resource;  just OPEN it and then SEEK
                                    // to the deisred entry.

  offset =
    3 +                           // PDM Resource Header
    highByte;                     // 1 words per PDM entry

    if(s_btHandle_PDM > 0)
    {
        // do nothing
    }
    else
    {
        s_btHandle_PDM = (BYTE) SysOpenResource(g_iPDM, RS_ROOT_HANDLE);
    }

    SysResourceFileSeek(s_btHandle_PDM, 0);
    SysResourceFileSeek(s_btHandle_PDM, offset);
    SysResourceFileRead(s_btHandle_PDM, 3, TARGET_MEM_Y, &DBCSMapEntry);

  return(DBCSMapEntry);
}

/////////////////////////////////////////////////////////////////////////////////
//> Name: LookupSecondaryDBCSMapEntry
//
//  Type: Function
//  Description:
//  Prototype:		WORD _reentrant LookupSecondaryDBCSMapEntry(WORD SecondaryMapNumber, WORD c);
//  Inputs:
//  Outputs:
//  Notes:
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant LookupSecondaryDBCSMapEntry_Private2(WORD SecondaryMapIndex, WORD c) {
  WORD DBCSMapEntry;
  WORD charIdx = c & 0x0000ff;
  int SDM_header;
  int SDM_size;
  int offset;
  BYTE btHandle;
                                    // The SDMs file is stored as nested resources.  Each of
                                    // the N entries is a separate SDM resource.  Each SDM
                                    // resource contains 256 map entries.  You don't LOAD the
                                    // whole SDM resource;  you just OPEN it and SEEK to the
                                    // desired entry.


  SDM_size =                        // size of one SDM resource, in bytes = 3 byte per word *
                3 +                 // header +
                256;                // 256 SDM entries


  offset =
    SecondaryMapIndex*SDM_size +    // skip to desired SDM
    3 +                           // skip over its 3-word header
    charIdx;                      // skip to desired entry


  if (s_bthandle_SDM > 0) {
     // nothing to do

  }
  else
  {
    btHandle = (BYTE)SysOpenResource(g_iSDMS, RS_ROOT_HANDLE);
    s_bthandle_SDM = (BYTE)SysOpenResource(1,btHandle);     // Seek past the RSRC table and 1st RSRC header
    SysCloseResource(btHandle);

  }

  SysResourceFileSeek(s_bthandle_SDM, 0);
  SysResourceFileSeek(s_bthandle_SDM, offset);
  SysResourceFileRead(s_bthandle_SDM, 3, TARGET_MEM_Y, &DBCSMapEntry); // Read the map entry
  return(DBCSMapEntry);
}
#endif //#if defined(STMP_BUILD_PLAYER) &&  defined(FUNCLET)
#endif //3200Merge

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextdbcs":
    #pragma endasm
#endif
#endif

//Following should not come out simultaneously with Funclet
//#if defined(USBMSC_BUILD) && defined(LIION) && defined(MMC)
#if defined(USBMSC_BUILD)
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextdbcs_usbmsc_overlay":
    #pragma endasm
#endif

#if 1 //3200Merge
#ifndef FUNCLET
#endif //3200Merge
/////////////////////////////////////////////////////////////////////////////////
//> Name: LookupPrimaryDBCSMapEntry
//
//  Type: Function
//  Description:
//  Prototype:		WORD _reentrant LookupPrimaryDBCSMapEntry(WORD c); 
//  Inputs: 
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant LookupPrimaryDBCSMapEntry(WORD c) {
  WORD highByte = c & 0x0000ff;
  WORD DBCSMapEntry;
  int offset;
                                    // The PDM file is stored as a single resource with
                                    // 256 entries, each one data word.  You don't LOAD
                                    // the whole resource;  just OPEN it and then SEEK
                                    // to the deisred entry.

  offset = 
    3 +                           // PDM Resource Header 
    highByte;                     // 1 words per PDM entry

    if(s_btHandle_PDM > 0)
    {
        // do nothing
    }
    else
    {
        s_btHandle_PDM = (BYTE) SysOpenResource(g_iPDM, RS_ROOT_HANDLE);    
    }

    SysResourceFileSeek(s_btHandle_PDM, 0);
    SysResourceFileSeek(s_btHandle_PDM, offset);
    SysResourceFileRead(s_btHandle_PDM, 3, TARGET_MEM_Y, &DBCSMapEntry);            

  return(DBCSMapEntry);
}

/////////////////////////////////////////////////////////////////////////////////
//> Name: LookupSecondaryDBCSMapEntry
//
//  Type: Function
//  Description:
//  Prototype:		WORD _reentrant LookupSecondaryDBCSMapEntry(WORD SecondaryMapNumber, WORD c); 
//  Inputs: 
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant LookupSecondaryDBCSMapEntry(WORD SecondaryMapIndex, WORD c) {
  WORD DBCSMapEntry;
  WORD charIdx = c & 0x0000ff;
  int SDM_header;
  int SDM_size;
  int offset;
  BYTE btHandle;
                                    // The SDMs file is stored as nested resources.  Each of
                                    // the N entries is a separate SDM resource.  Each SDM
                                    // resource contains 256 map entries.  You don't LOAD the
                                    // whole SDM resource;  you just OPEN it and SEEK to the
                                    // desired entry.


  SDM_size =                        // size of one SDM resource, in bytes = 3 byte per word *
                3 +                 // header +
                256;                // 256 SDM entries


  offset =
    SecondaryMapIndex*SDM_size +    // skip to desired SDM
    3 +                           // skip over its 3-word header
    charIdx;                      // skip to desired entry

    
  if (s_bthandle_SDM > 0) {
     // nothing to do
 
  }
  else 
  {
    btHandle = (BYTE)SysOpenResource(g_iSDMS, RS_ROOT_HANDLE);
    s_bthandle_SDM = (BYTE)SysOpenResource(1,btHandle);     // Seek past the RSRC table and 1st RSRC header
    SysCloseResource(btHandle);
 
  }
 
  SysResourceFileSeek(s_bthandle_SDM, 0);
  SysResourceFileSeek(s_bthandle_SDM, offset);
  SysResourceFileRead(s_bthandle_SDM, 3, TARGET_MEM_Y, &DBCSMapEntry); // Read the map entry
  return(DBCSMapEntry);
}
#if 1 //3200Merge
#endif //#ifndef FUNCLET
#endif

//Following should not come out simultaneously with Funclet
//#if defined(USBMSC_BUILD) && defined(LIION) && defined(MMC)
#if defined(USBMSC_BUILD)
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextdbcs":
    #pragma endasm
#endif

#ifndef STMP_BUILD_PLAYER
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextdbcs":
    #pragma endasm
#endif

//***************************************************************************************************************************
//#if defined(USBMSC_BUILD) && defined(LIION) && defined(MMC)
#if defined(USBMSC_BUILD)
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextdbcs_usbmsc_overlay":
    #pragma endasm

_reentrant void DBCSToUnicode_wrapper(_packed unsigned char *pDBCS, WORD *pUnicode,INT iLength)
{
    int iDBCSIndex=0;
    int iUnicodeIndex=0;
    WORD c;
    WORD DBCSMapEntry;
    while (1) 
    {
        c = packed_get(pDBCS,iDBCSIndex++);

        if (c == 0)
        {
            *pUnicode++ = 0;
            break;
        }
        DBCSMapEntry = LookupPrimaryDBCSMapEntry(c);
        if (DBCSMapEntry & MBC_FLAG) 
        {

          c = packed_get(pDBCS, iDBCSIndex++);
          c = LookupSecondaryDBCSMapEntry(DBCSMapEntry & (unsigned)~MBC_FLAG, c);
        }
        else
        {
          c = DBCSMapEntry;
        }

        *pUnicode++ = c;
        iUnicodeIndex++;
        if(iUnicodeIndex > iLength -2 )
        {
            *pUnicode=0;
            break;
        }
    }
}

_reentrant void DBCSToUnicodeDstXSrcY_wrapper(_packed BYTE *pDBCS, WORD _X * _X pUnicode, INT iLength)
{
	int	iDBCSIndex = 0;
    INT	iUnicodeIndex = 0;
    WORD	c;
    WORD	DBCSMapEntry;

    while (1) {
		c = packed_get(pDBCS, iDBCSIndex++);

        if (c == 0) {
            *pUnicode++ = 0;
            break;
        }
        DBCSMapEntry = LookupPrimaryDBCSMapEntry(c);
        if (DBCSMapEntry & MBC_FLAG) {
          c = packed_get(pDBCS, iDBCSIndex++);
          c = LookupSecondaryDBCSMapEntry(DBCSMapEntry & (unsigned)~MBC_FLAG, c);
        } else
          c = DBCSMapEntry;

        *pUnicode++ = c;
        iUnicodeIndex++;
        if(iUnicodeIndex > iLength -2) {
            *pUnicode=0;
            break;
        }
    }
}
//#if defined(USBMSC_BUILD) && defined(LIION) && defined(MMC)
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextdbcs":
    #pragma endasm
#endif

#endif //STMP_BUILD_PLAYER

//***************************************************************************************************************************

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
