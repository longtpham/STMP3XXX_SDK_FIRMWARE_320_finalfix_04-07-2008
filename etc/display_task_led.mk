ifndef DISPLAY_TASK_LED_MK
DISPLAY_TASK_LED_MK=TRUE
include $(ETC_PATH)\includes.mk

DISPLAY_DRIVER_PATH=$(ROOT)\DeviceDriver\Display

DISPLAYSECTIONS = $(DISPLAYSECTIONS)\
        .ptextdisplaytask

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\displaytask_led.obj\
          $(OBJ_PATH)\displaytaskmem.obj\
	  $(OBJ_PATH)\heartbeat.obj\
	  $(OBJ_PATH)\LED_hal.obj


"$(OBJ_PATH)\displaytask_led.obj" : $(SYS_PATH)\tasks\display\displaytask_led.c 
"$(OBJ_PATH)\displaytask_led.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h
"$(OBJ_PATH)\displaytask_led.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h

"$(OBJ_PATH)\displaytaskmem.obj" : $(SYS_PATH)\tasks\display\displaytaskmem.c 
"$(OBJ_PATH)\displaytaskmem.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h

"$(OBJ_PATH)\heartbeat.obj" : $(PROJ_PATH)\heartbeat.c
"$(OBJ_PATH)\heartbeat.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h

"$(OBJ_PATH)\LED_hal.obj"   : $(DISPLAY_DRIVER_PATH)\LED\LED_hal.c
"$(OBJ_PATH)\LED_hal.obj"   : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
"$(OBJ_PATH)\LED_hal.obj"   : $(INC_PATH)\display_usb_led.h

endif