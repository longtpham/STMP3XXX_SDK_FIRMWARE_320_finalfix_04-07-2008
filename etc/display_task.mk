ifndef DISPLAY_TASK_MK
DISPLAY_TASK_MK=TRUE
include $(ETC_PATH)\includes.mk

include $(ETC_PATH)\display_usbmsc.mk

DISPLAY_DRIVER_PATH=$(ROOT)\devicedriver\display

DISPLAYSECTIONS = $(DISPLAYSECTIONS)\
        .ptextdisplaytask

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\displaytask.obj\
          $(OBJ_PATH)\displaytaskmem.obj


$(OBJ_PATH)\displaytask.obj : $(SYS_PATH)\tasks\display\displaytask.c 
$(OBJ_PATH)\displaytask.obj : $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\displaytask.obj : $(INC_PATH)\displaydrv.h

$(OBJ_PATH)\displaytaskmem.obj : $(SYS_PATH)\tasks\display\displaytaskmem.c 
$(OBJ_PATH)\displaytaskmem.obj : $(OUTPUT_INCLUDE_PATH)\resource.h

endif