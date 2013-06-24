#include "SmvJpegDecodeDisplay.h"
#include "fstypes.h"
#include "fsapi.h"
//#include "fsdef.h"
#include "FileSystem.h"				// SigmaTel-specific
#include "resource.h"

#define HALDisplaySendCommand SMVJPEG_HALDisplaySendCommand
#define HALDisplaySendDataCommand SMVJPEG_HALDisplaySendDataCommand
#define HALDisplaySendData SMVJPEG_HALDisplaySendData
#define HALDisplaySendPixelData SMVJPEG_HALDisplaySendPixelData
#define HALDisplaySetContrast SMVJPEG_HALDisplaySetContrast
#define HALDisplayDecContrast SMVJPEG_HALDisplayDecContrast
#define HALDisplayIncContrast SMVJPEG_HALDisplayIncContrast
#define HALDisplayBacklightOn SMVJPEG_HALDisplayBacklightOn
#define HALDisplayBacklightOff SMVJPEG_HALDisplayBacklightOff
#define HALHandleMessage SMVJPEG_HALHandleMessage
#define HALDisplayReadStatus SMVJPEG_HALDisplayReadStatus
#define HALSetScreenSaver SMVJPEG_HALSetScreenSaver
#define HALScreenSaverType SMVJPEG_HALScreenSaverType

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

extern INT	SmvJpeg_Dec_B_init_done;
extern unsigned int  SmvJpeg_Dec_Output_grayscale_flag; // ei_112 

JpegControl SmvCurrentJpeg;
unsigned int _circ SmvInputBuffer[JPEG_INPUT_BUFFER_SIZE];
unsigned int SmvOutputBuffer[JPEG_OUTPUT_BUFFER_SIZE];
int g_SmvJpegBGColor;
int	g_iSmvj_PictureType;
int	g_iSmvj_DecoderType; // JPEG application will load decoder for JPEG part first

unsigned int (*SmvJpegInit)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct) = smvjpeg_init;

unsigned int (*SmvJpegDecode)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_outputbuf_ptr Jpeg_dec_output_buff_struct) = smvjpeg_decode;

void _reentrant SmvJpeg_SetDecoder(int iDecoderType)
{
	if (g_iSmvj_DecoderType != iDecoderType)
	{
		#pragma asm
		include "resource.inc"
		include "sysequ.inc"
		include "sysresources.inc"
		extern SysLoadResource
		extern F_lc_u_b_Jpeg_P
		extern F_lc_u_e_Jpeg_P

		    ;; Load the Jpeg decoder JPEG part into P memory
		    ;move    #>RSRC_SMVJPEG_DECODER_P,x0             ; Resource number
		    ;move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
		    ;move    #>(F_lc_u_e_Jpeg_P-F_lc_u_b_Jpeg_P)*3,y1 ; overlay area sz
		    ;move    #>F_lc_u_b_Jpeg_P,y0      ; Address of the overlay area
		    ;move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
		    ;jsr     SysLoadResource

			; Load the ADPCM decoder into P memory
			move    #>RSRC_DEC_SMVADPCMP_CODE,x0    ; Resource number
			move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
			move    #>(F_lc_u_e_SMVADPCM_Dec_P-F_lc_u_b_SMVADPCM_Dec_P)*3,y1 ; Size of the overlay area
			move    #>F_lc_u_b_SMVADPCM_Dec_P,y0              ; Address of the overlay area
			move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
			jsr     SysLoadResource

		    ; Load the Jpeg decoder JPEG part into Y memory
		    move    #>RSRC_SMVJPEG_DECODER_Y,x0             ; Resource number
		    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
		    move    #>(F_lc_u_e_SmvJpeg_Y-F_lc_u_b_SmvJpeg_Y)*3,y1 ; overlay area sz
		    move    #>F_lc_u_b_SmvJpeg_Y,y0      ; Address of the overlay area
		    move    #TARGET_MEM_Y,a1                ; Overlay area is in P memory
		    jsr     SysLoadResource

		#pragma endasm
		g_iSmvj_DecoderType = iDecoderType;
	}
}

