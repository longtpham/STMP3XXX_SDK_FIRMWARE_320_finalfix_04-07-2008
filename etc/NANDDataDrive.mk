#Filename: NANDDataDrive.mk
#This is the makefile for the MediaTypeNAND DDI.
#

ifdef D_FUNCLETS
DDINANDDATDATADRIVE_LIBNAME     =   dd_nand_data_drive.a
else
DDINANDDATDATADRIVE_LIBNAME     =   dd_nand_data_drive.a
endif

MEDIATYPENAND_PATH       = $(ROOT)\devicedriver\media\MediaTypeNAND
DDINAND_PATH             = $(MEDIATYPENAND_PATH)\DDI
DDINANDDATDATADRIVE_PATH = $(DDINAND_PATH)\DataDrive
HALNAND_PATH             = $(MEDIATYPENAND_PATH)\HAL

MEDIATYPENAND_INCLUDEPATH        = $(MEDIATYPENAND_PATH)\include
DDINAND_INCLUDEPATH              = $(DDINAND_PATH)\include
DDINANDDATDATADRIVE_INCLUDEPATH  = $(DDINANDDATDATADRIVE_PATH)\include
HALNAND_INCLUDEPATH              = $(HALNAND_PATH)\include

INC_ALL=$(INC_ALL);$(DDINAND_INCLUDEPATH);$(MEDIATYPENAND_INCLUDEPATH);$(DDINANDDATDATADRIVE_INCLUDEPATH);$(HALNAND_INCLUDEPATH);$(DDINANDDATDATADRIVE_PATH);$(DDILDL_INCLUDE_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(MEDIATYPENAND_INCLUDEPATH) -Wc-I$(DDINANDDATDATADRIVE_INCLUDEPATH) -Wc-I$(DDINAND_INCLUDEPATH) -Wc-I$(HALNAND_INCLUDEPATH) -Wc-I$(DDINANDDATDATADRIVE_PATH) -Wc-I$(DDILDL_INCLUDE_PATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDINANDDATDATADRIVELIB)

ifdef D_PLIBS                
DDINANDDATDATADRIVELIB   =   $(OBJ_PATH)\$(DDINANDDATDATADRIVE_LIBNAME)
else
DDINANDDATDATADRIVELIB   =   $(LIB_PATH)\$(DEVICE)\$(DDINANDDATDATADRIVE_LIBNAME)
endif

################################################################################

include $(ROOT)\etc\registers.mk

CMP_PATH = $(ROOT)\FileSystem\Fat32\cmp
CMP_STDTYPE_PATH = $(CMP_PATH)\cmp_stdtype
CMP_DEBUG_PATH = $(CMP_PATH)\cmp_debug
CMP_FILE_SYSTEM_PATH = $(CMP_PATH)\cmp_file_system
CMP_MEDIA_NAND_PATH = $(ROOT)\DeviceDriver\Media\cmp\cmp_media_nand

INC_ALL_C =\
  $(INC_ALL_C)\
  -Wc-I$(INC_FS)\
  -Wc-I$(CMP_STDTYPE_PATH)\
  -Wc-I$(CMP_DEBUG_PATH)\
  -Wc-I$(CMP_FILE_SYSTEM_PATH)\
  -Wc-I$(CMP_MEDIA_NAND_PATH)

ifndef PLAYER
#include the perf enhancement switch for all non player DDI builds. Player had mem limits.
# Since standard.mk does not use C_ASM_OPTS directly, & since they are included into CC_OPTS  
# in several apps, we currently only need to define this for CC_OPTS (avoids redef warnings).
#C_ASM_OPTS=$(C_ASM_OPTS) -Wa-Dk_opt_performance_enhancement
CC_OPTS=$(CC_OPTS) -Wc-Dk_opt_performance_enhancement
endif

################################################################################

DDINANDDATDATADRIVELIST  = \
                $(OBJ_PATH)\NANDDataDriveStubs.obj \
                $(OBJ_PATH)\nddFlush.obj \
                $(OBJ_PATH)\NANDDataDriveInit.obj \
                $(OBJ_PATH)\NANDDataDriveWriteSector.obj \
                $(OBJ_PATH)\NANDDataDriveMultiWriteCache.obj \
                $(OBJ_PATH)\NANDDataDriveReadSector.obj \
                $(OBJ_PATH)\NANDDataDriveDescriptorsInit.obj \
                $(OBJ_PATH)\NANDDataDriveGetInfo.obj \
                $(OBJ_PATH)\NANDDataDriveSetInfo.obj \
                $(OBJ_PATH)\NANDDataDriveErase.obj \
                $(OBJ_PATH)\nddAddBlkToBBTbl.obj \
                $(OBJ_PATH)\nddConvertLogicalToRelative.obj \
                $(OBJ_PATH)\nddConvertRelativeToPhysical.obj \
                $(OBJ_PATH)\nddGetFreeErasedRBlock.obj \
                $(OBJ_PATH)\nddLinConvertRelativeToPhys.obj \
                $(OBJ_PATH)\nddLinGetFreeErasedRBlock.obj \
                $(OBJ_PATH)\nddLookUpRBlock.obj \
                $(OBJ_PATH)\nddMpConvertRelativeToPhys.obj \
                $(OBJ_PATH)\nddMpGetFreeErasedRBlock.obj \
                $(OBJ_PATH)\nddRecoverBadHBlock.obj \
                $(OBJ_PATH)\nddRecoverBadRBlock.obj \
                $(OBJ_PATH)\nddSearchBBTblForPhysBlock.obj \
                $(OBJ_PATH)\nddStartNewUpdate.obj \
                $(OBJ_PATH)\nddLookupUBlock.obj \
                $(OBJ_PATH)\nddLookupUSector.obj \
                $(OBJ_PATH)\nddKillUpdate.obj \
                $(OBJ_PATH)\nddAddUSector.obj \
                $(OBJ_PATH)\nddGetUSectorsMap.obj \
                $(OBJ_PATH)\nddCloseUpdate.obj \
                $(OBJ_PATH)\nddUSectorsMapsUtils.obj \
                $(OBJ_PATH)\nddUBlkUtils.obj \
                $(OBJ_PATH)\nddGetFreeErasedUBlock.obj \
                $(OBJ_PATH)\nddGetFreeErasedUBlock.obj \
                $(OBJ_PATH)\nddRecoverBadUBlock.obj \
                $(OBJ_PATH)\nddKillUSector.obj \
                $(OBJ_PATH)\nddProtectUBlocks.obj \
                $(OBJ_PATH)\nddGetRecycledUBlock.obj \
        $(OBJ_PATH)\nddCopySectors.obj \
        $(OBJ_PATH)\nddFgGC.obj \
        $(OBJ_PATH)\nddFindFreeUBlock.obj \
        $(OBJ_PATH)\nddFreshenUpdate.obj \
        $(OBJ_PATH)\nddBgGC.obj \
        $(OBJ_PATH)\nddkillUSectorsMap.obj \
        $(OBJ_PATH)\nddWriteSector.obj \
        $(OBJ_PATH)\ndd_write_sector_in_place.obj \
        $(OBJ_PATH)\ndd_pin_allocation.obj





#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDINANDDATDATADRIVE_DEPENDENCIES= \
    "$(DDINAND_INCLUDEPATH)\NANDDdi.h" \
    "$(DDINANDDATDATADRIVE_INCLUDEPATH)\NANDDataDrive.h" \
    "$(OUTPUT_INCLUDE_PATH)\errors.h"\
    "$(OUTPUT_INCLUDE_PATH)\regsicoll.h" 
    
# The SDK projects do not include sources for the libs, so the ifdef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

$(DDINANDDATDATADRIVELIB) : $(DDINANDDATDATADRIVELIST)
    @echo Creating IEEE archive library file for DriveTypeNAND Data Drive $@
        @$(AR_EXE) -r $(DDINANDDATDATADRIVELIB) $(DDINANDDATDATADRIVELIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\NANDDataDriveStubs.obj"             : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveStubs.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddFlush.obj"                       : "$(DDINANDDATDATADRIVE_PATH)\nddFlush.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveInit.obj"              : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveInit.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveWriteSector.obj"       : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveWriteSector.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveMultiWrite.obj"        : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveMultiWrite.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveMultiWriteCache.obj"   : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveMultiWriteCache.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveDescriptors.obj"       : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveDescriptors.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveDescriptorsInit.obj"   : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveDescriptorsInit.asm" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveWrite.obj"             : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveWrite.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveReadSector.obj"        : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveReadSector.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveGetInfo.obj"           : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveGetInfo.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveSetInfo.obj"           : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveSetInfo.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDDataDriveErase.obj"             : "$(DDINANDDATDATADRIVE_PATH)\NANDDataDriveErase.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)

"$(OBJ_PATH)\nddStartNewUpdate.obj"              : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddStartNewUpdate.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddLookupUBlock.obj"                : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddLookupUBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddLookupUSector.obj"               : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddLookupUSector.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddKillUpdate.obj"                  : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddKillUpdate.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddAddUSector.obj"                  : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddAddUSector.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddGetUSectorsMap.obj"              : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddGetUSectorsMap.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddCloseUpdate.obj"                 : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddCloseUpdate.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddUSectorsMapsUtils.obj"           : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddUSectorsMapsUtils.asm" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddUBlkUtils.obj"                   : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddUBlkUtils.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddGetFreeErasedUBlock.obj"         : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddGetFreeErasedUBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddRecoverBadUBlock.obj"            : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddRecoverBadUBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddKillUSector.obj"                 : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddKillUSector.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddProtectUBlocks.obj"              : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddProtectUBlocks.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddGetRecycledUBlock.obj"           : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddGetRecycledUBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddkillUSectorsMap.obj"             : "$(DDINANDDATDATADRIVE_PATH)\UpdateBlocks\nddkillUSectorsMap.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddCopySectors.obj"                 : "$(DDINANDDATDATADRIVE_PATH)\nddCopySectors.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddWriteSector.obj"                 : "$(DDINANDDATDATADRIVE_PATH)\nddWriteSector.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\ndd_write_sector_in_place.obj"      : "$(DDINANDDATDATADRIVE_PATH)\ndd_write_sector_in_place.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\ndd_pin_allocation.obj"             : "$(DDINANDDATDATADRIVE_PATH)\ndd_pin_allocation.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)


