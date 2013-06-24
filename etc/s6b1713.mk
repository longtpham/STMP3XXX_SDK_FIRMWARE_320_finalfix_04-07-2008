ifndef S6B1713_MK
S6B1713_MK=TRUE

include $(ETC_PATH)\registers.mk

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\s6b1713.obj\
          $(OBJ_PATH)\s6b1713init.obj\

DISPLAY_HAL_SECTIONS=.ptexts6b1713,.ptexts6b1713init

"$(OBJ_PATH)\s6b1713.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
"$(OBJ_PATH)\s6b1713.obj" : $(DISPLAY_DRIVER_PATH)\s6b1713\s6b1713.c 
"$(OBJ_PATH)\s6b1713.obj" : $(DISPLAY_DRIVER_PATH)\s6b1713\s6b1713.h
"$(OBJ_PATH)\s6b1713.obj" : $(DISPLAY_DRIVER_PATH)\displaydriver.h 
"$(OBJ_PATH)\s6b1713.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h 
"$(OBJ_PATH)\s6b1713.obj" : $(PROJECT_H)
"$(OBJ_PATH)\s6b1713.obj" : $(INC_PATH)\sysresourceapi.h

$(OBJ_PATH)\s6b1713init.obj : $(DISPLAY_DRIVER_PATH)\s6b1713\s6b1713init.c
$(OBJ_PATH)\s6b1713init.obj : $(DISPLAY_DRIVER_PATH)\s6b1713\s6b1713.h



endif