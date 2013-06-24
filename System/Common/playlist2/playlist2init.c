////////////////////////////////////////////////////////////////////////////////
// Copyright (c) SigmaTel, Inc. 2003-2006
// Filename: playlist2.c
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
#include "playlist2.h"
#include "playlist2internal.h"
#include "sysmem.h"
#include "FileSystem.h"
#include "fserror.h"
#include "FileSpec.h"
#include "playlist.h"
#include "syscoder.h"

#pragma optimize 1

int g_iHighestVoiceNumber = 0;
int g_iHighestFMNumber = 0;
int g_iHighestLineNumber = 0;
int* pHighestNumber;

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
//////////////////////////////////////////////////////////////////////
int g_iTotalDir=0;
int g_iTotalFiles=0;
int g_iNumStoredSFN = 0;


#ifdef USE_PLAYLIST2
BOOL g_Rebuild = FALSE;
#endif


//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant Playlist_BuildContentTree(void)
//
//   Type:          Function
//
//   Description:   clears out the old content tree and then cycles through
//                  each logical device to build the new content tree.
//
//   Inputs:
//
//   Outputs:
//
//   Notes:         This function will change the current directory on each
//                  device to the root.
//<
//////////////////////////////////////////////////////
INT _reentrant Playlist_BuildContentTree(INT iIgnored1, INT iIgnored2, INT* pIgnored)
{
    int i;
//	int j;
	_packed BYTE pszName[SFN_LENGTH];
	pszName[0]=0;		//make NULL for string operations

    // Use C Speed client  Increase call. Defines are in SysSpeed.inc. Sysspeed.h is generated
    SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_PLAYLIST1);
#ifdef USE_PLAYLIST3
	dStart = GetDclkCount();
#else
	dStart = GetDclkCount_PL2();
#endif
	//Clear ROOT
	for(i=0;i<MAX_LOGICAL_DEVICES;i++)
		_memset(&g_DirEntryPerDevice[i],0,sizeof(g_DirEntryPerDevice[i]));

	//Clear DirEntryPool
    for(i=0;i<MAX_NUM_DIR;i++)
       _memset(&g_DirEntryPool[i],0,sizeof(g_DirEntryPool[i]));

	//Clear FileEntryPool
    for(i=0;i<MAX_NUM_FILES;i++)
        _memset(&g_FileEntryPool[i],0,sizeof(g_FileEntryPool[i]));

    for(i=0;i<MAX_LOGICAL_DEVICES;i++)
    {//cycle through the logical devices

        g_DirEntryPerDevice[i].m_bAllocated = TRUE;
        g_DirEntryPerDevice[i].m_iDevice = i;
		g_DirEntryPerDevice[i].m_bIsRoot = TRUE;
		g_DirEntryPerDevice[i].m_iRecordNum = 0;	//keep 0 for roots.
		g_DirEntryPerDevice[i].m_iDirSector1 = 0;		//keep 0 for roots.
		g_DirEntryPerDevice[i].m_iDirSector2 = 0;		//keep 0 for roots.
		g_DirEntryPerDevice[i].m_iDirOffset = 0;		//keep 0 for roots.

		Playlist_GetSFN(TYPE_DIR, (INT) &g_DirEntryPerDevice[i],(INT*)pszName);
		if(Chdir((_packed char*) pszName) == FS_SUCCESS)
		{//change to the root, if successful, populate the content tree from that node
            if(Playlist_PopulateDirectory(&g_DirEntryPerDevice[i],0) == PLAYLIST_END_OF_LIST)
				break;	//no more room available
        }
        else
        {//otherwise, mark it as invalid
			//__asm(" debug");
            g_DirEntryPerDevice[i].m_bAllocated=FALSE;
        }
    }
#ifdef USE_PLAYLIST3
	dEnd = GetDclkCount();
#else
	dEnd = GetDclkCount_PL2();
