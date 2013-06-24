#include "types.h"
#include "exec.h"
#include "fstypes.h"
#include "fsapi.h"
#include "menumanager.h"
#include "resource.h"
#include "sysmem.h"
#include "menus.h"
#include "display.h"
#include "playerstatemachine.h"
#include "hwequ.h"
#include "playerlib.h"
#include "SysResourceApi.h"
#include "stringlib.h"
#include "FileSpec.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include  "jpegfileutil.h"
#include  "musiclib_ghdr.h"
#else
#ifdef USE_PLAYLIST5
#include "playlist5.h"
#endif
#endif
#endif
#endif

#define MVIDEO_ERROR        1
#define MVIDEO_SUCCESS      0

#include "smvjpegdecodedisplay.h"

	#pragma asm
	nolist
	include "sysresources.inc"
	include "sysequ.inc"
	list
	#pragma endasm

extern INT DecoderResourcePtr;

extern INT stc_iHandle;
extern _X INT stc_iByteinJpegBlock, stc_iVideoWidth, stc_iVideoHeight, stc_iFrameHeight; // stc_iFrameHeight is aligned to 16 boundary
extern _X INT stc_iFrameDuration, stc_iFramePerSec, stc_iFrameInJpegBlock, stc_iDurationInJpegBlock;
extern _X INT stc_iFrameDrop, stc_iNumFrameStop;
extern _X LONG stc_dwLastTime, stc_dwCurrentTime;
extern _X LONG stc_dwTotalFrame, stc_dwTotalFrameBackup, stc_dwVideoStart;

int _reentrant OpenSMVFile(int aa, int b, int *pPtr);
int _reentrant CloseSMVFile(int a, int b, int *pPtr);

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant MotionVideoMenuInitState(int a, int b, int *pPtr)
//
//   Type:          Function (user menu)
//
//   Description:   This function is the motionvideomenuinitstate
//
//   Inputs:        none used.
//
//   Outputs:       value of which menu to return to
//
//   Notes:
//
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant MotionVideoMenuInitState(int a, int b, int *pPtr)
{
	union DisplayHints *pDisplayHint = (union DisplayHints *)pPtr;
	int  iEvent;

    a;b;
    if(!(g_wDecoderSR & DECODER_STOPPED))
    {   if(DecoderResourcePtr != RSRC_DEC_SMVADPCM_MOD_CODE)
        {   //we need to refresh the screen since entering the menu
            pDisplayHint->I = JPEG_DISPLAY_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

            SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,NULL);
#ifdef USE_PLAYLIST3
	        SysWaitOnEvent(0,0,0);
#endif
            g_iPlayerState = DECODER_STOPPED;

        	// Flush the message in queue
        	do	{
            	iEvent = SysWaitOnEvent(EVENT_MESSAGE,&gEventInfo.msg, 500);
            }while(iEvent != EVENT_TIMER);
        }
		else
		{
				EnterNonReentrantSection();
#pragma asm
				include "resource.inc"
				org     p,".ptextmotionvideomenuinitstate":

		        ;; Load the Jpeg decoder JPEG part into P memory
		        ;move    #>RSRC_SMVJPEG_DECODER_P,x0             ; Resource number
		        ;move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
		        ;move    #>(F_lc_u_e_SmvJpeg_P-F_lc_u_b_Jpeg_P)*3,y1 ; overlay area sz
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

#pragma endasm
				LeaveNonReentrantSection();
		}
    }
    return 0;
}

int _reentrant CloseSMVFile(int a, int b, int *pPtr)
{
    a;b;pPtr;
	if( stc_iHandle != -1)
	{
		Fclose(stc_iHandle);
		stc_iHandle = -1;
	}
	return 1;
}

int _reentrant OpenSMVFile(int aa, int b, int *pPtr)
{
	void _Y *       pBuffer = (WORD *) &g_wUserScratchY;
	_packed char    szFilename[MAX_FILENAME_LENGTH];
	SONGFILEINFO    SongInfo;
    int             a, rtn = MVIDEO_ERROR;
    DWORD           dwValue;

    aa;b;pPtr;
	if(stc_iHandle == -1)   {
    	SongInfo.m_wBufferLength = MAX_FILENAME_LENGTH;
    	SongInfo.m_pFilename = (_packed BYTE*)szFilename;
#ifndef USE_PLAYLIST3
        if( SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetCurrentSongFileInfo, 0, 0, (INT*)&SongInfo ) == PLAYLIST_SUCCESS )
#else
        if(JpegFile_GetCurrentSongFileInfo(0,0,(INT*)&SongInfo) == PLAYLIST_SUCCESS)
#endif
        {
    
        //open smw file
#ifdef   USE_PLAYLIST5
            g_CurrentFastKey = ((g_PL5_Playback.queue)->pos + g_PL5_Playback.currentplay)->file.lFastKey;
            stc_iHandle = FSFastOpen(g_CurrentFastKey, READ);
            if(stc_iHandle < 0)
#else
            if((stc_iHandle = Fopen( szFilename, (_packed char *)"r" )) < 0)
#endif
            {   stc_iHandle = -1;
                return(rtn);
            }

            // read RIFF trunk
            Fread( stc_iHandle, (void*)pBuffer, 8, DEFAULT_MEMORY, (INT)-1 );
            dwValue = 0;
            for( a = 7; a > 3; a-- )
                dwValue = (dwValue << 8 ) + GetByteFromArrayInY( pBuffer, a );

            //skip wave audio part
            if(Fseek( stc_iHandle, dwValue, SEEK_CUR ) < 0)
            {   Fclose( stc_iHandle );
                stc_iHandle = -1;
                return(rtn);
            }

			//extract motion video graphic header
            Fread( stc_iHandle, (void*)pBuffer, (16*3)+3, DEFAULT_MEMORY, (INT)-1 );
			stc_iByteinJpegBlock = 0;
    		for( a = 23; a > 20; a-- )
    			stc_iByteinJpegBlock = (stc_iByteinJpegBlock << 8) + GetByteFromArrayInY( pBuffer, a );
            stc_iVideoWidth = GetByteFromArrayInY( pBuffer, 9 );
            stc_iVideoHeight = GetByteFromArrayInY( pBuffer, 12 );
            if(stc_iVideoHeight & 0x0F)
                stc_iFrameHeight = (stc_iVideoHeight + 16) - (stc_iVideoHeight % 16);
            else
                stc_iFrameHeight = stc_iVideoHeight;
			stc_iFramePerSec = GetByteFromArrayInY( pBuffer, 24 );
            stc_iFrameDuration = (DWORD)1000000 / (DWORD)stc_iFramePerSec;  // Use us for handling non-integer FrameDuration
    		stc_dwTotalFrame = 0;
    		for( a = 29; a > 26; a-- )
    			stc_dwTotalFrame = (stc_dwTotalFrame << 8) + GetByteFromArrayInY( pBuffer, a );
            stc_dwTotalFrameBackup = stc_dwTotalFrame;
            stc_iFrameDrop = stc_iFrameDuration * 3 / 2;

			stc_iFrameInJpegBlock = 0;
			for( a = 38; a > 35; a-- )
				stc_iFrameInJpegBlock = (stc_iFrameInJpegBlock << 8) + GetByteFromArrayInY( pBuffer, a );

            stc_iDurationInJpegBlock = stc_iFrameDuration * stc_iFrameInJpegBlock;

			//the beginning of the first Jpeg block for frame data
            stc_dwVideoStart = Ftell(stc_iHandle);

            rtn = MVIDEO_SUCCESS;
        }
    }
    return(rtn);
}
