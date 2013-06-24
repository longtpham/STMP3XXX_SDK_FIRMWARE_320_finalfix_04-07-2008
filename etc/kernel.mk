ifndef KERNEL_MK
KERNEL_MK=TRUE
#kernel.mk

include "$(ETC_PATH)\datamanagement.mk"
include "$(ETC_PATH)\registers.mk"
include "$(ETC_PATH)\interrupt.mk"

OSLIB= $(OSLIB)\
    $(OBJ_PATH)\kernellib.a


include "$(ETC_PATH)\stdlib.mk"

LK_OPTS = $(LK_OPTS) -u Exec -u FSysWait

# Included to prevent FSysReleaseObject being "undefined" ?
LK_OPTS = $(LK_OPTS) -uFSysReleaseObject

KERNEL_OBJ_LIST = $(KERNEL_OBJ_LIST)\
    $(OBJ_PATH)\kernel_syscallfunction.obj\
    $(OBJ_PATH)\kernel_getnexttask.obj\
    $(OBJ_PATH)\kernel_mainloop.obj\
    $(OBJ_PATH)\kernel_startup.obj\
    $(OBJ_PATH)\kernel_mem.obj\
    $(OBJ_PATH)\kernel_bootstrap.obj\
    $(OBJ_PATH)\project_startup.obj\
    $(OBJ_PATH)\kernel_switchtotask.obj\
    $(OBJ_PATH)\kernel_istaskready.obj\
    $(OBJ_PATH)\kernel_contextswitch.obj\
    $(OBJ_PATH)\kernel_sysassignobject.obj\
    $(OBJ_PATH)\kernel_isrreleaseobject.obj\
    $(OBJ_PATH)\kernel_syswaitonmessage.obj\
    $(OBJ_PATH)\kernel_syswaitonobject.obj\
    $(OBJ_PATH)\kernel_syswaitonqueue.obj\
    $(OBJ_PATH)\kernel_syswait.obj\
    $(OBJ_PATH)\kernel_settimeout.obj \
    $(OBJ_PATH)\kernel_postmessage.obj \
    $(OBJ_PATH)\kernel_syspostmessage_c.obj \
    $(OBJ_PATH)\kernel_sysreleaseobject.obj\
    $(OBJ_PATH)\kernel_sysgettaskid.obj\
    $(OBJ_PATH)\kernel_issystemalive.obj\
    $(OBJ_PATH)\kernel_heartbeat.obj\
    $(OBJ_PATH)\kernel_ticklewatchdog.obj\
    $(OBJ_PATH)\kernel_syswaitonsignal.obj\
    $(OBJ_PATH)\kernel_syssignal.obj\

$(OBJ_PATH)\kernellib.a : $(KERNEL_OBJ_LIST) 
    @echo Creating IEEE archive library file for kernel $@
        @$(AR_EXE) -r $@ $!


$(OBJ_PATH)\kernel_syssignal.obj  : $(SYS_PATH)\kernel\kernel_syssignal.c
$(OBJ_PATH)\kernel_syssignal.obj  : $(SYS_PATH)\common\sysmacro.asm 


$(OBJ_PATH)\kernel_syswaitonsignal.obj  : $(SYS_PATH)\kernel\kernel_syswaitonsignal.asm
$(OBJ_PATH)\kernel_syswaitonsignal.obj  : $(SYS_PATH)\common\sysmacro.asm 

$(OBJ_PATH)\kernel_syscallfunction.obj  : $(SYS_PATH)\kernel\kernel_syscallfunction.asm
$(OBJ_PATH)\kernel_syscallfunction.obj  : $(SYS_PATH)\common\sysmacro.asm 

$(OBJ_PATH)\kernel_postmessage.obj   : $(SYS_PATH)\kernel\kernel_postmessage.asm 
$(OBJ_PATH)\kernel_postmessage.obj   : $(SYS_PATH)\common\sysmacro.asm 
$(OBJ_PATH)\kernel_postmessage.obj   : $(XREF_PATH)\const.xref


$(OBJ_PATH)\kernel_syspostmessage_c.obj : $(SYS_PATH)\kernel\kernel_syspostmessage_c.c
$(OBJ_PATH)\kernel_syspostmessage_c.obj : $(INC_PATH)\kernel.h
$(OBJ_PATH)\kernel_syspostmessage_c.obj : $(SYS_PATH)\kernel\kernel_internal.h

$(OBJ_PATH)\kernel_syswaitonmessage.obj : $(SYS_PATH)\kernel\kernel_syswaitonmessage.asm
$(OBJ_PATH)\kernel_syswaitonmessage.obj : $(SYS_PATH)\common\sysmacro.asm 
$(OBJ_PATH)\kernel_syswaitonmessage.obj : $(XREF_PATH)\const.xref
$(OBJ_PATH)\kernel_syswaitonmessage.obj : $(INC_PATH)\kernel.inc 

$(OBJ_PATH)\kernel_syswaitonobject.obj  : $(SYS_PATH)\kernel\kernel_syswaitonobject.asm
$(OBJ_PATH)\kernel_syswaitonobject.obj  : $(SYS_PATH)\common\sysmacro.asm 
$(OBJ_PATH)\kernel_syswaitonobject.obj  : $(XREF_PATH)\const.xref
$(OBJ_PATH)\kernel_syswaitonobject.obj  : $(INC_PATH)\kernel.inc 

