#This is the makefile for the STFM Decoder.
#
#This file should be included into the project makefile and 


OBJLIST=$(OBJLIST)\
        $(DECSTFMLIST)        


MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\decstfmmod.src\
                 $(OUTPUT_RSRC_PATH)\dec_stfmp.src\
                 $(OUTPUT_RSRC_PATH)\dec_stfmx.src\
                 $(OUTPUT_RSRC_PATH)\dec_stfmy.src

 ifdef MIN_FILTER
DECSTFMLIST = \
            $(OBJ_PATH)\dec_stfm_nonoverlay.obj\
            $(OBJ_PATH)\dec_stfm_overlay.obj\
            $(OBJ_PATH)\dec_stfm_mem.obj\
			$(OBJ_PATH)\stfm_decode.obj\
            $(OBJ_PATH)\dec_stfm_filter.obj\

 else
DECSTFMLIST = \
            $(OBJ_PATH)\dec_stfm_nonoverlay.obj\
            $(OBJ_PATH)\dec_stfm_overlay.obj\
            $(OBJ_PATH)\dec_stfm_mem.obj\
			$(OBJ_PATH)\stfm_decode.obj\
            $(OBJ_PATH)\sw_stereocontrol.obj\
            $(OBJ_PATH)\sw_stereopath_xcall_asm.obj\

 endif


#///////////////////////////////////////////////////////////////////////////////
# STFM Decoder
#///////////////////////////////////////////////////////////////////////////////
$(OBJ_PATH)\dec_stfm_overlay.obj   : $(STFM_DEC_PATH)\dec_stfm_overlay.asm
                                                              
$(OBJ_PATH)\dec_stfm_nonoverlay.obj: $(STFM_DEC_PATH)\dec_stfm_nonoverlay.asm
                                                              
$(OBJ_PATH)\dec_stfm_mem.obj       : $(STFM_DEC_PATH)\dec_stfm_mem.asm $(ROOT)\inc\dec_stfmequ.inc
$(OBJ_PATH)\stfm_decode.obj      : $(STFM_ALGO_DEC_PATH)\stfm\src\stfm_decode.asm

 ifdef MIN_FILTER
$(OBJ_PATH)\dec_stfm_filter.obj     : $(STFM_ALGO_DEC_PATH)\stfm\src\dec_stfm_filter.c
$(OBJ_PATH)\dec_stfm_filter.obj     : $(STFM_ALGO_DEC_PATH)\stfm\src\dec_stfm_filter.h

 else
$(OBJ_PATH)\sw_stereocontrol.obj     : $(STFM_ALGO_DEC_PATH)\stfm\src\sw_stereocontrol.c
$(OBJ_PATH)\sw_stereocontrol.obj     : $(STFM_ALGO_DEC_PATH)\stfm\src\sw_stereocontrol.h

$(OBJ_PATH)\sw_stereopath_xcall_asm.obj     : $(STFM_ALGO_DEC_PATH)\stfm\src\sw_stereopath_xcall_asm.asm

 endif
#;///////////////////////////////////////////////////////////////////////////////

# Decoder STFM module & code
"$(OUTPUT_RSRC_PATH)\decstfmmod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract STFM Decoder Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=DECMOD_Stfm_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\dec_stfmp.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\dec_stfmx.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\dec_stfmy.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract STFM Dec X & STFM Dec Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\stfmdec_abs_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
# contains : DECSTFMMOD_DecStfmMem_X|DECMOD_DecStfmNonOverlay_X         
