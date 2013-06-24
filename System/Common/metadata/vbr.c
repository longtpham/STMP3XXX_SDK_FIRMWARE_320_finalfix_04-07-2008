///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2003
//
// Filename: vbr.c
// Description: Functions for extracting the mp3 meta-data.
///////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "mp3metadata.h"
#include "menumanager.h"
#ifdef USE_PLAYLIST3
#include "fsapi.h"
#include "fstypes.h"
#include "playlist3.h"
#include "playerlib.h"
#endif

_X WORD	g_wFrameLengthInBytes = 551;	// EOF safety offset bytes, SL

_Y VBR_GLOBAL_HEADER_DATA g_VBRGlobalData;
_Y VBR_HEADER_DATA g_VBRHeaderData;                  // struct for VBR Data (Xing or VBRI)

extern WORD g_wVBRSignature;   // Xing or VBR ("X"/"V"/"U"(unknown) or 0 for none)


DWORD _reentrant ExtractI4(BYTE *buf, INT i);	// stmp 6724  Change type from INT to DWORD to allow 4-byte storage
WORD _reentrant ExtractIVar(BYTE *buf, INT i, BYTE iByteCount);

INT _reentrant GetXingHeader(VBR_HEADER_DATA * VbrHeader, _packed BYTE *buf, WORD * pOffset);
INT _reentrant GetVBRiHeader(VBR_HEADER_DATA * VbrHeader, _packed BYTE *buf, WORD * pOffset);


