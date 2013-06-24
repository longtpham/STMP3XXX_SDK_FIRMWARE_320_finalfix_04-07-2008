///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbopen.c
//! \brief
//!  The implementation of the DBOpen and DBClean database APIs.
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
#include "devicetable.h"

extern INT DriveLetter[];

//!
//! a:/ststore.dat
//!
static _packed char ststore_dat[]="/:ASTSROTD.E\0TA";
//!
//! a:/ststore.idx
//!
static _packed char ststore_idx[]="/:ASTSROTI.E\0XD";
//!
//! a:/ststring.dat
//!
static _packed char ststring_dat[]="/:ASTSIRT.GNTAD";
//!
//! a:/ststring.idx
//!
static _packed char ststring_idx[]="/:ASTSIRT.GNXDI";

static _reentrant INT OpenDatabaseFiles(INT MediaIndex);
static _reentrant void SetDatabaseFileNames(INT MediaIndex);
static _reentrant INT OpenDatabaseFile(_packed char Filename[], INT *pFileHandle);
static _reentrant INT GrowDatabaseFile(INT DBFileHandle, LONG SizeToGrow);

//!
//! extern declarations.
//!
extern FSMEDIA_TABLE *      MediaTable; //declared in filesystem code

//=============================================================================
//  FUNCTION: DBOpen
// ----------------------------------------------------------------------------
//! This API is used to open and initialize the database indicated by the 
//! MediaIndex parameter before it is used in the system.  This call should be 
//! made once when starting up the system and not again until a call to DBClose()
//! has been made.   
//! 
//! \param MediaIndex
//!     This indicates which database to open.
//!
//! \retval DB_ERROR_NONE          
//!      The database was opened successfully.
//! \retval DB_ERROR_MEDIA
//!      The media database number was not valid.
//! \retval DB_ALREADY_OPEN
//!      The media database number was already opened.
//! \retval DB_OPEN_CORRUPT
//!      The media database number was corrupt and needs to be recreated.
//!
//=============================================================================
INT _reentrant DBOpen(INT MediaIndex)
{
    INT iTemp;
    INT NewDB;
    INT i;
    
    //
    // Make sure that the index is correct.
    //
    if (MediaIndex >= DB_MAX_DATABASE_STORES)
    {
        return(DB_ERROR_MEDIA);
    }

    //
    // Make sure that the database has not already been opened so that we don't
    // lose information just because someone forgot that they already opened 
    // the database.
    //
    if (0 != g_DBState[MediaIndex].hStStore_Dat)
    {
        return(DB_ALREADY_OPEN);
    }
    
    // 
    // If the database could not be opened then try to recreated it.
    //
    NewDB = OpenDatabaseFiles(MediaIndex);
    if (NewDB < 0)
    {
        return(DB_OPEN_CORRUPT);
    }
    
    Fseek(g_DBState[MediaIndex].hStStore_Dat, 0, SEEK_SET);

    iTemp = Fread(g_DBState[MediaIndex].hStStore_Dat, 
                 (INT *)&(g_DBState[MediaIndex].TopRecord), 
                 sizeof(DBTopRecord) * 3, 
                 DEFAULT_MEMORY,-1);
            
    //
    // This entry does not yet exist so it needs to be written to the file.
    //                 
    if (iTemp == 0)
    {
        //
        // Set the defaults.
        //
        g_DBState[MediaIndex].TopRecord.BlockSize = DB_RECORD_SIZE_BYTES;
        g_DBState[MediaIndex].TopRecord.RecordEntrySize = DB_RECORD_SIZE_BYTES;
        g_DBState[MediaIndex].TopRecord.StorageID = 0;
        g_DBState[MediaIndex].TopRecord.TotalRecords = 0;
        g_DBState[MediaIndex].TopRecord.TotalStrings = 0;
        g_DBState[MediaIndex].TopRecord.TotalDelStrings = 0;
        g_DBState[MediaIndex].TopRecord.TotalDelRecords = 0;
        g_DBState[MediaIndex].TopRecord.LastDelRecordIdx = 0;
        g_DBState[MediaIndex].TopRecord.GetDelRecordIdx = 0;
        g_DBState[MediaIndex].TopRecord.PutDelRecordIdx = 0;

        //
        // Seek back to the beginning to write out the first record.
        //
        if (SUCCESS != Fseek(g_DBState[MediaIndex].hStStore_Dat, 
                            0, SEEK_SET))
        {
            DebugBuildAssert(0);
        }
        
        //
        // Indicate that someone has touched the database.
        //
        g_DB_Dirty = TRUE;

        //
        // Write the buffer out to the storage media, this will write some 
        // uninitialized data out to the file as well but will be unused.
        //
        if (DB_RECORD_SIZE_BYTES != 
                Fwrite(g_DBState[MediaIndex].hStStore_Dat, 
                    (INT *)&g_DBState[MediaIndex].TopRecord, 
                    DB_RECORD_SIZE_BYTES, 
                    DEFAULT_MEMORY, -1))
        {
            DebugBuildAssert(0);
        }
    }
    //
    // Could just reset everything at this point.
    //
    else if (iTemp != (sizeof(DBTopRecord) * 3))
    {
        return(DB_OPEN_CORRUPT);
    }
    //
    // if totalstrings are 0 then no need to consider ststring.idx entries when 
    // allocating new strings.
    //
    else if( g_DBState[MediaIndex].TopRecord.TotalStrings == 0 )
    {
        //
        // this is an empty database so empty string table and string index
        // table
        //
        if (g_DBState[MediaIndex].TopRecord.TotalDelStrings != 0)
        {
            g_DBState[MediaIndex].TopRecord.TotalDelStrings = 0;

            //
            // Indicate that someone has touched the database.
            //
            g_DB_Dirty = TRUE;
        }
    }

    //
    // Mark the buffered data invalid.
    //    
    g_DB_BufferIndex = -1;
    g_DB_BufferState = 0;
    
    return(NewDB);
}

