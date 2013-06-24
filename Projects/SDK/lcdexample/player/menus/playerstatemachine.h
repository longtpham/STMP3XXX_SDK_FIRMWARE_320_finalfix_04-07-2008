#ifndef _HANDLEPLAYERSTATEMACHINE_H
#define _HANDLEPLAYERSTATEMACHINE_H

#ifdef USE_PLAYLIST3
#include "musiclib_ghdr.h"
#endif  // USE_PLAYLIST3

#ifndef QUICK_SWITCH_TIMEOUT_TIME
// change from 500ms t0 400ms  to reduce inter-song delay by 0.1 sec (8-11-2005)
#define QUICK_SWITCH_TIMEOUT_TIME 400   
#endif

//These are in the DecoderSR/DecoderCSR
#define DECODER_VBRFlag         1<<1
#define DECODER_PAUSED          1<<5
#define DECODER_STOPPED         1<<6
#define DECODER_SYNCED          1<<10
#define DECODER_PLAYING         1<<12
#define DECODER_END_OF_SONG     1<<13           // in DecoderCSR
#define DECODER_SONG_INFO       1<<15
#define DECODER_FILE_IS_OPEN    1<<16
#define DECODER_A_SET           1<<18
#define DECODER_B_SET           1<<19
#define DECODER_BAD_FILE        1<<21
#define DECODER_LOOKING_FOR_SYNC 1<<22

#define SEEK_NONE 0
#define SEEK_FFWD 1
#define SEEK_RWND -1

extern _X int g_iSeeking;
extern _X INT g_iPlayerState;

#ifdef USE_PLAYLIST3
extern BOOL bSystemInit;
extern uint24   g_is_SD_inserted;
extern BOOL bResume;
#endif  // USE_PLAYLIST3

#ifdef USE_PLAYLIST3
void _reentrant ML_browsing_app_init(void);
#endif  // USE_PLAYLIST3

#endif
