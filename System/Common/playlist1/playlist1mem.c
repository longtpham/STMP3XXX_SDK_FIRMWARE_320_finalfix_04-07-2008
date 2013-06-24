#include "types.h"
#include "project.h"
#include "playlist1.h"
#include "playlist1internal.h"

//these declarations are placed in a separate file so that we can more easily locate where the playlist specific information ends up.


//these are special file entries for the roots of the logical devices.
struct FileEntry g_FileEntryPerDevice[MAX_LOGICAL_DEVICES];

//This is the pool of file entries that the content tree will use.
struct FileEntry  g_FileEntryPool[PLAYLIST_MAX_FILES];

//This contains a list of pointers to file entry's in the g_FileEntryPool
struct FileEntry *g_PlayList[PLAYLIST_MAX_FILES];

//This contains a list of indexes to the g_PlayList Array
int    g_iInternalList[PLAYLIST_MAX_FILES];