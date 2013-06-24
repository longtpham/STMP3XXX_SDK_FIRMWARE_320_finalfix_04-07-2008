#This is the makefile for the MediaTypeRAM DDI.
#

DDIRAMDATDATADRIVE_LIBNAME     =   dd_ram_data_drive.a

MEDIATYPERAM_PATH       = $(ROOT)\devicedriver\media\MediaTypeRAM
DDIRAM_PATH             = $(MEDIATYPERAM_PATH)\DDI
DDIRAMDATDATADRIVE_PATH = $(DDIRAM_PATH)\DataDrive
HALRAM_PATH             = $(MEDIATYPERAM_PATH)\HAL

MEDIATYPERAM_INCLUDEPATH        = $(MEDIATYPERAM_PATH)\include
DDIRAM_INCLUDEPATH              = $(DDIRAM_PATH)\include
DDIRAMDATDATADRIVE_INCLUDEPATH  = $(DDIRAMDATDATADRIVE_PATH)\include
HALRAM_INCLUDEPATH              = $(HALRAM_PATH)\include

INC_ALL=$(INC_ALL);$(DDIRAM_INCLUDEPATH);$(MEDIATYPERAM_INCLUDEPATH);$(DDIRAMDATDATADRIVE_INCLUDEPATH);$(HALRAM_INCLUDEPATH);$(DDIRAMDATDATADRIVE_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(MEDIATYPERAM_INCLUDEPATH) -Wc-I$(DDIRAMDATDATADRIVE_INCLUDEPATH) -Wc-I$(DDIRAM_INCLUDEPATH) -Wc-I$(HALRAM_INCLUDEPATH) -Wc-I$(DDIRAMDATDATADRIVE_PATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDIRAMDATDATADRIVELIB)

ifdef D_PLIBS
DDIRAMDATDATADRIVELIB   =   $(OBJ_PATH)\$(DDIRAMDATDATADRIVE_LIBNAME)
else                
DDIRAMDATDATADRIVELIB   =   $(LIB_PATH)\$(DDIRAMDATDATADRIVE_LIBNAME)
endif

DDIRAMDATDATADRIVELIST  = \
                $(OBJ_PATH)\RAMDataDriveInit.obj \
                $(OBJ_PATH)\RAMDataDriveMultiWrite.obj \
                $(OBJ_PATH)\RAMDataDriveRead.obj \
                $(OBJ_PATH)\RAMDataDriveDescriptors.obj \
                $(OBJ_PATH)\RAMDataDriveDescriptorsInit.obj \
                $(OBJ_PATH)\RAMDataDriveWrite.obj

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDIRAMDATDATADRIVE_DEPENDENCIES= \
    "$(DDIRAM_INCLUDEPATH)\RAMDdi.h" \
    "$(DDIRAMDATDATADRIVE_INCLUDEPATH)\RAMDataDrive.h"
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

"$(DDIRAMDATDATADRIVELIB)" : $(DDIRAMDATDATADRIVELIST)
    @echo Creating IEEE archive library file for DriveTypeRAM Data Drive $@
        @$(AR_EXE) -r $(DDIRAMDATDATADRIVELIB) $(DDIRAMDATDATADRIVELIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\RAMDataDriveInit.obj"              : "$(DDIRAMDATDATADRIVE_PATH)\RAMDataDriveInit.c" $(DDIRAMDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMDataDriveMultiWrite.obj"        : "$(DDIRAMDATDATADRIVE_PATH)\RAMDataDriveMultiWrite.c" $(DDIRAMDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMDataDriveRead.obj"              : "$(DDIRAMDATDATADRIVE_PATH)\RAMDataDriveRead.c" $(DDIRAMDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMDataDriveDescriptors.obj"       : "$(DDIRAMDATDATADRIVE_PATH)\RAMDataDriveDescriptors.c" $(DDIRAMDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMDataDriveDescriptorsInit.obj"   : "$(DDIRAMDATDATADRIVE_PATH)\RAMDataDriveDescriptorsInit.asm" $(DDIRAMDATDATADRIVE_DEPENDENCIES)
"$(OBJ_PATH)\RAMDataDriveWrite.obj"             : "$(DDIRAMDATDATADRIVE_PATH)\RAMDataDriveWrite.c" $(DDIRAMDATDATADRIVE_DEPENDENCIES)



endif
