
OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\usbmsc_colorlcdsal.obj\

DISPLAY_SAL_SECTIONS=.ptextusbmsc_colorlcdsal
COLOR_LCD_SAL_PATH = $(HW_MSGMODULES_PATH)\Display\SAL\colorlcdsal

#add the colorlcdsal path to INC_ALL..any file can use SAL enums and #defines by #include "colorlcdsal.h"
INC_ALL= $(INC_ALL);$(COLOR_LCD_SAL_PATH)
INC_ALL_C = $(INC_ALL_C) -Wc-I$(COLOR_LCD_SAL_PATH)



"$(OBJ_PATH)\usbmsc_colorlcdsal.obj" : $(HW_MSGMODULES_PATH)\Display\SAL\colorlcdsal\usbmsc_colorlcdsal.c
"$(OBJ_PATH)\usbmsc_colorlcdsal.obj" : $(DISPLAY_DRIVER_PATH)\displaydriver_color.h
"$(OBJ_PATH)\usbmsc_colorlcdsal.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h

"$(OBJ_PATH)\usbmsc_colorlcdsal.obj" : $(INC_PATH)\sysresourceapi.h
"$(OBJ_PATH)\usbmsc_colorlcdsal.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h