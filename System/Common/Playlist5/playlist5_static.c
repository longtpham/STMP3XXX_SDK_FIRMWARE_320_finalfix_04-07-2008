#include "types.h"
#include "playlist.h"       //required include file
#include "stringlib.h"
#include "playlist5.h"
#include "fserror.h"

///////////////Extern FAT32 functions/////////////////////////////////
extern _reentrant INT GetShortfilename(LONG Key,INT *Buffer);
extern _reentrant INT ChdirFromOffset(_packed char *filepath,INT RecordNumber);
extern _reentrant INT Chdir(_packed char *filepath);
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

_reentrant RETCODE Playlist_GetRootString(_packed BYTE* pBuffer, INT iDevice)
{
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

_reentrant RETCODE Playlist_GetSFN (PL5_FOLDER_RECORD * ptFolderRec, _packed BYTE * pszDirName)
{
	int j;
	RETCODE rtn = PLAYLIST_TRACK_NOT_FOUND;

	if (ptFolderRec->bIsDir)
	{
	    if(ptFolderRec->bIsRoot)
    	{
	    	rtn = Playlist_GetRootString((_packed BYTE*) pszDirName,ptFolderRec->iDevice);
		}
		else
		{
			if(GetShortfilename(ptFolderRec->lFastKey,(INT*)pszDirName) == FS_SUCCESS)
			{
				j= DIR_SEPARATOR;
				packed_strcat((_packed BYTE*) pszDirName,(_packed BYTE*)&j);
				rtn = PLAYLIST_SUCCESS;
			}
		}
	}
	else
	{
		if(GetShortfilename(ptFolderRec->lFastKey,(INT*)pszDirName) == FS_SUCCESS)
			rtn = PLAYLIST_SUCCESS;
	}

	return rtn;
}

_reentrant INT PL5_GetFileExtention(_packed BYTE* pFileName)
{
	WORD wFileExtension = 0;
	WORD wStrLen;
	int i;

	//Get length of file name
	wStrLen= packed_strlen(pFileName);

	//Find the "."; Traverse backwards from EOF
    while(wStrLen--)
    {
        if (packed_get(pFileName,wStrLen) == '.')
        	break;
    }

    if (wStrLen)
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


/*----------------------------------------------------------------------------

   Function Name:  _reentrant INT PL5_Chdir(_packed char *filepath, INT RecordNumber)

   FunctionType:   Reentrant

   Inputs:        1) Pointer to folder name
                  2) Directory Offset from where to search the next record from.

   Outputs:       FS_SUCCESS OR error code if error occurs

   Description:  Searches for the given directoy path in corrent working directory and
                if the path is found then changes the current working directoy to the
                given directory path.
----------------------------------------------------------------------------*/
_reentrant INT PL5_Chdir (_packed char *filepath, INT RecordNumber)
{
    if (((filepath[0] & 0xffff00) == 0x2f3a00) || (filepath[0] == 0x2e2e00))
        return Chdir(filepath);
    else
        return ChdirFromOffset(filepath, RecordNumber);
}