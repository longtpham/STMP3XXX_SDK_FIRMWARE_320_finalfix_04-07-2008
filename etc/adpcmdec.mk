#This is the makefile for the ADPCM Decoder.
#
#This file should be included into the project makefile and 


OBJLIST=$(OBJLIST)\
        $(DECADPCMLIST)        


MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\decadpcmimamod.src\
                 $(OUTPUT_RSRC_PATH)\dec_adpcmp.src\
                 $(OUTPUT_RSRC_PATH)\dec_adpcmx.src\
                 $(OUTPUT_RSRC_PATH)\dec_adpcmy.src

DECADPCMLIST = \
            $(OBJ_PATH)\dec_adpcm_nonoverlay.obj\
            $(OBJ_PATH)\dec_adpcm_overlay.obj\
            $(OBJ_PATH)\dec_adpcm_mem.obj\
			$(OBJ_PATH)\msadpcm_decode.obj\  
            $(OBJ_PATH)\imaadpcm_decode.obj\ 
            $(OBJ_PATH)\pcm_decode.obj\ 
            $(OBJ_PATH)\wavedec.obj\
            $(OBJ_PATH)\bitstreamdec.obj




#///////////////////////////////////////////////////////////////////////////////
# ADPCM Decoder
#///////////////////////////////////////////////////////////////////////////////
$(OBJ_PATH)\dec_adpcm_overlay.obj   : $(ADPCM_DEC_PATH)\dec_adpcm_overlay.asm

$(OBJ_PATH)\dec_adpcm_nonoverlay.obj: $(ADPCM_DEC_PATH)\dec_adpcm_nonoverlay.asm

$(OBJ_PATH)\dec_adpcm_mem.obj       : $(ADPCM_DEC_PATH)\dec_adpcm_mem.asm
$(OBJ_PATH)\msadpcm_decode.obj      : $(ADPCM_ALGO_DEC_PATH)\msadpcm\src\msadpcm_decode.asm 
$(OBJ_PATH)\imaadpcm_decode.obj     : $(ADPCM_ALGO_DEC_PATH)\imaadpcm\src\imaadpcm_decode.asm 
$(OBJ_PATH)\pcm_decode.obj          : $(ALGO_COMMON_PATH)\pcm_decode.asm   
$(OBJ_PATH)\wavedec.obj             : $(ALGO_COMMON_PATH)\wavedec.c 
$(OBJ_PATH)\bitstreamdec.obj        : $(ALGO_COMMON_PATH)\bitstreamdec.asm 


#;///////////////////////////////////////////////////////////////////////////////

# Decoder Adpcm Ima module & code
"$(OUTPUT_RSRC_PATH)\decadpcmimamod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract ADPCM Decoder Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=DECMOD_AdpcmIma_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\dec_adpcmp.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\dec_adpcmx.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\dec_adpcmy.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract ADPCM Dec X & ADPCM Dec Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\adpcmdec_abs_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
# contains : DECADPCMMOD_DecAdpcmMem_X|DECMOD_DecAdpcmImaNonOverlay_X         
