////////////////////////////////////////////////////////////////////////////////
///
/// \file StStore.h
/// \brief This is a header for ST-Store API, functions, enums, defines, typedefs and data structures definitions.
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
////////////////////////////////////////////////////////////////////////////////
#ifndef __STSTORE_H_
#define __STSTORE_H_

#include <metadata.h>
#include <mtphandler.h>
#include <kernel.h>

//////////////////////////////////////////////////////////////////
//! Defines
//////////////////////////////////////////////////////////////////
#if (NUM_REMOVABLE_MEDIA == 1)
#define MAX_NUM_STORES                  (2) 
#else
#define MAX_NUM_STORES                  (1) 
#endif
//
// Defined by project as the number of stores in the system.  This can be a max
// of MAX_NUM_STORES.
//
extern const int g_NumStores;                       
#define MAX_STRING_LENGTH               (255)

#define DBFieldOffset_StorageID                                     (0)
#define DBFieldOffset_FormatCode                                    (DBFieldOffset_StorageID + 6)
#define DBFieldOffset_ProtectionStatus                              (DBFieldOffset_FormatCode + 6)
#define DBFieldOffset_ObjectSize                                    (DBFieldOffset_ProtectionStatus + 6)
#define DBFieldOffset_AssocType                                     (DBFieldOffset_ObjectSize + 6)
#define DBFieldOffset_Filename                                      (DBFieldOffset_AssocType + 6)
#define DBFieldOffset_DateCreated                                   (DBFieldOffset_Filename + 6)
#define DBFieldOffset_ParentObject                                  (DBFieldOffset_DateCreated + 6)
#define DBFieldOffset_Artist                                        (DBFieldOffset_ParentObject + 6)
#define DBFieldOffset_NonConsumable                                 (DBFieldOffset_Artist + 6)
#define DBFieldOffset_Duration                                      (DBFieldOffset_NonConsumable + 6)
#define DBFieldOffset_AlbumName                                     (DBFieldOffset_Duration + 6)
#define DBFieldOffset_BitrateType                                   (DBFieldOffset_AlbumName + 6)
#define DBFieldOffset_SampleRate                                    (DBFieldOffset_BitrateType + 6)
#define DBFieldOffset_NumChannels                                   (DBFieldOffset_SampleRate + 6)
#define DBFieldOffset_AudioWaveCodec                                (DBFieldOffset_NumChannels + 6)
#define DBFieldOffset_AudioBitrate                                  (DBFieldOffset_AudioWaveCodec + 6)
#define DBFieldOffset_ObjectHandle                                  (DBFieldOffset_AudioBitrate + 6)
#define DBFieldOffset_Title                                         (DBFieldOffset_ObjectHandle + 6)
#define DBFieldOffset_ShortFilename                                 (DBFieldOffset_Title + 6)
#define DBFieldOffset_FastFileHandle                                (DBFieldOffset_ShortFilename + 6)
#define DBFieldOffset_PathChecksum                                  (DBFieldOffset_FastFileHandle + 9)
#define DBFieldOffset_FilenameChecksum                              (DBFieldOffset_PathChecksum + 6)
#define DBFieldOffset_RefreshNeeded                                 (DBFieldOffset_FilenameChecksum + 6) 

#define DB_UINT8_SIZE           (1)
#define DB_UINT16_SIZE          (2)
#define DB_UINT32_SIZE          (4)
#define DB_UINT48_SIZE          (6)
#define DB_UINT64_SIZE          (8)
#define DB_UINT128_SIZE          (16)
#define DB_STRING_SIZE          (128)
#define DB_SHORTFILENAME_SIZE   (13)

#define RECORD_HANDLE_SIZE_IN_BYTES                               (3)
#define FILE_INDEX_READ_BUFFER_SIZE                             (512)
#define HANDLE_ARRAY_SIZE                                       (100)

#define MAX_STRING_SIZE_IN_STOREINFO                             (30)

#define MAX_DELETED_RECORDS_IN_DATABASE                         (10000)

//////////////////////////////////////////////////////////////////
//! Enums & Typedefs
//////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// \enum EnumStorStatus
///
/// defines all possible values for store status
///
typedef enum EnumStorStatus {
    Dirty_NONE = 0,
    Dirty_FRIT = 1,    
    Dirty_OHIT = 2,    
    Dirty_BOTH = 3
} EnStorStatus;

