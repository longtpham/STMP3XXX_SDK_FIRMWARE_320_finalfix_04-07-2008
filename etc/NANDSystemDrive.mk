#This is the makefile for the MediaTypeNAND DDI.
#
ifdef D_FUNCLETS
ifdef MTP_ONLY
DDINANDSYSTEMDRIVE_LIBNAME     =   dd_nand_system_drive_mtp_only.a
else
DDINANDSYSTEMDRIVE_LIBNAME     =   dd_nand_system_drive.a
endif           #endif MTP_ONLY
else
ifdef MTP_ONLY
DDINANDSYSTEMDRIVE_LIBNAME     =   dd_nand_system_drive_mtp_only.a
else
DDINANDSYSTEMDRIVE_LIBNAME     =   dd_nand_system_drive.a
endif           #endif MTP_ONLY
endif           #endif FUNCLETS

MEDIATYPENAND_PATH       = $(ROOT)\devicedriver\media\MediaTypeNAND
DDINAND_PATH             = $(MEDIATYPENAND_PATH)\DDI
DDINANDSYSTEMDRIVE_PATH  = $(DDINAND_PATH)\SystemDrive
HALNAND_PATH             = $(MEDIATYPENAND_PATH)\HAL

MEDIATYPENAND_INCLUDEPATH        = $(MEDIATYPENAND_PATH)\include
DDINAND_INCLUDEPATH              = $(DDINAND_PATH)\include
DDINANDSYSTEMDRIVE_INCLUDEPATH   = $(DDINANDSYSTEMDRIVE_PATH)\include
HALNAND_INCLUDEPATH              = $(HALNAND_PATH)\include

INC_ALL=$(INC_ALL);$(DDINAND_INCLUDEPATH);$(MEDIATYPENAND_INCLUDEPATH);$(DDINANDSYSTEMDRIVE_INCLUDEPATH);$(HALNAND_INCLUDEPATH);$(DDINANDSYSTEMDRIVE_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(MEDIATYPENAND_INCLUDEPATH) -Wc-I$(DDINANDSYSTEMDRIVE_INCLUDEPATH) -Wc-I$(DDINAND_INCLUDEPATH) -Wc-I$(HALNAND_INCLUDEPATH) -Wc-I$(DDINANDSYSTEMDRIVE_PATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDINANDSYSTEMDRIVELIB)
                
                
ifdef D_PLIBS
DDINANDSYSTEMDRIVELIB   =   $(OBJ_PATH)\$(DDINANDSYSTEMDRIVE_LIBNAME)
else                
DDINANDSYSTEMDRIVELIB   =   $(LIB_PATH)\$(DEVICE)\$(DDINANDSYSTEMDRIVE_LIBNAME)
endif

DDINANDSYSTEMDRIVELIST  = \
                $(OBJ_PATH)\NANDSystemDriveInit.obj \
                $(OBJ_PATH)\NANDSystemDriveSetInfo.obj \
                $(OBJ_PATH)\NANDSystemDriveGetInfo.obj \
                $(OBJ_PATH)\NANDSystemDriveReadSector.obj \
                $(OBJ_PATH)\NANDSystemDriveWriteSector.obj \
                $(OBJ_PATH)\NANDSystemDriveFlush.obj \
                $(OBJ_PATH)\NANDSystemDriveErase.obj \
                $(OBJ_PATH)\NANDSystemDriveMultiWrite.obj \
                $(OBJ_PATH)\NANDSystemDriveFlush.obj \
                $(OBJ_PATH)\NANDSystemDriveDescInit.obj\

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDINANDSYSTEMDRIVE_DEPENDENCIES= \
    "$(DDINAND_INCLUDEPATH)\NANDDdi.h" \
    "$(DDINANDSYSTEMDRIVE_INCLUDEPATH)\NANDSystemDrive.h" \
    "$(OUTPUT_INCLUDE_PATH)\errors.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsdcdc.h" \
    "$(OUTPUT_INCLUDE_PATH)\regspwm.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsrtc.h"
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

$(DDINANDSYSTEMDRIVELIB) : $(DDINANDSYSTEMDRIVELIST)
    @echo Creating IEEE archive library file for DriveTypeNAND System Drive $@
        @$(AR_EXE) -r $(DDINANDSYSTEMDRIVELIB) $(DDINANDSYSTEMDRIVELIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\NANDSystemDriveInit.obj"              : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveInit.c" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDSystemDriveSetInfo.obj"           : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveSetInfo.c" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDSystemDriveGetInfo.obj"           : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveGetInfo.c" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDSystemDriveReadSector.obj"        : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveReadSector.c" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDSystemDriveWriteSector.obj"       : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveWriteSector.c" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDSystemDriveFlush.obj"             : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveFlush.c" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDSystemDriveErase.obj"             : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveErase.c" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDSystemDriveMultiWrite.obj"        : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveMultiWrite.c" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\NANDSystemDriveFlush.obj"             : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveFlush.c" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)

"$(OBJ_PATH)\NANDSystemDriveDescInit.obj"          : "$(DDINANDSYSTEMDRIVE_PATH)\NANDSystemDriveDescInit.asm" $(DDINANDSYSTEMDRIVE_DEPENDENCIES)

endif
