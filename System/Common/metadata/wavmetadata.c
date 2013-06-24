///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: wavmetadata.c
// Description: Functions for extracting the .wav (PCM/ADPCM) meta-data.
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "wavmetadata.h"
#include "resource.h"
#ifdef USE_PLAYLIST3
#include "fsapi.h"
#include "fstypes.h"

extern _reentrant INT FastOpen(LONG Key,_packed char *mode);

extern BOOL g_Rebuild;

extern DWORD g_FileKey;
extern DWORD g_CurrentSongFastkey;

extern DIR_DATE g_dirdate;
extern DIR_TIME g_dirtime;
#endif
#ifdef USE_PLAYLIST5
#include "fsapi.h"
#include "playlist5.h"
#include "playerlib.h"
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetWavMetaData
//
//   Type:          Function
//
//   Description:   Finds the meta-data within a .wav file.
//
//   Inputs:        Ptr to pathname, current device id, ptr to meta-data struct.
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant GetWavMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
    RETCODE rtn = META_DATA_NOT_FOUND;
    INT iHandle = NOERROR;                          // indicates the handle of file opened (or error if failure)
    DWORD dwNumBytesTemp;                           // stores 48 bits, number of bytes in file
    DWORD dwAvgBytesPerSec;                         // average bytes per second
    WORD wBufferPacked[ADPCM_READ_BUFFER_SIZE];     // read file (packed bytes) into this buffer
    BYTE btUnpack[ADPCM_READ_BUFFER_SIZE];          // uppack bytes into this buffer

    _packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;

    // Open the file.
#ifdef USE_PLAYLIST3
    if (g_Rebuild == TRUE)
    	iHandle = FSFastOpen(g_FileKey,READ);
    else
	{
		if(g_CurrentSongFastkey==0)
		{
      		iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);
			g_CurrentSongFastkey=FgetFastHandle(iHandle);
		}
		else
			iHandle = FSFastOpen(g_CurrentSongFastkey,READ);
	    //if((iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice)) != ERROR)
	}
#endif
#ifdef USE_PLAYLIST2
			iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);
#endif
#ifdef USE_PLAYLIST5
	g_CurrentFastKey = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->file.lFastKey;
	iHandle = FSFastOpen(g_CurrentFastKey,READ);