//=============================================================================
//  FUNCTION: DBClean
// ----------------------------------------------------------------------------
//! This API is used to remove all records from the database based on MediaIndex
//! 
//! \param MediaIndex
//!      This indicates which media database to clear out.
//!
//! \retval DB_ERROR_NONE
//!      All records were removed from the database.
//! \retval DB_ERROR_MEDIA
//!      The media database number was not valid.
//!
//=============================================================================
INT _reentrant DBClean(INT MediaIndex)
{
    INT RetCode = DB_ERROR_NONE;

    //
    // Make sure that the index is correct.
    //
    if (MediaIndex >= DB_MAX_DATABASE_STORES)
    {
        return(DB_ERROR_MEDIA);
    }

    //
    // Set the names to match the drive that we are on.
    //
    SetDatabaseFileNames(MediaIndex);
    
    //
    // This hacky delete is to be sure that the file system removes the file
    // without an unexpected first error.  The file system seems to report an
    // error when removing a file 
    //
    // TODO look at why I get this error (INVALID_CLUSTER_NO) if the idx file
    // exists.
    //
    Fremove(ststore_dat);
    if (Fremove(ststore_dat) != FILE_NOT_FOUND)
    {
        RetCode = DB_ERROR_CRITICAL;
    }
    
    Fremove(ststore_idx);
    if (Fremove(ststore_idx) != FILE_NOT_FOUND)
    {
        RetCode = DB_ERROR_CRITICAL;
    }
    
    Fremove(ststring_dat);
    if (Fremove(ststring_dat) != FILE_NOT_FOUND)
    {
        RetCode = DB_ERROR_CRITICAL;
    }

    Fremove(ststring_idx);
    if (Fremove(ststring_idx) != FILE_NOT_FOUND)
    {
        RetCode = DB_ERROR_CRITICAL;
    }

    return(RetCode);
}

