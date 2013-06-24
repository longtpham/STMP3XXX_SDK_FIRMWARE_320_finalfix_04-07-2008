ifndef SAMSUNG_S6B33B0A_MK
SAMSUNG_S6B33B0A_MK=TRUE

include $(ETC_PATH)\registers.mk

COLOR_DISPLAY_DRIVER_PATH = $(DISPLAY_DRIVER_PATH)\samsung_s6b33b0a

INC_ALL_C =\
		  $(INC_ALL_C)\
		  -Wc-I$(COLOR_DISPLAY_DRIVER_PATH)


ifdef COLOR_262K
LCD_CONTROLLER_FILE = s6b33b0a_262k
DISPLAY_HAL_SECTIONS=.ptexts6b33b0a_262k,.ptexts6b33b0ainit
else
ifdef COLOR_65K
LCD_CONTROLLER_FILE = s6b33b0a_65k
DISPLAY_HAL_SECTIONS=.ptexts6b33b0a_65k,.ptexts6b33b0ainit
else
LCD_CONTROLLER_FILE = s6b33b0a
DISPLAY_HAL_SECTIONS=.ptexts6b33b0a,.ptexts6b33b0ainit
endif
endif

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\$(LCD_CONTROLLER_FILE).obj\
          $(OBJ_PATH)\s6b33b0ainit.obj\

ifndef STMP_BUILD_PLAYER
OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\s6b33b0ainitstatic.obj
endif

CCFLAGS= $(CCFLAGS) -Wc-I$(DISPLAY_DRIVER_PATH)\samsung_s6b33b0a\


"$(OBJ_PATH)\$(LCD_CONTROLLER_FILE).obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
"$(OBJ_PATH)\$(LCD_CONTROLLER_FILE).obj" : $(DISPLAY_DRIVER_PATH)\samsung_s6b33b0a\$(LCD_CONTROLLER_FILE).c 
"$(OBJ_PATH)\$(LCD_CONTROLLER_FILE).obj" : $(DISPLAY_DRIVER_PATH)\samsung_s6b33b0a\s6b33b0a.h
"$(OBJ_PATH)\$(LCD_CONTROLLER_FILE).obj" : $(DISPLAY_DRIVER_PATH)\displaydriver_color.h 
"$(OBJ_PATH)\$(LCD_CONTROLLER_FILE).obj" : $(OUTPUT_INCLUDE_PATH)\messages.h 
"$(OBJ_PATH)\$(LCD_CONTROLLER_FILE).obj" : $(PROJECT_H)
"$(OBJ_PATH)\$(LCD_CONTROLLER_FILE).obj" : $(INC_PATH)\sysresourceapi.h

$(OBJ_PATH)\s6b33b0ainit.obj : $(DISPLAY_DRIVER_PATH)\samsung_s6b33b0a\s6b33b0ainit.c
$(OBJ_PATH)\s6b33b0ainit.obj : $(DISPLAY_DRIVER_PATH)\samsung_s6b33b0a\s6b33b0a.h

$(OBJ_PATH)\s6b33b0ainitstatic.obj : $(DISPLAY_DRIVER_PATH)\samsung_s6b33b0a\s6b33b0ainitstatic.c
$(OBJ_PATH)\s6b33b0ainitstatic.obj : $(DISPLAY_DRIVER_PATH)\samsung_s6b33b0a\s6b33b0a.h


endif
