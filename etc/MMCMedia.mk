ifndef MMCMEDIA_MK
MMCMEDIA_MK = TRUE
#This is the makefile for the MediaTypeMMC DDI.
#

ifdef D_FUNCLETS
DDIMMCMEDIA_LIBNAME     =   dd_mmc_media_funclets.a
else
DDIMMCMEDIA_LIBNAME     =   dd_mmc_media.a
endif

MEDIATYPEMMC_PATH       = $(ROOT)\devicedriver\media\MediaTypeMMC
DDIMMC_PATH             = $(MEDIATYPEMMC_PATH)\DDI
DDIMMCMEDIA_PATH        = $(DDIMMC_PATH)\Media
DDIMMCCOMMON_PATH       = $(DDIMMC_PATH)\Common
HALMMC_PATH             = $(MEDIATYPEMMC_PATH)\HAL
HALMMCSPI_PATH       	= $(HALMMC_PATH)\SPI
DDIMEDIASPI_PATH        = $(MEDIA_PATH)\SPI

DDIMMC_INCLUDEPATH              = $(DDIMMC_PATH)\include
#DDIMMCMEDIA_INCLUDEPATH         = $(DDIMMCMEDIA_PATH)\include
HALMMC_INCLUDEPATH              = $(HALMMC_PATH)\include

