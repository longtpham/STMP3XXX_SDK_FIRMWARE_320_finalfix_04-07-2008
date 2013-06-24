///////////////////////////////////////////////////////////////////////////////
///
/// \file database.h 
/// \brief This is the header file used for accessing the SigmaTel database files.
///  
/// Copyright (c) SigmaTel, Inc. Unpublished
///
/// SigmaTel, Inc.
/// Proprietary  Confidential
///
/// This source code and the algorithms implemented therein constitute
/// confidential information and may comprise trade secrets of SigmaTel, Inc.
/// or its associates, and any use thereof is subject to the terms and
/// conditions of the Confidential Disclosure Agreement pursuant to which this
/// source code was originally received.
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _database_h_
#define _database_h_

//
// Define for an invalid handle.
//
#define DB_INVALID_HANDLE       0

// 
// The size of a record entry in the database.  This is currently two words
// long.
//
#define DB_RECORD_ENTRY_SIZE_BYTES      (6)
#define DB_STRING_SIZE_BYTES            (512)
#define DB_RECORD_SIZE_BYTES            (512)

//
// This macro should be used to allocate space in a record for a buffer.
// There is a slight overhead when allocating a buffer.  
//
#define RECORD_BUFFER_SIZE(ByteSize)     (ByteSize + 3)       

//
// All possible errors from the database.
//
#define DB_CREATED              (1)
#define DB_ERROR_NONE           (0)
#define DB_ERROR_CRITICAL       (-1)        //0xFFFFFF
#define DB_ERROR_MEDIA          (-2)        //0xFFFFFE 
#define DB_ALREADY_OPEN         (-3)        //0xFFFFFD
#define DB_OPEN_CORRUPT         (-4)        //0xFFFFFC
#define DB_ERROR                (-5)        //0xFFFFFB
#define DB_ERROR_NOT_OPEN       (-6)        //0xFFFFFA
#define DB_ERROR_INDEX          (-7)        //0xFFFFF9
#define DB_ERROR_OFFSET         (-8)        //0xFFFFF8
#define DB_ERROR_SIZE           (-9)        //0xFFFFF7
#define DB_ERROR_STRING         (-10)       //0xFFFFF6
#define DB_ERROR_LAST           (-255)      //0xFFFF01

// 
// This defines the maximum number of databases that can be open at one time. 
//
#define DB_MAX_DATABASE_STORES  2

INT _reentrant DBOpen(INT MediaIndex);
INT _reentrant DBClose(INT MediaIndex);
INT _reentrant DBClean(INT MediaIndex);
INT _reentrant DBGetNumRecords(INT MediaIndex, INT *pNumRecords);
INT _reentrant DBGetNumDeletedRecords(INT MediaIndex, INT *pNumDeletedRecords);
INT _reentrant DBNewRecord (INT MediaIndex, INT *pRecordIndex);
INT _reentrant DBDeleteRecord(INT RecordIndex);
//INT DBCopyRecord(INT SourceRecord, INT MediaIndex, INT *pDestRecord);
INT _reentrant DBGetString(INT RecordIndex, INT RecordOffset, INT *pSize, void *pStringAPIParams);
INT _reentrant DBSetString(INT RecordIndex, INT RecordOffset, INT Size, void *pStringAPIParams);
INT _reentrant DBGetNumber(INT RecordIndex, INT RecordOffset, INT Size, void *pBuffer);
INT _reentrant DBSetNumber(INT RecordIndex, INT RecordOffset, INT Size, void *pBuffer);
INT _reentrant DBGetBlock(INT RecordIndex, INT RecordOffset, INT *pSize, void *pBuffer);
INT _reentrant DBSetBlock(INT RecordIndex, INT RecordOffset, INT Size, void *pBuffer);

//
// External Definitions for SysCallFunction.
//
int _asmfunc exSysCallFunction_i(int);
int _asmfunc exSysCallFunction_ii(int, int);
int _asmfunc exSysCallFunction_iii(int, int, int);

//
// SysWait kernel Function.
//
_reentrant WORD SysWait(INT wTimeout);

//! 
//! typedef data structure
//!
//!
//! Structure to pass to DBSetString and DBGetString API.
//!
typedef struct _StringAPIParams {
    INT*    pBuffer;
    INT*    pOffsetInBuffer;
    INT     MemoryType;
} StringAPIParams;
//!
//! SysCall Macros for the Database functions.
//!
//!
//! This assembly trickery is to allow the resource number to be defined 
//! the library.
//! WARNING: Do not move any pointers into the SysCallFunction as that will
//! reuse the r0 and r4 pointers.
//!
int _inline SysCallDBOpen(int MediaIndex)               
{
__asm ("    extern  RSRC_DATABASE_OPEN          \n"     
       "    extern  FDBOpen                     \n"     
       "    move    #RSRC_DATABASE_OPEN,r0      \n"     
       "    move    #FDBOpen,r4                 \n");
    return exSysCallFunction_i(MediaIndex);
}

//!
//! 
//!
int _inline SysCallDBClean(int MediaIndex)               
{
__asm ("    extern  RSRC_DATABASE_OPEN          \n"     
       "    extern  FDBClean                    \n"     
       "    move    #RSRC_DATABASE_OPEN,r0      \n"     
       "    move    #FDBClean,r4                \n");
    return exSysCallFunction_i(MediaIndex);
}

