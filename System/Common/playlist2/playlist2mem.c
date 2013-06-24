#include "types.h"
#include "project.h"
#include "playlist2.h"
#include "playlist2internal.h"

DirEntry _X		g_DirEntryPerDevice[MAX_LOGICAL_DEVICES];	//ROOT(s)
DirEntry _X	    g_DirEntryPool[MAX_NUM_DIR];		//Pool of directory entries, Does not contain files.
FileEntry	g_FileEntryPool[MAX_NUM_FILES];

