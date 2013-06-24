#include "exec.h"
#include "types.h"
#include "project.h"
#include "resource.h"
#include "FileSystem.h"
#include "menumanager.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#include "playlist1internal.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#include  "playlist2internal.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include  "playlist3internal.h"
#endif
#endif
#endif

#include "stringlib.h"
#include "encoderproperties.h"


int g_iCurrentVoiceFile=0;

extern INT DriveLetter[];

#ifndef DIGITS_IN_VOICE_FILENAME
#define DIGITS_IN_VOICE_FILENAME   3
#endif

#ifndef DIRECTORY_SEPARATOR
#define DIRECTORY_SEPARATOR 0x2f
#endif

#ifndef MAX_FOLDER_SIZE
#define MAX_FOLDER_SIZE 12		//# of max bytes
#endif

#ifndef VOICEDIR_0
#define VOICEDIR_0		0x494f56  //   i o v 
#define VOICEDIR_1		0x004543  //     e c
#endif

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Voice_IsVoiceFile(struct FileEntry *pEntry)
//
//   Type:          Function
//
//   Description:   used to simplify the code for building the current play queue
//
//   Inputs:        struct FileEntry *pEntry        
//
//   Outputs:       TRUE or FALSE
//
//   Notes:         This function encapsulates the logic for 'voice' vs. 'music' files so that
//                  the developer could easily change the requirements for what is a voice file or not.
//
//                  In this implementation:
//                      a voice file is a wav file that resides in the /voice/ directory on either device
//                      a non-voice file is any other playable file 
//                  Other possible implementations could be:
//                      a voice file is any wav file
//                      a music file is all others
//                  This is (or was) cut and pasted from Playlist_IsVoiceFile();
//<
//////////////////////////////////////////////////////
BOOL _reentrant Voice_IsVoiceFile(_packed BYTE *pFilename)
{
	//we do not need to check the path since we only check files in the record paths via GetHighestRecordNumber
	WORD wFileExtension;
	WORD wStrLen;
	int i;

    wStrLen = packed_strlen(pFilename);	
    if(wStrLen>(MAX_FILENAME_LENGTH-1))
      return 0;

    while(wStrLen--)
    {
        wFileExtension = packed_get(pFilename,wStrLen);
        if(wFileExtension=='.')
        	break;
    }

	//if the '.' could not be found return an error; else fill up wFileExtension
	if(!wStrLen)
		return 0;
	else
	{
		//Pack 1st 3 characters after '.' for the extension
    	for(i=0;i<3;i++)
    	{
        	packed_set((void*)&wFileExtension,i,packed_get(pFilename,++wStrLen));
    	}
	}	
	
	return (wFileExtension == WAV_FILE_EXT ? 1 : 0);
}

_reentrant WORD GetVoiceNumber(_packed BYTE *pFilename)
{
//    WORD wVoiceNumber;
    WORD wIterator=0;
    WORD wValue=0;
    WORD wChar;
    BOOL bStarted = FALSE;

    while((wChar= packed_get(pFilename,wIterator++)))
    {
        if(wChar >='0' && wChar <='9')
        {
            bStarted = TRUE;
            wValue = (wValue * 10)+wChar-'0';
        }
        else
        {
            if(bStarted)
                break;
        }
    }

    return wValue;
}

_reentrant WORD GetHighestVoiceNumber(int iDevice)
{
	WORD wHighestNumber=0;
    WORD wCurrentNumber;
	_packed BYTE EntryName[MAX_FOLDER_SIZE+3];	//size of this array will depend on what is returned by getting name from Fast File Handle + size of "a:\" 	
	INT j[3];
	DirEntry _X* pVoiceDirectory = 0;	
	FileEntry* pVoiceFile = 0;
	EntryName[0] = 0;	//make sure the string is empty

	packed_strcpy(EntryName, (_packed BYTE*) &DriveLetter[iDevice]);	//get the root into the string
	j[0] = ROOT_SEPARATOR;
	packed_strcat(EntryName, (_packed BYTE*) j);

 	j[0] = VOICEDIR_0; 	  		//word 0 (3 chars)
	j[1] = VOICEDIR_1;	  		//word 1
	
	packed_strcat(EntryName, (_packed BYTE*) j);
	j[0] = DIRECTORY_SEPARATOR;
	packed_strcat(EntryName, (_packed BYTE*) j);

	pVoiceDirectory = (DirEntry _X*) SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_LocateDirEntryFromName, 0, 0, (INT *) EntryName);
	if(!pVoiceDirectory){
		return 0;
	}
	pVoiceFile = (FileEntry*) pVoiceDirectory->m_pFileContents;	//get a pointer to the first file in the folder.

	while(pVoiceFile){		
		SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetSFN, TYPE_FILE, (INT) pVoiceFile, (INT *) EntryName);
		if(Voice_IsVoiceFile(EntryName)){
			wCurrentNumber = GetVoiceNumber(EntryName);
			if(wCurrentNumber > wHighestNumber)
				wHighestNumber = wCurrentNumber;	
		}
		pVoiceFile = (FileEntry*) pVoiceFile->m_pNext;	
	}	
	return wHighestNumber;
}

_reentrant RETCODE Voice_CreateFilename(_packed BYTE *pPtr, WORD iNumber)
{
    RETCODE rtn = !SUCCESS;
    WORD wTempString[MAX_FOLDER_SIZE/3+1];	 //make sure there is enough room in words
    WORD wSeparator = DIRECTORY_SEPARATOR;
	WORD wFileInitial ='V';					//Must be CAPS!!
    int i;

    //zero out the beginning so that strcat will work right
    pPtr[0]=0;

	//add the directory separator
	packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));

    //Select Folder Name and Initial
	wTempString[0]=VOICEDIR_0; 	  //word 0 (3 chars)
	wTempString[1]=VOICEDIR_1;	  //word 1
	wFileInitial ='V';	//add V to end.  MUST BE CAPS!!!
	packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the folder name
	packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));//add the directory separator

	packed_strcat(pPtr,(_packed BYTE*)(&wFileInitial));//add the initial letter

    for(i=0;i<DIGITS_IN_VOICE_FILENAME;i++)
    {
        wTempString[i]=0x30;
    }
    i=0;
    while(iNumber)
    {
        if(i<DIGITS_IN_VOICE_FILENAME)
            wTempString[i] = (iNumber%10)+0x30;
        iNumber/=10;
        i++;
    }
    if(i<=DIGITS_IN_VOICE_FILENAME)
    {
        for(i=DIGITS_IN_VOICE_FILENAME-1;i>=0;i--)
        {
            packed_strcat(pPtr,(_packed BYTE*)(&wTempString[i]));
        }
        wTempString[0]=0x41572e;//.wav
        wTempString[1]=0x000056;
        packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add .wav
        rtn = SUCCESS;

    }
    return rtn;
}


_reentrant RETCODE Voice_GetNextFilename(int iDevice, int iFilenameBufferLength, int*pPtr)
{
    RETCODE rtn=!SUCCESS;

    if(pPtr && iFilenameBufferLength > 16)//must have room for /voice/wwwwwwww.wav\0
    {
        rtn = Voice_CreateFilename((_packed BYTE*)pPtr,GetHighestVoiceNumber(iDevice)+1);
    }
    return rtn;
}