////////////////////////////////////////////////////////////////////////////////
/// \enum EnumRecordStatus 
///
/// defines all possible values for recordstatus field in database
///
typedef enum EnumRecordStatus {
    Valid   = 0,
    Deleted = 1,
    Moved   = 2,
    Dirty   = 3,
    Invalid = 4
} EnRecordStatus;

////////////////////////////////////////////////////////////////////////////////
/// \enum EnumAccessType 
///
/// defines all possible values for access type
///
typedef enum EnumAccessType {
    RWD = 0,    //!< read, write and delete access
    RO  = 1,    //!< read only access
    RD  = 2     //!< read and delete access
} EnAccessType;

////////////////////////////////////////////////////////////////////////////////
/// \enum EnumFileSystemType 
///
/// defines all possible values for filesystem 
///
typedef enum EnumFileSystemType {//!< as defined in MTP Spec
    FSUndefined,        
    Generic_Flat,
    Generic_Hierarchical,   
    DCF,
    Reserved,
    MTP_Vendor_Extension,
    MTP_defined
} EnFileSystemType; 

////////////////////////////////////////////////////////////////////////////////
/// \enum EnumStoreType 
///
/// defines all possible values for store type 
///
typedef enum EnumStoreType {//!< as defined in MTP Spec
    ST_Undefined,
    Fixed_ROM,
    Removable_ROM,
    Fixed_RAM,
    Removable_RAM,
    ST_Reserved
} EnStoreType;

////////////////////////////////////////////////////////////////////////////////
/// \enum EnumGetSet 
///
/// defines EnumGetSet used in Store Object functions to get and set fields stored in database
///
typedef enum EnumGetSet {
    GetIt = 0,
    SetIt = 1
} EnGetSet;

////////////////////////////////////////////////////////////////////////////////
/// \enum EnumNonConsumable 
///
/// defines all possible values for non-consumable field in database
///
typedef enum EnumNonConsumable {
    Consumable    = 0,    //!< for consumable 
    NonConsumable = 1	  //!< for non consumable 
} EnNonConsumable;


////////////////////////////////////////////////////////////////////////////////
/// \enum EnumAudioWaveCodec 
///
/// defines all possible values for AudioWaveCodec field in database
///
typedef enum EnumAudioWaveCodec {
    WaveFormatSiproLabACELPNET      = 0x130,    //!< for WAVE_FORMAT_SIPROLAB_ACELPNET
    WaveFormatMSAudio1              = 0x160,    //!< for WAVE_FORMAT_MSAUDIO1
    WaveFormatMSAudio2              = 0x161     //!< for WAVE_FORMAT_MSAUDIO2
} EnAudioWaveCodec;

////////////////////////////////////////////////////////////////////////////////
/// \enum EnumAudibleCodec
///
/// defines all possible values for the Audible Codec property
///
typedef enum EnumAudibleCodec {
    NonAudibleFormat = 0,     
    AudibleFormat1Raw = 1,
    AudibleFormat2Raw = 2,
    AudibleFormat3Raw = 3,
    AudibleFormat4Raw = 4,
    AudibleFormat1Packaged = 5,
    AudibleFormat2Packaged = 6,
    AudibleFormat3Packaged = 7,
    AudibleFormat4Packaged = 8	   
} EnumAudibleCodec;

////////////////////////////////////////////////////////////////////////////////
/// \enum EnumAudiblePlayedThrough
///
/// defines all possible values for the Audible Played Through property
///
typedef enum EnumAudiblePlayedThrough {
    NotPlayedThrough = 0,     
    PlayedThrough = 1   
} EnumAudiblePlayedThrough;

typedef SPtpString StString;

//////////////////////////////////////////////////////////////////
//! Data Structures
//////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// \struct _StStoreInfo 
///
/// This structure will be initialized at store enumeration time. This holds 
/// information about one store on the device. This structure will be referenced 
/// and updated by most of the functions and API
///
typedef struct _StStoreInfo {
    long            ID;                     //!< store number
    EnStoreType     Type;                   //!< one of EnumStoreType
    SHORT           FileSystemType;         //!< one of EnumFileSystemType
    SHORT           AccessType;             //!< one of EnumAccessType
    DWORD           MaxCapacity;            //!< media size in bytes
    DWORD           SpaceAvail;             //!< free space available on media
//    SHORT           Description[MAX_STRING_SIZE_IN_STOREINFO+1];        //!< a unicode string describing the store
//    SHORT           VolumeID[MAX_STRING_SIZE_IN_STOREINFO+1];           //!< a unicode string storing volume id
    _packed BYTE    Root[4];                //!< packed string to store the root path a:/ or b:/
    INT             MediaPresent;
} StStoreInfo;

