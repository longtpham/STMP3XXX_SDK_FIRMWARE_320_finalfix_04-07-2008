#include "exec.h"
#include "types.h"
#include "project.h"
#include "resource.h"
#include "FileSystem.h"
#include "voicemenu.h"
#include "playerlib.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#include "playlist1internal.h"
#else
#ifdef   USE_PLAYLIST2
#include "menumanager.h"
#include  "playlist2.h"
#include  "playlist2internal.h"
#else
#ifdef   USE_PLAYLIST3
#include "menumanager.h"
#include  "playlist3.h"
#include  "playlist3internal.h"
#endif
#endif
#endif

#include "stringlib.h"
#include "encoderproperties.h"

#ifdef USE_PLAYLIST3
#include "recordsettingsmenu.h"
#include "extern.h"

extern int* pHighestNumber;
extern int g_iHighestVoiceNumber;
extern int g_iHighestFMNumber;
extern int g_iHighestLineNumber;
#endif

#ifdef USE_PLAYLIST5
#include "recordsettingsmenu.h"
extern int* pHighestNumber;
#endif
#ifdef USE_PLAYLIST2
#include "recordsettingsmenu.h"
#endif


int g_iCurrentMP3File=0;
#if defined (TEST)
WORD g_wFileInitial ='T';
#else
WORD g_wFileInitial ='M';
#endif

#ifndef MP3_DIGITS
#define MP3_DIGITS 3
#endif

#ifndef DIRECTORY_SEPARATOR
#define DIRECTORY_SEPARATOR 0x2f
#endif

#if defined (TEST)
extern BOOL g_bRecordTest;
#endif

#if (defined USE_PLAYLIST2) || (defined USE_PLAYLIST5)
#define MAX_NUM_OF_VOICE (1000)
extern int* pHighestNumber;
extern int g_iHighestVoiceNumber;
extern int g_iHighestFMNumber;
extern int g_iHighestLineNumber;
#endif

#ifdef   USE_PLAYLIST2
extern INT DriveLetter[];

// if use MP3 directory for storing mp3 then uncomment the next line
//#define USE_MP3_FOLDER	1
#ifndef MAX_FOLDER_SIZE
#define MAX_FOLDER_SIZE 12		//# of max bytes
#endif

#ifndef MP3DIR_0
#define MP3DIR_0		0x33504d  //   3 P M
#define MP3DIR_1		0x000000  //
#endif


//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Voice_IsMP3File(struct FileEntry *pEntry)
//
//   Type:          Function
//
//   Description:   used to simplify the code for building the current play queue
//
//   Inputs:        struct FileEntry *pEntry
//
//   Outputs:       TRUE or FALSE
//
//<
//////////////////////////////////////////////////////
BOOL _reentrant Voice_IsMP3File(_packed BYTE *pFilename)
{
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

	return (wFileExtension == MP3_FILE_EXT ? 1 : 0);
}

#endif

_reentrant WORD GetMP3Number(_packed BYTE *pFilename)
{
    WORD wIterator=0;
    WORD wValue=0;
    WORD wChar;
    BOOL bStarted = FALSE;
	int i;
    //start from last '/' DIR_SEPERATER
    i=0;
    while(wChar= packed_get(pFilename,i++))
    {
        if(wChar == DIRECTORY_SEPARATOR)
            wIterator=i;
    }

    //Check #1 Compare first character of filename with g_wFileInitial, if its same then only find the highest number
    wChar= packed_get(pFilename,wIterator++);
    if(wChar==g_wFileInitial)
    {
        for(i=0;i<MP3_DIGITS;i++)
        {
           //Check #2 check that next three character must be digit.
            wChar= packed_get(pFilename,wIterator++);
            if(wChar >='0' && wChar <='9')
            {
                wValue = (wValue * 10)+wChar-'0';
            }
            else
            {
                wValue = 0;
            }
        }
        if (i == MP3_DIGITS)
        {
            //Check #3 the file format should be Mxxx.mp3 for encoded file
	        //if the '.' could not be found after 3 digit return wValue=0
            wChar= packed_get(pFilename,wIterator++);
            if(wChar!=0x2e)
                wValue=0;
        }
    }

    return wValue;
}

