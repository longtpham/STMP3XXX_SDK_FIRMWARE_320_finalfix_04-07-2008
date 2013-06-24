////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : attachedpictureframe.h
// Description : Header file for attachedpictureframe.c
////////////////////////////////////////////////////////////////////////////////
#ifndef __ATTACHED_PICTURE_FRAME_H
#define __ATTACHED_PICTURE_FRAME_H

#define	MAX_APIC_FRAME_COUNT	1

// V2.2 frame identifiers
#define PIC		0x434950

// V2.3 and V2.4 frame identifiers
#define APIC	0x434950

// ASF Header Extension Object
#define ASF_HEADER_EXTENSION_GUID_0		0xbf03b5
#define ASF_HEADER_EXTENSION_GUID_1		0xa92e5f
#define ASF_HEADER_EXTENSION_GUID_2		0x8e11cf
#define ASF_HEADER_EXTENSION_GUID_3		0xc000e3
#define ASF_HEADER_EXTENSION_GUID_4		0x53200c
#define ASF_HEADER_EXTENSION_GUID_5		0x000065

// ASF Reserved 1
#define ASF_RESERVED_1_GUID_0			0xd3d211
#define ASF_RESERVED_1_GUID_1			0xa9baab
#define ASF_RESERVED_1_GUID_2			0x8e11cf
#define ASF_RESERVED_1_GUID_3			0xc000e6
#define ASF_RESERVED_1_GUID_4			0x53200c
#define ASF_RESERVED_1_GUID_5			0x000065

// ASF Metadata Library Object
#define ASF_METADATA_LIBRARY_GUID_0		0x231c94
#define ASF_METADATA_LIBRARY_GUID_1		0x949844
#define ASF_METADATA_LIBRARY_GUID_2		0xa149d1
#define ASF_METADATA_LIBRARY_GUID_3		0x131d41
#define ASF_METADATA_LIBRARY_GUID_4		0x70454e
#define ASF_METADATA_LIBRARY_GUID_5		0x000054

#define	MIME_TYPE_JPEG	0x00
#define	MIME_TYPE_PNG	0x01
#define	MIME_TYPE_BMP	0x02
#define	MIME_TYPE_NIL	0xff

#define OTHER					0x00	// A type not enumerated below.
#define FILE_ICON				0x01	// 32x32 PNG image that should be used as the file icon
#define OTHER_FILE_ICON			0x02	// File icon of a different size or format.
#define FRONT_COVER				0x03	// Front cover image of the album.
#define BACK_COVER				0x04	// Back cover image of the album.
#define LEAFLET_PAGE			0x05	// Inside leaflet page of the album.
#define MEDIA					0x06	// Image from the album itself.
#define LEAD_ARTIST				0x07	// Picture of the lead artist or soloist.
#define ARTIST					0x08	// Picture of the artist or performer.
#define CONDUCTOR				0x09	// Picture of the conductor.
#define BAND					0x0a	// Picture of the band or orchestra.
#define COMPOSER				0x0b	// Picture of the composer.
#define LYRICIST				0x0c	// Picture of the lyricist or text writer.
#define RECORDING_LOCATION		0x0d	// Picture of the recording location or studio.
#define DURING_RECORDING		0x0e	// Picture of the artists during recording.
#define DURING_PERFORMANCE		0x0f	// Picture of the artists during performance.
#define MOVIE_SCREEN_CAPTURE	0x10	// Picture from a movie or video related to the track.
#define COLOURRE_FISH			0x11	// Picture of a large, coloured fish.
#define ILLUSTRATION			0x12	// Illustration related to the track.
#define BAND_LOGO				0x13	// Logo of the band or performer.
#define PUBLISHER_LOGO			0x14	// Logo of the publisher (record company).

#define MAX_MIME_TYPE_STRING	20

typedef struct {
    BYTE	btPictureType;
    DWORD	dwPicturedataOffset;
    DWORD	dwPicturedataSize;
} APIC_DATA;

RETCODE _reentrant ApicV2_3and2_4ReadFrame(INT iHandle, WORD uFrameSize, INT *ptr);
RETCODE _reentrant ApicV2_2ReadFrame(INT iHandle, WORD uFrameSize, INT *ptr);
RETCODE _reentrant ApicReadObject(INT iHandle, INT iObjectSize, INT *ptr);
RETCODE _reentrant ApicInit(INT a, INT b, INT *c);
RETCODE _reentrant ApicGetPicturePostion(INT a, INT b, INT *pDataOffset);
RETCODE _reentrant ApicPostMessage(INT a, INT b, INT *c);

#endif
