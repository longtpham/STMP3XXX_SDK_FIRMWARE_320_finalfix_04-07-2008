ifndef DISPLAY_USBMSC_MK
DISPLAY_USBMSC_MK=TRUE
DISPLAY_DRIVER_PATH=$(ROOT)\devicedriver\display

include $(ETC_PATH)\stringlib.mk
include $(ETC_PATH)\dbcs.mk



OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\displaydrv_color.obj\
          $(OBJ_PATH)\displayinit_color.obj\
          $(OBJ_PATH)\unicode_color.obj\
          $(OBJ_PATH)\font_color.obj\
	  $(OBJ_PATH)\displaygetcontrast.obj

ifdef CODEBANKS

CODEBANKLIST= $(CODEBANKLIST)\
            $(OUTPUT_RSRC_PATH)\RSRC_DISPLAY_CODEBANK.src\
            $(OUTPUT_RSRC_PATH)\RSRC_FONTS_CODEBANK.src


DISPLAYSECTIONS = $(DISPLAYSECTIONS)\
        .ptextdisplaydrv_color\
        .ptextdisplayinit_color\
        $(DISPLAY_HAL_SECTIONS)\
        $(DISPLAY_SAL_SECTIONS)


RESOURCE_INC_LIST = $(RESOURCE_INC_LIST)\
            $(SYS_PATH)\hardware\display\display_resource.inc
endif 

CC_OPTS= $(CC_OPTS) -Wc-I$(DISPLAY_DRIVER_PATH) -Wc-I$(HW_MSGMODULES_PATH)\Display\


$(OBJ_PATH)\displayinit_color.obj : $(HW_MSGMODULES_PATH)\display\displayinit_color.c 

$(OBJ_PATH)\displaydrv_color.obj     : $(HW_MSGMODULES_PATH)\display\displaydrv_color.c
$(OBJ_PATH)\displaydrv_color.obj     : $(INC_PATH)\displaydrv_color.h

$(OBJ_PATH)\unicode_color.obj: $(HW_MSGMODULES_PATH)\Display\unicode_color.c 
$(OBJ_PATH)\unicode_color.obj: $(OUTPUT_INCLUDE_PATH)\resource.h 
$(OBJ_PATH)\unicode_color.obj: $(INC_PATH)\sysresourceapi.h

$(OBJ_PATH)\font_color.obj: $(HW_MSGMODULES_PATH)\Display\font_color.c 
$(OBJ_PATH)\font_color.obj: $(OUTPUT_INCLUDE_PATH)\resource.h 
$(OBJ_PATH)\font_color.obj: $(INC_PATH)\sysresourceapi.h


"$(OUTPUT_RSRC_PATH)\RSRC_DISPLAY_CODEBANK.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Display Library
        $(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=$(separate "," $(DISPLAYSECTIONS)) -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\RSRC_FONTS_CODEBANK.src":  $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Font sections
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextfont_color -f$(OUTPUT_PATH)\$(PROJ).map

endif
$(OBJ_PATH)\displaygetcontrast.obj : $(PROJECT_H)
$(OBJ_PATH)\displaygetcontrast.obj : $(SYS_COMMON_PATH)\displaygetcontrast.c


