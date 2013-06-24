////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Sigmatel Inc 2005-2007
// Filename: playlist3init.c
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

///////////////Extern FAT32 functions/////////////////////////////////
extern _reentrant INT Chdir(_packed char *filepath);
extern _reentrant INT FindFirst(Finddata *_finddata,_packed char *FileName);
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern _reentrant INT Fclose(INT HandleNumber);
extern _reentrant INT GetShortfilename(LONG Key,INT *Buffer);
extern _reentrant LONG GetRootdirkey(INT DeviceNumber);
extern _reentrant INT FastOpen(LONG Key,_packed char *mode);
extern _reentrant INT ConstructLongFileName(INT HandleNumber, INT RecordNumber, INT *LFNBuffer);
extern _reentrant INT ChdirFromOffset(_packed char *filepath,INT RecordNumber) ;
extern _reentrant INT GetCWDHandle(void);
extern _reentrant INT ConstructLongFileName(INT HandleNumber, INT RecordNumber, INT *LFNBuffer);
extern _reentrant INT GetChar(_packed char *Buffer,INT *offset);
extern _reentrant void PutChar(_packed char *Buffer,INT *offset,INT Char);
extern _reentrant int write_detection_access(int option);		//added by samuel on 31/1/05 for flash write detection
#ifdef JANUS_BUILD
extern _reentrant RETCODE Fast_Playlist_GetLFN(INT iEntryType,INT pEntry,INT* pName);
#endif
extern _reentrant INT SaveSortedRecords(void);
extern _reentrant INT LoadSortedRecords(void);
extern INT DriveLetter[];
//////////////////////////////////////////////////////////////////////
int g_iTotalDir=0;
int g_iTotalFiles=0;
int g_iNumStoredSFN = 0;
extern FILESPEC         FileSpec;

extern UCS3 g_wSongGenre[];
extern UCS3 g_wSongYear[];
extern BOOL g_bMMCPreviousTest;
extern _packed BYTE g_shortname[MAX_FILESNAME];


_asmfunc MMCDetectPresenceWrapper(void*);
_reentrant void Rebuild_GetFileMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA *MetaData);
_reentrant INT AddSongToLibrary(int a, int drive, void* b);
_reentrant INT AddSingleFolderSongToLibrary(INT a, INT drive, void * b);
_reentrant void Rebuild_GetMetaData(WORD wMode,int ignored1,int*ignored2);
#ifdef JANUS_BUILD
_reentrant INT FastAddSongToLib(int iDevice);
#endif

DWORD g_dwFastKey = 0;
DWORD g_FileKey = 0;
int g_iRecordNum = 0;
BOOL g_Rebuild = FALSE;
int g_unicode = 0;
int g_total_song = 0;
RAM_SONG_INFO_T song_info;
DIR_DATE g_dirdate;
DIR_TIME g_dirtime;
int g_file_time;

////////////////////////////////////////////////////////////////
// Enable sort by track number under album level (PLAYLIST3 only)
//      0:  disble
//      1:  enable
BOOL g_SortByTracknum = TRUE;
////////////////////////////////////////////////////////////////

int g_option = ADD_OPTION_MUSIC;
BOOL SD_state_change = FALSE;
BOOL SD_build_success = FALSE;
_packed BYTE DirPath[MAX_DIRNAME_LENGTH];
extern uint24 g_is_SD_inserted;
extern BOOL bResume;
extern BOOL bSystemInit;
int g_iTotalPath;

//added by Raymond for CR5xx
BOOL g_build_voice_file = FALSE;
BOOL g_build_fm_file = FALSE;
BOOL g_build_linein_file = FALSE;
DWORD g_dwVoiceFastKey = 0;
DWORD g_dwFmFastKey = 0;
DWORD g_dwLineinFastKey = 0;
_packed BYTE g_MusicAdd_FilePath[] = "/:ASUMACI.DDTAD\0\0\0"; //open file on a:\musicadd.dat

_packed BYTE g_recordfile_path[12];
int g_iHighestVoiceNumber = 0;
int g_iHighestFMNumber = 0;
int g_iHighestLineNumber = 0;
int* pHighestNumber;
extern _packed BYTE g_RecordDirectory[];
Finddata _X g_AddToLibraryData[FINDDATA_CACHE_SIZE];
int _X g_NumAddToLibrary = 0;

// Forward declarations
// Ken 2005/7/20 12:09
void _reentrant ML_building_engine_init_playlist3(void);
void _reentrant ML_voice_build_engine_init_playlist3(void);
void _reentrant ML_merging_engine_init_playlist3(void);
_reentrant void ML_BuildingEngineInitPlaylist3For1stPass(void);
_reentrant void ML_BuildingEngineInitPlaylist3For2ndPass(void);
// Ken 2005/7/20 12:09
// Ken 2005/7/15 04:44
#define PATHARRAYSLOT_BYTELEN 15
#define PATHARRAYSLOT_WORDLEN PATHARRAYSLOT_BYTELEN/3
_packed BYTE TempPathArray[_MAX_DIR_DEPTH*PATHARRAYSLOT_BYTELEN];
_packed BYTE TempPath[MAX_DIRNAME_LENGTH];

#define	FAST_KEY_BIT_POS_VOICE	0
#define FAST_KEY_BIT_POS_FM		1
#define FAST_KEY_BIT_POS_LINEIN	2

//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant PathFormationPartial(_packed BYTE* dst, _packed BYTE* src)
//
//   Type:          Function
//
//   Description:   Forming a path from the TempPathArray according to the Depth
//
//   Inputs:        dst - Pointer to destination of path
//   		    src - Pointer to the source path array
//		    iDepth - Depth of path to build
//
//   Outputs:       None
//
//   Notes:         It's assumed that the directory separator is already appended in each slot of the array
//<
//////////////////////////////////////////////////////
INT _reentrant PathFormationPartial(_packed BYTE* dst, _packed BYTE* src, int iDepth)
{
    // Initialize the destination buffer
    *((WORD*)dst) = 0x000000;
    packed_strcat(dst,(src+(iDepth*PATHARRAYSLOT_WORDLEN)));

    return 0;
}

//////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant PathFormation(_packed BYTE* dst, _packed BYTE* src, int iDepth)
//
//   Type:          Function
//
//   Description:   Forming a path from the TempPathArray according to the Depth
//
//   Inputs:        dst - Pointer to destination of path
//   		    src - Pointer to the source path array
//		    iDepth - Depth of path to build
//
//   Outputs:       None
//
//   Notes:         It's assumed that the directory separator is already appended in each slot of the array
//<
//////////////////////////////////////////////////////
INT _reentrant PathFormation(_packed BYTE* dst, _packed BYTE* src, int iDepth)
{
	int i;

	// Initialize the destination buffer
	*((WORD*)dst) = 0x000000;
	for(i=0;i<iDepth;i++)
	{
		packed_strcat(dst,(src+(i*PATHARRAYSLOT_WORDLEN)));
	}

	return 0;
}

