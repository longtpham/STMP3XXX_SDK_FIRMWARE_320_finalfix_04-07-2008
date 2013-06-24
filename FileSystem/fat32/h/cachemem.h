#include "types.h"
#include "fstypes.h"

typedef struct {
	INT CacheValid;
    INT DeviceNum;
	LONG SectorNumber;
	INT WriteAttribute;
	INT Mode;
	INT CacheCounter;
	INT _X *CacheMem;
} tCACHEDESCR;

