#=============================================================================
#
# DESCRIPTION:
#  The build rules necessary to include the database in the system.
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
ifndef DATABASE_MK
DATABASE_MK = TRUE

ifdef D_PLIBS
DATABASE_LIB = $(OBJ_PATH)\stdatabase.a
else
DATABASE_LIB = $(CHIP_LIB_PATH)\stdatabase.a
endif

LK_OPTS = $(LK_OPTS) -uFDBGetNumber -uFDBSetNumber -uFDBSetString -uFDBGetString
LK_OPTS = $(LK_OPTS) -uFDBGetBlock -uFDBSetBlock -uFDBGetNumDeletedRecords
LK_OPTS = $(LK_OPTS) -uFDBClose -uFDBGetNumRecords -uFDBOpen -uFDBClean
LK_OPTS = $(LK_OPTS) -uFDBNewRecord -uFDBDeleteRecord

ifdef D_PLIBS

INC_ALL =   $(INC_ALL);$(ROOT)\filesystem\fat32\h
INC_ALL_C = $(INC_ALL_C) -Wc-I$(ROOT)\filesystem\fat32\h


DATABASE_OBJLIST =\
    $(OBJ_PATH)\database.obj\
    $(OBJ_PATH)\dbopen.obj\
    $(OBJ_PATH)\dbclose.obj\
    $(OBJ_PATH)\dbgetstring.obj\
    $(OBJ_PATH)\dbsetstring.obj\
    $(OBJ_PATH)\dbgetnumber.obj\
    $(OBJ_PATH)\dbsetnumber.obj\
    $(OBJ_PATH)\dbgetblock.obj\
    $(OBJ_PATH)\dbsetblock.obj\
    $(OBJ_PATH)\dbdeleterecord.obj\
    $(OBJ_PATH)\dbnewrecord.obj\
    $(OBJ_PATH)\dbgetnumrecords.obj\
    $(OBJ_PATH)\dbgetnumdeletedrecords.obj\
    $(OBJ_PATH)\dbwriterecord.obj

# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
$(DATABASE_LIB) : $(DATABASE_OBJLIST) $(ROOT)\etc\database.mk
    @echo Creating IEEE archive library database file for the MTP Driver$@
    $(AR_EXE) -r $(DATABASE_LIB) $(DATABASE_OBJLIST)

