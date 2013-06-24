#ifndef _FILESPEC_H_
#define _FILESPEC_H_
#include  "fstypes.h"
#define MAX_FILESNAME   13

typedef struct {
    INT     gCurrentRecord;
    INT     DirAttribute;
    _packed char    FileName[9];
    _packed char    FileExtension[4];
}FILESPEC;

typedef struct {
    INT attrib;
	LONG FileSize;
    int  device;
    INT startrecord;
    _packed char name[MAX_FILESNAME];
	LONG Key;
}Finddata;
#endif