////////////////////////////////////////////////////////////////////////////////
/// \struct _StStoreInfo 
///
/// This structure holds a pointer array to StStoreInfo data structure 
/// instantiated by Store enumeration process. It can hold a maximum of MAX_NUM_STORES.
///
typedef struct _StStoreArray {
    LONG            LastFileSystemError;    //!< last file system error returned by file system API
    INT             NumStores;              //!< number of active stores.
    StStoreInfo*    Entry[MAX_NUM_STORES];      //!< an array of StStoreInfo data structures
} StStoreArray;

////////////////////////////////////////////////////////////////////////////////
/// \struct _StStorParams 
///
/// This structure holds a set of 5 integer parameters. This is useful for functions that have a long list of 
/// parameters, they all can be put together in this structure and pass just one pointer to this structure instead
/// of sending each parameter separately. For convenience currently it has one parameters of type unsigned int, 
/// 3 of type int and a pointer to int.
///
typedef struct _StStorParams {
    WORD    wParam1;
    INT     nParam1;
    INT     nParam2;
    INT     nParam3;
    INT*    pParam1;
} StStorParams;

//////////////////////////////////////////////////////////////////
//! Prototypes for functions and API in store 
//////////////////////////////////////////////////////////////////
//! Store Enumeration API
extern INT _reentrant StorInit(void);

//! Objects API
extern _reentrant INT StorGetNumObjects(INT _StoreNumber, WORD _ParentObjectHandle, INT * _NumObjects );
extern INT _reentrant StorGetObjectList(StStorParams * Params, WORD _X *_raObjectHandles);
extern INT _reentrant StorAddNewObject(StStorParams *Params, UCS3 * _ObjectName);
extern INT _reentrant StorDeleteObject(StStorParams *Params, INT* rbAtleastOneDeleted);
extern INT _reentrant StorReadObjectData (WORD ObjectHandle, WORD _X * FileData, WORD BufSize, INT * FileHandle);
extern INT _reentrant StorWriteObjectData(WORD _ObjectHandle, WORD _X * _FileData, UINT16 _BufSize, INT * _FileHandle, LONG * rFileSize, LONG * rBytesWritten );

//! Miscellaneous functions
extern INT _reentrant StorClose(void);
extern INT _reentrant StorFlush(void);
extern _reentrant INT ChDirToParentOf(WORD _ObjHandle, WORD _StoreNumber);
extern _reentrant INT StorIsObjectValid(WORD _ObjectHandle);
extern DWORD _reentrant GetStorageIDFromStoreNumber(INT _StoreNumber);
extern INT _reentrant GetStoreNumberFromStorageID(DWORD _StorageID);
//! Index files i/o
extern _reentrant INT StorOpenFRIT (WORD FolderHandle, INT StoreNumber, INT _CreateFlag);
extern _reentrant INT StorCloseFRIT (INT _RefHandle);
extern _reentrant INT StorAddObjectToIndexTable (INT _RefHandle, WORD _ObjectHandle);
extern _reentrant INT StorDeleteObjectFromIndexTable( INT _RefFileHandle, WORD _ObjHandle );
extern _reentrant INT StorOpenOHIT( INT _Create );
extern _reentrant INT StorCloseOHIT(void);
//! Storage Info API
extern INT _reentrant StorGetStorageInfo(INT StoreNumber, StStoreInfo ** p_store_info);
//extern INT _reentrant StorGetNumStores( UINT16 * pNumStores );
extern INT _reentrant StorGetIDs(UINT32 * pStorageID, UINT16 Size);
extern INT _reentrant StorGetStoreType(INT StoreNumber, EnStoreType*   pStoreType);
extern INT _reentrant StorGetFileSystemType( INT StoreNumber,  EnFileSystemType * pFileSystemType  );
extern INT _reentrant StorGetAccessType(INT StoreNumber, EnAccessType * pAccessType);
extern INT _reentrant StorGetSize(INT StoreNumber, UINT64 * MaxCapacity );
extern INT _reentrant StorGetAvailableFreespace(INT StoreNumber, UINT64 * pFreeSpaceAvail );
//extern INT _reentrant StorGetVolumeID(INT StoreNumber, StString * pVolId );
extern INT _reentrant UpdateSpaceAvailable(INT _StoreNumber);
extern INT _reentrant StorGetLastFileSystemError(INT* FSError);

