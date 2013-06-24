////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : attachedpictureframe.c
// Description : Album art source file.
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "attachedpictureframe.h"
#include "fstypes.h"
#include "fsapi.h"
#include "metadata.h"
#include "id3v2_internal.h"
#include "dbcs.h"
#include "stringlib.h"
#include "messages.h"
#include "menus.h"
#include "exec.h"
#include "playerlib.h"
#include "resource.h"
#include "menumanager.h"
#include "filesystem.h"

extern _X INT	g_iPlayerState;

_X APIC_DATA	g_ApicFrameData[MAX_APIC_FRAME_COUNT];
_X INT			g_iApicFrameCount = 0;
_X INT			g_iApicCurrentFrame = 0;
_X BOOL			g_bApicFrameFound = FALSE;
_X BYTE			g_btMIMEType = MIME_TYPE_NIL;

#define			MIME_TYPE_JPG_STRING_V2_2	0x47504a
#define			MIME_TYPE_BMP_STRING_V2_2	0x504d42

RETCODE _reentrant ApicReadVariedStringToUnicodeBuffer(INT iHandle, WORD uFrameSize, UCS3 *pString, INT iBufferLength, BYTE btCodingType);
RETCODE _reentrant ApicReadUTF16String(INT iHandle, WORD uEndianNess, WORD uFrameSize, UCS3*pString, INT iBufferLength);
INT _reentrant ApicReadString(BYTE iHandle, UCS3 *pPointer, WORD wBufferLength, INT iObjectSize);

RETCODE _reentrant ApicV2_3and2_4ReadFrame(INT iHandle, WORD uFrameSize, INT *ptr)
{
	APIC_DATA _X * _Y	pApicFrameData;
	BYTE				btCodingType = 0, btPictureType = 0;
	UCS3				uString[MAX_MIME_TYPE_STRING];

	pApicFrameData = &g_ApicFrameData[g_iApicFrameCount];
	g_bApicFrameFound = FALSE;
	if (g_iApicFrameCount < MAX_APIC_FRAME_COUNT) {
		// Read in the encoding type
		Fread(iHandle, (INT *)&btCodingType, 1, Y_MEMORY, (INT)-1);
		// Subtract one off for the encoding type
		uFrameSize--;

		// Read in the MIME type
		uFrameSize = ApicReadVariedStringToUnicodeBuffer(iHandle, uFrameSize, uString, MAX_MIME_TYPE_STRING, btCodingType);
		if (uString[0]=='i' &&
			uString[1]=='m' &&
			uString[2]=='a' &&
			uString[3]=='g' &&
			uString[4]=='e' &&
			uString[5]=='/') {
			if ((uString[6]=='j' && uString[7]=='p' && uString[8]=='g')
				|| (uString[6]=='j' && uString[7]=='p' && uString[8]=='e' && uString[9]=='g')) {
				g_btMIMEType = MIME_TYPE_JPEG;
			} else if (uString[6]=='b' && uString[7]=='m' && uString[8]=='p')
				g_btMIMEType = MIME_TYPE_BMP;
			else
				g_btMIMEType = MIME_TYPE_NIL;
		} else
			g_btMIMEType = MIME_TYPE_NIL;

		// Read in the Picture type
		Fread(iHandle, (INT *)&btPictureType, 1, Y_MEMORY, (INT)-1);
		pApicFrameData->btPictureType = btPictureType;
		// Subtract one off for the picture type
		uFrameSize--;

		// Read in the Description
		uFrameSize = ApicReadVariedStringToUnicodeBuffer(iHandle, uFrameSize, uString, MAX_MIME_TYPE_STRING, btCodingType);

		// Read in the Picture data
		pApicFrameData->dwPicturedataOffset = Ftell(iHandle);
		pApicFrameData->dwPicturedataSize = uFrameSize;

        if (pApicFrameData->dwPicturedataSize != 0)
        {
            if (g_btMIMEType == MIME_TYPE_JPEG || g_btMIMEType == MIME_TYPE_BMP)
            {
			    g_bApicFrameFound = TRUE;
            }
		    g_iApicFrameCount++;
        }
	}

	return 0;
}

