
OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\colorlcdsal.obj\

DISPLAY_SAL_SECTIONS=.ptextcolorlcdsal
COLOR_LCD_SAL_PATH = $(HW_MSGMODULES_PATH)\Display\SAL\colorlcdsal

#add the colorlcdsal path to INC_ALL..any file can use SAL enums and #defines by #include "colorlcdsal.h"
INC_ALL= $(INC_ALL);$(COLOR_LCD_SAL_PATH)
INC_ALL_C = $(INC_ALL_C) -Wc-I$(COLOR_LCD_SAL_PATH)



"$(OBJ_PATH)\colorlcdsal.obj" : $(HW_MSGMODULES_PATH)\Display\SAL\colorlcdsal\colorlcdsal.c
"$(OBJ_PATH)\colorlcdsal.obj" : $(DISPLAY_DRIVER_PATH)\displaydriver_color.h
"$(OBJ_PATH)\colorlcdsal.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h

"$(OBJ_PATH)\colorlcdsal.obj" : $(INC_PATH)\sysresourceapi.h
"$(OBJ_PATH)\colorlcdsal.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h