_reentrant BOOL IsMusicPath (_packed BYTE * path)
{
	INT i,j;
	INT tmpchar;
	_packed BYTE szVoice[12];
	_packed BYTE szFM[6];
	_packed BYTE szLinein[12];
	_packed BYTE tmpstr[12];

	((int*)szVoice)[0] = VOICE_PATH_1;  //VOICE/
	((int*)szVoice)[1] = VOICE_PATH_2;
	((int*)szVoice)[2] = VOICE_PATH_3;

	((int*)szFM)[0] = FM_PATH_1;        //FM/
	((int*)szFM)[1] = FM_PATH_2;

	((int*)szLinein)[0] = LINEIN_PATH_1;    //LINE-IN/
	((int*)szLinein)[1] = LINEIN_PATH_2;
	((int*)szLinein)[2] = LINEIN_PATH_3;

    i=0;j=0;
	while ((tmpchar = GetChar((_packed char*)path, &i)) != 0)
	{
        if (i>3)
        {
            PutChar((_packed char*)tmpstr,&j,tmpchar);
            if (tmpchar == 0x2f)
                break;
        }
    }
    PutChar((_packed char*)tmpstr,&j,0);

    if((packed_strcmp(tmpstr,szVoice) == 0) || (packed_strcmp(tmpstr,szFM) == 0) || (packed_strcmp(tmpstr,szLinein) == 0))
        return FALSE;

    return TRUE;
}


//////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant Playlist_BuildMusicLib(INT iIgnored1, INT iIgnored2, INT* pIgnored)
//
//   Type:          Function
//
//   Description:   Calling Playlist_PopulateMusicLib(void) to build the
//		    music library
//
//   Inputs:
//
//   Outputs:
//
//   Notes:
//<
//////////////////////////////////////////////////////
INT _reentrant Playlist_BuildMusicLib(INT iIgnored1, INT iIgnored2, INT* pIgnored)
{
	int i;
	int iDepth=0;
	DWORD dwFastKey;
	BOOL Build_Flash = FALSE;
	INT Hash_Result = -1;
#ifdef JANUS_BUILD
	RETCODE returnCode;
	BOOL bBitResult = FALSE;
    //_packed BYTE g_MusicAdd_FilePath[] = "/:ASUMACI.DDTAD\0\0\0"; //open file on a:\musicadd.dat


	returnCode = ReadStickyBit((volatile _X WORD *)&HW_RTC_SKIP_REBUILD_REG,
		HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS,
		&bBitResult);
#endif

	// Use C Speed client  Increase call. Defines are in SysSpeed.inc. Sysspeed.h is generated
    //increased the speed to speed_peak(75 mhz) for building the music lib.
    SysSpeedIncrease(SPEED_PEAK, SPEED_CLIENT_PLAYLIST1);

	// Make sure that the other message queues are cleared before proceeding to
	//  music library rebuild, because code is supposed to be overwriting the
	//  same area as the decoder/encoder processes.
	SysWaitOnEvent(0,0,0);

	// Loading music library building engine and FAT write functions
	ML_building_engine_init_playlist3();
	SysLoadFATWrite();
	ML_InitLibraryParameter();

    // for clear screen before rendering message
    SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);

	////// Let's build the music library for files on FLASH first
	// do once only during system bootup
	if (!bSystemInit)
	{
		// Show a message
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_FLASH1);
		SysWaitOnEvent(0,0,100);

		ML_LoadOnTheGo(TRUE);

		ML_ChOnTheGo(MEDIA_TYPE_FLASH, TRUE);

#ifdef JANUS_BUILD
		//detect the write occurred thru MTP before
        if ( (bBitResult == TRUE) && (returnCode == SUCCESS) && !write_detection_access(0))
        {
            //clear the sticky bit at the start of the incremental build.
            //so if user inserts the USB while building the incremental build ,it rebuilds the whole music library next time.
    	    ClearStickyBit((volatile _X WORD *)&HW_RTC_SKIP_REBUILD_REG,
				HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS);

			returnCode = FastAddSongToLib(0);

			//set  the sticky bit indicate that Incremental build is successfull and next time we can again go for incremental build.

			SetStickyBit((volatile _X WORD *)&HW_RTC_SKIP_REBUILD_REG,
				HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS);


        }
#endif  // JANUS_BUILD

		//detect the write occurred before
		if(write_detection_access(0)
#ifdef JANUS_BUILD
			|| (returnCode != SUCCESS) || (bBitResult == FALSE)
#endif
		  )
		{
#ifdef JANUS_BUILD
            FSFileAttributeClear(ATTR_SYSTEM|ATTR_HIDDEN,g_MusicAdd_FilePath,0);
            Fremove((_packed char *)g_MusicAdd_FilePath);
#endif
			// Show a message
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_FLASH2);
			SysWaitOnEvent(0,0,100);
			bResume = FALSE;

            // Clear on the go list
#ifdef MMC
			ML_ChOnTheGo(MEDIA_TYPE_FLASH, INDEX_EOF);
			// Initialize the *TempPathArray to the root string
			Playlist_GetRootString(TempPathArray,0);
#else
			ML_ChOnTheGo(MEDIA_TYPE_FLASH, INDEX_EOF);
			// Initialize the *TempPathArray to the root string
			// To save space, hardcode it instead
			((int*)TempPathArray)[0]=0x2F3A61;	// "/:A"
			((int*)TempPathArray)[1]=0x000000;	// Terminating zero
#endif

            // Load intermediate memory for 1st pass
            ML_BuildingEngineInitPlaylist3For1stPass();

			// Get the fast key of root directory
			dwFastKey=GetRootdirkey(0);
			// Call Playlist_PopulateMusicLib() to add files in sub-folders to music library recursively

#ifdef _FOLDER_BROWSE_
						g_tDirInfoBuffer[iDepth].iDirRecord = 0;
						g_tDirInfoBuffer[iDepth].dwFastKey = dwFastKey;
						g_tDirInfoBuffer[iDepth].u4Added = FALSE;
#endif  // _FOLDER_BROWSE_

			g_total_song = 0;
            Playlist_PopulateMusicLib(0,iDepth,dwFastKey,0);
			// Showing a message
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_FLASH3);
			SysWaitOnEvent(0,0,0);
            // Flush the updated music library and sorted records onto flash
            SaveSortedRecords();

            // Flush sorted list
            ML_FlushSortedListToFlash(MEDIA_TYPE_FLASH);

            // Load intermediate memory for 2nd pass
            ML_BuildingEngineInitPlaylist3For2ndPass();

            // Flush the updated music library to flash
			SysCallFunction(RSRC_MUSICLIB_BUILD_FUNCTION_FLUSH,ML_FlushLibraryToFlash,MEDIA_TYPE_FLASH,0,NULL);

			//clear the write detection so that next time we can detect it again
			write_detection_access(4);
			write_detection_access(2);

#ifdef JANUS_BUILD
			SetStickyBit((volatile _X WORD *)&HW_RTC_SKIP_REBUILD_REG,
				HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS);
#endif

			Build_Flash = TRUE;
		}
		else
		{
			// Retrieve voice, fm and line-in directory fast key
			GetVoiceFastKey(((1<<FAST_KEY_BIT_POS_VOICE) | (1<<FAST_KEY_BIT_POS_FM) | (1<<FAST_KEY_BIT_POS_LINEIN)));
		}
	}
	else // (!bSystemInit)
	{
		ML_LoadOnTheGo(FALSE);
#ifdef MMC
		ML_ChOnTheGo(MEDIA_TYPE_FLASH, TRUE);
#else
		ML_ChOnTheGo(MEDIA_TYPE_FLASH, TRUE);
#endif
	}

	//disable the write detection because the MMC database want to rebuild and write to flash
	write_detection_access(3);

	////// Building the music library for files on FLASH first completed upon reaching this point
	////// Let's build the music library for files on MMC now, if there is any
