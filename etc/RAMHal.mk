#This is the makefile for the MediaTypeRAM DDI.
#

DDIRAMHAL_LIBNAME     =   dd_ram_hal.a

MEDIATYPERAM_PATH       = $(ROOT)\devicedriver\media\MediaTypeRAM
DDIRAM_PATH             = $(MEDIATYPERAM_PATH)\DDI
HALRAM_PATH             = $(MEDIATYPERAM_PATH)\HAL

MEDIATYPERAM_INCLUDEPATH        = $(MEDIATYPERAM_PATH)\include
DDIRAM_INCLUDEPATH              = $(DDIRAM_PATH)\include
HALRAM_INCLUDEPATH              = $(HALRAM_PATH)\include

INC_ALL=$(INC_ALL);$(DDIRAM_INCLUDEPATH);$(MEDIATYPERAM_INCLUDEPATH);$(HALRAM_INCLUDEPATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(MEDIATYPERAM_INCLUDEPATH) -Wc-I$(DDIRAM_INCLUDEPATH) -Wc-I$(HALRAM_INCLUDEPATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDIRAMHALLIB)
                
ifdef D_PLIBS
DDIRAMHALLIB   =   $(OBJ_PATH)\$(DDIRAMHAL_LIBNAME)
else
DDIRAMHALLIB   =   $(LIB_PATH)\$(DDIRAMHAL_LIBNAME)
endif

DDIRAMHALLIST  = \
                $(OBJ_PATH)\RAMHalRawMedia.obj \
                $(OBJ_PATH)\RAMHalRead.obj \
                $(OBJ_PATH)\RAMHalStructures.obj \
                $(OBJ_PATH)\RAMHalWrite.obj

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDIRAMHAL_DEPENDENCIES= \
    "$(DDIRAM_INCLUDEPATH)\RAMDdi.h" \
    "$(HALRAM_INCLUDEPATH)\RAMHal.h" \
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

"$(DDIRAMHALLIB)" : $(DDIRAMHALLIST)
    @echo Creating IEEE archive library file for DriveTypeRAM Media $@
        @$(AR_EXE) -r $(DDIRAMHALLIB) $(DDIRAMHALLIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\RAMHalRawMedia.obj"        : "$(HALRAM_PATH)\RAMHalRawMedia.c" $(DDIRAMHAL_DEPENDENCIES)
"$(OBJ_PATH)\RAMHalRead.obj"            : "$(HALRAM_PATH)\RAMHalRead.c" $(DDIRAMHAL_DEPENDENCIES)
"$(OBJ_PATH)\RAMHalStructures.obj"      : "$(HALRAM_PATH)\RAMHalStructures.c" $(DDIRAMHAL_DEPENDENCIES)
"$(OBJ_PATH)\RAMHalWrite.obj"           : "$(HALRAM_PATH)\RAMHalWrite.c" $(DDIRAMHAL_DEPENDENCIES)

endif
