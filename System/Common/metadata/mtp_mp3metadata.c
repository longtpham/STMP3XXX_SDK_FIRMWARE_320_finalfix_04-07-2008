///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2003
//
// Filename: mp3metadata.c
// Description: Functions for extracting the mp3 meta-data.
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "mp3metadata.h"
#include "resource.h"
#include "dbcs.h"
#include "id3v2.h"
#include "kernel.h"
#include "project.h"			

// For metadata display. Manually changing the global 
// variable before compiling will make it effective.
//	Use:	NO_TAG_SELECTED	----display ID3V2, or ID3V1 if ID3V2 is not available.
//			ID3V1_TAG_SELECTED---display ID3V1 only
//			ID3V2_TAG_SELECTED---display ID3V2 only


//extern WORD g_wVBRSignature;   // Xing or VBR ("X"/"V"/"U"(unknown) or 0 for none)

INT g_iTagSelected = NO_TAG_SELECTED;

////////////////////////////////////////////////////////////////////////////////
//    local function prototypes
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MP3MetaDataReadVBRHeaders(INT iHandle,FILE_META_DATA*pMetaData);

RETCODE _reentrant CheckID3Headers(INT iHandle, FILE_META_DATA * MetaData);

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant GetMp3MetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
//
//   Type:          Function
//
//   Description:   Finds the meta-data within an mp3 file.
//
//   Inputs:        INT PackedPathNameAddress:          "Pointer" to the fully qualified path name (_packed BYTE*)
//                  INT btCurrentDevice:                which device its on.
//                  FILE_META_DATA * MetaData:          structure to fill out
//
//   Outputs:       SUCCESS or !SUCCESS
//
//   Notes:         the pathname pointer must be passed in as an integer because of the calling conventions of
//                  SysCallFunction.  It holds the numeric value of the the _packed BYTE pointer.
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant GetMp3MetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
    RETCODE rtn = META_DATA_NOT_FOUND;
    INT iHandle;                            // indicates the handle of file opened (or error if failure)
    DWORD dwNumBytesTemp;                           // stores 48 bits, number of bytes in file
    _packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;

    // Set some defaults
    MetaData->dwStartPos = 0;
    MetaData->dwNumFrames = (DWORD)0;
    MetaData->iTime = 0;

    // Open the file.
    iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);
    if (iHandle != ERROR)
    {
        // we know this is an MP3 file, so fill in file type and resource
        MetaData->iFileType = (INT)MP3_TYPE;
        MetaData->iResourceNum = -1;//RSRC_DECMOD_CODE;
    
        // Get the number of bytes in song (updated as all the headers are found)
        MetaData->dwNumBytes = FSFileSize(iHandle);
        MetaData->dwStartPos = 0;
    
        CheckID3Headers(iHandle, MetaData);

        // The XING header should be just right after ID3V2 tag
        //this will leave the file position at where the function entered if no XING header, or just after the XING header
        SysCallFunction(RSRC_VBR_CODEBANK,MP3MetaDataReadVBRHeaders,iHandle,0,(void*)MetaData);
    
        // Do a fast scan, looking for "FF" in the file.
        // ScanForFF returns at the first FF byte, or with an Error Code
        rtn = ScanForFF(iHandle, (WORD)MP3_SYNC_THRESHOLD, MetaData);

        if (rtn == SUCCESS)
        {
           // search for frame header to get sample rate and bitrate 
          rtn = GetMp3FrameInfo(MetaData, iHandle);

          if (rtn != META_DATA_FOUND)
          {
              FSFileClose(iHandle);
              return rtn;
          }
            
          // calculate time for files with no Xing header
          if ((MetaData->dwNumFrames == 0) || (MetaData->iTime == 0))
            MetaData->iTime = (INT) (MetaData->dwNumBytes * (DWORD)8 / (MetaData->wBitRate * (WORD) 1000));
        
          //Mp3DecoderStartFrameFilePosition = MetaData->dwStartPos;

        } // if (rtn == SUCCESS)
        FSFileClose(iHandle);
    }
    else
      rtn = META_DATA_FILEOPEN_ERROR;
    return rtn;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant CheckID3Headers(INT iHandle, FILE_META_DATA * MetaData)