///////////////////////////////////////////////////////////////////////////////
//> Name:        RETCODE _reentrant FileReadWithSync (INT BytesToRead, INT iHandle, WORD *ReadBuffer)
//
//  Type:        Function
//
//  Description:
//
//  Inputs:      
//  Outputs:
//
//  Notes:
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant FileReadWithSync (INT BytesToRead, INT iHandle, WORD *ReadBuffer)
{
	int i;
	WORD wCount=0;	
	
	while (FSFileRead(150, iHandle, MEM_SPACE_Y, (INT)(-1), ReadBuffer) != ERROR)
	{
		for (i = 0; i < 150/3; i++)
		{
			if (((ReadBuffer[i]&0xFF)==0xFF)||((ReadBuffer[i]&0xFF00)==0xFF00)||((ReadBuffer[i]&0xFF0000)==0xFF0000))
			{
				fseek(iHandle,-150+i*3,SEEK_CUR);
				return FSFileRead(BytesToRead, iHandle, MEM_SPACE_Y, (INT)(-1), ReadBuffer);
			}

            wCount += 3;
            if (wCount > MP3_SYNC_THRESHOLD)
              return ERROR;
		}
	}
	return ERROR;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        RETCODE _reentrant MP3MetaDataReadVBRHeaders(INT iHandle,FILE_META_DATA*pMetaData)
//
//  Type:        Function
//
//  Description: Reads in the XING header, and fills in the related parts of the meta data structure.
//
//  Inputs:      
//  Outputs:     0 if Success, -1 if Failure.
//
//  Notes:       Currently we're checking for VBRi (Fraunhofer) and Xing headers.
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MP3MetaDataReadVBRHeaders(INT iHandle, FILE_META_DATA *pMetaData)
{
    WORD ReadBuffer[READ_BUFFER_SIZE];              // read file (packed bytes) into this buffer
    LONG uPosition = ftell(iHandle);
    WORD wOffset = 0;
    INT i;

    g_VBRGlobalData.VBRTable = &g_wVBRTOCTable[0];     // Initialize VBR Table pointer (Global)
    g_wVBRSignature = 0;                            // Initialize to zero (No VBR).

//    Clear the lookup table.
    for (i=0; i<VBR_TOC_SIZE; i++) 
    {
      g_wVBRTOCTable[i]=0;
    }
    g_VBRGlobalData.VBRTotalSeconds = 0;
    g_VBRGlobalData.VBRDurPerBin = 0;

	// Jan 27 05 (in sdk june 2005)- Find SyncByte before checking VBR header. stmp00006282
	if (FileReadWithSync(VBR_BYTES_TO_READ, iHandle, ReadBuffer) != ERROR)
    {
        // Check if Xing Header exists -- if so, use it
        if ((GetXingHeader(&g_VBRHeaderData, (_packed BYTE*)ReadBuffer, &wOffset) == SUCCESS) || 
            (GetVBRiHeader(&g_VBRHeaderData, (_packed BYTE *)ReadBuffer, &wOffset) == SUCCESS) )
        { // fill in Xing Meta-Data if Xing header found
          // fill in the sampling rate
          pMetaData->wSampRate = g_VBRHeaderData.VBRSampleRate;
          // fill in the number of frames
          pMetaData->dwNumFrames = g_VBRHeaderData.VBRStreamFrames;
          // fill in the number of bytes
          pMetaData->dwDataBytes = g_VBRHeaderData.VBRStreamBytes;
          // Fill in the time
          pMetaData->iTime = g_VBRGlobalData.VBRTotalSeconds;   
        }
        // increment the StartPosition so we're positioned after the Xing or VBR Headers.
        pMetaData->dwStartPos += wOffset;
        fseek(iHandle,(uPosition+wOffset),SEEK_SET);
    }
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        GetXingHeader
//
//  Type:        Function
//
//  Description: Used to check for Xing header at beginning of file and extract
//               meta-data if found. 
//
//  Inputs:      XHEADDATA *X  Pointer to Xing header data structure to be filled in
//               *buf          Pointer to packed buffer containing file read contents (to search)
//               *pOffset      Number of bytes from beginning of file to end of Xing header.
//
//  Outputs:     Returns SUCCESS if Xing header found, ERROR if not.
//
//  Notes: Bytes - description.
// 4   Xing
// 4   flags
// 4   frames
// 4   bytes
// 100 toc
// 
//<
/////////////////////////////////////////////////////////////////////////////////
INT _reentrant GetXingHeader(VBR_HEADER_DATA * VbrHeader, _packed BYTE *buf, WORD * pOffset)
{
    BYTE btVBRUnpack[VBR_BYTES_TO_READ];  // Careful - not much room on stack.
    BYTE i;
    INT iXingPtr=0;
    INT head_flags;
    INT h_id, h_mode, h_sr_index;
    WORD sr_table[4] = { 44100, 48000, 32000, 99999 };
    DWORD  dwPreviousByteCount=0, dwCurrentByteCount;
    WORD   wLookup256,wTemp;

    // unpack first XING_BYTES_TO_READ bytes
    unpack_data(buf,btVBRUnpack,XING_BYTES_TO_READ);

    // get selected MPEG header data
    h_id       = (btVBRUnpack[1] >> 3) & 3;
    h_sr_index = (btVBRUnpack[2] >> 2) & 3;
    h_mode     = (btVBRUnpack[3] >> 6) & 3;

    // determine offset of header
    if( h_id == 3)

    {        // mpeg1
      if( h_mode != 3 ) 
        iXingPtr+=(32+4);
      else
        iXingPtr+=(17+4);
    }
    else
    {      // mpeg2
      if( h_mode != 3 )
        iXingPtr+=(17+4);
      else
        iXingPtr+=(9+4);
    }

    // look for header title "Xing"
    if( btVBRUnpack[iXingPtr]   != 'X' ) return ERROR;  // fail
    if( btVBRUnpack[iXingPtr+1] != 'i' ) return ERROR;  // header not found
    if( btVBRUnpack[iXingPtr+2] != 'n' ) return ERROR;
    if( btVBRUnpack[iXingPtr+3] != 'g' ) return ERROR;

    // if we get this far, we have a Xing header
    // extract the info from it
    //  buf+=iXingPtr/3;

    VbrHeader->VBRSampleRate = sr_table[h_sr_index];
    if( h_id == 0 ) VbrHeader->VBRSampleRate >>= 2;
    else if( h_id == 2 ) VbrHeader->VBRSampleRate >>= 1;
    // unpack next 12 bytes (4 flags bytes, 4 frames bytes (if exist), 4 bytes bytes (if exist))
    //  unpack_data(buf,btXingUnpack,12);

    iXingPtr+=4;
    head_flags = ExtractI4(btVBRUnpack,iXingPtr); // get flags
    iXingPtr+=4;

    if( head_flags & FRAMES_FLAG ) 
    {
      VbrHeader->VBRStreamFrames = ExtractI4(btVBRUnpack,iXingPtr);
      iXingPtr+=4;
    }
    else
      VbrHeader->VBRStreamFrames = 0;

    if( head_flags & BYTES_FLAG )  
    {
      VbrHeader->VBRStreamBytes = ExtractI4(btVBRUnpack,iXingPtr); 
      iXingPtr+=4;
    }

    g_VBRGlobalData.VBRTableSize = XING_TOC_SIZE;
    
    // Calculate the total seconds
    wTemp = VbrHeader->VBRSampleRate >= 32000 ? MP3L2_3FRAME_SIZE : MP3VBR_LOW_FRAME_SIZE;
    g_VBRGlobalData.VBRTotalSeconds = (WORD)(((DWORD)VbrHeader->VBRStreamFrames * 
                                           wTemp) / VbrHeader->VBRSampleRate);
    
    // Note - multiply by 128 so we can preserve two digits of accuracy.
    g_VBRGlobalData.VBRDurPerBin = ((g_VBRGlobalData.VBRTotalSeconds * 128)/
                                 (g_VBRGlobalData.VBRTableSize + 1));
    
    // If we made it down here, we're probably OK - but need to validate table (see below).
    if( head_flags & TOC_FLAG )  
    {
	  g_wVBRSignature = 'X';
      for (i=0; i<XING_TOC_SIZE; i++) 
      {
        if (i == (XING_TOC_SIZE-1)) 
          wLookup256 = 256;
        else
          wLookup256 = btVBRUnpack[iXingPtr+i+1];
        // 4/7/04 - Change integrity check of table.  Instead of marking a
        // VBR file as invalid if the last entry is zero, mark as invalid if
        // a zero is found in the first 90.  Zeros in the last 10 entries will
        // will be filled with 0xFF.  We'll try to use as much of the table as
        // possible.
        if ((wLookup256 == 0) && (i>0))  // Check integrity of table.
        {
          if (i < 90)
          {
             g_wVBRSignature = 'U';      // Error in table (invalid) - Don't use Xing header
          } else
          {
             // We'll just fill the array with 255 if a zero is found.
             wLookup256 = 255;
          }
        }
 
        // In order to emulate the VBRi template, we only need to store the difference
        // between samples.
        dwCurrentByteCount = ((( (DWORD)VbrHeader->VBRStreamBytes * (WORD)wLookup256)+128) / 256);
        g_wVBRTOCTable[i] = (WORD)((DWORD)dwCurrentByteCount - (DWORD)dwPreviousByteCount);
        dwPreviousByteCount = dwCurrentByteCount;

      }
      iXingPtr+=XING_TOC_SIZE;
    }
	else
    	g_wVBRSignature = 'U';

    *pOffset = iXingPtr;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        GetVBRiHeader
//
//  Type:        Function
//
//  Description: Used to check for VBRi header at beginning of file and extract
//               meta-data if found. 
//
//  Inputs:      VBR_HEADER_DATA *V   Pointer to Xing header data structure to be filled in
//               *buf                 Pointer to packed buffer containing file read contents 
//                                    (to search)
//
//  Outputs:     Returns SUCCESS if VBR header found, ERROR if not.
//
//  Notes: 
// 4    VBRI          // Recognition string.
// 2    Version       // MPEG Version.
// 2    Delay         // Not filled in - I'm not sure what it's purpose is.
// 2    Quality       // Not filled in
// 4    Stream Bytes  // Number of bytes in media stream.
// 4    Stream Frames // Number of frames in media stream.
// 2    Table Size    // Size of Table
// 2    Table Scale   // ???
// 2    Entry Bytes   // # bytes in each entry of table
// 2    Entry Frames  // ???
// 200  Table         // Table correlating linear space (consumed bytes) to non-linear
//                    // space (Time) since this is variable bit rate.
/////////////////////////////////////////////////////////////////////////////////
INT _reentrant GetVBRiHeader(VBR_HEADER_DATA * VbrHeader, _packed BYTE *buf, WORD * pOffset)
{
    BYTE btVBRUnpack[VBR_BYTES_TO_READ];  // Careful - not much room on stack.
    BYTE i;
    INT iVBRiPtr=0;
    INT head_flags;
    INT h_id, h_mode, h_sr_index;
    WORD sr_table[4] = { 44100, 48000, 32000, 99999 };
    WORD wTemp1, wTemp2;

    // unpack bytes
    unpack_data(buf, btVBRUnpack, VBR_BYTES_TO_READ);

    // get selected MPEG header data
    h_id       = (btVBRUnpack[1] >> 3) & 3;
    h_sr_index = (btVBRUnpack[2] >> 2) & 3;
    h_mode     = (btVBRUnpack[3] >> 6) & 3;

    // determine offset of header
    if( h_id == 3) 
    {        // mpeg1
      if( h_mode != 3 ) 
        iVBRiPtr+=(32+4);
      else
        iVBRiPtr+=(17+4);
    }
    else
    {      // mpeg2
      if( h_mode != 3 )
        iVBRiPtr+=(17+4);
      else
        iVBRiPtr+=(9+4);
    }

    // look for header title "VBRI"
    if( btVBRUnpack[iVBRiPtr]   != 'V' ) return ERROR;  // fail
    if( btVBRUnpack[iVBRiPtr+1] != 'B' ) return ERROR;  // header not found
    if( btVBRUnpack[iVBRiPtr+2] != 'R' ) return ERROR;
    if( btVBRUnpack[iVBRiPtr+3] != 'I' ) return ERROR;

    // if we get this far, we have a VBR header
    // extract the info from it

    VbrHeader->VBRVersion = h_id;
    VbrHeader->VBRSampleRate = sr_table[h_sr_index];
    if     ( h_id == 0 ) VbrHeader->VBRSampleRate >>= 2; //divide by 4 if MPEG2.5
    //else if( h_id == 1 ) ;                             //reserved
    else if( h_id == 2 ) VbrHeader->VBRSampleRate >>= 1; //divide by 2 if MPEG2
    //else if( h_id == 3 ) ;                             //divide by 1 if MPEG1, i.e. keep what was read

    
    // Now extract the tag information.
    iVBRiPtr+=4;
    VbrHeader->VBRVersion = ExtractIVar(btVBRUnpack,iVBRiPtr,2); // get Version
    iVBRiPtr+=6;     // Skip Delay & Quality.
    VbrHeader->VBRStreamBytes = ExtractIVar(btVBRUnpack,iVBRiPtr,3); // get Hi 3 Bytes - Stream Bytes
    VbrHeader->VBRStreamBytes <<= 8;
    iVBRiPtr+=3;     // Point to Low word.
    VbrHeader->VBRStreamBytes |= ExtractIVar(btVBRUnpack,iVBRiPtr,1); // get Low Byte - Stream bytes
    iVBRiPtr+=1;     // Point to Stream Frames.
    VbrHeader->VBRStreamFrames = ExtractIVar(btVBRUnpack,iVBRiPtr,3); // get Hi Word - Stream Frames
    VbrHeader->VBRStreamFrames <<= 8;
    iVBRiPtr+=3;     // Point to Low word.
    VbrHeader->VBRStreamFrames |= ExtractIVar(btVBRUnpack,iVBRiPtr,1); // get Low Word - Stream Frames
    iVBRiPtr+=1;     // Point to Table Size.
    g_VBRGlobalData.VBRTableSize = ExtractIVar(btVBRUnpack,iVBRiPtr,2); // get Table Size
    iVBRiPtr+=2;     // Point to Table Scale.
    VbrHeader->VBRTableScale = ExtractIVar(btVBRUnpack,iVBRiPtr,2); // get Table Scale
    iVBRiPtr+=2;     // Point to Entry Bytes.
    VbrHeader->VBREntryBytes = ExtractIVar(btVBRUnpack,iVBRiPtr,2); // get Entry Bytes
    iVBRiPtr+=2;     // Point to Entry Frames.
    VbrHeader->VBREntryFrames = ExtractIVar(btVBRUnpack,iVBRiPtr,2); // get Entry Frames
    iVBRiPtr+=2;     // Point to Entry Bytes.
    
    
    g_VBRGlobalData.VBRTableSize = (g_VBRGlobalData.VBRTableSize < VBR_TOC_SIZE) ? 
                                    g_VBRGlobalData.VBRTableSize : VBR_TOC_SIZE;

    // Calculate the total seconds
    wTemp1 = VbrHeader->VBRSampleRate >= 32000 ? MP3L2_3FRAME_SIZE : MP3VBR_LOW_FRAME_SIZE;
    g_VBRGlobalData.VBRTotalSeconds = (WORD)(((DWORD)VbrHeader->VBRStreamFrames * 
                                           wTemp1) / VbrHeader->VBRSampleRate);

    g_VBRGlobalData.VBRDurPerBin = ((g_VBRGlobalData.VBRTotalSeconds * 128)/
                                 (g_VBRGlobalData.VBRTableSize + 1));

    // Retrieve the table values from the files.
    for (i=0; i<g_VBRGlobalData.VBRTableSize, i<VBR_TOC_SIZE; i++) 
    {
      // TBD - we should set some parameters that will alert us if the table is corrupt.
      //       I have seen zero entries in a VBRI table, so that is not invalid.
      g_wVBRTOCTable[i] = ExtractIVar(btVBRUnpack,iVBRiPtr,VbrHeader->VBREntryBytes); // get Data Frames.
      iVBRiPtr+=VbrHeader->VBREntryBytes;     // Increment Pointer.
    }

    g_wVBRSignature = 'V';
    *pOffset = iVBRiPtr;
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ExtractI4
//
//   Type:          Function
//
//   Description:   Big endian extract for Xing finder.
//
//   Inputs:        Ptr to buffer containing unpacked header, offset in buffer
//
//   Outputs:       RETCODE dword value
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
DWORD _reentrant ExtractI4(BYTE *buf, INT i)		// stmp 6724  Change type from INT to DWORD to allow 4-byte storage
{
  	DWORD x;									// stmp 6724  Change type from INT to DWORD to allow 4-byte storage
    // big endian extract
    x = buf[i];
    x <<= 8;
    x |= buf[i+1];
    x <<= 8;
    x |= buf[i+2];
    x <<= 8;
    x |= buf[i+3];

    return x;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ExtractIVar
//
//   Type:          Function
//
//   Description:   Big endian extract for Buffer Grab.
//
//   Inputs:        Ptr to buffer containing unpacked header, offset in buffer,
//                  Number of Bytes.
//
//   Outputs:       WORD value - Result of fetch.
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
WORD _reentrant ExtractIVar(BYTE *buf, INT i, BYTE iByteCount)
{
    WORD x = 0;
    BYTE iCount;

    if (iByteCount > 3)
    {
      SystemHalt();     // Can't return more than 3 bytes.
    }

    // big endian extract
    for (iCount=0; iCount<iByteCount; iCount++) 
    {
      x <<= 8;
      x |= buf[i+iCount];
    }

    return x;
}
///////////////////////////////////////////////////////////////////////////////
//> Name:        CalculateCurrentTimeVBR
//
//  Type:        Function
//
//  Description: Given a consumed byte count, interpolate where we are in
//               time using the VBR lookup table - for use when playing VBR files. 
//
//  Inputs:      NONE
//
//  Outputs:     NONE
//
//  Notes: 
//              1)  We can have up to 200 entries.
//              2)  Although not used here, compiling here then copying to
//                  decoder_overlay.asm eases changes.
/////////////////////////////////////////////////////////////////////////////////
#if (0)
void _reentrant CalculateCurrentTimeVBR(void)
{
  BYTE  i=0;
  WORD  wRoundError;
  WORD  wCurrentSecondsx128=0;
  WORD  wTemp;
  DWORD  dwCurrentBytePointer = ((DWORD)g_wSongByteTotalConsumedHigh<<24) + 
                                g_wSongByteTotalConsumedLow;
  DWORD  dwAccumulatedBytes=0;
  WORD  Subtraction;
  
  // loop while accumulating bytes until we exceed CurrentPosition in file or at end of table.
  while (((DWORD)dwCurrentBytePointer >= (DWORD)dwAccumulatedBytes) && (i<g_VBRGlobalData.VBRTableSize))
  {
    dwAccumulatedBytes += g_wVBRTOCTable[i++];
    wCurrentSecondsx128 += g_VBRGlobalData.VBRDurPerBin;
  }

  // Get the previous bin's value which is differential bytes between bin[i-1] and bin[i];
  wTemp = g_wVBRTOCTable[i-1];

  // Calculate the time to subtract off by interpolating between bins.  Multiply by duration per
  // bin to estimate where we are.
  wRoundError = (wTemp >> 1);
  Subtraction = (WORD)(dwAccumulatedBytes - dwCurrentBytePointer);
  Subtraction = ((g_VBRGlobalData.VBRDurPerBin * Subtraction)
                     + wRoundError) / wTemp;

  // Subtract off the interpolated difference.
  wCurrentSecondsx128 -= Subtraction;

  g_wAccumulatedSecs = wCurrentSecondsx128/128;
 
}
#endif


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
    volatile BYTE  btMPEGVer,btBitRate,btSampRate,btPaddingBit;
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

      if((btMPEGVer!=1)&(btSampRate != 3)& (btBitRate != 0)&(iLayer!=0))
       {
      // get the sampling rate and bitrate
      GetMp3Rates(btMPEGVer,btSampRate,btBitRate,iLayer,&wSampRate,&wBitRate);
      // calculate frame length
      wFrameLengthInBytes = GetFrameLength(btMPEGVer,wBitRate,wSampRate,btPaddingBit,iLayer);
      // Update frame size, SL
      g_wFrameLengthInBytes = wFrameLengthInBytes + 1;

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

            if ((wBitRate != wBitRateTemp) || (g_wVBRSignature == 'X') || (g_wVBRSignature == 'V'))
            {
            	if (MetaData->iTime)
	            	MetaData->wBitRate = (MetaData->dwNumBytes / MetaData->iTime*8) / 1000; // for stmp6282 vbr w/o TOC uses avg time
                bVBR = TRUE;
                MetaData->bVBR = VBR_TRUE;
            }
            bFrameFound = TRUE;      // second frame matches first, so take it to be right
            break;
        }
      }
       }
      // ELSE -- the frame did not match, so start over
      dwStartPosOffset++;            // increment start position to prevent looping on bad data
      bFirstFrameFound = FALSE;      // begin looking for first frame again
    }    // while (!bFrameFound)

    if (!bVBR)
    {
      // check for variable bit rates -- this is not 100% right since need to search entire file
      for (i=0; i<(MP3_FRAMES_TO_CHECK_VBR-2); i++)
      {
        iExtract = ExtractMp3Frame(iHandle,wFrameBufferPacked,btFrameUnpack);
        btBitRateTemp=((btFrameUnpack[2])>>4);
        btPaddingBitTemp=((btFrameUnpack[2]>>1)&(0x01));
		if (iExtract != SUCCESS)
			break;
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

    if (!bVBR)
      MetaData->bVBR = VBR_NOT_DETERMINED;

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
    if (FSFileRead(MP3_FRAME_HEADER_BYTES_TO_READ, iHandle, MEM_SPACE_Y, (INT)(-1), wFrameBufferPacked) != MP3_FRAME_HEADER_BYTES_TO_READ)
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
// leave 0     caused hang when 1
#define LAYER3_ONLY 0
#if LAYER3_ONLY
WORD _P wBitRateIndex3[16] = { 0, 32, 40, 48, 56,  64,  80,  96,  112, 128, 160, 192, 224, 256, 320, 999};
WORD _P wMPEG1SampRate[4] = {44100, 48000, 32000, 999};
#else
WORD _P wBitRateIndex1[16] = { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 999};
WORD _P wBitRateIndex2[16] = { 0, 32, 48, 56, 64,  80,  96,  112, 128, 160, 192, 224, 256, 320, 384, 999};
WORD _P wBitRateIndex3[16] = { 0, 32, 40, 48, 56,  64,  80,  96,  112, 128, 160, 192, 224, 256, 320, 999};
WORD _P wBitRateIndex4[16] = { 0, 32, 48, 56, 64,  80,  96,  112, 128, 144, 160, 176, 192, 224, 256, 999};
WORD _P wBitRateIndex5[16] = { 0, 8,  16, 24, 32,  40,  48,  56,  64,  80,  96,  112, 128, 144, 160, 999};
WORD _P wMPEG1SampRate[4] = {44100, 48000, 32000, 999};
WORD _P wMPEG2SampRate[4] = {22050, 24000, 16000, 999};
WORD _P wMPEG2pt5SampRate[4] = {11025, 12000, 8000, 999};
#endif
INT _reentrant GetMp3Rates(BYTE btMPEGVer,BYTE btSampRate,BYTE btBitRate,INT iLayer, WORD *wSampRate,WORD *wBitRate)
{
#if LAYER3_ONLY
    // When sdk2.600.025 is merged to sdk3.0.152, the locator tries and fails to locate these in yconst rather than on the y stack.
    // y const space allocation reduction acquired by dropping support for layer 1 and 2 here (temporary) TOVERIFY: TODO: change back & stop const cause.
    *wSampRate = wMPEG1SampRate[btSampRate];
    *wBitRate = wBitRateIndex3[btBitRate];  // assume layer 3 (mp3 means mpeg1 layer3). This is 99% of mp3 files.
#else
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
#endif
    return(SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        MP3MetaDataReadNonID3Info
//
//  Type:        Function
//
//  Description: Read VBR header and Frame header
//
//  Inputs:      NONE
//
//  Outputs:     NONE
//
//  Notes: 
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MP3MetaDataReadNonID3Info(INT iHandle, FILE_META_DATA *pMetaData)
{
    RETCODE rtn = META_DATA_NOT_FOUND;

	// The XING header should be just right after ID3V2 tag
	//this will leave the file position at where the function entered if no XING header, or just after the XING header
	MP3MetaDataReadVBRHeaders(iHandle,pMetaData);

	// Do a fast scan, looking for "FF" in the file.
	// ScanForFF returns at the first FF byte, or with an Error Code
	rtn = ScanForFF(iHandle, (WORD)MP3_SYNC_THRESHOLD, pMetaData);

	if (rtn == SUCCESS)
	{
		// search for frame header to get sample rate and bitrate
		rtn = GetMp3FrameInfo(pMetaData, iHandle);

		if (rtn != META_DATA_FOUND)
		{
			FSFileClose(iHandle);

			return rtn;
		}

		// calculate time for files with no Xing header
		if ((pMetaData->dwNumFrames == 0) || (pMetaData->iTime == 0))
			pMetaData->iTime = (INT) (pMetaData->dwNumBytes * (DWORD)8 / (pMetaData->wBitRate * (WORD) 1000));

		Mp3DecoderStartFrameFilePosition = pMetaData->dwStartPos;

	}
	return rtn;
}
