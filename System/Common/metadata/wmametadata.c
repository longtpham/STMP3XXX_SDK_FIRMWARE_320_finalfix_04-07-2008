///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: mp3metadata.c
// Description: Functions for extracting the mp3 meta-data.
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "resource.h"
#include "metadata.h"
#include "wmametadata.h"
#if (defined JPEG_ALBUM_ART) || (defined SYNC_LYRICS)
#include "menumanager.h"
#ifdef SYNC_LYRICS
#include "SyncLyrics.h"
extern LONG _Y Mp3DecoderStartFrameFilePosition;
#endif
#ifdef JPEG_ALBUM_ART
#include "attachedpictureframe.h"
#endif
#endif
#ifdef USE_PLAYLIST3
#include "fsapi.h"
#include "fstypes.h"
#include "playlist3.h"
#include "playerlib.h"

extern _reentrant INT FastOpen(LONG Key,_packed char *mode);
#endif
#ifdef USE_PLAYLIST5
#include "fsapi.h"
#include "playlist5.h"
#include "playerlib.h"
#endif
#pragma optimize 1

//This value is the current known header object header size
//(this must be seeked past to get to the objects in the header)
#ifdef USE_PLAYLIST3
// added start, SL
// Constants for iExtendedContent, to indicate which part of extended content description object is need to update
#define	EXTENDED_CONTENT_BIT_ALBUM	0
#define EXTENDED_CONTENT_BIT_GENRE	1
#define EXTENDED_CONTENT_BIT_YEAR	2
#ifdef JPEG_ALBUM_ART
#define EXTENDED_CONTENT_BIT_ALBUMART	3
#endif
#ifdef SYNC_LYRICS
#define EXTENDED_CONTENT_BIT_LYRICS	4
#endif
#define EXTENDED_CONTENT_BIT_TRACKNUM       5
// added end
#endif

typedef union
{
    WORD wValues[6];            //there is some wasted space here  (6 words is 18 bytes, not 16)
    _packed BYTE cValues[16];
} GUID;

typedef struct
{
    GUID ObjectGUID;
    DWORD ObjectSize;         //more wasted space (3 words is 9 bytes, not 8)
} ASFOBJECTHEADER;

#ifdef JPEG_ALBUM_ART
BOOL bPicOnlyFlag=FALSE;
#endif

BYTE _reentrant  GetWmaMetaData_ReadBYTE(BYTE iHandle)
{
    WORD TmpVal;
    TmpVal=0;//zero out

    FSFileRead(1,iHandle,MEM_SPACE_Y,-1,&TmpVal);//read in 1 byte
    return (BYTE)TmpVal;

}


WORD _reentrant  GetWmaMetaData_ReadWORD(BYTE iHandle)
{
    WORD TmpVal;
    TmpVal=0;//zero out

    FSFileRead(2,iHandle,MEM_SPACE_Y,-1,&TmpVal);//read in 2 bytes
    return (WORD)TmpVal;

}

DWORD _reentrant  GetWmaMetaData_ReadDWORD(BYTE iHandle)
{
    WORD TmpVal[2];
    TmpVal[1]=0;//zero out the upper word.

    FSFileRead(4,iHandle,MEM_SPACE_Y,-1,TmpVal);//read in 4 bytes
    return ((DWORD)TmpVal[0] + ((DWORD)TmpVal[1]<<24));//they just so happen to be in that order

}

DWORD _reentrant  GetWmaMetaData_ReadQWORD(BYTE iHandle)
{//this reads in a QWORD from the ASF file, but truncates to a DWORD
    WORD TmpVal[3];
    TmpVal[2]=0;

    FSFileRead(8,iHandle,MEM_SPACE_Y,-1,TmpVal);//read in 8 bytes
    return ((DWORD)TmpVal[0] + ((DWORD)TmpVal[1]<<24));//we only use the bottom two words (6 bytes).

}



RETCODE _reentrant GetWmaMetaData_ReadObjectHeaderPrologue(BYTE iHandle, ASFOBJECTHEADER*pObjectHeader)
{
    int iError;
    //this zero's out the non even sized pieces
    pObjectHeader->ObjectGUID.wValues[5] = 0;
    iError = FSFileRead(16,iHandle,MEM_SPACE_Y,-1,(void*)pObjectHeader);
    pObjectHeader->ObjectSize = GetWmaMetaData_ReadQWORD(iHandle);
    return iError;
}


RETCODE _reentrant GetWmaMetaData_CompareGUID(GUID *pLeft,GUID *pRight)
{
    int i;
    register int *pLeftGUID=(int*)pLeft;
    register int *pRightGUID=(int*)pRight;
    RETCODE returnValue = FALSE;
    for(i=0;i<6;i++)
    {
        if(*pLeftGUID++ != *pRightGUID++)
        {
            returnValue = FALSE;
            break;
        }
        returnValue = TRUE;
    }
    return returnValue;
}