_reentrant WORD GetHighestMP3Number(int iDevice)
{
#ifdef USE_PLAYLIST1
    WORD wHighestNumber=0;
    WORD wCurrentNumber;
    WORD i;
    for(i=0;i<PLAYLIST_MAX_FILES;i++)
    {
        if(g_FileEntryPool[i].m_iDecoder > FILE_ENTRY_DIR && g_FileEntryPool[i].m_iDevice==iDevice)
        {//0 is directory, -1 is unused
            wCurrentNumber = GetMP3Number(g_FileEntryPool[i].m_pszFilename);
            if(wCurrentNumber > wHighestNumber)
                wHighestNumber = wCurrentNumber;
        }
    }
    return wHighestNumber;
#else
#ifdef USE_PLAYLIST2
	WORD wHighestNumber=0;
    WORD wCurrentNumber;
	_packed BYTE EntryName[MAX_FOLDER_SIZE+3];	//size of this array will depend on what is returned by getting name from Fast File Handle + size of "a:\"
	INT j[3];
	DirEntry _X* pMP3Directory = 0;
	FileEntry* pMP3File = 0;
	EntryName[0] = 0;	//make sure the string is empty

	packed_strcpy(EntryName, (_packed BYTE*) &DriveLetter[iDevice]);	//get the root into the string
	j[0] = ROOT_SEPARATOR;
	packed_strcat(EntryName, (_packed BYTE*) j);

    #if defined (TEST)
	if ( g_bRecordTest == TRUE )
	{
		j[0] = REC_TEST_0;    //word 0 (3 chars)
		j[1] = REC_TEST_1;	  //word 1
	}
    packed_strcat(EntryName, (_packed BYTE*) j);
	j[0] = DIRECTORY_SEPARATOR;
	packed_strcat(EntryName, (_packed BYTE*) j);
    #endif

	#ifdef USE_MP3_FOLDER
 	j[0] = MP3DIR_0; 	  		//word 0 (3 chars)
	j[1] = MP3DIR_1;	  		//word 1

	packed_strcat(EntryName, (_packed BYTE*) j);
	j[0] = DIRECTORY_SEPARATOR;
	packed_strcat(EntryName, (_packed BYTE*) j);
	#endif	// end of MP3 directory

    switch(g_ADCsource)
	{
    	case SOURCE_FM:
	   		j[0]=FMDIR_0;    //word 0 (3 chars)
			break;
		case SOURCE_LINEIN:
    		j[0]=LINE1DIR_0;    //word 0 (3 chars)
    		j[1]=LINE1DIR_1;	  //word 1
    		j[2]=LINE1DIR_2;	  //word 2
			break;
		//default to voice directory or SOURCE_MIC
		default:
    		j[0]=VOICEDIR_0; 	  //word 0 (3 chars)
    		j[1]=VOICEDIR_1;	  //word 1
			break;
	}
	packed_strcat(EntryName, (_packed BYTE*) j);
	j[0] = DIRECTORY_SEPARATOR;
	packed_strcat(EntryName, (_packed BYTE*) j);


	pMP3Directory = (DirEntry _X*) SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_LocateDirEntryFromName, 0, 0, (INT *) EntryName);

	if(!pMP3Directory){
		return 0;
	}

	pMP3File = (FileEntry*) (pMP3Directory->m_pFileContents);	//get a pointer to the first file in the folder.

	while(pMP3File){
		SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetSFN, TYPE_FILE, (INT) pMP3File, (INT *) EntryName);
		if(Voice_IsMP3File(EntryName)){
			wCurrentNumber = GetMP3Number(EntryName);
			if(wCurrentNumber > wHighestNumber)
				wHighestNumber = wCurrentNumber;
		}
		pMP3File = (FileEntry*) pMP3File->m_pNext;
	}
	return wHighestNumber;
#else	// #ifdef USE_PLAYLIST2
	if (*pHighestNumber >= 999)
		*pHighestNumber = 0;
	return (*pHighestNumber)++;
#endif	// #ifdef USE_PLAYLIST2
#endif
}

_reentrant RETCODE MP3_CreateFilename(_packed BYTE *pPtr, WORD iNumber)
{
#ifdef USE_PLAYLIST1
    RETCODE rtn = !SUCCESS;
    WORD wTempString[MAX_FOLDER_SIZE/3+1];
    WORD wSeparator = DIRECTORY_SEPARATOR;
    //wFileInitial ='M';
    int i;

    #if defined (TEST)
	if ( g_bRecordTest == TRUE )
	{
	    wTempString[0]=REC_TEST_0;    //word 0 (3 chars)
	    wTempString[1]=REC_TEST_1;	  //word 1
	    //wFileInitial ='T';	//add L to end.  MUST BE CAPS!!!
	    pPtr[0]=0;
	    //add the directory separator
	    packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));
	    packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the folder name
	    packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));//add the directory separator
	    packed_strcat(pPtr,(_packed BYTE*)(&g_wFileInitial));//add the initial letter
	}
	else
	{
	#endif

    //zero out the beginning so that strcat will work right
    pPtr[0]=0;

    packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));//add the directory separator
    wTempString[0]='M';//remember, must be capital letters!
    packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the 'V' of the name

    #if defined (TEST)
    }
    #endif


    for(i=0;i<MP3_DIGITS;i++)
    {
        wTempString[i]=0x30;
    }
    i=0;
    while(iNumber)
    {
        if(i<MP3_DIGITS)
            wTempString[i] = (iNumber%10)+0x30;
        iNumber/=10;
        i++;
    }
    if(i<=MP3_DIGITS)
    {
        for(i=MP3_DIGITS-1;i>=0;i--)
        {
            packed_strcat(pPtr,(_packed BYTE*)(&wTempString[i]));
        }

        wTempString[0]=0x504D2E;//.MP3
        wTempString[1]=0x000033;
        packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the 'MP3' directory
        rtn = SUCCESS;

    }
    return rtn;