#endif
	dDiff = dEnd - dStart;
    //SET SPEED BACK TO NORMAL HERE
    SysSpeedClockFree(SPEED_CLIENT_PLAYLIST1); // C Speed client frees the shared speed resource
	return PLAYLIST_SUCCESS;
}

//////////////////////////////////////////////////////
//
//>  Name:          _reentrant void Playlist_PopulateDirectory(FileEntry *pEntry, int depth)
//
//   Type:          Function
//
//   Description:   recursively populates the content tree starting at the entry pointed to by pEntry.
//
//   Inputs:        FileEntry *pEntry        points to a directory node to parse/populate
//                  int depth                       used to prevent it from scanning so deep it overflows some buffers
//
//   Outputs:       if the input has any contents, they will be included in the m_pContents node chain
//
//   Notes:         This function is a true reentrant/recursive function.   It will build a content tree
//                  onto the current file entry from the directory pointed toward in the file entry.
//
//<
//////////////////////////////////////////////////////
_reentrant RETCODE Playlist_PopulateDirectory(DirEntry _X *pParentDirEntry, int depth)
{
	DirEntry _X * pCurrentDir;
	DirEntry _X * pPrevDir;
	_packed BYTE pszDirName[SFN_LENGTH];
	int iDevice, j;
	RETCODE rtn;
	rtn = PLAYLIST_SUCCESS;
	pszDirName[0]=0;

	Playlist_GetSFN(TYPE_DIR, (INT) pParentDirEntry, (INT*)pszDirName);
	if(Chdir((_packed char*)pszDirName) == FS_SUCCESS)
    {//change into the directory named in the Dirname string
        DirEntry _X *pCurrentDirEntry=pParentDirEntry;
        _packed BYTE sFileSpec[4];
        Finddata finddata;
        int i;
        int find_handle ;
		ClearSFNStorageSpace(0,0,0);	//since we are in a new directory now, so sort from here.
        ((int*)(sFileSpec))[0]=0x2a2e2a; //*.*
        ((int*)(sFileSpec))[1]=0; //null terminator
		_memset(&finddata,0,sizeof(finddata));
        iDevice = pParentDirEntry->m_iDevice;
		finddata.device = iDevice;
        finddata.startrecord = 0;//start at the 0th record.

        //get the first that matches our specifications
        find_handle = FindFirst(&finddata,(_packed char*) sFileSpec);
        while((find_handle > 0))
        {//while our find handle is valid, keep getting more files
            if((!(finddata.attrib & ATTR_UNACCEPTABLE))
					&& finddata.startrecord<MAX_RECORDS_PER_DIRECTORY)
            {//if the attributes of the file found are acceptable (this will skip hidden, system, or volume id entries.  It also skips long file entries
                if(finddata.attrib & (ATTR_DIRECTORY))
				{
					DirEntry _X *pNewDirEntry;
					if(! Playlist_ValidateEntry(TYPE_DIR,IS_TRASH,(INT*)&finddata))	//ignore MAC_TRASH
					{
						pNewDirEntry = Playlist_GetFreeDirEntry();
						if(pNewDirEntry)
						{//only parse further if we have a valid Dir entry.  If we run out...we stop
							pNewDirEntry->m_bAllocated = TRUE;
							pNewDirEntry->m_iDevice=iDevice;
							pNewDirEntry->m_iRecordNum = finddata.startrecord - 1;	//find increments the record number
							pNewDirEntry->m_bIsRoot = FALSE;		//only one ROOT per device.
							pNewDirEntry->m_iDirSector1 = ExtractDirSector1(finddata.Key);
                            pNewDirEntry->m_iDirSector2 = ExtractDirSector2(finddata.Key);
							pNewDirEntry->m_iDirOffset = ExtractDirOffset(finddata.Key);
#ifdef DEBUG_SFN
							packed_strcpy(pNewDirEntry->name, (_packed BYTE*) finddata.name);
							j = DIR_SEPARATOR ;	// "\"
							packed_strcat(pNewDirEntry->name, (_packed BYTE*) &j);
#endif
							Playlist_InsertSortDir(pParentDirEntry,pNewDirEntry, (_packed BYTE*) finddata.name);
							g_iTotalDir++;		//for debug purposes
						}
// stmp6726	Allow playlist with more than 50 directories.					else	//out of space
// stmp6726	Allow playlist with more than 50 directories.						rtn = PLAYLIST_END_OF_LIST;
					}
				}
			   else  //otherwise it is a file so enter it in the FileEntryPool
			   {
					FileEntry *pNewFileEntry;
					if(Playlist_ValidateEntry(TYPE_FILE,IS_VALID_AUDIO, (INT*)&finddata))
					{
						pNewFileEntry = Playlist_GetFreeFileEntry();
						if(pNewFileEntry)
						{
							pNewFileEntry->m_bAllocated = TRUE;
							pNewFileEntry->m_iDevice=iDevice;
#ifdef PLAYLIST2_MULTIMEDIA
							pNewFileEntry->m_iMediaType = ENTRY_IS_AUDIO;
#endif
							pNewFileEntry->m_iRecordNum = finddata.startrecord - 1;	//find increments the record number
							pNewFileEntry->m_iDirSector1 = ExtractDirSector1(finddata.Key);
							pNewFileEntry->m_iDirSector2 = ExtractDirSector2(finddata.Key);
							pNewFileEntry->m_iDirOffset = ExtractDirOffset(finddata.Key);
#ifdef DEBUG_SFN
							packed_strcpy(pNewFileEntry->name, (_packed BYTE*) finddata.name);
#endif
							Playlist_InsertSortFile(pParentDirEntry,pNewFileEntry,(_packed BYTE*) finddata.name);
							g_iTotalFiles++;		//for debug purposes.
						}
					   else
						   rtn = PLAYLIST_END_OF_LIST;	//out of space
					}
#ifdef PLAYLIST2_MULTIMEDIA
					else if(Playlist_ValidateEntry(TYPE_FILE,IS_VALID_PHOTO, (INT*)&finddata))
					{
						pNewFileEntry = Playlist_GetFreeFileEntry();
						if(pNewFileEntry)
						{
							pNewFileEntry->m_bAllocated = TRUE;
							pNewFileEntry->m_iDevice=iDevice;
							pNewFileEntry->m_iMediaType = ENTRY_IS_PHOTO;
							pNewFileEntry->m_iRecordNum = finddata.startrecord - 1;	//find increments the record number
							pNewFileEntry->m_iDirSector1 = ExtractDirSector1(finddata.Key);
							pNewFileEntry->m_iDirSector2 = ExtractDirSector2(finddata.Key);
							pNewFileEntry->m_iDirOffset = ExtractDirOffset(finddata.Key);
#ifdef DEBUG_SFN
							packed_strcpy(pNewFileEntry->name, (_packed BYTE*) finddata.name);
#endif
							Playlist_InsertSortFile(pParentDirEntry,pNewFileEntry,(_packed BYTE*) finddata.name);
							g_iTotalFiles++;		//for debug purposes.
						}
					   else
						   rtn = PLAYLIST_END_OF_LIST;	//out of space
					}
					else if(Playlist_ValidateEntry(TYPE_FILE,IS_VALID_MVIDEO, (INT*)&finddata))
					{
						pNewFileEntry = Playlist_GetFreeFileEntry();
						if(pNewFileEntry)
						{
							pNewFileEntry->m_bAllocated = TRUE;
							pNewFileEntry->m_iDevice=iDevice;
							pNewFileEntry->m_iMediaType = ENTRY_IS_MVIDEO;
							pNewFileEntry->m_iRecordNum = finddata.startrecord - 1;	//find increments the record number
							pNewFileEntry->m_iDirSector1 = ExtractDirSector1(finddata.Key);
							pNewFileEntry->m_iDirSector2 = ExtractDirSector2(finddata.Key);
							pNewFileEntry->m_iDirOffset = ExtractDirOffset(finddata.Key);
#ifdef DEBUG_SFN
							packed_strcpy(pNewFileEntry->name, (_packed BYTE*) finddata.name);
#endif
							Playlist_InsertSortFile(pParentDirEntry,pNewFileEntry,(_packed BYTE*) finddata.name);
							g_iTotalFiles++;		//for debug purposes.
						}
					   else
						   rtn = PLAYLIST_END_OF_LIST;	//out of space
					}
#endif // PLAYLIST2_MULTIMEDIA
			   }
			}
			//now get the next record
			_memset(&finddata,0,sizeof(finddata));

			if(rtn == PLAYLIST_END_OF_LIST					//if we aren't out of room
				|| (FindNext(find_handle,&finddata)<0))	//if we still have records to fetch from media.
			{//if there's no next entry, lets close it out.

				Fclose(find_handle);
				find_handle = -1;
			}
		}

        //go back and populate all of those entries that happen to be directories.
        pCurrentDirEntry = (DirEntry _X *)(pParentDirEntry->m_pDirContents);
		pPrevDir = NULL; //needed to deallocate and relink empty directories

        while((rtn != PLAYLIST_END_OF_LIST) && pCurrentDirEntry)
        {
            if((pCurrentDirEntry->m_bAllocated) && (depth < _MAX_DIR_DEPTH))
            {
                rtn = Playlist_PopulateDirectory(pCurrentDirEntry, depth+1);
            }
#if 0
            pCurrentDir = pCurrentDirEntry; //Save CurrentDirectory - needed to pass in to DeAllocateEmptyDir below.
            pCurrentDirEntry= (DirEntry _X *)(pCurrentDirEntry->m_pNext);
            if( DeAllocateEmptyDir(pCurrentDir,pPrevDir,pParentDirEntry) != PLAYLIST_SUCCESS) //entry not empty
              pPrevDir = pCurrentDir;
#else       // older ver
            pCurrentDirEntry= (DirEntry _X *)(pCurrentDirEntry->m_pNext);
#endif
        }
        //go back up one level so the filesystem is in the same state as when we entered it
        ((int*)(sFileSpec))[0]=0x002e2e;
		Chdir((_packed char*) sFileSpec);
    }
    else
    {
		//__asm("debug");
 		pParentDirEntry->m_bAllocated=FALSE;//return it to the pool since we can't change directories to it
    }
	return rtn;
}
#if 0
_reentrant RETCODE DeAllocateEmptyDir(DirEntry _X *pCurrentDirEntry, DirEntry _X* pPrevDirEntry, DirEntry _X* pParentDirEntry)
{
	if((pCurrentDirEntry->m_pDirContents == NULL ) && (pCurrentDirEntry->m_pFileContents == NULL))
	{
		if(pPrevDirEntry)
			pPrevDirEntry->m_pNext = pCurrentDirEntry->m_pNext;
		else //first in the list so re-link the parent to point to the next
			pParentDirEntry->m_pDirContents = pCurrentDirEntry->m_pNext;

		pCurrentDirEntry->m_bAllocated = FALSE;//return back to the pool
		g_iTotalDir--; //one less
		return PLAYLIST_SUCCESS;
	}
	return PLAYLIST_DIR_NOT_EMPTY;
}
#endif
//////////////////////////////////////////////////////
//
//>  Name:          _reentrant FileEntry *Playlist_GetFreeDirEntry(void)
//
//   Type:          Function
//
//   Description:   gets an empty DirEntry from the g_DirEntryPool
//
//   Inputs:        none
//
//   Outputs:       pointer to a unused DirEntry, or NULL if none exist
//
//   Notes:
//<
//////////////////////////////////////////////////////
_reentrant DirEntry _X *Playlist_GetFreeDirEntry(void)
{
    int i;
    DirEntry _X *pEntry =NULL;
    for(i=0;i<MAX_NUM_DIR;i++)
    {
        if(g_DirEntryPool[i].m_bAllocated == FALSE)
        {
            pEntry = &g_DirEntryPool[i];
            pEntry->m_pNext = NULL;
            pEntry->m_pDirContents = NULL;
			pEntry->m_pFileContents = NULL;
            break;
        }
    }
    return pEntry;
}
//////////////////////////////////////////////////////
//
//>  Name:          _reentrant FileEntry *Playlist_GetFreeFileEntry(void)
//
//   Type:          Function
//
//   Description:   gets an empty FileEntry from g_FileEntryPool
//
//   Inputs:        none
//
//   Outputs:       pointer to a unused FileEntry, or NULL if none exist
//
//   Notes:
//<
//////////////////////////////////////////////////////
_reentrant FileEntry *Playlist_GetFreeFileEntry(void)
{
    int i;
    FileEntry *pFileEntry =NULL;
    for(i=0;i<MAX_NUM_FILES;i++)
    {
        if(g_FileEntryPool[i].m_bAllocated == FALSE)
        {
            pFileEntry = &g_FileEntryPool[i];
            pFileEntry->m_pNext = NULL;
            break;
        }
    }
    return pFileEntry;
}
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
       (wFileExtension==WAV_FILE_EXT))
	{//Valid Extension
        	rtn = TRUE;
	}
