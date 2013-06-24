// filename: updater_scsi_commands.c
#include "scsi.h"
#include "usbmsc.h"
#include "misc.h"
#include "ddildl.h"
#include "stringlib.h"
#include "sysresourceapi.h"
#include "project.h"
#include "sysserialnumber.h"
#include "sectordef.h"
//#include "regsrevision.h"


RETCODE lcl_Status=0;

#define SCSI_OPCODE_SIGMATEL_READ_COMMAND  0xc0
#define SCSI_OPCODE_SIGMATEL_WRITE_COMMAND 0xc1

#define GET_PROTOCOL_VERSION        0x00
#define GET_STATUS                  0x01
#define GET_LOGICAL_MEDIA_INFO      0x02
#define GET_LOGICAL_MEDIA_NUM       0x03
#define GET_ALLOCATION_TABLE        0x05
#define ALLOCATE_MEDIA              0x06
#define ERASE_LOGICAL_MEDIA         0x07
#define GET_LOGICAL_DRIVE_INFO      0x12
#define READ_LOGICAL_DRIVE_SECTOR   0x13
#define SET_LOGICAL_DRIVE_INFO      0x20
#define WRITE_LOGICAL_DRIVE_SECTOR  0x23
#define ERASE_LOGICAL_DRIVE         0x2f
#define GET_CHIP_ID                 0x30
#define CHIP_RESET                  0x31
#ifdef MFG_TOOL
    #define GET_CHIP_SN_INFO        0x32
    #define BOGUS_COMMAND           0x33
#else
    #define BOGUS_COMMAND           0x32
#endif
#define GET_PHYSICAL_MEDIA_INFO     0x34
#define READ_PHYSICAL_MEDIA_SECTOR  0x35
#define GET_CHIP_PART_ID            0x36
#define GET_ROM_REVISION            0x37
#define WRITE_BOOTMANAGER_SECTOR    0x38



extern volatile WORD g_wActivityState;
extern int g_iBuildType;

RETCODE _reentrant usbmsc_send_command_response(USBMSC_DEVICE * pDev, SCSI_LUN *pLun,WORD _X *pwBuffer, WORD wLength);


