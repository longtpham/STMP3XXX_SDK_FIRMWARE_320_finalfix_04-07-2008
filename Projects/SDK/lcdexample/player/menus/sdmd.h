/*==================================================================================================

                                         MODULE DESCRIPTION

====================================================================================================
                               Sigmatel Inc Confidential Proprietary
                               (c) Copyright Sigmatel Inc 2005, All Rights Reserved

MODULE NAME: sdmd.h        AUTHOR:      CREATION DATE:03/02/2005

PRODUCT NAMES: All

GENERAL DESCRIPTION:

   SD Card Modification Detection

Portability: All


Revision History:

                         Modification        Tracking
Author                       Date             Number           Description of Changes
---------------------    ------------        ----------        -------------------------------------

*/

#ifndef __SDMD_H__
#define __SDMD_H__

/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/
#include "types.h"
#include "platform.h"

/*==================================================================================================
                                           TYPE DEFINITION
==================================================================================================*/
typedef struct sdmd_dir_s {
	long	CurrentCluster;
	int	SectorIndex;
	int	EntryIndex;
} sdmd_dir_t;

/*==================================================================================================
                                           EXTERN FUNCTION
==================================================================================================*/

extern _reentrant void sdmd_InitHash(void);
extern _reentrant RETCODE sdmd_GetBufferHash (INT DeviceNum, LONG Sector);
extern _reentrant RETCODE sdmd_GetDirHash (INT DeviceNum, LONG Cluster);
extern _reentrant LONG sdmd_RootDirHash (INT DeviceNum);
extern _reentrant RETCODE sdmd_GenerateHash (INT DeviceNum);
extern RETCODE _reentrant sdmd_CompareHash (INT DeviceNum, INT b, INT* pPtr);
extern RETCODE _reentrant sdmd_UpdateHash (INT DeviceNum, INT b, INT* pPtr);

#endif /* __SDMD_H__ */
