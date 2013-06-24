#This mp3 implementation depends highly on the playlist1 implementation.  Many data structures are 
#shared between the two (particularly the content tree)


INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\mp3filename

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\mp3filename.obj

CODEBANKLIST =$(CODEBANKLIST)\
             $(OUTPUT_RSRC_PATH)\mp3_implementation.src\



$(OBJ_PATH)\mp3filename.obj : $(SYS_COMMON_PATH)\mp3filename\mp3filename.c
$(OBJ_PATH)\mp3filename.obj : $(OUTPUT_INCLUDE_PATH)\messages.h


"$(OUTPUT_RSRC_PATH)\mp3_implementation.src" 	: $(OUTPUT_PATH)\$(PROJ).abs $(SYS_COMMON_PATH)\mp3filename\mp3filename.loc
        @echo Extract mp3 Implementation 
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

