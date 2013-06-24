///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: mtp_audiblemetadata.c
// Description: Functions for extracting the audible meta-data.
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "aamtpaccess.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetAudibleMetaData
//
//   Type:          Function
//
//   Description:   Finds the meta-data within a audible file.
//
//   Inputs:        Ptr to pathname, current device id, ptr to meta-data struct.
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant GetAudibleMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
    RETCODE rtn = META_DATA_NOT_FOUND;
    INT error = 0;
    INT iHandle = NOERROR;                          // indicates the handle of file opened (or error if failure)
    DWORD dwNumBytesTemp;                           // stores 48 bits, number of bytes in file
    _packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;

    // Open the file.
    iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);

    if(iHandle != ERROR)
    {
        // Get the number of bytes in song
        dwNumBytesTemp = FSFileSize(iHandle);
        MetaData->dwNumBytes = dwNumBytesTemp;
    
        // Clear MetaData members that do not apply
        MetaData->dwNumFrames = (DWORD)0;
        MetaData->bVBR = (BOOL)0;
        MetaData->wBitRate = (WORD)0;
        
        if ((FSFileClose(iHandle)) == ERROR)
        {
           rtn = META_DATA_FILECLOSE_ERROR;
        }
        else
        {
            //Get the audible specific metadata.  Add more fields if needed.
            error = AAMTPGetTitle((INT)pcFilePathName, btCurrentDevice, (char *)MetaData->wTitle);
            error = AAMTPGetAuthor((INT)pcFilePathName, btCurrentDevice, (char *)MetaData->wArtist);
            error = AAMTPGetDuration((INT)pcFilePathName, btCurrentDevice, (int *)&MetaData->iTime);
            rtn = META_DATA_FOUND;
        }
    }
    else
    {
      rtn = META_DATA_FILEOPEN_ERROR;
    }
    return(rtn);
}