//=============================================================================
// FUNCTION: OpenDatabaseFiles
// ----------------------------------------------------------------------------
//! This function opens all of the database file for read/write/append mode.
//!
//! \param MediaIndex 
//!      The media index to media that contains the database to be opened.
//!
//! \retval DB_ERROR_CRITICAL
//!      Could not open all database files.
//! \retval DB_CREATED
//!      Database files created a new.
//! \retval DB_ERROR_NONE
//!      Database files are opened successfully for read/write access. 
//!
//=============================================================================
static _reentrant INT OpenDatabaseFiles(INT MediaIndex)
{
    INT RetCode; 
    LONG AvailableSpace;

    //
    // Set the names to match the drive that we are on.
    //
    SetDatabaseFileNames(MediaIndex);

    //
    // we are going to yield to kernel to update the display if we are creating
    // new db. To avoid any other task access db while control is with 
    // kernel, we are creating ststring.dat first followed by ststore.dat
    //
    RetCode = OpenDatabaseFile(ststring_dat,
                                &g_DBState[MediaIndex].hStString_Dat);
    if (RetCode < 0)
    {
        return(DB_ERROR_CRITICAL);
    }
    //
    // if ststring.dat file is newly created then reserve 
    // DB_DEFAULT_SIZE_ST_STRING_DAT for it. This is to avoid fragmentation of 
    // string table file.
    //
    
    if (0==MediaIndex)
    {
        //
        // We are not doing this for external media (MMC) because writes are too
        // slow to do this here. It was causing some host enumeration problems.
        // So reserving 8MB for ststore.dat and 1MB for ststring.dat is 
        // exclusively for NAND and not for external media.
        // 
        if( RetCode == DB_CREATED )
        {   
            //
            // Indicate that someone has touched the database.
            //
            g_DB_Dirty = TRUE;
            
            SysWait(500); // this should update the display.

            // Only reserve space if the predefined amount of space is available
            if((AvailableSpace = (LONG)FSFreeClusters(MediaIndex)*(LONG)BytesPerCluster(MediaIndex)) > DB_DEFAULT_SIZE_ST_STRING_DAT)
            {
                if(DB_ERROR_NONE != GrowDatabaseFile(g_DBState[MediaIndex].hStString_Dat, 
                    DB_DEFAULT_SIZE_ST_STRING_DAT))
                {
                    return ( DB_ERROR_CRITICAL );
                }
            }
        }
    }

    RetCode |= OpenDatabaseFile(ststring_idx,
                                &g_DBState[MediaIndex].hStString_Idx);
    if (RetCode < 0)
    {
        return(DB_ERROR_CRITICAL);
    }

    RetCode |= OpenDatabaseFile(ststore_dat, 
                               &g_DBState[MediaIndex].hStStore_Dat);
    if (RetCode < 0)
    {
        return(DB_ERROR_CRITICAL);
    }
    
    //
    // if ststore.dat file is newly created then reserve 
    // DB_DEFAULT_SIZE_ST_STORE_DAT for it. This is to avoid fragmentation of 
    // store database file.
    //
    
    if (0==MediaIndex)
    {
        //
        // We are not doing this for external media (MMC) because writes are too
        // slow to do this here. It was causing some host enumeration problems.
        // So reserving 8MB for ststore.dat and 1MB for ststring.dat is 
        // exclusively for NAND and not for external media.
        // 
        if( RetCode == DB_CREATED )
        {   
            // Only reserve space if the predefined amount of space is available
            if((AvailableSpace = (LONG)FSFreeClusters(MediaIndex)*(LONG)BytesPerCluster(MediaIndex)) > DB_DEFAULT_SIZE_ST_STORE_DAT)
            {
                if( DB_ERROR_NONE != GrowDatabaseFile(g_DBState[MediaIndex].hStStore_Dat, 
                    DB_DEFAULT_SIZE_ST_STORE_DAT) )
                {
                    return(DB_ERROR_CRITICAL);
                }
            }
        }
    }

    RetCode |= OpenDatabaseFile(ststore_idx, 
                                &g_DBState[MediaIndex].hStStore_Idx);
    
    return(RetCode);
}

