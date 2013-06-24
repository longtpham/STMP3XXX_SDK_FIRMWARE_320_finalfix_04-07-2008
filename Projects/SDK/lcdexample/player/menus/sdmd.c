/*================================================================================================                                         MODULE DESCRIPTION

===================================================================================================                               Perception Digital Confidential Proprietary
                               (c) Copyright Sigmatel Inc 2005, All Rights Reserved

MODULE NAME: sdmd.c        AUTHOR:      CREATION DATE:02/02/2005

PRODUCT NAMES: All

GENERAL DESCRIPTION:

   SD Card Modification Detection

Portability: All


Revision History:

                         Modification        Tracking
Author                       Date             Number           Description of Changes
---------------------    ------------        ----------        -------------------------------------

*/
/*=================================================================================================                                           INCLUDE FILES
==================================================================================================*/
#include "types.h"
#include "platform.h"
#include "extern.h"
#include "filesystem.h"
#include "fserror.h"
#include "sdmd.h"

// added 05/03/17 for music.lib file
#define COMP_OPT _reentrant
#include "..\..\..\..\..\..\System\MsgModules\Software\MusicLib\hdr\musiclib_flash_function.h"
//////

extern RETCODE _reentrant DataDriveReadSector(DWORD dwSectorNumber, WORD wDriveNumber, INT iType, SECTOR_BUFFER * Buffer);
extern RETCODE _reentrant DataDriveWriteSector(WORD wDriveNumber, DWORD dwSectorNumber, SECTOR_BUFFER * Buffer, INT iType);
extern _reentrant void get_map_sec(uint24 Media_type, uint24 dbsec_id, LONG *sec_id);


#define READ_TYPE_RANDOM 0

#define HASH_KEY_SIZE	4
#define HASH_KEY_MASK   0x03
#define HASH_SEQ	191
//#define HASH_AREA1	0x34000
//#define HASH_AREA2	0x34200
#define HASH_PER_SECTOR	42		// floor of 512/3/HASH_KEY_SIZE
#define HASH_SEQ_MASK	0xBFBFBF	// 191,191,191
#define BUFFER_SIZE	171
#define SECTOR_BUFFER_SIZE	683
#define MAX_DIR_LEVEL	15
#define NAND_DRIVE		0
#define HASH_MAX_SECTOR	50

unsigned int	g_sdmd_Skew;
unsigned int	g_sdmd_hash[HASH_KEY_SIZE];
//unsigned int _X	g_sdmd_Buffer[SECTOR_BUFFER_SIZE+5];	// avoid memory corruption bug in Sigmatel SDK
//unsigned int _X	g_sdmd_hashlist[SECTOR_BUFFER_SIZE+5];	// avoid memory corruption bug in Sigmatel SDK
unsigned int _X	g_sdmd_Buffer[SECTOR_BUFFER_SIZE+24];	// avoid memory corruption bug in Sigmatel SDK
unsigned int _X	g_sdmd_hashlist[SECTOR_BUFFER_SIZE+24];	// avoid memory corruption bug in Sigmatel SDK
sdmd_dir_t	g_sdmd_dir[MAX_DIR_LEVEL];
int		g_sdmd_current_level;
int		g_sdmd_HashOffset;
int		g_sdmd_CurLoc;
int		g_sdmd_error;

_reentrant void sdmd_InitHash(void) {
	int i;
	
	g_sdmd_Skew = 0;
	for (i=0; i<HASH_KEY_SIZE; i++) {
		g_sdmd_hash[i] = 0x000000;
	}
}

_reentrant RETCODE sdmd_GetBufferHash (INT DeviceNum, LONG Sector) {

	int i;	
	RETCODE status;

	// Pre-pack a zero at the end of read sector
	g_sdmd_Buffer[BUFFER_SIZE-1] = 0; 
	// read each sector in cluster and generate hash
	status = DataDriveReadSector(Sector, DeviceNum, (INT)READ_TYPE_RANDOM, g_sdmd_Buffer);
	if (status!=SUCCESS) {
		g_sdmd_error++;
		return status;
	}
	
	// compute the hash value
	for (i=0; i<BUFFER_SIZE; i++) {
		g_sdmd_hash[(i&HASH_KEY_MASK)] ^= (g_sdmd_Buffer[i]+g_sdmd_Skew);
		g_sdmd_Skew += 0x010101;
		if (g_sdmd_Skew==HASH_SEQ_MASK) {
			g_sdmd_Skew = 0x000000;
		}
	}
	return SUCCESS;
}

