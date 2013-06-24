////////////////////////////////////////////////////////////////////////////////
// Copyright (c) SigmaTel, Inc. 2003
// Filename: ShuffleTrackOrder.c
//
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "menumanager.h"
#include "playlist.h"       //required include file
#include "playerlib.h"
#include "io.h"
#include "project.h"
#include "stringlib.h"
#include "playlist2.h"
#include "playlist2internal.h"
#include "sysmem.h"

////////////////////////////////////////////////////////////////////////////////
//	 External variables
////////////////////////////////////////////////////////////////////////////////
extern int g_iTotalTracks;

////////////////////////////////////////////////////////////////////////////////
//	 Function prototypes
////////////////////////////////////////////////////////////////////////////////
_reentrant WORD rand(void);

//////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant iGetTrackOrder( INT iTrackNumber )
//
//   Type:          Function
//
//   Description:   Read the track order number from the given track number
//
//   Inputs:        Track number
//
//   Outputs:       INT track order number if OK, 0 if error
//
//   Notes:          
//<
//////////////////////////////////////////////////////
INT _reentrant iGetTrackOrder(INT iTrackNumber )
{
	INT iFileNextEntry = 0;
	INT iTrackOrder = 0;
	
	// return if track number is invalid
	if ((iTrackNumber > 0) && (iTrackNumber <= g_iTotalTracks))
	{
	
	// traverse through the file entry pool to the track number
		while ((iTrackNumber) && (iFileNextEntry < MAX_NUM_FILES))
		{
			if ((g_FileEntryPool[iFileNextEntry].m_bAllocated) && (g_FileEntryPool[iFileNextEntry].m_bCurrentPlayset))
			{
				iTrackNumber--;
				iFileNextEntry++;
			}
			else
				iFileNextEntry++;
		}
	// return the valid track order
		if ((iFileNextEntry <= MAX_NUM_FILES))
	   		iTrackOrder = g_FileEntryPool[iFileNextEntry-1].m_iTrackOrder;
		else
		   SystemHalt();		// trap the invalid track order here
	}
	return iTrackOrder;
}

//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant vPulTrackOrder( INT iTrackNumber, INT iTrackOrder )
//
//   Type:          Function
//
//   Description:   write the track order number of the given track number
//
//   Inputs:        Track number, track order number
//
//   Outputs:       none
//
//   Notes:          
//<
//////////////////////////////////////////////////////
void _reentrant vPutTrackOrder(INT iTrackNumber, INT iTrackOrder )
{
	INT iFileNextEntry = 0;
	
	// return if track number is invalid
	if ((iTrackNumber > 0) && (iTrackNumber <= g_iTotalTracks))
	{
	
	// traverse through the file entry pool to the track number
		while ((iTrackNumber) && (iFileNextEntry < MAX_NUM_FILES))
		{
			if ((g_FileEntryPool[iFileNextEntry].m_bAllocated) && (g_FileEntryPool[iFileNextEntry].m_bCurrentPlayset))
			{
				iTrackNumber--;
				iFileNextEntry++;
			}
			else
				iFileNextEntry++;
		}
	// return the valid track order
		if ((iFileNextEntry <= MAX_NUM_FILES))
	   		g_FileEntryPool[iFileNextEntry-1].m_iTrackOrder = iTrackOrder;
		else
			SystemHalt();	// trap the invalid track number here
	}
}

#if 0
//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant RotateTracks( INT iFromTrack, INT iToTrack )
//
//   Type:          Function
//
//   Description:   Rotate the tracks from iFromTrack to the iToTrack
//
//   Inputs:        INT iFromTrack, INT iToTrack
//
//   Outputs:       none
//
//   Notes:          
//<
//////////////////////////////////////////////////////
void _reentrant RotateTracks( INT iFromTrack, INT iToTrack )
{
	INT iTrackOrder, iTopTrack;
	INT i;

	if (iFromTrack < iToTrack)
	{
		// save the track order from the top track
		iTopTrack = iGetTrackOrder( iFromTrack );
		for (i=iFromTrack+1;i<=iToTrack;i++)
		{
			// move the track order up one track
			iTrackOrder = iGetTrackOrder( i );
			vPutTrackOrder( i-1, iTrackOrder );
		} 
		// put the saved track order in the bottom track
		vPutTrackOrder( i-1, iTopTrack );
	}
}
#endif

#if 0
//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant CutAndStack( cutline, #tracks to be cut )
//
//   Type:          Function
//
//   Description:   cut a number of tracks and stack them on top
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         Tracks start with 1 to g_iTotalTracks 
//<
//////////////////////////////////////////////////////
void _reentrant CutAndStack( INT iCutLine, INT iTracksCut)
{
	INT iTopTrack, iTrackOrder, iTotalTracks;
	INT i, j;

	// check the range of cutting
	if ((iTotalTracks = iCutLine + iTracksCut) > g_iTotalTracks)
		iTotalTracks = g_iTotalTracks;

	for (i=1; i<iCutLine; i++)
		RotateTracks(1, iTotalTracks);
}

