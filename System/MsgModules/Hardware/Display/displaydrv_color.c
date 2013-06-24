////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : displaydrv_color.c
// Description : LCD display driver
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "project.h"
#include "stringlib.h"
#include "SysResourceApi.h"


#ifdef STMP_BUILD_PLAYER
#include "sysmem.h"
#include "sysequ.h"
#include "fstypes.h"
#include "fsapi.h"
#include "FileSystem.h"				// SigmaTel-specific
#else
#include "kernel.h"
#include "system.h"
#endif

#pragma optimize 1

#include "sal\display_sal_color.h"
#include "displaydriver_color.h"
#include "displaydrv_color.h"
#include "unicode.h"
#include "dbcs.h"

//#define BITMAPCORE_SUPPORT

#define MAX_MASKS 5
#define MAX_DIGITS 10

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm
#endif

#ifdef STMP_BUILD_PLAYER
// Prototypes
_reentrant void BitmapPrintPalette( INT iFile, _BITMAPINFOHEADER* pBitmapInfo, INT x, INT y, INT bInvert, INT iFrameBufferID );
_reentrant void BitmapPrintNonPalette( INT iFile, _BITMAPINFOHEADER* pBitmapInfo, INT x, INT y, INT bInvert, INT iFrameBufferID );
_reentrant void BitmapReadBitMask( INT iFile, _BITMAPINFOHEADER* pBitmapInfo );
_reentrant INT BitmapReadPalette( INT iFile, _BITMAPINFOHEADER* pBitmapInfo, WORD* pPalette, INT iBufferSize );
_reentrant INT BitmapReadHeader( INT iFile, _BITMAPINFOHEADER* pBitmapInfo );
_reentrant INT FixBitmapHeader( DWORD* pBitmapInfo );
#endif //STMP_BUILD_PLAYER

_reentrant BITMAP *DisplayLoadBitmap(WORD iResource);


_reentrant void DisplayUpdateDisplay(void)
{
    SALDisplayUpdate();
}

_reentrant void DisplaySetFont(INT iFont)
{
    LCDSetFont(iFont);
}

_reentrant void DisplayPrintBitmapAddress(INT x,INT y,BITMAP *pBitmap,INT bInvert)
{
    SALDisplayPrintBitmapAddress(x,y,(void *)pBitmap,bInvert,DIRECT_TO_LCD_BUFFER);
}

//3200Merge+
#if defined(FUNCLET) && defined(STMP_BUILD_PLAYER)
    #pragma asm
        FDisplayPrintBitmapResource:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_DISPLAYPRINTBITMAPRESOURCE
        CallFunclet RSRC_FUNCLET_DISPLAYPRINTBITMAPRESOURCE

        org p,"SYSFUNCLET_DisplayPrintBitmapResource_P":
        dc      RSRC_FUNCLET_DISPLAYPRINTBITMAPRESOURCE
    #pragma endasm
    
        //Use the exact same prototype of the static function to convert
_reentrant void DisplayPrintBitmapResource_Funclet(INT x,INT y,WORD iResource  ,INT bInvert, INT iFrameBufferID)
//3200Merge-
#else
_reentrant void DisplayPrintBitmapResource  (INT x,INT y,WORD iResource  ,INT bInvert, INT iFrameBufferID)
#endif //3200Merge
{
	WORD  *pBITMAP = (WORD *) &g_wUserScratchY;			
	INT iSize;
	WORD wRscSize;
    WORD wHandle;
    WORD wChunkSize;
	int iSourceWidth,iSourceHeight,iSourceColorInfo;
	iSize = SCRATCH_USER_Y_SIZE*3;

    // open the bitmap resource
    wHandle = SysOpenResource(iResource, RS_ROOT_HANDLE);
    // we need to draw the bitmap incrementally.
    // read the bitmap information
    SysResourceFileRead( wHandle, 18, BITMAP_MEMORY_SPACE, pBITMAP );
	iSourceColorInfo = pBITMAP[3];
	iSourceWidth = pBITMAP[4];
	iSourceHeight = pBITMAP[5];
    // calculate the number of data to be drawn in the following operation
    if(iSourceColorInfo == MONOCHROME_BITMAP_RESOURCE)
    {
        wChunkSize = ((iSourceWidth + 23)/24); // the number of word in a bitmap row
        //STMP00010151 is solved here
        //pBITMAP[5] = SCRATCH_USER_Y_SIZE/wChunkSize; // the number of row to be drawn in every operation
        pBITMAP[5] = (SCRATCH_USER_Y_SIZE-6)/wChunkSize; // the number of row to be drawn in every operation
    }
    else
    {
#if (defined(COLOR_65K)) || (defined(COLOR_262K))
        wChunkSize = iSourceWidth;
#else
        wChunkSize = (iSourceWidth+1)/2;
#endif
        //STMP00010151 is solved here
        //pBITMAP[5] = SCRATCH_USER_Y_SIZE/wChunkSize; // the number of row to be drawn in every operation
        pBITMAP[5] = (SCRATCH_USER_Y_SIZE-6)/wChunkSize; // the number of row to be drawn in every operation
    }
    if( iSourceHeight < pBITMAP[5] )
        pBITMAP[5] = iSourceHeight;

    wChunkSize = pBITMAP[5]*wChunkSize*3; // the number of bytes in one portion

    // send the bitmap portion to portion until the whole bitmap is sent
    while(iSourceHeight >= pBITMAP[5])
    {
        //read  the bitmap portion for the resource
        SysResourceFileRead(wHandle, wChunkSize, BITMAP_MEMORY_SPACE, pBITMAP+6);
        //draw it
        SALDisplayPrintBitmapAddress(x, y, pBITMAP+3, bInvert, iFrameBufferID);
        //advance the drawing location after this portion
        y += pBITMAP[5];
        iSourceHeight -= pBITMAP[5];
        // if the remaining bitmap is smaller than the predefined portion, resize the drawing portion
        if(iSourceHeight < pBITMAP[5] && iSourceHeight != 0 )
        {
            wChunkSize = wChunkSize/pBITMAP[5]*iSourceHeight;
            pBITMAP[5] = iSourceHeight;
        }
    }
    SysCloseResource(wHandle);
}
//3200Merge+
#ifdef FUNCLET
    #pragma asm
        org p,".ptextdisplaydrv_color":
    #pragma endasm