DWORD _reentrant GetWmaMetaData_FindNextObject(BYTE iHandle, int iMaxObjects,GUID *pGUID)
//Will return -1 if no object found,  or the size of the object.  (We shouldn't have any objects that are
//0xffffffffffff in size (thats 281 trillion bytes)
{
    int i;
    ASFOBJECTHEADER ObjectHeader;
    WORD returnValue=0;
    for(i=0;i<iMaxObjects;i++)
    {
        //read the GUID in
        GetWmaMetaData_ReadObjectHeaderPrologue(iHandle,&ObjectHeader);
        if(GetWmaMetaData_CompareGUID(&ObjectHeader.ObjectGUID,pGUID))
        {
            returnValue=ObjectHeader.ObjectSize;
            break;//they match, lets stop seeking.
        }
        else
        {
            fseek(iHandle,ObjectHeader.ObjectSize-24,SEEK_CUR);//skip to the next object.
        }
    }
    return returnValue;
}

#if (defined JPEG_ALBUM_ART) || (defined SYNC_LYRICS)
DWORD _reentrant GetWmaMetaData_FindNextObjectBySize(BYTE iHandle, DWORD dwObjectSize, GUID *pGUID)
//Will return 0 if no object found,  or the size of the object.
{
    INT				i;
    ASFOBJECTHEADER	ObjectHeader;
    DWORD			returnValue = 0, dwNumBytesToSeek;
    LONG			remain = (LONG)dwObjectSize;

	while (remain > 0) {
        //read the GUID in
        GetWmaMetaData_ReadObjectHeaderPrologue(iHandle, &ObjectHeader);
        if (GetWmaMetaData_CompareGUID(&ObjectHeader.ObjectGUID, pGUID)) {
            returnValue = ObjectHeader.ObjectSize;
            break;	//they match, lets stop seeking.
        } else {
        	dwNumBytesToSeek = ObjectHeader.ObjectSize - 24;
			fseek(iHandle, (LONG)dwNumBytesToSeek, SEEK_CUR);	//skip to the next object.
			remain -= ObjectHeader.ObjectSize;
        }
    }

    return returnValue;
}
#endif	// JPEG_ALBUM_ART || SYNC_LYRICS


DWORD _reentrant GetWmaMetaData_FindFirstObject(BYTE iHandle, int iMaxObjects,GUID *pGUID)
{
    fseek(iHandle,WMA_HEADER_OBJECT_HEADER_SIZE,SEEK_SET);

    return GetWmaMetaData_FindNextObject(iHandle,iMaxObjects,pGUID);
}


void  _reentrant GetWmaMetaData_ReadString(BYTE iHandle, UCS3 *pPointer, WORD wBufferLength,WORD wStoredLength)
{
    int i;
    UCS3 *pTmpPtr = pPointer;
    UCS3 wData;
    if(pPointer)
    {
        for(i=0;i<wStoredLength;i++)
        {
            wData = GetWmaMetaData_ReadWORD(iHandle);
            if(i<wBufferLength)
            {
                *pTmpPtr++ = wData;
            }
            else
            {
                //don't store the word into the string (but we still want to read the whole string out)
            }
        }
        pPointer[wBufferLength-1] = 0;  //terminate the string, just in case there's no zero at the end.
    }

}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetWmaMetaData
//
//   Type:          Function
//
//   Description:   Finds the meta-data within a wma file.
//
//   Inputs:        Ptr to pathname, current device id, ptr to meta-data struct.
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

RETCODE _reentrant GetWmaMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
    int     bContinue = FALSE;
    int     iError;
    RETCODE rtn = META_DATA_NOT_FOUND;
    ASFOBJECTHEADER HeaderHeader;
	GUID    TempGUID;
	GUID	TempGUID2;
    WORD    wObjectCount=0;
    int     iHandle;
    _packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;

    WORD TmpVal[3];
#ifdef USE_PLAYLIST3
    UCS3	*pPointer;			// SL
    WORD wBufferLength;		// SL
	INT iExtendedContent = 0;	// SL

	WORD pTracknum[TRACKNUM_SIZE];
    WORD TracknumSize;
    WORD* pTmp;

#endif
    // Open the file.
#ifdef USE_PLAYLIST3
    if (g_Rebuild == TRUE)
    	iHandle = FSFastOpen(g_FileKey,READ);
    else
#endif
#ifdef USE_PLAYLIST3
	   //	iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);
		iHandle = FSFastOpen(g_CurrentSongFastkey,READ);
#endif
#ifdef USE_PLAYLIST2
   iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);
#endif
#ifdef USE_PLAYLIST5
	g_CurrentFastKey = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->file.lFastKey;
	iHandle = FSFastOpen(g_CurrentFastKey,READ);
