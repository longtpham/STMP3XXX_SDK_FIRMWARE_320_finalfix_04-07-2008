#ifndef _JPEGFILEUTIL_H
#define _JPEGFILEUTIL_H

/* Following is copied form playlist2.h */
#define PLAYSET_PHOTO 4
#define PLAYSET_MVIDEO	5

extern _packed BYTE DirPath[];
extern _packed char sStartDir[][25];

extern INT _reentrant JpegFile_SetPlaySet(INT iPlayset, INT bUnused, INT *pUnused);
extern RETCODE _reentrant JpegFile_GetCurrentSongFileInfo(int, int, int*);
extern RETCODE _reentrant JpegFile_GetPreviousSongFileInfo(int, int, int*);
extern RETCODE _reentrant JpegFile_GetNextSongFileInfo(int, int, int*);

#endif
