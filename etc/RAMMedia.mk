ifndef RAMMEDIA_MK
RAMMEDIA_MK = TRUE
#This is the makefile for the MediaTypeRAM DDI.
#

DDIRAMMEDIA_LIBNAME     =   dd_ram_media.a

MEDIATYPERAM_PATH       = $(ROOT)\devicedriver\media\MediaTypeRAM
DDIRAM_PATH             = $(MEDIATYPERAM_PATH)\DDI
DDIRAMMEDIA_PATH        = $(DDIRAM_PATH)\Media
HALRAM_PATH             = $(MEDIATYPERAM_PATH)\HAL

MEDIATYPERAM_INCLUDEPATH        = $(MEDIATYPERAM_PATH)\include
DDIRAM_INCLUDEPATH              = $(DDIRAM_PATH)\include
DDIRAMMEDIA_INCLUDEPATH         = $(DDIRAMMEDIA_PATH)\include
HALRAM_INCLUDEPATH              = $(HALRAM_PATH)\include

INC_ALL=$(INC_ALL);$(DDIRAM_INCLUDEPATH);$(MEDIATYPERAM_INCLUDEPATH);$(DDIRAMMEDIA_INCLUDEPATH);$(HALRAM_INCLUDEPATH);$(DDIRAMMEDIA_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(MEDIATYPERAM_INCLUDEPATH) -Wc-I$(DDIRAMMEDIA_INCLUDEPATH) -Wc-I$(DDIRAM_INCLUDEPATH) -Wc-I$(HALRAM_INCLUDEPATH) -Wc-I$(DDIRAMMEDIA_PATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDIRAMMEDIALIB)

ifdef D_PLIBS
DDIRAMMEDIALIB   =   $(OBJ_PATH)\$(DDIRAMMEDIA_LIBNAME)
else                
DDIRAMMEDIALIB   =   $(LIB_PATH)\$(DDIRAMMEDIA_LIBNAME)
endif

DDIRAMMEDIALIST  = \
                $(OBJ_PATH)\RAMMediaInit.obj \
                $(OBJ_PATH)\RAMMediaRead.obj \
                $(OBJ_PATH)\RAMMediaDescriptors.obj \
                $(OBJ_PATH)\RAMMediaDescriptorsInit.obj \
                $(OBJ_PATH)\RAMMediaWrite.obj


ifdef CODEBANKS
RESOURCE_INC_LIST = $(RESOURCE_INC_LIST) \
 $(SYMBOLS_PATH)\ramddiresource.inc
else
RESOURCE_INC_LIST = $(RESOURCE_INC_LIST) \
 $(SYMBOLS_PATH)\ramddiresource_flat.inc
endif 

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\ramddi_resource.obj \


#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDIRAMMEDIA_DEPENDENCIES= \
    "$(DDIRAM_INCLUDEPATH)\RAMDdi.h" \
    "$(DDIRAMMEDIA_INCLUDEPATH)\RAMMedia.h"
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

"$(DDIRAMMEDIALIB)" : $(DDIRAMMEDIALIST)
    @echo Creating IEEE archive library file for DriveTypeRAM Media $@
        @$(AR_EXE) -r $(DDIRAMMEDIALIB) $(DDIRAMMEDIALIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\RAMMediaInit.obj"              : "$(DDIRAMMEDIA_PATH)\RAMMediaInit.c" $(DDIRAMMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\RAMMediaRead.obj"              : "$(DDIRAMMEDIA_PATH)\RAMMediaRead.c" $(DDIRAMMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\RAMMediaDescriptors.obj"       : "$(DDIRAMMEDIA_PATH)\RAMMediaDescriptors.c" $(DDIRAMMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\RAMMediaDescriptorsInit.obj"   : "$(DDIRAMMEDIA_PATH)\RAMMediaDescriptorsInit.asm" $(DDIRAMMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\RAMMediaWrite.obj"             : "$(DDIRAMMEDIA_PATH)\RAMMediaWrite.c" $(DDIRAMMEDIA_DEPENDENCIES)



endif

$(OBJ_PATH)\ramddi_resource.obj : $(SYMBOLS_PATH)\ramddi_resource.asm  $(OUTPUT_INCLUDE_PATH)\resource.inc

endif