#endif
	if(iHandle != ERROR)
	{
#ifdef USE_PLAYLIST3
	    if (g_Rebuild == TRUE)
		{
			filegetdate(iHandle,CREATION_DATE,&g_dirdate,&g_dirtime);
		}
#endif
#ifdef SYNC_LYRICS
#ifdef USE_PLAYLIST3
		if (!g_Rebuild)
#endif
		{
			LYRICS_FILE_META_DATA	LyricFileMetaData;

			LyricFileMetaData.wTitle = MetaData->wTitle;
			LyricFileMetaData.iTitleBufferLength = MetaData->iTitleBufferLength;
			LyricFileMetaData.wArtist = MetaData->wArtist;
			LyricFileMetaData.iArtistBufferLength = MetaData->iArtistBufferLength;
			LyricFileMetaData.wAlbum = MetaData->wAlbum;
			LyricFileMetaData.iAlbumBufferLength = MetaData->iAlbumBufferLength;
			LyricFileMetaData.dwNumBytes = MetaData->dwNumBytes;

		if (MetaData->wTitle)
			*MetaData->wTitle = 0;
		if (MetaData->wArtist)
			*MetaData->wArtist = 0;
		if  (MetaData->wAlbum)
			*MetaData->wAlbum = 0;

			SysCallFunction(RSRC_LRC_CODEBANK, Mp3MetaDataReadLRC, 0, 0, (INT *)&LyricFileMetaData);

		// Throw the LRC metadata away
		if (MetaData->wTitle)
			*MetaData->wTitle = 0;
		if (MetaData->wArtist)
			*MetaData->wArtist = 0;
		if  (MetaData->wAlbum)
			*MetaData->wAlbum = 0;
		}
#endif	// SYNC_LYRICS
        MetaData->iFileType = (INT)WMA_TYPE;
        MetaData->iResourceNum = RSRC_WMADEC_CODE;
        MetaData->bVBR = FALSE;
        iError = fseek(iHandle,0,SEEK_SET);
        //verify that that header object is there.
        if(iError != -1)
        {
            TempGUID.wValues[0]=ASF_HEADER_GUID_0;
            TempGUID.wValues[1]=ASF_HEADER_GUID_1;
            TempGUID.wValues[2]=ASF_HEADER_GUID_2;
            TempGUID.wValues[3]=ASF_HEADER_GUID_3;
            TempGUID.wValues[4]=ASF_HEADER_GUID_4;
            TempGUID.wValues[5]=ASF_HEADER_GUID_5;

            GetWmaMetaData_ReadObjectHeaderPrologue(iHandle,&HeaderHeader);
            if(GetWmaMetaData_CompareGUID(&HeaderHeader.ObjectGUID,&TempGUID))
            {
                wObjectCount = GetWmaMetaData_ReadDWORD(iHandle);
                GetWmaMetaData_ReadBYTE(iHandle);
                if(GetWmaMetaData_ReadBYTE(iHandle)==0x02)
                {
                    bContinue = TRUE;
					rtn = META_DATA_FOUND;
#ifdef SYNC_LYRICS
					Mp3DecoderStartFrameFilePosition = HeaderHeader.ObjectSize;
#endif	// SYNC_LYRICS
                }
            }
        }
        else
          rtn = META_DATA_FILESEEK_ERROR;
        if(bContinue)
        {
//			int wSize;
            //we've got a header object, so lets start getting objects out of it
            //first, the
            TempGUID.wValues[0]=ASF_FILE_PROPERTIES_GUID_0;
            TempGUID.wValues[1]=ASF_FILE_PROPERTIES_GUID_1;
            TempGUID.wValues[2]=ASF_FILE_PROPERTIES_GUID_2;
            TempGUID.wValues[3]=ASF_FILE_PROPERTIES_GUID_3;
            TempGUID.wValues[4]=ASF_FILE_PROPERTIES_GUID_4;
            TempGUID.wValues[5]=ASF_FILE_PROPERTIES_GUID_5;
            if(GetWmaMetaData_FindFirstObject(iHandle,wObjectCount,&TempGUID)>0)
            {
              // The following code should work properly, but doesn't because the Windows Media
              // Player messes up the File Size parameter (adds a 00100 to it).  Thus, we'll use
              // our own file size function.
              //  fseek(iHandle,16,SEEK_CUR);//skip the FILE ID
              //  MetaData->dwNumBytes = GetWmaMetaData_ReadQWORD(iHandle);
                MetaData->dwNumBytes = FSFileSize(iHandle);
                MetaData->dwDataBytes = (MetaData->dwNumBytes - HeaderHeader.ObjectSize);
                // Skip the File ID, the File Size, and the creation date. (16 + 8 + 8)
                fseek(iHandle,32,SEEK_CUR);

                MetaData->dwNumFrames= GetWmaMetaData_ReadQWORD(iHandle);
                MetaData->iTime      = GetWmaMetaData_ReadQWORD(iHandle)/10000;
                fseek(iHandle,8,SEEK_CUR);//skip the send duration
                MetaData->iTime     -= GetWmaMetaData_ReadQWORD(iHandle);//subtract off the preroll amount
                MetaData->iTime     /= 1000; //convert the time to seconds

                TempGUID.wValues[0]=ASF_STREAM_PROPERTIES_GUID_0;
                TempGUID.wValues[1]=ASF_STREAM_PROPERTIES_GUID_1;
                TempGUID.wValues[2]=ASF_STREAM_PROPERTIES_GUID_2;
                TempGUID.wValues[3]=ASF_STREAM_PROPERTIES_GUID_3;
                TempGUID.wValues[4]=ASF_STREAM_PROPERTIES_GUID_4;
                TempGUID.wValues[5]=ASF_STREAM_PROPERTIES_GUID_5;
            }
            else
            {
                bContinue = FALSE;
            }
        }
            //get information from the STREAM object
        if(bContinue)
        {
            int wSize;
            wSize = GetWmaMetaData_FindFirstObject(iHandle,wObjectCount,&TempGUID);
            while(wSize > 0)
            {

//				GUID TempGUID2;
                TempGUID2.wValues[0] = ASF_AUDIO_MEDIA_GUID_0;
                TempGUID2.wValues[1] = ASF_AUDIO_MEDIA_GUID_1;
                TempGUID2.wValues[2] = ASF_AUDIO_MEDIA_GUID_2;
                TempGUID2.wValues[3] = ASF_AUDIO_MEDIA_GUID_3;
                TempGUID2.wValues[4] = ASF_AUDIO_MEDIA_GUID_4;
                TempGUID2.wValues[5] = ASF_AUDIO_MEDIA_GUID_5;

                FSFileRead(16,iHandle,MEM_SPACE_Y,-1,(void*)&TempGUID);
                if(GetWmaMetaData_CompareGUID(&TempGUID,&TempGUID2))
                {
                    break;
                }
                //seek to the next object
                fseek(iHandle,wSize-16,SEEK_CUR);
                wSize = GetWmaMetaData_FindNextObject(iHandle,wObjectCount,&TempGUID);
            }
            if(wSize > 0)
            {//wSize will be positive when we've found our stream
                iError = fseek(iHandle,16+8+4+4+2+4+2+2,SEEK_CUR);//skip past error correction type, time offset, type specific data length(since we know what
                                                        //it is),error correction data length, flags, reserved
                                                        //plus parts of the codec specific data (codec,channels)
                MetaData->wSampRate= GetWmaMetaData_ReadDWORD(iHandle);
                MetaData->wBitRate = GetWmaMetaData_ReadDWORD(iHandle)*8;
            }
            else
            {
                bContinue = FALSE;
            }
        }
            //now get the song information, etc.
        if(bContinue)
        {
			int i;
			int wContentDescriptorCount;
			UCS3 sDescriptorName[24];
			int wDescriptorNameLength;

            TempGUID.wValues[0]=ASF_CONTENT_DESCRIPTION_GUID_0;
            TempGUID.wValues[1]=ASF_CONTENT_DESCRIPTION_GUID_1;
            TempGUID.wValues[2]=ASF_CONTENT_DESCRIPTION_GUID_2;
            TempGUID.wValues[3]=ASF_CONTENT_DESCRIPTION_GUID_3;
            TempGUID.wValues[4]=ASF_CONTENT_DESCRIPTION_GUID_4;
            TempGUID.wValues[5]=ASF_CONTENT_DESCRIPTION_GUID_5;
            if(GetWmaMetaData_FindFirstObject(iHandle,wObjectCount,&TempGUID)>0)
            {
                int wTitleLength = GetWmaMetaData_ReadWORD(iHandle)/2;
                int wArtistLength = GetWmaMetaData_ReadWORD(iHandle)/2;

                fseek(iHandle,2+2+2,SEEK_CUR);//seek passed Copyright Length, Description Length, Rating Length

                GetWmaMetaData_ReadString(iHandle, MetaData->wTitle,MetaData->iTitleBufferLength,wTitleLength);
                GetWmaMetaData_ReadString(iHandle, MetaData->wArtist,MetaData->iArtistBufferLength,wArtistLength);
#ifdef USE_PLAYLIST3
				// added start, SL
				// Clear the bit field
				iExtendedContent = 0;
				if (MetaData->wAlbum)
					iExtendedContent |= (1 << EXTENDED_CONTENT_BIT_ALBUM);
				if (MetaData->wGenre)
					iExtendedContent |= (1 << EXTENDED_CONTENT_BIT_GENRE);
				if (MetaData->wYear)
					iExtendedContent |= (1 << EXTENDED_CONTENT_BIT_YEAR);

				iExtendedContent |= (1 << EXTENDED_CONTENT_BIT_TRACKNUM);

#ifdef JPEG_ALBUM_ART
				if (!g_Rebuild)
					iExtendedContent |= (1 << EXTENDED_CONTENT_BIT_ALBUMART);
#endif
#ifdef SYNC_LYRICS
				if (!g_Rebuild)
					iExtendedContent |= (1 << EXTENDED_CONTENT_BIT_LYRICS);
#endif
				// If the corresponding bit is set, i.e. the corresponding is need to update
				pPointer = NULL;
				wBufferLength = 0;
				// added end, SL
#endif
                //Find the album information
#ifndef USE_PLAYLIST3
                if(MetaData->wAlbum)
#else
#ifdef JPEG_ALBUM_ART
				if(bPicOnlyFlag == TRUE)
				{
					TempGUID.wValues[0]=ASF_EXTENDED_CONTENT_GUID_0;
					TempGUID.wValues[1]=ASF_EXTENDED_CONTENT_GUID_1;
					TempGUID.wValues[2]=ASF_EXTENDED_CONTENT_GUID_2;
					TempGUID.wValues[3]=ASF_EXTENDED_CONTENT_GUID_3;
					TempGUID.wValues[4]=ASF_EXTENDED_CONTENT_GUID_4;
					TempGUID.wValues[5]=ASF_EXTENDED_CONTENT_GUID_5;
					if(GetWmaMetaData_FindFirstObject(iHandle,wObjectCount,&TempGUID)>0)
					{
//						int i;
//						int wContentDescriptorCount ;
//						UCS3 sDescriptorName[15];
						wContentDescriptorCount = GetWmaMetaData_ReadWORD(iHandle);
						for(i=0;i<wContentDescriptorCount;i++)
						{
							int wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle)/2;
							GetWmaMetaData_ReadString(iHandle, sDescriptorName, 14,wDescriptorNameLength);
							// added start, SL
							if (sDescriptorName[ 0]=='W' &&
								sDescriptorName[ 1]=='M' &&
								sDescriptorName[ 2]=='/' &&
								sDescriptorName[ 3]=='P' &&
								sDescriptorName[ 4]=='i' &&
								sDescriptorName[ 5]=='c' &&
								sDescriptorName[ 6]=='t' &&
								sDescriptorName[ 7]=='u' &&
								sDescriptorName[ 8]=='r' &&
								sDescriptorName[ 9]=='e')
							{
								fseek(iHandle, 2, SEEK_CUR);//seek past the data type
								wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle);
								// Extract WM Picture
								SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicReadObject, iHandle, wDescriptorNameLength, NULL);
								break;
							} else {
								// Skip over this extended content descriptor
								fseek(iHandle,2,SEEK_CUR);	//seek past the data type
								// get the descriptor value length, and skip past it
								fseek(iHandle, GetWmaMetaData_ReadWORD(iHandle), SEEK_CUR);
							}
						}
					}
				}else if(iExtendedContent)