#ifdef MMC
	if (g_is_SD_inserted == HAS_SD)
	{
		ML_ChOnTheGo(MEDIA_TYPE_SD, TRUE);

		SD_build_success = TRUE;
		g_wCurrentDirDevId = 1;

		// Showing a message
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_SD1);
		SysWaitOnEvent(0,0,100);

		// Comparing the hash value stored in music library with that computed from the current SD
		Hash_Result = SysCallFunction(RSRC_SDMD_MENU_CODE_BANK,sdmd_CompareHash,g_wCurrentDirDevId,0,0);

		// If the hash values doesn't match, start a rebuild for files on SD
		if(Hash_Result == 1)
		{
			// Showing a message
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_SD2);
			SysWaitOnEvent(0,0,100);


			ML_InitLibraryParameter();
			ML_ChOnTheGo(MEDIA_TYPE_SD, INDEX_EOF);

            // Load intermediate memory for 1st pass
            ML_BuildingEngineInitPlaylist3For1stPass();

			// Initialize the *TempPathArray to the root string
			Playlist_GetRootString(TempPathArray,1);
			// Get the fast key of root directory
			dwFastKey=GetRootdirkey(1);
#ifdef _FOLDER_BROWSE_
			g_tDirInfoBuffer[iDepth].dwFastKey = dwFastKey;
#endif  // _FOLDER_BROWSE_
			// Call Playlist_PopulateMusicLib() to add files in sub-folders to music library recursively
			g_total_song = 0;
            Playlist_PopulateMusicLib(1,iDepth,dwFastKey,0);

			SysWaitOnEvent(0,0,50);
			// should add some delay here to make sure if the removal of card is too fast that the add
			// songto library quit but the SD_state_change have not updated to FALSE due to that the
			// button_isr has not yet served.

			if ((SD_state_change == FALSE) && (SD_build_success == TRUE))
			{
				SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_SD3);
				SysWaitOnEvent(0,0,0);

                // Flush sorted list
                ML_FlushSortedListToFlash(MEDIA_TYPE_SD);

                // Load intermediate memory for 2nd pass
                ML_BuildingEngineInitPlaylist3For2ndPass();

				SysCallFunction(RSRC_MUSICLIB_BUILD_FUNCTION_FLUSH,ML_FlushLibraryToFlash,MEDIA_TYPE_SD,0,NULL);
				SysCallFunction(RSRC_SDMD_MENU_CODE_BANK,sdmd_UpdateHash,g_wCurrentDirDevId,0,0);
			}
			else
			{
				SD_build_success = FALSE;
			}

			//end of building SD card
		}
		else if (Hash_Result != 0)
		{
			SD_build_success = FALSE;
		}
	}
	else
	{
		ML_ChOnTheGo(MEDIA_TYPE_SD, INDEX_EOF);
	}
#endif

	// Update the new music and on-the-go lists according to the new libraries
	if ((!bSystemInit) || (g_is_SD_inserted == NO_SD) || (SD_build_success))
	{
		if (!SD_build_success)
			g_is_SD_inserted = NO_SD;
		// show create new music screen
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_BUILD_NEWMUSIC);
		ML_UpdateNewMusic();
		ML_UpdateOnTheGo();
		ML_SaveOnTheGo();
	}

	// If VOICE folder is found, build the library
	BuildVoiceFilePath();
	// Build the library for VOICE completed

	// If FM folder is found, build the library
	BuildFMFilePath();
	// Build the library for FM completed

    // If LINE-IN folder is found, build the library
    BuildLINEINFilePath();
    // Build the library for LINEIN completed
#ifdef MMC
	////// Building the music library for files on MMC completed upon reaching this point

	if ((SD_state_change == FALSE) && (SD_build_success == TRUE))
	{
		ML_merging_engine_init_playlist3();
		ML_MergeLibraryTables();
	}
#endif

	// Rebuild completed
	g_Rebuild = FALSE;

	// Clean up the mess before leaving
	SysUnLoadFATWrite();

	//SET SPEED BACK TO NORMAL HERE
	SysSpeedClockFree(SPEED_CLIENT_PLAYLIST1); // C Speed client frees the shared speed resource
	return PLAYLIST_SUCCESS;
}