_reentrant RETCODE sdmd_GetDirHash (INT DeviceNum, LONG Cluster) {
	LONG Sector;
	LONG NumRootSectors;
	int i;
	RETCODE status;

	sdmd_InitHash();	
	status = SUCCESS;

	if (Cluster>0) {
		// If FAT type is FAT32 then find the first root directory cluster from media tabel 
		// and then find the sector no
		// Compute hash value of directory of directory
		do {
			Sector = Firstsectorofcluster(DeviceNum,Cluster);
			for (i=0; i< MediaTable[DeviceNum].SectorsPerCluster; i++) {
				// compute the hash value
				status = sdmd_GetBufferHash (DeviceNum, Sector+i);
				if (status!=SUCCESS) {
					g_sdmd_error++;
					return status;
				}
			}
			Cluster = Findnextcluster(DeviceNum, Cluster);
		} while ((Cluster!=FS_EOF) && (Cluster!=NOT_VALID_SECTOR) && (status==SUCCESS));
	} else {
		// If FAT type is FAT12 or FAT16  then  root directory starts after reserved sector 
		// and primary and secondary FAT table
		// Special case: FAT12 / FAT16 root directory is not in cluster format
		Sector = MediaTable[DeviceNum].RsvdSectors +  (MediaTable[DeviceNum].NoOfFATs * MediaTable[DeviceNum].FATSize);
		NumRootSectors = MediaTable[DeviceNum].MaxRootDirEntries >> 5;

		// scan through the entire root directory (even if it's empty)
		for (i=0; i<NumRootSectors; i++) {
			status = sdmd_GetBufferHash (DeviceNum, Sector+i);
			if (status!=SUCCESS) {
				g_sdmd_error++;
				return status;
			}
		}
	}
		
	return SUCCESS;
}

_reentrant LONG sdmd_RootDirHash (INT DeviceNum) {
	LONG Cluster = 0;
	RETCODE status;
	if(DeviceNum > maxdevices || DeviceNum < 0)
		return FS_MAX_DEVICES_EXCEEDED;

	if (MediaTable[DeviceNum].FATType == FAT32)
	{
		// Compute the hash of directory starting at Cluster
		Cluster = MediaTable[DeviceNum].RootdirCluster;
		status = sdmd_GetDirHash (DeviceNum, Cluster);
		if (status!=SUCCESS) {
			g_sdmd_error++;
		}
	}
	else if(MediaTable[DeviceNum].FATType ==FAT12 ||  MediaTable[DeviceNum].FATType ==FAT16)
	{
		// Compute the hash of directory for FAT12/FAT16 system
		status = sdmd_GetDirHash (DeviceNum, Cluster);
		if (status!=SUCCESS) {
			g_sdmd_error++;
		}
	}

	else
	   return  MEDIA_TYPE_NOT_SUPPORTED;

	return (Cluster);
}

