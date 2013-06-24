ifndef D_AUDIBLEDEC
OBJLIST=    $(OBJLIST)\

MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\AudibleAcelpDec.src\
             $(OUTPUT_RSRC_PATH)\AudibleAcelpP.src\
             $(OUTPUT_RSRC_PATH)\AudibleAcelpX.src\
             $(OUTPUT_RSRC_PATH)\AudibleAcelpY.src

"$(OUTPUT_RSRC_PATH)\AudibleAcelpDec.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL AudibleAcelpDec.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\AudibleAcelpDec.src

"$(OUTPUT_RSRC_PATH)\AudibleAcelpP.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL AudibleAcelpP.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\AudibleAcelpP.src
        
"$(OUTPUT_RSRC_PATH)\AudibleAcelpX.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL AudibleAcelpX.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\AudibleAcelpX.src

"$(OUTPUT_RSRC_PATH)\AudibleAcelpY.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL AudibleAcelpY.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\AudibleAcelpY.src

else

AUD_PATH			= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\AASource
AUD_PATH2			= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\AADevProject
AUD_INC1			= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\IncludePublic
ACELP_LIB_PATH	= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\ACELP
AUD_ACELP_PATH		= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp
AUD_ACELP_WRAPPER_PATH = ..\..\..\..\..\System\MsgModules\Software\Decoder\AudibleAcelp

CCFLAGS = $(CCFLAGS) -I$(AUD_PATH) -I$(AUD_INC1) -I$(AUD_PATH2) -I$(AUD_ACELP_PATH) -I$(ACELP_LIB_PATH)


GENERALLIB = $(GENERALLIB)\
            $(AUD_ACELP_LIBLIST)

OBJLIST = $(OBJLIST)\
            $(AUD_ACELP_OBJLIST)


MODULELIST = $(MODULELIST)\
                $(AUD_ACELP_MODULERSRCLIST)

DEC_AUD_ACELPLIB_OPTS		= -u AcelpDecoderModule

AUD_ACELP_LIBLIST = $(ACELP_LIB_PATH)\va_acelp_lib.a\
		  		    $(ACELP_LIB_PATH)\libcr.a\
			        $(ACELP_LIB_PATH)\libfp.a\
 			        $(ACELP_LIB_PATH)\librt.a

AUD_ACELP_OBJLIST = \
             $(OBJ_PATH)\AudibleAcelpmem.obj\
			 $(OBJ_PATH)\AudibleAcelpDec_c.obj\
             $(OBJ_PATH)\AudibleAcelp_decoder_nonoverlay.obj\
             $(OBJ_PATH)\AudibleAcelp_decoder_overlay.obj\
             $(OBJ_PATH)\AudibleAcelp_decodermem.obj\
				$(OBJ_PATH)\AudCStandAllocation.obj\
				$(OBJ_PATH)\AAFileAccessImpl.obj \
				$(OBJ_PATH)\AAFileMiscHelpers.obj	\
				$(OBJ_PATH)\AAGetAudio.obj	\
				$(OBJ_PATH)\AAOpenFile.obj	\
				$(OBJ_PATH)\AudCStandImpGeneric.obj	\
				$(OBJ_PATH)\CircularBuffer.obj	\
				$(OBJ_PATH)\CoreEncryption.obj	\
				$(OBJ_PATH)\DecryptHeader.obj	\
				$(OBJ_PATH)\FillAudibleFileInfo.obj	\
				$(OBJ_PATH)\ReadAAFileSectionsHeader.obj	\
				$(OBJ_PATH)\AcelpCodecImpl.obj \
				$(OBJ_PATH)\InputStreamImplCStand.obj \
				$(OBJ_PATH)\AAFileDecode.obj \
				$(OBJ_PATH)\AAFileAccess_Nav.obj\
				$(OBJ_PATH)\Conv2416.obj\

#				$(OBJ_PATH)\PlaybackPosition.obj	\
#				$(OBJ_PATH)\activationrecords.obj\
#				$(OBJ_PATH)\checktargeting.obj\
#				$(OBJ_PATH)\dsaverify.obj\
#				$(OBJ_PATH)\keys.obj\
#				$(OBJ_PATH)\ronsdsa.obj\
#				$(OBJ_PATH)\sha.obj


AUD_ACELP_MODULERSRCLIST =    \
             $(OUTPUT_RSRC_PATH)\AudibleAcelpDec.src\
             $(OUTPUT_RSRC_PATH)\AudibleAcelpP.src\
             $(OUTPUT_RSRC_PATH)\AudibleAcelpX.src\
             $(OUTPUT_RSRC_PATH)\AudibleAcelpY.src


# Decoder AudibleAcelp module & code
# --------------------------------------------------------------------
"$(OUTPUT_INCLUDE_PATH)\AudibleAcelpDec.h" : $(INC_PATH)\AudibleAcelpdec.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