// GUID Calculating routines
extern UINT32 _reentrant CalcShortPathCRC(_packed BYTE _Y *Buffer);
extern UINT32 _reentrant CalcFileNameCRC(INT *Buffer);
void _reentrant GetBothCRCValues(void);

//! Object Properties API
//!
//! Macros
#define StorObjectStorageID(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, rValue, DB_UINT32_SIZE, DBFieldOffset_StorageID)
#define StorObjectProtectionStatus(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT16_SIZE, DBFieldOffset_ProtectionStatus)
#define StorObjectFormatCode(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT16_SIZE, DBFieldOffset_FormatCode)
#define StorObjectAssocType(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT16_SIZE, DBFieldOffset_AssocType)
#define StorObjectObjectSize(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT32_SIZE, DBFieldOffset_ObjectSize)
#define StorObjectFileName(OPType, ObjectHandle, rBuffer, rOffsetInBuffer, MemType) StorGetSetObjectDBString(OPType, ObjectHandle, DBFieldOffset_Filename, rBuffer, rOffsetInBuffer, MemType)
#define StorObjectDateCreated(OPType, ObjectHandle, rBuffer, rOffsetInBuffer, MemType) StorGetSetObjectDBString(OPType, ObjectHandle, DBFieldOffset_DateCreated, rBuffer, rOffsetInBuffer, MemType)
#define StorObjectArtist(OPType, ObjectHandle, rBuffer, rOffsetInBuffer, MemType) StorGetSetObjectDBString(OPType, ObjectHandle, DBFieldOffset_Artist, rBuffer, rOffsetInBuffer, MemType)
#define StorObjectAlbumName(OPType, ObjectHandle, rBuffer, rOffsetInBuffer, MemType) StorGetSetObjectDBString(OPType, ObjectHandle, DBFieldOffset_AlbumName, rBuffer, rOffsetInBuffer, MemType)
#define StorObjectNonConsumable(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT8_SIZE, DBFieldOffset_NonConsumable)
#define StorObjectNumChannels(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT16_SIZE, DBFieldOffset_NumChannels)
#define StorObjectDuration(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, rValue, DB_UINT32_SIZE, DBFieldOffset_Duration)
#define StorObjectSampleRate(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, rValue, DB_UINT32_SIZE, DBFieldOffset_SampleRate)
#define StorObjectAudioWaveCodec(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, rValue, DB_UINT32_SIZE, DBFieldOffset_AudioWaveCodec)
#define StorObjectAudioBitrate(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, rValue, DB_UINT32_SIZE, DBFieldOffset_AudioBitrate)
#define StorObjectParent(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT32_SIZE, DBFieldOffset_ParentObject)
#define StorObjectObjectHandle(OPType, ObjectHandle, rOH) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rOH, DB_UINT32_SIZE, DBFieldOffset_ObjectHandle)
#define StorObjectFastFileHandle(OPType, ObjectHandle, rValue) StorGetSetObjectBlockInfo(OPType, ObjectHandle, (void*)rValue, DB_UINT48_SIZE, DBFieldOffset_FastFileHandle)
#define StorObjectShortFilename(OPType, ObjectHandle, rValue) StorGetSetObjectPackedStrInfo(OPType, ObjectHandle, rValue, DBFieldOffset_ShortFilename)
#define StorObjectName(OPType, ObjectHandle, rBuffer, rOffsetInBuffer, MemType) StorGetSetObjectDBString(OPType, ObjectHandle, DBFieldOffset_Title, rBuffer, rOffsetInBuffer, MemType)
#define StorObjectPathChecksum(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT32_SIZE, DBFieldOffset_PathChecksum)
#define StorObjectFilenameChecksum(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT32_SIZE, DBFieldOffset_FilenameChecksum)
#define StorObjectRefreshNeeded(OPType, ObjectHandle, rValue) StorGetSetObjectNumInfo(OPType, ObjectHandle, (UINT32*)rValue, DB_UINT32_SIZE, DBFieldOffset_RefreshNeeded)

