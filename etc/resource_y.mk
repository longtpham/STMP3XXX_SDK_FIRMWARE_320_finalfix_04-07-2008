#resourcey.mk

ifndef RESOURCE_MK
RESOURCE_MK=1

include $(ETC_PATH)\datamanagement.mk

DRIVERLIBS= $(DRIVERLIBS)\
    $(OBJ_PATH)\resource_y.a

LK_OPTS = $(LK_OPTS) 

RESOURCE_OBJ_LIST = $(RESOURCE_OBJ_LIST)\
    $(OBJ_PATH)\sysresourcey.obj \

$(OBJ_PATH)\resource_y.a : $(RESOURCE_OBJ_LIST) 
    @echo Creating IEEE archive library file for resource library $@
        @$(AR_EXE) -r $@ $!


$(OBJ_PATH)\sysresourcey.obj                  : $(SYS_PATH)\data_management\resources\sysresourcey.asm 
$(OBJ_PATH)\sysresourcey.obj                  : $(XREF_PATH)\const.xref


endif   




















