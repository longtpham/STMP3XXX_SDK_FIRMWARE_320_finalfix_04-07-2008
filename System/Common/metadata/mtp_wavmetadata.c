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
    WORD ReadBuffer[ADPCM_READ_BUFFER_SIZE];        // read file (packed bytes) into this buffer
    _packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;

    // Open the file.
    iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);

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
          rtn = GetRiffInfo(MetaData, (_packed BYTE*)ReadBuffer);
        else
          rtn = META_DATA_FILEREAD_ERROR;

        if ((FSFileClose(iHandle)) == ERROR)
          rtn = META_DATA_FILECLOSE_ERROR;
    }
    else
      rtn = META_DATA_FILEOPEN_ERROR;
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
INT _reentrant GetRiffInfo(FILE_META_DATA * MetaData,  _packed BYTE *buf)
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
    MetaData->iResourceNum = -1;//RSRC_DEC_ADPCM_MOD_CODE;
    break;
  case WAVE_FORMAT_IMA_ADPCM:
    MetaData->iFileType = (INT)IMA_ADPCM_TYPE;
    MetaData->iResourceNum = -1;//RSRC_DEC_ADPCM_MOD_CODE;
    break;
  case WAVE_FORMAT_PCM:
    MetaData->iFileType = (INT)PCM_WAV_TYPE;
    MetaData->iResourceNum = -1;//RSRC_DEC_ADPCM_MOD_CODE;
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