RETCODE SmvJpeg_SetFile(_packed BYTE* pFileName, WORD wDataOffset){
	int iHandle;
#if 0	
	if((iHandle = Fopen((_packed char *)pFileName,(_packed char *)"r")) >= 2)//handles 0 and 1 are reserved by the FS
	{
		SmvCurrentJpeg.iHandle = iHandle;
		SmvCurrentJpeg.wJpegDataOffset = wDataOffset;
		g_iSmvj_PictureType = SMVJPEG_TYPE;
		SmvJpeg_SetDecoder(RSRC_SMVJPEG_DECODER_P);
		return JPEG_SUCCESS;
	}
#endif	
	return JPEG_ERROR;
}

//RETCODE SmvBMP_SetFile(_packed BYTE* pFileName, WORD wDataOffset){
//	int iHandle;
//	if((iHandle = Fopen((_packed char *)pFileName,(_packed char *)"r")) >= 2)//handles 0 and 1 are reserved by the FS
//	{
//		SmvCurrentJpeg.iHandle = iHandle;
//		SmvCurrentJpeg.wJpegDataOffset = wDataOffset;
//		g_iSmvj_PictureType = BMP_TYPE;
//		SmvJpeg_SetDecoder(RSRC_BMP_DECODER_P);
//		return JPEG_SUCCESS;
//	}
//	return JPEG_ERROR;
//}

RETCODE SmvJpeg_DecodeDisplay(void){
	int iRet = JPEG_ERROR;
	LONG nBytesRead;

	if (g_iSmvj_PictureType == SMVJPEG_TYPE)
	{
		iRet = (*SmvJpegDecode)(&SmvCurrentJpeg.DecParams,
							&SmvCurrentJpeg.InBufDesc,
							&SmvCurrentJpeg.OutBufDesc);

		if (iRet == JPEG_RUNTIME_COMPLETE) //end of file
		{
			SmvJpeg_DisplayBitmap(&SmvCurrentJpeg);
			iRet = JPEG_END_OF_DECODE;
		}
		else if(iRet == JPEG_RUNTIME_MOREDATA_REQUIRED) //more data requested
		{
			if((SmvCurrentJpeg.InBufDesc.Input_buffer_filled_space =  Fread(SmvCurrentJpeg.iHandle,
																	(INT *)SmvCurrentJpeg.InBufDesc.pBuffPointer,
																	(JPEG_INPUT_BUFFER_SIZE) *3,
																	Y_MEMORY,
																	-1)) >= 0)
																	iRet = JPEG_SUCCESS;

		}
		else if(iRet == JPEG_RUNTIME_MCU_BLOCK_DONE) //current decode call successful
		{
			SmvJpeg_DisplayBitmap(&SmvCurrentJpeg);
			iRet = JPEG_SUCCESS;
		}
		else
			iRet = JPEG_ERROR;
	}

	if( iRet == JPEG_ERROR )
	{
		WORD x,y,width,height;
		int iBGColorTemp, iStripColor, i;

		x = SmvCurrentJpeg.DisplayParams.x;
		y = SmvCurrentJpeg.DisplayParams.y;
		width = SmvCurrentJpeg.DecParams.output_image_width;
		height = SmvCurrentJpeg.DecParams.output_image_height;

		iBGColorTemp = iStripColor = g_SmvJpegBGColor;
		for( i = x; i < (x + width); i+=3 )
		{
			SmvJpeg_FillRectangleColor( i, y, 3, height, iStripColor );
			iStripColor = ~iStripColor;
		}
		g_SmvJpegBGColor = iBGColorTemp;
	}
	return iRet;
}