#ifdef AUDIBLE
    if(wFileExtension==AUDIBLE_FILE_EXT)
    {
        rtn=TRUE;
    }
#endif
		//Check for MAC Resource
       	if (packed_get(pFileName,0) == '_')
       	{ // first character is _, so check size
        	if (lFileSize <= MAC_RESOURCE_NUM_BYTES)
           	{ // if <= 1kB, remove from playlist
            	rtn = FALSE;
           	}
       	}

	return rtn;
}

#ifdef PLAYLIST2_MULTIMEDIA
//////////////////////////////////////////////////////
//
//>  Name:          BOOL _reentrant Playlist_IsValidPhotoFile(LONG lFileSize, _packed BYTE* pFileName)
//
//   Type:          Function
//
//   Description:   Determine if the file is an photo file
//
//   Inputs:        LONG File Size; _packed BYTE* File Name String
//
//   Outputs:       BOOL True/False
//
//   Notes: 	 Function will check for a valid file extension and will
//				 check for MAC resources
//<
//////////////////////////////////////////////////////
BOOL _reentrant Playlist_IsValidPhotoFile(LONG lFileSize, _packed BYTE* pFileName)
{
	BOOL rtn = FALSE;
	WORD wFileExtension;

	//Get the file extension
	wFileExtension = Playlist_GetFileExtention(0,0,(INT*)pFileName);

    //Check the extension for a valid case
#if 1
    if((wFileExtension==JPG_FILE_EXT) ||
       (wFileExtension==BMP_FILE_EXT))
#else
	if(wFileExtension==JPG_FILE_EXT)
#endif
	{//Valid Extension
        	rtn = TRUE;
	}
		//Check for MAC Resource
       	if (packed_get(pFileName,0) == '_')
       	{ // first character is _, so check size
        	if (lFileSize <= MAC_RESOURCE_NUM_BYTES)
           	{ // if <= 1kB, remove from playlist
            	rtn = FALSE;
           	}
       	}

	return rtn;
}