RETCODE _reentrant UpdaterGetProtocolVersion(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterGetStatus(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterGetLogicalMediaNum(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterGetLogicalMediaInfo(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterGetAllocationTable(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterEraseLogicalMedia(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterGetLogicalDriveInfo(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterReadLogicalDriveSector(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterSetLogicalDriveInfo(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterWriteLogicalDriveSector(WORD wDeviceNum, WORD _X* pCdb);
// remove line below eventually (when new updater is ready and debugged)
RETCODE _reentrant UpdaterWriteBootManagerSector(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterEraseLogicalDrive(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterGetChipID(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterGetChipPartID(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterGetROMRevision(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterChipReset(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterBogusCommand(WORD wDeviceNum, WORD _X* pCdb);
RETCODE _reentrant UpdaterAllocateMedia(WORD wDeviceNum, WORD _X* pCdb);

RETCODE _reentrant UpdaterConvert2KSectorTo512Sector(WORD *pBuffer);

#ifdef MFG_TOOL
RETCODE _reentrant UpdaterGetChipSnInfo(WORD wDeviceNum, WORD _X* pCdb);
#endif

extern WORD CapGetChipID(void);
extern WORD CapGetChipROMRevID(void);
extern WORD CapGetChipPartID(void);

long GetBigEndianINT64FromByteBufferInX(void _X *pBuffer, int iStartIndex);
void PutBigEndianINT64IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue);
long GetBigEndianINT32FromByteBufferInX(void _X *pBuffer, int iStartIndex);
void PutBigEndianINT32IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue);
int  GetBigEndianINT16FromByteBufferInX(void _X *pBuffer, int iStartIndex);
void PutBigEndianINT16IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue);
extern WORD g_wNumDrives;
extern WORD g_wNumMedia;
extern RETCODE _reentrant ScsiHandleFailedCommand(WORD wDevNum, WORD wInputOutput);
extern WORD g_wSTMPDeviceId;

#ifdef MFG_TOOL
SCSI_COMMAND_ENTRY g_UpdaterCommands[] =
{
    {GET_PROTOCOL_VERSION,      &UpdaterGetProtocolVersion},
    {GET_STATUS,                &UpdaterBogusCommand},
    {GET_LOGICAL_MEDIA_INFO,    &UpdaterGetLogicalMediaInfo},
    {GET_LOGICAL_MEDIA_NUM,     &UpdaterGetLogicalMediaNum},
    {GET_ALLOCATION_TABLE,      &UpdaterGetAllocationTable},
    {ERASE_LOGICAL_MEDIA,       &UpdaterEraseLogicalMedia},
    {GET_LOGICAL_DRIVE_INFO,    &UpdaterGetLogicalDriveInfo},
    {READ_LOGICAL_DRIVE_SECTOR, &UpdaterReadLogicalDriveSector},
    {SET_LOGICAL_DRIVE_INFO,    &UpdaterSetLogicalDriveInfo},
    {WRITE_LOGICAL_DRIVE_SECTOR,&UpdaterWriteLogicalDriveSector},
    {ERASE_LOGICAL_DRIVE,       &UpdaterEraseLogicalDrive},
    {GET_CHIP_ID,               &UpdaterGetChipID},
    {ALLOCATE_MEDIA,            &UpdaterAllocateMedia},
    {CHIP_RESET,                &UpdaterChipReset},
    {GET_CHIP_SN_INFO,          &UpdaterGetChipSnInfo},
    {GET_CHIP_PART_ID,          &UpdaterGetChipPartID},
    {GET_ROM_REVISION,          &UpdaterGetROMRevision},
    /* remove line below eventually (when new updater is ready and debugged) */
    {WRITE_BOOTMANAGER_SECTOR,  &UpdaterWriteBootManagerSector},
    {BOGUS_COMMAND,             &UpdaterBogusCommand},
    {0xffffff,NULL}
};
#else
SCSI_COMMAND_ENTRY g_UpdaterCommands[] =
{
    {GET_PROTOCOL_VERSION,      &UpdaterGetProtocolVersion},
    {GET_STATUS,                &UpdaterBogusCommand},
    {GET_LOGICAL_MEDIA_INFO,    &UpdaterGetLogicalMediaInfo},
    {GET_LOGICAL_MEDIA_NUM,     &UpdaterGetLogicalMediaNum},
    {GET_ALLOCATION_TABLE,      &UpdaterGetAllocationTable},
    {ERASE_LOGICAL_MEDIA,       &UpdaterEraseLogicalMedia},
    {GET_LOGICAL_DRIVE_INFO,    &UpdaterGetLogicalDriveInfo},
    {READ_LOGICAL_DRIVE_SECTOR, &UpdaterReadLogicalDriveSector},
    {SET_LOGICAL_DRIVE_INFO,    &UpdaterSetLogicalDriveInfo},
    {WRITE_LOGICAL_DRIVE_SECTOR,&UpdaterWriteLogicalDriveSector},
    {ERASE_LOGICAL_DRIVE,       &UpdaterEraseLogicalDrive},
    {GET_CHIP_ID,               &UpdaterGetChipID},
    {ALLOCATE_MEDIA,            &UpdaterAllocateMedia},
    {CHIP_RESET,                &UpdaterChipReset},
    {BOGUS_COMMAND,             &UpdaterBogusCommand},
    {GET_CHIP_PART_ID,          &UpdaterGetChipPartID},
    {GET_ROM_REVISION,          &UpdaterGetROMRevision},
    /* remove line below eventually (when new updater is ready and debugged) */
    {WRITE_BOOTMANAGER_SECTOR,  &UpdaterWriteBootManagerSector},
    {0xffffff,NULL}
};
#endif

RETCODE _reentrant SigmalSCSICommandHandler(WORD wDeviceNum, WORD _X *pCdb);

struct Command{
    union {
        struct {
            WORD wSCSICommand       : 8;
            WORD wSigmatelCommand   : 8;
            WORD wReserved          : 8;
        } Generic;
        struct {
            WORD wSCSICommand       : 8;
            WORD wSigmatelCommand   : 8;
            WORD wDriveNumber       : 8;
        } WithDriveNumber;
        struct {
            WORD wSCSICommand       : 8;
            WORD wSigmatelCommand   : 8;
            WORD wType              : 8;
        } WithType;
        struct {
            WORD wSCSICommand       : 8;
            WORD wSigmatelCommand   : 8;
            WORD wMaxDescriptors    : 8;
        } WithMaxDescriptors;
    } FirstWord;
    union {
        struct {
            WORD wType              : 8;
            WORD wTag               : 8;
            WORD wUnused            : 8;
        } WithTypeAndTag;
        struct {
            WORD wType              : 8;
            WORD wVersionMajorMSb   : 8;
            WORD wVersionMajorLSb   : 8;
        } WithTypeAndVersion;
        struct {
            WORD wMSB7              :8;
            WORD wMSB6              :8;
            WORD wMSB5              :8;
        } StartSector;
    } SecondWord;
    union {
        struct {
            WORD wVersionMinorMSb   :8;
            WORD wVersionMinorLSb   :8;
            WORD wVersionRevisionMSb:8;
        } Version;
        struct {
            WORD wMSB4              :8;
            WORD wMSB3              :8;
            WORD wMSB2              :8;
        } StartSector;
    } ThirdWord;
    union {
        struct {
            WORD wMSB1              :8;
            WORD wMSB0              :8;
            WORD wUnused            :8;
        } StartSector;
        struct {
            WORD wVersionRevisionLSb:8;
            WORD wUnused            :16;
        } Version;
        struct {
            WORD wUnused            :16;
            WORD wMSB3              :8;
        } SectorCount;
    } FourthWord;
    union {
        struct {
            WORD wMSB2              :8;
            WORD wMSB1              :8;
            WORD wMSB0              :8;
        } SectorCount;
    } FifthWord;
} ;

typedef enum
{
  eUpdateDriveInfo,
  eUpdateMediaInfo
} UPDATE_INFO_TYPES;

typedef enum
{
  eProtocolVersion,
  eLogicalMediaNum,
  eGetChipID,
  eGetChipPartID,
  eGetROMRevision
} UPDATE_GET_WORD_TYPES;

BOOL g_UpdaterChipReset = FALSE;
int g_iUpdaterCommands=0;
int g_iUnknownCommands=0;
SCSI_CH _reentrant GetVendorSpecificSCSIHandler(WORD wOpCode)
{
    SCSI_CH pFunc;

    switch(wOpCode)
    {
    case SCSI_OPCODE_SIGMATEL_READ_COMMAND:
    case SCSI_OPCODE_SIGMATEL_WRITE_COMMAND:
        g_iUpdaterCommands++;
        pFunc = &SigmalSCSICommandHandler;
        break;
    default:
        pFunc  = GetCustomerExtentionSCSIHandler(wOpCode);
        if(!pFunc)
            g_iUnknownCommands++;
        break;
    }

    return pFunc;
}

RETCODE _reentrant SigmalSCSICommandHandler(WORD wDeviceNum, WORD _X *pCdb)
{
    WORD RetValue = 0;
    WORD wCommand;
    SCSI_CH pFunc;
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;

    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);

    //set up default sense data.
    ScsiLunSetupDefaultSenseData(pLun);

    if((pFunc =  SearchForSCSICommand(g_UpdaterCommands,((struct Command _X*)pCdb)->FirstWord.Generic.wSigmatelCommand)))
    {
        RetValue = (pFunc)(wDeviceNum,pCdb);
        if(RetValue!=SUCCESS)
        {
            pLun->wCompletionStatus = USBMSC_CSW_COMMAND_FAILED;
            pLun->SenseData.wSenseKey = SCSI_SENSE_KEY_VENDOR_SPECIFIC;
            // Flip the bytes for the ASC/ASCQ
            pLun->SenseData.wAscqAsc = (lcl_Status & 0x00FF00)  >> 8;
            pLun->SenseData.wAscqAsc |= (lcl_Status & 0x0000FF) << 8;
        }
    }
    else
    {
        //Shouldn't see any commands we don't support.
        RetValue = SCSI_ASC_INVALID_COMMAND_OPERATION_CODE;
    }
    return RetValue;
}

RETCODE _reentrant UpdaterCommonGetWords(WORD wDeviceNum, WORD _X* pCdb, 
                               UPDATE_GET_WORD_TYPES eUpdaterGetWordType)
{
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;
    WORD _X * pwBuffer;
    WORD wSize;

    // Get a local pointer to the device
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);
    pwBuffer = pDev->CommandInBuffer;

    switch (eUpdaterGetWordType)
    {
      case eProtocolVersion:
        //01.00 updater.sb or 02.00 hostlink.sb protocol version
        if (g_iBuildType == BUILD_TYPE_UPDATER)
        {   pwBuffer[0]=0x000001; 
        }
        else // hostlink
        {   pwBuffer[0]=0x000002; // sdk3.x, and 2.610 and above use 2 to result in bootmanager erase during update. 
            //Hostlink actually links mtp_updater_scsi_commands.obj/c so this ver 2 code here probably never executes in sdk3, but it does in sdk2.610.
        }
        wSize = 2;
        break;
      case eLogicalMediaNum:
        pwBuffer[0] = pLun->wFileSystemMediaNumber;
        wSize = 1;
        break;
      case eGetChipID:
        PutBigEndianINT16IntoByteBufferInX(pwBuffer,0, CapGetChipID() ); 
        wSize = 2;
        break;
      case eGetChipPartID:
        PutBigEndianINT16IntoByteBufferInX(pwBuffer,0, CapGetChipPartID() );
        wSize = 2;
        break;
      case eGetROMRevision:
        PutBigEndianINT16IntoByteBufferInX(pwBuffer,0, CapGetChipROMRevID() );
     //   pwBuffer[0] = (*((WORD _X *)0xFA02)) & 0xF; 
        wSize = 2;
        break;
    }
    

    return usbmsc_send_command_response(pDev, pLun,pwBuffer, 2); 
    
}

RETCODE _reentrant UpdaterGetProtocolVersion(WORD wDeviceNum, WORD _X* pCdb)
{
    return UpdaterCommonGetWords(wDeviceNum, pCdb, eProtocolVersion); 
    
}

RETCODE _reentrant UpdaterGetLogicalMediaNum(WORD wDeviceNum, WORD _X* pCdb)
{
     return UpdaterCommonGetWords(wDeviceNum, pCdb, eLogicalMediaNum); 
}

RETCODE _reentrant UpdaterGetChipID(WORD wDeviceNum, WORD _X* pCdb)
{
    // This is now actually trapped in usbmsc.c - look there.
    return UpdaterCommonGetWords(wDeviceNum, pCdb, eGetChipID);
}

// TOVERIFY: TODO:  note 2 functions can be changed to save P memory. Consider inline if called once or 2nd choice: macro func call.
RETCODE _reentrant UpdaterGetChipPartID(WORD wDeviceNum, WORD _X* pCdb)
{
    // This is now actually trapped in usbmsc.c - look there.
    return UpdaterCommonGetWords(wDeviceNum, pCdb, eGetChipPartID);
}

RETCODE _reentrant UpdaterGetROMRevision(WORD wDeviceNum, WORD _X* pCdb)
{
    // This is now actually trapped in usbmsc.c - look there.
    return UpdaterCommonGetWords(wDeviceNum, pCdb, eGetROMRevision);
}

WORD _reentrant PrepareReturnBuffer(WORD _X *pwBuffer, void * pInfoBuffer, WORD wReturnType)
{
    WORD wSize = 0;
    switch(wReturnType )
    {
    case 1://WORD
    case 3://LogicalDriveType
        PutBigEndianINT32IntoByteBufferInX(pwBuffer,0,*((WORD*)pInfoBuffer));
        wSize = 4;
        break;
    case 5://BOOL
        PutByteInArrayInX(pwBuffer,0,*((WORD*)pInfoBuffer) & 0xff);
        wSize = 1;
        break;
    case 2: //dword
        PutBigEndianINT64IntoByteBufferInX(pwBuffer,0,*((DWORD*)pInfoBuffer));
        wSize = 8;
        break;
    case 4://version
		  PutByteInArrayInX(pwBuffer,0,((SYSTEM_VERSION*)pInfoBuffer)->PARTIAL_VERSION.MajorH & 0xff);
		  PutByteInArrayInX(pwBuffer,1,((SYSTEM_VERSION*)pInfoBuffer)->PARTIAL_VERSION.MajorL & 0xff);
		  PutByteInArrayInX(pwBuffer,2,((SYSTEM_VERSION*)pInfoBuffer)->PARTIAL_VERSION.MiddleH & 0xff);
		  PutByteInArrayInX(pwBuffer,3,((SYSTEM_VERSION*)pInfoBuffer)->PARTIAL_VERSION.MiddleL & 0xff);
		  PutByteInArrayInX(pwBuffer,4,((SYSTEM_VERSION*)pInfoBuffer)->PARTIAL_VERSION.MinorH & 0xff);
		  PutByteInArrayInX(pwBuffer,5,((SYSTEM_VERSION*)pInfoBuffer)->PARTIAL_VERSION.MinorL & 0xff);
        wSize = 6;
        break;
    case 6:
        {
            int i;
            for(i=0;i<(67+2)/3;i++)
            {
                ((WORD _X *)pwBuffer)[i]=((WORD*)pInfoBuffer)[i];
            }
            wSize = 67;
        } break;
    }
    return wSize;
}


RETCODE _reentrant UpdaterCommonGetInfo(WORD wDeviceNum, WORD _X* pCdb, UPDATE_INFO_TYPES eMediaOrData)
{
    void            *pInfoBuffer=NULL;
    WORD _X         *pwBuffer;
    WORD            wMediaNum=0;
    WORD            wDrive=0;
    WORD            wTypeReturnType=0;
    USBMSC_DEVICE * pDev;
    WORD            wSize=0;
    SCSI_LUN * pLun;
    struct Command _X * pCommand = (struct Command _X*)pCdb;    
    PHYSICAL_MEDIA_TYPE MediaType;
    MEDIA_STATE     MediaState;
    SYSTEM_VERSION Version;
    DWORD          dwWord;
    WORD            wWord;        
    BOOL            bBool;
    WORD            wType;
    int i;
    LOGICAL_DRIVE_TYPE LogicalDriveType;
    _packed BYTE wSerialNumBuffer[67];
    // Get a local pointer to the device
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);
    pwBuffer = pDev->CommandInBuffer;
    if (eMediaOrData == eUpdateMediaInfo)
    {
      wMediaNum = pLun->wFileSystemMediaNumber;
      wType = pCommand->FirstWord.WithType.wType;
      // Dang - can't combine the Media & Drive cases
      // because separately declared enums...
      switch(wType)
      {
      case MediaInfoNumberOfDrives:
      case MediaInfoAllocationUnitSizeInBytes :
      case MediaInfoSizeOfSerialNumberInBytes:
          wTypeReturnType = 1;
          pInfoBuffer = &wWord;
          break;
      case MediaInfoSizeInBytes :
          wTypeReturnType = 2;
          pInfoBuffer = &dwWord;
          break;
      case MediaInfoIsInitialized:
      case MediaInfoIsWriteProtected:
      case MediaInfoIsSystemMedia :
          wTypeReturnType = 5;
          pInfoBuffer = &bBool;
          break;
      case MediaInfoSerialNumber:
          wTypeReturnType = 6;
          pInfoBuffer = wSerialNumBuffer;
          break;
      case MediaInfoMediaState:
          wTypeReturnType = 7;
          pInfoBuffer = &MediaState;
          break;
      case MediaInfoPhysicalMediaType:
          wTypeReturnType = 8;
          pInfoBuffer = &MediaType;
          break;
      default:
          break;
      }
    } else
    {
      wDrive = pCommand->FirstWord.WithDriveNumber.wDriveNumber;
      wType = pCommand->SecondWord.WithTypeAndTag.wType;
      switch(wType)
      {
      case DriveInfoSectorSizeInBytes:
      case DriveInfoEraseSizeInBytes :
      case DriveInfoTag:
      case DriveInfoSizeOfSerialNumberInBytes:
          wTypeReturnType = 1;
          pInfoBuffer = &wWord;
          break;
      case DriveInfoSizeInBytes :
      case DriveInfoSizeInMegaBytes:
      case DriveInfoSizeInSectors :
          wTypeReturnType = 2;
          pInfoBuffer = &dwWord;
          break;
      case DriveInfoType:
          wTypeReturnType = 3;
          pInfoBuffer = &LogicalDriveType;
          break;
      case DriveInfoComponentVersion:
      case DriveInfoProjectVersion:
          wTypeReturnType = 4;
          pInfoBuffer = &Version;
          break;
      case DriveInfoIsWriteProtected:
          wTypeReturnType = 5;
          pInfoBuffer = &bBool;
          break;
      case DriveInfoSerialNumber:
          wTypeReturnType = 6;
          pInfoBuffer = wSerialNumBuffer;
          break;
      default:
          break;
      }
    }
    
    
    if(pInfoBuffer)
    {
        if (eMediaOrData == eUpdateMediaInfo)
        {
          lcl_Status = MediaGetInfo(wMediaNum, wType, pInfoBuffer);
        } else
        {
          lcl_Status = DriveGetInfo(wDrive, wType, pInfoBuffer);
        }        
        
        if(lcl_Status == SUCCESS)
        {
            wSize = PrepareReturnBuffer(pwBuffer,pInfoBuffer,wTypeReturnType);
            if(!wSize)
            {         
                lcl_Status = 
              ((eMediaOrData == eUpdateMediaInfo) ?
                  LMEDIA_ERROR_INVALID_INFO_TYPE : LDRIVE_ERROR_INVALID_INFO_TYPE);
            }
        }
    }
    else
    {
        lcl_Status = LMEDIA_ERROR_INVALID_INFO_TYPE;
    }
    if(lcl_Status == SUCCESS)
        return usbmsc_send_command_response(pDev, pLun,pwBuffer, wSize); 
    else
        return SCSI_ERROR_DDI_CALL_FAILED;

}

RETCODE _reentrant UpdaterGetLogicalMediaInfo(WORD wDeviceNum, WORD _X* pCdb)
{
  return UpdaterCommonGetInfo(wDeviceNum, pCdb, eUpdateMediaInfo);
}
// Change from previous value 10 to 11 since host updater sends mediaallocate command needs
// 11 descriptor table to support 3 hostrsc.bin and 3 resource.bin
// Mar 15 2007: 12 descriptor table entries to support 1 extra system drive in future
#define MAX_ALLOCATION_DESCRIPTORS  12
#define ALLOCATION_DESCRIPTOR_SIZE  11
SyncObject g_AllocationSyncObject={0};
_packed BYTE _X g_AllocationTableBuffer[2+MAX_ALLOCATION_DESCRIPTORS*ALLOCATION_DESCRIPTOR_SIZE];
MEDIA_ALLOCATION_TABLE g_AllocationTable;
RETCODE _reentrant UpdaterGetAllocationTable(WORD wDeviceNum, WORD _X* pCdb)
{
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;
    SCSI_SENSE_DATA * pSense;
    WORD _X * pwBuffer;
    RETCODE rtCode = SUCCESS;
    P_MEDIA_ALLOCATION_TABLE pMediaTable;

    // Get a local pointer to the device
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);
    pSense = &(pLun->SenseData);
    SysWaitOnObject(-1,&g_AllocationSyncObject);

    if((lcl_Status = MediaGetMediaTable(1+wDeviceNum,&pMediaTable))==SUCCESS)
    {
        int i;
        int iIndex = 0;
        PutBigEndianINT16IntoByteBufferInX(g_AllocationTableBuffer,iIndex,pMediaTable->wNumEntries);
        iIndex+=2;
        for(i=0;i<pMediaTable->wNumEntries;i++)
        {
            PutByteInArrayInX(g_AllocationTableBuffer,iIndex++, pMediaTable->Entry[i].wDriveNumber);
            PutByteInArrayInX(g_AllocationTableBuffer,iIndex++, pMediaTable->Entry[i].Type);
            PutByteInArrayInX(g_AllocationTableBuffer,iIndex++, pMediaTable->Entry[i].wTag);
            PutBigEndianINT64IntoByteBufferInX(g_AllocationTableBuffer,iIndex, pMediaTable->Entry[i].dwSizeInBytes);
            iIndex += 8;
        
        }
        rtCode = usbmsc_send_command_response(pDev, pLun,(void _X *)g_AllocationTableBuffer, iIndex); 
    }
    else
    {
        pLun->wCompletionStatus = USBMSC_CSW_COMMAND_FAILED;
        ScsiHandleFailedCommand(wDeviceNum, SCSI_DATA_TO_HOST);

        rtCode = SCSI_ERROR_DDI_CALL_FAILED;
    }

    SysReleaseObject(&g_AllocationSyncObject);
    return rtCode;
}

RETCODE _reentrant UpdaterAllocateMedia(WORD wDeviceNum, WORD _X* pCdb)
{
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;
    RETCODE rtCode = SUCCESS;
    WORD wDescriptors;
    struct Command _X * pCommand=(struct Command _X *)pCdb;
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);

    SysWaitOnObject(-1,&g_AllocationSyncObject);

    wDescriptors =     pCommand->FirstWord.WithMaxDescriptors.wMaxDescriptors;

    if (wDescriptors > MAX_ALLOCATION_DESCRIPTORS)
    {
        lcl_Status = SCSI_ERROR_DDI_CALL_FAILED;
    }
    else
    {
        SyncObject *pObject = &pDev->TransferObject;
        SysAssignObject(pObject,ISR_OWNER);
        g_wActivityState = SCSI_WRITING;
        usb_device_recv_data(pDev->wBulkOutEndpointNum,(void _X *)g_AllocationTableBuffer,0,wDescriptors*ALLOCATION_DESCRIPTOR_SIZE,COMMAND_HANDLER|((WORD)pDev),FALSE);
        while(SysWaitOnObject(100,pObject)!=EVENT_OBJECT)
        {
            if(pDev->bMSCReset)
            {
            rtCode= SCSI_ERROR_USB_MSC_RESET_DURING_SCSI_COMMAND;
            }
        };
        //now parse it all out and call mediaallocate
        if(rtCode == SUCCESS)
        {
            int i;
            int iIndex = 0;
            g_AllocationTable.wNumEntries = wDescriptors;
            for(i=0;i<wDescriptors;i++)
            {
                g_AllocationTable.Entry[i].Type         = GetByteFromArrayInX(g_AllocationTableBuffer,iIndex++);
                g_AllocationTable.Entry[i].wTag         = GetByteFromArrayInX(g_AllocationTableBuffer,iIndex++);
                g_AllocationTable.Entry[i].dwSizeInBytes = GetBigEndianINT64FromByteBufferInX(g_AllocationTableBuffer,iIndex);
                iIndex += 8;
            }
            lcl_Status = MediaAllocate(wDeviceNum+1,&g_AllocationTable);
            g_wNumDrives = g_wNumMedia + 1;
            if(MediaDiscoverAllocation(0) != SUCCESS)
            {
                SystemHalt();
            }

            if(MediaDiscoverAllocation(1) != SUCCESS)
            {
                SystemHalt();
            }


            // Init All Drives
            for(i=0;i<g_wNumDrives;i++)
            {
                if(DriveInit(i) != SUCCESS)
                {
//                    SystemHalt();
                }
            }

            SysSetResourceTag(DRIVE_TAG_RESOURCE_BIN);
            if(lcl_Status != SUCCESS)
                rtCode =     SCSI_ERROR_DDI_CALL_FAILED;


            if(UsbMscDriverInitialize() != SUCCESS)
            {
//                SystemHalt();
            }


        }

    }
    SysReleaseObject(&g_AllocationSyncObject);
    return rtCode;

}

RETCODE _reentrant UpdaterEraseLogicalMedia(WORD wDeviceNum, WORD _X* pCdb)
{
    
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;
    // Get a local pointer to the device
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);

    lcl_Status = MediaErase(pLun->wFileSystemMediaNumber,DDI_MAGIC_NUMBER);

    return lcl_Status == SUCCESS ? SUCCESS : SCSI_ERROR_DDI_CALL_FAILED;
}
RETCODE _reentrant UpdaterGetLogicalDriveInfo(WORD wDeviceNum, WORD _X* pCdb)
{
    return UpdaterCommonGetInfo(wDeviceNum, pCdb, eUpdateDriveInfo);
}


RETCODE _reentrant UpdaterReadLogicalDriveSector(WORD wDeviceNum, WORD _X* pCdb)
{
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;

    struct Command _X * pCommand = (struct Command _X*)pCdb;    
    DWORD   dwStartSector;
    WORD    wSectorCount;
    WORD    wDrive;

    // Get a local pointer to the device
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);
    wDrive = pCommand->FirstWord.WithDriveNumber.wDriveNumber;
    dwStartSector = GetBigEndianINT64FromByteBufferInX(pCdb,3);
    wSectorCount = GetBigEndianINT32FromByteBufferInX(pCdb,11);
    lcl_Status = SCSIReadSectorHelper(wDeviceNum,wDrive,dwStartSector,wSectorCount);

    return lcl_Status == SUCCESS ? SUCCESS : SCSI_ERROR_DDI_CALL_FAILED;
}
RETCODE _reentrant UpdaterSetLogicalDriveInfo(WORD wDeviceNum, WORD _X* pCdb)
{
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;
    WORD    wDrive;
    WORD    wType;
    WORD    wTag;
    DWORD   dwVersion;
    void *  pBuffer=NULL;
    struct Command _X * pCommand = (struct Command _X*)pCdb;
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);
    wDrive = pCommand->FirstWord.WithDriveNumber.wDriveNumber;
    wType = pCommand->SecondWord.WithTypeAndTag.wType;
    switch(wType)
    {
        case DriveInfoTag:
        {
            wTag = pCommand->SecondWord.WithTypeAndTag.wTag;
            pBuffer = &wTag;
        }   break;
        case DriveInfoProjectVersion:
        case DriveInfoComponentVersion:
        {
            dwVersion = GetBigEndianINT64FromByteBufferInX(pCdb, 2);
            pBuffer = &dwVersion;
        }   break;

    }
    if(pBuffer)
    {
        lcl_Status = DriveSetInfo(wDrive, wType,pBuffer);
    }
    else
    {
        lcl_Status = LDRIVE_ERROR_INVALID_INFO_TYPE;
    }
    
    return lcl_Status == SUCCESS ? SUCCESS : SCSI_ERROR_DDI_CALL_FAILED;
}

//
//
// Notes: 
//
RETCODE _reentrant UpdaterWriteLogicalDriveSector(WORD wDeviceNum, WORD _X* pCdb)
{
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;

    struct Command _X * pCommand;
    DWORD   dwStartSector;
    WORD    wSectorCount;
    WORD    wDrive;

    // Get a local pointer to the device
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);
    pCommand = (struct Command _X*)pCdb;
    wDrive = pCommand->FirstWord.WithDriveNumber.wDriveNumber;
    dwStartSector = GetBigEndianINT64FromByteBufferInX(pCdb,3);
    wSectorCount = GetBigEndianINT32FromByteBufferInX(pCdb,11);
    lcl_Status = SCSIWriteSectorHelper(wDeviceNum,wDrive,dwStartSector,wSectorCount,FALSE/*don't use multiwrite*/);

    return lcl_Status == SUCCESS ? SUCCESS : SCSI_ERROR_DDI_CALL_FAILED;
}

#if 1  // remove this function eventually so we can eliminate this updater commnad (for a common updater for both sdks)
//
//
RETCODE _reentrant UpdaterWriteBootManagerSector(WORD wDeviceNum, WORD _X* pCdb)
{
 #if 0
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;
    WORD wSectorSizeInBytes;
    WORD wFsLun;


    // Get a local pointer to the device
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);
    wFsLun = pLun->wFileSystemDriveNumber;
    
    /* Get the drive sectorsizeinbytes */

    DriveGetInfo(wFsLun,DriveInfoSectorSizeInBytes,(void *)(&wSectorSizeInBytes));

    if(wSectorSizeInBytes == LARGE_SECTOR_DATA_SIZE)
        {
        g_bWriteBmngr512Bytes = TRUE;
        }

    lcl_Status = UpdaterWriteLogicalDriveSector(wDeviceNum, pCdb);

    /* Reset the write flag */
    
    g_bWriteBmngr512Bytes = FALSE;

    return lcl_Status ;
 #else
	return SUCCESS;
 #endif
}
#endif


RETCODE _reentrant UpdaterEraseLogicalDrive(WORD wDeviceNum, WORD _X* pCdb)
{
    struct Command _X * pCommand;

    pCommand = (struct Command _X*)pCdb;

    lcl_Status = DriveErase(pCommand->FirstWord.WithDriveNumber.wDriveNumber,DDI_MAGIC_NUMBER);

    return lcl_Status == SUCCESS ? SUCCESS : SCSI_ERROR_DDI_CALL_FAILED;
}

#ifdef MFG_TOOL
RETCODE _reentrant UpdaterGetChipSnInfo(WORD wDeviceNum, WORD _X* pCdb)
{
    USBMSC_DEVICE * pDev;
    SCSI_LUN * pLun;
    WORD _X * pwBuffer;
    INT i, j;
	WORD wType;
	WORD wRetSize=0;
 
    struct Command _X * pCommand = (struct Command _X*)pCdb;    

    // Get a local pointer to the device
    pDev = &(UsbMscDevice[wDeviceNum]);
    pLun = &(pDev->Lun[pDev->CBW.wCBWLUN]);
    pwBuffer = pDev->CommandInBuffer;
    wType = pCommand->FirstWord.WithType.wType;

    switch(wType)
    {
    case SerialNoInfoSizeOfSerialNumberInBytes:
		wRetSize = 2;
		pwBuffer[0] = g_ChipSerialNumber.wRawSizeInBytes;
		break;
    case SerialNoInfoSerialNumber:
	    wRetSize = g_ChipSerialNumber.wRawSizeInBytes;
		j = 0;
	    for(i=(INT)g_ChipSerialNumber.wRawSizeInBytes-1;i>=0;i--)
	    {
	        pwBuffer[j++] =  GetByteFromArrayInY( g_ChipSerialNumber.wRaw, i--)        |
							(GetByteFromArrayInY( g_ChipSerialNumber.wRaw, i--) <<  8) |
							(GetByteFromArrayInY( g_ChipSerialNumber.wRaw, i)   << 16);
	    }
        break;
    default:
        wRetSize = 0;
		break;
    }
   	if(wRetSize)
		return usbmsc_send_command_response(pDev, pLun, pwBuffer, wRetSize);
    else
	    return SCSI_ERROR_DDI_CALL_FAILED;
}
#endif

RETCODE _reentrant UpdaterChipReset(WORD wDeviceNum, WORD _X* pCdb)
{
    g_UpdaterChipReset = TRUE;
    return SUCCESS; 
}

RETCODE _reentrant UpdaterBogusCommand(WORD wDeviceNum, WORD _X* pCdb)
{
    return SUCCESS; 
}


RETCODE _reentrant UpdaterPerformChipReset(void)
{
    // Shut down
    SystemShutdown();

    // Reset the device
    SystemReset();
    
    return SUCCESS; 
}


long GetBigEndianINT64FromByteBufferInX(void _X *pBuffer, int iStartIndex)
{
    long lValue=0;
    int i;
    for(i=0;i<8;i++)
    {
        lValue = (lValue << 8) | GetByteFromArrayInX(pBuffer,iStartIndex+i);
    }
    return lValue;
}

void PutBigEndianINT64IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue)
{
    int i;
    for(i=0;i<8;i++)
    {
        PutByteInArrayInX(pBuffer,iStartIndex + 7 - i,lValue & 0xff);
        lValue=lValue>>8;
    }
}

long GetBigEndianINT32FromByteBufferInX(void _X *pBuffer, int iStartIndex)
{
    long lValue=0;
    int i;
    for(i=0;i<4;i++)
    {
        lValue = (lValue << 8) | GetByteFromArrayInX(pBuffer,iStartIndex+i);
    }
    return lValue;
}

void PutBigEndianINT32IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue)
{
    int i;
    for(i=0;i<4;i++)
    {
        PutByteInArrayInX(pBuffer,iStartIndex + 3 - i,lValue & 0xff);
        lValue=lValue>>8;
    }
}