RETCODE SmvJpeg_InitDecoder(void) //This will update the dimensions of input image in JpegApp structure
{
	int nBytesRead;
	int iRet;

	SmvCurrentJpeg.DecParams.zoom_no = 1;
	// SmvCurrentJpeg.DecParams.output_grayscale_flag = FALSE; // ei_112 , ei_105 Gray Scale Defect 
                                                            // This flag should be set/cleared by 
                                                            // Jpeg_DisplayGrayScale function for
                                                            // gray scale decoding.
	if(SmvJpeg_ResetDecoder() == JPEG_SUCCESS)
	{
		if (g_iSmvj_PictureType == SMVJPEG_TYPE) {
			do
			{
				iRet = (*SmvJpegInit)(&SmvCurrentJpeg.DecParams, &SmvCurrentJpeg.InBufDesc);
				if(iRet == JPEG_INIT_MOREDATA_REQUIRED)
				{
					if((SmvCurrentJpeg.InBufDesc.Input_buffer_filled_space = Fread(SmvCurrentJpeg.iHandle,
																		(INT *)SmvCurrentJpeg.InBufDesc.pBuffPointer,
																		(JPEG_INPUT_BUFFER_SIZE) *3,
																		Y_MEMORY,
																		-1)) < 0)
																		return JPEG_ERROR;

				}
				else if(iRet == JPEG_INIT_COMPLETE){
					SmvCurrentJpeg.DecParams.x_pos_zoom = SmvCurrentJpeg.DecParams.image_width/2;
					SmvCurrentJpeg.DecParams.y_pos_zoom = SmvCurrentJpeg.DecParams.image_height/2;
					return JPEG_SUCCESS;
				}
				else	//any other error, halt here.
				{
					return JPEG_ERROR;
				}

			} while (iRet != JPEG_INIT_COMPLETE); //TBD-if total num byts read exceeds a limit, data is corrupted and return error
		}
	}
	else
		return JPEG_ERROR;

	return JPEG_SUCCESS;
}

RETCODE SmvJpeg_ResetDecoder(void){
	int nBytesRead;
	smvj_reset_Decoder(&SmvCurrentJpeg.DecParams);

	// Clear this flag to force BMP_decode to setup the variables
	SmvJpeg_Dec_B_init_done = FALSE;

	SmvCurrentJpeg.InBufDesc.pBuffPointer = SmvInputBuffer;
	SmvCurrentJpeg.OutBufDesc.pBuffPointer = SmvOutputBuffer;
	SmvCurrentJpeg.OutBufDesc.output_buffer_size = JPEG_OUTPUT_BUFFER_SIZE;

	if(Fseek(SmvCurrentJpeg.iHandle,SmvCurrentJpeg.wJpegDataOffset,SEEK_SET) == FS_SUCCESS)
	{
		if((SmvCurrentJpeg.InBufDesc.Input_buffer_filled_space = Fread(SmvCurrentJpeg.iHandle,
																(INT *)SmvCurrentJpeg.InBufDesc.pBuffPointer,
																(JPEG_INPUT_BUFFER_SIZE) *3,
																Y_MEMORY,
																-1)) < 0)
																return JPEG_ERROR;

	}
	else
		return JPEG_ERROR;

	return JPEG_SUCCESS;
}

void SmvJpeg_StopDecoder(void){
	Fclose(SmvCurrentJpeg.iHandle);
}

void SmvJpeg_DisplayBitmap(JpegControl * pJpeg){
	int iSourceWidth,iSourceHeight,iSourceColorInfo;
	unsigned int x, y;

	x = pJpeg->DisplayParams.x + pJpeg->OutBufDesc.pBuffPointer[0];
	y = pJpeg->DisplayParams.y + pJpeg->OutBufDesc.pBuffPointer[1];
	iSourceColorInfo = pJpeg->OutBufDesc.pBuffPointer[2];
	iSourceWidth = pJpeg->OutBufDesc.pBuffPointer[3];
	iSourceHeight = pJpeg->OutBufDesc.pBuffPointer[4];

	HALDisplaySendData(x,y, iSourceWidth, iSourceHeight, &pJpeg->OutBufDesc.pBuffPointer[5], DISPLAY_COLOR);
}

void SmvJpeg_UpdateDisplayParams(WORD top_left_x,WORD top_left_y,WORD display_width, WORD display_height){
	SmvCurrentJpeg.DisplayParams.x = top_left_x;
	SmvCurrentJpeg.DisplayParams.y = top_left_y;
	SmvCurrentJpeg.DecParams.output_image_width = display_width;
	SmvCurrentJpeg.DecParams.output_image_height = display_height;
}
void SmvJpeg_FillRectangleColor(WORD x,WORD y,WORD width,WORD height,WORD color){
	g_SmvJpegBGColor = g_BackgroundColor;
	g_BackgroundColor = color;
	HALDisplaySendData(x,y, width, height,0, DISPLAY_CLEAR | DISPLAY_COLOR);
	g_BackgroundColor = g_SmvJpegBGColor;
	g_SmvJpegBGColor = color;
}
