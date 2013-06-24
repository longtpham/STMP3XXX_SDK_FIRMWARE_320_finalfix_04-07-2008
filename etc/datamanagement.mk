#datamanagement.mk

ifndef DATAMANGEMENT_MK
DATAMANGEMENT_MK= TRUE

SYSTEMLIBS = $(SYSTEMLIBS)\
    $(OBJ_PATH)\datamanagement.a

LK_OPTS = $(LK_OPTS) -u QueueGetFilledSpace -u QueueGetEmptySpace -u SaveSystemStack -u RestoreSystemStack

DATAMANGEMENT_OBJ_LIST = $(DATAMANGEMENT_OBJ_LIST)\
    $(OBJ_PATH)\const.obj \
    $(OBJ_PATH)\queues.obj \
    $(OBJ_PATH)\stackcheck.obj\
    $(OBJ_PATH)\stack.obj\
    


$(OBJ_PATH)\datamanagement.a : $(DATAMANGEMENT_OBJ_LIST) 
    @echo Creating IEEE archive library file for data management library $@
        @$(AR_EXE) -r $@ $!


$(OBJ_PATH)\queues.obj                  : $(SYS_PATH)\data_management\queues.asm 
$(OBJ_PATH)\queues.obj                  : $(XREF_PATH)\const.xref

$(OBJ_PATH)\stackcheck.obj              : $(SYS_PATH)\data_management\stack\stackcheck.asm 
$(OBJ_PATH)\stackcheck.obj              : $(XREF_PATH)\const.xref

$(OBJ_PATH)\stack.obj                   : $(SYS_PATH)\data_management\stack\stack.asm 
$(OBJ_PATH)\stack.obj                   : $(SYS_PATH)\common\sysmacro.asm

$(OBJ_PATH)\const.obj                   : $(SYS_PATH)\data_management\const.asm


endif