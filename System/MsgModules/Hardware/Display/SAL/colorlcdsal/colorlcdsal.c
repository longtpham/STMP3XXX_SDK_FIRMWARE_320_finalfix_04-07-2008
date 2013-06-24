#include "types.h"
#include "messages.h"
#include "project.h"
#include "resource.h"
#include "sysmem.h"
//#include "sysequ.h"
//#include "sysresources.h"
#include "sysresourceapi.h"
#include "displaydriver_color.h"
#include "exec.h"
#include "colorlcdsal.h"

#include "..\..\displaymodule_color.h"
#pragma optimize 1

_reentrant void SALDisplayCopyBitmapToBitmap(INT xDestination, INT yDestination, WORD iWidthSource, WORD iHeightSource, WORD *pSource, INT wDisplayOperation, ColorFrameBuffer *pDestination);
_reentrant void CopyWordToBuffer(int x, int y, unsigned int iDataWord, int iBufferWidth, int iNumBitsToUse,WORD* pBuffer);


//These Two Globals are used to control the color of text on screen.  Change them via the LCD_SET_COLOR message.
WORD g_ForegroundColor = RGB(0xFF,0x0,0x0);	//make the fonts red by default
WORD g_BackgroundColor = RGB(0xFF,0xFF,0xFF);	//background is white by default  

ColorFrameBuffer g_FrameBuffers[NUM_COLOR_BUFFERS];
INT g_FrameBufferUpdateOrder[NUM_COLOR_BUFFERS] = {SONG_TITLE_BUFFER};
//////////////////////////////////////////////////////////////////////////////
//Allocate Space for buffers here                                           //
WORD SongTitleBuffer[SONG_TITLE_BUFFER_HEIGHT*(1+(SONG_TITLE_BUFFER_WIDTH-1)/24)] = {0x0};
//////////////////////////////////////////////////////////////////////////////


_reentrant void SALDisplayUpdate(void)
{
	int i,j;
	int iBGColorTemp;
	int iFGColorTemp;
	
	iBGColorTemp = 	g_BackgroundColor;	//save the global colors
	iFGColorTemp = 	g_ForegroundColor;
	
	for(i = 0; i < NUM_COLOR_BUFFERS; i++){
		j = g_FrameBufferUpdateOrder[i];
		if(g_FrameBuffers[j].m_bDirty){	//if the current buffer is dirty then we need to update it
			g_BackgroundColor = g_FrameBuffers[j].m_rgbBGndColor;
			g_ForegroundColor = g_FrameBuffers[j].m_rgbFGndColor;
			HALDisplaySendData(	g_FrameBuffers[j].m_iXPos,g_FrameBuffers[j].m_iYPos, g_FrameBuffers[j].m_iWidth, 
								g_FrameBuffers[j].m_iHeight, g_FrameBuffers[j].m_pBuffer, DISPLAY_MONOCHROME | DISPLAY_BUFFER);		
			
			g_FrameBuffers[j].m_bDirty = FALSE;	//clear the dirty bit since it has been updated to the LCD
		}
	} 
	
	g_BackgroundColor = iBGColorTemp;	//restore the global colors
	g_ForegroundColor = iFGColorTemp;
}

 //3200Merge+
#if defined(FUNCLET) && defined(STMP_BUILD_PLAYER)
    #pragma asm
        global FSALDisplayInit
        FSALDisplayInit:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_SAL_DISPLAY_INIT

        org p,"SYSFUNCLET_SALDisplayInit_P":
        dc      RSRC_FUNCLET_SAL_DISPLAY_INIT
    #pragma endasm