#endif
//3200Merge-

#ifdef STMP_BUILD_PLAYER
_reentrant DWORD BytesToDWORD( _packed BYTE* pBytes, WORD nOffset, WORD nByte )
{
	DWORD dwReturn;
    dwReturn = packed_get(pBytes, nOffset) | (packed_get(pBytes, nOffset+1) << 8);
	if( nByte == 4 )
		dwReturn = dwReturn | (packed_get(pBytes, nOffset+2) << 16) | (packed_get(pBytes, nOffset+3) << 24);
	return dwReturn;
}
//3200Merge+
//InfoHeader -> |biSize(4)|biWidth(4)            |biHeight(4)              |biPlanes(2)|biBitCount(2)
//CoreHeader -> |bcSize(4)|bcWidth(2)|bcHeigth(2)|bcPlanes(2)|bcBitCount(2)|
//_BITMAP..  -> |biSize(6)|biWidth(6)|biHeight(6)|biPlanes(3)|biBitCount(3)|
//              biCompression(6)|biSizeImage(6)|biXPelsPerMeter(6)|biYPelsPerMeter(6)|
//              biClrUsed(6)|biClrImportant(6)
#ifdef FUNCLET
    #pragma asm
        FFixBitmapHeader:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_FIXBITMAPHEADER
        CallFunclet RSRC_FUNCLET_FIXBITMAPHEADER

        org p,"SYSFUNCLET_FixBitmapHeader_P":
        dc      RSRC_FUNCLET_FIXBITMAPHEADER
    #pragma endasm

        //Use the exact same prototype of the static function to convert