//=============================================================================
//  FUNCTION: GrowDatabaseFile
// ----------------------------------------------------------------------------
//! This will call fwrite command with buffer set to zeroes until the size of 
//! database file grows to the required size.
//!
//! \param DBFileHandle
//!     file handle of database table.
//! \param SizeToGrow
//!     file handle of database table.
//!
//! \retval DB_ERROR_NONE
//!     The file has been successfully grown to the required size.
//! \retval DB_ERROR_CRITICAL
//!     A serious error occured.
//!
//=============================================================================
static _reentrant INT GrowDatabaseFile(INT DBFileHandle, LONG SizeToGrow)
{
    LONG loop_count;

    //
    // fill the buffer with FFs.
    // well, we found that initializing the file with FFs created some 
    // problems so initializing with 0s for now.
    //
    for( loop_count=0; loop_count<DB_RECORD_SIZE_WORDS; loop_count++ )
    {
        g_DB_Buffer[loop_count]=(INT)0x000000;
    }
    
    //
    // write the buffer until the size of string table becomes 
    // DB_DEFAULT_SIZE_ST_STRING_DAT(5MB).
    //
    loop_count = SizeToGrow/DB_RECORD_SIZE_BYTES; 
    while( loop_count )
    {
        if (DB_RECORD_SIZE_BYTES != 
                Fwrite(DBFileHandle, 
                    (INT*)g_DB_Buffer, 
                    DB_RECORD_SIZE_BYTES, 
                    DB_BUFFER_MEMORY_TYPE, -1))
        {
//                DebugBuildAssert(0);
            return (DB_ERROR_CRITICAL);
        }
        loop_count --;
    }
    return DB_ERROR_NONE;
}

//=============================================================================
//  FUNCTION: OpenDatabaseFile
// ----------------------------------------------------------------------------
//! This just opens a database file for reading and writing.  There needed to 
//! be a special case for the file not existing so this is done as a call.
//!
//! \param Filename
//!     The premunged filename to open.
//!
//! \retval DB_ERROR_NONE
//!     The file was opened successfully.
//! \retval DB_CREATED
//!     The file was created successfully as it was not present.
//! \retval FSErrorCode 
//!     The file system error code indicating why the file could not be opened.
//!
//=============================================================================
static _reentrant INT OpenDatabaseFile(_packed char Filename[], INT *pFileHandle)
{
    INT RetCode;
    INT iAttrib;

    RetCode = Fopen(Filename,(_packed char *)"r+");

    if (RetCode == FILE_NOT_FOUND)
    {
        RetCode = Fopen(Filename,(_packed char *)"w+");

        if (RetCode >= 0)
        {
            *pFileHandle = RetCode;

            // Give the file Hidden attributes so it does
            // not show up in the data folder in Explorer.
            iAttrib = filegetattrib ((_packed char *)Filename);
            filesetattrib (*pFileHandle, iAttrib | ATTR_HIDDEN); 

            RetCode = DB_CREATED;
        }
    }
    else
    {
        if (RetCode >= 0)
        {
            *pFileHandle = RetCode;
            RetCode = DB_ERROR_NONE;
        }
    }
    return(RetCode);
}

//=============================================================================
//  FUNCTION: SetDatabaseFileNames
// ----------------------------------------------------------------------------
//! This function just replaces the drive letter for all filename strings.
//!
//! \param MediaIndex
//!     This is the drive number to set the drive letter to.
//! \retval void
//!
//=============================================================================
static _reentrant void SetDatabaseFileNames(INT MediaIndex)
{
    ststore_dat[0] = (ststore_dat[0] & 0xffff00) | DriveLetter[MediaIndex]; 
    ststore_idx[0] = (ststore_idx[0] & 0xffff00) | DriveLetter[MediaIndex]; 
    ststring_dat[0] = (ststring_dat[0] & 0xffff00) | DriveLetter[MediaIndex]; 
    ststring_idx[0] = (ststring_idx[0] & 0xffff00) | DriveLetter[MediaIndex]; 
}
