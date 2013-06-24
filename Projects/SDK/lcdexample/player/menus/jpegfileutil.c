/***********************************************************************************************************/
// jpegfileutil.c and jpepfileutilextra.c only exist when Playlist3 is enabled.
// It is a work around since play list3 does not support extra file type such as jpg and smv
/***********************************************************************************************************/
#ifdef USE_PLAYLIST3
#include "types.h"
#include "fsapi.h"
#include "platform.h"
#include "musiclib_ghdr.h"
#include "stringlib.h"
#include "playlist.h"
#include "syscoder.h"
#include "sysmem.h"
#include "metadata.h"
#include "dbcs.h"
#include "FileSystem.h"
#include "playlist3internal.h"
#include "playerlib.h"
#include "jpegfileutil.h"

////////////////////////////////////////////////////////////////////////////////////
#define OTG_READONLY_FILE   0x21
#define OTG_TYPE_IS_DIR     0x10                                             
#define OTG_TYPE_IS_DIR_1   0x10    //normal directory                           
#define OTG_TYPE_IS_DIR_2   0x30    //Archive directory                          
#define OTG_TYPE_IS_FILE    0x20                                             

#define DIR_NAMEOFFSET              0
#define DIR_EXTOFFSET               8
#define DIR_ATTRIBUTEOFFSET        11
#define DIR_CRTTIMEOFFSET          14
#define DIR_CRTDATEOFFSET          16
#define DIR_LSTACCDATEOFFSET       18
#define DIR_FSTCLUSHIOFFSET        20         //high 16 bits of cluster number
#define DIR_WRTTIMEOFFSET          22
#define DIR_WRTDATEOFFSET          24
#define DIR_FSTCLUSLOOFFSET        26         //low 16 bits of cluster number
#define DIR_FILESIZEOFFSET         28

extern int              g_current_index;
#ifdef MMC
extern uint24   g_is_SD_inserted;
#endif
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern _reentrant INT FindFirst(Finddata *_finddata,_packed char *FileName);

INT             iJpegPlaySet;
INT             iDriveWaterShed;
INT             iExtWaterShed[2];
DWORD           dwJpegFileFastKey[2];
WORD            *pwTotal;
WORD            *pwCurrent;
_packed char    sSpec[15];
_packed char    sStartDir[2][25];

/***********************************************************************************************************/
// A work around function for Jpeg and motion video as if calling Playlist_GetCurrentSongFileInfo()
// This would coexist with motion video and jpeg applicaiton to reduce the bank switching redundancy.
/***********************************************************************************************************/
RETCODE _reentrant JpegFile_GetCurrentSongFileInfo(INT iUnused1, INT iUnused2, INT *pPtr)
{
    SONGFILEINFO*   SongInfo = (SONGFILEINFO*)pPtr;
	INT             iDirHandle;
    int             HandleNumber;
    int             iLocal;
    int             iDrive;
	Finddata        _finddata;
	WORD            wSearchSongNumber;
	DWORD           dwFasKey;
    RETCODE         rtn=PLAYLIST_END_OF_LIST;
#ifdef MMC
	BOOL            bReent;
#endif
    iUnused1; iUnused2;
    if(*pwTotal)
    {   if(iJpegPlaySet == PLAYSET_PHOTO)
        {   if((*pwCurrent-1) < iDriveWaterShed)iDrive = 0;
            else                                iDrive = 1;
        }else
        {   if(*pwCurrent < iDriveWaterShed)    iDrive = 0;
            else                                iDrive = 1;
        }
        if((Chdir(sStartDir[iDrive]))<0)
            return PLAYLIST_END_OF_LIST;
        
        wSearchSongNumber = *pwCurrent;    
        _memset(&_finddata,0,sizeof(_finddata));
    	_finddata.device = iDrive;
        _finddata.startrecord = 0;//start at the 0th record.
    	
    	// Set up the correct file extenision for the selected playset.
    	((int*)sSpec)[2] = 0;
        if(iJpegPlaySet == PLAYSET_MVIDEO)
        {   ((int*)sSpec)[0] = (int)'*' + (((int)'.') << 8) + (((int)'S') << 16);
            ((int*)sSpec)[1] = (int)'M' + (((int)'V') << 8);
            if(iDrive == 1) wSearchSongNumber -= iDriveWaterShed;
        }else if(iJpegPlaySet == PLAYSET_PHOTO)
        {   if(wSearchSongNumber > 0)
                wSearchSongNumber--;    // Since g_CurrentTrack.iTrackOrder starts from 1
            if(wSearchSongNumber < iExtWaterShed[iDrive])
            {   ((int*)sSpec)[0] = (int)'*' + (((int)'.') << 8) + (((int)'J') << 16);
                ((int*)sSpec)[1] = (int)'P' + (((int)'G') << 8);
                if(iDrive == 1) wSearchSongNumber -= iDriveWaterShed;
            }else
            {   ((int*)sSpec)[0] = (int)'*' + (((int)'.') << 8) + (((int)'B') << 16);
                ((int*)sSpec)[1] = (int)'M' + (((int)'P') << 8);
                wSearchSongNumber = *pwCurrent - iExtWaterShed[iDrive] - 1;
            }
        }

#ifdef MMC
        bReent = FALSE;
        if(g_is_SD_inserted == HAS_SD)
        {   EnterNonReentrantSection();
            bReent = TRUE;
        }
#endif
        if((HandleNumber = FindFirst(&_finddata,sSpec))<0)
            return PLAYLIST_END_OF_LIST;    // if no such file, return.
        
        // from here, wSearchSongNumber is the desire searching index for the selected file extension.
        iLocal = 0;     
        while(HandleNumber>0)
        {   if(_finddata.attrib&OTG_TYPE_IS_FILE)
            {   if(iLocal == wSearchSongNumber)
                {   // searching index reached.
                    packed_strcpy(SongInfo->m_pFilename, (_packed BYTE*)_finddata.name);    // copy the short filename
                    Fclose(HandleNumber);
                    packed_strcpy(g_CurrentSongName,(_packed BYTE *)sStartDir[iDrive]); // copy the directory name.
                    packed_strcat(g_CurrentSongName,SongInfo->m_pFilename);     // construct the full path short file name.
                	if(!((iDirHandle = FastOpen(dwJpegFileFastKey[iDrive] ,(_packed char*)"d")) <0))
                	{   if(!(ConstructLongFileName(iDirHandle, _finddata.startrecord - 1, (INT*)g_wSongTitle) > 0)) // construct the long file name, it is required by motion video.
                	    {   DBCSToUnicode(SongInfo->m_pFilename,g_wSongTitle,TITLE_SIZE-1); // if long file name fail, short filename instead.
                	    }
                		Fclose(iDirHandle);
                	}
        			HandleNumber = -1;
                    rtn = PLAYLIST_SUCCESS;
                }else
                {   iLocal++;
                }
    		}
    		if(HandleNumber > 0)
    		{   if(FindNext(HandleNumber,&_finddata)<0 )
                {   Fclose(HandleNumber);
        			HandleNumber = -1;
                } 
            }
    	}
#ifdef MMC
        if(bReent == TRUE)
        {   LeaveNonReentrantSection();
        }
#endif
	}
    return rtn;
}