#endif
                if(iExtendedContent)	// SL
#endif
                {
                    TempGUID.wValues[0]=ASF_EXTENDED_CONTENT_GUID_0;
                    TempGUID.wValues[1]=ASF_EXTENDED_CONTENT_GUID_1;
                    TempGUID.wValues[2]=ASF_EXTENDED_CONTENT_GUID_2;
                    TempGUID.wValues[3]=ASF_EXTENDED_CONTENT_GUID_3;
                    TempGUID.wValues[4]=ASF_EXTENDED_CONTENT_GUID_4;
                    TempGUID.wValues[5]=ASF_EXTENDED_CONTENT_GUID_5;
                    if(GetWmaMetaData_FindFirstObject(iHandle,wObjectCount,&TempGUID)>0)
                    {
//						int i;
//						int wContentDescriptorCount ;
//						UCS3 sDescriptorName[15];
                        wContentDescriptorCount = GetWmaMetaData_ReadWORD(iHandle);
                        for(i=0;i<wContentDescriptorCount;i++)
                        {
                            wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle)/2;
                            GetWmaMetaData_ReadString(iHandle, sDescriptorName, 23,wDescriptorNameLength);
#ifndef USE_PLAYLIST3
                            if(sDescriptorName[ 0]=='W' &&
                               sDescriptorName[ 1]=='M' &&
                               sDescriptorName[ 2]=='/' &&
                               sDescriptorName[ 3]=='A' &&
                               sDescriptorName[ 4]=='l' &&
                               sDescriptorName[ 5]=='b' &&
                               sDescriptorName[ 6]=='u' &&
                               sDescriptorName[ 7]=='m' &&
                               sDescriptorName[ 8]=='T' &&
                               sDescriptorName[ 9]=='i' &&
                               sDescriptorName[10]=='t' &&
                               sDescriptorName[11]=='l' &&
                               sDescriptorName[12]=='e')
                            {
                                if(GetWmaMetaData_ReadWORD(iHandle)==0)//must be a unicode string
                                {
                                    wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle)/2;
                                    GetWmaMetaData_ReadString(iHandle, MetaData->wAlbum,MetaData->iAlbumBufferLength,wDescriptorNameLength);
                                }
#if !(defined JPEG_ALBUM_ART) && !(defined SYNC_LYRICS)
                                break;
#endif	// !(defined JPEG_ALBUM_ART) && !(defined SYNC_LYRICS)
                            }
#ifdef JPEG_ALBUM_ART
                            else if(sDescriptorName[ 0]=='W' &&
									sDescriptorName[ 1]=='M' &&
									sDescriptorName[ 2]=='/' &&
									sDescriptorName[ 3]=='P' &&
									sDescriptorName[ 4]=='i' &&
									sDescriptorName[ 5]=='c' &&
									sDescriptorName[ 6]=='t' &&
									sDescriptorName[ 7]=='u' &&
									sDescriptorName[ 8]=='r' &&
									sDescriptorName[ 9]=='e')
							{
	                                fseek(iHandle, 2, SEEK_CUR);//seek past the data type
                                    wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle);
								// Extract WM Picture
				            		SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicReadObject, iHandle, wDescriptorNameLength, NULL);
                            }
