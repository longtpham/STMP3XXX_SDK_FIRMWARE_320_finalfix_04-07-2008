
ifndef D_AUDIBLEDEC
OBJLIST=    $(OBJLIST)\

MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\audibledecmod.src\
             $(OUTPUT_RSRC_PATH)\audiblemp3x.src\
             $(OUTPUT_RSRC_PATH)\audiblemp3y.src\
             $(OUTPUT_RSRC_PATH)\audiblemp3p.src

"$(OUTPUT_RSRC_PATH)\audibledecmod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL audibledecmod.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audibledecmod.src

"$(OUTPUT_RSRC_PATH)\audiblemp3x.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL audiblemp3x.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblemp3x.src
        
"$(OUTPUT_RSRC_PATH)\audiblemp3y.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL audiblemp3y.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblemp3y.src

"$(OUTPUT_RSRC_PATH)\audiblemp3p.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL audiblemp3p.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblemp3p.src
        
else
AUD_PATH			= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\AASource
AUD_PATH2			= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\AADevProject
AUD_INC1			= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\IncludePublic
AUD_ACELP_PATH		= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp
AUD_MP3_PATH		= ..\..\..\..\..\Algorithms\Decoders\AudibleMp3\Audible_Mp3_Src

CCFLAGS = $(CCFLAGS) -I$(AUD_PATH) -I$(AUD_INC1) -I$(AUD_PATH2) -I$(AUD_MP3_PATH)


OBJLIST=$(OBJLIST)\
        $(DECAUDIBLEMP3LIST)

#GENERALLIB = $(GENERALLIB)\
#           $(AUDIBLEMP3LIBLIST)

# NOTE: If a build is desired with the Parthus volume and tone
# control included in the mp3 build, then replace "coremp3_NoVolTone.a"
# file with "coremp3.a" below:

#AUDIBLEMP3LIBLIST = \
#			 $(MP3_LIB_PATH)\coremp3_NoVolTone.a\
#			 $(MP3_LIB_PATH)\bit_lib.a\
#			 $(MP3_LIB_PATH)\Layer12.a


MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\audibledecmod.src\
             $(OUTPUT_RSRC_PATH)\audiblemp3x.src\
             $(OUTPUT_RSRC_PATH)\audiblemp3y.src\
             $(OUTPUT_RSRC_PATH)\audiblemp3p.src


DECAUDIBLEMP3LIST = \
             $(OBJ_PATH)\audiblemp3dec.obj\
             $(OBJ_PATH)\audiblemp3mem.obj\
             $(OBJ_PATH)\audible_decoder_nonoverlay.obj\
             $(OBJ_PATH)\audible_decoder_overlay.obj\
             $(OBJ_PATH)\audibledecodermem.obj\
             $(OBJ_PATH)\AudibleMP3Dec_c.obj\
             $(OBJ_PATH)\audiblemp3overlay.obj\
             $(OBJ_PATH)\audiblemp3nonoverlay.obj\
             $(OBJ_PATH)\audiblemp3common.obj


# --------------------------------------------------------------------
"$(OUTPUT_INCLUDE_PATH)\AudibleMP3Dec.h" : $(INC_PATH)\AudibleMP3dec.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@


#///////////////////////////////////////////////////////////////////////////////
# Audible MP3 Decoder
#///////////////////////////////////////////////////////////////////////////////
 
"$(OBJ_PATH)\AudibleMP3Dec_c.obj" : $(AUDIBLE_MP3_ALG_PATH)\audiblemp3dec_c.c "$(OUTPUT_INCLUDE_PATH)\AudibleMP3Dec.h"

"$(OBJ_PATH)\audiblemp3overlay.obj" : $(AUD_MP3_PATH)\audiblemp3overlay.c $(AUD_MP3_PATH)\audiblemp3overlay.h $(AUD_MP3_PATH)\audiblemp3common.h

"$(OBJ_PATH)\audiblemp3common.obj" : $(AUD_MP3_PATH)\audiblemp3common.c $(AUD_MP3_PATH)\audiblemp3common.h

"$(OBJ_PATH)\audiblemp3nonoverlay.obj" : $(AUD_MP3_PATH)\audiblemp3nonoverlay.c $(AUD_MP3_PATH)\audiblemp3nonoverlay.h $(AUD_MP3_PATH)\audiblemp3common.h

$(OBJ_PATH)\audiblemp3dec.obj : $(AUDIBLE_MP3_ALG_PATH)\audiblemp3dec.asm
 
$(OBJ_PATH)\audiblemp3mem.obj : $(AUDIBLE_MP3_ALG_PATH)\audiblemp3mem.asm
 
$(OBJ_PATH)\audible_decoder_overlay.obj : $(AUDIBLE_MP3_PATH)\audible_decoder_overlay.asm
 
$(OBJ_PATH)\audible_decoder_nonoverlay.obj : $(AUDIBLE_MP3_PATH)\audible_decoder_nonoverlay.asm
 
$(OBJ_PATH)\audibledecodermem.obj : $(AUDIBLE_MP3_PATH)\audibledecodermem.asm

# Mp3 module & code
"$(OUTPUT_RSRC_PATH)\audibledecmod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Decoder Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=Audible_DECMOD_DecoderOverlay_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map


"$(OUTPUT_RSRC_PATH)\audiblemp3p.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract Decoder P "
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\audible_mp3_abs_rsrc_p.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

"$(OUTPUT_RSRC_PATH)\audiblemp3x.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\audiblemp3y.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract Decoder X asdasd& Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\audible_mp3_abs_rsrc_x_y.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
########
endif


