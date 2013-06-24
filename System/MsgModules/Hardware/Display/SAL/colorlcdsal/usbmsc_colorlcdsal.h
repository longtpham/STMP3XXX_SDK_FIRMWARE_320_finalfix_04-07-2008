#ifndef COLOR_FRAME_BUFFER_H
#define COLOR_FRAME_BUFFER_H

#include "types.h"

#if defined(COLOR_262K) || defined(COLOR_65K)
typedef struct{
	unsigned int m_iWidth: 11;	//width	in pixels
	unsigned int m_bDirty: 1;
	unsigned int m_iHeight: 12;	//height in pixels
	unsigned int m_iXPos: 12;	//x coordinate on display
	unsigned int m_iYPos: 12;	//give the Y an extra bit since most color displays are taller than wide 
	unsigned int m_rgbFGndColor;
	unsigned int m_rgbBGndColor;
	WORD* 		m_pBuffer;	//pointer to the actual buffer in memory
}ColorFrameBuffer;
#else
typedef struct{
	unsigned int m_iWidth: 11;	//width	in pixels
	unsigned int m_bDirty: 1;
	unsigned int m_iHeight: 12;	//height in pixels
	unsigned int m_iXPos: 12;	//x coordinate on display
	unsigned int m_iYPos: 12;	//give the Y an extra bit since most color displays are taller than wide 
	unsigned int m_rgbFGndColor: 12;
	unsigned int m_rgbBGndColor: 12;
	WORD* 		m_pBuffer;	//pointer to the actual buffer in memory
}ColorFrameBuffer;
#endif

//do not change this #define statement
#define DIRECT_TO_LCD_BUFFER -1

//use the below enum to give meaningful names to your frame buffers
//names should appear in the order in which the ColorFrameBuffer structs are in g_FrameBuffers array
#define NUM_COLOR_BUFFERS 1	//increase this when adding more buffers
enum buffer_names { 
	SONG_TITLE_BUFFER		//give each buffer a meaningful name for use in SysPostMessage calls
};


//use these to define attributes for the FrameBuffers.  These can be used in SALDisplayInit to setup the FrameBuffers
//
#define SONG_TITLE_BUFFER_WIDTH		128
#define SONG_TITLE_BUFFER_HEIGHT 	10
#define SONG_TITLE_BUFFER_X			0
#define SONG_TITLE_BUFFER_Y			70
#define SONG_TITLE_BUFFER_FGND		RGB(0xFF,0xFF,0x0)
#define SONG_TITLE_BUFFER_BGND		RGB(0x0,0x0,0x0)


//do not modify the below constants as they are tied to color and monochrome resources in resource.bin
#define MONOCHROME_BITMAP_RESOURCE	0
#define COLOR_BITMAP_256_RESOURCE	1
#define COLOR_BITMAP_4096_RESOURCE	2
#define COLOR_BITMAP_65K_RESOURCE	3

#endif	//end of #ifndef COLOR_FRAME_BUFFER_H