#endif
	if(iHandle!= ERROR)
    {
#ifdef USE_PLAYLIST3
	    if (g_Rebuild == TRUE)
		{
			filegetdate(iHandle,CREATION_DATE,&g_dirdate,&g_dirtime);
		}
#endif
        // Get the number of bytes in song
        dwNumBytesTemp = FSFileSize(iHandle);
        MetaData->dwNumBytes = dwNumBytesTemp;

        // Clear MetaData members that do not apply
        MetaData->dwNumFrames = (DWORD)0;
        MetaData->bVBR = (BOOL)0;
        MetaData->wBitRate = (WORD)0;

        // Read from start of file
        if (FSFileRead(20, iHandle, MEM_SPACE_Y, (INT)(-1), wBufferPacked) != ERROR)  //read RIFF 	header
        {
            unpack_data((_packed BYTE *)wBufferPacked, btUnpack,20);

            if ((btUnpack[0]=='R') && (btUnpack[1]=='I') && (btUnpack[2]=='F') && (btUnpack[3]=='F') &&
	           (btUnpack[8]=='W') && (btUnpack[9]=='A') && (btUnpack[10]=='V') && (btUnpack[11]=='E') &&
	           (btUnpack[12]=='f') && (btUnpack[13]=='m') && (btUnpack[14]=='t') && (btUnpack[15]==' '))
            {
                // success, header is found
                dwNumBytesTemp = ((DWORD)btUnpack[16] | ((DWORD)btUnpack[17]<<8) |
	            ((DWORD)btUnpack[18]<<16) | ((DWORD)btUnpack[19]<<24)) - 12;

                MetaData->iResourceNum = -1;           // Set Resource Number to -1 if WAV file type NOT supported.

                if (FSFileRead(12, iHandle, MEM_SPACE_Y, (INT)(-1), wBufferPacked) != ERROR)   // read 'fmt ' chunk
                {
                    unpack_data((_packed BYTE *)wBufferPacked, btUnpack,12);

                    // Identify the file type with the wformattag (2 bytes)
	                switch(((WORD)btUnpack[0])|((WORD)btUnpack[1]<<8))
	                {
	                        case WAVE_FORMAT_ADPCM:
	                            MetaData->iFileType = (INT)MS_ADPCM_TYPE;
	                            MetaData->iResourceNum = RSRC_DEC_ADPCM_MOD_CODE;
	                            break;
	                        case WAVE_FORMAT_IMA_ADPCM:
	                            MetaData->iFileType = (INT)IMA_ADPCM_TYPE;
	                            MetaData->iResourceNum = RSRC_DEC_ADPCM_MOD_CODE;
	                            break;
	                        case WAVE_FORMAT_PCM:
	                            MetaData->iFileType = (INT)PCM_WAV_TYPE;
	                            MetaData->iResourceNum = RSRC_DEC_ADPCM_MOD_CODE;
	                            break;
	                        default:
	                            rtn = META_DATA_FILE_NOT_SUPPORTED;
	                }

                    if (rtn != META_DATA_FILE_NOT_SUPPORTED)
                    {
	                        // nchannels next 2 bytes
	                        MetaData->wNumChannels = ((WORD)btUnpack[2])|((WORD)btUnpack[3]<<8);

	                        // Get nsamplespersec
	                        MetaData->wSampRate =((WORD)btUnpack[4])|((WORD)btUnpack[5]<<8)|((WORD)btUnpack[6]<<16);

	                        // next byte is 00

	                        // next 4 bytes are navgbytespersec
	                        dwAvgBytesPerSec = (DWORD)btUnpack[8] | ((DWORD)btUnpack[9]<<8) |
                        	((DWORD)btUnpack[10]<<16) | ((DWORD)btUnpack[11]<<24);

                             // Calculate the song time
                             while (fseek(iHandle,dwNumBytesTemp,SEEK_CUR) != ERROR)       // search 'data' chunk
                             {
                                if (FSFileRead(8, iHandle, MEM_SPACE_Y, (INT)(-1), wBufferPacked) != ERROR)
	                            {
	                                unpack_data((_packed BYTE *)wBufferPacked, btUnpack,8);

	                                dwNumBytesTemp = (DWORD)btUnpack[4] | ((DWORD)btUnpack[5]<<8) |
                                	((DWORD)btUnpack[6]<<16) | ((DWORD)btUnpack[7]<<24);
	                                if ((btUnpack[0]=='d') && (btUnpack[1]=='a') && (btUnpack[2]=='t') && (btUnpack[3]=='a'))
	                                {
	                                // 'data' chunk is found
                                       MetaData->iTime = (DWORD) dwNumBytesTemp / (DWORD)dwAvgBytesPerSec;
                                     // get wave length in seconds
	                                    rtn = META_DATA_FOUND;
	                                    break;
	                                }
	                            }
	                            else
	                                break;
	                        }
                     }
                }
            }
       }

          else
            rtn = META_DATA_FILEREAD_ERROR;

#ifdef USE_PLAYLIST3
        if (g_Rebuild == TRUE)
        {
            if ((Fclose(iHandle)) == ERROR)
          	rtn = META_DATA_FILECLOSE_ERROR;
         }
        else
#endif
        {
        if ((FSFileClose(iHandle)) == ERROR)
          rtn = META_DATA_FILECLOSE_ERROR;
        }
    }
    else
      rtn = META_DATA_FILEOPEN_ERROR;

#ifdef USE_PLAYLIST3
        if (g_Rebuild == TRUE)
        {
            if(iHandle)
			{
	            if ((Fclose(iHandle)) == ERROR)
		          	rtn = META_DATA_FILECLOSE_ERROR;
			}
        }
