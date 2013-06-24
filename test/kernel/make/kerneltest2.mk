#kerneltest.mk

PROJ        = kerneltest
ROOT        = ..\..\..
PROJ_PATH   = $(ROOT)\test\kernel
PROJECT_H   = $(OUTPUT_INCLUDE_PATH)\project.h
#########################################
#
include $(ROOT)\etc\standard.mk
#########################################
INC_ALL=-I$(PROJRSRC_PATH);$(PROJ_PATH);$(PROJSYS_PATH);$(OUTPUT_PATH);$(INC_ALL);$(PROJSUPPORT_PATH)
INC_ALL_C = -Wc-I$(PROJ_PATH) -Wc-I$(OUTPUT_INCLUDE_PATH) -Wc-I$(INC_PATH) -Wc-I$(SYS_COMMON_PATH) 

CC_OPTS= -Wc-s  $(BLD) -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL) 
AS_OPTS=$(INC_ALL) -OJMPS  -l -LCeIn -DTASKING $(BLD) 


include $(ETC_PATH)\overlay.mk
#include $(ETC_PATH)\kernel.mk
include $(ETC_PATH)\resource_y.mk

ALL: $(PROJECT_H) $(OUTPUT_PATH)\$(PROJ).s 

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\sample.obj\
    $(OBJ_PATH)\task0.obj\
    $(OBJ_PATH)\task0_region1_bank2.obj\
    $(OBJ_PATH)\task0_region2_bank1.obj\
    $(OBJ_PATH)\task1.obj\
    $(OBJ_PATH)\task2.obj\
    $(OBJ_PATH)\task3.obj\
    $(OBJ_PATH)\task4.obj\
    $(OBJ_PATH)\heartbeat.obj\

ifdef D_D3410
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\cfporthil_resource.obj
endif

CODEBANKLIST = $(CODEBANKLIST)\
    RSRC_REGION1_TASK0_BANK1.src\
    RSRC_REGION1_TASK1_BANK1.src\
    RSRC_REGION1_TASK2_BANK1.src\
    RSRC_REGION2_TASK3_BANK1.src\
    RSRC_REGION2_TASK4_BANK1.src\
    RSRC_REGION2_TASK0_BANK1.src\
    RSRC_REGION1_TASK0_BANK2.src\

    
RSRCLIST = $(RSRCLIST)\
    $(CODEBANKLIST) \


#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).out" : testdflt.dsc $(OBJLIST)  $(PROJ).mk $(PROJECT_H) $(OBJ_PATH)\kerneltest_resource.obj $(ALLLIBLIST)
    @echo Linking $(PROJ).out
        @$(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(match .a $!) 

"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" testdflt.dsc 
	@echo Creating IEEE-695 absolute file $@
		@$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out


$(OUTPUT_PATH)\$(PROJ).s : $(OUTPUT_PATH)\$(PROJ).abs
    @$(EXTRACT) 


$(OBJ_PATH)\kerneltest_resource.obj : $(PROJ_PATH)\kerneltest_resource.asm $(OUTPUT_PATH)\resource.asm 
#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////

$(OUTPUT_PATH)\$(PROJ)_temp.abs : $(OUTPUT_PATH)\$(PROJ)_temp.out testdflt.dsc 
    @echo Creating IEEE-695 absolute file $@
		@$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ)_temp.out

$(OUTPUT_PATH)\$(PROJ)_temp.out : testdflt.dsc $(OBJLIST) $(PROJ).mk $(PROJECT_H) $(OBJ_PATH)\resource_temp.obj $(ALLLIBLIST) 
    @echo Linking $(OUTPUT_PATH)\$(PROJ)_temp
        @$(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(match .a $!) 


$(OBJ_PATH)\resource_temp.obj : $(PROJ_PATH)\resource_temp.asm
    
"$(OUTPUT_PATH)\resource.asm" : $(RSRCLIST) $(PROJ_PATH)\resource.inc 
	@echo Creating $@
    @$(BIN_PATH)\rsclink.EXE -h$(PROJ_PATH)\resource.inc \
	-i$(ROOT)\fonts\iso8859\sigmatel_8 \
	-i$(ROOT)\fonts\iso8859\ \
	-i$(RESOURCE_PATH)\shingyih\New_Type_LCD \
    -i$(RESOURCE_PATH)\misc \
    -i$(OUTPUT_RSRC_PATH) \
	-i$(PROJRSRC_PATH) \
	-o$(OUTPUT_PATH)\resource.bin \
     -a
    @$(BIN_PATH)\resparse $(OUTPUT_PATH)\resource.bin $(OUTPUT_PATH)\resource.txt

RSRC_REGION1_TASK0_BANK1.src: $(OUTPUT_PATH)\$(PROJ)_temp.abs codebanks.loc
RSRC_REGION1_TASK1_BANK1.src: $(OUTPUT_PATH)\$(PROJ)_temp.abs codebanks.loc
RSRC_REGION1_TASK2_BANK1.src: $(OUTPUT_PATH)\$(PROJ)_temp.abs codebanks.loc
RSRC_REGION2_TASK3_BANK1.src: $(OUTPUT_PATH)\$(PROJ)_temp.abs codebanks.loc
RSRC_REGION2_TASK4_BANK1.src: $(OUTPUT_PATH)\$(PROJ)_temp.abs codebanks.loc
RSRC_REGION2_TASK0_BANK1.src: $(OUTPUT_PATH)\$(PROJ)_temp.abs codebanks.loc
RSRC_REGION1_TASK0_BANK2.src: $(OUTPUT_PATH)\$(PROJ)_temp.abs codebanks.loc
        @echo Extract Codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ)_temp.map -P$(OUTPUT_RSRC_PATH)\


#///////////////////////////////////////////////////////////////////////////////
# Individual elements
#///////////////////////////////////////////////////////////////////////////////


$(OBJ_PATH)\task0.obj: $(PROJ_PATH)\task0.c

$(OBJ_PATH)\task0_region1_bank2.obj: $(PROJ_PATH)\task0_region1_bank2.c
$(OBJ_PATH)\task0_region2_bank1.obj: $(PROJ_PATH)\task0_region2_bank1.c


$(OBJ_PATH)\task1.obj: $(PROJ_PATH)\task1.c
    
$(OBJ_PATH)\task2.obj: $(PROJ_PATH)\task2.c

$(OBJ_PATH)\task3.obj: $(PROJ_PATH)\task3.c

$(OBJ_PATH)\task4.obj: $(PROJ_PATH)\task4.c

$(OBJ_PATH)\sample.obj: $(PROJ_PATH)\sample.c
$(OBJ_PATH)\sample.obj: $(OUTPUT_INCLUDE_PATH)\resource.h

$(OBJ_PATH)\heartbeat.obj : $(PROJ_PATH)\heartbeat.c

ifdef D_D3410
$(OBJ_PATH)\cfporthil_resource.obj : $(SYMBOLS_PATH)\cfporthil_resource.asm
endif

ifndef __RESOURCE_H
__RESOURCE_H=TRUE
$(OUTPUT_INCLUDE_PATH)\resource.h :$(PROJ_PATH)\resource.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif

ifndef CONVERT_PROJECT_H
CONVERT_PROJECT_H=TRUE
$(OUTPUT_INCLUDE_PATH)\project.h :$(PROJ_PATH)\project.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
endif