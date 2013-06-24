////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : JpegDecodeDisplay.c
// Description : Jpeg decoder interfaces. It is between jpeg decoder and jpeg
//               applications.
////////////////////////////////////////////////////////////////////////////////
#include "JpegDecodeDisplay.h"
#include "fstypes.h"
#include "fsapi.h"
#ifdef USE_PLAYLIST5
#include "playlist5.h"
#include "playerlib.h"
#endif
//#include "fsdef.h"
#include "FileSystem.h"				// SigmaTel-specific
#include "resource.h"

#define HALDisplaySendCommand JPEG_HALDisplaySendCommand
#define HALDisplaySendDataCommand JPEG_HALDisplaySendDataCommand
#define HALDisplaySendData JPEG_HALDisplaySendData
#define HALDisplaySendPixelData JPEG_HALDisplaySendPixelData
#define HALDisplaySetContrast JPEG_HALDisplaySetContrast
#define HALDisplayDecContrast JPEG_HALDisplayDecContrast
#define HALDisplayIncContrast JPEG_HALDisplayIncContrast
#define HALDisplayBacklightOn JPEG_HALDisplayBacklightOn
#define HALDisplayBacklightOff JPEG_HALDisplayBacklightOff
#define HALHandleMessage JPEG_HALHandleMessage
#define HALDisplayReadStatus JPEG_HALDisplayReadStatus
#define HALSetScreenSaver JPEG_HALSetScreenSaver
#define HALScreenSaverType JPEG_HALScreenSaverType

#ifdef LDS514_LCD
#include "lds514.c"
#endif

#ifdef ILI814_LCD
#include "ili814.c"
#endif

#ifdef ML9341_LCD
#include "ml9341.c"
#endif

#ifdef SSD1332_LCD
#include "ssd1332.c"
#endif

#ifdef S6B33B0A_LCD
#define IGNORE_LCD16BIT_CONTROL
#ifdef COLOR_262K
#include "s6b33b0a_262k.c"
#else
#include "s6b33b0a_65k.c"
#endif
#endif

#include "fserror.h"

extern INT	Jpeg_Dec_B_init_done;
extern  unsigned int  Jpeg_Dec_Output_grayscale_flag; // ei_112

JpegControl CurrentJpeg;
unsigned int _circ InputBuffer[JPEG_INPUT_BUFFER_SIZE];
unsigned int	OutputBuffer[JPEG_OUTPUT_BUFFER_SIZE];
int g_JpegBGColor;
INT	g_iPictureType = JPEG_TYPE;
INT	g_iDecoderType = RSRC_JPEG_DECODER_P; // JPEG application will load decoder for JPEG part first

unsigned int (*JpegInit)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct) = jpeg_init;

unsigned int (*JpegDecode)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_outputbuf_ptr Jpeg_dec_output_buff_struct) = jpeg_decode;

void _reentrant Jpeg_SetDecoder(int iDecoderType)
{
	if (g_iDecoderType != iDecoderType)
	{
		if (iDecoderType == RSRC_BMP_DECODER_P)
		{
			#pragma asm
			include "resource.inc"
			include "sysequ.inc"
			include "sysresources.inc"
			extern SysLoadResource
			extern F_lc_u_b_Jpeg_P
			extern F_lc_u_e_BMP_P

		        ; Load the Jpeg decoder BMP part into P memory
		        move    #>RSRC_BMP_DECODER_P,x0             ; Resource number
		        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
		        move    #>(F_lc_u_e_BMP_P-F_lc_u_b_Jpeg_P)*3,y1 ; overlay area sz
		        move    #>F_lc_u_b_Jpeg_P,y0      ; Address of the overlay area
		        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
		        jsr     SysLoadResource

		        ; Load the Jpeg decoder BMP part into Y memory
		        move    #>RSRC_BMP_DECODER_Y_2,x0             ; Resource number
		        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
		        move    #>(F_lc_u_e_BMP_Y_2-F_lc_u_b_Jpeg_Y_2)*3,y1 ; overlay area sz
		        move    #>F_lc_u_b_Jpeg_Y_2,y0      ; Address of the overlay area
		        move    #TARGET_MEM_Y,a1                ; Overlay area is in P memory
		        jsr     SysLoadResource

			#pragma endasm
		}
		else
		{
			#pragma asm
			include "resource.inc"
			include "sysequ.inc"
			include "sysresources.inc"
			extern SysLoadResource
			extern F_lc_u_b_Jpeg_P
			extern F_lc_u_e_Jpeg_P

		        ; Load the Jpeg decoder JPEG part into P memory
		        move    #>RSRC_JPEG_DECODER_P,x0             ; Resource number
		        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
		        move    #>(F_lc_u_e_Jpeg_P-F_lc_u_b_Jpeg_P)*3,y1 ; overlay area sz
		        move    #>F_lc_u_b_Jpeg_P,y0      ; Address of the overlay area
		        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
		        jsr     SysLoadResource

		        ; Load the Jpeg decoder JPEG part into Y memory
		        move    #>RSRC_JPEG_DECODER_Y_2,x0             ; Resource number
		        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
		        move    #>(F_lc_u_e_Jpeg_Y_2-F_lc_u_b_Jpeg_Y_2)*3,y1 ; overlay area sz
		        move    #>F_lc_u_b_Jpeg_Y_2,y0      ; Address of the overlay area
		        move    #TARGET_MEM_Y,a1                ; Overlay area is in P memory
		        jsr     SysLoadResource

			#pragma endasm
		}
		g_iDecoderType = iDecoderType;
	}
}

