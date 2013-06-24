///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: smvmetadata.c
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
#ifdef USE_PLAYLIST5
#include "playlist5.h"
#include "playerlib.h"
#endif
#define SMV_FILE_SIGNATURE  ('S'|('M'<<8)|('V'<<16))
#define SMV_FILE_MAJOR_VER  2
#define SMV_FILE_MINOR_VER  0

RETCODE _reentrant CheckSMVInfo(INT ihandle, FILE_META_DATA * MetaData);
INT _reentrant SMV_GetRiffInfo(FILE_META_DATA * MetaData,  _packed BYTE *buf);

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
RETCODE _reentrant GetSMVMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
    RETCODE rtn = META_DATA_NOT_FOUND;
    INT iHandle = NOERROR;                          // indicates the handle of file opened (or error if failure)
    DWORD dwNumBytesTemp;                           // stores 48 bits, number of bytes in file
    WORD ReadBuffer[ADPCM_READ_BUFFER_SIZE];        // read file (packed bytes) into this buffer
    _packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;

    // Open the file.
#ifdef USE_PLAYLIST5
    g_CurrentFastKey = (g_PL5_PL_queue.pos + g_PL5_Playback.currentplay)->file.lFastKey;
    iHandle = FSFastOpen(g_CurrentFastKey,READ);
#else
    iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);
#endif

    if(iHandle != ERROR)
    {
        // Get the number of bytes in song
        dwNumBytesTemp = FSFileSize(iHandle);
        MetaData->dwNumBytes = dwNumBytesTemp;

        // Clear MetaData members that do not apply
        MetaData->dwNumFrames = (DWORD)0;
        MetaData->bVBR = (BOOL)0;
        MetaData->wBitRate = (WORD)0;

        // Read from start of file
        if ((FSFileRead(ADPCM_READ_BUFFER_SIZE, iHandle, MEM_SPACE_Y, (INT)(-1), ReadBuffer)) != ERROR)
          // Extract info from the RIFF header -- Fills in MetaData members: iFileType,iResourceNum,wSampRate,iTime
          rtn = SMV_GetRiffInfo(MetaData, (_packed BYTE*)ReadBuffer);
        else
          rtn = META_DATA_FILEREAD_ERROR;

        if (rtn == META_DATA_FOUND)
            rtn = CheckSMVInfo(iHandle, MetaData);

        if ((FSFileClose(iHandle)) == ERROR)
          rtn = META_DATA_FILECLOSE_ERROR;
    }
    else
      rtn = META_DATA_FILEOPEN_ERROR;
    return(rtn);
}

RETCODE _reentrant CheckSMVInfo(INT ihandle, FILE_META_DATA * MetaData)
{
    RETCODE rtn = META_DATA_NOT_FOUND;
    WORD wSMVBuffer[6];
	void _Y *pBuffer = (WORD *)wSMVBuffer;
    WORD wMajorVer, wMinorVer, wSignature;
    INT iVideoWidth, iVideoHeight;

    //skip wave audio part
    if(fseek( ihandle, (LONG)(MetaData->dwNumBytes), SEEK_SET ) < 0)
    {
        return(rtn);
    }

    FSFileRead( 6*3, ihandle, MEM_SPACE_Y, (INT)-1, (void*)pBuffer );
    wSignature = GetByteFromArrayInY(pBuffer, 0) + (GetByteFromArrayInY(pBuffer, 1) << 8) + (GetByteFromArrayInY(pBuffer, 2) << 16);
    if (wSignature != SMV_FILE_SIGNATURE)
        return (rtn);
    wMajorVer = ((GetByteFromArrayInY(pBuffer, 3) - 0x30) << 16);
    wMajorVer += ((GetByteFromArrayInY(pBuffer, 4) - 0x30) << 8);
    wMajorVer += GetByteFromArrayInY(pBuffer, 5) - 0x30;
    wMinorVer = ((GetByteFromArrayInY(pBuffer, 6) - 0x30) << 16);
    wMinorVer += ((GetByteFromArrayInY(pBuffer, 7) - 0x30) << 8);
    wMinorVer += GetByteFromArrayInY(pBuffer, 8) - 0x30;
    if (wMajorVer != SMV_FILE_MAJOR_VER)
        return (rtn);
    if (wMinorVer > SMV_FILE_MINOR_VER)
        return (rtn);
    iVideoWidth = GetByteFromArrayInY( pBuffer, 9 );
    iVideoHeight = GetByteFromArrayInY( pBuffer, 12 );
    if( iVideoWidth > LCD_X_SIZE || iVideoHeight > LCD_Y_SIZE )
        return (rtn);
    return META_DATA_FOUND;
}


///////////////////////////////////////////////////////////////////////////////
//> Name:        SMV_GetRiffInfo
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
INT _reentrant SMV_GetRiffInfo(FILE_META_DATA * MetaData,  _packed BYTE *buf)
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
  MetaData->dwNumBytes = (DWORD)btRiffUnpack[4] | ((DWORD)btRiffUnpack[5] << 8)
						| ((DWORD)btRiffUnpack[6] << 16) | ((DWORD)btRiffUnpack[7] << 24);
  MetaData->dwNumBytes += 8; // Add the length of RIFF header and chunksize DWORD.

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
  case WAVE_FORMAT_IMA_ADPCM:
    MetaData->iFileType = (INT)SMV_ADPCM_TYPE;
    MetaData->iResourceNum = RSRC_DEC_SMVADPCM_MOD_CODE;
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

  return META_DATA_FOUND;
}
