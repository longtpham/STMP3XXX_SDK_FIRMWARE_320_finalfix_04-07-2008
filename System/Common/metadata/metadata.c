////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2003
//
// File        : metadata.c
// Description : MetaData Extraction Routines
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "stringlib.h"
#include "menumanager.h"
#include "hwequ.h"
#include "project.h"
#include "metadata.h"
#include "FileSystem.h"
#include "resource.h"
#include "dbcs.h"
#include "sysspeed.h"

#include "playlist.h"
#include "mp3metadata.h"
#include "wavmetadata.h"
#include "wmametadata.h"

#ifdef USE_PLAYLIST2
#include "playlist2.h"
#endif

#ifdef SYNC_LYRICS
#include "synclyrics.h"
#endif
#ifdef JPEG_ALBUM_ART
#include "attachedpictureframe.h"
extern BOOL bPicOnlyFlag;
#endif
#ifdef USE_PLAYLIST3
#include "exec.h"
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "sysresource.h"
#include "playlist3.h"
#include "musiclib_ghdr.h"
#include "playerlib.h"
#include "mainmenu.h"
#include "browsemenu.h"
#else
#ifdef  USE_PLAYLIST5
#include "playlist5.h"
#endif
#endif
#ifdef AUDIBLE
#include "resource.h"
#include "sysresources.h"
#include "audiblesongposition.h"
#include "sysequ.h"
#endif

#pragma optimize 1
////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm
#endif

#ifdef USE_PLAYLIST1
extern int g_iInternalTrack;
#endif

#ifdef AUDIBLE
extern _Y int _lc_u_b_AUDIBLE_METADATA_Y[];
extern _Y int _lc_u_e_AUDIBLE_METADATA_Y[];
#endif
////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
INT _reentrant GetFileName(FILE_META_DATA * MetaData,_packed BYTE * pcFilePathName);
#ifdef AUDIBLE
RETCODE _reentrant GetAudibleMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData);
static _reentrant void LoadAudibleMetadataResource(int rsrc_num, int rsrc_type, int rsrc_size, int start_addr, int target_mem);
#endif
////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////
UCS3 g_wSongTitle[TITLE_SIZE];
UCS3 g_wSongArtist[ARTIST_SIZE];
UCS3 g_wSongAlbum[ALBUM_SIZE];
#ifdef USE_PLAYLIST3
UCS3 g_wSongGenre[GENRE_SIZE];
UCS3 g_wSongYear[YEAR_SIZE];
WORD g_wSongTrackNum;
#endif
WORD g_wVBRTOCTable[VBR_TOC_SIZE];
////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void TrimString(UCS3* pString)
//
//   Type:          Function
//
//   Description:   Determines type of file and calls type-specific meta data program.
//
//   Inputs:        UCS3* pString:      pointer to string to trim spaces from
//
//   Outputs:       none
//
//   Notes:         Some ripper applications pad the ID3 tags with spaces.  This
//                  cleans that up.
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant void TrimString(UCS3* pString)
{
    while(pString[strlen(pString)-1]==' ')
        pString[strlen(pString)-1]=0;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetFileMetaData
//
//   Type:          Function
//
//   Description:   Determines type of file and calls type-specific meta data program.
//
//   Inputs:        Ptr to pathname, current device id, meta-data struct location
//
//   Outputs:       RETCODE (0 if MetaData found, -1 if not)
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant GetFileMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA *MetaData)
{
    RETCODE rtn = META_DATA_FOUND;
    WORD wExtension;
    WORD iStrLen;
    INT i;
	int CurrentFolder;

    _packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;
    // check if path + filename will fit in unpacked buffer
    iStrLen= packed_strlen(pcFilePathName);
    if(iStrLen>(MAX_FILENAME_LENGTH-1))
      return META_DATA_NOT_FOUND;

    // clear out the artist field
    if(MetaData->wTitle)
      *MetaData->wTitle = 0;

    if(MetaData->wArtist)
      *MetaData->wArtist = 0;

    // clear out the album field
    if(MetaData->wAlbum)
      *MetaData->wAlbum = 0;

#ifdef USE_PLAYLIST3
    // clear out the genre field
    if(MetaData->wGenre)
      *MetaData->wGenre = 0;

    // clear out the genre field
    if(MetaData->wYear)
      *MetaData->wYear = 0;
#endif
    // Clear the genre byte
    MetaData->btGenre = (BYTE)0;

#ifdef SYNC_LYRICS
	// Init. Lyric's Data Buffer
	SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsInit,0,0,0);
#endif
#ifdef JPEG_ALBUM_ART
	// Initialize the APIC values
	SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicInit, 0, 0, NULL);