RETCODE Jpeg_SetFile(_packed BYTE* pFileName, WORD wDataOffset){
	int iHandle;
#ifdef  USE_PLAYLIST5
	g_CurrentFastKey = ((g_PL5_Playback.queue)->pos + g_PL5_Playback.currentplay)->file.lFastKey;
	iHandle = FSFastOpen(g_CurrentFastKey, READ);
	if(iHandle == ERROR)
		iHandle = Fopen((_packed char *)pFileName,(_packed char *)"r");
	else
#else
	if((iHandle = Fopen((_packed char *)pFileName,(_packed char *)"r")) >= 2)//handles 0 and 1 are reserved by the FS
#endif
	{
		CurrentJpeg.iHandle = iHandle;
		CurrentJpeg.wJpegDataOffset = wDataOffset;
		g_iPictureType = JPEG_TYPE;
		Jpeg_SetDecoder(RSRC_JPEG_DECODER_P);
		return JPEG_SUCCESS;
	}
	return JPEG_ERROR;
}

RETCODE BMP_SetFile(_packed BYTE* pFileName, WORD wDataOffset){
	int iHandle;
#ifdef USE_PLAYLIST5
	g_CurrentFastKey = ((g_PL5_Playback.queue)->pos + g_PL5_Playback.currentplay)->file.lFastKey;
	iHandle = FSFastOpen(g_CurrentFastKey,READ);
	if(iHandle == ERROR)
		iHandle = Fopen((_packed char *)pFileName,(_packed char *)"r");
	else
#else
	if((iHandle = Fopen((_packed char *)pFileName,(_packed char *)"r")) >= 2)//handles 0 and 1 are reserved by the FS
#endif
	{
		CurrentJpeg.iHandle = iHandle;
		CurrentJpeg.wJpegDataOffset = wDataOffset;
		g_iPictureType = BMP_TYPE;
		Jpeg_SetDecoder(RSRC_BMP_DECODER_P);
		return JPEG_SUCCESS;
	}
	return JPEG_ERROR;
}

