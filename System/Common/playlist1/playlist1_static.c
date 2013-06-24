#include "types.h"
#include "playlist.h"       //required include file
#include "playlist1.h"
#include "playlist1internal.h"


//////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant Playlist_GetPlaySet(void)
//
//   Type:          Function
//
//   Description:   Returns the current playset (PLAYSET_MUSIC or PLAYSET_VOICE)
//
//   Inputs:        none
//
//   Outputs:       INT             current playset
//
//   Notes:         
//<
//////////////////////////////////////////////////////
INT _reentrant Playlist_GetPlaySet(void)
//////////////////////////////////////////////////////
{
    return g_iPlaySet;
}
