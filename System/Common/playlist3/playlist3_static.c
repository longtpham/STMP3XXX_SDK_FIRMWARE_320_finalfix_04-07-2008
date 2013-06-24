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
#include "sysmem.h"
#include "FileSystem.h"
#include "fserror.h"
#include "FileSpec.h"
#include "playlist.h"
#include "syscoder.h"
///////////////Extern FAT32 functions/////////////////////////////////
extern _reentrant INT Chdir(_packed char *filepath);
extern _reentrant INT FindFirst(Finddata *_finddata,_packed char *FileName);
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern _reentrant INT Fclose(INT HandleNumber);
extern _reentrant INT GetShortfilename(LONG Key,INT *Buffer);
extern _reentrant LONG GetRootdirkey(INT DeviceNumber);
extern _reentrant INT FastOpen(LONG Key,_packed char *mode);
extern _reentrant INT ConstructLongFileName(INT HandleNumber, INT RecordNumber, INT *LFNBuffer);
extern INT DriveLetter[];
//////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant Playlist_GetPlaySet(void)
//
//   Type:          Function
//
//   Description:   Returns the current playset (PLAYSET_MUSIC or PLAYSET_VOICE)
//
//   Inputs:        none
//
//   Outputs:       INT             current playset
//
//   Notes:         
//<
//////////////////////////////////////////////////////
INT _reentrant Playlist_GetPlaySet(void)
{
    return g_iPlaySet;
}

_reentrant RETCODE Playlist_GetRootString(_packed BYTE* pBuffer, INT iDevice){
	int j;
	if(iDevice>MAX_LOGICAL_DEVICES)
		return PLAYLIST_FILE_ERROR;
	else
	{
		packed_strcpy(pBuffer,(_packed BYTE*)&DriveLetter[iDevice]);
		j = ROOT_SEPARATOR ;	// ":\"
		packed_strcat(pBuffer, (_packed BYTE*) &j);	//gives you "a:\" or "b:\"
		return PLAYLIST_SUCCESS;
	}
}

_reentrant DWORD Playlist_GenerateFastKey(INT iDevice,LONG DirSector,INT iDirOffset){
	DWORD dwFastKey;
	dwFastKey = (((DWORD)iDevice << 44) | ((DWORD)iDirOffset<<32) | (LONG)DirSector);
	return dwFastKey;
}


_reentrant INT ExtractDirSector1(DWORD Key){
	return((INT)(Key & 0x00001FFFFF));

}
_reentrant INT ExtractDirSector2(DWORD Key){
	return((INT)((Key & 0x00FFFFFFFF)>>21));

}
_reentrant INT ExtractDirOffset(DWORD Key){
	return((INT)((Key >> 32)& 0xFFF));

}
_reentrant RETCODE Playlist_GetSFN(INT iEntryType , INT pEntry, INT* pName){
	return PLAYLIST_SUCCESS;
}
// MLP: 6/29/06
// These routines are needed in the STFM code so if we don't build with
// Playlist2 option we get unresolved externals.  It does not appear that
// these functions are used by any other code so I'm moving them to
// stfm1000.c
//DWORD GetDclkCount(void)
//{
//    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
//}

