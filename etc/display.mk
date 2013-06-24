
DISPLAY_DRIVER_PATH=$(ROOT)\devicedriver\display


MODULELIST= $(MODULELIST)\
            $(OUTPUT_RSRC_PATH)\DisplayModule.src

FUNCLETSRCLIST=$(FUNCLETSRCLIST)\
            $(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressUnicode.src\
            $(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceUnicode.src\
            $(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressDBCS.src\
            $(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceDBCS.src\
            $(OUTPUT_RSRC_PATH)\Funclet_LCDSetFont.src\
            $(OUTPUT_RSRC_PATH)\Funclet_GetUnicodeCharacterBitmap.src


OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\displaymodule.obj\
          $(OBJ_PATH)\displaydrv.obj\
          $(OBJ_PATH)\unicode.obj\
          $(OBJ_PATH)\font.obj\
	  $(OBJ_PATH)\displaygetcontrast.obj


CCFLAGS= $(CCFLAGS) -Wc-I$(DISPLAY_DRIVER_PATH) -Wc-I$(HW_MSGMODULES_PATH)\Display\

$(OBJ_PATH)\displaydrv.obj : $(HW_MSGMODULES_PATH)\Display\displaydrv.c 


$(OBJ_PATH)\displaymodule.obj : $(HW_MSGMODULES_PATH)\Display\displaymodule.c 
$(OBJ_PATH)\displaymodule.obj : $(OUTPUT_INCLUDE_PATH)\resource.h 
$(OBJ_PATH)\displaymodule.obj : $(OUTPUT_INCLUDE_PATH)\sysequ.h 
$(OBJ_PATH)\displaymodule.obj : $(OUTPUT_INCLUDE_PATH)\sysresources.h
$(OBJ_PATH)\displaymodule.obj : $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\displaymodule.obj : $(PROJECT_H)


$(OBJ_PATH)\displaygetcontrast.obj : $(SYS_COMMON_PATH)\displaygetcontrast.c
$(OBJ_PATH)\displaygetcontrast.obj : $(PROJECT_H)

$(OBJ_PATH)\font.obj: $(HW_MSGMODULES_PATH)\Display\font.c $(OUTPUT_INCLUDE_PATH)\resource.h $(OUTPUT_INCLUDE_PATH)\sysequ.h $(OUTPUT_INCLUDE_PATH)\sysresources.h

$(OBJ_PATH)\unicode.obj: $(HW_MSGMODULES_PATH)\Display\unicode.c $(OUTPUT_INCLUDE_PATH)\resource.h $(OUTPUT_INCLUDE_PATH)\sysequ.h $(OUTPUT_INCLUDE_PATH)\sysresources.h


"$(OUTPUT_RSRC_PATH)\DisplayModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract DisplayModule .ptextdisplaymodule,.ptextdisplaydrv,$(HAL_SECTIONS),$(SAL_SECTIONS)
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextdisplaymodule,.ptextdisplaydrv,$(DISPLAY_HAL_SECTIONS),$(DISPLAY_SAL_SECTIONS) -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_LCDSetFont.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_LCDSetFont 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_SET_FONT_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressUnicode.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_GetTextWidthAddressUnicode
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_GET_TEXT_WIDTH_ADDRESS_UNICODE_P,.ptextunicode -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceUnicode.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_GetTextWidthResourceUnicode
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_GET_TEXT_WIDTH_RESOURCE_UNICODE_P,.ptextunicode -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthAddressDBCS.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_GetTextWidthAddressDBCS
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_GET_TEXT_WIDTH_ADDRESS_DBCS_P,.ptextunicode -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_GetTextWidthResourceDBCS.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_GetTextWidthResourceDBCS
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_GET_TEXT_WIDTH_RESOURCE_DBCS_P,.ptextunicode -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_GetUnicodeCharacterBitmap.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_GetUnicodeCharacterBitmap
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_GetUnicodeCharacterBitmap_P,.ptextunicode -f$(OUTPUT_PATH)\$(PROJ).map