//////////////////////////////////////////////////////
//
//>  Name:          _reentrant RETCODE Playlist_PopulateMusicLib(int iDevice, int iDepth, DWORD dwDirKey,INT RecordNumber,INT FindNextRecord)
//
//   Type:          Function
//
//   Description:   recursively populates the Music Library starting from the root directory of the device
//
//   Inputs:        int iDevice        The number of the device being scanned
//                  int iDepth           Used to limit the depth of sub-folder to be added to music library
//
//   Outputs:       if the input has any contents, they will be added to the music library with function
//		    AddSongToMusicLibrary
//
//   Notes:
//	The process in original Playlist_PopulateMusicLib has been divided into three parts 
//	New version of Playlist_PopulateMusicLib
//		- located in this c file 
//		- search file through folder structure and copy the finddata into the cache
//		- Call AddSongToLibrary in MP3 metadata codebank if cache is full or change processing to another folder
//	AddSongToLibrary in the beginning of MP3, WMA and WAV metadata codebank 
//		- located in playlist3init_metadata.c
//		- detect file type and select correct metadata codebank
//		- if correct is selected in previous file, no codebank switch will be performed.
//	AddSongToLibraryStatic in P App cluster
//		- located in playlist3init_static.c
//		- Call metadata extraction in menu codebank
//		- Add file into music library
//<
//////////////////////////////////////////////////////
_reentrant RETCODE Playlist_PopulateMusicLib(int iDevice, int iDepth, DWORD dwDirKey,INT DirRecordNumber)
{
	RETCODE rtn = PLAYLIST_FILE_ERROR;
	int RecordNo=0;
	DWORD dwFastKey;
	int b;

	// Preparing the fast key and DirPath[] for the music library building phase
	g_dwFastKey=dwDirKey;
    PathFormation(DirPath,TempPathArray,iDepth+1);
    //build the relative path instead of absolute path
    PathFormationPartial(TempPath,TempPathArray,iDepth);

	if( iDepth == 1 )
	{
#if 1
        if (!IsMusicPath(DirPath)) 
        	return PLAYLIST_SUCCESS;
#else		
		if( ((INT *)DirPath)[1] == VOICE_PATH_1 &&
			((INT *)DirPath)[2] == VOICE_PATH_2 &&
			(((INT *)DirPath)[3]&0xff0000) == VOICE_PATH_3 )
		{
			g_dwVoiceFastKey = dwDirKey;
			return PLAYLIST_SUCCESS;
		}
		if( ((INT *)DirPath)[1] == FM_PATH_1 &&
			(((INT *)DirPath)[2]&0xff0000) == FM_PATH_2 )
		{
			g_dwFmFastKey = dwDirKey;
			return PLAYLIST_SUCCESS;
		}
		if( ((INT *)DirPath)[1] == LINEIN_PATH_1 &&
			((INT *)DirPath)[2] == LINEIN_PATH_2 &&
			((INT *)DirPath)[3] == LINEIN_PATH_3 )
		{
			g_dwLineinFastKey = dwDirKey;
			return PLAYLIST_SUCCESS;
		}
#endif		
	}

	if(ChdirFromOffset((_packed char*)TempPath,DirRecordNumber) == FS_SUCCESS)
	{
		_packed BYTE sFileSpec[4];
		Finddata finddata;
		int i;
		int find_handle ;
		((int*)(sFileSpec))[0]=0x2a2e2a; //*.*
		((int*)(sFileSpec))[1]=0; //null terminator
		_memset(&finddata,0,sizeof(finddata));
		finddata.device = iDevice;
		finddata.startrecord = 0;

		// Get the first that matches our specifications
		find_handle = FindFirst(&finddata,(_packed char*) sFileSpec);
		RecordNo=finddata.startrecord-1;

		while(find_handle > 0)
		{
			if((!(finddata.attrib & ATTR_UNACCEPTABLE)))
			{
				if(finddata.attrib & (ATTR_DIRECTORY))
				{
					if((iDepth+1) < _MAX_DIR_DEPTH)
					{
						RecordNo = finddata.startrecord - 1;
						dwFastKey = finddata.Key;

			      		// Jot down the folder name for further exploration or library building
                        packed_strcpy((TempPathArray+((iDepth+1)*PATHARRAYSLOT_WORDLEN)), (_packed BYTE*) finddata.name);
					    b = DIR_SEPARATOR ;	// "\" Never ever forget to do this
                        packed_strcat((TempPathArray+((iDepth+1)*PATHARRAYSLOT_WORDLEN)), (_packed BYTE*) &b);
  						Fclose(find_handle);
						find_handle = -1;

#ifdef _FOLDER_BROWSE_
						g_tDirInfoBuffer[iDepth+1].iDirRecord = RecordNo;
						g_tDirInfoBuffer[iDepth+1].dwFastKey = dwFastKey;
						g_tDirInfoBuffer[iDepth+1].u4Added = FALSE;
#endif  // _FOLDER_BROWSE_

                        //Process the files found in the current before go into subdirectory
				        if( g_NumAddToLibrary != 0 )
						{
#ifdef _FOLDER_BROWSE_
							if( g_tDirInfoBuffer[iDepth].u4Added == FALSE )
								AddDirToLibrary( iDevice, iDepth );
#endif  // _FOLDER_BROWSE_
				        	SysCallFunction(RSRC_MP3_METADATA_CODEBANK,AddSongToLibrary,0,iDevice,0);
						}
				        	
					    Playlist_PopulateMusicLib(iDevice,iDepth+1,dwFastKey,RecordNo);

						// Preparing the fast key and DirPath[] for the music library building phase
						g_dwFastKey=dwDirKey;
						PathFormation(DirPath,TempPathArray,iDepth+1);

						finddata.startrecord;
						find_handle = FindFirst(&finddata,(_packed char*) sFileSpec);
						continue;
					}
				}
				else
				{
					//file
					if(Playlist_ValidateEntry(TYPE_FILE, IS_VALID_AUDIO, (INT *)&finddata))
					{
						//backup the finddata into the cache
						int j;
						WORD *src = (WORD*)&finddata;
						WORD _X * dst = (WORD _X *)&g_AddToLibraryData[g_NumAddToLibrary];
						
						for( j = 0; j < (int)sizeof(Finddata); j++ )
							*dst++ = *src++;
						g_NumAddToLibrary++;
					}
					//cache is full, process the files found in the current folder
					if( g_NumAddToLibrary >= FINDDATA_CACHE_SIZE )
					{
#ifdef _FOLDER_BROWSE_
						if( g_tDirInfoBuffer[iDepth].u4Added == FALSE )
							AddDirToLibrary( iDevice, iDepth );
#endif  // _FOLDER_BROWSE_
			            SysCallFunction(RSRC_MP3_METADATA_CODEBANK,AddSongToLibrary,0,iDevice,0);
					}
				}
			}

			if((FindNext(find_handle,&finddata)<0))
			{
				//if there's no next entry, lets close it out.
				Fclose(find_handle);
				find_handle = -1;
				rtn = PLAYLIST_SUCCESS;
			}
		}
        //Process the files found in the current before go into subdirectory
        if( g_NumAddToLibrary != 0 )
		{
#ifdef _FOLDER_BROWSE_
			if( g_tDirInfoBuffer[iDepth].u4Added == FALSE )
				AddDirToLibrary( iDevice, iDepth );
#endif  // _FOLDER_BROWSE_
            SysCallFunction(RSRC_MP3_METADATA_CODEBANK,AddSongToLibrary,0,iDevice,0);
		}

		((int*)TempPath)[0]=0x002e2e;//change dir to one level up(..).
		Chdir((_packed char*)TempPath);
	}
	return rtn;
}
// Ken 2005/7/15 04:44

//////////////////////////////////////////////////////
//
//>  Name:          BOOL _reentrant Playlist_IsValidAudioFile(LONG lFileSize, _packed BYTE* pFileName)
//
//   Type:          Function
//
//   Description:   Determine if the file is an audio file
//
//   Inputs:        LONG File Size; _packed BYTE* File Name String
//
//   Outputs:       BOOL True/False
//
//   Notes: 	 Function will check for a valid file extension and will
//				 check for MAC resources
//<
//////////////////////////////////////////////////////
BOOL _reentrant Playlist_IsValidAudioFile(LONG lFileSize, _packed BYTE* pFileName)
{
	BOOL rtn = FALSE;
	WORD wFileExtension;

	//Get the file extension
	wFileExtension = Playlist_GetFileExtention(0,0,(INT*)pFileName);

	//Check the extension for a valid case
	if((wFileExtension==MP3_FILE_EXT) ||
		(wFileExtension==WMA_FILE_EXT) ||
#ifdef AUDIBLE
		(wFileExtension==WAV_FILE_EXT) ||
		(wFileExtension==AUDIBLE_FILE_EXT))
#else
		(wFileExtension==WAV_FILE_EXT))
#endif
    {
        //Valid Extension
		rtn = TRUE;
	}
	//Check for MAC Resource
	if (packed_get(pFileName,0) == '_')
    {
        // first character is _, so check size
		if (lFileSize <= MAC_RESOURCE_NUM_BYTES)
        {
            // if <= 1kB, remove from playlist
			rtn = FALSE;
		}
	}

	return rtn;
}

//////////////////////////////////////////////////////
//
//>  Name:          WORD _reentrant Playlist_GetFileExtention(_packed BYTE* pFileName)
//
//   Type:          Function
//
//   Description:   Finds the file extension
//
//   Inputs:        _packed BYTE*   File Name String
//
//   Outputs:       WORD   File Extension
//
//   Notes: 	Routine
//				1 - Find length of file
//				2 - Find the "." by traversing backwards from the end of the string
//				3 - Fill File Extention string
//				4 - Return the file extention as a word
//<
//////////////////////////////////////////////////////
_reentrant INT Playlist_GetFileExtention(INT iIgnored1, INT iIgnored2, INT* pPtr)
{
	WORD wFileExtension;
	WORD wStrLen;
	int i;
	_packed BYTE* pFileName = (_packed BYTE*) pPtr;

	//Get length of file name
	wStrLen= packed_strlen(pFileName);

	//Check File Length, return error if too long
	if(wStrLen>(MAX_FILENAME_LENGTH-1))
		return META_DATA_FILE_NOT_SUPPORTED;

	//Find the "."; Traverse backwards from EOF
	while(wStrLen--)
	{
		wFileExtension = packed_get(pFileName,wStrLen);
		if(wFileExtension=='.')
			break;
	}

	//if the '.' could not be found return an error; else fill up wFileExtension
	if(!wStrLen)
		return META_DATA_FILE_NOT_SUPPORTED;
	else
	{
		//Pack 1st 3 characters after '.' for the extension
		for(i=0;i<3;i++)
		{
			packed_set((void*)&wFileExtension,i,packed_get(pFileName,++wStrLen));
		}
	}

	//If we got here, a valid file extension was found.
	return wFileExtension;
}

