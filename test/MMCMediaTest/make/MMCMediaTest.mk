ifndef VERSION
VERSION=999.999
endif

ifdef D_D3410
DEVICE=3410
#else
#ifdef D_D3500
#                 ALREADY DEFINED IN STANDARD.MK   DELETE THIS COMMENTED LINES EVENTUALLY.
#DEVICE=3500
#else
#BLD=$(BLD) -DDEVICE_3500 
#C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DDEVICE_3500   
#DEVICE=3500
#endif
endif

# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
PROJ    = mmcmediatest
# relative pointer to SDK root directory \Mp3Firmware
RELBASE_PATH        =..\..\..
ROOT                = $(RELBASE_PATH)

# these macros are project dependent
PROJ_PATH           = $(ROOT)\test\mmcmediatest
MAIN_PATH           = $(PROJ_PATH)

#This is needed to use the RTC for system time instead of a timer interrupt.
BLD=$(BLD) -DUSE_RTC_FOR_TIME

#Uncomment the following line to keep tabs on the NAND Data Drive routines
#BLD=$(BLD) -DNDD_COUNT_ROUTINES

#This is needed to ensure we build as MMC.
BLD=$(BLD) -DMMC

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
include $(ROOT)\etc\MMCDataDrive.mk
#########################################
#
#include $(ROOT)\etc\MMCCommonDdi.mk
#########################################
#
include $(ROOT)\etc\MMCMedia.mk
#########################################
#
#include $(ROOT)\etc\MMCHal.mk
#########################################
#########################################
#
#include $(ROOT)\etc\CFPortHil.mk
#########################################

#########################################
#
include $(ROOT)\etc\registers.mk
#########################################

include $(ROOT)\etc\sysserialnumber.mk
######################################
#
include $(ROOT)\etc\Stringlib.mk
#########################################


CC_OPTS = $(CC_OPTS) 
#LK_OPTS = $(LK_OPTS) -u SysWaitOnObject -u FSysReleaseObject -u MMCSystemDriveApi
LK_OPTS = $(LK_OPTS) -u SysWaitOnObject -u FSysReleaseObject

# must follow standard.mk, where base INC_ALL gets defined
INC_ALL=-I$(PROJRSRC_PATH);$(COMMON_PROJ_PATH);$(OUTPUT_PATH);$(OUTPUT_INCLUDE_PATH);$(FAT12INC_PATH);$(INC_ALL);$(PROJ_PATH);
INC_ALL_C = -Wc-I$(OUTPUT_INCLUDE_PATH) $(INC_ALL_C)

               
OBJLIST =   $(OBJLIST)\
            $(OBJ_PATH)\task0.obj \
            $(OBJ_PATH)\kernelproject.obj \
            $(OBJ_PATH)\prototypeproject.obj \
            $(OBJ_PATH)\systime.obj\
            $(OBJ_PATH)\miscproject.obj



MODLIST =   
MODULELIST = 
DEBUGABSLIST= 

MAKELIST =  $(OBJLIST) $(ALLLIBLIST)

ALL : $(OUTPUT_PATH)\$(PROJ).abs   $(DEBUGABSLIST)
# remove comment before $(DEBUGABSLIST) above to build debug .abs

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).out" : $(PROJ).mk stmp3500.dsc stmpsys.dsc $(MAKELIST)
    @echo Linking $(PROJ).out
        @$(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(LIB_OPT_LIST) $(match .a $!)

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
"$(OBJ_PATH)\task0.obj" : $(PROJ_PATH)\task0.c $(OUTPUT_INCLUDE_PATH)\resource.h  $(OUTPUT_INCLUDE_PATH)\project.h $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h  $(OUTPUT_INCLUDE_PATH)\regsrtc.h $(OUTPUT_INCLUDE_PATH)\regsgpflash.h $(OUTPUT_INCLUDE_PATH)\regsgpio.h 
"$(OBJ_PATH)\kernelproject.obj" : $(PROJ_PATH)\kernelproject.c $(OUTPUT_INCLUDE_PATH)\resource.h $(OUTPUT_INCLUDE_PATH)\project.h
"$(OBJ_PATH)\prototypeproject.obj" : $(PROJ_PATH)\prototypeproject.c
"$(OBJ_PATH)\ddiproject.obj" : $(PROJ_PATH)\ddiproject.c 
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h  
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\project.h
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\errors.h
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\regsspi.h
"$(OBJ_PATH)\miscproject.obj" : $(PROJ_PATH)\miscproject.c
"$(OBJ_PATH)\systime.obj"     : $(SYS_COMMON_PATH)\systime.asm

#///////////////////////////////////////////////////////////////////////////////
# General System
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\const.obj : $(SYS_PATH)\data_management\const.asm

#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////