#endif

    if(iStrLen > 4)
    {
        // Use C Speed client  Increase call
        // Defines are in SysSpeed.inc.  Sysspeed.h is generated
        SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_METADATA);
        //find the '.'
        while(iStrLen--)
        {
            wExtension = packed_get(pcFilePathName,iStrLen);
            if(wExtension=='.')
                break;
        }
        if(iStrLen)
        {
            for(i=0;i<3;i++)
            {
                packed_set((void*)&wExtension,i,packed_get(pcFilePathName,++iStrLen));
            }
            // Search file path/name for "." -- get the extension
            // Call Decoder Specific MetaData routine for file extension found.
            //pcExtString should point to ".mp3" or ".wma", etc
#ifdef	AUDIBLE
            g_AudibleFlags.bits.IsAudibleFile = FALSE;
#endif
            switch(wExtension)
            {
                case 0x33504d:  //"MP3"
                    rtn = SysCallFunction(RSRC_MP3_METADATA_CODEBANK,GetMp3MetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
#ifdef USE_PLAYLIST3
                    g_unicode = 0;
                    MetaData->dwStartPos = 0;
#endif
                    break;
                case 0x414D57:  //"WMA"
#ifdef USE_PLAYLIST3
#ifdef JPEG_ALBUM_ART
                    bPicOnlyFlag = TRUE;
#endif
#endif
                    rtn = SysCallFunction(RSRC_WMA_METADATA_CODEBANK,GetWmaMetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
#ifdef USE_PLAYLIST3
                    g_unicode = 1;
                    MetaData->dwStartPos = 0;
#endif
                    break;
                case 0x564157:  //"WAV"
                    rtn = SysCallFunction(RSRC_WAV_METADATA_CODEBANK,GetWavMetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
#ifdef USE_PLAYLIST3
                    g_unicode = 1;
                    MetaData->dwStartPos = 0;
#endif
                    break;
#ifdef MOTION_VIDEO
                case 0x564d53:	// "SMV"
                    rtn = SysCallFunction(RSRC_SMV_METADATA_CODEBANK,GetSMVMetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
#ifdef USE_PLAYLIST3
                    g_unicode = 1;
                    MetaData->dwStartPos = 0;
#endif
                    break;
#endif
#ifdef AUDIBLE
				case 0x004141:  //"AA "
					LoadAudibleMetadataResource(RSRC_AUDIBLE_METADATA_Y,
								RSRC_TYPE_DATA,
								(_lc_u_e_AUDIBLE_METADATA_Y-_lc_u_b_AUDIBLE_METADATA_Y)*3,
								(int)_lc_u_b_AUDIBLE_METADATA_Y,
								TARGET_MEM_Y);

                    rtn = SysCallFunction(RSRC_AUDIBLE_METADATA_P,GetAudibleMetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
					if ( rtn == META_DATA_FOUND )
						g_AudibleFlags.bits.IsAudibleFile = TRUE;
					break;
#endif
                default:
                    rtn = META_DATA_NOT_FOUND;
                    break;
            }
        }

        if(!strlen(MetaData->wTitle)){		//if the string length is zero then check if LFN or SFN is present.
//!!HACK!! Following LFN will work only for current track.
//Ideally, a SFN should be passed to playlists and long filename for that file should be returned.
#ifdef USE_PLAYLIST1
        	if(SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_LFNGetFileName,g_iInternalTrack,0,(void*)MetaData->wTitle) >0 )	 //is there a LFN
#else
#ifdef USE_PLAYLIST2
			if(SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetLFN,TYPE_FILE,(INT) g_CurrentTrack.pFileEntry,(INT*)MetaData->wTitle) == PLAYLIST_SUCCESS)	 //is there a LFN
#else
#ifdef USE_PLAYLIST3
			if (1)
#else
#ifdef USE_PLAYLIST5
            {
                PL5_PL_QITEM * tmp;
                int recordnum;
                LONG fastkey;

                tmp = g_PL5_PL_queue.pos + g_PL5_Playback.currentplay;
                recordnum = tmp->file.iRecordNum;
                fastkey = (tmp->folder + tmp->depth - 1)->record.lFastKey;

                if(SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetLFN,(INT)(&fastkey),
                    recordnum,(INT*)MetaData->wTitle) == PLAYLIST_SUCCESS)      // is there a LFN
#endif
#endif
#endif
#endif	// #ifdef USE_PLAYLIST1
			{
			}
            else	//no long file name so we must diplay the short one
			{
            	GetFileName(MetaData,pcFilePathName);
#ifdef USE_PLAYLIST3
            	g_unicode = 1;
#endif
        	}
#ifdef USE_PLAYLIST5        	
        }
#endif        
        }
        TrimString(MetaData->wArtist);
        TrimString(MetaData->wTitle);
        TrimString(MetaData->wAlbum);
#ifdef USE_PLAYLIST3
        TrimString(MetaData->wGenre);
        TrimString(MetaData->wYear);
#endif
        // C Speed client free call
        SysSpeedClockFree(SPEED_CLIENT_METADATA); // free the shared speed resource

    }
	else
		rtn = META_DATA_NOT_FOUND;
    return rtn;
}