#endif	// JPEG_ALBUM_ART
#ifdef SYNC_LYRICS
                            else if(sDescriptorName[ 0]=='W' &&
									sDescriptorName[ 1]=='M' &&
									sDescriptorName[ 2]=='/' &&
									sDescriptorName[ 3]=='L' &&
									sDescriptorName[ 4]=='y' &&
									sDescriptorName[ 5]=='r' &&
									sDescriptorName[ 6]=='i' &&
									sDescriptorName[ 7]=='c' &&
									sDescriptorName[ 8]=='s' &&
									sDescriptorName[ 9]=='_' &&
									sDescriptorName[10]=='S' &&
									sDescriptorName[11]=='y' &&
									sDescriptorName[12]=='n' &&
									sDescriptorName[13]=='c' &&
									sDescriptorName[14]=='h' &&
									sDescriptorName[15]=='r' &&
									sDescriptorName[16]=='o' &&
									sDescriptorName[17]=='n' &&
									sDescriptorName[18]=='i' &&
									sDescriptorName[19]=='s' &&
									sDescriptorName[20]=='e' &&
									sDescriptorName[21]=='d')
							{
	                                fseek(iHandle, 2, SEEK_CUR);//seek past the data type
                                    wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle);	// Descriptor Value Length
									// Extract WMA Lyrics
									SysCallFunction(RSRC_WMA_LYRICS_CODEBANK, WMALyricsReadFrame, iHandle, wDescriptorNameLength, NULL);
                            }
