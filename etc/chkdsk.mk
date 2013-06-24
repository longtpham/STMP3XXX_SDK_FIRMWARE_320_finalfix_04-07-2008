ifndef CHKDSK_MK
CHKDSK_MK = TRUE
# This is the makefile for the chkdsk utility
# To use chkdsk utility you must have at least one device driver (SM, MMC)

include $(ROOT)\etc\standard.mk


CHKDSK_PATH = $(ROOT)\FileSystem\chkdsk
INC_FAT32= $(ROOT)\FileSystem\Fat32\h

INC_CHKDSK=$(CHKDSK_PATH)\include
INC_ALL=$(INC_ALL);$(INC_CHKDSK);$INC_FAT32
INC_ALL_C =$(INC_ALL_C) -I$(INC_CHKDSK) -I$(INC_FAT32)


RESOURCELIST = $(RESOURCELIST)\
		$(OUTPUT_RSRC_PATH)\chkdskstartup.src\
		$(OUTPUT_RSRC_PATH)\chkdskstartupy.src
		
RESOURCE_INC_LIST = $(RESOURCE_INC_LIST) \
 $(SYMBOLS_PATH)\chkdsk_resource.inc		

CHKDSKLIST = \
    "$(OBJ_PATH)\chkdsk.obj"\
    "$(OBJ_PATH)\cwrapfunc.obj"\
    "$(OBJ_PATH)\fatutils.obj" \
    "$(OBJ_PATH)\chkdskload.obj" \
    "$(OBJ_PATH)\testchkdsk.obj"

#    $(OBJ_PATH)\fsddinit.obj
    
    
OBJLIST = $(OBJLIST) \
          $(CHKDSKLIST)\
     	  $(OBJ_PATH)\bitio.obj
	   
OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\chkdsk_resource.obj \	   

"$(OBJ_PATH)\chkdsk.obj"        : "$(CHKDSK_PATH)\chkdsk.c" $(CHKDSK_PATH)\include\chkdsk.h
"$(OBJ_PATH)\cwrapfunc.obj"     : "$(CHKDSK_PATH)\cwrapfunc.c"
"$(OBJ_PATH)\fatutils.obj"      : "$(CHKDSK_PATH)\FatUtils.c" $(CHKDSK_PATH)\include\chkdsk.h
"$(OBJ_PATH)\fatutils.obj"      : "$(OUTPUT_INCLUDE_PATH)\regsswizzle.h"
"$(OBJ_PATH)\bitio.obj"         : "$(SYS_COMMON_PATH)\bitio.c" $(CHKDSK_PATH)\include\chkdsk.h

"$(OBJ_PATH)\chkdskload.obj"    : "$(CHKDSK_PATH)\chkdskload.asm" $(CHKDSK_PATH)\include\chkdsk.h
"$(OBJ_PATH)\testchkdsk.obj"	: "$(CHKDSK_PATH)\testchkdsk.c" $(CHKDSK_PATH)\include\chkdsk.h




$(OBJ_PATH)\chkdsk_resource.obj : $(RESOURCE_INC_LIST) 
$(OBJ_PATH)\chkdsk_resource.obj : $(SYMBOLS_PATH)\chkdsk_resource.asm 

# Point out dependencies for resource file.
$(OUTPUT_RSRC_PATH)\chkdskstartup.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\chkdskstartup.loc
	@echo Extract $@
        $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)

$(OUTPUT_RSRC_PATH)\chkdskstartupy.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\chkdskstartup.loc
	@echo Extract $@
        $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)

endif