RETCODE Jpeg_DecodeDisplay(void){
	int iRet = JPEG_ERROR;
	LONG nBytesRead;

	if (g_iPictureType == JPEG_TYPE)
	{
		iRet = (*JpegDecode)(&CurrentJpeg.DecParams,
							&CurrentJpeg.InBufDesc,
							&CurrentJpeg.OutBufDesc);

		if (iRet == JPEG_RUNTIME_COMPLETE) //end of file
		{
			Jpeg_DisplayBitmap(&CurrentJpeg);
			iRet = JPEG_END_OF_DECODE;
		}
		else if(iRet == JPEG_RUNTIME_MOREDATA_REQUIRED) //more data requested
		{
			if((CurrentJpeg.InBufDesc.Input_buffer_filled_space =  Fread(CurrentJpeg.iHandle,
																	(INT *)CurrentJpeg.InBufDesc.pBuffPointer,
																	(JPEG_INPUT_BUFFER_SIZE) *3,
																	Y_MEMORY,
																	-1)) >= 0)
																	iRet = JPEG_SUCCESS;

		}
		else if(iRet == JPEG_RUNTIME_MCU_BLOCK_DONE) //current decode call successful
		{
			Jpeg_DisplayBitmap(&CurrentJpeg);
			iRet = JPEG_SUCCESS;
		}
		else
			iRet = JPEG_ERROR;
	}
	else
	{
		iRet = BMP_decode(&CurrentJpeg.DecParams,
							&CurrentJpeg.InBufDesc,
							&CurrentJpeg.OutBufDesc);
		if (iRet == JPEG_RUNTIME_COMPLETE) //end of file
		{
			Jpeg_DisplayBitmap(&CurrentJpeg);
			iRet = JPEG_END_OF_DECODE;
		}
		else if(iRet == JPEG_RUNTIME_MCU_BLOCK_DONE) //current decode call successful
		{
			if((CurrentJpeg.InBufDesc.Input_buffer_filled_space =  Fread(CurrentJpeg.iHandle,
																	(INT *)CurrentJpeg.InBufDesc.pBuffPointer,
																	(JPEG_INPUT_BUFFER_SIZE) *3,
																	Y_MEMORY,
																	-1)) >= 0)
																	iRet = JPEG_SUCCESS;
		}
		else if(iRet == JPEG_RUNTIME_MOREDATA_REQUIRED) //more data requested
		{
			Jpeg_DisplayBitmap(&CurrentJpeg);
			iRet = JPEG_SUCCESS;
		}
		else
			iRet = JPEG_ERROR;
	}

	if( iRet == JPEG_ERROR )
	{
		WORD x,y,width,height;
		int iBGColorTemp, iStripColor, i;

		x = CurrentJpeg.DisplayParams.x;
		y = CurrentJpeg.DisplayParams.y;
		width = CurrentJpeg.DecParams.output_image_width;
		height = CurrentJpeg.DecParams.output_image_height;

		iBGColorTemp = iStripColor = g_JpegBGColor;
		for( i = x; i < (x + width); i+=3 )
		{
			Jpeg_FillRectangleColor( i, y, 3, height, iStripColor );
			iStripColor = ~iStripColor;
		}
		g_JpegBGColor = iBGColorTemp;
	}
	return iRet;
}

RETCODE Jpeg_InitDecoder(void) //This will update the dimensions of input image in JpegApp structure
{
	int nBytesRead;
	int iRet;

	CurrentJpeg.DecParams.zoom_no = 1;
	// CurrentJpeg.DecParams.output_grayscale_flag = FALSE; // ei_112 , ei_105 Gray Scale Defect
                                                            // This flag should be set/cleared by
                                                            // Jpeg_DisplayGrayScale function for
                                                            // gray scale decoding.
	if(Jpeg_ResetDecoder() == JPEG_SUCCESS)
	{
		if (g_iPictureType == JPEG_TYPE) {
			do
			{
				iRet = (*JpegInit)(&CurrentJpeg.DecParams, &CurrentJpeg.InBufDesc);
				if(iRet == JPEG_INIT_MOREDATA_REQUIRED)
				{
					if((CurrentJpeg.InBufDesc.Input_buffer_filled_space = Fread(CurrentJpeg.iHandle,
																		(INT *)CurrentJpeg.InBufDesc.pBuffPointer,
																		(JPEG_INPUT_BUFFER_SIZE) *3,
																		Y_MEMORY,
																		-1)) < 0)
																		return JPEG_ERROR;

				}
				else if(iRet == JPEG_INIT_COMPLETE){
					CurrentJpeg.DecParams.x_pos_zoom = CurrentJpeg.DecParams.image_width/2;
					CurrentJpeg.DecParams.y_pos_zoom = CurrentJpeg.DecParams.image_height/2;
					return JPEG_SUCCESS;
				}
				else	//any other error, halt here.
				{
					return JPEG_ERROR;
				}

			} while (iRet != JPEG_INIT_COMPLETE); //TBD-if total num byts read exceeds a limit, data is corrupted and return error
		}
		else
		{
			CurrentJpeg.DecParams.image_width = CurrentJpeg.InBufDesc.pBuffPointer[6];
			CurrentJpeg.DecParams.image_height = ((CurrentJpeg.InBufDesc.pBuffPointer[7] >> 8) & 0xffff)
				| ((CurrentJpeg.InBufDesc.pBuffPointer[8] & 0xff) << 16);
			CurrentJpeg.DecParams.x_pos_zoom = CurrentJpeg.DecParams.image_width/2;
			CurrentJpeg.DecParams.y_pos_zoom = CurrentJpeg.DecParams.image_height/2;
		}
	}
	else
		return JPEG_ERROR;

	return JPEG_SUCCESS;
}