#endif	// SYNC_LYRICS
#else // else USE_PLAYLIST3
                            // added start, SL
							if (sDescriptorName[ 0]=='W' &&
								sDescriptorName[ 1]=='M' &&
								sDescriptorName[ 2]=='/') {
								if (sDescriptorName[ 3]=='A' &&
									sDescriptorName[ 4]=='l' &&
									sDescriptorName[ 5]=='b' &&
									sDescriptorName[ 6]=='u' &&
									sDescriptorName[ 7]=='m' &&
									sDescriptorName[ 8]=='T' &&
									sDescriptorName[ 9]=='i' &&
									sDescriptorName[10]=='t' &&
									sDescriptorName[11]=='l' &&
									sDescriptorName[12]=='e') {
									// The title of the album
									if (iExtendedContent & (1 << EXTENDED_CONTENT_BIT_ALBUM)) {
										pPointer = MetaData->wAlbum;
										wBufferLength = MetaData->iAlbumBufferLength;
										iExtendedContent &= ~(1 << EXTENDED_CONTENT_BIT_ALBUM);
										}  else {
											// Skip over this extended content descriptor
											fseek(iHandle,2,SEEK_CUR);	//seek past the data type
											// get the descriptor value length, and skip past it
											fseek(iHandle, GetWmaMetaData_ReadWORD(iHandle), SEEK_CUR);
									}
								} else if (sDescriptorName[ 3]=='G' &&
											sDescriptorName[ 4]=='e' &&
											sDescriptorName[ 5]=='n' &&
											sDescriptorName[ 6]=='r' &&
											sDescriptorName[ 7]=='e' &&
											sDescriptorName[ 8]==0) {
									// Genre of the content
									if (iExtendedContent & (1 << EXTENDED_CONTENT_BIT_GENRE)) {
										pPointer = MetaData->wGenre;
										wBufferLength = MetaData->iGenreBufferLength;
										iExtendedContent &= ~(1 << EXTENDED_CONTENT_BIT_GENRE);
									} else {
											// Skip over this extended content descriptor
											fseek(iHandle,2,SEEK_CUR);	//seek past the data type
											// get the descriptor value length, and skip past it
											fseek(iHandle, GetWmaMetaData_ReadWORD(iHandle), SEEK_CUR);
									}
								} else if (sDescriptorName[ 3]== 'Y' &&
											sDescriptorName[ 4]== 'e' &&
											sDescriptorName[ 5]== 'a' &&
											sDescriptorName[ 6]== 'r') {
									// The year the content was published
									if (iExtendedContent & (1 << EXTENDED_CONTENT_BIT_YEAR)) {
										pPointer = MetaData->wYear;
										wBufferLength = MetaData->iYearBufferLength;
										iExtendedContent &= ~(1 << EXTENDED_CONTENT_BIT_YEAR);
								    } else {
		                            // Skip over this extended content descriptor
	                                fseek(iHandle,2,SEEK_CUR);	//seek past the data type
	                                // get the descriptor value length, and skip past it
	                                fseek(iHandle, GetWmaMetaData_ReadWORD(iHandle), SEEK_CUR);
								    }
            					} else if (sDescriptorName[ 3]== 'T' &&
											sDescriptorName[ 4]== 'r' &&
											sDescriptorName[ 5]== 'a' &&
											sDescriptorName[ 6]== 'c' &&
											sDescriptorName[ 7]== 'k' &&
											sDescriptorName[ 8]== 'N' &&
											sDescriptorName[ 9]== 'u' &&
											sDescriptorName[10]== 'm' &&
											sDescriptorName[11]== 'b' &&
											sDescriptorName[12]== 'e' &&
											sDescriptorName[13]== 'r' &&
											sDescriptorName[14]== 0) {
                                    if (iExtendedContent & (1 << EXTENDED_CONTENT_BIT_TRACKNUM)) {
                                        WORD iType = GetWmaMetaData_ReadWORD(iHandle);
                                        wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle);

                                        switch (iType)
                                        {
                                            case TYPE_UNICODE:
                                                GetWmaMetaData_ReadString(iHandle, pTracknum, TRACKNUM_SIZE, wDescriptorNameLength/2);

                                                for (pTmp=pTracknum,TracknumSize=0; *pTmp!=0; pTmp++)
                                                    TracknumSize++;

                                                TmpVal[0]=0;
                                                TmpVal[1]=1;
                                                while (TracknumSize)
                                                {
                                                    pTmp--;
                                                    TmpVal[2] = *pTmp - '0';
                                                    TmpVal[0] += TmpVal[1]*TmpVal[2];
                                                    TmpVal[1] *= 10;
                                                    TracknumSize--;
                                                }

                                                MetaData->wTrackNum = TmpVal[0];
                                                break;
                                            case TYPE_DWORD:
                                                MetaData->wTrackNum = (WORD) GetWmaMetaData_ReadDWORD(iHandle);
                                                break;
                                            case TYPE_QWORD:
                                                MetaData->wTrackNum = (WORD) GetWmaMetaData_ReadQWORD(iHandle);
                                                break;
                                            case TYPE_WORD:
                                                MetaData->wTrackNum = (WORD) GetWmaMetaData_ReadWORD(iHandle);
                                                break;
                                            default:
                                                // other Descriptor Value Data Type (BOOL, BYTE ARRAY, etc.)
                                                //MetaData->wTrackNum = 0x000fff;
                                                fseek(iHandle, wDescriptorNameLength, SEEK_CUR);
                                        }
                                    } else {
                                        // Skip over this extended content descriptor
	                                    fseek(iHandle,2,SEEK_CUR);	//seek past the data type
	                                    // get the descriptor value length, and skip past it
	                                    fseek(iHandle, GetWmaMetaData_ReadWORD(iHandle), SEEK_CUR);
                                    }
#ifdef JPEG_ALBUM_ART
								} else if(sDescriptorName[ 3]=='P' &&
									sDescriptorName[ 4]=='i' &&
									sDescriptorName[ 5]=='c' &&
									sDescriptorName[ 6]=='t' &&
									sDescriptorName[ 7]=='u' &&
									sDescriptorName[ 8]=='r' &&
									sDescriptorName[ 9]=='e') {
									if (iExtendedContent & (1 << EXTENDED_CONTENT_BIT_ALBUMART)) {
	                                    fseek(iHandle, 2, SEEK_CUR);//seek past the data type
                                        wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle);
										// Extract WM Picture
				            		    SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicReadObject, iHandle, wDescriptorNameLength, NULL);
										iExtendedContent &= ~(1 << EXTENDED_CONTENT_BIT_ALBUMART);
									} else {
										// Skip over this extended content descriptor
										fseek(iHandle,2,SEEK_CUR);	//seek past the data type
										// get the descriptor value length, and skip past it
										fseek(iHandle, GetWmaMetaData_ReadWORD(iHandle), SEEK_CUR);
                                    }
