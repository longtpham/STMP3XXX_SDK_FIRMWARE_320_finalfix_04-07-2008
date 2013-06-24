#ifndef SMVJPEGDECODEDISPLAY_H
#define SMVJPEGDECODEDISPLAY_H


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
#define SMVJPEG_TYPE    3

RETCODE SmvJpeg_ResetDecoder(void);
WORD SmvJpeg_GetEmptySpace(JpegControl*);
WORD SmvJpeg_GetFilledSpace(JpegControl*);
RETCODE SmvJpeg_SetFile(_packed BYTE* pFileName, WORD wDataOffset);
//RETCODE SmvBMP_SetFile(_packed BYTE* pFileName, WORD wDataOffset);
RETCODE SmvJpeg_InitDecoder(void);
void SmvJpeg_UpdateInputImageCoordinates(WORD x, WORD y);
//void SmvJpeg_UpdateZoom(WORD zoom_number);
void SmvJpeg_DisplayGrayScale(BOOL bIsGrayScale);
void SmvJpeg_UpdateDisplayParams(WORD top_left_x,WORD top_left_y,WORD display_width, WORD display_height);
RETCODE SmvJpeg_DecodeDisplay(void);
void SmvJpeg_StopDecoder(void);
void SmvJpeg_DisplayBitmap(JpegControl * pJpeg);
void SmvJpeg_FillRectangleColor(WORD x,WORD y,WORD width,WORD height,WORD color);
extern unsigned int (*SmvJpegInit)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct);
extern unsigned int (*SmvJpegDecode)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_outputbuf_ptr Jpeg_dec_output_buff_struct);

#endif