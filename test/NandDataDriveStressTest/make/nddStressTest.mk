##/////////////////////////////////////////////////////////////////////////////
##!
##! \file nddStressTest.mk
##! \brief Build file for the Nand Stress Test.
##!
##!
##! Copyright (c) SigmaTel, Inc. Unpublished
##!
##! SigmaTel, Inc.
##! Proprietary  Confidential
##!
##! This source code and the algorithms implemented therein constitute
##! confidential information and may comprise trade secrets of SigmaTel, Inc.
##! or its associates, and any use thereof is subject to the terms and
##! conditions of the Confidential Disclosure Agreement pursuant to which this
##! source code was originally received.
##!
##/////////////////////////////////////////////////////////////////////////////
ifndef VERSION
VERSION=999.999
endif

# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
PROJ    = nddStressTest
# relative pointer to SDK root directory \Mp3BaseCc
RELBASE_PATH        =..\..\..
ROOT                = $(RELBASE_PATH)

# these macros are project dependent
PROJ_PATH           = $(ROOT)\test\NANDDataDriveStressTest
MAIN_PATH           = $(PROJ_PATH)

#This is needed to use the RTC for system time instead of a timer interrupt.
BLD=$(BLD) -DUSE_RTC_FOR_TIME

#Uncomment the following line to keep tabs on the NAND Data Drive routines
BLD=$(BLD) -DNDD_COUNT_ROUTINES

#Uncomment the following line to run the NAND Data Drive Stress Test
BLD=$(BLD) -DNDD_STRESS_TEST

#########################################
#
include $(ROOT)\etc\options.mk
#########################################
#
include $(ROOT)\etc\standard.mk
#########################################
#
include $(ROOT)\etc\Stringlib.mk
#########################################
#
include $(ROOT)\etc\registers.mk
#########################################
######################################
include $(RELBASE_PATH)\etc\sysserialnumber.mk
######################################
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
include $(ETC_PATH)\display_task.mk
#########################################
#
include $(ETC_PATH)\framebuffer.mk
#########################################
#
include $(ETC_PATH)\epson_sed15xx.mk
#########################################
#
include $(ROOT)\etc\NANDDataDrive.mk
#########################################
#
include $(ROOT)\etc\NANDCommonDdi.mk
#########################################
#
include $(ROOT)\etc\NANDSystemDrive.mk
#########################################
#
include $(ROOT)\etc\NANDMedia.mk
#########################################
#
include $(ROOT)\etc\NANDHal.mk
#########################################
#########################################
#
include $(ROOT)\etc\PortHil.mk
#########################################
#
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
#
include $(ROOT)\etc\ResourceManager.mk
#########################################

CC_OPTS = $(CC_OPTS) $(C_ASM_OPTS)
LK_OPTS = $(LK_OPTS) -u SysWaitOnObject -u FSysReleaseObject -u NANDSystemDriveApi

# must follow standard.mk, where base INC_ALL gets defined
INC_ALL=-I$(PROJRSRC_PATH);$(COMMON_PROJ_PATH);$(OUTPUT_PATH);$(OUTPUT_INCLUDE_PATH);$(FAT12INC_PATH);$(INC_ALL);$(PROJ_PATH);
INC_ALL_C = -Wc-I$(OUTPUT_INCLUDE_PATH) $(INC_ALL_C)

OBJLIST =   $(OBJLIST)\
            $(OBJ_PATH)\task0.obj \
            $(OBJ_PATH)\flashtiming.obj\
            $(OBJ_PATH)\main.obj \
            $(OBJ_PATH)\kernelproject.obj \
            $(OBJ_PATH)\systime.obj\
            $(OBJ_PATH)\nddstresstest_resource.obj\
            $(OBJ_PATH)\miscproject.obj\
            $(OBJ_PATH)\pllspeed.obj

MODLIST =   
MODULELIST = 
DEBUGABSLIST= 

MAKELIST =  $(OBJLIST) $(ALLLIBLIST)

