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
#include "voicemenu.h"
#include "recordsettingsmenu.h"

// Playlist2 and playlist3 use the same way to get the record file number.
#include "menus.h"
#include "extern.h"


#if (defined USE_PLAYLIST2) || (defined USE_PLAYLIST5)
#define MAX_NUM_OF_VOICE (1000)
#endif

extern int* pHighestNumber;
extern int g_iHighestVoiceNumber;
extern int g_iHighestFMNumber;
extern int g_iHighestLineNumber;

int g_iCurrentVoiceFile=0;

extern INT DriveLetter[];

#ifndef DIGITS_IN_VOICE_FILENAME
#define DIGITS_IN_VOICE_FILENAME   3
#endif

#if defined (TEST)
extern BOOL g_bRecordTest;
#endif

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Record_IsRecordFile(struct FileEntry *pEntry)
//
//   Type:          Function
//
//   Description:   used to simplify the code for building the current play queue
//
//   Inputs:        struct FileEntry *pEntry
//
//   Outputs:       TRUE or FALSE
//
//   Notes:         This function encapsulates the logic for finding recorded files so that
//                  the developer could easily change the requirements for what is a record file or not.
//
//                  In this implementation:
//                      a record file is a wav file that resides in the /voice/, /fm/, or /line-in/ directory on either device
//                      a non-voice file is any other playable file
//                  Other possible implementations could be:
//                      a voice file is any wav file
//                      a music file is all others
//                  This is (or was) cut and pasted from Playlist_IsVoiceFile();
//<
//////////////////////////////////////////////////////
#ifdef USE_PLAYLIST1
BOOL _reentrant Record_IsRecordFile(struct FileEntry *pEntry)
{
    BOOL rtn = FALSE;
	WORD wSlash = DIRECTORY_SEPARATOR;

    if(pEntry->m_iDecoder == RSRC_DEC_ADPCM_MOD_CODE)
    {//make sure its a wave
        if(pEntry->m_pContainer)
        {//make sure it has a container (This will cull the '/' directory)
            if(pEntry->m_pContainer->m_pContainer)
            {//make sure the container has a container--this would be ensure any file is at least one down
                if(!pEntry->m_pContainer->m_pContainer->m_pContainer)
                {//this ensures that this file is no more than one directory level deep
                	// Build folder name with only the trailing slash.
                	//initialize arrays and pointer for building path name
                    _packed BYTE szRecordFile[MAX_FOLDER_SIZE];
					_packed BYTE FolderName[MAX_FOLDER_SIZE];
					_packed BYTE* pRecFile;

					pRecFile = szRecordFile;	//set pointer equal to the 0 element
					pRecFile[0] = 0;  			//Start with 0, needed to build string
					#if defined (TEST)
					if ( g_bRecordTest == TRUE )
					{
						((WORD*)FolderName)[0]=REC_TEST_0;    //word 0 (3 chars)
						((WORD*)FolderName)[1]=REC_TEST_1;	  //word 1
					}
					else
					{
					#endif
						switch(g_ADCsource)
						{
				    		case SOURCE_FM:
	    						((WORD*)FolderName)[0]=FMDIR_0;    //word 0 (3 chars)
							break;

							case SOURCE_LINEIN:
	    						((WORD*)FolderName)[0]=LINE1DIR_0;    //word 0 (3 chars)
					    		((WORD*)FolderName)[1]=LINE1DIR_1;	  //word 1
	    						((WORD*)FolderName)[2]=LINE1DIR_2;	  //word 2
							break;

			    			//default to voice directory or SOURCE_MIC
							default:
					    		((WORD*)FolderName)[0]=VOICEDIR_0; 	  //word 0 (3 chars)
					    		((WORD*)FolderName)[1]=VOICEDIR_1;	  //word 1
							break;
						}
					#if defined (TEST)
					}
					#endif
					packed_strcat(pRecFile,(_packed BYTE*)(FolderName));//add the directory name
					packed_strcat(pRecFile,(_packed BYTE*)(&wSlash)); //add end slash

                    if(!packed_strcmp(pEntry->m_pContainer->m_pszFilename,szRecordFile))
                    {//make sure its parent is the correct record directory
                        rtn = TRUE;
                    }
                }
            }
        }
    }

    return rtn;
}
#else
#if (defined USE_PLAYLIST2) || (defined USE_PLAYLIST3)
BOOL _reentrant Record_IsRecordFile(_packed BYTE *pFilename){
	//we do not need to check the path since we only check files in the record paths via GetHighestRecordNumber
	WORD wFileExtension;
	WORD wStrLen;
	int i;

	wStrLen= packed_strlen(pFilename);
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
#endif
#endif

_reentrant WORD GetRecordNumber(_packed BYTE *pFilename)
{
    WORD wVoiceNumber;
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



#ifdef USE_PLAYLIST1
_reentrant WORD GetHighestRecordNumber(int iDevice)
{

    WORD wHighestNumber=0;
    WORD wCurrentNumber;
    WORD i;
    for(i=0;i<PLAYLIST_MAX_FILES;i++)
    {
        if(g_FileEntryPool[i].m_iDecoder > FILE_ENTRY_DIR && g_FileEntryPool[i].m_iDevice==iDevice)
        {//0 is directory, -1 is unused
            if(Record_IsRecordFile(&g_FileEntryPool[i]))
            {
                wCurrentNumber = GetRecordNumber(g_FileEntryPool[i].m_pszFilename);
                if(wCurrentNumber > wHighestNumber)
                    wHighestNumber = wCurrentNumber;
            }
        }
    }
    return wHighestNumber;
}
#else
#if (defined USE_PLAYLIST2) || (defined USE_PLAYLIST3) || (defined USE_PLAYLIST5)
_reentrant WORD GetHighestRecordNumber(int iDevice)
{
#if 0	// USE_PLAYLIST2
	WORD wHighestNumber=0;
    WORD wCurrentNumber;
	_packed BYTE EntryName[MAX_FOLDER_SIZE+3];	//size of this array will depend on what is returned by getting name from Fast File Handle + size of "a:\"
	INT j[3];
	DirEntry _X* pRecordDirectory = 0;
	FileEntry* pRecordFile = 0;
	EntryName[0] = 0;	//make sure the string is empty

	packed_strcpy(EntryName, (_packed BYTE*) &DriveLetter[iDevice]);	//get the root into the string
	j[0] = ROOT_SEPARATOR;
	packed_strcat(EntryName, (_packed BYTE*) j);

	#if defined (TEST)	//to keep the test functionality alive
	if ( g_bRecordTest == TRUE )
	{
		j[0] = REC_TEST_0;    //word 0 (3 chars)
		j[1] = REC_TEST_1;	  //word 1
	}
	else
	{
	#endif
		switch(g_ADCsource)
		{
   			case SOURCE_FM:
				j[0] = FMDIR_0;			//word 0 (3 chars)
				break;

			case SOURCE_LINEIN:
				j[0] = LINE1DIR_0;    	//word 0 (3 chars)
   		 		j[1] = LINE1DIR_1;	  	//word 1
				j[2] = LINE1DIR_2;	  	//word 2
				break;

   			//default to voice directory or SOURCE_MIC
			default:
  			j[0] = VOICEDIR_0; 	  		//word 0 (3 chars)
	   		j[1] = VOICEDIR_1;	  		//word 1
			break;
		}
	#if defined (TEST)
	}
	#endif

	packed_strcat(EntryName, (_packed BYTE*) j);
	j[0] = DIRECTORY_SEPARATOR;
	packed_strcat(EntryName, (_packed BYTE*) j);

	pRecordDirectory = (DirEntry _X*) SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_LocateDirEntryFromName, 0, 0, (INT *) EntryName);
	if(!pRecordDirectory){
		return 0;
	}
	pRecordFile = (FileEntry*) pRecordDirectory->m_pFileContents;	//get a pointer to the first file in the folder.

	while(pRecordFile){
		SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetSFN, TYPE_FILE, (INT) pRecordFile, (INT *) EntryName);
		if(Record_IsRecordFile(EntryName)){
			wCurrentNumber = GetRecordNumber(EntryName);
			if(wCurrentNumber > wHighestNumber)
				wHighestNumber = wCurrentNumber;
		}
		pRecordFile = (FileEntry*) pRecordFile->m_pNext;
	}
	return wHighestNumber;
#else // Playlist2 and playlist3 use the same way to get the record file number
	if (*pHighestNumber >= 999)
		*pHighestNumber = 0;
	return (*pHighestNumber)++;
#endif
}
#endif
#endif