#endif	// JPEG_ALBUM_ART
#ifdef SYNC_LYRICS
							    } else if( sDescriptorName[ 3]=='L' &&
									sDescriptorName[ 4]=='y' &&
									sDescriptorName[ 5]=='r' &&
									sDescriptorName[ 6]=='i' &&
									sDescriptorName[ 7]=='c' &&
									sDescriptorName[ 8]=='s' &&
									sDescriptorName[ 9]=='_' &&
									sDescriptorName[10]=='S' &&
									sDescriptorName[11]=='y' &&
									sDescriptorName[12]=='n' &&
									sDescriptorName[13]=='c' &&
									sDescriptorName[14]=='h' &&
									sDescriptorName[15]=='r' &&
									sDescriptorName[16]=='o' &&
									sDescriptorName[17]=='n' &&
									sDescriptorName[18]=='i' &&
									sDescriptorName[19]=='s' &&
									sDescriptorName[20]=='e' &&
									sDescriptorName[21]=='d') {
									if (iExtendedContent & (1 << EXTENDED_CONTENT_BIT_LYRICS)) {
	                                    fseek(iHandle, 2, SEEK_CUR);//seek past the data type
                                        wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle);	// Descriptor Value Length
    									// Extract WMA Lyrics
	    								SysCallFunction(RSRC_WMA_LYRICS_CODEBANK, WMALyricsReadFrame, iHandle, wDescriptorNameLength, NULL);
		    							iExtendedContent &= ~(1 << EXTENDED_CONTENT_BIT_LYRICS);
									} else {
										// Skip over this extended content descriptor
										fseek(iHandle,2,SEEK_CUR);	//seek past the data type
										// get the descriptor value length, and skip past it
										fseek(iHandle, GetWmaMetaData_ReadWORD(iHandle), SEEK_CUR);
                                    }
#endif	// SYNC_LYRICS
								} else {
									// Skip over this extended content descriptor
									fseek(iHandle,2,SEEK_CUR);	//seek past the data type
									// get the descriptor value length, and skip past it
									fseek(iHandle, GetWmaMetaData_ReadWORD(iHandle), SEEK_CUR);
								}

								if (pPointer) {
									if (GetWmaMetaData_ReadWORD(iHandle)==0) {
										//must be a unicode string
										wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle)/2;
										GetWmaMetaData_ReadString(iHandle, pPointer, wBufferLength, wDescriptorNameLength);
									}
									pPointer = NULL;
								}

								if (!iExtendedContent) {
									// all of the field have been updated, quit the loop
									break;
								}
							}
							// added end, SL
#endif
                            else
                            {//skip over this extended content descriptor
                                fseek(iHandle,2,SEEK_CUR);//seek past the data type
                                //get the descriptor value length, and skip past it
                                fseek(iHandle,GetWmaMetaData_ReadWORD(iHandle),SEEK_CUR);
                            }
                        }
                    }
                }

            }
