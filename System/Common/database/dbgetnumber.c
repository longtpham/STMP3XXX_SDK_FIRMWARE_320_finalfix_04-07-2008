///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbgetnumber.c
//! \brief
//!  The implementation of the DBGetNumber database API.
//!
//! Copyright (c) SigmaTel, Inc. Unpublished
//!
//! SigmaTel, Inc.
//! Proprietary  Confidential
//!
//! This source code and the algorithms implemented therein constitute
//! confidential information and may comprise trade secrets of SigmaTel, Inc.
//! or its associates, and any use thereof is subject to the terms and
//! conditions of the Confidential Disclosure Agreement pursuant to which this
//! source code was originally received.
//!
///////////////////////////////////////////////////////////////////////////////
#include <types.h>
#include <filesystem.h>
#include <fsapi.h>
#include <fstypes.h>
#include <fserror.h>
#include <database.h>
#include <database_internal.h>

//=============================================================================
//  FUNCTION: DBGetNumber
// ----------------------------------------------------------------------------
//! This API is used to return a number value from the record referenced by 
//! RecordIndex.  The actual location that is set in the record is based on the
//! offset indicated by the RecordOffset parameter.  The Size parameter
//! indicates the type number that will be returned.   
//! \verbatim
//! List of supported sizes and their corresponding type:
//!  Size (bytes)    Type
//!  1               DB_UCHAR8
//!  2               DB_USHORT16
//!  3               DB_UINT24
//!  4               DB_UINT32
//!  6               DB_ULONG48
//! \endverbatim
//! 
//! \param RecordIndex
//!      The index of the record that the caller wants to access.
//! \param RecordOffset
//!      The offset (in bytes) into the record that the caller wants to read.
//! \param Size
//!      The size (in bytes) of the array pointed to by pBuffer it also indicates
//!      the type of the number being retrieved.
//! \param pBuffer
//!      The buffer that the number will be copied into.
//!
//! \retval DB_ERROR_NONE
//!      The number was read from the database.
//! \retval DB_ERROR_OFFSET
//!      Invalid record offset passed in.
//! \retval DB_ERROR_SIZE
//!      Invalid number size was passed in.
//! \retval DB_ERROR_HANDLE
//!      Invalid handle passed in.
//
//=============================================================================
INT _reentrant DBGetNumber(INT RecordIndex, INT RecordOffset, INT Size, void *pBuffer)
{
    INT Buffer[2];
    INT RetVal;
    INT *pTemp;

    if (Size > 4)
    {
        return(DB_ERROR_SIZE);
    }
    
    pTemp = (INT *)pBuffer;
    
    //
    // Read this out of the file.
    //
    RetVal = ReadDatabaseRecord(RecordIndex, RecordOffset, 
                                DB_RECORD_ENTRY_SIZE_BYTES, Buffer);
    
    *pTemp++ = Buffer[1];
    
    if (Size > 3)
    {
        *pTemp = Buffer[0] & DB_TAG_NUMBER_VALUE_MASK;
    }
    
    return(RetVal);
}