_reentrant RETCODE Record_CreateFilename(_packed BYTE *pPtr, WORD iNumber)
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
	#if defined (TEST)
	if ( g_bRecordTest == TRUE )
	{
		wTempString[0]=REC_TEST_0;    //word 0 (3 chars)
		wTempString[1]=REC_TEST_1;	  //word 1
		wFileInitial ='T';	//add L to end.  MUST BE CAPS!!!
	}
	else
	{
	#endif
		switch(g_ADCsource)
		{
    		case SOURCE_FM:
	    		wTempString[0]=FMDIR_0;    //word 0 (3 chars)
				wFileInitial ='F';	//add F to end.  MUST BE CAPS!!!
			break;

			case SOURCE_LINEIN:
	    		wTempString[0]=LINE1DIR_0;    //word 0 (3 chars)
	    		wTempString[1]=LINE1DIR_1;	  //word 1
	    		wTempString[2]=LINE1DIR_2;	  //word 2
				wFileInitial ='L';	//add L to end.  MUST BE CAPS!!!
			break;

			//default to voice directory or SOURCE_MIC
			default:
	    		wTempString[0]=VOICEDIR_0; 	  //word 0 (3 chars)
	    		wTempString[1]=VOICEDIR_1;	  //word 1
				wFileInitial ='V';	//add V to end.  MUST BE CAPS!!!
			break;
		}
	#if defined (TEST)
	}
	#endif
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


_reentrant RETCODE Record_GetNextFilename(int iDevice, int iFilenameBufferLength, int*pPtr)
{
    // Playlist2 and playlist3 use the same way to get the record file number.
    INT	HandleNumber, i;

    RETCODE rtn=!SUCCESS;

    if(pPtr && iFilenameBufferLength > 16)//must have room for /xxxxx/wwwwwwww.wav\0
    {
    	   //loop if create a used filename
		i = 0;

		while (i < MAX_NUM_OF_VOICE)
    	{
    	   	if (g_ADCsource == SOURCE_FM)
                 pHighestNumber = &g_iHighestFMNumber;
            else if(g_ADCsource == SOURCE_MIC)				// The MIC and LineIn have its recod file number respectively.
                 pHighestNumber = &g_iHighestVoiceNumber;
            else
                 pHighestNumber = &g_iHighestLineNumber;
            rtn = Record_CreateFilename((_packed BYTE*)pPtr,GetHighestRecordNumber(iDevice)+1);
        	if ((rtn == SUCCESS) && ((HandleNumber = Fopen((_packed char*)pPtr, (_packed char *)"r")) >= 0))
			    FSFileClose(HandleNumber);
        	else
        		break;
        	i++;
        }
    }
    return rtn;
}

