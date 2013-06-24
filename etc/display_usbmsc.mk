ifndef DISPLAY_USBMSC_MK
DISPLAY_USBMSC_MK=TRUE
DISPLAY_DRIVER_PATH=$(ROOT)\devicedriver\display

include $(ETC_PATH)\stringlib.mk
include $(ETC_PATH)\dbcs.mk



OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\displaydrv.obj\
          $(OBJ_PATH)\displayinit.obj\
          $(OBJ_PATH)\unicode.obj\
          $(OBJ_PATH)\font.obj\
	  $(OBJ_PATH)\displaygetcontrast.obj

ifdef CODEBANKS

CODEBANKLIST= $(CODEBANKLIST)\
            $(OUTPUT_RSRC_PATH)\RSRC_DISPLAY_CODEBANK.src\
            $(OUTPUT_RSRC_PATH)\RSRC_FONTS_CODEBANK.src


DISPLAYSECTIONS = $(DISPLAYSECTIONS)\
        .ptextdisplaydrv\
        .ptextdisplayinit\
        $(DISPLAY_HAL_SECTIONS)\
        $(DISPLAY_SAL_SECTIONS)
					 

RESOURCE_INC_LIST = $(RESOURCE_INC_LIST)\
            $(SYS_PATH)\hardware\display\display_resource.inc
endif 

CC_OPTS= $(CC_OPTS) -Wc-I$(DISPLAY_DRIVER_PATH) -Wc-I$(HW_MSGMODULES_PATH)\Display\


$(OBJ_PATH)\displayinit.obj : $(HW_MSGMODULES_PATH)\display\displayinit.c 

$(OBJ_PATH)\displaydrv.obj     : $(HW_MSGMODULES_PATH)\display\displaydrv.c
$(OBJ_PATH)\displaydrv.obj     : $(INC_PATH)\displaydrv.h

$(OBJ_PATH)\unicode.obj: $(HW_MSGMODULES_PATH)\Display\unicode.c 
$(OBJ_PATH)\unicode.obj: $(OUTPUT_INCLUDE_PATH)\resource.h 
$(OBJ_PATH)\unicode.obj: $(INC_PATH)\sysresourceapi.h

$(OBJ_PATH)\font.obj: $(HW_MSGMODULES_PATH)\Display\font.c 
$(OBJ_PATH)\font.obj: $(OUTPUT_INCLUDE_PATH)\resource.h 
$(OBJ_PATH)\font.obj: $(INC_PATH)\sysresourceapi.h


"$(OUTPUT_RSRC_PATH)\RSRC_DISPLAY_CODEBANK.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Display Library
        $(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=$(separate "," $(DISPLAYSECTIONS)) -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\RSRC_FONTS_CODEBANK.src":  $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Font sections
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextfont -f$(OUTPUT_PATH)\$(PROJ).map

endif
$(OBJ_PATH)\displaygetcontrast.obj : $(PROJECT_H)
$(OBJ_PATH)\displaygetcontrast.obj : $(SYS_COMMON_PATH)\displaygetcontrast.c