//
//   Type:          Function
//
//   Description:   Finds the ID3 meta-data within an mp3 file.
//
//   Inputs:        INT iHandle:                    File handle that we're checking
//                  FILE_META_DATA * MetaData:          structure to fill out
//
//   Outputs:       SUCCESS or !SUCCESS
//
//   Notes:         Since the arrays are allocated on the stack, this localizes the effect.
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant CheckID3Headers(INT iHandle, FILE_META_DATA * MetaData)
{
  	UCS3 wSongTitle[TITLE_SIZE];
  	UCS3 wSongArtist[ARTIST_SIZE];
  	UCS3 wSongAlbum[ALBUM_SIZE];

    //This leaves the file position the same.
    Mp3MetaDataReadID3V1(iHandle,MetaData);
    // Save ID3V1 data in MetaDataTemp
    if ( g_iTagSelected != ID3V1_TAG_SELECTED )
    {
      if ( g_iTagSelected == NO_TAG_SELECTED)
      {
        strcpy(wSongTitle,  MetaData->wTitle);
        strcpy(wSongArtist, MetaData->wArtist);
        strcpy(wSongAlbum,  MetaData->wAlbum);
      }
      if ( g_iTagSelected == ID3V2_TAG_SELECTED) // Do not use ID3V1 data
      {
        *MetaData->wTitle = 0;
        *MetaData->wArtist = 0;
        *MetaData->wAlbum = 0;
      }
      //Attempt to read the ID3V2 tag 
      //this leaves the file position at the first byte after the Id3V2 tag, or where it entered the function
      SysCallFunction(RSRC_ID3V2_CODEBANK,Mp3MetaDataReadID3V2,iHandle,0,(void*)MetaData);
  
      if ( g_iTagSelected == NO_TAG_SELECTED) //Use ID3V2, or ID3V1 if ID3V2 is not avaiable.
      {
        if ( packed_strlen((_packed BYTE *)(MetaData->wTitle)) == 0 )
          strcpy(MetaData->wTitle, wSongTitle);
        if ( packed_strlen((_packed BYTE *)(MetaData->wArtist)) == 0 )
          strcpy(MetaData->wArtist, wSongArtist);
        if ( packed_strlen((_packed BYTE *)(MetaData->wAlbum)) == 0 )
          strcpy(MetaData->wAlbum, wSongAlbum);
      }
    }
    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//> Name:        RETCODE  _reentrant Mp3MetaDataReadID3V1(INT iHandle,FILE_META_DATA*pMetaData)
//
//  Type:        Function
//
//  Description: Used to check for ID3v1 tag at end of file (last 128 bytes)
//
//  Inputs:      INT iHandle:               handle to the open file
//               FILE_META_DATA*pMetaData:  pointer to the meta data structure to fill in
//  Outputs:     SUCCESS or !SUCCESS.
//
//  Notes:       If TAG is found, fills in the following in the meta-data struct:
//                 Title, Artist, Album, Genre.
//               Always returns the file position to the same as it was prior to entering this function.
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant Mp3MetaDataReadID3V1(INT iHandle,FILE_META_DATA*pMetaData)
{
    WORD uLength;
    WORD uTemp=0;
    _packed BYTE TempString[31];

    LONG uPosition = ftell(iHandle);

    if(fseek(iHandle,-128,SEEK_END)!= ERROR)
        FSFileRead(3, iHandle, MEM_SPACE_Y, (INT)(-1), &uTemp);

    if(uTemp== 0x474154) 
    {

        // if we get this far, we have found an ID3v1 tag
        //subtract off 128 bytes from the length of the song.
        pMetaData->dwNumBytes-= 128;

        //Read in the song title.
        FSFileRead(30, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)TempString);
        packed_set(TempString,30,0);//null terminate.
        if(pMetaData->wTitle)
        {
            uLength = 31;
            if(pMetaData->iTitleBufferLength < uLength)
              uLength = pMetaData->iTitleBufferLength;
            DBCSToUnicode(TempString,pMetaData->wTitle,uLength);
        }

        //Read in the song artist.
        FSFileRead(30, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)TempString);
        packed_set(TempString,30,0);//null terminate.
        if(pMetaData->wArtist)
        {
            uLength = 31;
            if(pMetaData->iArtistBufferLength < uLength)
              uLength = pMetaData->iArtistBufferLength;
            DBCSToUnicode(TempString,pMetaData->wArtist,uLength);
        }

        //Read in the song title.
        FSFileRead(30, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)TempString);
        packed_set(TempString,30,0);//null terminate.
        if(pMetaData->wAlbum)
        {
            uLength = 31;
            if(pMetaData->iAlbumBufferLength < uLength)
              uLength = pMetaData->iAlbumBufferLength;
            DBCSToUnicode(TempString,pMetaData->wAlbum,uLength);
        }

        //seek past the comment and year
        fseek(iHandle,34,SEEK_CUR);

        //Read in the genre.
        uTemp = 0;
        FSFileRead(1, iHandle, MEM_SPACE_Y, (INT)(-1), &uTemp);
        pMetaData->btGenre = uTemp;
    
    }
    //return to where we were when we entered the function.
    fseek(iHandle,uPosition,SEEK_SET);
    return (SUCCESS);
}






