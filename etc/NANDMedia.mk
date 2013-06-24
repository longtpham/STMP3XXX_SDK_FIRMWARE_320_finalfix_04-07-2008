ifndef NANDMEDIA_MK
NANDMEDIA_MK = TRUE
#This is the makefile for the MediaTypeNAND DDI.
#

ifdef D_FUNCLETS
ifdef MTP_ONLY
DDINANDMEDIA_LIBNAME     =   dd_nand_media_mtp_only.a
else
DDINANDMEDIA_LIBNAME     =   dd_nand_media.a
endif           #endif MTP_ONLY
else
ifdef MTP_ONLY
DDINANDMEDIA_LIBNAME     =   dd_nand_media_mtp_only.a
else
DDINANDMEDIA_LIBNAME     =   dd_nand_media.a
endif           #endif MTP_ONLY
endif           #endif FUNCLETS

MEDIATYPENAND_PATH       = $(ROOT)\devicedriver\media\MediaTypeNAND
DDINAND_PATH             = $(MEDIATYPENAND_PATH)\DDI
DDINANDMEDIA_PATH        = $(DDINAND_PATH)\Media
HALNAND_PATH             = $(MEDIATYPENAND_PATH)\HAL
MEDIA_PATH               = $(ROOT)\devicedriver\media

MEDIATYPENAND_INCLUDEPATH        = $(MEDIATYPENAND_PATH)\include
DDINAND_INCLUDEPATH              = $(DDINAND_PATH)\include
DDINANDMEDIA_INCLUDEPATH         = $(DDINANDMEDIA_PATH)\include
HALNAND_INCLUDEPATH              = $(HALNAND_PATH)\include
MEDIA_INCLUDEPATH                = $(MEDIA_PATH)\include

INC_ALL=$(INC_ALL);$(DDINAND_INCLUDEPATH);$(MEDIATYPENAND_INCLUDEPATH);$(DDINANDMEDIA_INCLUDEPATH);$(HALNAND_INCLUDEPATH);$(DDINANDMEDIA_PATH);$(MEDIA_INCLUDEPATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(MEDIATYPENAND_INCLUDEPATH) -Wc-I$(DDINANDMEDIA_INCLUDEPATH) -Wc-I$(DDINAND_INCLUDEPATH) -Wc-I$(HALNAND_INCLUDEPATH) -Wc-I$(DDINANDMEDIA_PATH) -Wc-I$(MEDIA_INCLUDEPATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDINANDMEDIALIB)

LK_OPTS =  $(LK_OPTS)\
                -u FNANDDataDriveApi -u FNANDSystemDriveApi
                
ifdef D_PLIBS
DDINANDMEDIALIB   =   $(OBJ_PATH)\$(DDINANDMEDIA_LIBNAME)
else
DDINANDMEDIALIB   =   $(LIB_PATH)\$(DEVICE)\$(DDINANDMEDIA_LIBNAME)
endif

DDINANDMEDIALIST  = \
                $(OBJ_PATH)\NANDMediaInit.obj \
                $(OBJ_PATH)\NANDMediaAllocate.obj \
                $(OBJ_PATH)\NANDMediaDiscover.obj \
                $(OBJ_PATH)\NANDMediaGetMediaTable.obj \
                $(OBJ_PATH)\NANDMediaGetInfo.obj \
                $(OBJ_PATH)\NANDMediaErase.obj \
                $(OBJ_PATH)\NANDMediaCloseMedia.obj \
                $(OBJ_PATH)\NANDMediaDescriptorsInit.obj\
                $(OBJ_PATH)\Updater.obj\ 
                $(OBJ_PATH)\ddildl_utils_asm.obj

# was 2nd to last in list above. Removed since unused.
#$(OBJ_PATH)\Update_Media.obj \



# Removed this from lib so customers can now receive nandmediadefaults.c and link the 
# obj later. Just add it to the OBJ list and add nandmediadefaults mk rule near EOF to make it happen.
#               $(OBJ_PATH)\nandmediadefaults.obj





ifdef CODEBANKS
RESOURCE_INC_LIST = $(RESOURCE_INC_LIST) \
 $(SYMBOLS_PATH)\nandddiresource.inc
else
RESOURCE_INC_LIST = $(RESOURCE_INC_LIST) \
 $(SYMBOLS_PATH)\nandddiresource_flat.inc
endif 

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\nandddi_resource.obj \
    $(OBJ_PATH)\nandmediadefaults.obj



#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDINANDMEDIA_DEPENDENCIES= \
    "$(DDINAND_INCLUDEPATH)\NANDDdi.h" \
    "$(DDINANDMEDIA_INCLUDEPATH)\NANDMedia.h" \
    "$(OUTPUT_INCLUDE_PATH)\errors.h"
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

$(DDINANDMEDIALIB) : $(DDINANDMEDIALIST)
    @echo Creating IEEE archive library file for DriveTypeNAND Media $@
        @$(AR_EXE) -r $(DDINANDMEDIALIB) $(DDINANDMEDIALIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\NANDMediaInit.obj"              : "$(DDINANDMEDIA_PATH)\NANDMediaInit.c" $(DDINANDMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\NANDMediaAllocate.obj"          : "$(DDINANDMEDIA_PATH)\NANDMediaAllocate.c" $(DDINANDMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\NANDMediaDiscover.obj"          : "$(DDINANDMEDIA_PATH)\NANDMediaDiscover.c" $(DDINANDMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\NANDMediaGetMediaTable.obj"     : "$(DDINANDMEDIA_PATH)\NANDMediaGetMediaTable.c" $(DDINANDMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\NANDMediaGetInfo.obj"           : "$(DDINANDMEDIA_PATH)\NANDMediaGetInfo.c" $(DDINANDMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\NANDMediaErase.obj"             : "$(DDINANDMEDIA_PATH)\NANDMediaErase.c" $(DDINANDMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\NANDMediaCloseMedia.obj"        : "$(DDINANDMEDIA_PATH)\NANDMediaCloseMedia.c" $(DDINANDMEDIA_DEPENDENCIES)
#"$(OBJ_PATH)\Update_Media.obj"      		 : "$(DDINANDMEDIA_PATH)\Update_Media.c" $(DDINANDMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\Updater.obj"      				 : "$(DDINANDMEDIA_PATH)\Updater.asm" $(DDINANDMEDIA_DEPENDENCIES)
"$(OBJ_PATH)\ddildl_utils_asm.obj"           : "$(DDILDL_PATH)\ddildl_utils_asm.asm" $(DDILDL_DEPENDENCIES)

"$(OBJ_PATH)\NANDMediaDescriptorsInit.obj"   : "$(DDINANDMEDIA_PATH)\NANDMediaDescriptorsInit.asm" $(DDINANDMEDIA_DEPENDENCIES)




endif

# Now compiled (and linked) by OEMs. Allows customers to reduce RAM footprint in some flash cases.
$(OBJ_PATH)\nandmediadefaults.obj : "$(DDINANDMEDIA_PATH)\nandmediadefaults.c" $(DDINANDMEDIA_DEPENDENCIES)

$(OBJ_PATH)\nandddi_resource.obj : $(SYMBOLS_PATH)\nandddi_resource.asm 
#$(OBJ_PATH)\nandddi_resource.obj : $(RESOURCE_INC) 
endif