#if (defined JPEG_ALBUM_ART) || (defined SYNC_LYRICS)
            TempGUID.wValues[0] = ASF_HEADER_EXTENSION_GUID_0;
            TempGUID.wValues[1] = ASF_HEADER_EXTENSION_GUID_1;
            TempGUID.wValues[2] = ASF_HEADER_EXTENSION_GUID_2;
            TempGUID.wValues[3] = ASF_HEADER_EXTENSION_GUID_3;
            TempGUID.wValues[4] = ASF_HEADER_EXTENSION_GUID_4;
            TempGUID.wValues[5] = ASF_HEADER_EXTENSION_GUID_5;

            if (GetWmaMetaData_FindFirstObject(iHandle, wObjectCount, &TempGUID) > 0) {
				// ASF Header Extension Object is found
            	DWORD	dwHeaderExtensionDataSize;
            	INT		iDataType, iDataLength;

                TempGUID2.wValues[0] = ASF_RESERVED_1_GUID_0;
                TempGUID2.wValues[1] = ASF_RESERVED_1_GUID_1;
                TempGUID2.wValues[2] = ASF_RESERVED_1_GUID_2;
                TempGUID2.wValues[3] = ASF_RESERVED_1_GUID_3;
                TempGUID2.wValues[4] = ASF_RESERVED_1_GUID_4;
                TempGUID2.wValues[5] = ASF_RESERVED_1_GUID_5;

                FSFileRead(16, iHandle, MEM_SPACE_Y, -1, (void *)&TempGUID);
                if (GetWmaMetaData_CompareGUID(&TempGUID, &TempGUID2)) {
               	// Reserved Field 1 is equal to ASF_Reserved_1
					if (GetWmaMetaData_ReadWORD(iHandle) == 0x06) {
	                	// Reserved Field 2 is equal to 6
						dwHeaderExtensionDataSize = GetWmaMetaData_ReadDWORD(iHandle);	// get header extension data size

		                TempGUID.wValues[0] = ASF_METADATA_LIBRARY_GUID_0;
		                TempGUID.wValues[1] = ASF_METADATA_LIBRARY_GUID_1;
		                TempGUID.wValues[2] = ASF_METADATA_LIBRARY_GUID_2;
		                TempGUID.wValues[3] = ASF_METADATA_LIBRARY_GUID_3;
		                TempGUID.wValues[4] = ASF_METADATA_LIBRARY_GUID_4;
		                TempGUID.wValues[5] = ASF_METADATA_LIBRARY_GUID_5;

		                if (GetWmaMetaData_FindNextObjectBySize(iHandle, dwHeaderExtensionDataSize, &TempGUID) > 0) {
		                	// Metadata Library object is found, find WM/Lyrics_Synchronised
	                        wContentDescriptorCount = GetWmaMetaData_ReadWORD(iHandle);	// get description records count
	                        for (i = 0; i < wContentDescriptorCount; i++) {
								fseek(iHandle, 2+2, SEEK_CUR); // seek past the language list index and stream number
                            	wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle) / 2;
								fseek(iHandle, 2, SEEK_CUR); // seek past the data type
                            	iDataLength = GetWmaMetaData_ReadDWORD(iHandle);
                            	GetWmaMetaData_ReadString(iHandle, sDescriptorName, 23, wDescriptorNameLength);
                            	if (0){}
#ifdef JPEG_ALBUM_ART
	                            else if (sDescriptorName[ 0]=='W' &&
									sDescriptorName[ 1]=='M' &&
									sDescriptorName[ 2]=='/' &&
									sDescriptorName[ 3]=='P' &&
									sDescriptorName[ 4]=='i' &&
									sDescriptorName[ 5]=='c' &&
									sDescriptorName[ 6]=='t' &&
									sDescriptorName[ 7]=='u' &&
									sDescriptorName[ 8]=='r' &&
									sDescriptorName[ 9]=='e') {
									// Extract WM Picture
            						SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicReadObject, iHandle, iDataLength, NULL);
	                               	break;
	                            }
#endif
#ifdef SYNC_LYRICS
	                            else if (sDescriptorName[ 0]=='W' &&
									sDescriptorName[ 1]=='M' &&
									sDescriptorName[ 2]=='/' &&
									sDescriptorName[ 3]=='L' &&
									sDescriptorName[ 4]=='y' &&
									sDescriptorName[ 5]=='r' &&
									sDescriptorName[ 6]=='i' &&
									sDescriptorName[ 7]=='c' &&
									sDescriptorName[ 8]=='s' &&
									sDescriptorName[ 9]=='_' &&
									sDescriptorName[10]=='S' &&
									sDescriptorName[11]=='y' &&
									sDescriptorName[12]=='n' &&
									sDescriptorName[13]=='c' &&
									sDescriptorName[14]=='h' &&
									sDescriptorName[15]=='r' &&
									sDescriptorName[16]=='o' &&
									sDescriptorName[17]=='n' &&
									sDescriptorName[18]=='i' &&
									sDescriptorName[19]=='s' &&
									sDescriptorName[20]=='e' &&
									sDescriptorName[21]=='d') {
									// Extract WMA Lyrics
									SysCallFunction(RSRC_WMA_LYRICS_CODEBANK, WMALyricsReadFrame, iHandle, iDataLength, NULL);
	                               	break;
	                            }
#endif
	                            else {
	                                fseek(iHandle, iDataLength, SEEK_CUR);	// seek past the actual metadata
	                            }	// end of if (sDescriptorName)
	                        }	// end of for(i)
		                }	// end of if (GetWmaMetaData_FindNextObjectByEndPos())
					}	// end of if (GetWmaMetaData_ReadWORD())
                }	// end of if (GetWmaMetaData_CompareGUID())
            }	// end of if (GetWmaMetaData_FindFirstObject())
#endif	// (defined JPEG_ALBUM_ART) || (defined SYNC_LYRICS)
        }
        if(iHandle != -1)
        {
#ifdef USE_PLAYLIST3
          if (g_Rebuild == TRUE)
            Fclose(iHandle);
          else
#endif
            FSFileClose(iHandle);
        }

    }
    else
      rtn = META_DATA_FILEOPEN_ERROR;
    return rtn;
}
