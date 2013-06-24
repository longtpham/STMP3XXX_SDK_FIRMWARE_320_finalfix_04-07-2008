#This is the makefile for the SMVADPCM Decoder.
#
#This file should be included into the project makefile and 

MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\decadpcmsmvmod.src\
                 $(OUTPUT_RSRC_PATH)\dec_smvadpcmp.src\
                 $(OUTPUT_RSRC_PATH)\dec_smvadpcmx.src\
                 $(OUTPUT_RSRC_PATH)\dec_smvadpcmy.src

ifdef SMV_ADPCM
OBJLIST=$(OBJLIST)\
        $(DECSMVADPCMLIST)        

DECSMVADPCMLIST = \
            $(OBJ_PATH)\dec_smvadpcm_nonoverlay.obj\
            $(OBJ_PATH)\dec_smvadpcm_overlay.obj\
            $(OBJ_PATH)\dec_smvadpcm_mem.obj\
			$(OBJ_PATH)\smvadpcm_decode.obj\  
            $(OBJ_PATH)\smvwavedec.obj\
            $(OBJ_PATH)\smvbitstreamdec.obj


#///////////////////////////////////////////////////////////////////////////////
# ADPCM Decoder
#///////////////////////////////////////////////////////////////////////////////
$(OBJ_PATH)\dec_smvadpcm_overlay.obj   : $(SMVADPCM_DEC_PATH)\dec_smvadpcm_overlay.asm

$(OBJ_PATH)\dec_smvadpcm_nonoverlay.obj: $(SMVADPCM_DEC_PATH)\dec_smvadpcm_nonoverlay.asm

$(OBJ_PATH)\dec_smvadpcm_mem.obj       : $(SMVADPCM_DEC_PATH)\dec_smvadpcm_mem.asm

$(OBJ_PATH)\smvadpcm_decode.obj      : $(ADPCM_ALGO_DEC_PATH)\smvadpcm\src\smvadpcm_decode.asm 

$(OBJ_PATH)\smvwavedec.obj             : $(ALGO_COMMON_PATH)\smvwavedec.c 

$(OBJ_PATH)\smvbitstreamdec.obj        : $(ALGO_COMMON_PATH)\smvbitstreamdec.asm 

#;///////////////////////////////////////////////////////////////////////////////

# Decoder Adpcm Smv module & code
"$(OUTPUT_RSRC_PATH)\decadpcmsmvmod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract SMVADPCM Decoder Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=DECMOD_AdpcmSmv_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\dec_smvadpcmp.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\dec_smvadpcmx.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract SMVADPCM Dec X & SMVADPCM Dec Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\smvadpcmdec_abs_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

else

"$(OUTPUT_RSRC_PATH)\decadpcmsmvmod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract SMVADPCM Decoder Module
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\decadpcmsmvmod.src

"$(OUTPUT_RSRC_PATH)\dec_smvadpcmp.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract SMVADPCM Dec P"
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\dec_smvadpcmp.src

"$(OUTPUT_RSRC_PATH)\dec_smvadpcmx.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract SMVADPCM Dec X & SMVADPCM Dec Y"
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\dec_smvadpcmx.src

endif

"$(OUTPUT_RSRC_PATH)\dec_smvadpcmy.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL dec_smvadpcmy.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\dec_smvadpcmy.src