//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant MiddleTracksCut( void )
//
//   Type:          Function
//
//   Description:   shuffle the track order in the file entry pool by pulling out a number of tracks and place them on top
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:        iCutLine is the last track before the cut.  
//<
//////////////////////////////////////////////////////
void _reentrant MiddleTracksCut( void )
{
	INT iCutLine = 0;	// where the tracks are pulled
	INT iTracksCut = 0;	// how many tracks were pulled

	// exit if thre is one or no track
	if (g_iTotalTracks <= 1)
		return;
	
	// randomly pull the trasks
	while ((iCutLine == 0) || (iCutLine == g_iTotalTracks))
		iCutLine = 	rand() % g_iTotalTracks;
	while ((iTracksCut == 0) || (iTracksCut == g_iTotalTracks))
		iTracksCut = rand() % g_iTotalTracks;
	CutAndStack(iCutLine, iTracksCut+iCutLine > g_iTotalTracks? g_iTotalTracks-iCutLine : iTracksCut);
}

//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant TopTracksCut( void )
//
//   Type:          Function
//
//   Description:   Split the tracks into 2 sections and stack the bottom section on top
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:          
//<
//////////////////////////////////////////////////////
void _reentrant TopTracksCut( void )
{
	INT iCutLine = 0;	// where the tracks are pulled

	// exit if thre is one or no track
	if (g_iTotalTracks <= 1)
		return;
	
	// randomly cut the trasks into two sections
	while ((iCutLine == 0) || (iCutLine == g_iTotalTracks))
		iCutLine = 	rand() % g_iTotalTracks;
	CutAndStack(iCutLine, g_iTotalTracks-iCutLine);
}
#endif

#if 0
//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant InterLeaveTracks( void )
//
//   Type:          Function
//
//   Description:   Equally devides the tracks into two piles and interleaf them
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:          
//<
//////////////////////////////////////////////////////
void _reentrant InterLeaveTracks( void )
{
	INT iPileA, iPileB;

	// equally cut the trasks into two sections
	iPileA = 	g_iTotalTracks/2;

	iPileB = g_iTotalTracks - iPileA;

	// interleave a track from each pile until one of them is empty
	while ((iPileA != 0) && (iPileB != 0))
	{
		 RotateTracks( iPileA+1, g_iTotalTracks);
		 iPileB--;
		 RotateTracks( 1, g_iTotalTracks);
		 iPileA--;
	}
	// append the non empty pile
	while ((iPileA > 0) || (iPileB > 0))
	{
		RotateTracks( 1, g_iTotalTracks);
		if ( iPileA > 0)
			iPileA--;
		else
			iPileB--;
	}
	// Done
}

#endif
//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant SuffleTrackOrder( void )
//
//   Type:          Function
//
//   Description:   shuffle the track order in the file entry pool
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:          
//<
//////////////////////////////////////////////////////
INT _reentrant ShuffleTrackOrder( INT iIgnored1, INT iIgnored2, INT* pIgnored )
{
	INT	iShuffleCount;
	INT iHeadTrackNumber, iTailTrackNumber;
	INT iHeadTrackOrder, iTailTrackOrder;

	// exit if thre is one or no track
	if (g_iTotalTracks <= 1)
		return PLAYLIST_SUCCESS;	//Its not an error. There are simply no tracks
#if 0	
	// do middle cut
	MiddleTracksCut();
#endif	
	// randomly swap the track order of two tracks g_iTotalTracks/2 times
	iShuffleCount = 2*g_iTotalTracks;

	while (iShuffleCount > 0)
	{
	// get the head and tail shuffling tracks
		iHeadTrackNumber = iTailTrackNumber = 0;
		while (iHeadTrackNumber == iTailTrackNumber)  // the head track cannot be the same as the tail track
		{
			iHeadTrackNumber = rand() % g_iTotalTracks;
			iTailTrackNumber = rand() % g_iTotalTracks;
		}
		
		if (iHeadTrackNumber == 0) iHeadTrackNumber = g_iTotalTracks;
		if (iTailTrackNumber == 0) iTailTrackNumber = g_iTotalTracks;
		
		// get the head and tail track orders
		if ((iHeadTrackOrder = iGetTrackOrder( iHeadTrackNumber )) == 0)
			SystemHalt();						// stop in debug mode or reset the system
		if ((iTailTrackOrder = iGetTrackOrder( iTailTrackNumber )) == 0)
			SystemHalt();						// stop or reset the system
		// swap the track order numbers
		vPutTrackOrder( iHeadTrackNumber, iTailTrackOrder );
		vPutTrackOrder( iTailTrackNumber, iHeadTrackOrder );
		// adjust the shuffle counter
		iShuffleCount -= 2;		
	}
#if 0
	// Do interleave tracks
	InterLeaveTracks();
	// Do top cut
	TopTracksCut();
#endif
   	// Done
	return PLAYLIST_SUCCESS;
}

