#This is the makefile for the WMA Decoder.
#
#This file should be included into the project makefile and 

GENERALLIB = $(GENERALLIB)\
              $(WMALIBLIST)

OBJLIST = $(OBJLIST)\
          $(DECWMAOBJLIST)


MODULELIST = $(MODULELIST)\
             $(WMAMODULERSRCLIST)

#Builds with or without DRM
ifdef D_DRM

LIB_OPT_LIST =  $(LIB_OPT_LIST)\
                -u Rmod_ululul


ifdef D_JANUS
WMALIBLIST = $(DECWMALIB)\
             $(WMA_LIB_PATH)\wmalibJanus.a\
             $(WMA_LIB_PATH)\drmpdlib.a\
             $(WMA_LIB_PATH)\libcdrm.a

else

WMALIBLIST = $(DECWMALIB)\
             $(WMA_LIB_PATH)\wmalib.a\
             $(WMA_LIB_PATH)\drmpdlib.a\
             $(WMA_LIB_PATH)\libcdrm.a

endif

else

WMALIBLIST = $(DECWMALIB)\
             $(WMA_LIB_PATH)\wmalibNoDRM.a
endif


DECWMAOBJLIST = \
             $(OBJ_PATH)\wmadec.obj\
             $(OBJ_PATH)\wmamem.obj\
             $(OBJ_PATH)\wmagetdata.obj\
             $(OBJ_PATH)\wma_decoder_nonoverlay.obj\
             $(OBJ_PATH)\wma_decoder_overlay.obj\
             $(OBJ_PATH)\wma_decodermem.obj




WMAMODULERSRCLIST =    \
             $(OUTPUT_RSRC_PATH)\wmaWrap.src\
             $(OUTPUT_RSRC_PATH)\wmaCore.src\
             $(OUTPUT_RSRC_PATH)\wmaMidLow.src\
             $(OUTPUT_RSRC_PATH)\wmaHigh.src\
             $(OUTPUT_RSRC_PATH)\wmaHighMid.src\
             $(OUTPUT_RSRC_PATH)\wmaMid.src\
             $(OUTPUT_RSRC_PATH)\wmaLow.src\
             $(OUTPUT_RSRC_PATH)\wmaCommon.src\
             $(OUTPUT_RSRC_PATH)\wmainit.src\
             $(OUTPUT_RSRC_PATH)\wmaX1mem.src\
             $(OUTPUT_RSRC_PATH)\wmaYmem.src\
             $(OUTPUT_RSRC_PATH)\wmaLXmem.src\
             $(OUTPUT_RSRC_PATH)\wmaLYmem.src\
             $(OUTPUT_RSRC_PATH)\wmaHuff16Ob.src\
             $(OUTPUT_RSRC_PATH)\wmaHuff44Ob.src\
             $(OUTPUT_RSRC_PATH)\wmaHuff44Qb.src\
             $(OUTPUT_RSRC_PATH)\drmpdcommon.src\
             $(OUTPUT_RSRC_PATH)\wmalicenseinit.src\
             $(OUTPUT_RSRC_PATH)\wma_tables.src\     
             $(OUTPUT_RSRC_PATH)\wma_constants.src  

# Decoder Wma module & code
# --------------------------------------------------------------------

# Define the .loc file to extract DRM and non-DRM sections.
ifdef D_DRM
WMA_SECTION_LOC=$(LOC_PATH)\wma_sect_rsrc.loc
else
WMA_SECTION_LOC=$(LOC_PATH)\wma_NoDRM_sect_rsrc.loc
endif

