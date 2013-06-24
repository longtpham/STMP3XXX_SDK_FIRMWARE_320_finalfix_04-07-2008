#This is the makefile for the MP3 Decoder.
#
#This file should be included into the project makefile and 
# $(DECMP3LIB) must be included in the 
# dependancy list of the .out file.


OBJLIST=$(OBJLIST)\
        $(DECMP3LIST)

GENERALLIB = $(GENERALLIB)\
           $(MP3LIBLIST)

# NOTE: If a build is desired with the Parthus volume and tone
# control included in the mp3 build, then replace "coremp3_NoVolTone.a"
# file with "coremp3.a" below:

MP3LIBLIST = \
			 $(MP3_LIB_PATH)\coremp3_NoVolTone.a\
			 $(MP3_LIB_PATH)\bit_lib.a\
			 $(MP3_LIB_PATH)\Layer12.a



MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\decmod.src\
             $(OUTPUT_RSRC_PATH)\mp3p.src\
             $(OUTPUT_RSRC_PATH)\mp3x.src\
             $(OUTPUT_RSRC_PATH)\mp3y.src


DECMP3LIST = \
             $(OBJ_PATH)\mp3dec.obj\
             $(OBJ_PATH)\mp3mem.obj\
             $(OBJ_PATH)\decoder_nonoverlay.obj\
             $(OBJ_PATH)\decoder_overlay.obj\
             $(OBJ_PATH)\decodermem.obj


#///////////////////////////////////////////////////////////////////////////////
# MP3 Decoder
#///////////////////////////////////////////////////////////////////////////////
 
$(OBJ_PATH)\mp3dec.obj : $(MP3_ALG_PATH)\mp3dec.asm
 
$(OBJ_PATH)\mp3mem.obj : $(MP3_ALG_PATH)\mp3mem.asm
 
$(OBJ_PATH)\decoder_overlay.obj : $(MP3_PATH)\decoder_overlay.asm
 
$(OBJ_PATH)\decoder_nonoverlay.obj : $(MP3_PATH)\decoder_nonoverlay.asm
 
$(OBJ_PATH)\decodermem.obj : $(MP3_PATH)\decodermem.asm

# Mp3 module & code
"$(OUTPUT_RSRC_PATH)\decmod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Decoder Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=DECMOD_DecoderOverlay_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\mp3p.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mp3x.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mp3y.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract Decoder P & X & Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\mp3_abs_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\