RETCODE _reentrant ApicV2_2ReadFrame(INT iHandle, WORD uFrameSize, INT *ptr)
{
	APIC_DATA _X * _Y	pApicFrameData;
	BYTE				btCodingType = 0, btPictureType = 0;
	UCS3				uString[MAX_MIME_TYPE_STRING];
	WORD				wMimeType;

	pApicFrameData = &g_ApicFrameData[g_iApicFrameCount];
	g_bApicFrameFound = FALSE;
	if (g_iApicFrameCount < MAX_APIC_FRAME_COUNT) {
		// Read in the encoding type
		Fread(iHandle, (INT *)&btCodingType, 1, Y_MEMORY, (INT)-1);
		// Subtract one off for the encoding type
		uFrameSize--;

		// Read in the MIME type
		Fread(iHandle, (INT *)&wMimeType, 3, Y_MEMORY, (INT)-1);
		uFrameSize -= 3;
		if (wMimeType == MIME_TYPE_JPG_STRING_V2_2)
			g_btMIMEType = MIME_TYPE_JPEG;
		else if (wMimeType == MIME_TYPE_BMP_STRING_V2_2)
			g_btMIMEType = MIME_TYPE_BMP;
		else
			g_btMIMEType = MIME_TYPE_NIL;

		// Read in the Picture type
		Fread(iHandle, (INT *)&btPictureType, 1, Y_MEMORY, (INT)-1);
		pApicFrameData->btPictureType = btPictureType;
		// Subtract one off for the picture type
		uFrameSize--;

		// Read in the Description
		uFrameSize = ApicReadVariedStringToUnicodeBuffer(iHandle, uFrameSize, uString, MAX_MIME_TYPE_STRING, btCodingType);

		// Read in the Picture data
		pApicFrameData->dwPicturedataOffset = Ftell(iHandle);
		pApicFrameData->dwPicturedataSize = uFrameSize;

        if (pApicFrameData->dwPicturedataSize != 0)
        {
            if (g_btMIMEType == MIME_TYPE_JPEG || g_btMIMEType == MIME_TYPE_BMP)
            {
			    g_bApicFrameFound = TRUE;
            }
		    g_iApicFrameCount++;
        }
	}

	return 0;
}

RETCODE _reentrant ApicReadObject(INT iHandle, INT iObjectSize, INT *ptr)
{
	APIC_DATA _X * _Y	pApicFrameData;
	BYTE				btCodingType = 0, btPictureType = 0;
	UCS3				uString[MAX_MIME_TYPE_STRING];

	pApicFrameData = &g_ApicFrameData[g_iApicFrameCount];
	g_bApicFrameFound = FALSE;
	if (g_iApicFrameCount < MAX_APIC_FRAME_COUNT) {
		// Read in the encoding type
		Fread(iHandle, (INT *)&btCodingType, 1, Y_MEMORY, (INT)-1);
		// Subtract one off byte for the encoding type
		iObjectSize--;

		// Subtract four bytes off for the size
		Fseek(iHandle, 4, SEEK_CUR);
		iObjectSize -= 4;

		// Read in the MIME type
		iObjectSize = ApicReadString(iHandle, uString, MAX_MIME_TYPE_STRING, iObjectSize);
		if (uString[0]=='i' &&
			uString[1]=='m' &&
			uString[2]=='a' &&
			uString[3]=='g' &&
			uString[4]=='e' &&
			uString[5]=='/') {
			if ((uString[6]=='j' && uString[7]=='p' && uString[8]=='g')
				|| (uString[6]=='j' && uString[7]=='p' && uString[8]=='e' && uString[9]=='g')) {
				g_btMIMEType = MIME_TYPE_JPEG;
			} else if (uString[6]=='b' && uString[7]=='m' && uString[8]=='p')
				g_btMIMEType = MIME_TYPE_BMP;
			else
				g_btMIMEType = MIME_TYPE_NIL;
		} else
			g_btMIMEType = MIME_TYPE_NIL;

		// Read in the Description
		iObjectSize = ApicReadString(iHandle, uString, MAX_MIME_TYPE_STRING, iObjectSize);

		// Read in the Picture data
		pApicFrameData->dwPicturedataOffset = Ftell(iHandle);
		pApicFrameData->dwPicturedataSize = iObjectSize;

        if (pApicFrameData->dwPicturedataSize != 0)
        {
            if (g_btMIMEType == MIME_TYPE_JPEG || g_btMIMEType == MIME_TYPE_BMP)
            {
			    g_bApicFrameFound = TRUE;
            }
		    g_iApicFrameCount++;

            fseek(iHandle,iObjectSize,SEEK_CUR);	//seek past remaining data
        }
	}

	return 0;
}