/////////////////////////////////////////////////////////
//playlist3 helper functions
/////////////////////////////////////////////////////////
BOOL _reentrant Playlist_ValidateEntry(INT iEntryType,INT iReason, INT* pPtr)
{
	Finddata* pFinddata;
	BOOL rtn = FALSE;
	pFinddata = (Finddata*) pPtr;
	switch(iEntryType)
	{
	case TYPE_FILE:
		switch(iReason)
		{
		case IS_VALID_AUDIO:
			if(Playlist_IsValidAudioFile(pFinddata->FileSize,(_packed BYTE*)pFinddata->name))
				rtn = TRUE;
			break;
		}
		break;
	}
	return rtn;
}


_reentrant void TrimUnicodeString(UCS3* pString)
{
	int i;
	i = strlen(pString)-1;
	while((i>=0) && (pString[i]==' '))
	{
		pString[i]=0;
		i--;
	}
}

_reentrant void TrimPackedString(UCS3* pString)
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


void _reentrant ML_building_engine_init_playlist3(void)
{
#pragma asm
	include "resource.inc"
	org     p,".ptextplaylist3init":
	extern	F_lc_u_e_MusicLibBuild_P
	extern	F_lc_u_b_MusicLibBuild_P
	extern	SysLoadResource

	; Load the MusicLib building engine into P memory
	move    #>RSRC_MUSICLIB_BUILD_MODULE,x0                      ; Resource number
	move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
	move    #>(F_lc_u_e_MusicLibBuild_P-F_lc_u_b_MusicLibBuild_P)*3,y1 ; overlay area sz
	move    #>F_lc_u_b_MusicLibBuild_P,y0                      ; Address of the overlay area
	move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
	jsr     SysLoadResource


#pragma endasm
}

void _reentrant ML_voice_build_engine_init_playlist3(void)
{
#pragma asm
	include "resource.inc"
	org     p,".ptextplaylist3init":
	extern	F_lc_u_e_MusicLibVoice_P
	extern	F_lc_u_b_MusicLibVoice_P
	extern	SysLoadResource

	; Load the MusicLib voice build engine into P memory
	move    #>RSRC_MUSICLIB_VOICE_APP,x0                      ; Resource number
	move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
	move    #>(F_lc_u_e_MusicLibVoice_P-F_lc_u_b_MusicLibVoice_P)*3,y1 ; overlay area sz
	move    #>F_lc_u_b_MusicLibVoice_P,y0                      ; Address of the overlay area
	move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
	jsr     SysLoadResource

#pragma endasm
}

#ifdef MMC
void _reentrant ML_merging_engine_init_playlist3(void)
{
#pragma asm
	include "resource.inc"
	org     p,".ptextplaylist3init":
	extern	F_lc_u_e_MusicLibMerge_P
	extern	F_lc_u_b_MusicLibMerge_P
	extern	SysLoadResource

	; Load the MusicLib merging engine into P memory
	move    #>RSRC_MUSICLIB_MERGE_FUNCTION,x0                      ; Resource number
	move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
	move    #>(F_lc_u_e_MusicLibMerge_P-F_lc_u_b_MusicLibMerge_P)*3,y1 ; overlay area sz
	move    #>F_lc_u_b_MusicLibMerge_P,y0                      ; Address of the overlay area
	move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
	jsr     SysLoadResource

#pragma endasm
}
#endif // #ifdef MMC

#if 0
_reentrant void GetFileNum(int iDevice)
{
	WORD wHighestNumber=0;
	WORD wCurrentNumber=0;

	if( Chdir((_packed char*)g_RecordDirectory) == FS_SUCCESS )
	{
		Finddata finddata;
		_packed BYTE sFileSpec[4];
		int i;
		int iFileNameLength;
		int iDotIndex;
		int find_handle;
		((int*)(sFileSpec))[0]=0x2a2e2a; //*.*
		((int*)(sFileSpec))[1]=0; //null terminator

		_memset(&finddata,0,sizeof(finddata));

		finddata.device = iDevice;
		finddata.startrecord = 0;//start at the 0th record.

		//get the first that matches our specifications
		find_handle = FindFirst(&finddata,(_packed char*) sFileSpec);

		while((find_handle > 0))
		{//while our find handle is valid, keep getting more files
			if(!(finddata.attrib &(ATTR_UNACCEPTABLE)))
			{//if the attributes of the file found are acceptable (this will skip hidden, system, or volume id entries.  It also skips long file entries
				if(!(finddata.attrib&ATTR_DIRECTORY))
				{
					//lets guess about the dot index
					if(Playlist_ValidateEntry(TYPE_FILE, IS_VALID_AUDIO, (INT*)&finddata))
					{
						packed_strcpy(g_recordfile_path,finddata.name);
						wCurrentNumber = SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetFileNumber,0,0x56,g_recordfile_path);
						if(wCurrentNumber > wHighestNumber)
							wHighestNumber = wCurrentNumber;
					}
				}
			}

			//now get the next record
			_memset(&finddata,0,sizeof(finddata));

			//now get the next FCB entry
			if((FindNext(find_handle,&finddata)<0))
			{//if there's no next entry, lets close it out.
				Fclose(find_handle);
				find_handle = -1;
			}
		}
	}

	g_iHighestNumber = wHighestNumber;
}
#endif

_reentrant INT AddSingleFolderSongToLibrary(INT a, INT drive, void * b)
{
    //change into the directory named in the Dirname string
	_packed BYTE sFileSpec[4];
	Finddata finddata;
	int iSongCount = 0;

	int find_handle ;
	((int*)(sFileSpec))[0]=0x2a2e2a; //*.*
	((int*)(sFileSpec))[1]=0; //null terminator
	_memset(&finddata,0,sizeof(finddata));
	finddata.device = drive;
	finddata.startrecord = 0;

	// Get the first that matches our specifications
	find_handle = FindFirst(&finddata,(_packed char*) sFileSpec);
	while( find_handle > 0 )
	{
		if( Playlist_ValidateEntry(TYPE_FILE, IS_VALID_AUDIO, (INT *)&finddata) )
		{
			//backup the finddata into the cache
			int j;
			WORD *src = (WORD*)&finddata;
			WORD _X * dst = (WORD _X *)&g_AddToLibraryData[g_NumAddToLibrary];
			
			for( j = 0; j < (int)sizeof(Finddata); j++ )
				*dst++ = *src++;
			g_NumAddToLibrary++;
		}
		if( g_NumAddToLibrary >= FINDDATA_CACHE_SIZE )
		{
	        iSongCount += SysCallFunction(RSRC_MP3_METADATA_CODEBANK,AddSongToLibrary,0,drive,0);
		}								
		if( FindNext(find_handle,&finddata) < 0 )
		{
			Fclose(find_handle);
			find_handle = -1;
		}
	}
	if( g_NumAddToLibrary > 0 )
	{
        iSongCount += SysCallFunction(RSRC_MP3_METADATA_CODEBANK,AddSongToLibrary,0,drive,0);
	}								
	return iSongCount;
}

#ifdef FUNCLET

    #pragma asm
        extern SysCallFunclet

        FBuildVoiceFilePath:

        nolist
        include "sysmacro.asm"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_BUILDVOICEFILEPATH
        CallFunclet RSRC_FUNCLET_BUILDVOICEFILEPATH

        org p,"SYSFUNCLET_BUILDVOICEFILEPATH_P":
        dc      RSRC_FUNCLET_BUILDVOICEFILEPATH
    #pragma endasm