INC_ALL=$(INC_ALL);$(DDIMMC_INCLUDEPATH);$(DDIMMCMEDIA_PATH);$(HALMMC_INCLUDEPATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(DDIMMC_INCLUDEPATH) -Wc-I$(DDIMMCMEDIA_PATH) -Wc-I$(HALMMC_INCLUDEPATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDIMMCMEDIALIB)

ifdef D_PLIBS
DDIMMCMEDIALIB   =   $(OBJ_PATH)\$(DDIMMCMEDIA_LIBNAME)
else
DDIMMCMEDIALIB   =   $(LIB_PATH)\$(DEVICE)\$(DDIMMCMEDIA_LIBNAME)
endif


DDIMMCMEDIALIST  =   $(OBJ_PATH)\MMCMediaInit.obj \
		     $(OBJ_PATH)\MMCMediaAllocate.obj \
		     $(OBJ_PATH)\MMCMediaDiscover.obj \
		     $(OBJ_PATH)\MMCMediaErase.obj \
		     $(OBJ_PATH)\MMCMediaGetInfo.obj \
		     $(OBJ_PATH)\MMCMediaGetMediaTable.obj \
		     $(OBJ_PATH)\MMCDetectMedia.obj \
		     $(OBJ_PATH)\MMCFunctions.obj \
		     $(OBJ_PATH)\MMCEnumerate.obj \
		     $(OBJ_PATH)\MMCResetDevice.obj \
		     $(OBJ_PATH)\MMCProcessCsd.obj \
		     $(OBJ_PATH)\MMCProcessCid.obj \
		     $(OBJ_PATH)\MMCProcessCid2.obj \
		     $(OBJ_PATH)\MMCEraseFunctions.obj \
		     $(OBJ_PATH)\mmchalspi.obj \
		     $(OBJ_PATH)\mmchalspiwrite.obj \
		     $(OBJ_PATH)\spihal.obj \
		     $(OBJ_PATH)\spistructs.obj \
		     $(OBJ_PATH)\spihalasm.obj \
		     $(OBJ_PATH)\MMCHalInit.obj \
		     $(OBJ_PATH)\MMCHalFunctions.obj \
		     $(OBJ_PATH)\MMCHalDetectPresence.obj \
		     $(OBJ_PATH)\MMCMediaDescriptorsInit.obj \
		     $(OBJ_PATH)\MMCSerialNumberInit.obj 		     
		     
ifdef CODEBANKS
RESOURCE_INC_LIST = $(RESOURCE_INC_LIST) \
 $(SYMBOLS_PATH)\mmcddiresource.inc
else
RESOURCE_INC_LIST = $(RESOURCE_INC_LIST) \
 $(SYMBOLS_PATH)\mmcddiresource_flat.inc
endif 

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\mmcddi_resource.obj \
		     
		     

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDIMMCMEDIA_DEPENDENCIES= \
    "$(DDIMMC_INCLUDEPATH)\MMCDdi.h" \
    "$(DDIMMC_INCLUDEPATH)\MMCMedia.h" \
    "$(DDIMMC_INCLUDEPATH)\MMCCommon.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsspi.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsgpio.h" \
    "$(OUTPUT_INCLUDE_PATH)\errors.h"
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

$(DDIMMCMEDIALIB) : $(DDIMMCMEDIALIST)
    @echo Creating IEEE archive library file for DriveTypeMMC Media $@
        @$(AR_EXE) -r $(DDIMMCMEDIALIB) $(DDIMMCMEDIALIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\MMCMediaInit.obj" 		: "$(DDIMMCMEDIA_PATH)\MMCMediaInit.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCMediaAllocate.obj" 	: "$(DDIMMCMEDIA_PATH)\MMCMediaAllocate.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCMediaDiscover.obj" 	: "$(DDIMMCMEDIA_PATH)\MMCMediaDiscover.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCMediaErase.obj" 	: "$(DDIMMCMEDIA_PATH)\MMCMediaErase.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCMediaGetInfo.obj" 	: "$(DDIMMCMEDIA_PATH)\MMCMediaGetInfo.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCMediaGetMediaTable.obj" : "$(DDIMMCMEDIA_PATH)\MMCMediaGetMediaTable.c" $(DDIMMCMEDIA_DEPENDENCIES)

"$(OBJ_PATH)\MMCDetectMedia.obj"    : "$(DDIMMCCOMMON_PATH)\MMCDetectMedia.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCFunctions.obj"      : "$(DDIMMCCOMMON_PATH)\MMCFunctions.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCEnumerate.obj"      : "$(DDIMMCCOMMON_PATH)\MMCEnumerate.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCResetDevice.obj"    : "$(DDIMMCCOMMON_PATH)\MMCResetDevice.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCProcessCsd.obj"     : "$(DDIMMCCOMMON_PATH)\MMCProcessCsd.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCProcessCid.obj"     : "$(DDIMMCCOMMON_PATH)\MMCProcessCid.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCProcessCid2.obj"    : "$(DDIMMCCOMMON_PATH)\MMCProcessCid2.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCEraseFunctions.obj" : "$(DDIMMCCOMMON_PATH)\MMCEraseFunctions.c" $(DDIMMCMEDIA_DEPENDENCIES)

"$(OBJ_PATH)\mmchalspi.obj"         : "$(HALMMCSPI_PATH)\mmchalspi.c" $(DDIMMCMEDIA_DEPENDENCIES) 
"$(OBJ_PATH)\mmchalspiwrite.obj"    : "$(HALMMCSPI_PATH)\mmchalspiwrite.c" $(DDIMMCMEDIA_DEPENDENCIES) 
"$(OBJ_PATH)\mmchalspidetect.obj"   : "$(HALMMCSPI_PATH)\mmchalspidetect.c" $(DDIMMCMEDIA_DEPENDENCIES)

"$(OBJ_PATH)\spihal.obj"   	    : "$(DDIMEDIASPI_PATH)\spihal.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\spistructs.obj"   	    : "$(DDIMEDIASPI_PATH)\spistructs.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\spihalasm.obj"   	    : "$(DDIMEDIASPI_PATH)\spihalasm.asm" $(DDIMMCMEDIA_DEPENDENCIES)

"$(OBJ_PATH)\MMCHalInit.obj"   	    : "$(HALMMC_PATH)\MMCHalInit.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCHalFunctions.obj"   : "$(HALMMC_PATH)\MMCHalFunctions.c" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MMCHalDetectPresence.obj" : "$(HALMMC_PATH)\MMCHalDetectPresence.c" $(DDIMMCMEDIA_DEPENDENCIES)

"$(OBJ_PATH)\MMCMediaDescriptorsInit.obj" : "$(DDIMMCMEDIA_PATH)\MMCMediaDescriptorsInit.asm" $(DDIMMCMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\MmcSerialNumberInit.obj"     : "$(DDIMMCCOMMON_PATH)\MmcSerialNumberInit.c" $(DDIMMCMEDIA_DEPENDENCIES)
endif

$(OBJ_PATH)\mmcddi_resource.obj : $(RESOURCE_INC_LIST) 
$(OBJ_PATH)\mmcddi_resource.obj : $(SYMBOLS_PATH)\mmcddi_resource.asm 

endif