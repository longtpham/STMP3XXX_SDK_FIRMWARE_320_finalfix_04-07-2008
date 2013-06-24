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

extern unsigned int _Y  g_wUserScratchY[];

extern int      g_current_index;
#ifdef MMC
extern uint24   g_is_SD_inserted;
#endif
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern _reentrant INT FindFirst(Finddata *_finddata,_packed char *FileName);
extern _asmfunc int unpack_string( _packed BYTE _Y * _Y pPacked,  UCS3 _Y * _Y pUnpacked);

int _reentrant SetRoot(_packed BYTE *root, int iDrive);
int _reentrant JpegGetTotalFileNum(int iDrive);
void SetUpPointer(void);

extern INT          iJpegPlaySet;
extern INT          iDriveWaterShed;
extern INT          iExtWaterShed[2];
extern DWORD        dwJpegFileFastKey[2];
extern WORD         *pwTotal;
extern WORD         *pwCurrent;
extern _packed char sSpec[];

/***********************************************************************************************************/
// A work around function for Jpeg and motion video as if calling Playlist_SetPlaySet()
// This would coexist with motion video and jpeg applicaiton to reduce the bank switching redundancy.
/***********************************************************************************************************/
INT _reentrant JpegFile_SetPlaySet(INT iPlayset, INT bUnused, INT *pUnused)
{
    INT                 i,iLength;
    _packed BYTE        sRoot[2];
    _packed BYTE        sKey[9];
    int                 HandleNumber;
    int                 RetVal=PLAYLIST_TRACK_NOT_FOUND;
	Finddata            _finddata;
	UCS3*               temp1;
	UCS3*               temp2;
	int                 tempval;
	int                 iDrive,iDriveCount;
#ifdef MMC
	BOOL                bReent;
#endif
	bUnused; pUnused;

	iJpegPlaySet = iPlayset;

	// Set up the correct file extenision for the selected playset.
    iDriveWaterShed = 0;
	if(iJpegPlaySet == PLAYSET_MVIDEO)
	{   ((int*)sKey)[0] = (int)'M' + (((int)'V') << 8) + (((int)'I') << 16);    // a:/MVIDEO/ is the default directory name for motion video.
        ((int*)sKey)[1] = (int)'D' + (((int)'E') << 8) + (((int)'O') << 16);    // sometimes it may be a:/MVIDEO~1/
        ((int*)sKey)[2] = 0;
        g_wTotalSongCount = 0;  // g_wTotalSongCount is the total song count for motion video.
    }else if(iJpegPlaySet == PLAYSET_PHOTO)
	{   ((int*)sKey)[0] = (int)'P' + (((int)'H') << 8) + (((int)'O') << 16);    // a:/PHOTO/ is the default directory name for motion video.
        ((int*)sKey)[1] = (int)'T' + (((int)'O') << 8);                         // sometimes it may be a:/PHOTO~1/
        ((int*)sKey)[2] = 0;
        g_iTotalTracks = 0;     // g_iTotalTracks is the total count for jpeg applications
    }
    iLength = packed_strlen((_packed unsigned char *)sKey); // iLength is the comparing length of the key
    temp1 = (UCS3*)(&g_wUserScratchY[0]);   // unicode buffer for comparison below
    temp2 = (UCS3*)(&g_wUserScratchY[50]);
#ifdef MMC
    iDriveCount = 2;
    bReent = FALSE;
    if(g_is_SD_inserted == HAS_SD)
    {   EnterNonReentrantSection();
        bReent = TRUE;
    }
#else
    iDriveCount = 1;
#endif
    unpack_string(sKey,temp2);              // set up the comparing directory name in unicode in temp2
    for(iDrive=0;iDrive<iDriveCount;iDrive++)
    {   SetRoot(sRoot,iDrive);              // set up the name of root
        ((int*)sSpec)[0] = (int)'*' + (((int)'.') << 8) + (((int)'*') << 16);
        ((int*)sSpec)[1] = 0;
        if((Chdir((_packed char*)sRoot))<0) // change to root directory to find the target directory
        {   RetVal = PLAYLIST_TRACK_NOT_FOUND;
            break;
        }
        _memset(&_finddata,0,sizeof(_finddata));
    	_finddata.device = iDrive;
        _finddata.startrecord = 0;//start at the 0th record.
        if((HandleNumber = FindFirst(&_finddata,(_packed char*)sSpec))<0)
        {   RetVal = PLAYLIST_TRACK_NOT_FOUND;
            break;
        }

        SetUpPointer(); // set up the pointers for the total and current variables
        while(HandleNumber>0)
        {   if(_finddata.attrib&OTG_TYPE_IS_DIR)
            {   unpack_string((_packed BYTE*)_finddata.name,temp1); //if it is directory, get the directory name in unicode
                for(i=0;i<iLength;i++)
                {   if(temp1[i] != temp2[i]) break; // compare character by character
                }
                if(i>=iLength) // if matched
                {   dwJpegFileFastKey[iDrive] = _finddata.Key;  // get the directory key for the long file name used in JpegFile_GetCurrentSongFileInfo()
                    SetRoot(sRoot,iDrive);
                    packed_strcpy((_packed BYTE *)(sStartDir[iDrive]), sRoot);
                    packed_strcat((_packed BYTE *)(sStartDir[iDrive]), (_packed BYTE*)_finddata.name);
                    packed_strcat((_packed BYTE *)(sStartDir[iDrive]), (_packed BYTE *)"/");  // copy the full path of the target directory.
                    Fclose(HandleNumber);
    			    HandleNumber = -1;
                    if(iJpegPlaySet == PLAYSET_MVIDEO)
                    {   ((int*)sSpec)[0] = (int)'*' + (((int)'.') << 8) + (((int)'S') << 16);   // set up the correct file extension
                        ((int*)sSpec)[1] = (int)'M' + (((int)'V') << 8) + (((int)'\0') << 16);
                        ((int*)sSpec)[2] = 0;
                        *pwTotal += JpegGetTotalFileNum(iDrive);  // get the total number of files in g_wTotalSongCount
                        iExtWaterShed[iDrive] = *pwTotal;
                    }else if(iJpegPlaySet == PLAYSET_PHOTO)
                    {   ((int*)sSpec)[0] = (int)'*' + (((int)'.') << 8) + (((int)'J') << 16);   // set up the correct file extension
                        ((int*)sSpec)[1] = (int)'P' + (((int)'G') << 8) + (((int)'\0') << 16);
                        ((int*)sSpec)[2] = 0;
                        *pwTotal += JpegGetTotalFileNum(iDrive);     // get the total number of jpg files
                        iExtWaterShed[iDrive] = *pwTotal;             // iExtWaterShed is the total number of jpg files
                        ((int*)sSpec)[0] = (int)'*' + (((int)'.') << 8) + (((int)'B') << 16);   // set up the correct file extension
                        ((int*)sSpec)[1] = (int)'M' + (((int)'P') << 8) + (((int)'\0') << 16);
                        ((int*)sSpec)[2] = 0;
                        *pwTotal += JpegGetTotalFileNum(iDrive);    // get the total number of jpg and bmp files in g_iTotalTracks
                        if(*pwTotal > 0)
                        {   *pwCurrent = 1;
                        }
                    }
                    RetVal = PLAYLIST_SUCCESS;
                }
    		}
    		if(HandleNumber > 0)
    		{   if(FindNext(HandleNumber,&_finddata)<0 )
                {   Fclose(HandleNumber);
        			HandleNumber = -1;
                }
            }
    	}
    	if(iDrive == 0)
    	    iDriveWaterShed = *pwTotal;
    }
#ifdef MMC
    if(bReent == TRUE)
    {   LeaveNonReentrantSection();
    }
#endif
	return RetVal;
}