//! Functions
extern INT _reentrant StorGetSetObjectNumInfo(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue, INT Size, INT FieldOffset);
//extern INT _reentrant StorGetSetObjectStrInfo(EnGetSet OPType, WORD ObjectHandle, StString *rValue, INT FieldOffset);
extern INT _reentrant StorGetSetObjectBlockInfo(EnGetSet OPType, WORD ObjectHandle, void *rValue, INT Size, INT FieldOffset);
extern INT _reentrant StorGetSetObjectPackedStrInfo(EnGetSet OPType, WORD ObjectHandle, _packed BYTE * rValue, INT FieldOffset);
extern INT _reentrant StorGetSetObjectDBString(EnGetSet OPType, WORD ObjectHandle, INT FieldOffset, INT* rBuffer, INT* rOffsetInBuffer, INT MemType);
//! Object References API
/*
extern INT _reentrant StorGetNumReferences(WORD ObjectHandle, UINT16 *NumReferences);
extern INT _reentrant StorGetObjectReferences(WORD ObjectHandle, UINT16 StartIndex, WORD *pObjectHandleList, UINT16 *ListSize); 
extern INT _reentrant StorAddObjectReferences(WORD ObjectHandle, WORD * pObjList, UINT16 ListSize);
extern INT _reentrant StorRemoveObjectReferences(WORD ObjectHandle);
*/ 
extern INT _reentrant ConvStmp2HostUnicodeStr( INT * pHostStr, INT * pStmpStr, INT SizeOfStmpStrInChars );

//!
//! 
//!
RETCODE _inline SysCallGetFileMetaData(int PackedPathNameAddress, int btCurrentDevice, FILE_META_DATA *MetaData)
{
__asm ("    extern  RSRC_METADATA_CODEBANK          \n"     
       "    extern  FGetFileMetaData                \n"     
       "    move    %0,r1                           \n"
       "    move    #RSRC_METADATA_CODEBANK,r0      \n"     
       "    move    #FGetFileMetaData,r4" : : "A"(MetaData));
    return exSysCallFunction_ii(PackedPathNameAddress, btCurrentDevice);
}


//! This assembly trickery is to allow the resource number to be defined 
//! the library.
//! WARNING: Do not move any pointers into the SysCallFunction as that will
//! reuse the r0 and r4 pointers.
//!
int _inline SysCallStorDeleteObject(StStorParams *Params, INT* rbAtleastOneDeleted)               
{
__asm ("    extern  RSRC_MTP_REMOVEOBJECT       \n"     
       "    extern  FStorDeleteObject           \n"     
       "    move    %0,r1                       \n"
       "    move    %1,r5                       \n"
       "    move    #RSRC_MTP_REMOVEOBJECT,r0   \n"     
       "    move    #FStorDeleteObject,r4" : : "A"(Params), "A"(rbAtleastOneDeleted));
    return exSysCallFunction_v();
}

//!
//! 
//!
int _inline SysCallStorAddNewObject(StStorParams *Params, _packed BYTE * _rObjectName)               
{
__asm ("    extern  RSRC_MTP_SENDOBJECTINFO     \n"     
       "    extern  FStorAddNewObject           \n"     
       "    move    %0,r1                       \n"
       "    move    %1,r5                       \n"
       "    move    #RSRC_MTP_SENDOBJECTINFO,r0 \n"     
       "    move    #FStorAddNewObject,r4" : : "A"(Params), "A"(_rObjectName));
    return exSysCallFunction_v();
}

//!
//! 
//!
int _inline SysCallStorGetObjectList(StStorParams * Params, WORD _X *_raObjectHandles)               
{
__asm ("    extern  RSRC_MTP_GETOBJECTHANDLES       \n"     
       "    extern  FStorGetObjectList              \n"     
       "    move    %0,r1                           \n"
       "    move    %1,r5                           \n"
       "    move    #RSRC_MTP_GETOBJECTHANDLES,r0   \n"     
       "    move    #FStorGetObjectList,r4" : : "A"(Params), "A"(_raObjectHandles));
    return exSysCallFunction_v();
}

//!
//! 
//!
extern volatile INT g_stor_dirty;

#endif //!__STSTORE_H_