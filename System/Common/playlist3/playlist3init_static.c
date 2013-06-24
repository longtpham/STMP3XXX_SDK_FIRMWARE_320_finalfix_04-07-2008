////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Sigmatel Inc 2005-2007
// Filename: playlist3init_static.c
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "menumanager.h"
#include "playlist.h"       //required include file
#include "playerlib.h"
#include "project.h"
#include "stringlib.h"
#include "resource.h"
#include "sysspeed.h"
#include "playlist3.h"
#include "playlist3internal.h"
#include "persist_bits.h"
#include "sysmem.h"
#include "FileSystem.h"
#include "fserror.h"
#include "FileSpec.h"
#include "playlist.h"
#include "syscoder.h"
#include "encoderproperties.h"
#ifdef AUDIBLE
#include "..\metadata\AAMetaStrings.h"
#endif
#include "display.h"
#include "playerstatemachine.h"


#ifdef MMC
#include "mmcddi.h"
#endif
#include "fsapi.h"
#include "metadata.h"
#include "..\metadata\mp3metadata.h"
#include "..\metadata\wavmetadata.h"
#include "..\metadata\wmametadata.h"
#include "sdmd.h"
#include "musiclib_ghdr.h"
#include "voicemenu.h"

#pragma optimize 2

#pragma asm
	nolist
		include "sysresources.inc"
		include "sysequ.inc"
	list
#pragma endasm

_asmfunc MMCDetectPresenceWrapper(void*);
extern _reentrant INT GetCWDHandle(void);

extern int MenuResourcePtr;
extern int g_total_song;
extern int g_option;
extern BOOL g_build_voice_file;
extern BOOL g_build_fm_file;
extern BOOL g_build_linein_file;
extern Finddata _X g_AddToLibraryData[];
extern int _X g_NumAddToLibrary;
extern WORD g_Playlist3InitExtension;
extern BOOL SD_build_success;
extern BOOL g_bMMCPreviousTest;

#ifdef AUDIBLE
static RETCODE _reentrant GetAudibleMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData);
#endif

RETCODE _reentrant ML_GetLFN(DWORD dwFastKey, INT iRecordNum, UCS3 *pBuf)
{
	RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;
	INT iDirHandle;
    UCS3    wLFN[MAX_FILENAME_LENGTH];
    UCS3*   uptmp;
    int i;

	if(!((iDirHandle = FastOpen(dwFastKey,(_packed char*)"d")) <0))
	{
		if(ConstructLongFileName(iDirHandle, iRecordNum, (INT*) wLFN) > 0)
		{
	        for (i=0,uptmp=wLFN; *uptmp!=0; uptmp++)
	            i++;
	        if (i<(LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD))
	        {   strcpy(pBuf, wLFN);
	        }
	        else
	        {   strncpy(pBuf, wLFN, LEN_OF_LONG_FILENAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD);
	        }
	        rtn = PLAYLIST_SUCCESS;
	    }
		Fclose(iDirHandle);
	}
	return rtn;
}

