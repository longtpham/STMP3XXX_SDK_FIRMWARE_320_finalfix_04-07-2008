#This voice implementation depends highly on the playlist1 implementation.  Many data structures are 
#shared between the two (particularly the content tree)


INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\record

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\sysrecord.obj

CODEBANKLIST =$(CODEBANKLIST)\
             $(OUTPUT_RSRC_PATH)\sysrecord.src\



$(OBJ_PATH)\sysrecord.obj : $(SYS_COMMON_PATH)\record\sysrecord.c
$(OBJ_PATH)\sysrecord.obj : $(OUTPUT_INCLUDE_PATH)\messages.h


"$(OUTPUT_RSRC_PATH)\sysrecord.src" 	: $(OUTPUT_PATH)\$(PROJ).abs $(SYS_COMMON_PATH)\record\sysrecord.loc
        @echo Extract SysRecord 
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

