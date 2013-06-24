////////////////////////////////////////////////////////////////////////////////
// Copyright (c) SigmaTel, Inc. 2003-2006
// Filename: playlist5init.c
//
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "menumanager.h"
#include "playlist.h"       //required include file
#include "playerlib.h"
#include "project.h"
#include "stringlib.h"
#include "resource.h"
#include "sysspeed.h"
#include "playlist5.h"
//#include "playlist5internal.h"
#include "playlist5_browsemenu.h"
#include "sysmem.h"
#include "FileSystem.h"
#include "fserror.h"
#include "FileSpec.h"
#include "playlist.h"
#include "syscoder.h"
#include "exec.h"
#include "extern.h"

#pragma optimize 1

////////////////////////////////////////////////////////////
// extern variables

////////////////////////////////////////////////////////////
// prototypes
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// glocal variables
////////////////////////////////////////////////////////////
BOOL g_Rebuild;
int g_iHighestVoiceNumber = 0;
int g_iHighestFMNumber = 0;
int g_iHighestLineNumber = 0;
int* pHighestNumber;
extern e_PL5BrowseType g_ePL5BrowseType;

////////////////////////////////////////////////////////////
// funcstions definitions
////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------

   Function Name:   INT _reentrant Playlist_InitRoot(int a, int b, int *c)

   FunctionType:    Reentrant

   Inputs:          ignored

   Outputs:         return code.

   Description:     Init from root ...
                    This function will initialize playback parameters. We must make sure one song is
                    ready for playback here before enter music menu.

