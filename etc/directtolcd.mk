
OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\directtolcd.obj\

DISPLAY_SAL_SECTIONS=.ptextdirecttolcd

"$(OBJ_PATH)\directtolcd.obj" : $(HW_MSGMODULES_PATH)\Display\SAL\directtolcd\directtolcd.c
"$(OBJ_PATH)\directtolcd.obj" : $(DISPLAY_DRIVER_PATH)\displaydriver.h
"$(OBJ_PATH)\directtolcd.obj" : $(OUTPUT_INCLUDE_PATH)\resource.h
"$(OBJ_PATH)\directtolcd.obj" : $(OUTPUT_INCLUDE_PATH)\sysequ.h
"$(OBJ_PATH)\directtolcd.obj" : $(OUTPUT_INCLUDE_PATH)\sysresources.h
"$(OBJ_PATH)\directtolcd.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h


