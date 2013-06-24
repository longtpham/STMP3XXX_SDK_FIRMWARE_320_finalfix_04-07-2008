#include "types.h"
#include "menumanager.h"
#include "playlist.h"       //required include file
#include "playerlib.h"
#include "project.h"
#include "stringlib.h"
#include "resource.h"
#include "sysspeed.h"
#include "playlist2.h"
#include "playlist2internal.h"
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
//////////////////////////////////////////////////////
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
#if 0   //move to playlist2init.c
//_reentrant INT ExtractDirSector1(DWORD Key){
//	return((INT)(Key & 0x00001FFFFF));
//
//}
//_reentrant INT ExtractDirSector2(DWORD Key){
//	return((INT)((Key & 0x00FFFFFFFF)>>21));
//
//}
//_reentrant INT ExtractDirOffset(DWORD Key){
//	return((INT)((Key >> 32)& 0xFFF));
//
//}
#endif

_reentrant RETCODE Playlist_GetSFN(INT iEntryType , INT pEntry, INT* pName){
	DirEntry _X* pDirEntry;
	FileEntry* pFileEntry;
	DWORD dwFastKey;
    LONG DirSector;
	INT j;
	RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;
	switch(iEntryType)
	{
	case TYPE_DIR:
		pDirEntry = (DirEntry _X*)pEntry;
		if(pDirEntry->m_bIsRoot)
		{
			rtn = Playlist_GetRootString((_packed BYTE*) pName,pDirEntry->m_iDevice);
		}
		else
		{
            DirSector= (LONG)pDirEntry->m_iDirSector1 | (((LONG)pDirEntry->m_iDirSector2) << 21);
			dwFastKey=Playlist_GenerateFastKey(pDirEntry->m_iDevice,DirSector,pDirEntry->m_iDirOffset);
			if(GetShortfilename((LONG) dwFastKey,(INT*)pName) == FS_SUCCESS)
			{
				j= DIR_SEPARATOR;
				packed_strcat((_packed BYTE*) pName,(_packed BYTE*)&j);
				rtn = PLAYLIST_SUCCESS;
			}
		}
		break;
	case TYPE_FILE:
		pFileEntry = (FileEntry*)pEntry;
        DirSector= (LONG)pFileEntry->m_iDirSector1 |(((LONG)pFileEntry->m_iDirSector2) << 21);
        dwFastKey=Playlist_GenerateFastKey(pFileEntry->m_iDevice,DirSector,pFileEntry->m_iDirOffset);
		if(GetShortfilename((LONG) dwFastKey,(INT*)pName) == FS_SUCCESS)
			rtn = PLAYLIST_SUCCESS;
		break;
	}
	return rtn;
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
//void SetDclkCount(DWORD dwCount)
//{
//    // TODO:  protect this from interrupts
//    HW_DCLKCNTU.I = (WORD)(dwCount >> 24);
//    HW_DCLKCNTL.I = (WORD)(dwCount);
//}
// MLP: End