//!
//! 
//!
int _inline SysCallDBClose(int MediaIndex)               
{
__asm ("    extern  RSRC_DATABASE_CLOSE         \n"     
       "    extern  FDBClose                    \n"     
       "    move    #RSRC_DATABASE_CLOSE,r0     \n"     
       "    move    #FDBClose,r4                \n");
    return exSysCallFunction_i(MediaIndex);
}

//!
//! 
//!
int _inline SysCallDBNewRecord(int MediaIndex, void * pRecord)               
{                                                       
__asm ("    extern RSRC_DATABASE_ACCESS         \n"     
       "    extern FDBNewRecord                 \n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_DATABASE_ACCESS,r0    \n"     
       "    move    #FDBNewRecord,r4" : : "A"(pRecord));                     
    return exSysCallFunction_i(MediaIndex);
}    

//!
//! 
//!
int _inline SysCallDBGetNumRecords(int MediaIndex, void * pNumRecords)
{                                                       
__asm ("    extern RSRC_DATABASE_ACCESS         \n"     
       "    extern FDBGetNumRecords             \n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_DATABASE_ACCESS,r0    \n"     
       "    move    #FDBGetNumRecords,r4" : : "A"(pNumRecords));                     
    return exSysCallFunction_i(MediaIndex);
}    

//!
//! 
//!
int _inline SysCallDBGetNumDeletedRecords(int MediaIndex, void * pNumDeletedRecords)
{                                                       
__asm ("    extern RSRC_DATABASE_ACCESS         \n"     
       "    extern FDBGetNumDeletedRecords      \n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_DATABASE_ACCESS,r0    \n"     
       "    move    #FDBGetNumDeletedRecords,r4" : : "A"(pNumDeletedRecords));                     
    return exSysCallFunction_i(MediaIndex);
} 

//!
//! 
//!
int _inline SysCallDBDeleteRecord(int RecordIndex)
{                                                       
__asm ("    extern RSRC_DATABASE_DELETE         \n"     
       "    extern FDBDeleteRecord              \n"     
       "    move    #RSRC_DATABASE_DELETE,r0    \n"     
       "    move    #FDBDeleteRecord,r4         \n");
    return exSysCallFunction_i(RecordIndex);
} 

//!
//! 
//!
int _inline SysCallDBSetString(int RecordIndex, int RecordOffset, int Size, void * pStringAPIParams)
{                                                       
__asm ("    extern RSRC_DATABASE_ACCESS         \n"     
       "    extern FDBSetString                 \n"     
       "    move    %0,r1                       \n"
       "    move    #RSRC_DATABASE_ACCESS,r0    \n"     
       "    move    #FDBSetString,r4" : : "A"(pStringAPIParams));                     
    return exSysCallFunction_iii(RecordIndex, RecordOffset, Size);
} 

//!
//! 
//!
int _inline SysCallDBSetNumber(int RecordIndex, int RecordOffset, int Size, void * pBuffer)
{                                                       
__asm ("    extern RSRC_DATABASE_ACCESS         \n"     
       "    extern FDBSetNumber                 \n"     
       "    move    %0,r1                       \n"                     
       "    move    #RSRC_DATABASE_ACCESS,r0    \n"     
       "    move    #FDBSetNumber,r4": : "A"(pBuffer));
    return exSysCallFunction_iii(RecordIndex, RecordOffset, Size);
} 

//!
//! 
//!
int _inline SysCallDBGetBlock(int RecordIndex, int RecordOffset, int * pSize, void * pBuffer)
{                                                       
__asm ("    extern RSRC_DATABASE_ACCESS         \n"     
       "    extern FDBGetBlock                  \n"     
       "    move    %0,r1                       \n"
       "    move    %1,r5                       \n"
       "    move    #RSRC_DATABASE_ACCESS,r0    \n"     
       "    move    #FDBGetBlock,r4" : : "A"(pSize), "A"(pBuffer));                     
    return exSysCallFunction_ii(RecordIndex, RecordOffset);
} 

//!
//! 
//!
int _inline SysCallDBSetBlock(int RecordIndex, int RecordOffset, int Size, void * pBuffer)
{                                                       
__asm ("    extern RSRC_DATABASE_ACCESS         \n"     
       "    extern FDBSetBlock                  \n"     
       "    move    %0,r1                       \n"     
       "    move    #FDBSetBlock,r4             \n"
       "    move    #RSRC_DATABASE_ACCESS,r0": : "A"(pBuffer));                     
    return exSysCallFunction_iii(RecordIndex, RecordOffset, Size);
} 

#define SysCallDBGetString(RecordIndex, RecordOffset, pSize, pStringAPIParams) DBGetString( RecordIndex, RecordOffset, pSize, (void*)pStringAPIParams)
#define SysCallDBGetNumber(RecordIndex, RecordOffset, Size, pBuffer) DBGetNumber( RecordIndex, RecordOffset, Size, (void *)pBuffer)

extern volatile INT g_DB_Dirty;
#endif // _database_h_