_reentrant RETCODE sdmd_GenerateHash (INT DeviceNum) {
	int i, isfree, attr, val, ExitRecursive, index;
	long cluster, sector;
	long NumRootSectors;
	RETCODE status;
	uint24 temp_sec;
	long	LibSector;

	// erase the first hash sector so that when sdmd_GenerateHash return error, the 
	// CompareHash function will correctly detect the media has changed, Astley 13/3/2005 16:41
	sector = HASH_AREA2;
	temp_sec = sector;
	get_map_sec(0x01, temp_sec, &LibSector);
	status = DataDriveWriteSector(NAND_DRIVE, LibSector, g_sdmd_hashlist, (INT)WRITE_TYPE_RANDOM);	
	if (status!=SUCCESS) {
		g_sdmd_error++;
		return status;
	}

	g_sdmd_error = 0;
	g_sdmd_current_level = 0;
	g_sdmd_dir[0].SectorIndex = 0;
	g_sdmd_dir[0].EntryIndex = 0;
	g_sdmd_dir[0].CurrentCluster = sdmd_RootDirHash(DeviceNum);

	if (g_sdmd_dir[0].CurrentCluster == NOT_VALID_SECTOR || g_sdmd_error!=0) {
		return NOT_VALID_SECTOR;
	}

	for (i=0; i<BUFFER_SIZE; i++) {
		g_sdmd_hashlist[i] = 0x000000;
	}
	
	NumRootSectors = MediaTable[DeviceNum].MaxRootDirEntries >> 5;
	g_sdmd_HashOffset = 0;
	g_sdmd_CurLoc = 1;
	for (i=0; i<HASH_KEY_SIZE; i++) {
		g_sdmd_hashlist[i] = g_sdmd_hash[i];
	}
	
	if (g_sdmd_dir[0].CurrentCluster==0) {
		sector = MediaTable[DeviceNum].RsvdSectors +  (MediaTable[DeviceNum].NoOfFATs * MediaTable[DeviceNum].FATSize);
	} else {
		sector = Firstsectorofcluster(DeviceNum, g_sdmd_dir[0].CurrentCluster);
	}
	status = DataDriveReadSector(sector, DeviceNum, (INT)READ_TYPE_RANDOM, g_sdmd_Buffer);
	if (status!=SUCCESS) {
		g_sdmd_error++;
		return status;
	}
	
	// __asm("debug");
	while ((g_sdmd_current_level >= 0) && (g_sdmd_HashOffset < HASH_MAX_SECTOR)) {
		// scan for directory
		index = g_sdmd_dir[g_sdmd_current_level].EntryIndex * 32;
		isfree = FSGetByte((void*)g_sdmd_Buffer, index, X_MEMORY);
		attr = FSGetByte((void*)g_sdmd_Buffer, index+11, X_MEMORY);

		// __asm("debug");
		if ((isfree!=0xe5) && ((attr&0x10)==0x10) && (attr!=0x0f) && (g_sdmd_current_level < MAX_DIR_LEVEL)) {
			val = FSGetByte((void*)g_sdmd_Buffer, index+21, X_MEMORY);
			cluster = val;			
			val = FSGetByte((void*)g_sdmd_Buffer, index+20, X_MEMORY);
			cluster = (cluster << 8) + val;
			val = FSGetByte((void*)g_sdmd_Buffer, index+27, X_MEMORY);
			cluster = (cluster << 8) + val;
			val = FSGetByte((void*)g_sdmd_Buffer, index+26, X_MEMORY);
			cluster = (cluster << 8) + val;
			
			// a directory found increment level when we found one
			g_sdmd_current_level++;
			g_sdmd_dir[g_sdmd_current_level].SectorIndex = 0;
			// skip two (as the following code will increase EntryIndex by 1) entry as it will be "." and ".."
			g_sdmd_dir[g_sdmd_current_level].EntryIndex = 1;
			g_sdmd_dir[g_sdmd_current_level].CurrentCluster = cluster;
			
			// __asm("debug");
			
			cluster = sdmd_GetDirHash(DeviceNum, cluster);
			if (cluster == NOT_VALID_SECTOR) {
				return NOT_VALID_SECTOR;
			}
			
			index = g_sdmd_CurLoc*HASH_KEY_SIZE;
			for (i=0; i<HASH_KEY_SIZE; i++) {
				g_sdmd_hashlist[index + i] = g_sdmd_hash[i];
			}

			g_sdmd_CurLoc++;
			if (g_sdmd_CurLoc>HASH_PER_SECTOR) {
				// write to disk
				sector = HASH_AREA2 + g_sdmd_HashOffset;
				temp_sec = sector;
				get_map_sec(0x01, temp_sec, &LibSector);
				status = DataDriveWriteSector(NAND_DRIVE, LibSector, g_sdmd_hashlist, (INT)WRITE_TYPE_RANDOM);	
				if (status!=SUCCESS) {
					g_sdmd_error++;
					return status;
				}
				g_sdmd_CurLoc = 0;
				for (i=0; i<BUFFER_SIZE; i++) {
					g_sdmd_hashlist[i] = 0x000000;
				}

				// erase the next hash sector so that when CompareHash return an error, 
				// the compare hash can still correctly detect media change, Astley 13/3/2005 16:43				
				sector++;
				temp_sec = sector;
				get_map_sec(0x01, temp_sec, &LibSector);
				status = DataDriveWriteSector(NAND_DRIVE, LibSector, g_sdmd_hashlist, (INT)WRITE_TYPE_RANDOM);	
				if (status!=SUCCESS) {
					g_sdmd_error++;
					return status;
				}
				
				g_sdmd_HashOffset++;
			}
			
			sector = Firstsectorofcluster(DeviceNum, cluster);
			status = DataDriveReadSector(sector, DeviceNum, (INT)READ_TYPE_RANDOM, g_sdmd_Buffer);
			if (status!=SUCCESS) {
				g_sdmd_error++;
				return status;
			}
			// __asm("debug");
		}

		do {
			ExitRecursive = 0;
			g_sdmd_dir[g_sdmd_current_level].EntryIndex++;
			if (g_sdmd_dir[g_sdmd_current_level].EntryIndex >= 512/32) {
				// advance to next sector
				g_sdmd_dir[g_sdmd_current_level].EntryIndex = 0;
				g_sdmd_dir[g_sdmd_current_level].SectorIndex++;
				if (g_sdmd_current_level == 0 && MediaTable[DeviceNum].FATType != FAT32) {
					// we're at root directory, special handle for FAT16/FAT12
					if (g_sdmd_dir[g_sdmd_current_level].SectorIndex >= NumRootSectors) {
						// we're at the end of the directory, go back one level
						g_sdmd_current_level--;
						continue;
					}
					// first root sector + index
					sector = MediaTable[DeviceNum].RsvdSectors +  (MediaTable[DeviceNum].NoOfFATs * MediaTable[DeviceNum].FATSize) + g_sdmd_dir[g_sdmd_current_level].SectorIndex;
					status = DataDriveReadSector(sector, DeviceNum, (INT)READ_TYPE_RANDOM, g_sdmd_Buffer);
					if (status!=SUCCESS) {
						g_sdmd_error++;
						return status;
					}
					// __asm("debug");
				} else {
					// using cluster concept to deal with the directory structure
					if (g_sdmd_dir[g_sdmd_current_level].SectorIndex >= MediaTable[DeviceNum].SectorsPerCluster) {
						// advance to next cluster
						g_sdmd_dir[g_sdmd_current_level].SectorIndex = 0;
						cluster = Findnextcluster(DeviceNum, g_sdmd_dir[g_sdmd_current_level].CurrentCluster);
						if (cluster == FS_EOF) {
							// we're at the end of the directory, go back one level
							g_sdmd_current_level--;
							ExitRecursive = 1;
				continue;
						}
						g_sdmd_dir[g_sdmd_current_level].CurrentCluster = cluster;
					}
					sector = Firstsectorofcluster(DeviceNum, g_sdmd_dir[g_sdmd_current_level].CurrentCluster) + g_sdmd_dir[g_sdmd_current_level].SectorIndex;
					status = DataDriveReadSector(sector, DeviceNum, (INT)READ_TYPE_RANDOM, g_sdmd_Buffer);
					if (status!=SUCCESS) {
						g_sdmd_error++;
						return status;
					}
				}
			} else {
				if (g_sdmd_current_level == 0 && MediaTable[DeviceNum].FATType != FAT32) {
					// first root sector + index
					sector = MediaTable[DeviceNum].RsvdSectors +  (MediaTable[DeviceNum].NoOfFATs * MediaTable[DeviceNum].FATSize) + g_sdmd_dir[g_sdmd_current_level].SectorIndex;
					status = DataDriveReadSector(sector, DeviceNum, (INT)READ_TYPE_RANDOM, g_sdmd_Buffer);
					if (status!=SUCCESS) {
						g_sdmd_error++;
						return status;
					}
				} else {
					sector = Firstsectorofcluster(DeviceNum, g_sdmd_dir[g_sdmd_current_level].CurrentCluster) + g_sdmd_dir[g_sdmd_current_level].SectorIndex;
					status = DataDriveReadSector(sector, DeviceNum, (INT)READ_TYPE_RANDOM, g_sdmd_Buffer);
					if (status!=SUCCESS) {
						g_sdmd_error++;
						return status;
					}
				}
			}
		} while (ExitRecursive==1 && (g_sdmd_current_level >= 0));
	}

	if (g_sdmd_CurLoc>0) {
		// write to disk
		sector = HASH_AREA2 + g_sdmd_HashOffset;
		temp_sec = sector;
		get_map_sec(0x01, temp_sec, &LibSector);
		status = DataDriveWriteSector(NAND_DRIVE, LibSector, g_sdmd_hashlist, (INT)WRITE_TYPE_RANDOM);
		if (status!=SUCCESS) {
			g_sdmd_error++;
			return status;
		}
		g_sdmd_CurLoc = 0;
		for (i=0; i<BUFFER_SIZE; i++) {
			g_sdmd_hashlist[i] = 0x000000;
		}
		g_sdmd_HashOffset++;
	}
	
	return SUCCESS;
}

