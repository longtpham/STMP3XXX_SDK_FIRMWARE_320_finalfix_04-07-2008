ifndef EPSON_SED15XX_MK
EPSON_SED15XX_MK=TRUE

include $(ETC_PATH)\registers.mk

ifdef D_NON_STFM1000_LCD
CC_OPTS= $(CC_OPTS) -Wc-DNON_STFM1000_LCD
endif

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\SED15xx.obj\
          $(OBJ_PATH)\SED15xxinit.obj

ifndef STMP_BUILD_PLAYER
OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\SED15xxinitstatic.obj
endif

DISPLAY_HAL_SECTIONS=.ptextsed15xx,.ptextsed15xxinit

"$(OBJ_PATH)\SED15xx.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
"$(OBJ_PATH)\SED15xx.obj" : $(DISPLAY_DRIVER_PATH)\Epson_SED15xx\SED15xx.c 
"$(OBJ_PATH)\SED15xx.obj" : $(DISPLAY_DRIVER_PATH)\Epson_SED15xx\SED15xx.h
"$(OBJ_PATH)\SED15xx.obj" : $(DISPLAY_DRIVER_PATH)\displaydriver.h 
"$(OBJ_PATH)\SED15xx.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h 
"$(OBJ_PATH)\SED15xx.obj" : $(PROJECT_H)
"$(OBJ_PATH)\SED15xx.obj" : $(INC_PATH)\sysresourceapi.h

$(OBJ_PATH)\sed15xxinit.obj : $(DISPLAY_DRIVER_PATH)\Epson_SED15xx\SED15xxinit.c
$(OBJ_PATH)\sed15xxinit.obj : $(DISPLAY_DRIVER_PATH)\Epson_SED15xx\SED15xx.h

$(OBJ_PATH)\sed15xxinitstatic.obj : $(DISPLAY_DRIVER_PATH)\Epson_SED15xx\SED15xxinitstatic.c
$(OBJ_PATH)\sed15xxinitstatic.obj : $(DISPLAY_DRIVER_PATH)\Epson_SED15xx\SED15xx.h

endif