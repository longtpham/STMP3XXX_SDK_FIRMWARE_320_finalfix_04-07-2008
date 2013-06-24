#This is the makefile for the Resource Loader
#

ifdef MTP_ONLY
RSCMANAGER_LIBNAME      = rsc_manager_mtp_only.a
else
RSCMANAGER_LIBNAME      = rsc_manager.a
endif           #endif MTP_ONLY

RSCMANAGER_PATH         = $(ROOT)\system\common\resourcemanager

RSCMANAGER_INCLUDE_PATH = $(RSCMANAGER_PATH)
COMMON_INCLUDE_PATH     = $(ROOT)\inc

INC_ALL=$(INC_ALL);$(RSCMANAGER_INCLUDE_PATH);$(COMMON_INCLUDE_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(RSCMANAGER_INCLUDE_PATH) -Wc-I$(COMMON_INCLUDE_PATH)

FILESYSLIB  =   $(FILESYSLIB) \
                $(RSCMANAGERLIB)

ifdef D_PLIBS
RSCMANAGERLIB   =   $(OBJ_PATH)\$(RSCMANAGER_LIBNAME)
else
RSCMANAGERLIB   =   $(LIB_PATH)\$(RSCMANAGER_LIBNAME)
endif

ifdef FORCE_HOSTLINK_RSCMGR
RSCMANAGERLIB   =   $(ROOT)\lib\debug\msc\$(RSCMANAGER_LIBNAME)
endif
#
#
# The duplicated functionality of 2400_SysResourceApi.obj and 2400_SysSetResourceTag.obj were combined
# into SysResourceApi.c.  These files were almost identical and should be maintained as a single file.
# Previously, the player version resided in the files beginning with 2400_.  By combining these files,
# this make file, doesn't need to know anything about different versions of the file.
#
#
RSCMANAGERLIST  = \
                $(OBJ_PATH)\SysResourceApi.obj \
                $(OBJ_PATH)\SysResource.obj \
                $(OBJ_PATH)\sysresource_wrapper.obj


#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
RSCMANAGER_DEPENDENCIES= \
    "$(COMMON_INCLUDE_PATH)\SysResourceApi.h" \
    "$(RSCMANAGER_INCLUDE_PATH)\SysResource.h"

# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

$(RSCMANAGERLIB) : $(RSCMANAGERLIST)
    @echo Creating IEEE archive library file for Resource Manager $@
        @$(AR_EXE) -r $(RSCMANAGERLIB) $(RSCMANAGERLIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////
# create regsrtc.h from regsrtc.inc for use by build target obj below

$(ROOT)\inc\regsrtc.h : $(ROOT)\inc\3500\regsrtc.inc $(ROOT)\inc\persist_bits.h
    @echo Converting Header File $!
    $(INCTOC_EXE) $(ROOT)\inc\3500\regsrtc.inc  $(ROOT)\inc\regsrtc.h


# The duplicated functionality of 2400_SysResourceApi.obj and 2400_SysSetResourceTag.obj were combined
# into SysResourceApi.c.  These files were almost identical and should be maintained as a single file.
# Previously, the player version resided in the files beginning with 2400_.  By combining these files,
# this make file, doesn't need to know anything about different versions of the file.
#
#
"$(OBJ_PATH)\SysResourceApi.obj"        : "$(RSCMANAGER_PATH)\SysResourceApi.c" "$(ROOT)\inc\regsrtc.h" "$(INC_PATH)\persist_bits.h" $(RSCMANAGER_DEPENDENCIES)
$(OBJ_PATH)\sysresource_wrapper.obj     : $(RSCMANAGER_PATH)\sysresource_wrapper.asm
"$(OBJ_PATH)\SysResource.obj"           : "$(RSCMANAGER_PATH)\SysResource.c"  "$(ROOT)\inc\regsrtc.h"  "$(INC_PATH)\persist_bits.h" $(RSCMANAGER_DEPENDENCIES)


endif