----------------------------------------------------------------------------*/
_reentrant RETCODE Playlist_InitRoot(INT a, INT b, INT *c)
{
    PL5_FOLDER_RECORD tmp_dirrec;
    int i;
    _packed BYTE pszDirName[SFN_LENGTH];
    int highlight;
	int ret = PLAYLIST_SUCCESS;
	a;b;c;

    g_Rebuild = FALSE;

    /* Initialize playback parameters */
    _memset(&g_PL5_Playback, 0, sizeof(PL5_PLAYBACK));
    g_PL5_Playback.queue = &g_PL5_PL_queue;
    g_PL5_Playback.search = &g_PL5_PL_search;
    g_PL5_Playback.type = PL5_MUSICTYPE;
    g_PL5_Playback.mode = PL5_MODE_PLAYFOLDERS;
	g_ePL5BrowseType = PL5_MUSICTYPE;

    _memset(&g_PL5_PL_queue, 0, sizeof(PL5_PL_QUEUE));
    g_PL5_PL_queue.pos = g_PL5_PL_buffer;
    g_PL5_PL_queue.size = PL5_PL_QUEUE_SIZE;

    _memset(&g_PL5_PL_search, 0, sizeof(PL5_PL_SEARCH));
    g_PL5_PL_search.status = PL5_STATE_IDLE;
    g_PL5_PL_search.phase = PL5_NEXT_FILE;
    g_PL5_PL_search.foregnd = FALSE;
    g_PL5_PL_search.piExtSupported = g_piPlayExtSupported;
    g_PL5_PL_search.tPath = g_PL5_PL_searchingfolders;
    g_PL5_PL_search.sPath = g_PL5_PL_searchingpath;
    g_PL5_PL_search.tPath_cnt = g_PL5_PL_searchingfolders_cnt;
    g_PL5_PL_search.sPath_cnt = g_PL5_PL_searchingpath_cnt;
    //Initialize searching path.
    // the path[0] is always the virtual folder, the real path is actually from path[1].
    _memset(&g_PL5_PL_searchingfolders[1], 0, sizeof(PL5_PL_FOLDER));
    g_PL5_PL_searchingfolders[1].record.iDevice = 0;
    g_PL5_PL_searchingfolders[1].record.bIsRoot = TRUE;
    g_PL5_PL_searchingfolders[1].record.bIsDir = TRUE;
    g_PL5_PL_searchingfolders[1].record.iRecordNum = 0;
    g_PL5_PL_searchingfolders[1].record.lFastKey = GetRootdirkey(0);
    ((int*)g_PL5_PL_searchingpath)[0] = 0x003a61;   // "a:"
    g_PL5_PL_search.depth = 2;

    switch (g_PL5_Playback.type)
    {
        case PL5_MUSICTYPE:
            g_piPlayExtSupported[0] = MP3_FILE_EXT;
            g_piPlayExtSupported[1] = WMA_FILE_EXT;
            g_piPlayExtSupported[2] = WAV_FILE_EXT;
            g_piPlayExtSupported[3] = 0;          // the end
            break;
        case PL5_VOICETYPE:
            g_piPlayExtSupported[0] = MP3_FILE_EXT;
            g_piPlayExtSupported[1] = WAV_FILE_EXT;
            g_piPlayExtSupported[2] = 0;          // the end
            break;
#ifndef REMOVE_FM
        case PL5_FMTYPE:
            g_piPlayExtSupported[0] = MP3_FILE_EXT;
            g_piPlayExtSupported[1] = WAV_FILE_EXT;
            g_piPlayExtSupported[2] = 0;          // the end
            break;
#endif
        case PL5_LINEINTYPE:
            g_piPlayExtSupported[0] = MP3_FILE_EXT;
            g_piPlayExtSupported[1] = WAV_FILE_EXT;
            g_piPlayExtSupported[2] = 0;          // the end
            break;
#ifdef JPEG_APP
        case PL5_PHOTOTYPE:
            g_piPlayExtSupported[0] = JPG_FILE_EXT;
            g_piPlayExtSupported[1] = BMP_FILE_EXT;
            g_piPlayExtSupported[2] = 0;          // the end
            break;
#endif
#ifdef MOTION_VIDEO
        case PL5_VIDEOTYPE:
            g_piPlayExtSupported[0] = SMV_FILE_EXT;
            g_piPlayExtSupported[1] = 0;          // the end
            break;
#endif
#ifdef AUDIBLE
        case PL5_AUDIBLETYPE:
            g_piPlayExtSupported[0] = AUD_FILE_EXT;
            g_piPlayExtSupported[1] = 0;          // the end
            break;
#endif
        default:
            // extsupported[0] is zero means *.*
            g_piPlayExtSupported[0] = 0;          // the end
    }

    g_iRandomNum = 0;

    /* Before enter music menu, it is necessary to get at least one song ready for playback,
       so set search foreground to find the 1st song here. */
    i = g_PL5_PL_queue.size;
    g_PL5_PL_queue.size = 2;            // set queue size 1 temporarily for quick end of search.
    g_PL5_PL_search.foregnd = TRUE;     // set search foreground, it will be reset to background after search complete.
    SysPostMessage(2, PL5_PL_MSG_START);
    SysWaitOnEvent(0,0,0);
    g_PL5_PL_queue.size = i;            // reset queue size to default.

    if (g_PL5_PL_queue.head == g_PL5_PL_queue.tail)
    {
        // Nothing was found
        g_wTotalSongCount = g_PL5_Playback.total = 0;
        g_PL5_Playback.foundall = TRUE;
        SysPostMessage(2, PL5_PL_MSG_STOP);
    }
    else
    {
        // One song is ready for playback
        g_wTotalSongCount = g_PL5_Playback.total = 0;

        // reset start point of counting
        _memcpy(g_PL5_PL_searchingfolders_cnt, g_PL5_PL_searchingfolders, sizeof(PL5_PL_FOLDER) * (MAX_DIR_DEPTH+1));
        packed_strcpy(g_PL5_PL_searchingpath_cnt, g_PL5_PL_searchingpath);
        g_PL5_PL_search.depth_cnt = g_PL5_PL_search.depth;
        g_PL5_PL_search.start_cnt = 0;	

        if (g_PL5_PL_queue.head != (g_PL5_PL_queue.tail + 1) % g_PL5_PL_queue.size)
        {
			int tail;
			
            // continue to search more next file if playback queue has space
            g_PL5_PL_search.phase = PL5_NEXT_FILE;
			
			// Reset the start point.
		    tail = (g_PL5_PL_queue.tail + g_PL5_PL_queue.size - 1) % g_PL5_PL_queue.size;
			g_PL5_PL_search.start = (g_PL5_PL_queue.pos + tail)->file.iRecordNum + 1;
		    g_PL5_PL_search.foregnd = FALSE;     
            SysPostMessage(2, PL5_PL_MSG_START);
            SysWaitOnEvent(0,0,0);
        }
    }

    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_SetPlaySet,g_iPlaySet,0,0);

	return PLAYLIST_SUCCESS;
}

#ifdef JPEG_APP
/*----------------------------------------------------------------------------

   Function Name:   WORD _reentrant Playlist_GetFileExtention(_packed BYTE* pFileName)

   FunctionType:    Function

   Inputs:        _packed BYTE*   File Name String

   Outputs:       WORD   File Extension

   Description:   Finds the file extension
				1 - Find length of file
				2 - Find the "." by traversing backwards from the end of the string
				3 - Fill File Extention string
				4 - Return the file extention as a word

----------------------------------------------------------------------------*/
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
#endif
