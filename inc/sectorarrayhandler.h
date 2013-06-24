#ifndef __SECTOR_ARRAY_HANDLER_H
#define __SECTOR_ARRAY_HANDLER_H

#define SECTOR_STATE_FREE 0
#define SECTOR_STATE_FILLING 1
#define SECTOR_STATE_FILLED 2

typedef struct
{
    int iState;
    WORD _X *pwData;
} SECTOR_TYPE;



typedef struct SectorArray
{
    SECTOR_TYPE*pSectors;
    INT iNumSectorBuffers;
    WORD wHeadIndex;
    WORD wFillingIndex;
    WORD wTailIndex;
    WORD wSectorsInFlight;
}  SECTOR_ARRAY;



_reentrant SECTOR_TYPE*ReadSectorArrayHandlerGetFreeSector(SECTOR_ARRAY*);
_reentrant void ReadSectorArrayHandlerFreeSector(SECTOR_ARRAY*);
_reentrant void ReadSectorArrayHandlerFreeAllSectors(SECTOR_ARRAY*);

_reentrant SECTOR_TYPE*WriteSectorArrayHandlerGetFreeSector(SECTOR_ARRAY*);
_reentrant SECTOR_TYPE*WriteSectorArrayHandlerGetFilledSector(SECTOR_ARRAY*);
_reentrant void WriteSectorArrayHandlerFillSector(SECTOR_ARRAY*);
_reentrant void WriteSectorArrayHandlerFreeSector(SECTOR_ARRAY*,SECTOR_TYPE*);

_reentrant void SectorArrayInitialize(SECTOR_ARRAY *, WORD wSectorCount,WORD wSectorSize, SECTOR_TYPE *pSectorTypeArray,WORD _X *pSectorDataBuffer);


#endif 