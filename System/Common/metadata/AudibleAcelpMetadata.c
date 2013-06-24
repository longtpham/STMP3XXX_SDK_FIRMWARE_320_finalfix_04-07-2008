///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: mp3metadata.c
// Description: Functions for extracting the mp3 meta-data.
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "resource.h"
#include "metadata.h"

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetAsfMetaData
//
//   Type:          Function
//
//   Description:   Finds the meta-data within a wma file.
//
//   Inputs:        Ptr to pathname, current device id, ptr to meta-data struct.
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

RETCODE _reentrant GetAudibleAcelpMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
    RETCODE rtn = META_DATA_FOUND;
    MetaData->iFileType = (INT)AUDIBLE_ACELP_TYPE;
    MetaData->iResourceNum = RSRC_AUDIBLE_ACELPDEC_CODE;

	// 10/10/2004 mmiu - The file size needs to be set here in order to rewind
	// across song boundaries. However, we can't actually read the metadata
	// without bringing in a lot of Audible routines. So we set a fake file
	// size that can't possibly be real to use as a message to fill in with the
	// real data when we get it. Actually, the variables we're *really*
	// interested in are SongByteTotalHigh, SongByteTotalLow, SongByteTotalConsumedHigh,
	// SongByteTotalConsumedLow, SongByteLengthHigh, and g_wSongByteLengthLow
	MetaData->dwNumBytes = 0x0FEDCBA98765;

    rtn = META_DATA_FOUND;
    return rtn;
}
