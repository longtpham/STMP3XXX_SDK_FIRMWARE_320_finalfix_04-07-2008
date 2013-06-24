#////////////////////////////////////////////////////////////////////////////////
#// Copyright(C) SigmaTel, Inc. 2003
#//
#// Filename: batterycharge.mk   
#// Description: 
#////////////////////////////////////////////////////////////////////////////////
ifndef BATTERY_CHARGE_MK
BATTERY_CHARGE_MK=1

BLD = $(BLD) -DBATTERY_CHARGE

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\batterycharge.obj\
    $(OBJ_PATH)\batterychargetask.obj

$(OBJ_PATH)\batterycharge.obj :     $(SYS_PATH)\hardware\batterycharge\batterycharge.c

$(OBJ_PATH)\batterychargetask.obj :     $(SYS_PATH)\hardware\batterycharge\batterychargetask.c

endif