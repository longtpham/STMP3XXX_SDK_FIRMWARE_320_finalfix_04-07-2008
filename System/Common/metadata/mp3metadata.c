///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2003
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
#include "mp3metadata.h"
#include "resource.h"
#include "dbcs.h"
#include "id3v2.h"
#include "menumanager.h"
#include "project.h"
#ifdef SYNC_LYRICS
#include "SyncLyrics.h"
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

// For metadata display. Manually changing the global
// variable before compiling will make it effective.
//	Use:	NO_TAG_SELECTED	----display ID3V2, or ID3V1 if ID3V2 is not available.
//			ID3V1_TAG_SELECTED---display ID3V1 only
//			ID3V2_TAG_SELECTED---display ID3V2 only


extern WORD g_wVBRSignature;   // Xing or VBR ("X"/"V"/"U"(unknown) or 0 for none)
extern BOOL g_Rebuild;

INT g_iTagSelected = NO_TAG_SELECTED;





////////////////////////////////////////////////////////////////////////////////
//    local function prototypes
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MP3MetaDataReadNonID3Info(INT iHandle, FILE_META_DATA *pMetaData);

RETCODE _reentrant CheckID3Headers(INT iHandle, FILE_META_DATA * MetaData);

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant GetMp3MetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
//
//   Type:          Function
//
//   Description:   Finds the meta-data within an mp3 file.
//
//   Inputs:        INT PackedPathNameAddress:          "Pointer" to the fully qualified path name (_packed BYTE*)
//                  INT btCurrentDevice:                which device its on.
//                  FILE_META_DATA * MetaData:          structure to fill out
//
//   Outputs:       SUCCESS or !SUCCESS
//
//   Notes:         the pathname pointer must be passed in as an integer because of the calling conventions of
//                  SysCallFunction.  It holds the numeric value of the the _packed BYTE pointer.
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant GetMp3MetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
    RETCODE rtn = META_DATA_NOT_FOUND;
    INT iHandle;                            // indicates the handle of file opened (or error if failure)
    DWORD dwNumBytesTemp;                           // stores 48 bits, number of bytes in file
    _packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;

    // Set some defaults
    MetaData->dwStartPos = 0;
    MetaData->dwNumFrames = (DWORD)0;
    MetaData->iTime = 0;

#ifndef USE_PLAYLIST3
    // Open the file.
#ifdef USE_PLAYLIST5
	g_CurrentFastKey = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->file.lFastKey;
	iHandle = FSFastOpen(g_CurrentFastKey,READ);
#else
    iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);
#endif //#ifdef USE_PLAYLIST5
    if (iHandle != ERROR)
    {
        // we know this is an MP3 file, so fill in file type and resource
        MetaData->iFileType = (INT)MP3_TYPE;
        MetaData->iResourceNum = RSRC_DECMOD_CODE;

        // Get the number of bytes in song (updated as all the headers are found)
        MetaData->dwNumBytes = FSFileSize(iHandle);
        MetaData->dwStartPos = 0;

        CheckID3Headers(iHandle, MetaData);

		rtn = SysCallFunction(RSRC_VBR_CODEBANK,MP3MetaDataReadNonID3Info,iHandle,0,(void*)MetaData);

        FSFileClose(iHandle);
    }
    else
      rtn = META_DATA_FILEOPEN_ERROR;
    return rtn;
#else
	// Open the file.
	if (g_Rebuild == FALSE)
	{
		if(g_CurrentSongFastkey==0)
		{
		iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);
			g_CurrentSongFastkey=FgetFastHandle(iHandle);
		}
		else
			iHandle = FSFastOpen(g_CurrentSongFastkey,READ);
   }
	else
		iHandle = FSFastOpen(g_FileKey,READ);

	if (iHandle != ERROR)
	{
		//get modification date for rebuild, added by Raymond
		if (g_Rebuild == TRUE)
		{
			filegetdate(iHandle,CREATION_DATE,&g_dirdate,&g_dirtime);
		}

		// we know this is an MP3 file, so fill in file type and resource
		MetaData->iFileType = (INT)MP3_TYPE;
		MetaData->iResourceNum = RSRC_DECMOD_CODE;

		// Get the number of bytes in song (updated as all the headers are found)
		MetaData->dwNumBytes = FSFileSize(iHandle);
		MetaData->dwStartPos = 0;

		CheckID3Headers(iHandle, MetaData);

		if (g_Rebuild == FALSE)
			rtn = SysCallFunction(RSRC_VBR_CODEBANK,MP3MetaDataReadNonID3Info,iHandle,0,(void*)MetaData);

		if (g_Rebuild == FALSE)
			FSFileClose(iHandle);
		else
			Fclose(iHandle);
	}
	else
		rtn = META_DATA_FILEOPEN_ERROR;
	return rtn;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant CheckID3Headers(INT iHandle, FILE_META_DATA * MetaData)
