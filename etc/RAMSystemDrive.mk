#This is the makefile for the MediaTypeRAM DDI.
#

DDIRAMSYSTEMDRIVE_LIBNAME     =   dd_ram_system_drive.a

MEDIATYPERAM_PATH       = $(ROOT)\devicedriver\media\MediaTypeRAM
DDIRAM_PATH             = $(MEDIATYPERAM_PATH)\DDI
DDIRAMSYSTEMDRIVE_PATH  = $(DDIRAM_PATH)\SystemDrive
HALRAM_PATH             = $(MEDIATYPERAM_PATH)\HAL

MEDIATYPERAM_INCLUDEPATH        = $(MEDIATYPERAM_PATH)\include
DDIRAM_INCLUDEPATH              = $(DDIRAM_PATH)\include
DDIRAMSYSTEMDRIVE_INCLUDEPATH   = $(DDIRAMSYSTEMDRIVE_PATH)\include
HALRAM_INCLUDEPATH              = $(HALRAM_PATH)\include

INC_ALL=$(INC_ALL);$(DDIRAM_INCLUDEPATH);$(MEDIATYPERAM_INCLUDEPATH);$(DDIRAMSYSTEMDRIVE_INCLUDEPATH);$(HALRAM_INCLUDEPATH);$(DDIRAMSYSTEMDRIVE_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(MEDIATYPERAM_INCLUDEPATH) -Wc-I$(DDIRAMSYSTEMDRIVE_INCLUDEPATH) -Wc-I$(DDIRAM_INCLUDEPATH) -Wc-I$(HALRAM_INCLUDEPATH) -Wc-I$(DDIRAMSYSTEMDRIVE_PATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDIRAMSYSTEMDRIVELIB)

ifdef D_PLIBS
DDIRAMSYSTEMDRIVELIB   =   $(OBJ_PATH)\$(DDIRAMSYSTEMDRIVE_LIBNAME)
else                
DDIRAMSYSTEMDRIVELIB   =   $(LIB_PATH)\$(DDIRAMSYSTEMDRIVE_LIBNAME)
endif

DDIRAMSYSTEMDRIVELIST  = \
                $(OBJ_PATH)\RAMSystemDriveInit.obj \
                $(OBJ_PATH)\RAMSystemDriveMultiWrite.obj \
                $(OBJ_PATH)\RAMSystemDriveRead.obj \
                $(OBJ_PATH)\RAMSystemDriveDescriptors.obj \
                $(OBJ_PATH)\RAMSystemDriveDescInit.obj \
                $(OBJ_PATH)\RAMSystemDriveWrite.obj

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDIRAMSYSTEMDRIVE_DEPENDENCIES= \
    "$(DDIRAM_INCLUDEPATH)\RAMDdi.h" \
    "$(DDIRAMSYSTEMDRIVE_INCLUDEPATH)\RAMSystemDrive.h"
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

"$(DDIRAMSYSTEMDRIVELIB)" : $(DDIRAMSYSTEMDRIVELIST)
    @echo Creating IEEE archive library file for DriveTypeRAM Data Drive $@
        @$(AR_EXE) -r $(DDIRAMSYSTEMDRIVELIB) $(DDIRAMSYSTEMDRIVELIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\RAMSystemDriveInit.obj"              : "$(DDIRAMSYSTEMDRIVE_PATH)\RAMSystemDriveInit.c" $(DDIRAMSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMSystemDriveMultiWrite.obj"        : "$(DDIRAMSYSTEMDRIVE_PATH)\RAMSystemDriveMultiWrite.c" $(DDIRAMSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMSystemDriveRead.obj"              : "$(DDIRAMSYSTEMDRIVE_PATH)\RAMSystemDriveRead.c" $(DDIRAMSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMSystemDriveDescriptors.obj"       : "$(DDIRAMSYSTEMDRIVE_PATH)\RAMSystemDriveDescriptors.c" $(DDIRAMSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMSystemDriveDescInit.obj"          : "$(DDIRAMSYSTEMDRIVE_PATH)\RAMSystemDriveDescInit.asm" $(DDIRAMSYSTEMDRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMSystemDriveWrite.obj"             : "$(DDIRAMSYSTEMDRIVE_PATH)\RAMSystemDriveWrite.c" $(DDIRAMSYSTEMDRIVE_DEPENDENCIES)

endif
