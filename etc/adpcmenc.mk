#This is the makefile for the ADPCM Encoder.
#
#This file should be included into the project makefile and 



OBJLIST=$(OBJLIST)\
        $(ENCADPCMLIST)
        
INC_ALL = $(INC_ALL);$(ADC_PATH)        

ENCADPCMLIST = \
            $(OBJ_PATH)\enc_adpcm_nonoverlay.obj\
            $(OBJ_PATH)\enc_adpcm_overlay.obj\
            $(OBJ_PATH)\enc_adpcm_mem.obj\
            $(OBJ_PATH)\imaadpcm_encode.obj\
            $(OBJ_PATH)\msadpcm_encode.obj\
            $(OBJ_PATH)\pcm_encode.obj\
            $(OBJ_PATH)\waveenc.obj\
			$(OBJ_PATH)\EncoderProperties.obj\
            $(OBJ_PATH)\bitstreamenc.obj\
            $(OBJ_PATH)\detectsilence.obj\    
            $(OBJ_PATH)\silenceprocess.obj\
            $(OBJ_PATH)\encodercommon.obj


MODULELIST = $(MODULELIST)\
            $(OUTPUT_RSRC_PATH)\encadpcmimamod.src\
            $(OUTPUT_RSRC_PATH)\enc_adpcmp.src\
            $(OUTPUT_RSRC_PATH)\enc_adpcmx.src\
            $(OUTPUT_RSRC_PATH)\enc_adpcmy.src\

FUNCLETSRCLIST =    $(FUNCLETSRCLIST)\
            $(OUTPUT_RSRC_PATH)\Funclet_EncSetProperties.src\


#///////////////////////////////////////////////////////////////////////////////
# ADPCM Encoder
#///////////////////////////////////////////////////////////////////////////////
$(OBJ_PATH)\enc_adpcm_overlay.obj : $(ADPCM_ENC_PATH)\enc_adpcm_overlay.asm
$(OBJ_PATH)\enc_adpcm_overlay.obj : $(INC_PATH)\encadpcmequ.inc
$(OBJ_PATH)\enc_adpcm_overlay.obj : $(COMMON_PROJ_PATH)\project.inc

$(OBJ_PATH)\enc_adpcm_nonoverlay.obj : $(ADPCM_ENC_PATH)\enc_adpcm_nonoverlay.asm 
$(OBJ_PATH)\enc_adpcm_nonoverlay.obj : $(ADC_PATH)\adcdrv.asm
$(OBJ_PATH)\enc_adpcm_nonoverlay.obj : $(INC_PATH)\encadpcmequ.inc
$(OBJ_PATH)\enc_adpcm_nonoverlay.obj : $(COMMON_PROJ_PATH)\project.inc

$(OBJ_PATH)\enc_adpcm_mem.obj : $(ADPCM_ENC_PATH)\enc_adpcm_mem.asm 

$(OBJ_PATH)\imaadpcm_encode.obj         : $(ADPCM_ALGO_ENC_PATH)\imaadpcm\src\imaadpcm_encode.asm
$(OBJ_PATH)\msadpcm_encode.obj          : $(ADPCM_ALGO_ENC_PATH)\msadpcm\src\msadpcm_encode.asm 
$(OBJ_PATH)\pcm_encode.obj              : $(ALGO_COMMON_PATH)\pcm_encode.asm
$(OBJ_PATH)\waveenc.obj                 : $(ALGO_COMMON_PATH)\waveenc.c
$(OBJ_PATH)\bitstreamenc.obj            : $(ALGO_COMMON_PATH)\bitstreamenc.asm
$(OBJ_PATH)\detectsilence.obj : $(ROOT)\System\MsgModules\Software\Encoder\detectsilence.c
$(OBJ_PATH)\silenceprocess.obj : $(ROOT)\System\MsgModules\Software\Encoder\silenceprocess.asm
$(OBJ_PATH)\encodercommon.obj : $(ROOT)\System\MsgModules\Software\Encoder\encodercommon.asm


$(OBJ_PATH)\encoderproperties.obj : $(ENC_PATH)\encoderproperties.c 
$(OBJ_PATH)\encoderproperties.obj : $(OUTPUT_INCLUDE_PATH)\adpcmequ.h 
$(OBJ_PATH)\encoderproperties.obj : $(OUTPUT_INCLUDE_PATH)\encadpcmequ.h 
$(OBJ_PATH)\encoderproperties.obj : $(INC_PATH)\encoderproperties.h 
$(OBJ_PATH)\encoderproperties.obj : $(OUTPUT_INCLUDE_PATH)\sysequ.h

$(OUTPUT_INCLUDE_PATH)\encadpcmequ.h: $(INC_PATH)\encadpcmequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 


"$(OUTPUT_INCLUDE_PATH)\adpcmequ.h ": $(INC_PATH)\adpcmequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 



# Encoder Adpcm Ima module & code
"$(OUTPUT_RSRC_PATH)\encadpcmimamod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract ADPCM Encoder Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=ENCMOD_AdpcmIma_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\enc_adpcmx.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\enc_adpcmy.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\enc_adpcmp.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract ADPCM Enc X & ADPCM Enc Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\enc_abs_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
# contains : ENCAPP_EncAdpcmMem_X|DECAPP_EncAdpcmImaNonOverlay_X        

"$(OUTPUT_RSRC_PATH)\Funclet_EncSetProperties.src" : $(OUTPUT_PATH)\$(PROJ).abs
       @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_EncSetProperties_P -f$(OUTPUT_PATH)\$(PROJ).map



