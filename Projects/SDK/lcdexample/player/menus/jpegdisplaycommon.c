////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : jpegdisplaycommon.c
// Description : Common modules for jpeg applications
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "exec.h"
#include "messages.h"
#include "menumanager.h"
#include "menus.h"
#include "display.h"
#include "JpegDecodeDisplay.h"
#include "resource.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include  "jpegfileutil.h"
#else
#ifdef  USE_PLAYLIST5
#include  "playlist5.h"
#endif
#endif
#endif
#endif

void _reentrant Jpeg_LoadDecoder(void)
{
	#pragma asm
	include "resource.inc"
	include "sysequ.inc"
	include "sysresources.inc"
	extern SysLoadResource
	extern F_lc_u_e_Jpeg_P
	extern F_lc_u_b_Jpeg_P
	extern F_lc_u_e_Jpeg_X
	extern F_lc_u_b_Jpeg_X
	extern F_lc_u_e_Jpeg_Y_1
	extern F_lc_u_b_Jpeg_Y_1
	extern F_lc_u_e_Jpeg_Y_2
	extern F_lc_u_b_Jpeg_Y_2

        ; Load the Jpeg decoder into P memory
        move    #>RSRC_JPEG_DECODER_P,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_Jpeg_P-F_lc_u_b_Jpeg_P)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_Jpeg_P,y0      ; Address of the overlay area
        move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
        jsr     SysLoadResource

        ; Load the Jpeg decoder into X memory
        move    #>RSRC_JPEG_DECODER_X,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_Jpeg_X-F_lc_u_b_Jpeg_X)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_Jpeg_X,y0      ; Address of the overlay area
        move    #TARGET_MEM_X,a1                ; Overlay area is in P memory
        jsr     SysLoadResource

        ; Load the Jpeg decoder into Y_1 and Y_2 memory
        move    #>RSRC_JPEG_DECODER_Y,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_Jpeg_Y_1-F_lc_u_b_Jpeg_Y_1)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_Jpeg_Y_1,y0      ; Address of the overlay area
        move    #TARGET_MEM_Y,a1                ; Overlay area is in P memory
        jsr     SysLoadResource

        move    #>RSRC_JPEG_DECODER_Y_2,x0             ; Resource number
        move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
        move    #>(F_lc_u_e_Jpeg_Y_2-F_lc_u_b_Jpeg_Y_2)*3,y1 ; overlay area sz
        move    #>F_lc_u_b_Jpeg_Y_2,y0      ; Address of the overlay area
        move    #TARGET_MEM_Y,a1                ; Overlay area is in P memory
        jsr     SysLoadResource

	#pragma endasm
}

void _reentrant Jpeg_Helper( _packed BYTE* szFilename, INT iCleanScreen )
{
	WORD	wFileExtension;

    if( szFilename != NULL )
    {
	    Jpeg_StopDecoder();
        if( iCleanScreen )
	    Jpeg_FillRectangleColor(0,0,LCD_SIZE_X,LCD_SIZE_Y,RGB(0xFF,0xFF,0xFF));
		wFileExtension = SysCallFunction(RSRC_PLAYLIST2INIT_CODEBANK, Playlist_GetFileExtention, 0, 0, (INT*)szFilename);
		if (wFileExtension == JPG_FILE_EXT)
			Jpeg_SetFile(szFilename, 0);
		else
			BMP_SetFile(szFilename, 0);

	    //if(Jpeg_InitDecoder() != JPEG_SUCCESS)
		//    __asm(" debug");
		Jpeg_InitDecoder();
		Jpeg_ResetDecoder();  //stmp00012780
    }
    else
    {
        if( iCleanScreen )
		Jpeg_FillRectangleColor(0,0,LCD_SIZE_X,LCD_SIZE_Y,RGB(0xFF,0xFF,0xFF));
		Jpeg_ResetDecoder();
    }
}

void _reentrant StartJpegCallbackTimer( void )
{
   	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_JPEG_DECODER_CALLBACK,0,TIMER_JPEG_DECODER_TIMEOUT_MS,MENU_MSG_JPEG_CALLBACK);
	SysWaitOnEvent(0,0,0);
}

void _reentrant KillJpegCallbackTimer( void )
{
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_JPEG_DECODER_CALLBACK);
	SysWaitOnEvent(0,0,0);
}