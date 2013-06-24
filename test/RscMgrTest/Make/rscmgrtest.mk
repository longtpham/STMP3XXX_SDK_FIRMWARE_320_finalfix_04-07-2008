PROJ        = rscmgrtest
ROOT        = ..\..\..
PROJ_PATH   = $(ROOT)\test\rscmgrtest
MAIN_PATH   = $(ROOT)\test\rscmgrtest
PROJECT_H   = $(OUTPUT_INCLUDE_PATH)\project.h

#########################################
#
include $(ROOT)\etc\options.mk
#########################################
#
include $(ROOT)\etc\standard.mk
#########################################


INC_ALL=-I$(PROJRSRC_PATH);$(PROJSYS_PATH);$(OUTPUT_PATH);$(INC_ALL);$(PROJSUPPORT_PATH)
INC_ALL_C = -Wc-I$(PROJ_PATH) -Wc-I$(OUTPUT_INCLUDE_PATH) -Wc-I$(INC_PATH) -Wc-I$(SYS_COMMON_PATH) -Wc-I$(PROJSUPPORT_PATH)


#
include $(ETC_PATH)\includes.mk
#########################################
#
include $(ROOT)\etc\ddildl.mk
#########################################
#
include $(ETC_PATH)\kernel.mk
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

include $(ROOT)\etc\includes.mk

ALL: $(PROJECT_H) $(OUTPUT_PATH)\$(PROJ).s 



OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\rscmgrtesttask.obj \
    $(OBJ_PATH)\sysinittask.obj \
    $(OBJ_PATH)\kernelproject.obj \
    $(OBJ_PATH)\rscmgrtest_resource.obj 

ifdef D_D3410
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\cfporthil_resource.obj
endif

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
# Module code extraction
#///////////////////////////////////////////////////////////////////////////////

"$(OUTPUT_PATH)\$(PROJ).s" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System S-rec 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -S$@=all -V$(VERSION) -OFFE8=1818 -OFFE9=1818 
        @echo Encrypt $(PROJ).s to $(PROJ).sb
        @$(BIN_PATH)\stosbcon "$(OUTPUT_PATH)\$(PROJ).s" $(OUTPUT_PATH)\$(PROJ).sb

#$(OUTPUT_PATH)\$(PROJ).s : $(OUTPUT_PATH)\$(PROJ).abs
#    @$(EXTRACT) 


$(OBJ_PATH)\rscmgrtest_resource.obj : $(PROJ_PATH)\rscmgrtest_resource.asm $(OUTPUT_PATH)\resource.asm

#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////

$(OUTPUT_PATH)\$(PROJ).abs : $(OUTPUT_PATH)\$(PROJ).out testdflt.dsc 
    @echo Creating IEEE-695 absolute file $@
        @$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out

$(OUTPUT_PATH)\$(PROJ).out : testdflt.dsc $(OBJLIST) $(PROJ).mk $(PROJECT_H) $(ALLLIBLIST)
    @echo Linking $(OUTPUT_PATH)\$(PROJ)
        $(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(match .a $!) $(match .sym $!)

"$(OUTPUT_PATH)\resource.asm" : $(OUTPUT_INCLUDE_PATH)\resource.inc  $(RSRCLIST)
    @echo Creating $@
    @$(BIN_PATH)\rsclink.EXE -h$(match .inc $!) \
    -i$(ROOT)\fonts\iso8859\sigmatel_8 \
    -i$(ROOT)\fonts\iso8859\ \
    -i$(ROOT)\test\testsupport\rsrc \
    -i$(RESOURCE_PATH)\shingyih\New_Type_LCD \
    -i$(RESOURCE_PATH)\misc \
    -i$(OUTPUT_RSRC_PATH) \
    -i$(PROJRSRC_PATH) \
    -o$(OUTPUT_PATH)\resource.bin \
     -a
    @$(BIN_PATH)\resparse $(OUTPUT_PATH)\resource.bin $(OUTPUT_PATH)\resource.txt

#///////////////////////////////////////////////////////////////////////////////
# Individual elements
#///////////////////////////////////////////////////////////////////////////////


$(OBJ_PATH)\rscmgrtesttask.obj: $(PROJ_PATH)\rscmgrtesttask.c
$(OBJ_PATH)\sysinittask.obj: $(PROJ_PATH)\sysinittask.c
"$(OBJ_PATH)\ddiproject.obj" : $(PROJ_PATH)\ddiproject.c 
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h  
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\project.h
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\errors.h


$(OBJ_PATH)\kernelproject.obj: $(PROJ_PATH)\kernelproject.c
$(OBJ_PATH)\kernelproject.obj: $(OUTPUT_INCLUDE_PATH)\resource.h

$(OBJ_PATH)\heartbeat.obj : $(PROJ_PATH)\heartbeat.c

ifdef D_D3410
$(OBJ_PATH)\cfporthil_resource.obj : $(SYMBOLS_PATH)\cfporthil_resource.asm
endif
