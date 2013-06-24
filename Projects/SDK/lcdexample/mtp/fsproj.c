/*----------------------------------------------------------------------------
 SigmaTel Inc
 $Archive: /Fatfs/FileSystem/Fat32/Fsproj.c $              
 $Revision: 17 $                                       
 $Date: 9/13/03 12:23p $      
 Description: Fsproj.c
 Notes:
----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
#include "fstypes.h"
#include "fsproj.h"
//------------------------------------------------------------------------------
INT maxhandles = NUMHANDLES;
INT maxdevices = NUMDEVICES;
INT maxcaches  = NUMCACHES; 
INT DriveLetter[] = DRIVELETTERS;
// The cache buffers are allocated in the X memory.
int _X bufx[XBUF_SIZE];
// All other global, custom configurable data is in Y memory.
int _Y bufy[YBUF_SIZE];
//---eof------------------------------------------------------------------------

