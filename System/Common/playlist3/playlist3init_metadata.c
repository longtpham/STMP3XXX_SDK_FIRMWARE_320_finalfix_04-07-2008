////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Sigmatel Inc 2005
// Filename: playlist3init.c
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

extern int MenuResourcePtr;
extern Finddata _X g_AddToLibraryData[];
extern int _X g_NumAddToLibrary;

_reentrant INT AddSongToLibraryStatic(int drive);

struct
{
	INT MP3MetaDataLoaded : 1;
	INT WMAMetaDataLoaded : 1;
	INT WAVMetaDataLoaded : 1;
}
g_Playlist3InitMetaDataCodeBank = { 0,0,0 };
WORD g_Playlist3InitExtension = 0;

_reentrant INT AddSongToLibrary(INT a, INT drive, void * b)
{
	BOOL bContinue = TRUE;
	
	switch( MenuResourcePtr )
	{
		case RSRC_MP3_METADATA_CODEBANK:
			g_Playlist3InitMetaDataCodeBank.MP3MetaDataLoaded = 1;
			break;
		case RSRC_WMA_METADATA_CODEBANK:
			g_Playlist3InitMetaDataCodeBank.WMAMetaDataLoaded = 1;
			break;
		case RSRC_WAV_METADATA_CODEBANK:
			g_Playlist3InitMetaDataCodeBank.WAVMetaDataLoaded = 1;
			break;
		default:
			return 0;
	}			
				
	while( bContinue == TRUE && g_NumAddToLibrary > 0 )
	{
		if( g_Playlist3InitExtension == 0 )
		{
			INT i;
			WORD iStrLen;
			_packed char filename[MAX_FILESNAME];
			_packed char _X *ptr = g_AddToLibraryData[g_NumAddToLibrary-1].name;
		
			for( i = 0; i < (int)sizeof(filename); i++ )
				filename[i] = ptr[i];		
			
			iStrLen = packed_strlen((_packed BYTE *)filename);
	
			if(iStrLen > 4)
			{
				while(iStrLen--)
				{
					g_Playlist3InitExtension = packed_get((_packed BYTE *)filename,iStrLen);
					if(g_Playlist3InitExtension=='.')
						break;
				}
				if(iStrLen)
				{
					for(i=0;i<3;i++)
					{
						packed_set((void*)&g_Playlist3InitExtension,i,packed_get((_packed BYTE *)filename,++iStrLen));
					}
				}
			}
		}
		switch(g_Playlist3InitExtension)
		{
		case 0x33504d:  //"MP3"
			if( g_Playlist3InitMetaDataCodeBank.MP3MetaDataLoaded == 0 )
				SysCallFunction(RSRC_MP3_METADATA_CODEBANK,AddSongToLibrary,0,drive,0);
			else if( MenuResourcePtr != RSRC_MP3_METADATA_CODEBANK )
			{
				bContinue = FALSE;
				continue;
			}
			break;
		case 0x414D57:  //"WMA"
			if( g_Playlist3InitMetaDataCodeBank.WMAMetaDataLoaded == 0 )
				SysCallFunction(RSRC_WMA_METADATA_CODEBANK,AddSongToLibrary,0,drive,0);
			else if( MenuResourcePtr != RSRC_WMA_METADATA_CODEBANK )
			{
				bContinue = FALSE;
				continue;
			}
			break;
		case 0x564157:  //"WAV"
			if( g_Playlist3InitMetaDataCodeBank.WAVMetaDataLoaded == 0 )
				SysCallFunction(RSRC_WAV_METADATA_CODEBANK,AddSongToLibrary,0,drive,0);
			else if( MenuResourcePtr != RSRC_WAV_METADATA_CODEBANK )
			{
				bContinue = FALSE;
				continue;
			}
			break;
		case 0x004141:  //"AA "
			break;
		default:
			g_NumAddToLibrary--;
			g_Playlist3InitExtension = 0;
			continue;
		}
		
		AddSongToLibraryStatic(drive);
		g_Playlist3InitExtension = 0;
	}

	switch( MenuResourcePtr )
	{
		case RSRC_MP3_METADATA_CODEBANK:
			g_Playlist3InitMetaDataCodeBank.MP3MetaDataLoaded = 0;
			break;
		case RSRC_WMA_METADATA_CODEBANK:
			g_Playlist3InitMetaDataCodeBank.WMAMetaDataLoaded = 0;
			break;
		case RSRC_WAV_METADATA_CODEBANK:
			g_Playlist3InitMetaDataCodeBank.WAVMetaDataLoaded = 0;
			break;
	}				
	return 0;
}