#endif

    return(rtn);
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        GetRiffInfo
//
//  Type:        Function
//
//  Description: Used to extract Riff information from the RIFF header.
//
//  Inputs:  FILE_META_DATA struct pointer, pointer to buffer containing packed header data
//  Outputs: Fills in the following FILE_META_DATA members:
//              iFileType
//              iResourceNum
//              wSampRate
//              iTime
//
//<
/////////////////////////////////////////////////////////////////////////////////
/*INT _reentrant GetRiffInfo(FILE_META_DATA * MetaData,  _packed BYTE *buf)
{
  BYTE btRiffUnpack[36];
  WORD wPcmType = 0;
  WORD wBitsPerSample = 0;

  // Initialize the Resource Number to -1 (Error case)
  // This will be overwritten if proper RIFF header found.
  MetaData->iResourceNum = -1;

  // unpack first 36 bytes
  unpack_data(buf,btRiffUnpack,36);

  if( btRiffUnpack[0] != 'R' ) return META_DATA_NOT_FOUND;  // fail
  if( btRiffUnpack[1] != 'I' ) return META_DATA_NOT_FOUND;  // header not found
  if( btRiffUnpack[2] != 'F' ) return META_DATA_NOT_FOUND;
  if( btRiffUnpack[3] != 'F' ) return META_DATA_NOT_FOUND;

  // if we get this far, we have found "RIFF" in the header
  // (+4,+5,+6,+7)next 4 bytes represent "chunksize" -- not needed here (?)

  // added code start
  MetaData->dwNumBytes = (DWORD)btRiffUnpack[4] | ((DWORD)btRiffUnpack[5] << 8)
          | ((DWORD)btRiffUnpack[6] << 16) | ((DWORD)btRiffUnpack[7] << 24);
  MetaData->dwNumBytes += 8; // Add the length of RIFF header and chunksize DWORD.
  // added code end

  // Check for "WAVE" and "fmt" tags
  if( btRiffUnpack[8]  != 'W' ) return META_DATA_NOT_FOUND;
  if( btRiffUnpack[9]  != 'A' ) return META_DATA_NOT_FOUND;
  if( btRiffUnpack[10] != 'V' ) return META_DATA_NOT_FOUND;
  if( btRiffUnpack[11] != 'E' ) return META_DATA_NOT_FOUND;
  if( btRiffUnpack[12] != 'f' ) return META_DATA_NOT_FOUND;
  if( btRiffUnpack[13] != 'm' ) return META_DATA_NOT_FOUND;
  if( btRiffUnpack[14] != 't' ) return META_DATA_NOT_FOUND;
  if( btRiffUnpack[15] != ' ' ) return META_DATA_NOT_FOUND;

  // wavechunksize next 3 bytes and a zero byte (16,17,18,19)

  // Identify the file type with the wformattag (2 bytes)
  wPcmType = (((WORD)btRiffUnpack[20])|((WORD)btRiffUnpack[21]<<8));

  switch(wPcmType)
  {
  case WAVE_FORMAT_ADPCM:
    MetaData->iFileType = (INT)MS_ADPCM_TYPE;
    MetaData->iResourceNum = RSRC_DEC_ADPCM_MOD_CODE;
    break;
  case WAVE_FORMAT_IMA_ADPCM:
    MetaData->iFileType = (INT)IMA_ADPCM_TYPE;
    MetaData->iResourceNum = RSRC_DEC_ADPCM_MOD_CODE;
    break;
  case WAVE_FORMAT_PCM:
    MetaData->iFileType = (INT)PCM_WAV_TYPE;
    MetaData->iResourceNum = RSRC_DEC_ADPCM_MOD_CODE;
    break;
  default:
    MetaData->iResourceNum = -1;           // Set Resource Number to -1 if WAV file type NOT supported.
    return META_DATA_FILE_NOT_SUPPORTED;
  }


  // nchannels next 2 bytes [22,23]
  MetaData->wNumChannels = (((WORD)btRiffUnpack[22])|((WORD)btRiffUnpack[23]<<8));

  // Get nsamplespersec
  MetaData->wSampRate = (((WORD)btRiffUnpack[24])|((WORD)btRiffUnpack[25]<<8)|((WORD)btRiffUnpack[26]<<16));

  // next byte [27] is 00

  // next bytes [28,29,30] are navgbytespersec and [31] is 00

  // next bytes [32,33] are nblockalign

  // Get wbitspersample -- use to calculate the time
  wBitsPerSample = (((WORD)btRiffUnpack[34])|((WORD)btRiffUnpack[35]<<8));

  // Calculate the song time
  MetaData->iTime = (INT) (MetaData->dwNumBytes * (DWORD)8 / (MetaData->wSampRate * wBitsPerSample * MetaData->wNumChannels));

  if(MetaData->iTime <= 0)
    return META_DATA_FILE_NOT_SUPPORTED;    // don't play the 0 byte file

  return META_DATA_FOUND;
} */
