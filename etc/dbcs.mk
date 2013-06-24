ifndef DBCS_MK
DBCS_MK= TRUE

include $(ETC_PATH)\stringlib.mk

OBJLIST =$(OBJLIST)\
    $(OBJ_PATH)\dbcs.obj\

$(OBJ_PATH)\dbcs.obj: $(SYS_COMMON_PATH)\dbcs.c 
$(OBJ_PATH)\dbcs.obj: $(OUTPUT_INCLUDE_PATH)\resource.h 



endif