_reentrant void BuildVoiceFilePath_Funclet(void)
#else
_reentrant void BuildVoiceFilePath(void)
#endif
{
	ML_voice_build_engine_init_playlist3();
	ML_InitVoiceParameter();

	((int*)DirPath)[0] = VOICE_PATH_0;
	((int*)DirPath)[1] = VOICE_PATH_1;
	((int*)DirPath)[2] = VOICE_PATH_2;
	((int*)DirPath)[3] = VOICE_PATH_3;

	g_dwFastKey = g_dwVoiceFastKey;
	g_build_voice_file = TRUE;	// Voice folder to be processed, set the flag so correct add function will be called
    if(Chdir((_packed char*)DirPath) == FS_SUCCESS)
    {
	    g_total_song = 0;
	    //popuplate voice file
		AddSingleFolderSongToLibrary(0,0,0);
    }
#ifdef MMC
    ((int*)DirPath)[0] &= ~(0xFF);
    ((int*)DirPath)[0] |= 0x62; // changed to b:/
    if(Chdir((_packed char*)DirPath) == FS_SUCCESS)
    {
        g_total_song = 0;
	    //popuplate voice file
		AddSingleFolderSongToLibrary(0,1,0);
    }
#endif
	g_build_voice_file = FALSE;	// Voice folder processed, we should reset the flag
	ML_FlushVoiceToFlash(REC_VOICE_TYPE);	// Flush built library to the FLASH
}

#ifdef FUNCLET
    #pragma asm
        org p,".ptextplaylist3init":
    #pragma endasm
#endif

#ifdef FUNCLET

    #pragma asm

        FBuildFMFilePath:

        nolist
        include "sysmacro.asm"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_BUILDFMFILEPATH
        CallFunclet RSRC_FUNCLET_BUILDFMFILEPATH

        org p,"SYSFUNCLET_BUILDFMFILEPATH_P":
        dc      RSRC_FUNCLET_BUILDFMFILEPATH
    #pragma endasm

_reentrant void BuildFMFilePath_Funclet(void)
#else
_reentrant void BuildFMFilePath(void)
#endif
{
	ML_voice_build_engine_init_playlist3();
	ML_InitVoiceParameter();

	((int*)DirPath)[0] = FM_PATH_0;
	((int*)DirPath)[1] = FM_PATH_1;
	((int*)DirPath)[2] = FM_PATH_2;

	g_dwFastKey = g_dwFmFastKey;
	g_build_fm_file = TRUE;	// FM folder to be processed, set the flag so correct add function will be called
	//record must add to HDD
    if(Chdir((_packed char*)DirPath) == FS_SUCCESS)
    {
        g_total_song = 0;
	    //popuplate voice file
		AddSingleFolderSongToLibrary(0,0,0);
    }
#ifdef MMC
    ((int*)DirPath)[0] &= ~(0xFF);
    ((int*)DirPath)[0] |= 0x62; // changed to b:/
    if(Chdir((_packed char*)DirPath) == FS_SUCCESS)
    {
        g_total_song = 0;
	    //popuplate voice file
		AddSingleFolderSongToLibrary(0,1,0);
    }
#endif
	g_build_fm_file = FALSE;	// FM folder processed, we should reset the flag
	ML_FlushVoiceToFlash(REC_FMREC_TYPE);	// Flush built library to the FLASH
}

#ifdef FUNCLET
    #pragma asm
        org p,".ptextplaylist3init":
    #pragma endasm
#endif


#ifdef FUNCLET

    #pragma asm

        FBuildLINEINFilePath:

        nolist
        include "sysmacro.asm"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_BUILDLINEINFILEPATH
        CallFunclet RSRC_FUNCLET_BUILDLINEINFILEPATH

        org p,"SYSFUNCLET_BUILDLINEINFILEPATH_P":
        dc      RSRC_FUNCLET_BUILDLINEINFILEPATH
    #pragma endasm

_reentrant void BuildLINEINFilePath_Funclet(void)
#else
_reentrant void BuildLINEINFilePath(void)
#endif
{
	ML_voice_build_engine_init_playlist3();
	ML_InitVoiceParameter();

	((int*)DirPath)[0] = LINEIN_PATH_0;
	((int*)DirPath)[1] = LINEIN_PATH_1;
	((int*)DirPath)[2] = LINEIN_PATH_2;
	((int*)DirPath)[3] = LINEIN_PATH_3;

	g_dwFastKey = g_dwLineinFastKey;
	g_build_linein_file = TRUE;	// Line-in folder to be processed, set the flag so correct add function will be called
	//record must add to HDD
    if(Chdir((_packed char*)DirPath) == FS_SUCCESS)
    {
        g_total_song = 0;
	    //popuplate voice file
		AddSingleFolderSongToLibrary(0,0,0);
    }
#ifdef MMC
    ((int*)DirPath)[0] &= ~(0xFF);
    ((int*)DirPath)[0] |= 0x62; // changed to b:/
    if(Chdir((_packed char*)DirPath) == FS_SUCCESS)
    {
        g_total_song = 0;
	    //popuplate voice file
		AddSingleFolderSongToLibrary(0,1,0);
    }
#endif
	g_build_linein_file = FALSE;	// Line-in folder processed, we should reset the flag
	ML_FlushVoiceToFlash(REC_PHOTO_TYPE);	// Flush built library to the FLASH
}

#ifdef FUNCLET
    #pragma asm
        org p,".ptextplaylist3init":
    #pragma endasm
#endif

#ifdef FUNCLET

    #pragma asm

        FGetVoiceFastKey:

        nolist
        include "sysmacro.asm"
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_GETVOICEFASTKEY
        CallFunclet RSRC_FUNCLET_GETVOICEFASTKEY

        org p,"SYSFUNCLET_GETVOICEFASTKEY_P":
        dc      RSRC_FUNCLET_GETVOICEFASTKEY
    #pragma endasm

