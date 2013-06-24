#This is the makefile for the Perception Digital MP3 Encoder to be used with STMP3420.
#This file should be included into the project makefile

#This mp3 implementation depends highly on the playlist1 implementation.  Many data structures are 
#shared between the two (particularly the content tree)




MP3_ENC_PATH  	=$(ENC_PATH)\Mp3
ENCMP3_LIB_PATH =$(MP3_ENC_PATH)\lib

INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\mp3filename


MODULELIST = $(MODULELIST)\
            $(OUTPUT_RSRC_PATH)\enc_mp3mod.src\
            $(OUTPUT_RSRC_PATH)\enc_mp3p.src\
            $(OUTPUT_RSRC_PATH)\enc_mp3x.src\
            $(OUTPUT_RSRC_PATH)\enc_mp3y.src\
   	        $(OUTPUT_RSRC_PATH)\string_mp3.src\


CODEBANKLIST =$(CODEBANKLIST)\
             $(OUTPUT_RSRC_PATH)\mp3_implementation.src\


ifdef PD_MP3_ENCODE

OBJLIST=$(OBJLIST)\
        $(ENCMP3LIST)
          
ALLLIBLIST = $(ALLLIBLIST)\
           $(ENCMP3LIBLIST)

ENCMP3LIBLIST = \
			 $(ENCMP3_LIB_PATH)\encmp3core.a

ENCMP3LIST = \
            $(OBJ_PATH)\enc_mp3_overlay.obj\
            $(OBJ_PATH)\enc_mp3_mem.obj\
            $(OBJ_PATH)\mp3filename.obj


#///////////////////////////////////////////////////////////////////////////////
# MP3 Encoder
#///////////////////////////////////////////////////////////////////////////////
$(OBJ_PATH)\enc_mp3_overlay.obj : $(MP3_ENC_PATH)\enc_mp3_overlay.asm
$(OBJ_PATH)\enc_mp3_overlay.obj : $(INC_PATH)\encmp3equ.inc
$(OBJ_PATH)\enc_mp3_overlay.obj : $(COMMON_PROJ_PATH)\project.inc

$(OBJ_PATH)\mp3filename.obj : $(SYS_COMMON_PATH)\mp3filename\mp3filename.c
$(OBJ_PATH)\mp3filename.obj : $(OUTPUT_INCLUDE_PATH)\messages.h

$(OBJ_PATH)\enc_mp3_mem.obj : $(MP3_ENC_PATH)\enc_mp3_mem.asm 

$(OUTPUT_INCLUDE_PATH)\encmp3equ.h: $(INC_PATH)\encmp3equ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 


"$(OUTPUT_INCLUDE_PATH)\mp3equ.h ": $(INC_PATH)\mp3equ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_RSRC_PATH)\string_mp3.src        : $(PROJRSRC_PATH)\strings\string_mp3.txt


# Encoder MP3 module & code
"$(OUTPUT_RSRC_PATH)\enc_mp3mod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract MP3 Encoder Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=ENCMOD_Mp3_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\enc_mp3p.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract MP3 Enc P
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=ENCAPP_EncMp3NonOverlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\enc_mp3x.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\enc_mp3y.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract MP3 Enc X & MP3 Enc Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\enc_mp3_abs_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
# contains : ENCAPP_EncMp3Mem_X|DECAPP_EncMp3NonOverlay_X        
# contains : ENCAPP_EncMp3Mem_Y|DECAPP_EncMp3NonOverlay_Y        


"$(OUTPUT_RSRC_PATH)\mp3_implementation.src" 	: $(OUTPUT_PATH)\$(PROJ).abs $(SYS_COMMON_PATH)\mp3filename\mp3filename.loc
        @echo Extract mp3 Implementation 
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

else
#### create dummy resources so there are no changes to resource.inc
"$(OUTPUT_RSRC_PATH)\enc_mp3mod.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\enc_mp3mod.src

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