_reentrant void Rebuild_GetFileMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA *MetaData)
{
	RETCODE rtn = META_DATA_FOUND;
	WORD iStrLen;
	_packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;

	INT fIn;

	// check if path + filename will fit in unpacked buffer
	iStrLen= packed_strlen(pcFilePathName);

	if(iStrLen<(MAX_FILENAME_LENGTH-1))
	{
		// clear out the artist field
		if(MetaData->wTitle)
		{
			MetaData->wTitle[0] = 0;
			MetaData->wTitle[1] = 0;
		}

		if(MetaData->wArtist)
		{
			MetaData->wArtist[0] = 0;
			MetaData->wArtist[1] = 0;
		}
		// clear out the album field
		if(MetaData->wAlbum)
		{
			MetaData->wAlbum[0] = 0;
			MetaData->wAlbum[1] = 0;
		}
		// clear out the genre field
		if(MetaData->wGenre)
		{
			MetaData->wGenre[0] = 0;
			MetaData->wGenre[1] = 0;
		}

		// clear out the year field
		if(MetaData->wYear)
		{
			MetaData->wYear[0] = 0;
			MetaData->wYear[1] = 0;
		}

		// Clear the genre byte
		MetaData->btGenre = (BYTE)0;

#ifndef MMC
		if( g_Playlist3InitExtension == 0 )
		{
			INT i;
			if(iStrLen > 4)
			{
				while(iStrLen--)
				{
					g_Playlist3InitExtension = packed_get((_packed BYTE *)pcFilePathName,iStrLen);
					if(g_Playlist3InitExtension=='.')
						break;
				}
				if(iStrLen)
				{
					for(i=0;i<3;i++)
					{
						packed_set((void*)&g_Playlist3InitExtension,i,packed_get((_packed BYTE *)pcFilePathName,++iStrLen));
					}
				}
			}
		}
#endif
		switch(g_Playlist3InitExtension)
		{
		case 0x33504d:  //"MP3"
//#ifndef MMC
			if( MenuResourcePtr == RSRC_MP3_METADATA_CODEBANK )
//#endif
				rtn = GetMp3MetaData((INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
//#ifndef MMC
			else
				rtn = SysCallFunction(RSRC_MP3_METADATA_CODEBANK,GetMp3MetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
//#endif
			g_unicode = 0;
			g_option = ADD_OPTION_MUSIC;
			break;
		case 0x414D57:  //"WMA"
//#ifndef MMC
			if( MenuResourcePtr == RSRC_WMA_METADATA_CODEBANK )
//#endif
				rtn = GetWmaMetaData((INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
//#ifndef MMC
			else
				rtn = SysCallFunction(RSRC_WMA_METADATA_CODEBANK,GetWmaMetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
//#endif
			g_unicode = 1;
			g_option = ADD_OPTION_MUSIC;
			break;
		case 0x564157:  //"WAV"
//#ifndef MMC
			if( MenuResourcePtr == RSRC_WAV_METADATA_CODEBANK )
//#endif
				rtn = GetWavMetaData((INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
//#ifndef MMC
			else
				rtn = SysCallFunction(RSRC_WAV_METADATA_CODEBANK,GetWavMetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
//#endif
			g_unicode = 0;
			g_option = ADD_OPTION_MUSIC;
			break;
#ifdef AUDIBLE
		case 0x004141:  //"AA "
			rtn = GetAudibleMetaData((INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
            g_unicode = 0;
			g_option = 2;
            break;
#endif
		default:
			rtn = META_DATA_NOT_FOUND;
			break;
		}

		if(!strlen(MetaData->wTitle))
		{	//if the string length is zero then check if LFN or SFN is present.

			strcpy(MetaData->wTitle, song_info.dir_name);
		}
	}
}

#ifdef AUDIBLE
static RETCODE _reentrant GetAudibleMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
	INT fIn; 

	// Open the AA file
	fIn = FSFileOpen( (_packed BYTE *) PackedPathNameAddress, READ, btCurrentDevice );
	if ( fIn < 0 )
		return META_DATA_NOT_FOUND;

	// Use new code bank APIs to retrieve metadata. No longer necessary to open
	// AA file via Audible SDK file open routines.
	SysCallFunction( RSRC_AUDIBLE_METASTRINGS_P, AAGetMetaTitle, fIn, MetaData->iTitleBufferLength, (INT *) MetaData->wTitle );

	// We're done with the file now at this point
	FSFileClose( fIn );

	return META_DATA_FOUND;
}
#endif

_reentrant void Rebuild_GetMetaData(WORD wMode,int ignored1,int*ignored2)
{
	FILE_META_DATA MetaData;

	MetaData.wTitle=g_wSongTitle;
	MetaData.iTitleBufferLength=TITLE_SIZE;
	MetaData.wArtist=g_wSongArtist;
	MetaData.iArtistBufferLength=ARTIST_SIZE;
	MetaData.wAlbum=g_wSongAlbum;
	MetaData.iAlbumBufferLength=ALBUM_SIZE;
	MetaData.wGenre=g_wSongGenre;
	MetaData.iGenreBufferLength=GENRE_SIZE;
	MetaData.wYear=g_wSongYear;
	MetaData.iYearBufferLength=YEAR_SIZE;
	MetaData.wTrackNum=UNKNOWN_TRACK_NUMBER;
	g_wSongTrackNum = UNKNOWN_TRACK_NUMBER;

	Rebuild_GetFileMetaData((INT)(&g_CurrentSongName), (BYTE)g_wCurrentDirDevId, (void*)&MetaData);

    if (MetaData.wTrackNum <= MAX_TRACK_NUMBER)
        g_wSongTrackNum = MetaData.wTrackNum;

}

_reentrant void TrimUnicodeString_Extra(UCS3* pString)
{
	int i;
	i = strlen(pString)-1;
	while((i>=0) && (pString[i]==' '))
	{
		pString[i]=0;
		i--;
	}
}

_reentrant void TrimPackedString_Extra(UCS3* pString)
{

	int i=0;
	int check_char=0;

	while ( (pString[i] != 0) && (i < 40))
	{
		i++;
	}
	i--;

    while (i>=0)
	{
		check_char = pString[i] & 0xFF0000;
		if ((check_char) && (check_char != 0x200000))
			break;
		else
			pString[i] = pString[i] & 0x00FFFF;

		check_char = pString[i] & 0x00FF00;
		if ((check_char) && (check_char != 0x002000))
			break;
		else
			pString[i] = pString[i] & 0x0000FF;

		check_char = pString[i] & 0x0000FF;
		if ((check_char) && (check_char != 0x000020))
			break;
		else
			pString[i] = 0;
        
	    i--;
	}
}

_reentrant INT AddSongToLibraryStatic(int drive)
{
    INT     iSongCount;
    INT     iDirHandle;

    iSongCount = 0;
	if ((drive == 0) || (drive == 1))
	{
		Finddata finddata;
		int i=0;
		BOOL bMMCPresent=FALSE;
		_packed BYTE genre[15]; // for add entry
		int year;  //for add entry
		int temp_conversion=0;
		int comp1=0;
		int comp2=0;

        ((int*)(genre))[0]=0x6b6e55; // Unk
        ((int*)(genre))[1]=0x776f6e; // now
        ((int*)(genre))[2]=0x47206e; // n G
        ((int*)(genre))[3]=0x726e65; // enr
        ((int*)(genre))[4]=0x000065; // e
		iDirHandle = GetCWDHandle();

		if((g_NumAddToLibrary > 0))
        {
		    g_NumAddToLibrary--;
			{
				int j;
				WORD *dst = (WORD*)&finddata;
				WORD _X * src = (WORD _X *)&g_AddToLibraryData[g_NumAddToLibrary];
				
				for( j = 0; j < (int)sizeof(Finddata); j++ )
					*dst++ = *src++;								
			}						     
			g_CurrentSongName[0] = 0;

			packed_strcpy(g_CurrentSongName, DirPath);
			packed_strcat(g_CurrentSongName, (_packed BYTE *)finddata.name);
			g_wCurrentDirDevId = drive;

			if ((((int *)g_CurrentSongName)[1]!= 0x454641) || (((int *)g_CurrentSongName)[2]!= 0x545341))//no AFEAST
			{
				//get LFN
				g_iRecordNum = finddata.startrecord - 1;
				g_FileKey = finddata.Key;
				song_info.g_songFastKey=finddata.Key;
                song_info.dir_name[0] = '\0';
#if 1
                if ((g_build_voice_file == TRUE) || (g_build_fm_file == TRUE) || (g_build_linein_file == TRUE))
                {
                    ConstructLongFileName(iDirHandle, g_iRecordNum, song_info.dir_name);
                }
                else
                    ML_GetLFN(g_dwFastKey, g_iRecordNum, song_info.dir_name);
#else
                ML_GetLFN(g_dwFastKey, g_iRecordNum, song_info.dir_name);
#endif
				Rebuild_GetMetaData(0,0,0);

				/* The bit mask for 2 bits Unicode Flag selection in "uint8 unicode".
				Variable Represented Bits used in "uint8 unicode"
				Artistname 1:0
				Albumname 3:2
				Genrename 5:4
				Songname 7:6
				----------------------------------------------------------
				Value (2 bits) Meanings
				0 RAW and All ASCII
				1 Uni-code
				2 Mixed, non-unicode
				*/
				//Stmp00013555
				g_wSongArtist[ARTIST_SIZE-1] = 0;
				g_wSongAlbum[ALBUM_SIZE-1]  = 0;
				g_wSongTitle[TITLE_SIZE-1]  = 0;

				if (g_unicode > 0) //non mp3 case
				{
                    if (g_wSongTitle[0] == 0)
                    {
						song_info.unicode = 0x15; //0x00010101;
						packed_strcpy((_packed BYTE *)g_wSongTitle, (_packed BYTE *)finddata.name);
                    }
                    else
                    {
						song_info.unicode = 0x55; //0x01010101;
					}
					TrimUnicodeString_Extra(g_wSongArtist);
					TrimUnicodeString_Extra(g_wSongAlbum);
					TrimUnicodeString_Extra(g_wSongGenre);
					TrimUnicodeString_Extra(g_wSongTitle);
					strcpy(song_info.artist_name, g_wSongArtist);
					strcpy(song_info.album_name, g_wSongAlbum);
					strcpy(song_info.genre_name, g_wSongGenre);
					strcpy(song_info.song_name, g_wSongTitle);
					if(g_wSongYear[0] != 0)
					{
						year = 0;

						for (i=0; i<4; i++)
						{
							temp_conversion = g_wSongYear[i];
							year = year + (temp_conversion-48); //perform hex range shift

							if (i != 3)
								year = year	* 10;
						}
						song_info.year = year;
					}
					else
						song_info.year = UNKNOWN_YEAR_CODE; // Unknown Year
				}
				else
				{
					song_info.unicode = 0;  //0x00000000;
                    if (g_wSongTitle[0] == 0)
                    {
						packed_strcpy((_packed BYTE *)g_wSongTitle, (_packed BYTE *)finddata.name);
					}
					if (g_wSongArtist[0] < 65536)
					{
						TrimUnicodeString_Extra(g_wSongArtist);
						strcpy(song_info.artist_name, g_wSongArtist);
						song_info.unicode = song_info.unicode | 0x00000001;
					}
					else
					{
						TrimPackedString_Extra(g_wSongArtist);
						packed_strcpy((_packed BYTE *)song_info.artist_name, (_packed BYTE *)g_wSongArtist);
					}

					if (g_wSongAlbum[0] < 65536)
					{
						TrimUnicodeString_Extra(g_wSongAlbum);
						strcpy(song_info.album_name, g_wSongAlbum);
						song_info.unicode = song_info.unicode | 0x00000004;
					}
					else
					{
						TrimPackedString_Extra(g_wSongAlbum);
						packed_strcpy((_packed BYTE *)song_info.album_name, (_packed BYTE *)g_wSongAlbum);
					}

					if (g_wSongGenre[0] != 0)
					{
						TrimPackedString_Extra(g_wSongGenre);
						packed_strcpy((_packed BYTE *)song_info.genre_name, (_packed BYTE *)g_wSongGenre);
					}
					else
					{
						packed_strcpy((_packed BYTE *)song_info.genre_name, genre);
					}


					if 	(g_wSongTitle[0] < 65536)
					{
						TrimUnicodeString_Extra(g_wSongTitle);
						strcpy(song_info.song_name, g_wSongTitle);
                        if (!strlen(song_info.song_name))
                        {
							// if the string length is zero then check if LFN or SFN is present.
                            if (!strlen(song_info.dir_name))
                            {
								packed_strcpy((_packed BYTE *)song_info.song_name, (_packed BYTE *)finddata.name);
                            }
                            else
                            {
								strcpy(song_info.song_name, song_info.dir_name);
								song_info.unicode = song_info.unicode | 0x0000040;
							}
                        }
                        else
                        {
							song_info.unicode = song_info.unicode | 0x0000040;
						}
					}
					else
					{
						TrimPackedString_Extra(g_wSongTitle);
						packed_strcpy((_packed BYTE *)song_info.song_name, (_packed BYTE *)g_wSongTitle);
                        if (!packed_strlen((_packed BYTE *)song_info.song_name))
                        {
							// if the string length is zero then check if LFN or SFN is present.
                            if (!strlen(song_info.dir_name))
                            {
								packed_strcpy((_packed BYTE *)song_info.song_name, (_packed BYTE *)finddata.name);
                            }
                            else
                            {
								strcpy(song_info.song_name, song_info.dir_name);
								song_info.unicode = song_info.unicode | 0x0000040;
							}
						}
					}

					if(g_wSongYear[0] != 0)
					{
						year = 0;
						temp_conversion = g_wSongYear[0];
						for (i=0; i<3; i++)
						{
							year = year + ((temp_conversion & 0x0000ff)-48); //perform hex range shift
							year = year * 10;
							temp_conversion = temp_conversion >> 8;
						}

						temp_conversion = g_wSongYear[1] & 0x0000ff;
						year = year + (temp_conversion-48); //perform hex range shift

						song_info.year = year;
					}
					else
						song_info.year = UNKNOWN_YEAR_CODE;
				}
				g_file_time = 0;
				g_file_time |= g_dirdate.Day;
				g_file_time |= g_dirdate.Year << 12;
				g_file_time |= g_dirdate.Month << 6;
				packed_strcpy((_packed BYTE *)song_info.path_name, g_CurrentSongName);
                // Copy directory path
                song_info.dir_name[0] = '\0';
                packed_strcpy((_packed BYTE *)song_info.dir_name, DirPath);

                song_info.track_number = g_wSongTrackNum;

                g_total_song++;
                iSongCount++;
				if (g_build_voice_file == TRUE)
				{
					if (g_total_song <= MAX_NUM_OF_VOICE)
					ML_AddEntryToVoice(REC_VOICE_TYPE, &song_info,VOICE_FILE_ADD);
                }
				else if (g_build_fm_file == TRUE)
                {
					if (g_total_song <= MAX_NUM_OF_FMREC)
					ML_AddEntryToVoice(REC_FMREC_TYPE, &song_info,VOICE_FILE_ADD);
				}
				else if (g_build_linein_file == TRUE)
				{
					if (g_total_song <= MAX_NUM_OF_PHOTO)
					ML_AddEntryToVoice(REC_PHOTO_TYPE, &song_info,VOICE_FILE_ADD);
				}
				else
                {
					if (g_total_song <= MAX_NUM_OF_SONG)
                    ML_AddEntryToLibrary(drive, &song_info, g_option);
                }
			}

			// once the MMC card couldn't be detected, we will determine that the whole
			// process fail

#ifdef MMC
			if (SD_build_success == TRUE)
			{
				bMMCPresent = MMCDetectPresenceWrapper(MmcMediaInfo.pMMCPhysicalBus);

				if (bMMCPresent != g_bMMCPreviousTest)
					SD_build_success = FALSE; // there is the only point that we will set SD_build_success to false
			}
#endif
		}

		Fclose(iDirHandle);
	}
	return iSongCount;
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