AUD_ACELP_SECTION_LOC=$(LOC_PATH)\AudibleAcelp_abs_rsrc.loc

"$(OUTPUT_RSRC_PATH)\AudibleAcelpDec.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Decoder Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=DECACELPMOD_DecoderOverlay_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\AudibleAcelpP.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\AudibleAcelpX.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\AudibleAcelpY.src" : $(OUTPUT_PATH)\$(PROJ).abs	
        @echo "Extract ACELP Decoder Sections"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(AUD_ACELP_SECTION_LOC) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\


#///////////////////////////////////////////////////////////////////////////////
# ACELP Decoder
#///////////////////////////////////////////////////////////////////////////////
 
#"$(OBJ_PATH)\AudibleAcelpdec.obj" : $(AUD_ACELP_PATH)\AudibleAcelpdec.asm
"$(OBJ_PATH)\AudibleAcelpDec_c.obj" : $(AUD_ACELP_PATH)\AudibleAcelpDec_c.c $(OUTPUT_INCLUDE_PATH)\AudibleAcelpDec.h

"$(OBJ_PATH)\AudibleAcelpmem.obj" : $(AUD_ACELP_PATH)\AudibleAcelpmem.asm
 
"$(OBJ_PATH)\AudibleAcelp_decoder_overlay.obj" : $(AUD_ACELP_WRAPPER_PATH)\AudibleAcelp_decoder_overlay.asm
 
"$(OBJ_PATH)\AudibleAcelp_decoder_nonoverlay.obj" : $(AUD_ACELP_WRAPPER_PATH)\AudibleAcelp_decoder_nonoverlay.asm
 
"$(OBJ_PATH)\AudibleAcelp_decodermem.obj" : $(AUD_ACELP_WRAPPER_PATH)\AudibleAcelp_decodermem.asm



"$(OBJ_PATH)\AudCStandAllocation.obj"	:	$(AUD_PATH)\AudCStandAllocation.c
"$(OBJ_PATH)\AAFileAccessImpl.obj"	:	$(AUD_PATH)\AAFileAccessImpl.c
"$(OBJ_PATH)\AAFileMiscHelpers.obj"	:	$(AUD_PATH)\AAFileMiscHelpers.c
"$(OBJ_PATH)\AAGetAudio.obj"	:	$(AUD_PATH)\AAGetAudio.c
"$(OBJ_PATH)\AAOpenFile.obj"		:	$(AUD_PATH)\AAOpenFile.c
"$(OBJ_PATH)\AudCStandImpGeneric.obj"	:	$(AUD_PATH)\AudCStandImpGeneric.c
"$(OBJ_PATH)\CircularBuffer.obj"	:	$(AUD_PATH)\CircularBuffer.c
"$(OBJ_PATH)\CoreEncryption.obj"	:	$(AUD_PATH)\CoreEncryption.c
"$(OBJ_PATH)\DecryptHeader.obj"	:	$(AUD_PATH)\DecryptHeader.c
"$(OBJ_PATH)\FillAudibleFileInfo.obj"	:	$(AUD_PATH)\FillAudibleFileInfo.c
"$(OBJ_PATH)\ReadAAFileSectionsHeader.obj"	:	$(AUD_PATH)\ReadAAFileSectionsHeader.c
"$(OBJ_PATH)\AA_NoCodecImpl.obj"	:	$(AUD_PATH)\AA_NoCodecImpl.c
"$(OBJ_PATH)\AcelpCodecImpl.obj"	:	$(AUD_PATH)\AcelpCodecImpl.c
"$(OBJ_PATH)\InputStreamImplCStand.obj"	:	$(AUD_PATH2)\InputStreamImplCStand.c
"$(OBJ_PATH)\AAFileDecode.obj"	:	$(AUD_PATH)\AAFileDecode.c
"$(OBJ_PATH)\AAFileAccess_Nav.obj"	:	$(AUD_PATH)\AAFileAccess_Nav.c
"$(OBJ_PATH)\Conv2416.obj"	:	$(ACELP_LIB_PATH)\Conv2416.c

#"$(OBJ_PATH)\PlaybackPosition.obj"	:	$(AUD_PATH)\PlaybackPosition.c
#"$(OBJ_PATH)\activationrecords.obj"	: $(AUD_PATH)\activationrecords.c
#"$(OBJ_PATH)\activationrecords.obj"	: $(AUD_PATH)\activationrecords.h
#"$(OBJ_PATH)\checktargeting.obj"	: $(AUD_PATH)\checktargeting.c
#"$(OBJ_PATH)\dsaverify.obj"			: $(AUD_PATH)\dsaverify.c
#"$(OBJ_PATH)\keys.obj"				: $(AUD_PATH)\keys.c
#"$(OBJ_PATH)\ronsdsa.obj"			: $(AUD_PATH)\ronsdsa.c
#"$(OBJ_PATH)\sha.obj"				: $(AUD_PATH)\sha.c
endif

