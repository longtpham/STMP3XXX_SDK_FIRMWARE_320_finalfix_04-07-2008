//;///////////////////////////////////////////////////////////////////////////////
//; Copyright(C) SigmaTel, Inc. 2000-2001
//;
//; Filename: syscoder.xref
//; Description: Global refs from syscoder.asm
//;///////////////////////////////////////////////////////////////////////////////

#ifndef SYSCODER_XREF_C
#define SYSCODER_XREF_C

typedef struct {
    WORD wMinutes;
    WORD wSeconds;
}SONGTIME;

// Functions  
extern _fract       _Y g_frSongSecondsPerByte;
extern unsigned int _Y g_wSongCurrentMinutes;
extern unsigned int _Y g_wSongCurrentSeconds;
extern unsigned int _Y g_wSongTotalMinutes;
extern unsigned int _Y g_wSongTotalSeconds;
extern unsigned int _Y g_wSongRemainMinutes; 
extern unsigned int _Y g_wSongRemainSeconds;
extern unsigned int _Y g_wCurrentSongNumber;
extern unsigned int _Y g_wTotalSongCount;

extern unsigned int _Y g_wAccumulatedSecs;    // Added for mp3 VBR support.
extern unsigned int _Y g_wAccumulatedMSecs;

extern _asmfunc SONGTIME DecGetCurrentTime(void);
extern _asmfunc DWORD DecGetSongPos(void);
extern _asmfunc RETCODE DecSetSongPos(DWORD);
extern _asmfunc void DecSetSongPosZero(void);
extern _asmfunc void DecSetSongPosEnd(void);
extern _asmfunc void DecClearSongTime(void);
extern _asmfunc void SysSetDecoderResource(INT);

#endif // SYSCODER_XREF