//
//   Type:          Function
//
//   Description:   Finds the ID3 meta-data within an mp3 file.
//
//   Inputs:        INT iHandle:                    File handle that we're checking
//                  FILE_META_DATA * MetaData:          structure to fill out
//
//   Outputs:       SUCCESS or !SUCCESS
//
//   Notes:         Since the arrays are allocated on the stack, this localizes the effect.
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant CheckID3Headers(INT iHandle, FILE_META_DATA * MetaData)
{
  	UCS3 wSongTitle[TITLE_SIZE];
  	UCS3 wSongArtist[ARTIST_SIZE];
  	UCS3 wSongAlbum[ALBUM_SIZE];
#ifdef USE_PLAYLIST3
	UCS3 wSongGenre[GENRE_SIZE];
	UCS3 wSongYear[YEAR_SIZE];
#endif

    //This leaves the file position the same.
    Mp3MetaDataReadID3V1(iHandle,MetaData);
    // Save ID3V1 data in MetaDataTemp
    if ( g_iTagSelected != ID3V1_TAG_SELECTED )
    {
      if ( g_iTagSelected == NO_TAG_SELECTED)
      {
        strcpy(wSongTitle,  MetaData->wTitle);
        strcpy(wSongArtist, MetaData->wArtist);
        strcpy(wSongAlbum,  MetaData->wAlbum);
#ifdef USE_PLAYLIST3
			if (g_Rebuild == TRUE)
			{
				strcpy(wSongGenre,  MetaData->wGenre);
				strcpy(wSongYear,  MetaData->wYear);
			}
#endif
      }
#ifdef SYNC_LYRICS
    if( !g_Rebuild )
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
		if (MetaData->wAlbum)
		*MetaData->wAlbum = 0;
		SysCallFunction(RSRC_LRC_CODEBANK,Mp3MetaDataReadLRC,0,0,(INT *)&LyricFileMetaData);

		if ( g_iTagSelected == NO_TAG_SELECTED)
		{
			if ( strlen(MetaData->wTitle) != 0 )
				strcpy(wSongTitle, MetaData->wTitle);
			if ( strlen(MetaData->wArtist) != 0 )
				strcpy(wSongArtist, MetaData->wArtist);
			if ( strlen(MetaData->wAlbum) != 0 )
				strcpy(wSongAlbum, MetaData->wAlbum);
		}

		if (MetaData->wTitle)
		*MetaData->wTitle = 0;
		if (MetaData->wArtist)
		*MetaData->wArtist = 0;
		if (MetaData->wAlbum)
		*MetaData->wAlbum = 0;
		SysCallFunction(RSRC_LYRICS3_CODEBANK,Mp3MetaDataReadLyrics3,iHandle,0,(INT  *)&LyricFileMetaData);

		if ( g_iTagSelected == NO_TAG_SELECTED)
		{
			if ( strlen(MetaData->wTitle) != 0 )
				strcpy(wSongTitle, MetaData->wTitle);
			if ( strlen(MetaData->wArtist) != 0 )
				strcpy(wSongArtist, MetaData->wArtist);
			if ( strlen(MetaData->wAlbum) != 0 )
				strcpy(wSongAlbum, MetaData->wAlbum);
		}
	}
#endif
      if ( g_iTagSelected == ID3V2_TAG_SELECTED) // Do not use ID3V1 data
      {
        *MetaData->wTitle = 0;
        *MetaData->wArtist = 0;
        *MetaData->wAlbum = 0;
#ifdef USE_PLAYLIST3
			if (g_Rebuild == TRUE)
			{
				*MetaData->wGenre = 0;
				*MetaData->wYear = 0;
			}
#endif
      }
      //Attempt to read the ID3V2 tag
      //this leaves the file position at the first byte after the Id3V2 tag, or where it entered the function
      Mp3MetaDataReadID3V2(iHandle,0,(void*)MetaData);

      if ( g_iTagSelected == NO_TAG_SELECTED) //Use ID3V2, or ID3V1 if ID3V2 is not avaiable.
      {
        if ( packed_strlen((_packed BYTE *)(MetaData->wTitle)) == 0 )
          strcpy(MetaData->wTitle, wSongTitle);
        if ( packed_strlen((_packed BYTE *)(MetaData->wArtist)) == 0 )
          strcpy(MetaData->wArtist, wSongArtist);
        if ( packed_strlen((_packed BYTE *)(MetaData->wAlbum)) == 0 )
          strcpy(MetaData->wAlbum, wSongAlbum);
#ifdef USE_PLAYLIST3
			if (g_Rebuild == TRUE)
			{
				if ( packed_strlen((_packed BYTE *)(MetaData->wGenre)) == 0 )
					strcpy(MetaData->wGenre, wSongGenre);

				if ( packed_strlen((_packed BYTE *)(MetaData->wYear)) == 0 )
					strcpy(MetaData->wYear, wSongYear);
			}
#endif
      }
    }
    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//> Name:        RETCODE  _reentrant Mp3MetaDataReadID3V1(INT iHandle,FILE_META_DATA*pMetaData)