RETCODE Jpeg_ResetDecoder(void){
	int nBytesRead;
	reset_Decoder(&CurrentJpeg.DecParams);

	// Clear this flag to force BMP_decode to setup the variables
	Jpeg_Dec_B_init_done = FALSE;

	CurrentJpeg.InBufDesc.pBuffPointer = InputBuffer;
	CurrentJpeg.OutBufDesc.pBuffPointer = OutputBuffer;
	CurrentJpeg.OutBufDesc.output_buffer_size = JPEG_OUTPUT_BUFFER_SIZE;

	if(Fseek(CurrentJpeg.iHandle,CurrentJpeg.wJpegDataOffset,SEEK_SET) == FS_SUCCESS)
	{
		if((CurrentJpeg.InBufDesc.Input_buffer_filled_space = Fread(CurrentJpeg.iHandle,
																(INT *)CurrentJpeg.InBufDesc.pBuffPointer,
																(JPEG_INPUT_BUFFER_SIZE) *3,
																Y_MEMORY,
																-1)) < 0)
																return JPEG_ERROR;

	}
	else
		return JPEG_ERROR;

	return JPEG_SUCCESS;
}

void Jpeg_StopDecoder(void){
	Fclose(CurrentJpeg.iHandle);
}

void Jpeg_DisplayBitmap(JpegControl * pJpeg){
	int iSourceWidth,iSourceHeight,iSourceColorInfo;
	unsigned int x, y;

	x = pJpeg->DisplayParams.x + pJpeg->OutBufDesc.pBuffPointer[0];
	y = pJpeg->DisplayParams.y + pJpeg->OutBufDesc.pBuffPointer[1];
	iSourceColorInfo = pJpeg->OutBufDesc.pBuffPointer[2];
	iSourceWidth = pJpeg->OutBufDesc.pBuffPointer[3];
	iSourceHeight = pJpeg->OutBufDesc.pBuffPointer[4];

	HALDisplaySendData(x,y, iSourceWidth, iSourceHeight, &pJpeg->OutBufDesc.pBuffPointer[5], DISPLAY_COLOR);
}

void Jpeg_PanImageLeft(WORD iSkipPixels){
	CurrentJpeg.DecParams.x_pos_zoom += iSkipPixels;
}
void Jpeg_PanImageRight(WORD iSkipPixels){
	CurrentJpeg.DecParams.x_pos_zoom -= iSkipPixels;
}

void Jpeg_PanImageUp(WORD iSkipPixels){
	CurrentJpeg.DecParams.y_pos_zoom += iSkipPixels;
}

void Jpeg_PanImageDown(WORD iSkipPixels){
	CurrentJpeg.DecParams.y_pos_zoom -= iSkipPixels;
}
void Jpeg_ZoomIn(void){
	CurrentJpeg.DecParams.zoom_no++;
}

void Jpeg_ZoomOut(void){
	CurrentJpeg.DecParams.zoom_no--;
}

void Jpeg_DisplayGrayScale(BOOL bIsGrayScale){
	CurrentJpeg.DecParams.output_grayscale_flag = bIsGrayScale;
	Jpeg_Dec_Output_grayscale_flag              =  bIsGrayScale; // ei_112 , ei_105 Gray Scale Defect
																 // Assumption : TRUE : 1, FALSE 0
}

void Jpeg_UpdateDisplayParams(WORD top_left_x,WORD top_left_y,WORD display_width, WORD display_height){
	CurrentJpeg.DisplayParams.x = top_left_x;
	CurrentJpeg.DisplayParams.y = top_left_y;
	CurrentJpeg.DecParams.output_image_width = display_width;
	CurrentJpeg.DecParams.output_image_height = display_height;
}
void Jpeg_FillRectangleColor(WORD x,WORD y,WORD width,WORD height,WORD color){
	g_JpegBGColor = g_BackgroundColor;
	g_BackgroundColor = color;
	HALDisplaySendData(x,y, width, height,0, DISPLAY_CLEAR | DISPLAY_COLOR);
	g_BackgroundColor = g_JpegBGColor;
	g_JpegBGColor = color;
}
