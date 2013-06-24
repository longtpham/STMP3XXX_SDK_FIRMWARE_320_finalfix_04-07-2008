#This voice implementation depends highly on the playlist1 implementation.  Many data structures are 
#shared between the two (particularly the content tree)


INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\voice2

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\voice2.obj

CODEBANKLIST =$(CODEBANKLIST)\
             $(OUTPUT_RSRC_PATH)\voice_implementation.src\



$(OBJ_PATH)\voice2.obj : $(SYS_COMMON_PATH)\voice2\voice2.c
$(OBJ_PATH)\voice2.obj : $(OUTPUT_INCLUDE_PATH)\messages.h


"$(OUTPUT_RSRC_PATH)\voice_implementation.src" 	: $(OUTPUT_PATH)\$(PROJ).abs $(SYS_COMMON_PATH)\voice2\voice2.loc
        @echo Extract Voice Implementation 
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

