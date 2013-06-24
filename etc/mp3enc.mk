MP3_ENC_PATH = $(ENC_PATH)\Mp3
MP3ENC_ALG_PATH = $(ADPCM_ALGO_ENC_PATH)\mp3
ENCMP3_LIB_PATH = $(MP3ENC_ALG_PATH)\lib

INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\mp3filename


MODULELIST = $(MODULELIST)\
            $(OUTPUT_RSRC_PATH)\enc_mp3mod.src\
            $(OUTPUT_RSRC_PATH)\enc_mp3p.src\
            $(OUTPUT_RSRC_PATH)\enc_mp3x.src\
            $(OUTPUT_RSRC_PATH)\enc_mp3y.src\
   	        $(OUTPUT_RSRC_PATH)\string_mp3.src\


CODEBANKLIST =$(CODEBANKLIST)\
             $(OUTPUT_RSRC_PATH)\mp3_implementation.src\


ifdef MP3_ENCODE

OBJLIST=$(OBJLIST)\
        $(ENCMP3LIST)
          
ALLLIBLIST = $(ALLLIBLIST)\
           $(ENCMP3LIBLIST)

ENCMP3LIBLIST = \
			 $(ENCMP3_LIB_PATH)\mp3encoder.a

ENCMP3LIST = \
            $(OBJ_PATH)\enc_mp3_overlay.obj\
            $(OBJ_PATH)\enc_mp3_nonoverlay.obj\
			$(OBJ_PATH)\enc_mp3_mem.obj\
            $(OBJ_PATH)\mp3encmem.obj\
            $(OBJ_PATH)\mp3filename.obj


#///////////////////////////////////////////////////////////////////////////////
# MP3 Encoder
#///////////////////////////////////////////////////////////////////////////////
$(OBJ_PATH)\enc_mp3_overlay.obj : $(MP3_ENC_PATH)\enc_mp3_overlay.asm
$(OBJ_PATH)\enc_mp3_overlay.obj : $(INC_PATH)\encmp3equ.inc
$(OBJ_PATH)\enc_mp3_overlay.obj : $(COMMON_PROJ_PATH)\project.inc

$(OBJ_PATH)\enc_mp3_nonoverlay.obj : $(MP3_ENC_PATH)\enc_mp3_nonoverlay.asm

$(OBJ_PATH)\enc_mp3_mem.obj : $(MP3_ENC_PATH)\enc_mp3_mem.asm

$(OBJ_PATH)\mp3filename.obj : $(SYS_COMMON_PATH)\mp3filename\mp3filename.c
$(OBJ_PATH)\mp3filename.obj : $(OUTPUT_INCLUDE_PATH)\messages.h

$(OBJ_PATH)\mp3encmem.obj : $(MP3ENC_ALG_PATH)\mp3encmem.asm 


$(OUTPUT_INCLUDE_PATH)\encmp3equ.h: $(INC_PATH)\encmp3equ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 


"$(OUTPUT_INCLUDE_PATH)\mp3equ.h ": $(INC_PATH)\mp3equ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_RSRC_PATH)\string_mp3.src        : $(PROJRSRC_PATH)\strings\string_mp3.txt


"$(OUTPUT_RSRC_PATH)\enc_mp3p.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\enc_mp3x.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\enc_mp3y.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract MP3 Enc X & MP3 Enc Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\enc_mp3_abs_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

"$(OUTPUT_RSRC_PATH)\enc_mp3mod.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mp3_implementation.src" 	: $(OUTPUT_PATH)\$(PROJ).abs $(SYS_COMMON_PATH)\mp3filename\mp3filename.loc
        @echo Extract mp3 Implementation 
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

else
#### create dummy resources so there are no changes to resource.inc
"$(OUTPUT_RSRC_PATH)\enc_mp3p.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\enc_mp3p.src

"$(OUTPUT_RSRC_PATH)\enc_mp3x.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\enc_mp3x.src

"$(OUTPUT_RSRC_PATH)\enc_mp3y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\enc_mp3y.src

"$(OUTPUT_RSRC_PATH)\mp3_implementation.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mp3_implementation.src

"$(OUTPUT_RSRC_PATH)\string_mp3.src"  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_mp3.src
endif