///////////////////////////////////////////////////////////////////////////////
//> Name:        GetFileName
//
//  Type:        Function
//
//  Description: Used when no ID3v1 tag is available. Clears title, artist, album, genre.
//               Fills in the title field with the filename.
//
//  Inputs:      Pointer to meta-data struct, pointer to path and filename.
//  Outputs:     0 if Success, -1 if Failure.
//
//<
/////////////////////////////////////////////////////////////////////////////////
INT _reentrant GetFileName(FILE_META_DATA * MetaData,_packed BYTE * pcFilePathName)
{
    INT  i;
    INT  iLength;
    BYTE wChar;
    BOOL bFound = FALSE;
    UCS3 * pcFileName;
    UCS3 * pcStringPtr;
    _packed BYTE btPackedFileName[8+1+3+1];//8.3 plus null.

    //find the last '/'
    for(i=packed_strlen(pcFilePathName);i>=0;i--)
    {//start at the end, go backward until the char is '/'
        if(packed_get(pcFilePathName,i)==0x2f)
            break;
    }

    //now go forward until we hit the terminating NULL, copying each byte
    iLength = 0;

    i = i+1;	// Don't display the '/'.

    while((wChar=packed_get(pcFilePathName,i++)))
    {
        packed_set(btPackedFileName,iLength++,wChar);
    }
    packed_set(btPackedFileName,iLength++,0);

    if(MetaData->wTitle)
        DBCSToUnicode(btPackedFileName,MetaData->wTitle,MetaData->iTitleBufferLength);

    return(SUCCESS);
}
#ifdef USE_PLAYLIST3
INT _reentrant RetrieveSongInfo(INT a, INT b, INT *p_path)
{
	int j;
	for (j=0; j<(MAX_DIRNAME_LENGTH/3); j++)
	{
		p_path[j] = song_info.path_name[j] & 0xffffff;
	}
	p_path[(MAX_DIRNAME_LENGTH/3)-1] = 0;
	g_CurrentSongFastkey=song_info.g_songFastKey;
	if (song_info.unicode & BITMASK_SONG)
	{
		for (j=0; j<TITLE_SIZE; j++)
			g_wSongTitle[j] = song_info.song_name[j] & 0xffffff;
	}
	else
	{
		DBCSToUnicode((_packed BYTE *)song_info.song_name, g_wSongTitle, TITLE_SIZE-1);
	}
	g_wSongTitle[TITLE_SIZE-1] = 0;

	if (song_info.unicode & BITMASK_ARTIST)
	{
		for (j=0; j<ARTIST_SIZE; j++)
			g_wSongArtist[j] = song_info.artist_name[j] & 0xffffff;
	}
	else
	{
		DBCSToUnicode((_packed BYTE *)song_info.artist_name, g_wSongArtist, ARTIST_SIZE-1);
	}
	g_wSongArtist[ARTIST_SIZE-1] = 0;

	if (song_info.unicode & BITMASK_ALBUM)
	{
		for (j=0; j<ALBUM_SIZE; j++)
			g_wSongAlbum[j] = song_info.album_name[j] & 0xffffff;
	}
	else
	{
		DBCSToUnicode((_packed BYTE *)song_info.album_name, g_wSongAlbum, ALBUM_SIZE-1);
	}
	g_wSongAlbum[ALBUM_SIZE-1] = 0;

	return 0;
}

INT _reentrant GetRelativePath(int highlight_index, int browse_type, INT* PackedPathNameAddress)
{
	//if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
	if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO))
	{
		SysPostMessage(5,MUSICLIB_START_PLAY_SONG, highlight_index, &g_current_size, &g_current_index);
		bPlayVoice = FALSE;
	}
	else
	{
		SysPostMessage(5,MUSICLIB_START_PLAY_VOICE, highlight_index, &g_current_size, &g_current_index);
		bPlayVoice = TRUE;
	}
	SysWaitOnEvent(0,0,0);
    RetrieveSongInfo(0,0,PackedPathNameAddress);
	return 0;
}
#endif

#ifdef AUDIBLE
static _reentrant void LoadAudibleMetadataResource(int rsrc_num, int rsrc_type, int rsrc_size, int start_addr, int target_mem) {
//caution : do not put anything before this #pragma block or else some registers will get trashed.
	#pragma asm
		extern	SysLoadResource
		movec SSH,y:(r7)+
		move x0,y1
		move a1,x0
		move x1,a1
		move b1,x1
		move y0,y0
		jsr	SysLoadResource
		movec y:-(r7),SSH
		nop
	#pragma endasm
}
#endif
