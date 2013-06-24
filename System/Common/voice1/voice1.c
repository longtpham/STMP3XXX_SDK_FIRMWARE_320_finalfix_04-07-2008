#include "exec.h"
#include "types.h"
#include "project.h"
#include "resource.h"
#include "FileSystem.h"
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

#ifndef VOICE_DIGITS
#define VOICE_DIGITS 3
#endif

#ifndef DIRECTORY_SEPARATOR
#define DIRECTORY_SEPARATOR 0x2f
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
#ifdef USE_PLAYLIST1
BOOL _reentrant Voice_IsVoiceFile(struct FileEntry *pEntry)
{
    BOOL rtn = FALSE;

    if(pEntry->m_iDecoder == RSRC_DEC_ADPCM_MOD_CODE)
    {//make sure its a wave
        if(pEntry->m_pContainer)
        {//make sure it has a container (This will cull the '/' directory)
            if(pEntry->m_pContainer->m_pContainer)
            {//make sure the container has a container--this would be ensure any file is at least one down
                if(!pEntry->m_pContainer->m_pContainer->m_pContainer)
                {//this ensures that this file is no more than one directory level deep
                    _packed BYTE szVoice[7];
                    //                      i o v
                    ((int*)szVoice)[0] = 0x494f56;
                    //                      / e c
                    ((int*)szVoice)[1] = 0x2f4543;
                    ((int*)szVoice)[2] = 0;
                    if(!packed_strcmp(pEntry->m_pContainer->m_pszFilename,szVoice))
                    {//make sure its parent is the /VOICE/ directory
                        rtn = TRUE;
                    }
                }
            }
        }
    }
    
    return rtn;
}
#endif
_reentrant WORD GetVoiceNumber(_packed BYTE *pFilename)
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

_reentrant WORD GetHighestVoiceNumber(int iDevice)
{
#ifdef USE_PLAYLIST1
    WORD wHighestNumber=0;
    WORD wCurrentNumber;
    WORD i;
    for(i=0;i<PLAYLIST_MAX_FILES;i++)
    {
        if(g_FileEntryPool[i].m_iDecoder > FILE_ENTRY_DIR && g_FileEntryPool[i].m_iDevice==iDevice)
        {//0 is directory, -1 is unused
            if(Voice_IsVoiceFile(&g_FileEntryPool[i]))
            {
                wCurrentNumber = GetVoiceNumber(g_FileEntryPool[i].m_pszFilename);
                if(wCurrentNumber > wHighestNumber)
                    wHighestNumber = wCurrentNumber;
            }
        }
    }
    return wHighestNumber;
#endif
}

_reentrant RETCODE Voice_CreateFilename(_packed BYTE *pPtr, WORD iNumber)
{
    RETCODE rtn = !SUCCESS;
    WORD wTempString[VOICE_DIGITS];
    WORD wSeparator = DIRECTORY_SEPARATOR;
    int i;

//                    i o v
    wTempString[0]=0x494f56;
//                      e c 
    wTempString[1]=0x004543;
    //zero out the beginning so that strcat will work right
    pPtr[0]=0;
    packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));//add the directory separator
    packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the 'voice' directory
    packed_strcat(pPtr,(_packed BYTE*)(&wSeparator));//add the directory separator
    wTempString[0]='V';//remember, must be capital letters!
    packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the 'V' of the name
    for(i=0;i<VOICE_DIGITS;i++)
    {
        wTempString[i]=0x30;
    }
    i=0;
    while(iNumber)
    {
        if(i<VOICE_DIGITS)
            wTempString[i] = (iNumber%10)+0x30;
        iNumber/=10;
        i++;
    }
    if(i<=VOICE_DIGITS)
    {
        for(i=VOICE_DIGITS-1;i>=0;i--)
        {
            packed_strcat(pPtr,(_packed BYTE*)(&wTempString[i]));
        }
        wTempString[0]=0x41572e;//.wav
        wTempString[1]=0x000056;
        packed_strcat(pPtr,(_packed BYTE*)(wTempString));//add the 'voice' directory
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