#=============================================================================
#                             Dependencies
#=============================================================================
$(OBJ_PATH)\database.obj :  $(SYS_COMMON_PATH)\database\database.c
$(OBJ_PATH)\database.obj :  $(INC_PATH)\database_internal.h
$(OBJ_PATH)\database.obj :  $(INC_PATH)\database.h
$(OBJ_PATH)\database.obj :  $(INC_PATH)\types.h
$(OBJ_PATH)\database.obj :  $(INC_PATH)\filesystem.h
$(OBJ_PATH)\database.obj :  $(INC_PATH)\fsapi.h
$(OBJ_PATH)\database.obj :  $(INC_PATH)\fstypes.h
$(OBJ_PATH)\database.obj :  $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbopen.obj :    $(SYS_COMMON_PATH)\database\dbopen.c
$(OBJ_PATH)\dbopen.obj :    $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbopen.obj :    $(INC_PATH)\database.h
$(OBJ_PATH)\dbopen.obj :    $(INC_PATH)\types.h
$(OBJ_PATH)\dbopen.obj :    $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbopen.obj :    $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbopen.obj :    $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbopen.obj :    $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbclose.obj :   $(SYS_COMMON_PATH)\database\dbclose.c
$(OBJ_PATH)\dbclose.obj :   $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbclose.obj :   $(INC_PATH)\database.h
$(OBJ_PATH)\dbclose.obj :   $(INC_PATH)\types.h
$(OBJ_PATH)\dbclose.obj :   $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbclose.obj :   $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbclose.obj :   $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbclose.obj :   $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbgetstring.obj :   $(SYS_COMMON_PATH)\database\dbgetstring.c
$(OBJ_PATH)\dbgetstring.obj :   $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbgetstring.obj :   $(INC_PATH)\database.h
$(OBJ_PATH)\dbgetstring.obj :   $(INC_PATH)\types.h
$(OBJ_PATH)\dbgetstring.obj :   $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbgetstring.obj :   $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbgetstring.obj :   $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbgetstring.obj :   $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbsetstring.obj :   $(SYS_COMMON_PATH)\database\dbsetstring.c
$(OBJ_PATH)\dbsetstring.obj :   $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbsetstring.obj :   $(INC_PATH)\database.h
$(OBJ_PATH)\dbsetstring.obj :   $(INC_PATH)\types.h
$(OBJ_PATH)\dbsetstring.obj :   $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbsetstring.obj :   $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbsetstring.obj :   $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbsetstring.obj :   $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbgetnumber.obj :   $(SYS_COMMON_PATH)\database\dbgetnumber.c
$(OBJ_PATH)\dbgetnumber.obj :   $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbgetnumber.obj :   $(INC_PATH)\database.h
$(OBJ_PATH)\dbgetnumber.obj :   $(INC_PATH)\types.h
$(OBJ_PATH)\dbgetnumber.obj :   $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbgetnumber.obj :   $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbgetnumber.obj :   $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbgetnumber.obj :   $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbsetnumber.obj :   $(SYS_COMMON_PATH)\database\dbsetnumber.c
$(OBJ_PATH)\dbsetnumber.obj :   $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbsetnumber.obj :   $(INC_PATH)\database.h
$(OBJ_PATH)\dbsetnumber.obj :   $(INC_PATH)\types.h
$(OBJ_PATH)\dbsetnumber.obj :   $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbsetnumber.obj :   $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbsetnumber.obj :   $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbsetnumber.obj :   $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbgetblock.obj :    $(SYS_COMMON_PATH)\database\dbgetblock.c
$(OBJ_PATH)\dbgetblock.obj :    $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbgetblock.obj :    $(INC_PATH)\database.h
$(OBJ_PATH)\dbgetblock.obj :    $(INC_PATH)\types.h
$(OBJ_PATH)\dbgetblock.obj :    $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbgetblock.obj :    $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbgetblock.obj :    $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbgetblock.obj :    $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbsetblock.obj :    $(SYS_COMMON_PATH)\database\dbsetblock.c
$(OBJ_PATH)\dbsetblock.obj :    $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbsetblock.obj :    $(INC_PATH)\database.h
$(OBJ_PATH)\dbsetblock.obj :    $(INC_PATH)\types.h
$(OBJ_PATH)\dbsetblock.obj :    $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbsetblock.obj :    $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbsetblock.obj :    $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbsetblock.obj :    $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbdeleterecord.obj :    $(SYS_COMMON_PATH)\database\dbdeleterecord.c
$(OBJ_PATH)\dbdeleterecord.obj :    $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbdeleterecord.obj :    $(INC_PATH)\database.h
$(OBJ_PATH)\dbdeleterecord.obj :    $(INC_PATH)\types.h
$(OBJ_PATH)\dbdeleterecord.obj :    $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbdeleterecord.obj :    $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbdeleterecord.obj :    $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbdeleterecord.obj :    $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbnewrecord.obj :   $(SYS_COMMON_PATH)\database\dbnewrecord.c
$(OBJ_PATH)\dbnewrecord.obj :   $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbnewrecord.obj :   $(INC_PATH)\database.h
$(OBJ_PATH)\dbnewrecord.obj :   $(INC_PATH)\types.h
$(OBJ_PATH)\dbnewrecord.obj :   $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbnewrecord.obj :   $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbnewrecord.obj :   $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbnewrecord.obj :   $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbgetnumrecords.obj :   $(SYS_COMMON_PATH)\database\dbgetnumrecords.c
$(OBJ_PATH)\dbgetnumrecords.obj :   $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbgetnumrecords.obj :   $(INC_PATH)\database.h
$(OBJ_PATH)\dbgetnumrecords.obj :   $(INC_PATH)\types.h
$(OBJ_PATH)\dbgetnumrecords.obj :   $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbgetnumrecords.obj :   $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbgetnumrecords.obj :   $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbgetnumrecords.obj :   $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbgetnumdeletedrecords.obj :    $(SYS_COMMON_PATH)\database\dbgetnumdeletedrecords.c
$(OBJ_PATH)\dbgetnumdeletedrecords.obj :    $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbgetnumdeletedrecords.obj :    $(INC_PATH)\database.h
$(OBJ_PATH)\dbgetnumdeletedrecords.obj :    $(INC_PATH)\types.h
$(OBJ_PATH)\dbgetnumdeletedrecords.obj :    $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbgetnumdeletedrecords.obj :    $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbgetnumdeletedrecords.obj :    $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbgetnumdeletedrecords.obj :    $(INC_PATH)\fserror.h

$(OBJ_PATH)\dbwriterecord.obj :     $(SYS_COMMON_PATH)\database\dbwriterecord.c
$(OBJ_PATH)\dbwriterecord.obj :     $(INC_PATH)\database_internal.h
$(OBJ_PATH)\dbwriterecord.obj :     $(INC_PATH)\database.h
$(OBJ_PATH)\dbwriterecord.obj :     $(INC_PATH)\types.h
$(OBJ_PATH)\dbwriterecord.obj :     $(INC_PATH)\filesystem.h
$(OBJ_PATH)\dbwriterecord.obj :     $(INC_PATH)\fsapi.h
$(OBJ_PATH)\dbwriterecord.obj :     $(INC_PATH)\fstypes.h
$(OBJ_PATH)\dbwriterecord.obj :     $(INC_PATH)\fserror.h
endif # ifdef D_PLIBS

OBJLIST =   $(OBJLIST)\
            $(OBJ_PATH)\database_resource.obj

RESOURCELIST = $(RESOURCELIST)\
            $(OUTPUT_RSRC_PATH)\database_open.src\
            $(OUTPUT_RSRC_PATH)\database_close.src\
            $(OUTPUT_RSRC_PATH)\database_delete.src\
            $(OUTPUT_RSRC_PATH)\database_access.src

$(OUTPUT_RSRC_PATH)\database_open.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\database.loc 
$(OUTPUT_RSRC_PATH)\database_close.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\database.loc 
$(OUTPUT_RSRC_PATH)\database_delete.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\database.loc 
$(OUTPUT_RSRC_PATH)\database_access.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\database.loc 
    @echo Extract Database Files
    $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(LOC_PATH)\database.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

$(OBJ_PATH)\database_resource.obj : $(SYMBOLS_PATH)\database_resource.asm 

endif # ifndef DATABASE_MK
