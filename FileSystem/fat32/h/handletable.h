#include "types.h"
#include "fstypes.h"
typedef struct  
{
INT		HandleActive;
INT		Device;
INT		Mode;
LONG       StartingCluster;
LONG		CurrentOffset;
LONG		CurrentCluster;
LONG		CurrentSector;
INT		BytePosInSector;
INT		SectorPosInCluster;
LONG		DirSector;
INT		DirOffset;
INT		ErrorCode;
LONG		FileSize;
}HANDLETABLE;







