#This is the makefile for the parser 
#

OBJLIST=$(OBJLIST)\
        $(PARSERLIST)

PARSERLIST  = \
            $(OBJ_PATH)\sysdir.obj\
            $(OBJ_PATH)\parser.obj\
            $(OBJ_PATH)\parser_voice.obj\
            $(OBJ_PATH)\parsermem.obj

MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\parvoimod.src\
             $(OUTPUT_RSRC_PATH)\parmod.src\


# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_LIBS
#///////////////////////////////////////////////////////////////////////////////
# Archive build : libraries (*.a) from object modules (*.obj)
#///////////////////////////////////////////////////////////////////////////////
"$(LIB_PATH)\systemlib.a"			: $(SYSTEMLIST)
	@echo Creating IEEE archive library file for the System Files $@
		@$(AR_EXE) -r $(SYSTEMLIB) $(SYSTEMLIST)

#///////////////////////////////////////////////////////////////////////////////
# System Files
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\sysdir.obj : $(SYS_COMMON_PATH)\sysdir.asm

$(OBJ_PATH)\parser.obj : $(SW_MSGMODULES_PATH)\parser\parser.asm

$(OBJ_PATH)\parser_voice.obj : $(SW_MSGMODULES_PATH)\parser\parser_voice.asm

$(OBJ_PATH)\parsermem.obj : $(SW_MSGMODULES_PATH)\parser\parsermem.asm
endif

"$(OUTPUT_RSRC_PATH)\parmod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Parser Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=PARMOD_Parser_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\parvoimod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Parser Voise Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=PARVOIMOD_Parser_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map