_reentrant void GetVoiceFastKey_Funclet(WORD wFastKeyBitField)
#else
_reentrant void GetVoiceFastKey(WORD wFastKeyBitField)
#endif
{
	_packed BYTE	sFileSpec[4];
	_packed BYTE	szVoice[6];
	_packed BYTE	szFM[3];
	_packed BYTE	szLinein[9];
	Finddata 		finddata;
	INT				find_handle;

	((int*)TempPathArray)[0] = 0x2f3a61;	// "/:a"
	((int*)TempPathArray)[1] = 0x000000;	// Terminating zero

	((int*)szVoice)[0] = VOICEDIR_0;
	((int*)szVoice)[1] = VOICEDIR_1;

	((int*)szFM)[0] = FMDIR_0;

	((int*)szLinein)[0] = LINE1DIR_0;
	((int*)szLinein)[1] = LINE1DIR_1;
	((int*)szLinein)[2] = LINE1DIR_2;

	wFastKeyBitField &= ((1<<FAST_KEY_BIT_POS_VOICE) | (1<<FAST_KEY_BIT_POS_FM) | (1<<FAST_KEY_BIT_POS_LINEIN));

	if (Chdir((_packed char*)TempPathArray) == FS_SUCCESS)
	{
		((INT *)(sFileSpec))[0] = 0x2a2e2a;	// *.*
		((INT *)(sFileSpec))[1] = 0;		// null terminator

		_memset(&finddata, 0, sizeof(finddata));

		finddata.device = 0;
		finddata.startrecord = 0;	//start at the 0th record.

		// get the first that matches our specifications
		find_handle = FindFirst(&finddata, (_packed char*)sFileSpec);

		while (find_handle > 0)
		{
			// while our find handle is valid, keep getting more files
			if (!(finddata.attrib &(ATTR_UNACCEPTABLE)))
			{
				// if the attributes of the file found are acceptable (this will skip hidden, system, or volume id entries.  It also skips long file entries
				if (finddata.attrib & (ATTR_DIRECTORY))
				{
					if (wFastKeyBitField & (1<<FAST_KEY_BIT_POS_VOICE))
					{
						if (packed_strcmp((_packed BYTE *)finddata.name, szVoice)==0)
						{
							g_dwVoiceFastKey = finddata.Key;
							wFastKeyBitField &= ~((WORD)(1<<FAST_KEY_BIT_POS_VOICE));
						}
					}
					else if (wFastKeyBitField & (1<<FAST_KEY_BIT_POS_FM))
					{
						if (packed_strcmp((_packed BYTE *)finddata.name, szFM)==0)
						{
							g_dwFmFastKey = finddata.Key;
							wFastKeyBitField &= ~((WORD)(1<<FAST_KEY_BIT_POS_FM));
						}
					}
					else if (wFastKeyBitField & (1<<FAST_KEY_BIT_POS_LINEIN))
					{
						if (packed_strcmp((_packed BYTE *)finddata.name, szLinein)==0)
						{
							g_dwLineinFastKey = finddata.Key;
							wFastKeyBitField &= ~((WORD)(1<<FAST_KEY_BIT_POS_LINEIN));
						}
					}
				}
			}

			// now get the next record
			_memset(&finddata, 0, sizeof(finddata));

			// now get the next FCB entry
			if (!wFastKeyBitField || (FindNext(find_handle,&finddata)<0))
			{
				// if there's no next entry, lets close it out.
				Fclose(find_handle);
				find_handle = -1;
			}
		}
	}
}

#ifdef FUNCLET
    #pragma asm
        org p,".ptextplaylist3init":
    #pragma endasm
#endif

