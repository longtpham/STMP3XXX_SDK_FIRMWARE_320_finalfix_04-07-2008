#kerneltest.mk

PROJ        = kerneltest
ROOT        = ..\..\..
PROJ_PATH   = $(ROOT)\test\kernel
PROJECT_H   = $(ROOT)\inc\project.h
#########################################
#
include $(ROOT)\etc\standard.mk
#########################################
INC_ALL=-I$(PROJRSRC_PATH);$(PROJ_PATH);$(PROJSYS_PATH);$(OUTPUT_PATH);$(INC_ALL);$(PROJSUPPORT_PATH)
INC_ALL_C = -Wc-I$(PROJ_PATH) -Wc-I$(OUTPUT_INCLUDE_PATH) -Wc-I$(INC_PATH) -Wc-I$(SYS_COMMON_PATH) 

CC_OPTS= -Wc-s  $(BLD) -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL) 
AS_OPTS=$(INC_ALL) -OJMPS  -l -LCeIn -DTASKING $(BLD) 


include $(ROOT)\etc\kernel.mk"

ALL: $(PROJECT_H) $(OUTPUT_PATH)\$(PROJ).s

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\sample.obj\
    $(OBJ_PATH)\task0.obj\
    $(OBJ_PATH)\task1.obj\
    $(OBJ_PATH)\task2.obj\
    $(OBJ_PATH)\task3.obj\
    $(OBJ_PATH)\task4.obj\
    $(OBJ_PATH)\heartbeat.obj\

ifdef D_D3410
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\cfporthil_resource.obj
endif

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).out" : $(CPU_PATH)\testdflt.dsc $(OBJLIST) $(ALLLIBLIST) $(PROJ).mk $(PROJECT_H)
    @echo Linking $(PROJ).out
        @$(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(match .a $!) 

"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" $(CPU_PATH)\testdflt.dsc 
	@echo Creating IEEE-695 absolute file $@
		@$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out


$(OUTPUT_PATH)\$(PROJ).s : $(OUTPUT_PATH)\$(PROJ).abs
    @$(EXTRACT) 

#///////////////////////////////////////////////////////////////////////////////
# Project-specific files
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\ddiproject.obj" : $(PROJ_PATH)\ddiproject.c 
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h  
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\project.h
"$(OBJ_PATH)\ddiproject.obj" : $(OUTPUT_INCLUDE_PATH)\errors.h

$(OBJ_PATH)\task0.obj: $(PROJ_PATH)\task0.c

$(OBJ_PATH)\task1.obj: $(PROJ_PATH)\task1.c
    
$(OBJ_PATH)\task2.obj: $(PROJ_PATH)\task2.c

$(OBJ_PATH)\task3.obj: $(PROJ_PATH)\task3.c

$(OBJ_PATH)\task4.obj: $(PROJ_PATH)\task4.c

$(OBJ_PATH)\sample.obj: $(PROJ_PATH)\sample.c

$(OBJ_PATH)\heartbeat.obj : $(PROJ_PATH)\heartbeat.c

ifdef D_D3410
$(OBJ_PATH)\cfporthil_resource.obj : $(SYMBOLS_PATH)\cfporthil_resource.asm
endif

$(PROJECT_H) :$(ROOT)\inc\project.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 
