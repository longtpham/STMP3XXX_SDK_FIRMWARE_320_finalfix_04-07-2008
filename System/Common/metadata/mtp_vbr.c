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
#include "kernel.h"


_Y VBR_GLOBAL_HEADER_DATA g_VBRGlobalData;
_Y VBR_HEADER_DATA g_VBRHeaderData;                  // struct for VBR Data (Xing or VBRI)

//extern WORD g_wVBRSignature;   // Xing or VBR ("X"/"V"/"U"(unknown) or 0 for none)


DWORD _reentrant ExtractI4(BYTE *buf, INT i);
WORD _reentrant ExtractIVar(BYTE *buf, INT i, BYTE iByteCount);

INT _reentrant GetXingHeader(VBR_HEADER_DATA * VbrHeader, _packed BYTE *buf, WORD * pOffset);
INT _reentrant GetVBRiHeader(VBR_HEADER_DATA * VbrHeader, _packed BYTE *buf, WORD * pOffset);


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
//    g_wVBRSignature = 0;                            // Initialize to zero (No VBR).

//    Clear the lookup table.
    for (i=0; i<VBR_TOC_SIZE; i++) 
    {
      g_wVBRTOCTable[i]=0;
    }
    g_VBRGlobalData.VBRTotalSeconds = 0;
    g_VBRGlobalData.VBRDurPerBin = 0;

    if (FSFileRead(VBR_BYTES_TO_READ, iHandle, MEM_SPACE_Y, (INT)(-1), ReadBuffer) != ERROR)
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
    h_id       = (btVBRUnpack[1] >> 3) & 1;
    h_sr_index = (btVBRUnpack[2] >> 2) & 3;
    h_mode     = (btVBRUnpack[3] >> 6) & 3;

    // determine offset of header
    if( h_id ) 
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
    if( h_id == 0 ) VbrHeader->VBRSampleRate >>= 1;

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
//    g_wVBRSignature = 'X';
    if( head_flags & TOC_FLAG )  
    {
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
//             g_wVBRSignature = 'U';      // Error in table (invalid) - Don't use Xing header
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
    h_id       = (btVBRUnpack[1] >> 3) & 1;
    h_sr_index = (btVBRUnpack[2] >> 2) & 3;
    h_mode     = (btVBRUnpack[3] >> 6) & 3;

    // determine offset of header
    if( h_id ) 
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
    if( h_id == 0 ) VbrHeader->VBRSampleRate >>= 1;

    
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

//    g_wVBRSignature = 'V';
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
//   Outputs:       RETCODE double value
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
DWORD _reentrant ExtractI4(BYTE *buf, INT i)
{
    DWORD x;
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