ALL: $(OUTPUT_PATH)\resource.asm $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).s
#ALL : $(OUTPUT_PATH)\$(PROJ).abs   $(DEBUGABSLIST)
# remove comment before $(DEBUGABSLIST) above to build debug .abs

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).s" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System S-rec 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -S$@=all -V$(VERSION) -OFFE8=1818 -OFFE9=1818 
        @echo Encrypt $(PROJ).s to nddStressTest.sb
        @$(BIN_PATH)\stosbcon "$(OUTPUT_PATH)\$(PROJ).s" $(OUTPUT_PATH)\nddStressTest.sb
    copy /Y $(OUTPUT_PATH)\nddStressTest.sb $(OUTPUT_PATH)\stmpsys.sb

"$(OUTPUT_PATH)\$(PROJ).abs"                 : "$(OUTPUT_PATH)\$(PROJ).out" stmpsys.dsc 
    @echo Creating IEEE-695 absolute file $@
        @$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out

"$(OUTPUT_PATH)\$(PROJ).out" : $(PROJ).mk stmp3500.dsc stmpsys.dsc $(MAKELIST)
    @echo Linking $(PROJ).out
        @$(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(LIB_OPT_LIST) $(match .a $!)

$(OBJ_PATH)\nddstresstest_resource.obj : $(PROJ_PATH)\nddstresstest_resource.asm $(OUTPUT_PATH)\resource.asm

#///////////////////////////////////////////////////////////////////////////////
# Debug Module Links
#      These links provide an .abs that can be used to load source-level symbols for each overlaid module
#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
# Project-specific files
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\task0.obj" : $(PROJ_PATH)\task0.c $(OUTPUT_INCLUDE_PATH)\resource.h  $(OUTPUT_INCLUDE_PATH)\project.h $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h  $(OUTPUT_INCLUDE_PATH)\regsrtc.h
"$(OBJ_PATH)\kernelproject.obj" : $(PROJ_PATH)\kernelproject.c $(OUTPUT_INCLUDE_PATH)\resource.h $(OUTPUT_INCLUDE_PATH)\project.h
"$(OBJ_PATH)\ddiproject.obj" : $(PROJ_PATH)\ddiproject.c 
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h  
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\project.h
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\errors.h
"$(OBJ_PATH)\nandporthilproject.obj" : $(PROJ_PATH)\nandporthilproject.c
"$(OBJ_PATH)\systime.obj" : $(PROJ_PATH)\systime.asm
"$(OBJ_PATH)\pllspeed.obj" : $(PROJ_PATH)\pllspeed.asm
"$(OBJ_PATH)\miscproject.obj" : $(PROJ_PATH)\miscproject.c
"$(OBJ_PATH)\main.obj" : $(PROJ_PATH)\main.c $(OUTPUT_INCLUDE_PATH)\regsgpflash.h

#///////////////////////////////////////////////////////////////////////////////
# General System
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\const.obj : $(SYS_PATH)\data_management\const.asm

"$(OBJ_PATH)\flashtiming.obj" : "$(SYS_COMMON_PATH)\flashtiming.c"
"$(OBJ_PATH)\flashtiming.obj" : $(INC_PATH)\types.h
"$(OBJ_PATH)\flashtiming.obj" : $(OUTPUT_INCLUDE_PATH)\regsemc.h
"$(OBJ_PATH)\flashtiming.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpflash.h

#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\resource.asm" : $(OUTPUT_INCLUDE_PATH)\resource.inc  $(RSRCLIST)
    @echo Creating $@
    @$(BIN_PATH)\rsclink.EXE -h$(match .inc $!) \
    -i$(ROOT)\fonts\iso8859\sigmatel_8 \
    -i$(ROOT)\fonts\iso8859\ \
    -i$(ROOT)\test\testsupport\rsrc \
    -i$(RESOURCE_PATH)\shingyih \
    -i$(RESOURCE_PATH)\misc \
    -i$(OUTPUT_RSRC_PATH) \
    -i$(PROJRSRC_PATH) \
    -o$(OUTPUT_PATH)\resource.bin \
     -a
    @$(BIN_PATH)\resparse $(OUTPUT_PATH)\resource.bin $(OUTPUT_PATH)\resource.txt