_reentrant INT FixBitmapHeader_Funclet(DWORD* pBitmapInfo)
#else
_reentrant INT FixBitmapHeader( DWORD* pBitmapInfo )
#endif
{
	WORD wOffset[11];	// = { 36, 32, 28, 24, 20, 16, 14, 12, 8, 4, 0 };  Cannot define constant in Funclet
	WORD wBytes[11];	// = {  4,  4,  4,  4,  4, 	4,  2,  2, 4, 4, 4 };
	INT	i;
    DWORD* pBuffer = pBitmapInfo + 10;
    DWORD bSize = BytesToDWORD( (_packed BYTE*)pBitmapInfo, 0, 4 );

    switch( bSize )
    {
        default:
        case 64: // OS2_BMP_HEADER
        case BITMAPINFOHEADER_STRUCT_SIZE:
			{
				for( i = 0; i < 11; i++ )	{
					if (i == 6 || i == 7)
						wBytes[i] = 2;
					else
						wBytes[i] = 4;
				}

				wOffset[0] = 36;
				for( i = 1; i < 11; i++ )	{
					wOffset[i] = wOffset[i-1] - wBytes[i];
				}
				
				for( i = 0; i < 11; i++ )
				{
					*(pBuffer--) = BytesToDWORD( (_packed BYTE*)pBitmapInfo, wOffset[i], wBytes[i] );
				}

				if( bSize == 64 || bSize == BITMAPINFOHEADER_STRUCT_SIZE )
				{
					bSize = bSize - BITMAPINFOHEADER_STRUCT_SIZE;
				}
				else if(pBitmapInfo[0] > BITMAPINFOHEADER_STRUCT_SIZE
						&& pBitmapInfo[6] == pBitmapInfo[2]*\
									((((pBitmapInfo[4]*pBitmapInfo[1])+31)/32)*4)
						&& pBitmapInfo[3] == 1
						&& pBitmapInfo[9] == 0
						)
				{
					bSize = bSize - BITMAPINFOHEADER_STRUCT_SIZE;
				}
				else
				{
					pBitmapInfo[0] = 0;
					bSize = 0;
				}
			}
            break;
#ifdef BITMAPCORE_SUPPORT
        case BITMAPCOREHEADER_STRUCT_SIZE:
			{
				WORD wOffset[] = { 10, 8, 6, 4, 0 };
				WORD wBytes[] = {   2, 2, 2, 2, 4 };
				for( i = 0; i < 5; i++ )
					*(pBuffer--) = 0;

				*(pBuffer++) = BI_RGB;

				for( i = 0; i < 5; i++ )
					*(pBuffer--) = BytesToDWORD( (_packed BYTE*)pBitmapInfo, wOffset[0], wBytes[0] );

				bSize = bSize - BITMAPINFOHEADER_STRUCT_SIZE;
			}
            break;
#endif
	}
	if(pBitmapInfo[9] == 0 && pBitmapInfo[4] <= 8)
	{
	    pBitmapInfo[9] = 1<<pBitmapInfo[4];
	}
	return bSize;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextdisplaydrv_color":
    #pragma endasm
#endif

#ifdef FUNCLET
    #pragma asm
        FBitmapReadHeader:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_BITMAPREADHEADER
        CallFunclet RSRC_FUNCLET_BITMAPREADHEADER

        org p,"SYSFUNCLET_BitmapReadHeader_P":
        dc      RSRC_FUNCLET_BITMAPREADHEADER
    #pragma endasm
    
        //Use the exact same prototype of the static function to convert
_reentrant INT BitmapReadHeader_Funclet(INT iFile, _BITMAPINFOHEADER* pBitmapInfo)
#else
_reentrant INT BitmapReadHeader( INT iFile, _BITMAPINFOHEADER* pBitmapInfo )
#endif
{
	DWORD dwBFOffBits;

	if( iFile < 0 )
		return 1;

    if( Fread(iFile ,(INT*)pBitmapInfo, 2, DEFAULT_MEMORY,(INT)-1) != 2)
        return 1;

    // check if the file header is available
    if( packed_get((_packed BYTE*)pBitmapInfo,0) == 0x42 && packed_get((_packed BYTE*)pBitmapInfo,1) == 0x4D )
    {
        // read the offset
        dwBFOffBits = 0; // we only read 4 bytes
        Fread(iFile, (INT*)&dwBFOffBits, 4, DEFAULT_MEMORY, (INT)-1);
        Fseek(iFile, BITMAPFILEHEADER_STRUCT_SIZE - 6, SEEK_CUR);
    }
    else
    {
		return 1;
    }

    //start to read bitmap info header
    if(Fread(iFile, (INT*)pBitmapInfo, BITMAPINFOHEADER_STRUCT_SIZE, DEFAULT_MEMORY, (INT)-1) != BITMAPINFOHEADER_STRUCT_SIZE)
        return 1;

    //correct different bitmap header
    Fseek(iFile, FixBitmapHeader( (DWORD*)pBitmapInfo ), SEEK_CUR);
    //error occur
    if(pBitmapInfo->biSize == 0)
        return 1;
	else
		return 0;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextdisplaydrv_color":
    #pragma endasm
#endif


#ifdef FUNCLET
    #pragma asm
        FBitmapReadPalette:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_BITMAPREADPALETTE
        CallFunclet RSRC_FUNCLET_BITMAPREADPALETTE

        org p,"SYSFUNCLET_BitmapReadPalette_P":
        dc      RSRC_FUNCLET_BITMAPREADPALETTE
    #pragma endasm
    
        //Use the exact same prototype of the static function to convert
_reentrant INT BitmapReadPalette_Funclet(INT iFile, _BITMAPINFOHEADER* pBitmapInfo, WORD* pPalette, INT iBufferSize)
#else
_reentrant INT BitmapReadPalette( INT iFile, _BITMAPINFOHEADER* pBitmapInfo, WORD* pPalette, INT iBufferSize )
#endif
{
	INT i;
	INT iEntrySize;
	WORD* pColorData;

    if( pBitmapInfo->biClrUsed != 0 && iBufferSize > pBitmapInfo->biClrUsed )
    {
        iEntrySize = 4;
#ifdef BITMAPCORE_SUPPORT
        // for old bitmap, 3 bytes for one color pixel. Otherwise, it is 4 bytes.
		if( pBitmapInfo->biSize == BITMAPCOREHEADER_STRUCT_SIZE )
			iEntrySize = 3;
#endif
		//read the palette table
		Fread( iFile, (INT*)pPalette, iEntrySize*pBitmapInfo->biClrUsed, DEFAULT_MEMORY, (INT)-1 );
#ifdef BITMAPCORE_SUPPORT
		if( iEntrySize == 4 )
		{
#endif
			pColorData = pPalette;
			for( i = 0; i < pBitmapInfo->biClrUsed; i++ )
			{
				*pPalette = BytesToDWORD( (_packed BYTE*)pColorData, i%3, 4 );
				if( (i%3) == 2 )
					pColorData++;
				*pPalette = RGB( (*pPalette & 0xFF0000) >> 16, (*pPalette & 0xFF00) >> 8, *pPalette & 0xFF );
				pPalette++;
				pColorData++;
			}
#ifdef BITMAPCORE_SUPPORT
		}
		else
		{
			//handle 3bytes entry palette table
		}
#endif
		return pBitmapInfo->biClrUsed;
    }
    else
		return 0;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextdisplaydrv_color":
    #pragma endasm
#endif


#ifdef FUNCLET
    #pragma asm
        FBitmapReadBitMask:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_BITMAPREADBITMASK
        CallFunclet RSRC_FUNCLET_BITMAPREADBITMASK

        org p,"SYSFUNCLET_BitmapReadBitMask_P":
        dc      RSRC_FUNCLET_BITMAPREADBITMASK
    #pragma endasm
    
        //Use the exact same prototype of the static function to convert
_reentrant void BitmapReadBitMask_Funclet(INT iFile, _BITMAPINFOHEADER* pBitmapInfo)
#else
_reentrant void BitmapReadBitMask( INT iFile, _BITMAPINFOHEADER* pBitmapInfo )
#endif
{
	INT iColorSize, i;
	WORD* wBuffer = pBitmapInfo->wBitMask;

    if( pBitmapInfo->biBitCount == 16 )
    {
        if( pBitmapInfo->biCompression == BI_BITFIELDS )
        {
            Fread( iFile, (INT*)wBuffer, 12, DEFAULT_MEMORY, (INT)-1 );
            pBitmapInfo->wBitMask[0] = BytesToDWORD((_packed BYTE*)wBuffer, 0, 4);
            pBitmapInfo->wBitMask[1] = BytesToDWORD((_packed BYTE*)wBuffer, 4, 4);
            pBitmapInfo->wBitMask[2] = BytesToDWORD((_packed BYTE*)wBuffer, 8, 4);

            // calculate the bit shift in RGB data
            for( iColorSize = 0; iColorSize < 3; iColorSize++ )
			{
				wBuffer = pBitmapInfo->wBitMask + iColorSize;

                for( i = 0; i < 24; i++ )
                    if( (*wBuffer >> i) & 0x1 )
                    {
                        pBitmapInfo->wBitShift[iColorSize] = i;
                        break;
                    }
			}
        }
        else
        {
            //RGB555
            pBitmapInfo->wBitMask[0] = 0x7C00;
            pBitmapInfo->wBitMask[1] = 0x3E0;
            pBitmapInfo->wBitMask[2] = 0x1F;
            pBitmapInfo->wBitShift[0] = 10;
            pBitmapInfo->wBitShift[1] = 5;
            pBitmapInfo->wBitShift[2] = 0;
        }
    }
    else
    {
        //for 32, 24, 8, 4, 1 bit count case
        pBitmapInfo->wBitMask[0] = 0xFF0000;
        pBitmapInfo->wBitMask[1] = 0xFF00;
        pBitmapInfo->wBitMask[2] = 0xFF;
        pBitmapInfo->wBitShift[0] = 16;
        pBitmapInfo->wBitShift[1] = 8;
        pBitmapInfo->wBitShift[2] = 0;
    }

}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextdisplaydrv_color":
    #pragma endasm
#endif


#ifdef FUNCLET
    #pragma asm
        FBitmapPrintNonPalette:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_BITMAPPRINTNONPALETTE
        CallFunclet RSRC_FUNCLET_BITMAPPRINTNONPALETTE

        org p,"SYSFUNCLET_BitmapPrintNonPalette_P":
        dc      RSRC_FUNCLET_BITMAPPRINTNONPALETTE
    #pragma endasm
    
        //Use the exact same prototype of the static function to convert
_reentrant void BitmapPrintNonPalette_Funclet(INT iFile, _BITMAPINFOHEADER* pBitmapInfo, INT x, INT y, INT bInvert, INT iFrameBufferID)
#else
_reentrant void BitmapPrintNonPalette( INT iFile, _BITMAPINFOHEADER* pBitmapInfo, INT x, INT y, INT bInvert, INT iFrameBufferID )
#endif
{
	INT i;
	LONG diSource;
	INT iSourceDelta;
	WORD* pColorData;
	WORD* p4096Data;
	WORD* pBitData;
	WORD* wBitMask = pBitmapInfo->wBitMask;
	WORD* wBitShift = pBitmapInfo->wBitShift;
	BITMAP* pBITMAP = pBitmapInfo->pBITMAP;
	LONG diHeight = pBitmapInfo->diHeight;
	DWORD dwReadByte = pBitmapInfo->dwWidthByte - pBitmapInfo->dwSkipByte - pBitmapInfo->dwHeadSkipByte;
	INT iColorSize = (pBitmapInfo->biBitCount + 7)/8;

	if (pBitmapInfo->biCompression == BI_BITFIELDS || pBitmapInfo->biCompression == BI_RGB)
	{
		if( pBitmapInfo->biHeight < 0 )
		{
			diSource = 0;
			iSourceDelta = 1;
			diHeight = -diHeight;
		}
		else
		{
			diSource = diHeight - 1;
			iSourceDelta = -1;
		}
	    //draw row by row
	    for( ; (diSource < diHeight) && (diSource >= 0); diSource += iSourceDelta )
	    {
			//locate the color buffer
	        pColorData = pBITMAP->m_wBitmap;
			//read from file
			if( pBitmapInfo->dwHeadSkipByte != 0 )
				Fseek( iFile, pBitmapInfo->dwHeadSkipByte, SEEK_CUR );
            Fread( iFile, (INT*)pColorData, dwReadByte, DEFAULT_MEMORY, (INT)-1 );
			pBitData = pColorData;
			//convert color format
			#if !defined(COLOR_65K)
			p4096Data = pColorData;
			#endif
	        for( i = 0; i < pBITMAP->m_iWidth; i++ )
	        {
				if( iColorSize == 4 ) //32 bits count
				{
					*pColorData = BytesToDWORD( (_packed BYTE*)pBitData, i%3, 4 );
					if( (i%3) == 2 )
						pBitData++;
					pBitData++;
				}
	            *pColorData = RGB( (*pColorData & wBitMask[0]) >> wBitShift[0],
	                               (*pColorData & wBitMask[1]) >> wBitShift[1],
	                               (*pColorData & wBitMask[2]) >> wBitShift[2]
	                             );
				#if !defined(COLOR_65K)
				if( (i%2) == 0 )
					*p4096Data = *pColorData << 12;
				else
				{
					*p4096Data |= *pColorData;
					p4096Data++;
				}
				#endif
	            pColorData++;
	        }
	        Fseek( iFile, pBitmapInfo->dwSkipByte, SEEK_CUR );
	        SALDisplayPrintBitmapAddress(x, y + diSource, (void *)pBITMAP, bInvert, iFrameBufferID);
	    }
	}
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextdisplaydrv_color":
    #pragma endasm
#endif


#ifdef FUNCLET
    #pragma asm
        FBitmapPrintPalette:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_BITMAPPRINTPALETTE
        CallFunclet RSRC_FUNCLET_BITMAPPRINTPALETTE

        org p,"SYSFUNCLET_BitmapPrintPalette_P":
        dc      RSRC_FUNCLET_BITMAPPRINTPALETTE
    #pragma endasm
    
        //Use the exact same prototype of the static function to convert
_reentrant void BitmapPrintPalette_Funclet(INT iFile, _BITMAPINFOHEADER* pBitmapInfo, INT x, INT y, INT bInvert, INT iFrameBufferID)
#else
_reentrant void BitmapPrintPalette( INT iFile, _BITMAPINFOHEADER* pBitmapInfo, INT x, INT y, INT bInvert, INT iFrameBufferID )
#endif
{
	WORD wBuffer;
	WORD wDataMask;
	INT i;
	LONG diSource;
	INT iSourceDelta;
	WORD* pColorData;
	WORD* p4096Data;
	WORD* pBitData;
	WORD* wBitMask = pBitmapInfo->wBitMask;
	WORD* wBitShift = pBitmapInfo->wBitShift;
	BITMAP* pBITMAP = pBitmapInfo->pBITMAP;
	LONG diHeight = pBitmapInfo->diHeight;
	DWORD dwReadByte = pBitmapInfo->dwWidthByte - pBitmapInfo->dwSkipByte - pBitmapInfo->dwHeadSkipByte;
	INT iColorSize = (pBitmapInfo->biBitCount + 7)/8;
	WORD* pPalette = (WORD*)&g_wUserScratchY;

	switch( pBitmapInfo->biBitCount )
	{
		// the bit mask data in the word will be swazzled before conversion
		case 8:
			wDataMask = 0x0000FF;
			break;
		case 4:
		    wDataMask = 0x00000F;
		    break;
	    case 1:
	        wDataMask = 0x000001;
	        break;
		default:
			return;
    }
    //finish the bitmap drawing for bit count = 8, 4, 1
    if( pBitmapInfo->biCompression == BI_RGB )
    {
        WORD wMask;
		WORD wBitCount = pBitmapInfo->biBitCount;
		WORD wBitEntry;

		if( pBitmapInfo->biHeight < 0 )
		{
			diSource = 0;
			iSourceDelta = 1;
			diHeight = -diHeight;
		}
		else
		{
			diSource = diHeight - 1;
			iSourceDelta = -1;
		}
	    //draw row by row
		iColorSize = (dwReadByte + 2)/3*3*(8/(INT)wBitCount)-1;
	    for( ; (diSource < diHeight) && (diSource >= 0); diSource += iSourceDelta )
	    {
			wMask = wDataMask;
			wBitEntry = 0;
			//locate the bit mask data and color buffer
			pBitData = pBITMAP->m_wBitmap + 1;
			pColorData = (WORD*)&g_wUserScratchY + SCRATCH_USER_Y_SIZE - 1;
			
			//read from file
			if( pBitmapInfo->dwHeadSkipByte != 0 )
				Fseek( iFile, pBitmapInfo->dwHeadSkipByte, SEEK_CUR );
			Fread( iFile, (INT*)pBitData, dwReadByte, DEFAULT_MEMORY, (INT)-1 );
			pBitData += (dwReadByte + 2)/3 - 1;

			*pColorData = 0;
			//swazzle the bit mask data for easily mask operation
			*pBitData = (*pBitData >> 16) | (*pBitData & 0xFF00) | (*pBitData << 16);
            for( i = iColorSize; i >= 0; i-- )
            {
				#if !defined(COLOR_65K)
	            wBuffer = pPalette[(*pBitData & wMask) >> wBitEntry];
				if( (i%2) == 0 )
				{
					*pColorData |= wBuffer << 12;
					pColorData--;
				}
				else
				{
					*pColorData = wBuffer;
				}
				#else
	            *pColorData = pPalette[(*pBitData & wMask) >> wBitEntry];
	            pColorData--;
				#endif

				wMask = wMask << wBitCount;
				wBitEntry += wBitCount;

				// once all bits in a WORD are finished, shift down the previous WORD to continue conversion.
				if( wMask == 0 )
				{
					wBitEntry = 0;
					wMask = wDataMask;
					pBitData--;
					//swazzle the bit mask data for easily mask operation
					*pBitData = (*pBitData >> 16) | (*pBitData & 0xFF00) | (*pBitData << 16);
				}
			}
			Fseek( iFile, pBitmapInfo->dwSkipByte, SEEK_CUR );
			*pColorData = pBITMAP->m_iHeight;
			pColorData--;
			*pColorData = pBITMAP->m_iWidth;
			pColorData--;
			*pColorData = pBITMAP->m_ColorType;
			SALDisplayPrintBitmapAddress(x, y + diSource, (void *)pColorData, bInvert, iFrameBufferID);
        }
    }
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextdisplaydrv_color":
    #pragma endasm
#endif

//3200Merge-
#endif //STMP_BUILD_PLAYER
//3200Merge+
_reentrant void DisplayPrintBitmapFile(INT x, INT y, void* pFileName, INT bInvert, INT iFrameBufferID)
{
#ifdef STMP_BUILD_PLAYER
    INT iFile;
	INT iSize;
	INT iColorSize;
	BITMAP* pBITMAP;
    _BITMAPINFOHEADER BitmapInfo;
	WORD wXSize, wYSize;

	if( bInvert & DISPLAY_ROTATION )
	{
		wXSize = LCD_Y_SIZE;
		wYSize = LCD_X_SIZE;
	}
	else
	{
		wXSize = LCD_X_SIZE;
		wYSize = LCD_Y_SIZE;
	}

	pBITMAP = (BITMAP*)&g_wUserScratchY;

    //open the file
    iFile = Fopen( (_packed char*)pFileName, (_packed char *)"r" );

    iSize = SCRATCH_USER_Y_SIZE;

	if( BitmapReadHeader( iFile, &BitmapInfo ) )
	{
		Fclose(iFile);
		return;
	}

	if( bInvert & DISPLAY_VERT_FLIP )
	{
		BitmapInfo.biHeight = -BitmapInfo.biHeight;
	}

    // prepare the palette table
	if( BitmapInfo.biClrUsed != 0 )
	{
		iColorSize = BitmapReadPalette( iFile, &BitmapInfo, (WORD*)pBITMAP, iSize );
		if( iColorSize != 0 )
		{
			pBITMAP = (BITMAP*)((WORD*)pBITMAP + iColorSize);
			iSize -= iColorSize;
		}
	}

	//if x is -ve, calculate the skip byte in the head of every row
	if( x < 0 )
	{
		switch( BitmapInfo.biBitCount )
		{
		case 4:
			x = (x - 3)/4*4;
		case 1:
			x = (x - 7)/8*8;
		}
		BitmapInfo.dwHeadSkipByte = -x * BitmapInfo.biBitCount / 8;
		pBITMAP->m_iWidth = x;
		x = 0;
	}
	else
	{
		BitmapInfo.dwHeadSkipByte = 0;
		pBITMAP->m_iWidth = 0;
	}
    pBITMAP->m_ColorType = COLOR_BITMAP_RESOURCE;
	if( BitmapInfo.biWidth < (wXSize - x) )
		pBITMAP->m_iWidth  += BitmapInfo.biWidth;
	else
		pBITMAP->m_iWidth  += wXSize - x;
    // every time, we draw one row to frame
    pBITMAP->m_iHeight = 1;
    BitmapInfo.diHeight = BitmapInfo.biHeight;

    //number of bytes per color data
    iColorSize = (BitmapInfo.biBitCount+7)/8;
    //number of bytes in a single row
	BitmapInfo.dwWidthByte = (( BitmapInfo.biWidth*BitmapInfo.biBitCount + 31 ) / 32) * 4;
    //number of bytes to skip from one row to another
    BitmapInfo.dwSkipByte  = BitmapInfo.dwWidthByte - (pBITMAP->m_iWidth*BitmapInfo.biBitCount + 7) / 8 - BitmapInfo.dwHeadSkipByte;

    // check if the scratch area is enough to print at least one row
	if( (iSize - 3 - 6/*error margin*/) < (BitmapInfo.dwWidthByte - BitmapInfo.dwSkipByte - BitmapInfo.dwHeadSkipByte) )
	{
		Fclose(iFile);
		return;
	}

    //read the bit mask for 16 color format
	BitmapReadBitMask( iFile, &BitmapInfo );

#if 0
    //locate the color data area
    if( dwBFOffBits != 0L )
        Fseek( iFile, dwBFOffBits, SEEK_SET );
#endif

	// it is top-down bitmap if diHeight < 0
    //trim the bitmap if it is larger than the available display area
    // check the last row location to see if it is outside of screen
    if( (BitmapInfo.diHeight < 0 && (-BitmapInfo.diHeight + y) > (wYSize+1))
        || (BitmapInfo.diHeight > 0 && (BitmapInfo.diHeight + y) > (wYSize+1)) )
    {
        if( BitmapInfo.diHeight >= 0 )
        {
            //locate the first display row
            Fseek( iFile, (BitmapInfo.diHeight + y - wYSize - 1) * BitmapInfo.dwWidthByte, SEEK_CUR );
			// when y is -ve, Fseek will handle this negative value 
			// but the height should be adjusted with y = 0
			if( y < 0 )
				y = 0;
			BitmapInfo.diHeight = (LONG)wYSize + 1 - y;
        }
		else
		{
			//when y is -ve, skip number of row in the file
			if( y < 0 )
			{
				Fseek( iFile, -y * BitmapInfo.dwWidthByte, SEEK_CUR );
				y = 0;
			}
			BitmapInfo.diHeight = (LONG)y - wYSize - 1;
		}
    }
	else if( y < 0 )
	{
		// when y is -ve and the bitmap is top-bottom format, skip number of row in the file
		if( BitmapInfo.diHeight < 0 )
		{
			y = -y;
			if( (bInvert & DISPLAY_VERT_FLIP) == 0  )
			Fseek( iFile, -y * BitmapInfo.dwWidthByte, SEEK_CUR );
		}
		// in both cases (top-bottom & bottom-top), adjust the height with y = 0
		BitmapInfo.diHeight = BitmapInfo.diHeight + y;
		y = 0;
	}

	BitmapInfo.pBITMAP = pBITMAP;
    //interrept the color data according to the bitmap definition.
    switch( BitmapInfo.biBitCount )
    {
		case 32:
		case 24:
		case 16:
			BitmapPrintNonPalette( iFile, &BitmapInfo, x, y, bInvert, iFrameBufferID );
			break;
		case 8:
		case 4:
		case 1:
			BitmapPrintPalette( iFile, &BitmapInfo, x, y, bInvert, iFrameBufferID );
			break;
	}

	Fclose(iFile);
#endif //STMP_BUILD_PLAYER
}
#if defined(FUNCLET) && defined(STMP_BUILD_PLAYER)
    #pragma asm
       ;FDisplayPrintBitmapFile:
        FDisplayPrintBitmapFile_2:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_DISPLAYPRINTBITMAPFILE
        CallFunclet2 RSRC_FUNCLET_DISPLAYPRINTBITMAPFILE

        org p,"SYSFUNCLET_DisplayPrintBitmapFile_P":
        dc      RSRC_FUNCLET_DISPLAYPRINTBITMAPFILE
    #pragma endasm

        //Use the exact same prototype of the static function to convert
_reentrant void DisplayPrintBitmapFile_Funclet(INT x, INT y, void* pFileName, INT bInvert, INT iFrameBufferID)
#else
_reentrant void DisplayPrintBitmapFile_2(INT x, INT y, void* pFileName, INT bInvert, INT iFrameBufferID)
#endif
{
#ifdef STMP_BUILD_PLAYER
    INT iFile;
	INT iSize;
	INT iColorSize;
	BITMAP* pBITMAP;
    _BITMAPINFOHEADER BitmapInfo;
	WORD wXSize, wYSize;

	if( bInvert & DISPLAY_ROTATION )
	{
		wXSize = LCD_Y_SIZE;
		wYSize = LCD_X_SIZE;
	}
	else
	{
		wXSize = LCD_X_SIZE;
		wYSize = LCD_Y_SIZE;
	}

	pBITMAP = (BITMAP*)&g_wUserScratchY;

    //open the file
    iFile = Fopen( (_packed char*)pFileName, (_packed char *)"r" );

    iSize = SCRATCH_USER_Y_SIZE;

	if( BitmapReadHeader( iFile, &BitmapInfo ) )
	{
		Fclose(iFile);
		return;
	}

	if( bInvert & DISPLAY_VERT_FLIP )
	{
		BitmapInfo.biHeight = -BitmapInfo.biHeight;
	}

    // prepare the palette table
	if( BitmapInfo.biClrUsed != 0 )
	{
		iColorSize = BitmapReadPalette( iFile, &BitmapInfo, (WORD*)pBITMAP, iSize );
		if( iColorSize != 0 )
		{
			pBITMAP = (BITMAP*)((WORD*)pBITMAP + iColorSize);
			iSize -= iColorSize;
		}
	}

	//if x is -ve, calculate the skip byte in the head of every row
	if( x < 0 )
	{
		switch( BitmapInfo.biBitCount )
		{
		case 4:
			x = (x - 3)/4*4;
		case 1:
			x = (x - 7)/8*8;
		}
		BitmapInfo.dwHeadSkipByte = -x * BitmapInfo.biBitCount / 8;
		pBITMAP->m_iWidth = x;
		x = 0;
	}
	else
	{
		BitmapInfo.dwHeadSkipByte = 0;
		pBITMAP->m_iWidth = 0;
	}
    pBITMAP->m_ColorType = COLOR_BITMAP_RESOURCE;
	if( BitmapInfo.biWidth < (wXSize - x) )
		pBITMAP->m_iWidth  += BitmapInfo.biWidth;
	else
		pBITMAP->m_iWidth  += wXSize - x;
    // every time, we draw one row to frame
    pBITMAP->m_iHeight = 1;
    BitmapInfo.diHeight = BitmapInfo.biHeight;

    //number of bytes per color data
    iColorSize = (BitmapInfo.biBitCount+7)/8;
    //number of bytes in a single row
	BitmapInfo.dwWidthByte = (( BitmapInfo.biWidth*BitmapInfo.biBitCount + 31 ) / 32) * 4;
    //number of bytes to skip from one row to another
    BitmapInfo.dwSkipByte  = BitmapInfo.dwWidthByte - (pBITMAP->m_iWidth*BitmapInfo.biBitCount + 7) / 8 - BitmapInfo.dwHeadSkipByte;

    // check if the scratch area is enough to print at least one row
	if( (iSize - 3 - 6/*error margin*/) < (BitmapInfo.dwWidthByte - BitmapInfo.dwSkipByte - BitmapInfo.dwHeadSkipByte) )
	{
		Fclose(iFile);
		return;
	}

    //read the bit mask for 16 color format
	BitmapReadBitMask( iFile, &BitmapInfo );

#if 0
    //locate the color data area
    if( dwBFOffBits != 0L )
        Fseek( iFile, dwBFOffBits, SEEK_SET );
#endif

	// it is top-down bitmap if diHeight < 0
    //trim the bitmap if it is larger than the available display area
    // check the last row location to see if it is outside of screen
    if( (BitmapInfo.diHeight < 0 && (-BitmapInfo.diHeight + y) > (wYSize+1))
        || (BitmapInfo.diHeight > 0 && (BitmapInfo.diHeight + y) > (wYSize+1)) )
    {
        if( BitmapInfo.diHeight >= 0 )
        {
            //locate the first display row
            Fseek( iFile, (BitmapInfo.diHeight + y - wYSize - 1) * BitmapInfo.dwWidthByte, SEEK_CUR );
			// when y is -ve, Fseek will handle this negative value 
			// but the height should be adjusted with y = 0
			if( y < 0 )
				y = 0;
			BitmapInfo.diHeight = (LONG)wYSize + 1 - y;
        }
		else
		{
			//when y is -ve, skip number of row in the file
			if( y < 0 )
			{
				Fseek( iFile, -y * BitmapInfo.dwWidthByte, SEEK_CUR );
				y = 0;
			}
			BitmapInfo.diHeight = (LONG)y - wYSize - 1;
		}
    }
	else if( y < 0 )
	{
		// when y is -ve and the bitmap is top-bottom format, skip number of row in the file
		if( BitmapInfo.diHeight < 0 )
		{
			y = -y;
			if( (bInvert & DISPLAY_VERT_FLIP) == 0  )
			Fseek( iFile, -y * BitmapInfo.dwWidthByte, SEEK_CUR );
		}
		// in both cases (top-bottom & bottom-top), adjust the height with y = 0
		BitmapInfo.diHeight = BitmapInfo.diHeight + y;
		y = 0;
	}

	BitmapInfo.pBITMAP = pBITMAP;
    //interrept the color data according to the bitmap definition.
    switch( BitmapInfo.biBitCount )
    {
		case 32:
		case 24:
		case 16:
			BitmapPrintNonPalette( iFile, &BitmapInfo, x, y, bInvert, iFrameBufferID );
			break;
		case 8:
		case 4:
		case 1:
			BitmapPrintPalette( iFile, &BitmapInfo, x, y, bInvert, iFrameBufferID );
			break;
	}

	Fclose(iFile);
#endif //STMP_BUILD_PLAYER
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextdisplaydrv_color":
    #pragma endasm
#endif
//3200Merge-

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


_reentrant void DisplayPrintStringResource  (INT x,INT y,WORD wResource,INT bInvert, INT iFrameBufferID)
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
    DisplayPrintStringAddress(x,y,pResourceString,bInvert, iFrameBufferID);
}
_reentrant void DisplayPrintStringResourceUnicode  (INT x,INT y,WORD wResource,INT bInvert, INT iFrameBufferID)
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
    DisplayPrintStringAddressUnicode(x,y,pResourceString,bInvert, iFrameBufferID);
}
_reentrant void DisplayPrintStringAddress   (INT x,INT y,void *pAddress ,INT bInvert, INT iFrameBufferID)
{
    INT     iLength;
    INT     iSize = SCRATCH_USER_Y_SIZE;
    WORD    *pUnicodeString;

    iLength = packed_strlen((_packed unsigned char *)pAddress)+1;
    pUnicodeString = ((WORD*)&g_wUserScratchY)+iSize-iLength-3;
    //convert the string to unicode.
    DBCSToUnicode((_packed unsigned char *)pAddress,pUnicodeString,iLength);
    //display the string to unicode.
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert, iFrameBufferID);


}

_reentrant int DisplayPrintCharUnicode   (INT  x,INT y,WORD wChar,INT bInvert, INT iFrameBufferID)
{
    COMPLEX_RESOURCE Resource;
    BITMAP *pCharacter;
    WORD wWidth = 0;

    Resource.I = wChar;
    Resource.Complex.m_Type = RESOURCE_TYPE_UNICODE;
    pCharacter = DisplayLoadBitmap(Resource.I);
    if(pCharacter)
    {
        SALDisplayPrintBitmapAddress(x,y,pCharacter,bInvert,iFrameBufferID);
        wWidth = pCharacter->m_iWidth;       
    }
    return wWidth;
}


_reentrant void DisplayPrintStringAddressUnicode   (INT  x,INT y,void *pAddress ,INT bInvert, INT iFrameBufferID)
{
    COMPLEX_RESOURCE Resource;
    WORD *pString=pAddress;
    BITMAP *pCharacter;
    INT iMaskRight;
    bInvert |= DISPLAY_TEXT;	//let the HAL know that we want to use the font color.
#ifdef STMP_BUILD_PLAYER
    if(iFrameBufferID == DIRECT_TO_LCD_BUFFER){
#endif
		iMaskRight = SALDisplayGetWidth();	
#ifdef STMP_BUILD_PLAYER
	}
	else{
		iMaskRight = SALGetBufferWidth(iFrameBufferID);
	}
#endif
    while(*pString)
    {
        x+=DisplayPrintCharUnicode(x,y,*pString++,bInvert, iFrameBufferID);
        if(x > iMaskRight)
            break;//short cut to end print string quicker
    }
}

_reentrant void StringPrintNumber(WORD *pUnicodeString,INT iNumber, INT iDigits, INT iFiller)
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

_reentrant void DisplayPrintNumber(INT  x,INT y,INT iNumber,INT iDigits, INT iFiller, INT bInvert, INT iFrameBufferID)
{
    WORD    pUnicodeString[MAX_DIGITS];
    if(iDigits < MAX_DIGITS)
    {
        StringPrintNumber(pUnicodeString, iNumber, iDigits,iFiller);
        DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert, iFrameBufferID);
    }
}


_reentrant void DisplayPrintTime    (INT x,INT y,INT iMinutes, INT iSeconds, INT bInvert, INT iFrameBufferID)
{
    WORD pUnicodeString[2+1+2+1+2];//2 for minutes, 1 for colon, 2 for seconds, 1 for terminator, and 2 for padding
    StringPrintNumber(pUnicodeString+0, iMinutes, 2,'0');
    pUnicodeString[2]=':';
    StringPrintNumber(pUnicodeString+3, iSeconds, 2,'0');
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert, iFrameBufferID);
}
_reentrant void DisplayPrintLongTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert, INT iFrameBufferID)
{
    WORD pUnicodeString[2+1+2+1+2+1+2];//2 for minutes, 1 for colon, 2 for seconds, 1 for terminator, and 2 for padding
    StringPrintNumber(pUnicodeString+0, iHours  , 2,' ');
    pUnicodeString[2]=':';
    StringPrintNumber(pUnicodeString+3, iMinutes, 2,'0');
    pUnicodeString[5]=':';
    StringPrintNumber(pUnicodeString+6, iSeconds, 2,'0');
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert, iFrameBufferID);
}

_reentrant void DisplayHandleMessage(Message *pMsg)
{
    SALHandleMessage(pMsg);
}

