// smv.h : main header file for the smv DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "il\il.h"
#include "il\ilu.h"
#include "il\ilut.h"

#define DllExport   __declspec( dllexport )
#define WM_UPDATE	WM_USER + 1
#define WM_SHOWMESSAGE WM_USER + 2

// message id for smv library
#define SMV_MESSAGE_HRESULT 0
#define SMV_MESSAGE_INVALID_GRAPHIC_DATA_SIZE 1
#define SMV_MESSAGE_NO_VIDEO_CODEC 2
#define SMV_MESSAGE_NO_AUDIO_CODEC 3
#define SMV_MESSAGE_NO_CODEC 4

typedef struct
{
	HWND hWnd;
	HDC  hDC;
	LONGLONG markA;
	LONGLONG markB;
	int width;
	int height;
	bool bFullImage;
	int samplingfrquency;
	int nchannel;
	int framerate;
	int compressionlevel;
	int nRotation; /* 0 - N/A, 1 - Rotation Left 90, 2 - Rotation 180, 3 - Rotation Right 90 */
	int nMirror; /* 0 - N/A, 1 - Horiz, 2 - Vertical */
	int nPreviewInternal;
	bool bGraphicOptimizer;
	void (*pxImageProcess)( ILint image );
}
TRANSCODER_PARAM;
// CsmvApp
// See smv.cpp for the implementation of this class
//
DllExport LONG NewTranscoder( HWND hWnd, LPSTR pszSrcFileName, LPSTR pszDstFileName, LONGLONG* pDuration );
DllExport BOOL CloseTranscoder( void );
DllExport HBITMAP Preview( LONGLONG offset, HDC dc, SIZE sz, int nRotation, int nMirror );
DllExport BOOL Transcode( TRANSCODER_PARAM* pParam ); //HWND hWnd, double markA, double markB, int width, int height, bool bFullImage, int framerate );
DllExport BOOL StopTranscode( void );

class CsmvApp : public CWinApp
{
public:
	CsmvApp();
// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};