RETCODE _reentrant sdmd_CompareHash (INT DeviceNum, INT b, INT* pPtr)
{
	int i, j, k, l;
	long sector;
	RETCODE status;
	uint24 temp_sec;
	long	LibSector;
	
	if (sdmd_GenerateHash(DeviceNum)!=SUCCESS) {
		return -1;
	}
	for (i=0; i<g_sdmd_HashOffset; i++) {
		sector = HASH_AREA1+i;
		temp_sec = sector;
		get_map_sec(0x01, temp_sec, &LibSector);
		status = DataDriveReadSector(LibSector, NAND_DRIVE, (INT)READ_TYPE_RANDOM, g_sdmd_Buffer);
		if (status!=SUCCESS) {
			g_sdmd_error++;
			return -1;
		}
		sector = HASH_AREA2+i;
		temp_sec = sector;
		get_map_sec(0x01, temp_sec, &LibSector);
		status = DataDriveReadSector(LibSector, NAND_DRIVE, (INT)READ_TYPE_RANDOM, g_sdmd_hashlist);
		if (status!=SUCCESS) {
			g_sdmd_error++;
			return -1;
		}
		for (j=0; j<HASH_PER_SECTOR*HASH_KEY_SIZE; j++) {
			if (g_sdmd_Buffer[j]!=g_sdmd_hashlist[j]) {
				return 1;
			}
		}
	}
	return SUCCESS;
}

RETCODE _reentrant sdmd_UpdateHash (INT DeviceNum, INT b, INT* pPtr)
{
	int i;
	long sector;
	uint24 temp_sec;
	long	LibSector;
	RETCODE status;
	
	for (i=0; i<g_sdmd_HashOffset; i++) {
		sector = HASH_AREA2+i;
		temp_sec = sector;
		get_map_sec(0x01, temp_sec, &LibSector);
		status = DataDriveReadSector(LibSector, NAND_DRIVE, (INT)READ_TYPE_RANDOM, g_sdmd_Buffer);
		if (status!=SUCCESS) {
			g_sdmd_error++;
			return -1;
		}
		sector = HASH_AREA1+i;
		temp_sec = sector;
		get_map_sec(0x01, temp_sec, &LibSector);
		status = DataDriveWriteSector(NAND_DRIVE, LibSector, g_sdmd_Buffer, (INT)WRITE_TYPE_RANDOM);
		if (status!=SUCCESS) {
			g_sdmd_error++;
			return -1;
		}
	}
	return 0;
}
