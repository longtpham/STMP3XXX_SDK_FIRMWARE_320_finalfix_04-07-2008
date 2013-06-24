//=============================================================================
//
// DESCRIPTION:
//
// Copyright (c) SigmaTel, Inc. Unpublished
// 
// SigmaTel, Inc.
// Proprietary  Confidential
// 
// This source code and the algorithms implemented therein constitute 
// confidential information and may comprise trade secrets of SigmaTel, Inc. 
// or its associates, and any unauthorized use thereof is prohibited.
//
//=============================================================================
#include <fstypes.h>

//!
//! The first record in the database has stores the state information for the 
//! database.
//!
typedef struct _DBTopRecord
{
    INT         StorageID;
    INT         BlockSize;
    INT         RecordEntrySize;
    INT         TotalRecords;
    INT         TotalDelRecords;
    INT         TotalStrings;
    INT         TotalDelStrings;
    
    //!
    //! The file offset in store.idx to the last valid position in the store.idx
    //! file.
    INT         LastDelRecordIdx;
    
    //!
    //! The file offset in store.idx to get the next record to reuse.
    INT         GetDelRecordIdx;

    //!
    //! The file offset in store.idx to put the next record into for later
    //! reuse.
    INT         PutDelRecordIdx;
} DBTopRecord;

//
// This structure is used to hold the state information for each database
// that is active in the system.
//
typedef struct _DBState
{
    // The file handle to the ststore.dat for this database.
    INT     hStStore_Dat;

    // The file handle to the ststore.idx file for this database.
    INT     hStStore_Idx;

    // The file handle to the ststring.dat file for this database.
    INT     hStString_Dat;

    // The file handle to the ststring.dat file for this database.
    INT     hStString_Idx;

    DBTopRecord TopRecord;
} DBState;

//
// This is the entry as allocated in the database file for strings.
//
typedef struct StringEntry_
{
    INT     Size;
    INT     Index;
} StringEntry;

//
// This is the entry as allocated in the database file for nubmers.
//
typedef struct NumberEntry_
{
    INT     Tag;
    INT     Value;
} NumberEntry;

//
// Definitions used by the interals of the database.
//
#define DB_RECORD_SIZE_WORDS        (DB_RECORD_SIZE_BYTES/3)
#define DB_RECORD_ENTRY_SIZE_BYTES  (6)
#define DB_RECORD_MODULO            (512)

#define DB_TAG_STRING               (0x800000)
#define DB_TAG_NUMBER               (0x400000)
#define DB_TAG_BLOCK                (0x200000)
#define DB_TAG_MASK                 (0xffff00)

#define DB_TAG_STRING_SIZE_MASK     (0x0003ff)

#define DB_TAG_NUMBER_VALUE_MASK    (0x0000ff)

#define DB_TAG_BLOCK_SIZE_MASK      (0x0003ff)

#define DB_MEDIA_INDEX_EXTERNAL     (0x800000)
#define DB_MEDIA_INDEX_SHIFT        (23)

#define DB_INDEX_MASK               (0x003fff)
#define DB_COUNT_ADD                (0x004000)
#define DB_COUNT_MASK               (0x7fc000)

//!
//! Size of a database handle in bytes.
//!
#define DB_HANDLE_SIZE              (3)
#define DB_STRING_HANDLE_SIZE       (3)

#define DB_DEFAULT_SIZE_ST_STORE_DAT ((long)8*(long)1024*(long)1024)  //8MB
#define DB_DEFAULT_SIZE_ST_STRING_DAT ((long)1*(long)1024*(long)1024) //1MB
//! 
//! Flags for database entries.
//!
#define DB_ENTRY_DIRTY              (1)

//!
//! Defines which memory to use for the database buffer.
//!
#define DB_BUFFER_MEMORY            _X

//!
//! Defines which memory to use for the database buffer when calling through
//! the filesystem.
//!
#define DB_BUFFER_MEMORY_TYPE       X_MEMORY

//
// Defined in database.c
//
extern DBState g_DBState[DB_MAX_DATABASE_STORES];
extern DB_BUFFER_MEMORY INT g_DB_Buffer[DB_RECORD_SIZE_WORDS+1];
extern INT g_DB_BufferIndex;
extern INT g_DB_BufferState;

//
// Prototypes for DB functions available to all internal database overlays.
//
extern INT _reentrant SeekDB(INT hFile, LONG lPos);
extern INT _reentrant BufferFlush(void);
extern INT _reentrant GetMediaIndex(INT DBIndex);
extern INT _reentrant WriteDatabaseRecord(INT RecordIndex, INT RecordOffset, INT Size, 
    void *pBuffer);
extern INT _reentrant ReadDatabaseRecord(INT RecordIndex, INT RecordOffset, INT Size, 
    void *pBuffer);
extern INT _reentrant BufferUpdate(INT RecordIndex);