//////////////////////////////////////////////////////
//
//>  Name:          BOOL _reentrant Playlist_IsValidMotionVideoFile(LONG lFileSize, _packed BYTE* pFileName)
//
//   Type:          Function
//
//   Description:   Determine if the file is an motion video file
//
//   Inputs:        LONG File Size; _packed BYTE* File Name String
//
//   Outputs:       BOOL True/False
//
//   Notes: 	 Function will check for a valid file extension and will
//				 check for MAC resources
//<
//////////////////////////////////////////////////////
BOOL _reentrant Playlist_IsValidMotionVideoFile(LONG lFileSize, _packed BYTE* pFileName)
{
	BOOL rtn = FALSE;
	WORD wFileExtension;

	//Get the file extension
	wFileExtension = Playlist_GetFileExtention(0,0,(INT*)pFileName);

    //Check the extension for a valid case
	if(wFileExtension==SMV_FILE_EXT)
	{//Valid Extension
        	rtn = TRUE;
	}

		//Check for MAC Resource
       	if (packed_get(pFileName,0) == '_')
       	{ // first character is _, so check size
        	if (lFileSize <= MAC_RESOURCE_NUM_BYTES)
           	{ // if <= 1kB, remove from playlist
            	rtn = FALSE;
           	}
       	}

	return rtn;
}
#endif //PLAYLIST2_MULTIMEDIA

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