$(OBJ_PATH)\kernel_sysassignobject.obj  : $(SYS_PATH)\common\sysmacro.asm 
$(OBJ_PATH)\kernel_sysassignobject.obj  : $(SYS_PATH)\kernel\kernel_sysassignobject.asm


$(OBJ_PATH)\kernel_syswaitonqueue.obj   : $(SYS_PATH)\kernel\kernel_syswaitonqueue.asm
$(OBJ_PATH)\kernel_syswaitonqueue.obj   : $(SYS_PATH)\common\sysmacro.asm 
$(OBJ_PATH)\kernel_syswaitonqueue.obj   : $(XREF_PATH)\const.xref
$(OBJ_PATH)\kernel_syswaitonqueue.obj   : $(INC_PATH)\kernel.inc 

$(OBJ_PATH)\kernel_syswait.obj          : $(SYS_PATH)\kernel\kernel_syswait.c
$(OBJ_PATH)\kernel_syswait.obj          : $(INC_PATH)\kernel.h
$(OBJ_PATH)\kernel_syswait.obj          : $(SYS_PATH)\kernel\kernel_internal.h

$(OBJ_PATH)\kernel_contextswitch.obj    : $(SYS_PATH)\kernel\kernel_contextswitch.asm
$(OBJ_PATH)\kernel_contextswitch.obj    : $(SYS_PATH)\common\sysmacro.asm 

$(OBJ_PATH)\kernel_incrementtick.obj    : $(SYS_PATH)\kernel\kernel_incrementtick.c
$(OBJ_PATH)\kernel_incrementtick.obj    : $(SYS_PATH)\kernel\kernel_internal.h

$(OBJ_PATH)\kernel_settimeout.obj       : $(SYS_PATH)\kernel\kernel_settimeout.c
$(OBJ_PATH)\kernel_settimeout.obj       : $(INC_PATH)\kernel.h
$(OBJ_PATH)\kernel_settimeout.obj       : $(SYS_PATH)\kernel\kernel_internal.h

$(OBJ_PATH)\kernel_sysgettaskid.obj     : $(SYS_PATH)\kernel\kernel_sysgettaskid.c
$(OBJ_PATH)\kernel_sysgettaskid.obj     : $(INC_PATH)\kernel.h
$(OBJ_PATH)\kernel_sysgettaskid.obj     : $(SYS_PATH)\kernel\kernel_internal.h

$(OBJ_PATH)\kernel_istaskready.obj      : $(SYS_PATH)\kernel\kernel_istaskready.c
$(OBJ_PATH)\kernel_istaskready.obj      : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_istaskready.obj      : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_switchtotask.obj     : $(SYS_PATH)\kernel\kernel_switchtotask.c
$(OBJ_PATH)\kernel_switchtotask.obj     : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_switchtotask.obj     : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_getnexttask.obj      : $(SYS_PATH)\kernel\kernel_getnexttask.c
$(OBJ_PATH)\kernel_getnexttask.obj      : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_getnexttask.obj      : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_mainloop.obj         : $(SYS_PATH)\kernel\kernel_mainloop.c
$(OBJ_PATH)\kernel_mainloop.obj         : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_mainloop.obj         : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_startup.obj          : $(SYS_PATH)\kernel\kernel_startup.c
$(OBJ_PATH)\kernel_startup.obj          : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_startup.obj          : $(INC_PATH)\kernel.h
$(OBJ_PATH)\kernel_startup.obj          : $(INC_PATH)\icoll.h

$(OBJ_PATH)\kernel_mem.obj              : $(SYS_PATH)\kernel\kernel_mem.c
$(OBJ_PATH)\kernel_mem.obj              : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_mem.obj              : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_bootstrap.obj        : $(SYS_PATH)\kernel\kernel_bootstrap.asm

$(OBJ_PATH)\project_startup.obj         : $(SYS_PATH)\kernel\project_startup.asm
$(OBJ_PATH)\project_startup.obj         : $(SYS_PATH)\common\sysmacro.asm 

$(OBJ_PATH)\kernel_sysreleaseobject.obj : $(SYS_PATH)\kernel\kernel_sysreleaseobject.c
$(OBJ_PATH)\kernel_sysreleaseobject.obj : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_sysreleaseobject.obj : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_isrreleaseobject.obj : $(SYS_PATH)\kernel\kernel_isrreleaseobject.c
$(OBJ_PATH)\kernel_isrreleaseobject.obj : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_isrreleaseobject.obj : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_issystemalive.obj    : $(SYS_PATH)\kernel\kernel_issystemalive.c
$(OBJ_PATH)\kernel_issystemalive.obj    : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_issystemalive.obj    : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_heartbeat.obj        : $(SYS_PATH)\kernel\kernel_heartbeat.c
$(OBJ_PATH)\kernel_heartbeat.obj        : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_heartbeat.obj        : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_ticklewatchdog.obj   : $(SYS_PATH)\kernel\kernel_ticklewatchdog.c
$(OBJ_PATH)\kernel_ticklewatchdog.obj   : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_ticklewatchdog.obj   : $(INC_PATH)\kernel.h

$(OBJ_PATH)\kernel_isrreleaseobject.obj : $(SYS_PATH)\kernel\kernel_isrreleaseobject.c
$(OBJ_PATH)\kernel_isrreleaseobject.obj : $(SYS_PATH)\kernel\kernel_internal.h
$(OBJ_PATH)\kernel_isrreleaseobject.obj : $(INC_PATH)\kernel.h

endif