/***********************************************************************************************************/
// A work around function for Jpeg and motion video as if calling Playlist_GetNextSongFileInfo()
// This would coexist with motion video and jpeg applicaiton to reduce the bank switching redundancy.
/***********************************************************************************************************/
RETCODE _reentrant JpegFile_GetNextSongFileInfo(INT iMode, INT iRepeat, INT *pPtr)
{
    RETCODE rtn = PLAYLIST_END_OF_LIST;
    BOOL    bEndReach = FALSE;
    
    if(*pwTotal)
    {   while(iRepeat >= 0)
        {   iRepeat--;
            (*pwCurrent)++;
	        if(iJpegPlaySet == PLAYSET_MVIDEO)
            {   if((*pwCurrent) >= *pwTotal)
                {   (*pwCurrent) = 0;
                }
            }else
            {   if((*pwCurrent) > *pwTotal)
                {   (*pwCurrent) = 1;   // Since g_CurrentTrack.iTrackOrder starts from 1
                    if(iMode != NEXTSONG_PLAY_BUTTON)
                    {   bEndReach = TRUE;
                    }
                }
            }
        }
    }
	if(iJpegPlaySet == PLAYSET_MVIDEO)
    {   g_current_index = (*pwCurrent);
        return(JpegFile_GetCurrentSongFileInfo(0,0,pPtr));
    }else
    {   if(bEndReach == FALSE)
        {   rtn = PLAYLIST_SUCCESS;
        }
        if(JpegFile_GetCurrentSongFileInfo(0,0,pPtr) != PLAYLIST_SUCCESS)
            rtn = PLAYLIST_END_OF_LIST;
    }
    return rtn;        
}

/***********************************************************************************************************/
// A work around function for Jpeg and motion video as if calling Playlist_GetPreviousSongFileInfo()
// This would coexist with motion video and jpeg applicaiton to reduce the bank switching redundancy.
/***********************************************************************************************************/
RETCODE _reentrant JpegFile_GetPreviousSongFileInfo(INT iUnused1, INT iRepeat, INT *pPtr)
{
    if(*pwTotal)
    {   while(iRepeat >= 0)
        {   iRepeat--;
	        if(iJpegPlaySet == PLAYSET_MVIDEO)
            {   if((*pwCurrent) == 0)
                {   (*pwCurrent) = *pwTotal - 1;
                }else
                {   (*pwCurrent)--;
                }
            }else
            {   if((*pwCurrent) == 1)
                {   (*pwCurrent) = *pwTotal;    // Since g_CurrentTrack.iTrackOrder starts from 1 to *pwTotal
                }else
                {   (*pwCurrent)--;
                }
            }
        }
    }
	if(iJpegPlaySet == PLAYSET_MVIDEO)
        g_current_index = (*pwCurrent);
    return(JpegFile_GetCurrentSongFileInfo(0,0,pPtr));
}
#endif // USE_PLAYLIST3