#prevent double inclusion
ifndef EXEC_MK
EXEC_MK = TRUE


OBJLIST =$(OBJLIST)\
    $(OBJ_PATH)\syspostmsg.obj\
    $(OBJ_PATH)\getmessage.obj\
    $(OBJ_PATH)\systime.obj\
    $(OBJ_PATH)\sysmodule.obj\
    $(OBJ_PATH)\modulemsg.obj

$(OBJ_PATH)\syspostmsg.obj  : $(SYS_COMMON_PATH)\syspostmsg.asm

$(OBJ_PATH)\sysmodule.obj   : $(SYS_EXEC_PATH)\sysmodule.asm

$(OBJ_PATH)\getmessage.obj  : $(SYS_COMMON_PATH)\getmessage.asm

$(OBJ_PATH)\modulemsg.obj   : $(PROJ_PATH)\system\modulemsg.asm

$(OBJ_PATH)\systime.obj     : $(SYS_COMMON_PATH)\systime.asm


$(OUTPUT_INCLUDE_PATH)\messages.h : $(INC_PATH)\msgequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 



#end of prevent double inclusion
endif 