_reentrant void  SALDisplayInit_Funclet (void)
{
#else
//3200merge-
_reentrant void SALDisplayInit(void)
{
#endif //3200Merge
	int i;
	//Initialize all of the buffers
 	// filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
 	for(i = 0; i < NUM_COLOR_BUFFERS; i++){
		g_FrameBuffers[i].m_bDirty = 0;	//none of the buffers should be dirty yet since this is init
	}	
 	g_FrameBuffers[0].m_iWidth = SONG_TITLE_BUFFER_WIDTH;
 	g_FrameBuffers[0].m_iHeight = SONG_TITLE_BUFFER_HEIGHT;
	g_FrameBuffers[0].m_iXPos = SONG_TITLE_BUFFER_X;
	g_FrameBuffers[0].m_iYPos = SONG_TITLE_BUFFER_Y;
	g_FrameBuffers[0].m_rgbBGndColor = SONG_TITLE_BUFFER_BGND;	
	g_FrameBuffers[0].m_rgbFGndColor = SONG_TITLE_BUFFER_FGND;
	g_FrameBuffers[0].m_pBuffer = SongTitleBuffer; 

}

//3200merge+
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextcolorlcdsal":
    #pragma endasm
#endif
//3200Merge-



/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void SALHandleMessage(Message  *pMsg)
//
//   Type:          Function 
//
//   Description:   Handles messages passed from the Display Module
//
//   Inputs:        pMsg --message passed from the Display Module                
//
//  Notes:          Avoid placing message handlers which are used often here.  Since this is
//                  funcletized frequent messages handled here will cause latency.
//<
//////////////////////////////////////////////////////////////////////////////////////////
//3200Merge+
#if defined(FUNCLET) && defined(STMP_BUILD_PLAYER)
    #pragma asm
        global FSALHandleMessage
        FSALHandleMessage:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_SAL_HANDLE_MSG

        org p,"SYSFUNCLET_SALHandleMessage_P":
        dc      RSRC_FUNCLET_SAL_HANDLE_MSG
    #pragma endasm

_reentrant void  SALHandleMessage_Funclet (Message  *pMsg)
{
#else
//3200Merge-
// Does not match prototype. Stmp00003899
//_reentrant void SALHandleMessage(struct CMessage  *pMsg)
_reentrant void SALHandleMessage(Message  *pMsg)
{
#endif //3200merge

    INT Param1,Param2,Param3;
	
	Param1 = pMsg->m_wParams[0];
	Param2 = pMsg->m_wParams[1];
	Param3 = pMsg->m_wParams[2];
    //switch(pMsg->m_uMsg[0])
	switch(pMsg->m_wMessage)
    {
    case LCD_SET_COLOR:
		g_ForegroundColor = Param1;
		g_BackgroundColor = Param2;
		break;
	case LCD_SET_BUFFER_COLOR:
		if(Param3 < NUM_COLOR_BUFFERS){
			g_FrameBuffers[Param3].m_rgbFGndColor = Param1;
			g_FrameBuffers[Param3].m_rgbBGndColor = Param2;
		}
		break;
	case LCD_FORCE_BUFFER_UPDATE:		//redraw buffer without writing anything to it
		if(Param3 < NUM_COLOR_BUFFERS){
			g_FrameBuffers[Param3].m_bDirty = TRUE;	
		}
		break;	
    case LCD_SET_BUFFER_POSITION:	//set up the x and y coordinates for a framebuffer
		if(Param3 < NUM_COLOR_BUFFERS){
			g_FrameBuffers[Param3].m_iXPos = Param1;
			g_FrameBuffers[Param3].m_iYPos = Param2;	
		}
		break;
    //Nothing to do here
    default://Any unhandled messages are sent to the HAL
        HALHandleMessage(pMsg);
        break;
    }
}

//3200Merge+
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextcolorlcdsal":
    #pragma endasm
#endif
//3200Merge-


_reentrant void SALDisplayPrintBitmapAddress   (INT x,INT y,void _Y *pAddress ,INT bInvert, INT iFrameBufferID)
{	//need to modify to support the new resource type.
	int iSourceWidth,iSourceHeight,iSourceColorInfo;
	WORD* pBuffer;
	pBuffer = (WORD*) pAddress;
	iSourceColorInfo = pBuffer[0];
	iSourceWidth = pBuffer[1];
	iSourceHeight = pBuffer[2];

	if(iSourceColorInfo != MONOCHROME_BITMAP_RESOURCE){	//check to see if it is monochrome or color
		bInvert |= DISPLAY_COLOR;
	}
	else{
		bInvert |= DISPLAY_MONOCHROME;
	}

	if(iFrameBufferID == DIRECT_TO_LCD_BUFFER){
		HALDisplaySendData(x,y, iSourceWidth, iSourceHeight, &pBuffer[3], bInvert);
	}
	else{	//if it is not direct-to-lcd then we need to write to a buffer
		if(iFrameBufferID < NUM_COLOR_BUFFERS){
			g_FrameBuffers[iFrameBufferID].m_bDirty = TRUE;	//make the buffer dirty
			SALDisplayCopyBitmapToBitmap(x, y, iSourceWidth, iSourceHeight, &pBuffer[3], bInvert, &g_FrameBuffers[iFrameBufferID]);
		}
	}
}


_reentrant void SALDisplayClearRange(INT x,INT y,WORD iSourceWidth,WORD iSourceHeight, INT iFrameBufferID)
{
	//y coordinate represents x address for the samsung controller
	//x coordinate represents y address for the samsung controller
	if(iFrameBufferID == DIRECT_TO_LCD_BUFFER){
		HALDisplaySendData(x,y, iSourceWidth, iSourceHeight, 0, DISPLAY_CLEAR | DISPLAY_COLOR);	
   	}
	else{	//need to test more exhaustively
		if(iFrameBufferID < NUM_COLOR_BUFFERS){
			g_FrameBuffers[iFrameBufferID].m_bDirty = TRUE;
			SALDisplayCopyBitmapToBitmap(x,y,iSourceWidth,iSourceHeight,0,DISPLAY_CLEAR | DISPLAY_MONOCHROME,&g_FrameBuffers[iFrameBufferID]);
		}
	}
}

_reentrant INT SALDisplayGetWidth(void)
{
    return LCD_X_SIZE;
}
_reentrant INT SALDisplayGetHeight(void)
{
    return LCD_Y_SIZE;
}

_reentrant INT SALGetBufferWidth(INT iFrameBufferID){
	if(iFrameBufferID < NUM_COLOR_BUFFERS){
		return g_FrameBuffers[iFrameBufferID].m_iWidth;
	}
		return 0;	//a non existent frame buffer has zero width
}


#define BITS_IN_WORD 24
_reentrant void SALDisplayCopyBitmapToBitmap(INT xDestination, INT yDestination, WORD iWidthSource, WORD iHeightSource, WORD *pSource, INT wDisplayOperation, ColorFrameBuffer *pDestination){
	int i,j,iWordsPerRow;
	unsigned int iDataWord, iWordsToSkip, iBitsToUse, temp;
	
	if(xDestination > pDestination->m_iWidth || yDestination > pDestination->m_iHeight || !(wDisplayOperation & DISPLAY_MONOCHROME)){
		return;	//do nothing since it will all fall outside of the buffer or it is color resource trying to fit in fbuffer
	}

	if(xDestination < 0){ //clip on left
		return;
	}

	iWordsPerRow = 1+(iWidthSource-1)/BITS_IN_WORD; 
	iWordsToSkip = 0;
	
	if((iHeightSource + yDestination) > pDestination->m_iHeight){
		iHeightSource = pDestination->m_iHeight - yDestination;	//change the height so that we clip on the bottom of the buffer
	}
	if((iWidthSource + xDestination) > pDestination->m_iWidth){
		iWordsToSkip = (iWidthSource + xDestination - pDestination->m_iWidth)/BITS_IN_WORD;
		iWordsPerRow -= iWordsToSkip;	
	}
	for(i=0; i<iHeightSource; i++){
		iBitsToUse = iWidthSource;
		temp = BITS_IN_WORD;
		for(j=0; j < iWordsPerRow; j++){ 
			if(iBitsToUse < BITS_IN_WORD){
				temp = iBitsToUse; 	
			}
			if((wDisplayOperation & DISPLAY_CLEAR) == DISPLAY_CLEAR){
				iDataWord = 0;	//clear it	
			}
			else{
				iDataWord = *pSource++;
			}
			if(wDisplayOperation & DISPLAY_INVERT){
				iDataWord = ~iDataWord;	
			}
			CopyWordToBuffer(xDestination + j*BITS_IN_WORD,yDestination + i, iDataWord, pDestination->m_iWidth, temp,pDestination->m_pBuffer);		
			iBitsToUse -= BITS_IN_WORD;
		}
		pSource += iWordsToSkip;
	}
}
																		//iBufferWidth is in WORDs not pixels
_reentrant void CopyWordToBuffer(int x, int y, unsigned int iDataWord, int iBufferWidth, int iNumBitsToUse,WORD* pBuffer){
	unsigned int iReadMaskWordOne,iReadMaskWordTwo;
	int shamtRight, shamtLeft;
	
	pBuffer += y*(1+(iBufferWidth-1)/BITS_IN_WORD)+x/BITS_IN_WORD;	//order of operation is important here for truncation to work

	if(x%BITS_IN_WORD == 0){	//does it line up on a WORD boundary?
		if(iNumBitsToUse != BITS_IN_WORD){	
			iReadMaskWordOne = (0xFFFFFF) >> iNumBitsToUse;
			*pBuffer = (*pBuffer & iReadMaskWordOne) | (iDataWord & ~iReadMaskWordOne);
		} 
		else{									 
			*pBuffer = 	iDataWord;
		}
	}
	else{	//WORD is not word aligned in destination buffer...must be split over two words
		shamtRight = x % BITS_IN_WORD;
		shamtLeft =  BITS_IN_WORD - shamtRight;
		iReadMaskWordOne = (0xFFFFFF) << shamtLeft; 		
		iReadMaskWordTwo = ~iReadMaskWordOne;

		if((x%BITS_IN_WORD + iNumBitsToUse) < BITS_IN_WORD){	//this is the case where it isn't word aligned and fits in one WORD
			iReadMaskWordOne |= iReadMaskWordTwo >> iNumBitsToUse;	
			//we now have a correct read mask...we still need to shift the data correctly
			*pBuffer = (*pBuffer & iReadMaskWordOne) | ((iDataWord >> shamtRight) & ~iReadMaskWordOne);
			//we are done since it all fits in one word
		}
		else{	//this is the case where it isn't word aligned but still spans over two WORDS
			*pBuffer = (*pBuffer & iReadMaskWordOne) | (iDataWord >> shamtRight);	//write the first part of the word in the buffer
			
			if((x+shamtLeft) < iBufferWidth){	//do we need to clip?
				pBuffer++;
				if(iNumBitsToUse != BITS_IN_WORD){	//if we aren't using all bits then we need to change the read mask
					iReadMaskWordTwo = ~((~iReadMaskWordTwo) << (BITS_IN_WORD - iNumBitsToUse));
				}
				*pBuffer = (*pBuffer & iReadMaskWordTwo) | (iDataWord << shamtLeft);
			}			
		}
	}
}
