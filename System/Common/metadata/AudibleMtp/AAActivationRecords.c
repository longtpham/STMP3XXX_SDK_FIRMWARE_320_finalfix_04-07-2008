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
#include "fserror.h"
#include "stringlib.h"				// SigmaTel-specific
#include "FileSystem.h"				// SigmaTel-specific
//#include <string.h>


#define	CHAR_TO_INT(a,b,c)	((((INT) (c)) << 16) | (((INT) (b)) << 8) | (INT) (a))

// "a:/Audible/AudibleActivation.sys"
const INT ActivationFolder[] = {
	CHAR_TO_INT( 'a', ':', '/' ),
	CHAR_TO_INT( 'A', 'u', 'd' ),
	CHAR_TO_INT( 'i', 'b', 'l' ),
	CHAR_TO_INT( 'e', '/', '\0' ),
};
const INT ActivationFilename[] = {
	CHAR_TO_INT( 'a', ':', '/' ),
	CHAR_TO_INT( 'A', 'u', 'd' ),
	CHAR_TO_INT( 'i', 'b', 'l' ),
	CHAR_TO_INT( 'e', '/', 'A' ),
	CHAR_TO_INT( 'u', 'd', 'i' ),
	CHAR_TO_INT( 'b', 'l', 'e' ),
	CHAR_TO_INT( 'A', 'c', 't' ),
	CHAR_TO_INT( 'i', 'v', 'a' ),
	CHAR_TO_INT( 't', 'i', 'o' ),
	CHAR_TO_INT( 'n', '.', 's' ),
	CHAR_TO_INT( 'y', 's', '\0' )
};
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
    int iFileAttribute;

    iNumRecords = 8;

	// Do a little error checking
	if ( (iNumRecords != 8) && !pActivationBuf )
		return -1;

    //Change to activation folder directory.  It may already exist if 
    //user has previously activated the account.  
    if( Chdir((_packed char *)ActivationFolder) != FS_SUCCESS )
    {
        //If there was an error, the folder may not exist.  Try to create it.  If 
        //it fails, something is wrong so return an error.
        if (Mkdir ((_packed char *)ActivationFolder) != FS_SUCCESS)
            return -2; 
    } 
	// Activation records are always stored in file on internal Flash
	fp = FSFileCreate( (_packed BYTE *) ActivationFilename, 0 );
	if ( fp < 0 ) {
		return -2;
	}

	// Write all the activation records at once
	bytesToWrite = 560;
	if ( FSFileWrite( bytesToWrite, fp, MEM_SPACE_Y, -1, (WORD *) pActivationBuf ) != bytesToWrite ) {
		return -3;
	}
    //Set the hidden attribute so the activation data will not be added to the database.
    iFileAttribute = filegetattrib ((_packed BYTE *) ActivationFilename);
    filesetattrib (fp, iFileAttribute | ATTR_HIDDEN );

	// Close the file and make sure stuff is saved off to Flash
	FSFileClose( fp );
	FlushCache( );

	return 0;
}