#else
    RETCODE rtn = !SUCCESS;
    WORD wTempString[MAX_FOLDER_SIZE/3+1];	 //make sure there is enough room in words
    WORD wSeparator = DIRECTORY_SEPARATOR;
#ifdef USE_PLAYLIST3
	WORD wFileInitial ='V';					//Must be CAPS!!
#endif
    int i;
	// wFileInitial ='M';					//Must be CAPS!!



    //zero out the beginning so that strcat will work right
    pPtr[0]=0;
#ifdef USE_PLAYLIST3
	//add the directory separator
	packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));
#endif

    #if defined (TEST)
	if ( g_bRecordTest == TRUE )
	{
	    wTempString[0]=REC_TEST_0;    //word 0 (3 chars)
	    wTempString[1]=REC_TEST_1;	  //word 1
	    wFileInitial ='T';	//add L to end.  MUST BE CAPS!!!
	}
	   // packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the folder name
	   // packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));//add the directory separator
	else
	{
    #endif //test endif

#if (defined USE_PLAYLIST2) || (defined USE_PLAYLIST5)
	#ifdef USE_MP3_FOLDER
    //Select Folder Name and Initial
	wTempString[0]=MP3DIR_0; 	  //word 0 (3 chars)
	wTempString[1]=MP3DIR_1;	  //word 1
	#else
	wTempString[0]=0;
	#endif

	switch(g_ADCsource)
	{
   		case SOURCE_FM:
    		wTempString[0]=FMDIR_0;    //word 0 (3 chars)
			g_wFileInitial ='F';	//add F to end.  MUST BE CAPS!!!
			break;

		case SOURCE_LINEIN:
    		wTempString[0]=LINE1DIR_0;    //word 0 (3 chars)
    		wTempString[1]=LINE1DIR_1;	  //word 1
    		wTempString[2]=LINE1DIR_2;	  //word 2
			g_wFileInitial ='L';	//add L to end.  MUST BE CAPS!!!
			break;

		//default to voice directory or SOURCE_MIC
		default:
    		wTempString[0]=VOICEDIR_0; 	  //word 0 (3 chars)
    		wTempString[1]=VOICEDIR_1;	  //word 1
			break;
	}

#else	// #if USE_PLAYLIST2
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

#endif	// #if USE_PLAYLIST2
#if defined (TEST)
 }
#endif
	packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the folder name
	packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));//add the directory separator
#ifdef USE_PLAYLIST3
	packed_strcat(pPtr,(_packed BYTE*)(&wFileInitial));//add the initial letter
#else
	packed_strcat(pPtr,(_packed BYTE*)(&g_wFileInitial));//add the initial letter
#endif

    for(i=0;i<MP3_DIGITS;i++)
    {
        wTempString[i]=0x30;
    }
    i=0;

    while(iNumber)
    {
        if(i<MP3_DIGITS)
            wTempString[i] = (iNumber%10)+0x30;
        iNumber/=10;
        i++;
    }
    if(i<=MP3_DIGITS)
    {
        for(i=MP3_DIGITS-1;i>=0;i--)
        {
            packed_strcat(pPtr,(_packed BYTE*)(&wTempString[i]));
        }

        wTempString[0]=0x504D2E;//.MP3
        wTempString[1]=0x000033;
        packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the 'MP3' directory
        rtn = SUCCESS;

    }
    return rtn;
#endif
}

_reentrant RETCODE MP3_GetNextFilename(int iDevice, int iFilenameBufferLength, int*pPtr)
{
#if defined(USE_PLAYLIST3) || defined(USE_PLAYLIST5)
    INT HandleNumber, i;
#endif	// #ifdef USE_PLAYLIST3
    RETCODE rtn=!SUCCESS;
    if(pPtr && iFilenameBufferLength > 16)//must have room for /wwwwwwww.wav\0
    {
#if defined(USE_PLAYLIST3) || defined(USE_PLAYLIST5)
        //loop if create a used filename
		i = 0;
		while (i < MAX_NUM_OF_VOICE)
    	{
    	   	if (g_ADCsource == SOURCE_FM)
    		   	pHighestNumber = &g_iHighestFMNumber;
        	else if(g_ADCsource == SOURCE_MIC)					// The MIC and LineIn have its recod file number respectively
	    	   	pHighestNumber = &g_iHighestVoiceNumber;
		    else
	    	   	pHighestNumber = &g_iHighestLineNumber;
#endif	// #ifdef USE_PLAYLIST3

            rtn = MP3_CreateFilename((_packed BYTE*)pPtr,GetHighestMP3Number(iDevice)+1);

#if defined(USE_PLAYLIST3) || defined(USE_PLAYLIST5)
        	if ((rtn == SUCCESS) && ((HandleNumber = Fopen((_packed char*)pPtr, (_packed char *)"r")) >= 0))
			    FSFileClose(HandleNumber);
        	else
        		break;
        	i++;
        }
#endif	// #ifdef USE_PLAYLIST3
    }
    return rtn;
}

