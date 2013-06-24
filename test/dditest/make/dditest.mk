# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
PROJ    = dditest
# relative pointer to SDK root directory \Mp3BaseCc
RELBASE_PATH        =..\..\..
ROOT                = $(RELBASE_PATH)

# these macros are project dependent
PROJ_PATH           = $(ROOT)\test\dditest
MAIN_PATH           = $(ROOT)\test\dditest

#########################################
#
include $(ROOT)\etc\options.mk
#########################################
#
include $(ROOT)\etc\standard.mk
#########################################
#
include $(ETC_PATH)\includes.mk
#########################################
#
include $(ROOT)\etc\ddildl.mk
#########################################
#
include $(ROOT)\etc\kernel.mk
#########################################
#
include $(ROOT)\etc\RAMDataDrive.mk
#########################################
#
include $(ROOT)\etc\RAMSystemDrive.mk
#########################################
#
include $(ROOT)\etc\RAMMedia.mk
#########################################
#
include $(ROOT)\etc\RAMHal.mk
#########################################

#########################################
#
include $(ROOT)\etc\ResourceManager.mk
#########################################

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////

PROJECT_H = $(OUTPUT_INCLUDE_PATH)\project.h

# must follow standard.mk, where base INC_ALL gets defined
INC_ALL=-I$(PROJRSRC_PATH);$(COMMON_PROJ_PATH);$(OUTPUT_PATH);$(OUTPUT_INCLUDE_PATH);$(FAT12INC_PATH);$(INC_ALL)
INC_ALL_C = -Wc-I$(OUTPUT_INCLUDE_PATH) $(INC_ALL_C)

LIB_OPT_LIST =  

OBJLIST =  $(OBJLIST)\
            $(OBJ_PATH)\task0.obj \
            $(OBJ_PATH)\kernelproject.obj \
            $(OBJ_PATH)\prototypeproject.obj \

ifdef D_D3410
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\cfporthil_resource.obj
endif


MODLIST =   
MODULELIST = 
DEBUGABSLIST= 

MAKELIST =  $(OBJLIST) $(ALLLIBLIST) $(SYMLIST)

ALL : $(OUTPUT_PATH)\$(PROJ).abs   $(DEBUGABSLIST)
# remove comment before $(DEBUGABSLIST) above to build debug .abs

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).out" : $(PROJ).mk stmp3500.dsc stmpsys.dsc $(MAKELIST)
    @echo Linking $(PROJ).out
        @$(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(LIB_OPT_LIST) $(match .a $!) $(match .sym $!)

"$(OUTPUT_PATH)\$(PROJ).abs"                 : "$(OUTPUT_PATH)\$(PROJ).out" stmpsys.dsc 
    @echo Creating IEEE-695 absolute file $@
        @$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out

#///////////////////////////////////////////////////////////////////////////////
# Debug Module Links
#      These links provide an .abs that can be used to load source-level symbols for each overlaid module
#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
# Project-specific files
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\task0.obj" : $(PROJ_PATH)\task0.c $(OUTPUT_INCLUDE_PATH)\resource.h  $(OUTPUT_INCLUDE_PATH)\project.h

"$(OBJ_PATH)\kernelproject.obj" : $(PROJ_PATH)\kernelproject.c $(OUTPUT_INCLUDE_PATH)\resource.h $(OUTPUT_INCLUDE_PATH)\project.h

"$(OBJ_PATH)\prototypeproject.obj" : $(PROJ_PATH)\prototypeproject.c

"$(OBJ_PATH)\ddiproject.obj" : $(PROJ_PATH)\ddiproject.c 
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h  
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\project.h
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\errors.h

ifdef D_D3410
$(OBJ_PATH)\cfporthil_resource.obj : $(SYMBOLS_PATH)\cfporthil_resource.asm
endif

#///////////////////////////////////////////////////////////////////////////////
# General System
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\const.obj : $(SYS_PATH)\data_management\const.asm

#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////
