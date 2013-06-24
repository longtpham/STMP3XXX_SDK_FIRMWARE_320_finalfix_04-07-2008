#=============================================================================
#
# DESCRIPTION:
#  The build rules necessary to include the store in the system.
#
# Copyright (c) SigmaTel, Inc. Unpublished
# 
# SigmaTel, Inc.
# Proprietary  Confidential
# 
# This source code and the algorithms implemented therein constitute 
# confidential information and may comprise trade secrets of SigmaTel, Inc. 
# or its associates, and any unauthorized use thereof is prohibited.
#
#=============================================================================
ifndef STORE_MK
STORE_MK = TRUE

INC_STOR = $(ROOT)\DeviceDriver\Media\mtp\Store
STOR_PATH = $(ROOT)\DeviceDriver\Media\mtp\Store
AUDIBLE_MTP_PATH =  $(ROOT)\system\common\metadata\AudibleMtp


include $(ROOT)\etc\database.mk
include $(ROOT)\etc\usbmtp.mk

ifdef D_FUNCLETS
STSTORE_LIBNAME = ststore_funclets.a
else
STSTORE_LIBNAME = ststore.a
endif

ifdef D_PLIBS
STSTORE_LIB = $(OBJ_PATH)\$(STSTORE_LIBNAME)
else
STSTORE_LIB = $(CHIP_LIB_PATH)\$(STSTORE_LIBNAME)
endif

#
# These symbols must be "undefined" to prevent them from being undefined in the
# library file.  This is not intuitive, but is required.
#
LK_OPTS = $(LK_OPTS) -upacked_strcpy -uFConvStmp2HostUnicodeStr

ifdef D_PLIBS

INC_ALL =   $(INC_ALL);$(INC_STOR);$(AUDIBLE_MTP_PATH)
INC_ALL_C = $(INC_ALL_C) -Wc-I$(INC_STOR) -Wc-I$(AUDIBLE_MTP_PATH)
    
STSTORE_OBJLIST = \
    $(OBJ_PATH)\StStore.obj\
    $(OBJ_PATH)\StObjects.obj\
    $(OBJ_PATH)\StStoreEnum.obj\
    $(OBJ_PATH)\StRebuild.obj\
    $(OBJ_PATH)\staddmetadata.obj\
    $(OBJ_PATH)\StAddNewObject.obj\
    $(OBJ_PATH)\StDeleteObject.obj\
    $(OBJ_PATH)\StReadObjectData.obj\
    $(OBJ_PATH)\StGetObjectList.obj\
    $(OBJ_PATH)\StWriteObjectData.obj\
    $(OBJ_PATH)\StIsObjectValid.obj\
    $(OBJ_PATH)\StIndexFilesIO.obj\
    $(OBJ_PATH)\StChDirToParentOf.obj\
    $(OBJ_PATH)\StGetNumObjects.obj\
    $(OBJ_PATH)\StStorClose.obj\
    $(OBJ_PATH)\StStorInfo.obj

#OBJLIST =   $(OBJLIST)\
#    $(OBJ_PATH)\StStorClose.obj\
#    $(OBJ_PATH)\StStorInfo.obj
    

#=============================================================================
#                             Dependencies
#=============================================================================
STOR_DEPENDENCIES= \
    "$(OUTPUT_INCLUDE_PATH)\regsrtc.h"\
    "$(MTP_PATH)\ptpstring.h" \
    "$(INC_STOR)\StError.h" \
    "$(INC_STOR)\StStore.h" \
    "$(INC_STOR)\StFastEnum.h" \
    "$(INC_PATH)\database.h" \
    "$(INC_PATH)\metadata.h" \
    "$(INC_PATH)\FileSpec.h" \
    "$(INC_PATH)\fserror.h" \
    "$(INC_PATH)\fstypes.h" \
    "$(INC_PATH)\fsapi.h" \
    "$(INC_PATH)\filesystem.h" \
    "$(INC_PATH)\stringlib.h" \
    "$(INC_PATH)\types.h" \
    "$(INC_PATH)\mtp.h" 

#
# Make sure that the resource file is built in case store is using 
# SysCallFunction.
#
"$(OBJ_PATH)\StStore.obj"            :     "$(STOR_PATH)\ststore.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StStoreEnum.obj"        :     "$(STOR_PATH)\StStoreEnum.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StRebuild.obj"          :     "$(STOR_PATH)\StRebuild.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\staddmetadata.obj"      :     "$(STOR_PATH)\staddmetadata.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StObjects.obj"          :     "$(STOR_PATH)\StObjects.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StAddNewObject.obj"     :     "$(STOR_PATH)\StAddNewObject.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StDeleteObject.obj"     :     "$(STOR_PATH)\StDeleteObject.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StGetNumObjects.obj"    :     "$(STOR_PATH)\StGetNumObjects.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StGetObjectList.obj"    :     "$(STOR_PATH)\StGetObjectList.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StReadObjectData.obj"   :     "$(STOR_PATH)\StReadObjectData.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StWriteObjectData.obj"  :     "$(STOR_PATH)\StWriteObjectData.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StIsObjectValid.obj"    :     "$(STOR_PATH)\StIsObjectValid.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StChDirToParentOf.obj"  :     "$(STOR_PATH)\StChDirToParentOf.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StIndexFilesIO.obj"     :     "$(STOR_PATH)\StIndexFilesIO.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StStorClose.obj"        :     "$(STOR_PATH)\StStorClose.c" $(STOR_DEPENDENCIES)
"$(OBJ_PATH)\StStorInfo.obj"         :     "$(STOR_PATH)\StStorInfo.c" $(STOR_DEPENDENCIES)

# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
$(STSTORE_LIB) : $(STSTORE_OBJLIST)
    @echo Creating IEEE archive library file for MTP Store$@
    @$(AR_EXE) -r $(STSTORE_LIB) $(STSTORE_OBJLIST)

endif # ifdef D_PLIBS
OBJLIST = $(OBJLIST)\                   
    $(OBJ_PATH)\store_resource.obj

endif # ifndef STORE_MK

$(OBJ_PATH)\store_resource.obj : $(SYMBOLS_PATH)\store_resource.asm 
