#overlay.mk

ifndef OVERLAY_MK
OVERLAY_MK=TRUE

CODEBANKS=TRUE

OSLIB= $(OSLIB)\
    $(OBJ_PATH)\overlaylib.a

include "$(ETC_PATH)\kernel.mk"


LK_OPTS = $(LK_OPTS) -u FKernel_IsTaskReady -u FKernel_Startup -u SysLoadResource


OVERLAY_OBJ_LIST = $(OVERLAY_OBJ_LIST)\
    $(OBJ_PATH)\overlay_istaskready.obj\
    $(OBJ_PATH)\overlay_syscallfunction.obj\
    $(OBJ_PATH)\overlay_istaskloaded.obj\
    $(OBJ_PATH)\kernel_getregion.obj\
    $(OBJ_PATH)\memorysetup.obj\
    $(OBJ_PATH)\overlay_startup.obj\
    $(OBJ_PATH)\overlaymanager.obj

#	$(OBJ_PATH)\overlay_contextswitch.obj

$(OBJ_PATH)\overlaylib.a : $(OVERLAY_OBJ_LIST) 
    @echo Creating IEEE archive library file for kernel overlay $@
        $(AR_EXE) -r $@ $!



$(OBJ_PATH)\memorysetup.obj         : $(PROJ_PATH)\memorysetup.c


$(OBJ_PATH)\overlaymanager.obj  : $(SYS_PATH)\kernel\overlaymanager.c


$(OBJ_PATH)\overlay_istaskloaded.obj  : $(SYS_PATH)\kernel\overlay_istaskloaded.c
$(OBJ_PATH)\overlay_istaskloaded.obj  : $(SYS_PATH)\common\sysmacro.asm 


$(OBJ_PATH)\overlay_syscallfunction.obj  : $(SYS_PATH)\kernel\overlay_syscallfunction.asm
$(OBJ_PATH)\overlay_syscallfunction.obj  : $(SYS_PATH)\common\sysmacro.asm 

$(OBJ_PATH)\overlay_contextswitch.obj    : $(SYS_PATH)\kernel\overlay_contextswitch.asm
$(OBJ_PATH)\overlay_contextswitch.obj    : $(SYS_PATH)\common\sysmacro.asm 

$(OBJ_PATH)\kernel_getregion.obj        : $(SYS_PATH)\kernel\kernel_getregion.asm
$(OBJ_PATH)\kernel_getregion.obj        : $(SYS_PATH)\common\sysmacro.asm 
    

$(OBJ_PATH)\overlay_istaskready.obj      : $(SYS_PATH)\kernel\overlay_istaskready.c
$(OBJ_PATH)\overlay_istaskready.obj      : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\overlay_istaskready.obj      : $(INC_PATH)\kernel.h

$(OBJ_PATH)\overlay_switchtotask.obj     : $(SYS_PATH)\kernel\overlay_switchtotask.c
$(OBJ_PATH)\overlay_switchtotask.obj     : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\overlay_switchtotask.obj     : $(INC_PATH)\kernel.h

$(OBJ_PATH)\overlay_getnexttask.obj      : $(SYS_PATH)\kernel\overlay_getnexttask.c
$(OBJ_PATH)\overlay_getnexttask.obj      : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\overlay_getnexttask.obj      : $(INC_PATH)\kernel.h

$(OBJ_PATH)\overlay_startup.obj      : $(SYS_PATH)\kernel\overlay_startup.c
$(OBJ_PATH)\overlay_startup.obj      : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\overlay_startup.obj      : $(SYS_PATH)\kernel\overlay_internal.h
$(OBJ_PATH)\overlay_startup.obj      : $(INC_PATH)\kernel.h


endif