//
//  Type:        Function
//
//  Description: Used to check for ID3v1 tag at end of file (last 128 bytes)
//
//  Inputs:      INT iHandle:               handle to the open file
//               FILE_META_DATA*pMetaData:  pointer to the meta data structure to fill in
//  Outputs:     SUCCESS or !SUCCESS.
//
//  Notes:       If TAG is found, fills in the following in the meta-data struct:
//                 Title, Artist, Album, Genre.
//               Always returns the file position to the same as it was prior to entering this function.
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant Mp3MetaDataReadID3V1(INT iHandle,FILE_META_DATA*pMetaData)
{
    WORD uLength;
    WORD uTemp=0;
    _packed BYTE TempString[31];

    LONG uPosition = ftell(iHandle);

    if(fseek(iHandle,-128,SEEK_END)!= ERROR)
        FSFileRead(3, iHandle, MEM_SPACE_Y, (INT)(-1), &uTemp);

    if(uTemp== 0x474154)
    {

        // if we get this far, we have found an ID3v1 tag
        //subtract off 128 bytes from the length of the song.
        pMetaData->dwNumBytes-= 128;

        //Read in the song title.
        FSFileRead(30, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)TempString);
        packed_set(TempString,30,0);//null terminate.
        if(pMetaData->wTitle)
        {
            uLength = 31;
            if(pMetaData->iTitleBufferLength < uLength)
              uLength = pMetaData->iTitleBufferLength;
#ifdef USE_PLAYLIST3
			if (g_Rebuild == TRUE){
				// if string length is less than 2, convert it to Unicode of AddSongToLibrary() use, SGTL
				if (packed_strlen(TempString) > 2)
					packed_strcpy((_packed BYTE *)pMetaData->wTitle,TempString);
				else
					DBCSToUnicode(TempString,pMetaData->wTitle,uLength);
			} else
#endif
            DBCSToUnicode(TempString,pMetaData->wTitle,uLength);
        }

        //Read in the song artist.
        FSFileRead(30, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)TempString);
        packed_set(TempString,30,0);//null terminate.
        if(pMetaData->wArtist)
        {
            uLength = 31;
            if(pMetaData->iArtistBufferLength < uLength)
              uLength = pMetaData->iArtistBufferLength;
#ifdef USE_PLAYLIST3
			if (g_Rebuild == TRUE) {
				// if string length is less than 2, convert it to Unicode of AddSongToLibrary() use, SGTL
				if (packed_strlen(TempString) > 2)
					packed_strcpy((_packed BYTE *)pMetaData->wArtist,TempString);
				else
					DBCSToUnicode(TempString,pMetaData->wArtist,uLength);
			} else
#endif
            DBCSToUnicode(TempString,pMetaData->wArtist,uLength);
        }

        //Read in the song title.
        FSFileRead(30, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)TempString);
        packed_set(TempString,30,0);//null terminate.
        if(pMetaData->wAlbum)
        {
            uLength = 31;
            if(pMetaData->iAlbumBufferLength < uLength)
              uLength = pMetaData->iAlbumBufferLength;
#ifdef USE_PLAYLIST3
			if (g_Rebuild == TRUE) {
				// if string length is less than 2, convert it to Unicode of AddSongToLibrary() use, SGTL
				if (packed_strlen(TempString) > 2)
					packed_strcpy((_packed BYTE *)pMetaData->wAlbum,TempString);
				else
					DBCSToUnicode(TempString,pMetaData->wAlbum,uLength);
			} else
#endif
            DBCSToUnicode(TempString,pMetaData->wAlbum,uLength);
        }
#ifdef USE_PLAYLIST3
		if (g_Rebuild == TRUE)
		{
			//added by Raymond (remark: the size of the year field is 4 character)
			//Read in the song year.
			FSFileRead(4, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)TempString);
			packed_set(TempString,4,0);//null terminate.
			if(pMetaData->wYear)
			{
				uLength = 5;
				if(pMetaData->iYearBufferLength < uLength)
					uLength = pMetaData->iYearBufferLength;

	           	packed_strcpy((_packed BYTE *)pMetaData->wYear,TempString);
			}
		}
		else
		    fseek(iHandle,4,SEEK_CUR);


//			//seek past the comment and year
//			fseek(iHandle,30,SEEK_CUR);

        // read in the last two bytes of comment field
        fseek(iHandle,28,SEEK_CUR);
        uTemp = 0;
        FSFileRead(2, iHandle, MEM_SPACE_Y, (INT)(-1), &uTemp);

        // second last byte should be 0, the last byte should not be zero, look zero as an unknown track number
        if (((uTemp&0xff) == 0) && ((uTemp>>8)&0xff) != 0)
            pMetaData->wTrackNum = (uTemp >> 8) & 0xff;
#else
        //seek past the comment and year
        fseek(iHandle,34,SEEK_CUR);
#endif
#ifdef USE_PLAYLIST3
        //Read in the genre.
        if (g_Rebuild == TRUE)
        {
#endif
            uTemp = 0;
            FSFileRead(1, iHandle, MEM_SPACE_Y, (INT)(-1), &uTemp);
            pMetaData->btGenre = uTemp;
#ifdef USE_PLAYLIST3
	    }
#endif

    }
    //return to where we were when we entered the function.
    fseek(iHandle,uPosition,SEEK_SET);
    return (SUCCESS);
}