RETCODE _reentrant ApicReadVariedStringToUnicodeBuffer(INT iHandle, WORD uFrameSize, UCS3 *pString, INT iBufferLength, BYTE btCodingType)
{
	INT 			iTemp = 0;
	INT				iCounter = 0;
	INT				iMaxCounter;
	_packed BYTE	TempString[MAX_STRING];

	switch (btCodingType) {
		case ENCODING_ISO_8559:
			TempString[0] = 0;
			iMaxCounter = uFrameSize + 1;
			while (iCounter < iMaxCounter) {
				iCounter++;
				Fread(iHandle, &iTemp, 1, Y_MEMORY, (INT)-1);
				if (iTemp != '\0') {
					if (iCounter <= MAX_STRING)
						packed_strcat(TempString, (_packed BYTE *)&iTemp);
				} else
					break;
			}
			uFrameSize -= iCounter;
			DBCSToUnicode(TempString, pString, iBufferLength);
			break;

		case ENCODING_UTF_16:
			//read in the BOM
			Fread(iHandle, &iTemp, 2, Y_MEMORY, (INT)-1);
			uFrameSize -= 2;
			uFrameSize = ApicReadUTF16String(iHandle, iTemp, uFrameSize, pString, iBufferLength);
			break;

		case ENCODING_UTF_16_NOBOM:
			uFrameSize = ApicReadUTF16String(iHandle, BOM_BIG_ENDIAN, uFrameSize, pString, iBufferLength);
			break;
	}

	return uFrameSize;
}

RETCODE _reentrant ApicReadUTF16String(INT iHandle, WORD uEndianNess, WORD uFrameSize, UCS3*pString, INT iBufferLength)
{
	INT		iTemp1 = 0;
	INT		iTemp2 = 0;
	UCS3	uValue;
	INT		iCounter = 0;
	INT		iMaxCounter;

	iMaxCounter = (uFrameSize + 1) / 2;//The frame size should be bytes, now its words.
	while (iCounter < iMaxCounter) {
		Fread(iHandle, &iTemp1, 1, Y_MEMORY, (INT)-1);
		Fread(iHandle, &iTemp2, 1, Y_MEMORY, (INT)-1);
		if(uEndianNess == BOM_BIG_ENDIAN)
			uValue = (iTemp1<<8) | (iTemp2);
		else
			uValue = (iTemp2<<8) | (iTemp1);
		if (iCounter < iBufferLength)
			pString[iCounter] = uValue;
		iCounter++;
		if (uValue == '\0')
			break;
	}
	uFrameSize -= (iCounter * 2);
	if (iCounter >= iBufferLength)
		iCounter = iBufferLength - 1;
	if (pString)
		pString[iCounter] = '\0';	//null terminate

	return uFrameSize;
}

INT _reentrant ApicReadString(BYTE iHandle, UCS3 *pPointer, WORD wBufferLength, INT iObjectSize)
{
	INT		i;
	UCS3	*pTmpPtr = pPointer;
	UCS3	wData = 0;

	if (pPointer) {
		for (i = 0; ; i++) {
			Fread(iHandle, (INT *)&wData, 2, Y_MEMORY, (INT)-1);

			if (i < wBufferLength) {
				*pTmpPtr++ = wData;
			} else {
				//don't store the word into the string (but we still want to read the whole string out)
			}
			if (!wData)
			{
				i++;
				break;
			}
		}
		pPointer[wBufferLength-1] = 0;  //terminate the string, just in case there's no zero at the end.
		iObjectSize -= (i * 2);
	}

	return iObjectSize;
}

RETCODE _reentrant ApicInit(INT a, INT b, INT *c)
{
	INT i;

	g_iApicFrameCount = 0;
	g_iApicCurrentFrame = 0;
	g_bApicFrameFound = FALSE;
	g_btMIMEType = MIME_TYPE_NIL;
	for (i = 0; i < MAX_APIC_FRAME_COUNT; i++) {
		g_ApicFrameData[i].btPictureType = OTHER;
		g_ApicFrameData[i].dwPicturedataOffset = 0;
		g_ApicFrameData[i].dwPicturedataSize = 0;
	}
	return TRUE;
}

RETCODE _reentrant ApicGetPicturePostion(INT a, INT b, INT *pDataOffset)
{
	DWORD	*pdwPos = (DWORD *)pDataOffset;

	if (g_bApicFrameFound && pdwPos) {
		*pdwPos = g_ApicFrameData[g_iApicCurrentFrame].dwPicturedataOffset;
	}

	return SUCCESS;
}

RETCODE _reentrant ApicPostMessage(INT a, INT b, INT *c)
{
	if (g_bApicFrameFound) {
		SysPostMessage(6, SOFT_TIMER_SET_TIMER, TIMER_APIC_UPDATE, 1, 1, MENU_MSG_APIC_UPDATE);
		SysWaitOnEvent(0, 0, 0);
	}
	return 0;
}