ifdef DEVICE_3400
"$(OUTPUT_RSRC_PATH)\wmaWrap.src" : $(RSRC_MISC_PATH)\null_data.src
        @echo "Create Dummy Wma Resources"
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaWrap.src /Y
"$(OUTPUT_RSRC_PATH)\wmaCore.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaCore.src /Y
"$(OUTPUT_RSRC_PATH)\wmaMidLow.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaMidLow.src /Y
"$(OUTPUT_RSRC_PATH)\wmaHigh.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaHigh.src /Y
"$(OUTPUT_RSRC_PATH)\wmaHighMid.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaHighMid.src /Y
"$(OUTPUT_RSRC_PATH)\wmaMid.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaMid.src /Y
"$(OUTPUT_RSRC_PATH)\wmaLow.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaLow.src /Y
"$(OUTPUT_RSRC_PATH)\wmaCommon.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaCommon.src /Y
"$(OUTPUT_RSRC_PATH)\wmainit.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmainit.src /Y
"$(OUTPUT_RSRC_PATH)\wmaX1mem.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaX1mem.src /Y
"$(OUTPUT_RSRC_PATH)\wmaYmem.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaYmem.src /Y
"$(OUTPUT_RSRC_PATH)\wmaHuff16Ob.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaHuff16Ob.src /Y
"$(OUTPUT_RSRC_PATH)\wmaHuff44Ob.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaHuff44Ob.src /Y
"$(OUTPUT_RSRC_PATH)\wmaHuff44Qb.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaHuff44Qb.src /Y
"$(OUTPUT_RSRC_PATH)\drmpdcommon.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\drmpdcommon.src /Y
"$(OUTPUT_RSRC_PATH)\wmalicenseinit.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmalicenseinit.src /Y
"$(OUTPUT_RSRC_PATH)\wma_tables.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wma_tables.src /Y
"$(OUTPUT_RSRC_PATH)\wma_constants.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wma_constants.src /Y

else

"$(OUTPUT_RSRC_PATH)\wmaCore.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wmaMidLow.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wmaHigh.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wmaHighMid.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wmaMid.src" : $(OUTPUT_PATH)\$(PROJ).abs		
"$(OUTPUT_RSRC_PATH)\wmaLow.src" : $(OUTPUT_PATH)\$(PROJ).abs		
"$(OUTPUT_RSRC_PATH)\wmaCommon.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wmainit.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wmaHuff16Ob.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\wmaHuff44Ob.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\wmaHuff44Qb.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\wmaX1mem.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\wmaYmem.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wma_constants.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract WMA Decoder Sections"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(WMA_SECTION_LOC) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

"$(OUTPUT_RSRC_PATH)\wmaWrap.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract WMA Decoder Sections"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=DECWMAMOD_DecoderOverlay_overlay_P,.wmadecinit -F$(OUTPUT_PATH)\$(PROJ).map
endif        

ifdef D_DRM
"$(OUTPUT_RSRC_PATH)\drmpdcommon.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\wmalicenseinit.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\wma_tables.src"  : $(OUTPUT_PATH)\$(PROJ).abs

        @echo "Extract WMA Decoder Sections"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\drmpd_sect_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

else

"$(OUTPUT_RSRC_PATH)\drmpdcommon.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\drmpdcommon.src /Y
"$(OUTPUT_RSRC_PATH)\wmalicenseinit.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmalicenseinit.src /Y
"$(OUTPUT_RSRC_PATH)\wma_tables.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wma_tables.src /Y

endif


ifdef DEVICE_3400
"$(OUTPUT_RSRC_PATH)\wmaLXmem.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaLXmem.src /Y
"$(OUTPUT_RSRC_PATH)\wmaLYmem.src" : $(RSRC_MISC_PATH)\null_data.src
        @copy $(RSRC_MISC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\wmaLYmem.src /Y

else

"$(OUTPUT_RSRC_PATH)\wmaLXmem.src" : $(OUTPUT_PATH)\$(PROJ).abs 
        @echo "Extract WMA Decoder Clusters"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -C$@=L_TablesX

"$(OUTPUT_RSRC_PATH)\wmaLYmem.src" : $(OUTPUT_PATH)\$(PROJ).abs 
        @echo "Extract WMA Decoder Clusters"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -C$@=L_TablesY

endif


#///////////////////////////////////////////////////////////////////////////////
# WMA Decoder
#///////////////////////////////////////////////////////////////////////////////
 
$(OBJ_PATH)\wmadec.obj : $(WMA_ALG_PATH)\wmadec.asm
 
$(OBJ_PATH)\wmamem.obj : $(WMA_ALG_PATH)\wmamem.asm
 
$(OBJ_PATH)\wmagetdata.obj : $(WMA_ALG_PATH)\wmagetdata.asm

$(OBJ_PATH)\wma_dacmem.obj : $(WMA_PATH)\wma_dacmem.asm
 
$(OBJ_PATH)\wma_decoder_overlay.obj : $(WMA_PATH)\wma_decoder_overlay.asm
 
$(OBJ_PATH)\wma_decoder_nonoverlay.obj : $(WMA_PATH)\wma_decoder_nonoverlay.asm
 
$(OBJ_PATH)\wma_decodermem.obj : $(WMA_PATH)\wma_decodermem.asm

$(OBJ_PATH)\drm_libc.obj : $(WMA_ALG_PATH)\drm_libc.asm