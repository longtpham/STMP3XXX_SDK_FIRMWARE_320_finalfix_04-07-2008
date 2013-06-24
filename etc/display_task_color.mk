ifndef DISPLAY_TASK_MK
DISPLAY_TASK_MK=TRUE
include $(ETC_PATH)\includes.mk

include $(ETC_PATH)\display_usbmsc_color.mk

DISPLAY_DRIVER_PATH=$(ROOT)\devicedriver\display

DISPLAYSECTIONS = $(DISPLAYSECTIONS)\
        .ptextdisplaytask_color

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\displaytask_color.obj\
          $(OBJ_PATH)\displaytaskmem.obj


$(OBJ_PATH)\displaytask_color.obj : $(SYS_PATH)\tasks\display\displaytask_color.c 
$(OBJ_PATH)\displaytask_color.obj : $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\displaytask_color.obj : $(INC_PATH)\displaydrv_color.h

$(OBJ_PATH)\displaytaskmem.obj : $(SYS_PATH)\tasks\display\displaytaskmem.c 
$(OBJ_PATH)\displaytaskmem.obj : $(OUTPUT_INCLUDE_PATH)\resource.h

endif