#ifdef JANUS_BUILD
//////////////////////////////////////////////////////
//
//>  Name:          _reentrant INT FastAddSongToLib(int iDevice)
//
//   Type:          Function
//
//   Description:   This function add the song to library by doing incremental build.it only adds the entry for the recently transfer songs
//                  in to the library.This function adds 10 to 15 songs in incremental build.if more songs are transferred then it returns with
//                  PLAYLIST_REBUILD.
//
//   Inputs:        int iDevice        The number of the device being scanned
//
//   Outputs:       Returns with SUCCESS if all songs entry added in to music lib
//                  Returns with  PLAYLIST_REBUILD if the file size is greater then 1024 bytes.
//
//   Notes:
//
//<
//////////////////////////////////////////////////////
_reentrant INT FastAddSongToLib(int iDevice)
{
    INT String_len=0;
    INT FileHandle=0;
    INT Attrib=0;
    INT Bytesread,i;
    int year;  //for add entry
    _packed BYTE genre[15]; // for add entry

    int temp_conversion=0;
    int comp1=0;
    int comp2=0;

    ((int*)(genre))[0]=0x6b6e55; // Unk
    ((int*)(genre))[1]=0x776f6e; // now
    ((int*)(genre))[2]=0x47206e; // n G
    ((int*)(genre))[3]=0x726e65; // enr
    ((int*)(genre))[4]=0x000065; // e

    //_packed BYTE g_MusicAdd_FilePath[] = "/:ASUMACI.DDTAD\0\0\0"; //open file on a:\musicadd.dat
    FileHandle = Fopen((_packed char *)g_MusicAdd_FilePath, (_packed char *) "r");
    if(FileHandle < 0 )
    {
        return SUCCESS ;
    }
    //get the file size and checks if it is greater then 1024 bytes then go for rebuilding the music lib otherwise
    //do incremental build.
#ifndef _FOLDER_BROWSE_
    if (GetFileSize(FileHandle) > 1024)
#endif
    {
        Fclose(FileHandle);
        Attrib= ATTR_SYSTEM|ATTR_HIDDEN;
        FSFileAttributeClear(Attrib,g_MusicAdd_FilePath,0);
        Fremove((_packed char *)g_MusicAdd_FilePath);
        return PLAYLIST_REBUILD;
    }
#ifndef _FOLDER_BROWSE_
	//StoreFromFlash();
    //load the sorted records in to memory for further sorting of newly added songs.
    LoadSortedRecords();
    while(1)
    {
        //read the path length
        Bytesread = Fread(FileHandle,&String_len,3, Y_MEMORY, -1);
        //fread returns 0 means all the song entry are processed so return with SUCCESS.
        if(Bytesread==0)
        {
            // Flush the updated music library and sorted records onto flash
            //StoreToFlash();
            SaveSortedRecords();
            //ML_FlushLibraryToFlash();
			SysCallFunction(RSRC_MUSICLIB_BUILD_FUNCTION_FLUSH,ML_FlushLibraryToFlash,MEDIA_TYPE_FLASH,0,NULL);
            Fclose(FileHandle);
            Attrib= ATTR_SYSTEM|ATTR_HIDDEN;
            FSFileAttributeClear(Attrib,g_MusicAdd_FilePath,0);
            Fremove((_packed char *)g_MusicAdd_FilePath);
            return SUCCESS;
        }
        //read the song path
        Bytesread = Fread(FileHandle,(INT *)g_CurrentSongName,String_len, Y_MEMORY, -1);
        PutByte((void *)g_CurrentSongName,0,String_len,Y_MEMORY);
        song_info.dir_name[0] = '\0';
        g_wCurrentDirDevId = iDevice;

        if (!IsMusicPath((_packed BYTE *)g_CurrentSongName))
            ;
        else
        if (Playlist_IsValidAudioFile(MAC_RESOURCE_NUM_BYTES+1,(_packed BYTE *)g_CurrentSongName))
        {
            SysCallFunction(RSRC_PLAYLIST_CODEBANK,Fast_Playlist_GetLFN, 0, 0, (INT *)song_info.dir_name);
            Rebuild_GetMetaData(0,0,0);
            //fill the song information in song_info stucture.
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
                if (g_wSongTitle[0] == 0) {
                    song_info.unicode = 0x15; //0x00010101;
                    packed_strcpy((_packed BYTE *)g_wSongTitle, (_packed BYTE *)g_shortname);
                }
                else {
                    song_info.unicode = 0x55; //0x01010101;
                }
                TrimUnicodeString(g_wSongArtist);
                TrimUnicodeString(g_wSongAlbum);
                TrimUnicodeString(g_wSongGenre);
                TrimUnicodeString(g_wSongTitle);
                strcpy(song_info.artist_name, g_wSongArtist);
                strcpy(song_info.album_name, g_wSongAlbum);
                strcpy(song_info.genre_name, g_wSongGenre);
                //packed_strcpy(song_info.genre_name, genre); // temp approach
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
                if (g_wSongTitle[0] == 0) {
                    packed_strcpy((_packed BYTE *)g_wSongTitle, (_packed BYTE *)g_shortname);
                }
                if (g_wSongArtist[0] < 65536)
                {
                    TrimUnicodeString(g_wSongArtist);
                    strcpy(song_info.artist_name, g_wSongArtist);
                    song_info.unicode = song_info.unicode | 0x00000001;
                }
                else
                {
                    TrimPackedString(g_wSongArtist);
                    packed_strcpy((_packed BYTE *)song_info.artist_name, (_packed BYTE *)g_wSongArtist);
                }

                if (g_wSongAlbum[0] < 65536)
                {
                    TrimUnicodeString(g_wSongAlbum);
                    strcpy(song_info.album_name, g_wSongAlbum);
                    song_info.unicode = song_info.unicode | 0x00000004;
                }
                else
                {
                    TrimPackedString(g_wSongAlbum);
                    packed_strcpy((_packed BYTE *)song_info.album_name, (_packed BYTE *)g_wSongAlbum);
                }

                if (g_wSongGenre[0] != 0)
                {
                    TrimPackedString(g_wSongGenre);
                    packed_strcpy((_packed BYTE *)song_info.genre_name, (_packed BYTE *)g_wSongGenre);
                }
                else
                {
                    packed_strcpy((_packed BYTE *)song_info.genre_name, genre);
                }


                if 	(g_wSongTitle[0] < 65536)
                {
                    TrimUnicodeString(g_wSongTitle);
                    strcpy(song_info.song_name, g_wSongTitle);
                    if (!strlen(song_info.song_name)) {
                        // if the string length is zero then check if LFN or SFN is present.
                        if (!strlen(song_info.dir_name)) {
                            packed_strcpy((_packed BYTE *)song_info.song_name, (_packed BYTE *)g_shortname);
                        }
                        else {
                            strcpy(song_info.song_name, song_info.dir_name);
                            song_info.unicode = song_info.unicode | 0x0000040;
                        }
                    }
                    else {
                        song_info.unicode = song_info.unicode | 0x0000040;
                    }
                }
                else
                {
                    TrimPackedString(g_wSongTitle);
                    packed_strcpy((_packed BYTE *)song_info.song_name, (_packed BYTE *)g_wSongTitle);
                    if (!packed_strlen((_packed BYTE *)song_info.song_name)) {
                        // if the string length is zero then check if LFN or SFN is present.
                        if (!strlen(song_info.dir_name)) {
                            packed_strcpy((_packed BYTE *)song_info.song_name, (_packed BYTE *)g_shortname);
                        }
                        else {
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
            packed_strcpy((_packed BYTE *)song_info.path_name, (_packed BYTE *)g_CurrentSongName);
            song_info.track_number = g_wSongTrackNum;
            //call this API to add the song entry in to music library.
            //ML_AddEntryToLibrary(&song_info, g_option);
            ML_AddEntryToLibrary(iDevice, &song_info, g_option);
            bResume = FALSE;
        }
    }
#endif //_FOLDER_BROWSE_
}
#endif



///////////////////////////////////////////////////////////////////////
//! \brief
//!
//! \fntype Function
//!
//! \param[in]
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
#ifdef _FOLDER_BROWSE_
_reentrant RETCODE AddDirToLibrary(INT iDrive, INT iDirDepth)
{
    ML_DIRINFO_T    *ptDirInfo;
    INT i;

	((int*)TempPath)[0]=0x002e2e;//change dir to one level up(..).
	Chdir((_packed char*)TempPath);

    if ((iDrive == 0) || (iDrive == 1))
    {
        if ((((INT *)DirPath)[1] != 0x454641) || (((INT *)DirPath)[2] != 0x545341))   // no AFEAST
        {
			ML_DIRINFO_T    *ptDirInfo;
			int iCurrent = iDirDepth;
			
			while( TRUE )
			{
				if( iCurrent == 0 || g_tDirInfoBuffer[iCurrent-1].u4Added == TRUE )
					break;
				iCurrent = iCurrent - 1;
				if( iCurrent > 0 )
					Chdir((_packed char*)TempPath);
			}

			while( iCurrent <= iDirDepth )
			{
				ptDirInfo = &g_tDirInfoBuffer[iCurrent];
				PathFormation((_packed BYTE *)ptDirInfo->u24PathName,TempPathArray,iCurrent+1);
				if( iCurrent > 0 && ML_GetLFN(g_tDirInfoBuffer[iCurrent-1].dwFastKey, ptDirInfo->iDirRecord, g_u24DirName) == PLAYLIST_SUCCESS )
				{
					ptDirInfo->u8Unicode = BITCHK_SONG_UNICODE;
				}
				else
				{
					PathFormationPartial((_packed BYTE *)g_u24DirName,TempPathArray,iCurrent);
					ptDirInfo->u8Unicode = BITCHK_SONG_ALLASCII;
				}

				ML_AddDirEntryToLibrary(iDrive, ptDirInfo, ADD_OPTION_DIR);
				iCurrent++;
				if( iCurrent > 1 )
				{
					//build the relative path instead of absolute path
					PathFormationPartial(TempPath,TempPathArray,iCurrent-1);
					ChdirFromOffset((_packed char*)TempPath,g_tDirInfoBuffer[iCurrent-1].iDirRecord);
				}
			}
		}
    }

    return SUCCESS;
}
#endif  // _FOLDER_BROWSE_

///////////////////////////////////////////////////////////////////////
//! \brief  Load intermediate memory
//!
//! \fntype Function
//!
//! \param[in]
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
_reentrant void ML_BuildingEngineInitPlaylist3For1stPass(void)
{
#pragma asm
    include "resource.inc"
    org     p,".ptextplaylist3init":
    extern  F_lc_u_e_MusicLibBuild_X
    extern  F_lc_u_b_MusicLibBuild_X
    extern  SysLoadResource

    ; Load the MusicLib genre table into X memory
    move    #>RSRC_MUSICLIB_BUILD_GENRE_TABLE,x0                        ; Resource number
    move    #>RSRC_TYPE_DATA,x1                                         ; Resource type is data
    move    #>(F_lc_u_e_MusicLibBuild_X-F_lc_u_b_MusicLibBuild_X)*3,y1  ; overlay area sz
    move    #>F_lc_u_b_MusicLibBuild_X,y0                               ; Address of the overlay area
    move    #TARGET_MEM_X,a1                                            ; Overlay area is in X memory
    jsr     SysLoadResource
#pragma endasm
}

///////////////////////////////////////////////////////////////////////
//! \brief  Load intermediate memory
//!
//! \fntype Function
//!
//! \param[in]
//!
//! \return
//!
///////////////////////////////////////////////////////////////////////
_reentrant void ML_BuildingEngineInitPlaylist3For2ndPass(void)
{
#pragma asm
    include "resource.inc"
    org     p,".ptextplaylist3init":
    extern  F_lc_u_e_MusicLibBuildFlushMem_X
    extern  F_lc_u_b_MusicLibBuildFlushMem_X
    extern  SysLoadResource

    ; Load the MusicLib flush memory into X memory
    move    #>RSRC_MUSICLIB_BUILD_FLUSH_MEM,x0                                          ; Resource number
    move    #>RSRC_TYPE_DATA,x1                                                         ; Resource type is data
    move    #>(F_lc_u_e_MusicLibBuildFlushMem_X-F_lc_u_b_MusicLibBuildFlushMem_X)*3,y1  ; overlay area sz
    move    #>F_lc_u_b_MusicLibBuildFlushMem_X,y0                                       ; Address of the overlay area
    move    #TARGET_MEM_X,a1                                                            ; Overlay area is in X memory
    jsr     SysLoadResource
#pragma endasm
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
