#ifndef _FS_TYPE_H_
#define _FS_TYPE_H_

#include   "types.h"

// move FSMEDIA_TABLE from devicetable.h  15Apr2005   First moved in SDK2.6.
typedef struct
{

INT	_Y BytesPerSector;
INT	_Y SectorsPerCluster;
INT	_Y RsvdSectors;
INT	_Y NoOfFATs;
INT	_Y MaxRootDirEntries;
LONG	_Y TotalSectors;
LONG	_Y FATSize;
LONG	_Y RootdirCluster;
//INT	_Y FSInfoSector;
//INT	_Y BkBootSector;
LONG	_Y NextFreeCluster;
LONG	_Y TotalFreeClusters;
INT	_Y RootDirSectors;
INT	_Y FIRSTDataSector;
INT    _Y FATType;
LONG   _Y TotalNoofclusters;
INT    _Y ClusterMask;
INT    _Y ClusterShift;
INT    _Y SectorShift;
INT    _Y SectorMask;
INT    _Y DevicePresent;
LONG   _Y FirRootdirsec;
INT		_Y FSInfoSector;
}FSMEDIA_TABLE;


#define 	MAXDEVICES 		2
//#define 	NUMCACHES 		8  // this is already in fsproj.h (2 for player 2 for mtp as of 28jun2005) TOVERIFY 2 ok for player and mtp. 

// NOTE:  This offset is the same no matter what the sector actual size!  
//        TOVERIFY 3.0 doesn't have this defined but uses it in filesystempresent.c. lbmlc def'd it here so I insert it here.
#define 	FATFS_SIGNATURE_OFFSET 	510
#define 	BOOTSECTOR		0
#define     FSINFOSECTOR	1

#define     READ_MODE           1
#define     WRITE_MODE          2
#define     APPEND_MODE         4
#define     SEQ_WRITE_MODE      8
#define     DIRECTORY_MODE	   16
#define     CREATE_MODE        32

#define     RPLUS               5
#define     WPLUS          	    6
#define     APLUS               7



#define     X_MEMORY            0
#define     Y_MEMORY            2
#define     P_MEMORY            4

#define     FAT12               0 
#define     FAT16               1   
#define     FAT32               2 


#define FAT12EOF            0x0FFF
#define FAT16EOF            0xFFFF
#define FAT32EOF            0x0FFFFFFF



#define FAT12FREECX         0x000
#define FAT16FREECX         0x0000
#define FAT32FREECX         0x00000000


#define	 DBCS               1
#define	 UNICODE            2


#define     CREATION_DATE       1
#define     CREATION_TIME       2
#define     MODIFICATION_DATE   3
#define     MODIFICATION_TIME   4


#define     READ_ONLY      0X01
#define     HIDDEN         0X02
#define     SYSTEM         0X04
#define     VOLUME_ID      0X08
#define     DIRECTORY      0X10
#define     ARCHIVE        0X20

#define READCOUNTER         105
#define WRITECOUNTER        100
#define FLUSHCOUNTER        200


#define DEFAULT_MEMORY      Y_MEMORY

#define  CWD_HANDLE           0
#define  DIRECTORY_HANDLE     1
#define  FIRST_VALID_HANDLE   2
#define  END_OF_DIR_PATH      3

//Constants for Sector read and write (Normal and FAT 
#define		NORMALTYPE		0
#define		FATTYPE			1
#define     RAWTYPE         2

#define  SHORTNAMERES_CH      6
#define  LONGNAMERES_CH       9
#define  MAXFILENAME_CH       260

#define VOLUME_TYPE          0
#define DIR_TYPE             1
#define FILE_TYPE            2
					   
#define WRITE_TYPE_RANDOM		0
#define WRITE_TYPE_SEQ_FIRST	1
#define	WRITE_TYPE_SEQ_NEXT		2
#define WRITE_TYPE_NOREADBACK   3
		  

#define     HANDLEENTRYSIZE         19

// DEVICERECORDSIZE is now only defined in cachedef.h so deleted from here in first 3.1 prelim 

#define     CACHEDESCRSIZE          8
#define     CACHEBUFSIZE            705

#define     UCS2s                     0
#define     UCS3s                     1

#define     FAT32FSIFREECOUNTSIZE	4

#endif // _FS_TYPE_H_

