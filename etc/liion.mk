#////////////////////////////////////////////////////////////////////////////////
#// Copyright(C) SigmaTel, Inc. 2003-2004
#//
#// Filename: liion.mk             May include batterycharge.mk   
#////////////////////////////////////////////////////////////////////////////////
ifndef LIION_MK
LIION_MK=1

BLD = $(BLD) -DBATTERY_TYPE_LI_ION
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DBATTERY_TYPE_LI_ION -Wc-DBATTERY_TYPE_LI_ION
# player asm and c files need define above to know if LIION levels for LRADC Vbat check
# which is used in at least 4 source files. Also allows for correct LIION brownout levels. 

ifdef BUILD_BATTERY_CHARGE
#Battery charging enabled.  batterycharge.obj will reside in system space since it contains 
#small function callable by the system (can be funcletized if needed).  batterychargetask.obj 
#and liion.obj will be combined inside a batterychargecodebank.

BLD = $(BLD) -DBATTERY_CHARGE
   
OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\liion.obj\
	$(OBJ_PATH)\batterycharge.obj\
    $(OBJ_PATH)\batterychargetask.obj\

$(OBJ_PATH)\liion.obj :     $(SYS_PATH)\hardware\batterycharge\liion\liion.c
$(OBJ_PATH)\liion.obj :     $(MAIN_PATH)\battery_config.h

$(OBJ_PATH)\batterycharge.obj :     $(SYS_PATH)\hardware\batterycharge\batterycharge.c

$(OBJ_PATH)\batterychargetask.obj :     $(SYS_PATH)\hardware\batterycharge\batterychargetask.c

####################################################################################################################
# So far no resources needed if build in MASS STORAGE or MTP
####################################################################################################################
ifdef STMP_BUILD_PLAYER 
MODULELIST = $(MODULELIST)\
    $(OUTPUT_RSRC_PATH)\batterychargecodebank.src

"$(OUTPUT_RSRC_PATH)\batterychargecodebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract batterychargecodebank 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextbatterychargetask,.pdatabatterychargetask,.ptextliion -f$(OUTPUT_PATH)\$(PROJ).map
endif

else
########################
# No Battery Charging  #
########################

####################################################################################################################
# So far no resources needed if build in MASS STORAGE or MTP
####################################################################################################################
ifdef STMP_BUILD_PLAYER 
MODULELIST = $(MODULELIST)\
    $(OUTPUT_RSRC_PATH)\batterychargecodebank.src

#the following are null sources if using batterychargecodebank. Here to make the resource linker happy.
"$(OUTPUT_RSRC_PATH)\batterychargecodebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL batterychargecodebank.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\batterychargecodebank.src
endif

endif

endif