///////////////////////////////////////////////////////////////////////////////
//> Name:        GetMp3FrameInfo
//
//  Type:        Function
//
//  Description: Used to find the Mp3 frame (searches for sync bytes -- 11 bits set to 1)
//               Also looks through consecutive frames for changing bitrates and sets VBR to be true 
//               bitrates are indeed changing.
//
//  Inputs:      File handle, MetaData struct
//  Outputs:     0 if Success, Error Code if Failure
//
//  Notes:       If frame is found, fills in the following in the meta-data struct:
//                 wSampRate, wBitRate, iTime (if no Xing header), bVBR
//               Currently: A frame is considered to have been found if the following criteria
//               are met:
//                   1. The 11 sync bits are found (11 ones in a row)  -- (ExtractMp3Frame does this)
//                             ****AND****
//                   2. The bitrate is NOT equal to 4 ones in a row (not allowed)  -- (ExtractMp3Frame does this)
//                             ****AND****
//                   3. Two frames in a row are found and critical values match between them
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant GetMp3FrameInfo(FILE_META_DATA * MetaData, INT iHandle)
{
    WORD wFrameBufferPacked[FRAME_HEADER_PACKED];
    BYTE btFrameUnpack[FRAME_HEADER_SIZE];
    INT  iExtract=NOERROR,i;
    BOOL bFrameFound=FALSE, bVBR=0, bFirstFrameFound=FALSE;
    WORD  wBitRate=0, wSampRate=0;
    WORD  wBitRateTemp=0, wSampRateTemp=0;
    WORD  wFrameLengthInBytes;
    BYTE  btBitRateTemp,btPaddingBitTemp;
    DWORD dwStartPosOffset=0;
    WORD  wByteCountWithoutSync=0;
    volatile BYTE  btMPEGVer,btBitRate,btSampRate,btPaddingBit,btChannels;
    volatile INT   iLayer=0;

    // Search through file until sync is found
    while (!bFrameFound)
    {
      while (!bFirstFrameFound)
      {
        // Reset to beginning of file or start position offset by the current offset
        if ((fseek(iHandle,MetaData->dwStartPos+dwStartPosOffset,SEEK_SET)) == ERROR)    // FileSeek to start position
          return(META_DATA_FILESEEK_ERROR);

        // Extract frame header from file
        iExtract = ExtractMp3Frame(iHandle,wFrameBufferPacked,btFrameUnpack);

        if(iExtract==ERROR)            // ERROR reading file
          return(META_DATA_FILEREAD_ERROR);

        if(iExtract==SUCCESS)          // SYNC bits found (11 ones)
        {
          bFirstFrameFound = TRUE;
          // Update start position
          MetaData->dwStartPos = MetaData->dwStartPos + dwStartPosOffset;
        }
        else
        {
          // else (iExtract == seek increment value)
          // Update Counters
          dwStartPosOffset+= (DWORD)iExtract;
          wByteCountWithoutSync+= (WORD)iExtract;

          // Return error if Sync not found within MP3_SYNC_THRESHOLD (project.inc) bytes
          // This is to prevent searching a large file that is perhaps bad.
          if(wByteCountWithoutSync > MP3_SYNC_THRESHOLD)
            return(META_DATA_NOT_FOUND);
        }
      }

      // At this point, we have found what looks like the first frame
      // We are pointing to the first byte of the frame header. (Sync byte).
      dwStartPosOffset = 0;                   // reset the offset

      // We have found the first frame header, extract the data from it
      btMPEGVer=(((btFrameUnpack[1])>>3)&(0x03));
      iLayer=(((btFrameUnpack[1])>>1)&(0x03));
      btBitRate=((btFrameUnpack[2])>>4);
      btSampRate=(((btFrameUnpack[2])>>2)&(0x03));
      btPaddingBit=((btFrameUnpack[2]>>1)&(0x01));
	  btChannels=(btFrameUnpack[3]>>6);
	  MetaData->wNumChannels=btChannels==3?1:2; //Added for Stmp00014780.
	  
      if((btMPEGVer!=1)&(btSampRate != 3)& (btBitRate != 0)&(iLayer!=0))  
       {
      // get the sampling rate and bitrate
      GetMp3Rates(btMPEGVer,btSampRate,btBitRate,iLayer,&wSampRate,&wBitRate);
      // calculate frame length
      wFrameLengthInBytes = GetFrameLength(btMPEGVer,wBitRate,wSampRate,btPaddingBit,iLayer);

      // Move forward to next frame and read it.
      if ((fseek(iHandle,(MetaData->dwStartPos+(DWORD)wFrameLengthInBytes),SEEK_SET))==ERROR)
        return META_DATA_FILESEEK_ERROR;
  
      // Extract frame header from file
      iExtract = ExtractMp3Frame(iHandle,wFrameBufferPacked,btFrameUnpack);

      if (iExtract==SUCCESS)
      {
        // Check that sampling rate and MPEG version and layer match
        if (((((btFrameUnpack[1])>>3)&(0x03))==(btMPEGVer)) &(((btFrameUnpack[2])>>4)!=0)&(((btFrameUnpack[2])>>4)!=15)&
            ((((btFrameUnpack[1])>>1)&(0x03))==iLayer) &
            ((((btFrameUnpack[2])>>2)&(0x03))==btSampRate))
        {
            // Fill in rates
            MetaData->wSampRate = wSampRate;
            MetaData->wBitRate = wBitRate;
            btBitRateTemp=((btFrameUnpack[2])>>4);
            btPaddingBitTemp=((btFrameUnpack[2]>>1)&(0x01));
            // get the sampling rate and bitrate
            GetMp3Rates(btMPEGVer,btSampRate,btBitRateTemp,iLayer,&wSampRateTemp,&wBitRateTemp);
            // calculate frame length
            wFrameLengthInBytes = GetFrameLength(btMPEGVer,wBitRateTemp,wSampRateTemp,btPaddingBitTemp,iLayer);
            // Move forward to next frame and read it.
            if ((fseek(iHandle,(wFrameLengthInBytes - (WORD)MP3_FRAME_HEADER_BYTES_TO_READ),SEEK_CUR)) == ERROR)
              return META_DATA_FILESEEK_ERROR;

/*            if ((wBitRate != wBitRateTemp) || (g_wVBRSignature == 'X') || (g_wVBRSignature == 'V'))
            {
                bVBR = TRUE;
                MetaData->bVBR = VBR_TRUE;
            }
*/          bFrameFound = TRUE;      // second frame matches first, so take it to be right
            break;
        }
      }
       }
      // ELSE -- the frame did not match, so start over
      dwStartPosOffset++;            // increment start position to prevent looping on bad data
      bFirstFrameFound = FALSE;      // begin looking for first frame again
    }    // while (!bFrameFound)

/*    if (!bVBR)
    {
      // check for variable bit rates -- this is not 100% right since need to search entire file
      for (i=0; i<(MP3_FRAMES_TO_CHECK_VBR-2); i++)
      {
        iExtract = ExtractMp3Frame(iHandle,wFrameBufferPacked,btFrameUnpack);
        btBitRateTemp=((btFrameUnpack[2])>>4);
        btPaddingBitTemp=((btFrameUnpack[2]>>1)&(0x01));
        // get the sampling rate and bitrate
        if(GetMp3Rates(btMPEGVer,btSampRate,btBitRateTemp,iLayer,&wSampRateTemp,&wBitRateTemp)==ERROR)
            break;
        // calculate frame length
        wFrameLengthInBytes = GetFrameLength(btMPEGVer,wBitRateTemp,wSampRateTemp,btPaddingBitTemp,iLayer);
        // Move forward to next frame and read it.
        if ((fseek(iHandle,(wFrameLengthInBytes - (WORD)MP3_FRAME_HEADER_BYTES_TO_READ),SEEK_CUR)) == ERROR)
          return META_DATA_FILESEEK_ERROR;

        if (wBitRate != wBitRateTemp)
        {
          bVBR = TRUE;
          MetaData->bVBR = VBR_TRUE;
          if (g_wVBRSignature == 0)
          {
            g_wVBRSignature = 'U';      // This is probably a VBR file, but not enough information.
          }
          break;
        }

      }
    }
*/
/*    if (!bVBR)
      MetaData->bVBR = VBR_NOT_DETERMINED;
*/
    if(iExtract==ERROR)
      return(META_DATA_NOT_FOUND);

    return(META_DATA_FOUND);
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        ExtractMp3Frame
//
//  Type:        Function
//
//  Description: Searches for mp3 frame headers.
//
//  Inputs:      File handle, ptr to packed buffer to read into, ptr to unpacked buffer to extract to
//
//  Outputs:     0 if Success, -1 if Failure, otherwise 1, 2, 3, or 4.
//                    A '1' is returned if byte number 1 (count starts with byte 0) is FF.
//                    A '2' is returned if byte number 2 is FF.
//                    A '3' is returned if byte number 3 is FF.
//                    A '4' is returned if none are FF.
//                    This allows calling routine to FileSeek forward this many bytes and call again.
//
//               Currently: A frame is considered to have been found if the following criteria
//               are met:
//                   1. The 11 sync bits are found (11 ones in a row)  -- (ExtractMp3Frame does this)
//                             ****AND****
//                   2. The bitrate is NOT equal to 4 ones in a row (not allowed)  -- (ExtractMp3Frame does this)
//                             ****AND****
//                   3. Two frames in a row are found and critical values match between them -- (GetMp3FrameInfo does this)
//<
/////////////////////////////////////////////////////////////////////////////////
INT _reentrant ExtractMp3Frame(INT iHandle, WORD * wFrameBufferPacked, BYTE * btFrameUnpack)
{
    INT  i;

    // Read from File
    if (FSFileRead(MP3_FRAME_HEADER_BYTES_TO_READ, iHandle, MEM_SPACE_Y, (INT)(-1), wFrameBufferPacked) == ERROR)
      return(ERROR);

    // unpack bytes read
    unpack_data((_packed BYTE *)wFrameBufferPacked,btFrameUnpack,FRAME_HEADER_SIZE);

    if ((btFrameUnpack[0]==0xFF)&((btFrameUnpack[1]>>5)==0x07))
    // IF sync bits are all set (11 of them)
    {
      // If the bitrate index == 0x0F, then this is not a valid frame
      if (((btFrameUnpack[2]>>4)!=0x0F))
        return SUCCESS;
    }

    for (i=1;i<=3;i++)
      if (btFrameUnpack[i]==0xFF)
        return i;

    // IF none of the 4 bytes are FF, return a '4' to seek forward to 5th byte
    return 4;
}


///////////////////////////////////////////////////////////////////////////////
//> Name:        ScanForFF
//
//  Type:        Function
//
//  Description: Quickly scans through file looking for the first instance of "FF"
//
//  Inputs:      File handle, threshold to stop looking if reached.
//  Outputs:     0 if SUCCESS & points to byte number of first FF.
//               ERROR CODE if not success.
//
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant ScanForFF(INT iHandle, WORD wThresholdMax,  FILE_META_DATA*pMetaData)
{
    RETCODE rtn = META_DATA_NOT_FOUND;
    INT  i;
    WORD wTempBuffer[50];
    WORD wCount=0;
    BOOL bLoopDone=FALSE;

    while(!bLoopDone)
    {
      // Read from File
      if (FSFileRead(150, iHandle, MEM_SPACE_Y, (INT)(-1), wTempBuffer) != ERROR)
      {
          // Scan buffer for "FF" in any of the words.
          for (i=0;i<50;i++)
          {
            if (((wTempBuffer[i]&0xFF)==0xFF)||((wTempBuffer[i]&0xFF00)==0xFF00)||((wTempBuffer[i]&0xFF0000)==0xFF0000))
            {
              rtn = SUCCESS;
              //update the start position of the MP3 data
              pMetaData->dwStartPos+=(DWORD)wCount;
              fseek(iHandle,pMetaData->dwStartPos,SEEK_SET);
              bLoopDone=TRUE;
              break;
            }
            wCount += 3;
            if (wCount > wThresholdMax)
            {
              bLoopDone=TRUE;
              break;
            }
          }
      }
      else
      {
        rtn = META_DATA_FILEREAD_ERROR;
        break;
      }
    } // while(1)
    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        GetFrameLength
//
//  Type:        Function
//
//  Description: Computes the frame length for mp3 file.
//
//  Inputs:      MPEG Version, Bitrate, sampling rate, padding bit (0 or 1), and MPEG layer (I, II, or III)
//  Outputs:     the compressed frame length for current frame, in Bytes
//
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant GetFrameLength(BYTE btMPEGVer, WORD wBitRate, WORD wSampRate, BYTE btPaddingBit, INT iLayer)
{
    WORD  wFrameLengthInBytes=0;

    //For Layer I files us this formula: 
    //FrameLengthInBytes = (12 * BitRate / SampleRate + Padding) * 4 
    if (iLayer == LayerI)
      wFrameLengthInBytes = ((((WORD)12000*wBitRate / wSampRate)+ (WORD)btPaddingBit)*(WORD)4);

    //For Layer II & III files use this formula: 
    //FrameLengthInBytes = (FrameSizeInBits/8) * BitRate / SampleRate + Padding 
    // This works out to:
    //    MPEG Version 1 :         FrameLengthInBytes = 144 * Bitrate / SampleRate + Padding
    //    MPEG Version 2 and 2.5 : FrameLengthInBytes = 72 * Bitrate / SampleRate + Padding
    else
    {
        // Layer II uses the same formula irrespective of MPEG versions.
        if ((btMPEGVer == MPEGv1)||(iLayer == LayerII))
           wFrameLengthInBytes = (((((LONG)wBitRate * (LONG)1000 * (LONG)144)/(LONG)wSampRate))+ (LONG)btPaddingBit);
        else
            wFrameLengthInBytes = (((((LONG)wBitRate * (LONG)1000 * (LONG)72)/(LONG)wSampRate))+ (LONG)btPaddingBit);
    }
    return(wFrameLengthInBytes);
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        GetMp3Rates
//
//  Type:        Function
//
//  Description: Finds the sampling rate and bitrate for the current frame.
//
//  Inputs:      Mpeg version, sampling rate bits, bit rate bits, MPEG layer, ptr to Sampling rate, ptr to Bitrate
//  Outputs:     Success or failure.
//
//<
/////////////////////////////////////////////////////////////////////////////////
INT _reentrant GetMp3Rates(BYTE btMPEGVer,BYTE btSampRate,BYTE btBitRate,INT iLayer, WORD *wSampRate,WORD *wBitRate)
{
    WORD  wBitRateIndex1[16] = { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 999};
    WORD  wBitRateIndex2[16] = { 0, 32, 48, 56, 64,  80,  96,  112, 128, 160, 192, 224, 256, 320, 384, 999};
    WORD  wBitRateIndex3[16] = { 0, 32, 40, 48, 56,  64,  80,  96,  112, 128, 160, 192, 224, 256, 320, 999};
    WORD  wBitRateIndex4[16] = { 0, 32, 48, 56, 64,  80,  96,  112, 128, 144, 160, 176, 192, 224, 256, 999};
    WORD  wBitRateIndex5[16] = { 0, 8,  16, 24, 32,  40,  48,  56,  64,  80,  96,  112, 128, 144, 160, 999};
    WORD  wMPEG1SampRate[4] = {44100, 48000, 32000, 999};
    WORD  wMPEG2SampRate[4] = {22050, 24000, 16000, 999};
    WORD  wMPEG2pt5SampRate[4] = {11025, 12000, 8000, 999};

    // Find BitRate & Sampling Rate
    if (btMPEGVer == MPEGv1)
    {
      *wSampRate = wMPEG1SampRate[btSampRate];
      if (iLayer == LayerI)
        *wBitRate = wBitRateIndex1[btBitRate];
      else if (iLayer == LayerII)
        *wBitRate = wBitRateIndex2[btBitRate];
      else
        *wBitRate = wBitRateIndex3[btBitRate];
    }
    else
    {
      if (btMPEGVer == MPEGv2)
        *wSampRate = wMPEG2SampRate[btSampRate];
      if (btMPEGVer == MPEGv2pt5)
        *wSampRate = wMPEG2pt5SampRate[btSampRate];
      if (iLayer == LayerI)
        *wBitRate = wBitRateIndex4[btBitRate];
      else
        *wBitRate = wBitRateIndex5[btBitRate];
    }

	*wBitRate *= 1000; //Fixed for Stmp00014780
	
    return(SUCCESS);
}


