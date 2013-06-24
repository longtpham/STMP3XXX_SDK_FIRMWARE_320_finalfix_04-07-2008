#resourcey.mk

ifndef RESOURCE_MK
RESOURCE_MK=1

include $(ETC_PATH)\smdd.mk

include $(ETC_PATH)\datamanagement.mk

DRIVERLIBS = $(DRIVERLIBS)\
    $(OBJ_PATH)\resource_nand.a

LK_OPTS = $(LK_OPTS) 

RESOURCE_OBJ_LIST = $(RESOURCE_OBJ_LIST)\
    $(OBJ_PATH)\sysresource_light_fs.obj \
    $(OBJ_PATH)\sysresource_nand_dd.obj \

$(OBJ_PATH)\resource_nand.a : $(RESOURCE_OBJ_LIST) 
    @echo Creating IEEE archive library file for resource library $@
        @$(AR_EXE) -r $@ $!


$(OBJ_PATH)\sysresource_light_fs.obj : $(SYS_PATH)\data_management\resources\sysresource_light_fs.asm
$(OBJ_PATH)\sysresource_light_fs.obj : $(XREF_PATH)\const.xref
$(OBJ_PATH)\sysresource_light_fs.obj : $(SYS_PATH)\common\sysmacro.asm


$(OBJ_PATH)\sysresource_nand_dd.obj  : $(DD_PATH)\media\smartmedia\system\sysresource_nand_dd.asm

endif   




