int _reentrant SetRoot(_packed BYTE *root, int iDrive)
{
    if(iDrive == 0)
    {   ((int*)root)[0] = (int)'A' + (((int)':') << 8) + (((int)'/') << 16);
        ((int*)root)[1] = 0;
    }else
    {   ((int*)root)[0] = (int)'B' + (((int)':') << 8) + (((int)'/') << 16);
        ((int*)root)[1] = 0;
    }
    return 0;
}

/***********************************************************************************************************/
// This routine count the total number of files with targeted extention
/***********************************************************************************************************/
int _reentrant JpegGetTotalFileNum(int iDrive)
{
    int         HandleNumber;
    int         RetVal=0;
	Finddata    _finddata;

    if((Chdir(sStartDir[iDrive]))<0)    // change to the desired directory first
        return RetVal;

    _memset(&_finddata,0,sizeof(_finddata));

    if((HandleNumber = FindFirst(&_finddata,sSpec))<0)
        return RetVal;

    while(HandleNumber>0)
    {   if(_finddata.attrib&OTG_TYPE_IS_FILE)
        {   RetVal++;
		}
		if(HandleNumber > 0)
    	{   if(FindNext(HandleNumber,&_finddata)<0 )
            {   Fclose(HandleNumber);
    			HandleNumber = -1;
            }
        }
	}

	return RetVal;
}

/***********************************************************************************************************/
// This routine set up the pointers of the total and current variables used by different playset.
// Jpeg uses the g_iTotalTracks and g_CurrentTrack.iTrackOrder while
// motion video uses the g_wTotalSongCount and g_wCurrentSongNumber
/***********************************************************************************************************/
void SetUpPointer(void)
{
	if(iJpegPlaySet == PLAYSET_MVIDEO)
	{   pwTotal = &g_wTotalSongCount;
	    pwCurrent = &g_wCurrentSongNumber;
	}else if(iJpegPlaySet == PLAYSET_PHOTO)
	{   pwTotal = (WORD*)&g_iTotalTracks;
	    pwCurrent = &(g_CurrentTrack.iTrackOrder);
    }
}
#endif