//////////////////////////////////////////////////////////////////////////////////////////////////
/////////Insert a directory in the directory pool in the sorted order/////////////////////////////
///////////HERE USER CAN SORT BASED ON ANY CRITERIA///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
void _reentrant Playlist_InsertSortDir(DirEntry _X* pParentDirEntry,DirEntry _X* pNewDirEntry, _packed BYTE* pNewDirName){
	DirEntry _X *pPreviousLink;
	DirEntry _X *pCurrentDirEntry;
	BOOL	IsFirst=TRUE;
#ifndef PERFORM_SORT
	//no sort
	if(pParentDirEntry && pNewDirEntry)
	{
		pPreviousLink = pParentDirEntry;
		pCurrentDirEntry = (DirEntry _X *)(pParentDirEntry->m_pDirContents);
		while(pCurrentDirEntry)	//till we have contents
		{
			IsFirst = FALSE;
			pPreviousLink = pCurrentDirEntry;
			pCurrentDirEntry = (DirEntry _X *)(pCurrentDirEntry->m_pNext);
		}
		if(IsFirst)
			pPreviousLink->m_pDirContents = (INT) pNewDirEntry;
		else
			pPreviousLink->m_pNext = (INT) pNewDirEntry;

		pNewDirEntry->m_pNext = NULL;
	}
#else
	//sort SFN
	_packed BYTE CurrentDirName[SFN_LENGTH];
	CurrentDirName[0] = 0;

	if(pParentDirEntry && pNewDirEntry)
	{
		pPreviousLink = pParentDirEntry;
		pCurrentDirEntry = (DirEntry _X *)(pParentDirEntry->m_pDirContents);
		while(pCurrentDirEntry)
		{
			if((g_iNumStoredSFN < MAX_SORT_ENTRIES_PER_DIRECTORY) && //if we have not exceed our SFN storage space
					(GetSFNFromTempArray((void*) pCurrentDirEntry, CurrentDirName) == PLAYLIST_SUCCESS))
			{
				//here compare strings and break when the correct sorted order is found
				if(packed_strcmp(pNewDirName,CurrentDirName)<0)
					break;
			}
			IsFirst = FALSE;
			pPreviousLink = pCurrentDirEntry;
			pCurrentDirEntry = (DirEntry _X*) (pCurrentDirEntry->m_pNext);
		}

		if(IsFirst)			//If this is the first in the list, update the m_pDirContent pointer for the parent.
			pPreviousLink->m_pDirContents = (INT) pNewDirEntry;
		else				//Else this is not first, so update the m_pNext pointer
			pPreviousLink->m_pNext = (INT) pNewDirEntry;

		pNewDirEntry->m_pNext = (INT) pCurrentDirEntry;
		//now insert the SFN of the new dir entry in the temp SFN array.
		StoreSFNInTempArray((void*)pNewDirEntry,pNewDirName);
	}
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/////////Insert a file in the file pool in the sorted order///////////////////////////////////////
///////////HERE USER CAN SORT BASED ON ANY CRITERIA///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
void _reentrant Playlist_InsertSortFile(DirEntry _X* pParentDirEntry,FileEntry* pNewFileEntry, _packed BYTE* pNewFileName){
	void *pPreviousLink;
	FileEntry *pCurrentFileEntry;
	BOOL	IsFirst=TRUE;
#ifndef PERFORM_SORT
	//no sort
	if(pParentDirEntry && pNewFileEntry)
	{
		pCurrentFileEntry = (FileEntry *) (pParentDirEntry->m_pFileContents);
		while(pCurrentFileEntry)	//till we have contents
		{
			IsFirst = FALSE;
			pPreviousLink = (FileEntry *) pCurrentFileEntry;
			pCurrentFileEntry = (FileEntry *)(pCurrentFileEntry->m_pNext);
		}
		if(IsFirst)
			((DirEntry _X*)pParentDirEntry)->m_pFileContents = (INT)pNewFileEntry;
		else
			((FileEntry*)pPreviousLink)->m_pNext = (INT)pNewFileEntry;

		pNewFileEntry->m_pNext = NULL;
	}
#else
	//sort SFN
	_packed BYTE CurrentFileName[SFN_LENGTH];
	CurrentFileName[0] = 0;

	if(pParentDirEntry && pNewFileEntry)
	{		
		pCurrentFileEntry = (FileEntry *) (pParentDirEntry->m_pFileContents);
		while(pCurrentFileEntry)
		{
			if((g_iNumStoredSFN < MAX_SORT_ENTRIES_PER_DIRECTORY) && //if we have not exceed our SFN storage space
					(GetSFNFromTempArray((void*) pCurrentFileEntry, CurrentFileName) == PLAYLIST_SUCCESS))
			{
				//here compare strings and break when the correct sorted order is found
				if(packed_strcmp(pNewFileName,CurrentFileName)<0)
					break;
			}
			IsFirst = FALSE;
			pPreviousLink = (FileEntry *) pCurrentFileEntry;
			pCurrentFileEntry = (FileEntry *)(pCurrentFileEntry->m_pNext);
		}

		if(IsFirst)
            ((DirEntry _X*)pParentDirEntry)->m_pFileContents = (INT)pNewFileEntry;
		else
			((FileEntry*)pPreviousLink)->m_pNext = (INT)pNewFileEntry;

		pNewFileEntry->m_pNext = (INT) pCurrentFileEntry;
		StoreSFNInTempArray((void*)pNewFileEntry,pNewFileName);
	}
#endif
}
/////////////////////////////////////////////////////////
//playlist2 helper functions
/////////////////////////////////////////////////////////
BOOL _reentrant Playlist_ValidateEntry(INT iEntryType,INT iReason, INT* pPtr)
{
	Finddata* pFinddata;
	BOOL rtn = FALSE;
	pFinddata = (Finddata*) pPtr;
	switch(iEntryType)
	{
		case TYPE_DIR:
			switch(iReason)
			{
				case IS_TRASH:
					if(IsTrashDir((_packed BYTE*) pFinddata->name))
						rtn = TRUE;
					break;
			}
			break;

		case TYPE_FILE:
			switch(iReason)
			{
				case IS_VALID_AUDIO:
					if(Playlist_IsValidAudioFile(pFinddata->FileSize,(_packed BYTE*)pFinddata->name))
						rtn = TRUE;
					break;
#ifdef PLAYLIST2_MULTIMEDIA
				case IS_VALID_PHOTO:
	                if(Playlist_IsValidPhotoFile(pFinddata->FileSize,(_packed BYTE*)pFinddata->name))
					    rtn = TRUE;
					break;
				case IS_VALID_MVIDEO:
	               if(Playlist_IsValidMotionVideoFile(pFinddata->FileSize,(_packed BYTE*)pFinddata->name))
					    rtn = TRUE;
					break;
#endif
			}
			break;
	}
	return rtn;
}

//////////////////////////////////////////////////////
//
//>  Name:          _reentrant BOOL IsTrashDir(_packed BYTE* pszName)
//
//   Type:          Function
//
//   Description:
//
//   Inputs:        Packed string
//
//   Outputs:       TRUE if "\TRASH\" or else FALSE
//
//   Notes:
//<
//////////////////////////////////////////////////////

_reentrant BOOL IsTrashDir(_packed BYTE* pszName)
{
	_packed BYTE szTrash[9];
	BOOL bTrash=FALSE;

	    // Check subdirectory for MAC trash cans
        // OS9 /TRASH/
        // OSX /TRASHE~1/

        //                      A R T
        ((int*)szTrash)[0] = 0x415254;

	    //                        H S
        ((int*)szTrash)[1] = 0x004853;

		if(!packed_strcmp((_packed BYTE *)pszName,szTrash))
        {  // OS9 trash directory
            bTrash = TRUE;
        }
        else
        {
            //                      A R T
            // ((int*)szTrash)[0] = 0x415254; // same as before
            //                      E H S
            ((int*)szTrash)[1] = 0x454853;
            //                        1 ~
            ((int*)szTrash)[2] = 0x00317E;
            if(!packed_strcmp((_packed BYTE *)pszName,szTrash))
            {   // OSX trash directory
                bTrash = TRUE;
            }
        }


		return bTrash;

}

void _reentrant ClearSFNStorageSpace(INT iUnused1, INT iUnused2, INT* pPtr){
	int i;
	g_iNumStoredSFN= 0;
    for(i=0;i<MAX_SORT_ENTRIES_PER_DIRECTORY;i++)
        _memset(&g_TempSFNSpace[i],0,sizeof(g_TempSFNSpace[i]));
}

INT _reentrant GetSFNFromTempArray(void* pEntry, _packed BYTE* pName){
	int i;
	RETCODE rtn = PLAYLIST_END_OF_LIST;

    for(i=0;i<MAX_SORT_ENTRIES_PER_DIRECTORY;i++){
		if(g_TempSFNSpace[i].pEntry == pEntry){
			packed_strcpy(pName,g_TempSFNSpace[i].name);
			rtn = PLAYLIST_SUCCESS;
			break;
		}
	}
	return rtn;
}

void _reentrant StoreSFNInTempArray(void* pEntry, _packed BYTE* pName){
	int i;
	if(g_iNumStoredSFN<MAX_SORT_ENTRIES_PER_DIRECTORY)
	{
		for(i=0;i<MAX_SORT_ENTRIES_PER_DIRECTORY;i++)
			if(g_TempSFNSpace[i].pEntry == NULL)
			{
				//we have found space to store the SFN
				g_TempSFNSpace[i].pEntry = pEntry;
				packed_strcpy(g_TempSFNSpace[i].name,pName);
				g_iNumStoredSFN++;
				break;
			}
	}
}
/////////////////////////////////////////////////////////////////////////
_reentrant INT CopyFindFirst(int mDirEntry, int mfinddata, int *mFileSpec)
{   int find_handle =0;
    DirEntry _X *pDirEntry =NULL;
    Finddata *pfinddata;
    int iDevice, j;
    _packed char* sFileSpec;
    _packed BYTE pszDirName[SFN_LENGTH];


	mDirEntry;
	pfinddata=(Finddata*)   mfinddata;
	sFileSpec=(_packed char*) mFileSpec;

	ClearSFNStorageSpace(0,0,0);	//since we are in a new directory now, so sort from here.
        ((int*)(sFileSpec))[0]=0x2a2e2a; //*.*
        ((int*)(sFileSpec))[1]=0; //null terminator
		_memset(pfinddata,0,sizeof(*pfinddata));
        iDevice = pDirEntry->m_iDevice;
		pfinddata->device = iDevice;
        pfinddata->startrecord = 0;//start at the 0th record.

        //get the first that matches our specifications
           find_handle = FindFirst(pfinddata,(_packed char*) sFileSpec);

        return find_handle;
 }
 ///////////////////////////////////////////////////////////////////////////
 _reentrant INT CopyFindNext(INT HandleNumber,int mfinddata, int* ptr)
{
	int rtn ;

    Finddata *pfinddata;

    pfinddata=(Finddata*) mfinddata;

    rtn=FindNext(HandleNumber,pfinddata);

    return rtn;
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

DWORD GetDclkCount_PL2(void)
{
    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
}