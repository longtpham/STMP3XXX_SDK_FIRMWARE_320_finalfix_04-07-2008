#This is the makefile for the JPEG decoder control code. This includes the JPEG algo library from eInfoChips
#This file should be included into the project makefile and 
# $(JPEGLIB) must be included in the 
# dependancy list of the .out file.

MODULELIST = $(MODULELIST)\
             $(OUTPUT_RSRC_PATH)\jpeg_p.src\
             $(OUTPUT_RSRC_PATH)\jpeg_x.src\
             $(OUTPUT_RSRC_PATH)\jpeg_y.src\
             $(OUTPUT_RSRC_PATH)\jpeg2_y.src\
             $(OUTPUT_RSRC_PATH)\bmp2_y.src\
             $(OUTPUT_RSRC_PATH)\bmp_p.src

ifdef JPEG_DECODER

OBJLIST=$(OBJLIST)\
        $(JPEGOBJLIST)

ifdef COLOR_262K
JPEGLIB =  $(JPEG_LIB_PATH)\jpegdecoder262k.a
else
JPEGLIB =  $(JPEG_LIB_PATH)\jpegdecoder.a
endif

GENERALLIB = $(GENERALLIB)\
			 $(JPEGLIB)


JPEGOBJLIST = \
             $(OBJ_PATH)\JpegDecodeDisplay.obj

#///////////////////////////////////////////////////////////////////////////////
# Jpeg Decoder and Display control
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\JpegDecodeDisplay.obj" : $(JPEG_ALG_PATH)\JpegDecodeDisplay.c

# Jpeg decoder and display control

#"$(OUTPUT_RSRC_PATH)\JpegDecodeDisplay.src" : $(OUTPUT_PATH)\$(PROJ)jpeg.abs
#        @echo Extract Jpeg Decode and Display code
#        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextjpegdecodedisplay -f$(OUTPUT_PATH)\$(PROJ)jpeg.map

"$(OUTPUT_RSRC_PATH)\jpeg_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\jpeg_x.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\jpeg_y.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\jpeg2_y.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\bmp2_y.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\bmp_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract Decoder algorithm P & X & Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\jpeg_abs_rsrc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

else

"$(OUTPUT_RSRC_PATH)\jpeg_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\jpeg_x.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\jpeg_y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\jpeg2_y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\bmp2_y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\bmp_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

endif
