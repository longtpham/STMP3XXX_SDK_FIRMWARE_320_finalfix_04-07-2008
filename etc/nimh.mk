#////////////////////////////////////////////////////////////////////////////////
#// Copyright(C) SigmaTel, Inc. 2004
#//
#// Filename: nimh.mk   
#// Description: Make the Nickel Metal Hydride battery charging firmware object
#////////////////////////////////////////////////////////////////////////////////
ifndef NIMH_MK
NIMH_MK=1

BLD = $(BLD) -DBATTERY_TYPE_NIMH


include $(ETC_PATH)\batterycharge.mk

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\nimh.obj

$(OBJ_PATH)\nimh.obj :     $(SYS_PATH)\hardware\batterycharge\nimh\nimh.c
$(OBJ_PATH)\nimh.obj :     $(MAIN_PATH)\battery_config.h

endif

