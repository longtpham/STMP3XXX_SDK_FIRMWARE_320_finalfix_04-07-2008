#ifndef __MP3METADATA_H
#define __MP3METADATA_H
///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: mp3metadata.h
// Description: Includes header data for mp3 meta-data functions.
///////////////////////////////////////////////////////////////////////////////

#define READ_BUFFER_SIZE 156
#define XING_BYTES_TO_READ 156
#define VBR_BYTES_TO_READ 400   // must be less than READ_BUFFER_SIZE * 3
#define MP3_BYTES_TO_READ 128
#define ID3_BYTES_TO_READ 128
#define MP3_FRAME_HEADER_BYTES_TO_READ 4
#define FRAME_HEADER_SIZE 4
#define FRAME_HEADER_PACKED 11

// *************** XING Header Equates ************************
// A Xing header may be present in the ancillary
// data field of the first frame of an mp3 bitstream
// The Xing header (optionally) contains
//      frames      total number of audio frames in the bitstream
//      bytes       total number of bytes in the bitstream
//      toc         table of contents

// toc (table of contents) gives seek points
// for random access
// the ith entry determines the seek point for
// i-percent duration
// seek point in bytes = (toc[i]/256.0) * total_bitstream_bytes
// e.g. half duration seek point = (toc[50]/256.0) * total_bitstream_bytes

#define FRAMES_FLAG     0x0001
#define BYTES_FLAG      0x0002
#define TOC_FLAG        0x0004
#define VBR_SCALE_FLAG  0x0008
#define FRAMES_AND_BYTES (FRAMES_FLAG | BYTES_FLAG)

// Frame MPEG Versions (matches MP3 frame format)
#define MPEGv2pt5    0x00
#define MPEGvERROR   0x01
#define MPEGv2       0x02
#define MPEGv1       0x03
// Frame Layer Info (matches MP3 frame format)
#define LayerI   3  // seems backwards, but matches mp3 frame format
#define LayerII  2
#define LayerIII 1

// Display selection on ID3V1 or ID3V2
#define NO_TAG_SELECTED		0
#define ID3V1_TAG_SELECTED	1
#define ID3V2_TAG_SELECTED	2

#define MP3L2_3FRAME_SIZE     1152    // Frame size for MPEG Layer 2 & 3 (# samples)
#define MP3L1FRAME_SIZE       384     // Frame size for MPEG Layer 1
#define MP3VBR_LOW_FRAME_SIZE 576     // Required constant for VBR - not sure where came from.


// VBR
#define MP3_FRAMES_TO_CHECK_VBR 100

// structure to receive extracted header for Xing or VBR
// VBRTable pointer may be NULL (but we're initializing it.)
typedef struct {
    WORD  VBRVersion;     // Version 0=MPEG2, 1=MPEG1
    WORD  VBRSampleRate;  // Sample Rate from VBR frame
    BYTE  VBRSignature;   // Xing or VBR
    DWORD VBRStreamFrames;// total bit stream frames from Xing or VBR header
    DWORD VBRStreamBytes; // total bit stream bytes from Xing or VBR header
    WORD  VBRTableScale;  // Scale of Table (only for VBRi)
    WORD  VBREntryBytes;  // Entry Bytes??
    WORD  VBREntryFrames; // Entry Frames
} VBR_HEADER_DATA;

typedef struct {
    WORD  VBRTableSize;   // Size of Table (100 for Xing, 200 for VBRi)
    WORD  VBRTotalSeconds;// Total Seconds - max out at 4660 hrs.
    WORD  VBRDurPerBin; // Duration per VBR Frame. x 128 for 2 digits of accuracy
    WORD  * VBRTable;     // VBR Table
} VBR_GLOBAL_HEADER_DATA;


extern LONG         _Y Mp3DecoderStartFrameFilePosition;



int SeekPoint(BYTE TOC[100], INT file_bytes, float percent);
// return seekpoint in bytes (may be at eof if percent=100.0)
// TOC = table of contents from Xing header
// file_bytes = number of bytes in mp3 file
// percent = play time percentage of total playtime. May be
//           fractional (e.g. 87.245)

INT _reentrant GetID3v1Tag(FILE_META_DATA * MetaData,_packed BYTE *buf);
RETCODE _reentrant GetMp3FrameInfo(FILE_META_DATA * MetaData,INT iHandle);
INT _reentrant ExtractMp3Frame(INT iHandle, WORD * wFrameBufferPacked, BYTE * btFrameUnpack);
WORD _reentrant GetFrameLength(BYTE btMPEGVer,WORD wBitRate, WORD wSampRate, BYTE btPaddingBit, INT iLayer);
INT _reentrant GetMp3Rates(BYTE btMPEGVer,BYTE btSampRate,BYTE btBitRate,INT iLayer, WORD *wSampRate,WORD *wBitRate);
DWORD _reentrant ExtractI4(unsigned char *buf, INT i); 	// stmp 6724  Change type from INT to DWORD to allow 4-byte storage
RETCODE _reentrant GetMp3MetaData (INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA *MetaData);
RETCODE _reentrant Mp3MetaDataReadID3V1(INT iHandle,FILE_META_DATA*pMetaData);

RETCODE _reentrant ScanForFF(INT iHandle, WORD wThresholdMax, FILE_META_DATA*pMetaData);

#endif
