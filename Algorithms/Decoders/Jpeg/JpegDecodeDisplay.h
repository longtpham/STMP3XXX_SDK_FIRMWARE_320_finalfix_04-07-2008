////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : JpegDecoderDisplay.h
// Description : Header file for JpegDecodeDisplay
////////////////////////////////////////////////////////////////////////////////
#ifndef JPEGDECODEDISPLAY_H
#define JPEGDECODEDISPLAY_H


#define SDK_PLATFORM

#include "JpegApp.h"
#include "types.h"

struct Jpeg_display_struct{
	WORD x;
	WORD y;
};

struct Jpeg_Decode_Display{
	struct Jpeg_dec_app_struct DecParams;
	struct Jpeg_dec_input_buff_struct InBufDesc;
	struct Jpeg_dec_output_buff_struct OutBufDesc;
	//the above are from eInfoChips
	struct Jpeg_display_struct DisplayParams;
	int	iHandle;
	WORD wJpegDataOffset;
};

typedef struct Jpeg_Decode_Display JpegControl;

//define buffer sizes here
#define JPEG_INPUT_BUFFER_SIZE 1024
#define JPEG_OUTPUT_BUFFER_SIZE (768+5)

//define error codes here
#define JPEG_ERROR -1
#define JPEG_SUCCESS 0
#define JPEG_END_OF_DECODE 1

#define UNKNOWN_TYPE	0
#define JPEG_TYPE		1
#define BMP_TYPE		2

RETCODE Jpeg_ResetDecoder(void);
WORD Jpeg_GetEmptySpace(JpegControl*);
WORD Jpeg_GetFilledSpace(JpegControl*);
RETCODE Jpeg_SetFile(_packed BYTE* pFileName, WORD wDataOffset);
RETCODE BMP_SetFile(_packed BYTE* pFileName, WORD wDataOffset);
RETCODE Jpeg_InitDecoder(void);
void Jpeg_UpdateInputImageCoordinates(WORD x, WORD y);
//void Jpeg_UpdateZoom(WORD zoom_number);
void Jpeg_DisplayGrayScale(BOOL bIsGrayScale);
void Jpeg_UpdateDisplayParams(WORD top_left_x,WORD top_left_y,WORD display_width, WORD display_height);
RETCODE Jpeg_DecodeDisplay(void);
void Jpeg_StopDecoder(void);
void Jpeg_DisplayBitmap(JpegControl * pJpeg);
void Jpeg_FillRectangleColor(WORD x,WORD y,WORD width,WORD height,WORD color);
void Jpeg_PanImageDown(WORD iSkipPixels);
void Jpeg_PanImageUp(WORD iSkipPixels);
void Jpeg_PanImageRight(WORD iSkipPixels);
void Jpeg_PanImageLeft(WORD iSkipPixels);
void Jpeg_ZoomOut(void);
void Jpeg_ZoomIn(void);
extern unsigned int (*JpegInit)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct);
extern unsigned int (*JpegDecode)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_outputbuf_ptr Jpeg_dec_output_buff_struct);

#endif