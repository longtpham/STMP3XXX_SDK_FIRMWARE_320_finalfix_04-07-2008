#This is the makefile for the JPEG decoder control code. This includes the JPEG algo library from eInfoChips
#This file should be included into the project makefile and 
# $(JPEGLIB) must be included in the 
# dependancy list of the .out file.

MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\smvjpeg_p.src\
             $(OUTPUT_RSRC_PATH)\smvjpeg_x.src\
             $(OUTPUT_RSRC_PATH)\smvjpeg_y.src

ifdef SMVJPEG_DECODER

OBJLIST=$(OBJLIST)\
        $(SMVJPEGOBJLIST)

ifdef COLOR_262K
SMVJPEGLIB =  $(SMVJPEG_LIB_PATH)\smvjpegdecoder262k.a
else
SMVJPEGLIB =  $(SMVJPEG_LIB_PATH)\smvjpegdecoder.a
endif

GENERALLIB = $(GENERALLIB)\
			 $(SMVJPEGLIB)


SMVJPEGOBJLIST = \
             $(OBJ_PATH)\SmvJpegDecodeDisplay.obj

#///////////////////////////////////////////////////////////////////////////////
# Jpeg Decoder and Display control
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\SmvJpegDecodeDisplay.obj" : $(SMVJPEG_ALG_PATH)\SmvJpegDecodeDisplay.c

# SmvJpeg decoder and display control

#"$(OUTPUT_RSRC_PATH)\JpegDecodeDisplay.src" : $(OUTPUT_PATH)\$(PROJ)jpeg.abs
#        @echo Extract Jpeg Decode and Display code
#        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextjpegdecodedisplay -f$(OUTPUT_PATH)\$(PROJ)jpeg.map

"$(OUTPUT_RSRC_PATH)\smvjpeg_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\smvjpeg_x.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\smvjpeg_y.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract Decoder algorithm P & X & Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\smvjpeg_abs_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

else #ifdef SMVJPEG_DECODER

"$(OUTPUT_RSRC_PATH)\smvjpeg_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\smvjpeg_x.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\smvjpeg_y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

endif
