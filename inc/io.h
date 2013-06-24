#ifndef _FILESYSTEM_HELPER_H
#define _FILESYSTEM_HELPER_H

#include "filesystem.h"

#define _MAX_FNAME 13

//enough room for 8.3 plus null

struct _finddata_t 
{
    unsigned    attrib;
    long        size;
    int         device;
    int         startrecord;
    _packed BYTE name[_MAX_FNAME];
};

_reentrant int _findclose( int handle );
_reentrant int _findfirst(_packed BYTE *filespec, struct _finddata_t *fileinfo );
_reentrant int _findnext ( int handle, struct _finddata_t *fileinfo );

#endif