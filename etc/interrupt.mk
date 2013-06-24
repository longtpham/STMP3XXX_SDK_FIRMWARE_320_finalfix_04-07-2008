#interrupt.mk

ifndef INTERRUPT_MK
INTERRUPT_MK= TRUE

include $(ETC_PATH)\registers.mk

SYSTEMLIBS = $(SYSTEMLIBS)\
    $(OBJ_PATH)\interrupt.a

LK_OPTS = $(LK_OPTS) -uIPRSetIVLPriority -uFSysGetIrqLevel -uFSysSetIrqLevel -uIcollResetVectorTable

INTERRUPT_OBJ_LIST = $(INTERRUPT_OBJ_LIST)\
    $(OBJ_PATH)\icoll_resetvectortable.obj \
    $(OBJ_PATH)\icoll_c.obj \
    $(OBJ_PATH)\icoll_invalidvector.obj \
    $(OBJ_PATH)\sysirq.obj \
    $(OBJ_PATH)\ipr.obj

$(OBJ_PATH)\interrupt.a : $(INTERRUPT_OBJ_LIST) 
    @echo Creating IEEE archive library file for interrupt library $@
        @$(AR_EXE) -r $@ $!


$(OBJ_PATH)\icoll_resetvectortable.obj  : $(SYS_PATH)\hardware\interrupt\icoll_resetvectortable.asm 
$(OBJ_PATH)\icoll_resetvectortable.obj  : $(XREF_PATH)\const.xref
$(OBJ_PATH)\icoll_resetvectortable.obj  : $(SYS_PATH)\common\syserror.asm

$(OBJ_PATH)\icoll_invalidvector.obj     : $(SYS_PATH)\hardware\interrupt\icoll_invalidvector.c 
$(OBJ_PATH)\icoll_invalidvector.obj     : $(INC_PATH)\types.h


$(OBJ_PATH)\icoll_c.obj                 : $(SYS_PATH)\hardware\interrupt\icoll_c.c
$(OBJ_PATH)\icoll_c.obj                 : $(OUTPUT_INCLUDE_PATH)\regsicoll.h

$(OBJ_PATH)\sysirq.obj                  : $(SYS_PATH)\hardware\interrupt\sysirq.c


$(OBJ_PATH)\ipr.obj                     : $(SYS_PATH)\hardware\interrupt\ipr.asm


endif