int GetBigEndianINT16FromByteBufferInX(void _X *pBuffer, int iStartIndex)
{
    int iValue=0;
    int i;
    for(i=0;i<2;i++)
    {
        iValue = (iValue << 8) | GetByteFromArrayInX(pBuffer,iStartIndex+i);
    }
    return iValue;
}

void PutBigEndianINT16IntoByteBufferInX(void _X *pBuffer, int iStartIndex, long lValue)
{
    int i;
    for(i=0;i<2;i++)
    {
        PutByteInArrayInX(pBuffer,iStartIndex + 1 - i,lValue & 0xff);
        lValue=lValue>>8;
    }
}


/*
RETCODE _reentrant MediaInit(WORD wLogMediaNumber)
{ return 0;}
RETCODE _reentrant MediaAllocate(WORD wLogMediaNumber,
    P_MEDIA_ALLOCATION_TABLE pMediaTable)
{ return 0;}
RETCODE _reentrant MediaDiscoverAllocation(WORD wLogMediaNumber)
{ return 0;}
RETCODE _reentrant MediaGetMediaTable(WORD wLogMediaNumber,
    P_MEDIA_ALLOCATION_TABLE *pMediaTable)
{ return 0;}
RETCODE _reentrant MediaGetInfo(WORD wLogMediaNumber, LOGICAL_MEDIA_INFO Type,
    void * _Y pInfo)
{ return 0;}
RETCODE _reentrant MediaErase(WORD wLogMediaNumber, WORD wMagicNumber)
{ return 0;}

RETCODE _reentrant DriveInit(WORD wLogDriveNumber)
{ return 0;}
RETCODE _reentrant DriveGetInfo(WORD wLogDriveNumber, LOGICAL_DRIVE_INFO Type,
    void * pInfo)
{ return 0;}
RETCODE _reentrant DriveSetInfo(WORD wLogDriveNumber, LOGICAL_DRIVE_INFO Type,
    void * pInfo)
{ return 0;}
RETCODE _reentrant DriveMultiWriteSetup(WORD wLogDriveNumber,
    DWORD dwStartSectorNumber, WORD wSectorCount, void * pCallBackFunction)
{ return 0;}
RETCODE _reentrant DriveMultiWriteSector(WORD wLogDriveNumber,
    P_SECTOR_BUFFER pSectorData, LONG lCallBackPassThru)
{ return 0;}
RETCODE _reentrant DriveErase(WORD wLogDriveNumber, WORD wMagicNumber)
{ return 0;}
RETCODE _reentrant DriveFlush(WORD wDriveNumber)
{ return 0;}

RETCODE DdiInitAll(void)
{ return 0;}

*/
