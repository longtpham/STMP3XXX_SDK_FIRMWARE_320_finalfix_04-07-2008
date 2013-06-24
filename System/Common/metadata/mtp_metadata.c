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
#include "kernel.h"
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

WORD g_wVBRTOCTable[VBR_TOC_SIZE];
#ifdef USE_PLAYLIST1
extern int g_iInternalTrack;
#endif

INT _reentrant GetFileName(FILE_META_DATA * MetaData,_packed BYTE * pcFilePathName);
extern RETCODE _reentrant GetAudibleMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData);

////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////

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

    // Clear the genre byte
    MetaData->btGenre = (BYTE)0;

    if(iStrLen > 4)
    {
        // Use C Speed client  Increase call
        // Defines are in SysSpeed.inc.  Sysspeed.h is generated
//        SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_METADATA);
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
            switch(wExtension)
            {
                case 0x33504d:  //"MP3"
                    rtn = SysCallFunction(RSRC_MP3_METADATA_CODEBANK,GetMp3MetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
                    break;
                case 0x414D57:  //"WMA"
                    rtn = SysCallFunction(RSRC_WMA_METADATA_CODEBANK,GetWmaMetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
                    break;
                case 0x564157:  //"WAV"
                    rtn = SysCallFunction(RSRC_WAV_METADATA_CODEBANK,GetWavMetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
                    break;
#ifdef AUDIBLE
                case 0x004141:  //"AA "
                    rtn = SysCallFunction(RSRC_AUDIBLE_METADATA_CODEBANK,GetAudibleMetaData,(INT)pcFilePathName,btCurrentDevice,(void*)MetaData);
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
			if( GetFileName(MetaData,pcFilePathName) == SUCCESS )
#endif
#endif
			{
			}
            else	//no long file name so we must diplay the short one
			{	
            	GetFileName(MetaData,pcFilePathName);
        	}
        }
        TrimString(MetaData->wArtist);
        TrimString(MetaData->wTitle);
        TrimString(MetaData->wAlbum);

        // C Speed client free call
  //      SysSpeedClockFree(SPEED_CLIENT_METADATA); // free the shared speed resource  

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
    for(i=packed_strlen(pcFilePathName);i>0;i--)
    {//start at the end, go backward until the char is '/'
        if(packed_get(pcFilePathName,i)==0x2f)
            break;
    }

    //now go forward until we hit the terminating NULL, copying each byte
    iLength = 0;
    while((wChar=packed_get(pcFilePathName,++i)))//Fixed for Stmp00014780. {i++ --> ++i, changed for romoving the '/' char.}
    {
        packed_set(btPackedFileName,iLength++,wChar);
    }
    packed_set(btPackedFileName,iLength++,0);

    if(MetaData->wTitle)
        DBCSToUnicode(btPackedFileName,MetaData->wTitle,MetaData->iTitleBufferLength);

    return(SUCCESS);
}