"$(OBJ_PATH)\nddAddBlkToBBTbl.obj"              : "$(DDINANDDATDATADRIVE_PATH)\nddAddBlkToBBTbl.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddConvertLogicalToRelative.obj"   : "$(DDINANDDATDATADRIVE_PATH)\nddConvertLogicalToRelative.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddConvertRelativeToPhysical.obj"  : "$(DDINANDDATDATADRIVE_PATH)\nddConvertRelativeToPhysical.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddGetFreeErasedRBlock.obj"        : "$(DDINANDDATDATADRIVE_PATH)\nddGetFreeErasedRBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddLinConvertRelativeToPhys.obj"   : "$(DDINANDDATDATADRIVE_PATH)\nddLinConvertRelativeToPhys.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddLinGetFreeErasedRBlock.obj"     : "$(DDINANDDATDATADRIVE_PATH)\nddLinGetFreeErasedRBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddLookUpRBlock.obj"               : "$(DDINANDDATDATADRIVE_PATH)\nddLookUpRBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddMpConvertRelativeToPhys.obj"    : "$(DDINANDDATDATADRIVE_PATH)\nddMpConvertRelativeToPhys.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddMpGetFreeErasedRBlock.obj"      : "$(DDINANDDATDATADRIVE_PATH)\nddMpGetFreeErasedRBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddRecoverBadHBlock.obj"           : "$(DDINANDDATDATADRIVE_PATH)\nddRecoverBadHBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddRecoverBadRBlock.obj"           : "$(DDINANDDATDATADRIVE_PATH)\nddRecoverBadRBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddSearchBBTblForPhysBlock.obj"    : "$(DDINANDDATDATADRIVE_PATH)\nddSearchBBTblForPhysBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)

"$(OBJ_PATH)\nddFgGC.obj"                       : "$(DDINANDDATDATADRIVE_PATH)\GarbageCollector\nddFgGC.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddFindFreeUBlock.obj"             : "$(DDINANDDATDATADRIVE_PATH)\GarbageCollector\nddFindFreeUBlock.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddBgGC.obj"                       : "$(DDINANDDATDATADRIVE_PATH)\GarbageCollector\nddBgGC.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\nddFreshenUpdate.obj"              : "$(DDINANDDATDATADRIVE_PATH)\GarbageCollector\nddFreshenUpdate.c" $(DDINANDDATDATADRIVE_DEPENDENCIES)


endif





