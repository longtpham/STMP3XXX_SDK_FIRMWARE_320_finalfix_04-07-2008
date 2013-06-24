#ifndef _VOICE_MENU_H
#define _VOICE_MENU_H

#ifdef USE_PLAYLIST3
#include "musiclib_ghdr.h"
#endif

#define MAX_FOLDER_SIZE 12		//# of max bytes

#define DIRECTORY_SEPARATOR 0x2f

#define VOICEDIR_0		0x494f56  //   i o v 
#define VOICEDIR_1		0x004543  //     e c

#define FMDIR_0		0x004d46  //    m f

#define LINE1DIR_0		0x4e494c  //    n i l 
#define LINE1DIR_1		0x492d45  //    i - e 
#define LINE1DIR_2		0x00004e  //        n          

#define REC_TEST_0		0x534554  //    S E T 
#define REC_TEST_1		0x000054  //        T       

#ifdef USE_PLAYLIST3
extern _packed BYTE g_LastEncName[30];
#endif  // _VOICE_MENU_H
#ifdef USE_PLAYLIST3
void _reentrant AddVoiceEntry(uint24);
#endif
#endif




