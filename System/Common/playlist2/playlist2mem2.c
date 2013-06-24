#include "playlist2internal.h"	 

struct Bookmark	g_MarkerMusic, g_MarkerVoice;
#ifdef PLAYLIST2_MULTIMEDIA
struct Bookmark		g_MarkerMVideo, g_MarkerPhoto;
#endif
EntryAccessInfo g_CurrentTrack;

