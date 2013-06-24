/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/

/*---------------------------------------------------------------------------*\
	File:	AAActivationRecords.c
	
	Description:
		This file contains routines to set and retrieve activation records
		from an activation records file. The location and filename of the
		activation records file is hard-coded here.
\*---------------------------------------------------------------------------*/

#include "fstypes.h"
#include "fsapi.h"
#include "stringlib.h"				// SigmaTel-specific
#include "FileSystem.h"				// SigmaTel-specific
//#include <string.h>


extern const INT ActivationFilename[];


/*---------------------------------------------------------------------------*\
	Name:	AAGetActivationRecords
	
	Description:
		This routine reads from the activation records file and returns the
		data as a packed char array.
\*---------------------------------------------------------------------------*/

INT	_reentrant AAGetActivationRecords( INT pNumRecords, INT unused2, INT * pActivationBuf )
{
	int				fp;
	int				bytesToRead;
	int				retCode = 0;
	int				i;
//	_packed BYTE	tempFilename[16];
//	_packed BYTE	*p;

	// Do a little error checking
	if ( !pActivationBuf )
		return -1;

/*
	// Copy the filename over to local storage
	p = tempFilename;
	for ( i = 0; i < sizeof(tempFilename); i++, p++ ) {
		int	temp;

		temp = *p = ActivationFilename[i];
		if ( !(temp & 0xFF0000) || !(temp & 0x00FF00) || !(temp & 0x0000FF) )
			break;
	}
*/

	// Activation records are always stored in file on internal Flash
	fp = FSFileOpen( (_packed BYTE *) ActivationFilename, READ, 0 );
	if ( fp < 0 )
		return -2;

	// Read all the activation records at once
	bytesToRead = 8 * 70;
	if ( FSFileRead( bytesToRead, fp, MEM_SPACE_Y, -1, (WORD *) pActivationBuf ) != bytesToRead )
		retCode = -3;
	else {
		// All records were read in. Return the count.
		if ( pNumRecords )
			*((int *) pNumRecords) = 8;
	}
	FSFileClose( fp );

	return retCode;
}



/*---------------------------------------------------------------------------*\
	Name:	AASetActivationRecords
	
	Description:
		This routine writes to the activation records file. It loads the FAT
		write code to application space. The flow is modelled after
		audiblesongposition.c written by Sree.
\*---------------------------------------------------------------------------*/

INT	_reentrant AASetActivationRecords( INT iNumRecords, INT unused2, INT * pActivationBuf )
{
	int	fp;
	int	bytesToWrite;

	// Do a little error checking
	if ( (iNumRecords != 8) && !pActivationBuf )
		return -1;

	// Load FAT Write code to application space
	SysLoadFATWrite( );   

	// Activation records are always stored in file on internal Flash
	fp = FSFileOpen( (_packed BYTE *) ActivationFilename, WRITE, 0 );
	if ( fp < 0 ) {
	    SysUnLoadFATWrite( );
		return -2;
	}

	// Write all the activation records at once
	bytesToWrite = iNumRecords * 70;
	if ( FSFileWrite( bytesToWrite, fp, MEM_SPACE_Y, -1, (WORD *) pActivationBuf ) != bytesToWrite ) {
	    SysUnLoadFATWrite( );
		return -3;
	}

	// Close the file and make sure stuff is saved off to Flash
	FSFileClose( fp );
	FlushCache( );
    SysUnLoadFATWrite( );

	return 0;
}
