#ifndef __ID3V2_INTERNAL_H
#define __ID3V2_INTERNAL_H

#ifndef MAX_STRING
#define MAX_STRING 100
#endif

#define ID3V2_HEADER_FLAG_FOOTER            0x10
#define ID3V2_HEADER_EXPERIMENTAL           0x20
#define ID3V2_HEADER_EXTENDED_HEADER        0x40
#define ID3V2_HEADER_UNSYNCRONIZATION       0x80

//We don't support these
#define ID3V2_FRAME_HEADER_TAG_ALTER_PREVERVE    0x0040
#define ID3V2_FRAME_HEADER_FILE_ALTER_PRESERVE   0x0020
#define ID3V2_FRAME_HEADER_FRAME_READ_ONLY       0x0010

//We don't support these
#define ID3V2_FRAME_HEADER_GROUPING_IDENTITY     0x4000
#define ID3V2_FRAME_HEADER_DATA_LENGTH_INDICATOR 0x0100

//We only use these to determine that we cannot read a frame with these bits present.
#define ID3V2_FRAME_HEADER_UNSYNCRONIZATION      0x0200
#define ID3V2_FRAME_HEADER_COMPRESSION           0x0800
#define ID3V2_FRAME_HEADER_ENCRYPTION            0x0400

#define V2_ID3_TAGSIZE				0x4
#define V2_ID3_EXTENDED_HEADER_SIZE	0x4
#define V2_2_FRAMESIZE				0x3
#define V2_3_AND_V2_4_FRAMESIZE		0x4

// V2.2 frame identifiers
#define TAL    0x4c4154
#define TT2    0x325454
#define TP1    0x315054

#ifdef USE_PLAYLIST3
#define TRK    0x4b5254
#endif


// V2.3 and V2.4 frame identifiers
//These represent the last 3 letters of the frame identifier.
//The function Id3V2ReadFrame() will read the first byte (the T, for example) in one word,
//and then the last 3 letters as another word.  This helps to easily determine what the identifier for
//the tag is easier.  Remember, these 3 bytes are read from the file in reversed order. (So TALB is actually
//read into 2 words:   'T' and 'BLA')
#define TALB    0x424c41
#define TIT1    0x315449
#define TIT2    0x325449
#define TPE1    0x314550
#define TRCK    0x4b4352
#define TCON    0x4e4f43
#ifdef USE_PLAYLIST3
#define TYER    0x524559
#endif

#define ENCODING_ISO_8559       0
#define ENCODING_UTF_16         1
#define ENCODING_UTF_16_NOBOM   2
#define ENCODING_UTF_8          3

#define BOM_BIG_ENDIAN          0xFFFE
#define BOM_LITTLE_ENDIAN       0xFEFF

BOOL    _reentrant Id3V2_2ReadFrame(INT iHandle, FILE_META_DATA*pMetaData, BOOL bSyncSafe);
BOOL    _reentrant Id3V2_3and2_4ReadFrame(INT iHandle, FILE_META_DATA*pMetaData, BOOL bSyncSafe);
RETCODE _reentrant Id3V2ReadVariedStringToUnicodeBuffer(INT iHandle, WORD uFlags, WORD uFrameSize,UCS3*pString,INT iBufferLength);
RETCODE _reentrant Id3V2ReadUTF16String(INT iHandle, WORD uEndianNess,WORD uFlags, WORD uFrameSize,UCS3*pString,INT iBufferLength);

INT _reentrant Id3V2ReadInt(INT iHandle, BOOL bSyncSafe, INT noOfBytes);


#endif