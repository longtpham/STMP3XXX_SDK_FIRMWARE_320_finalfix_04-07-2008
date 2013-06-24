#////////////////////////////////////////////////////////////////////////////////
#// Copyright(C) SigmaTel, Inc. 2003-2004
#//
#// Filename: alkaline.mk              
#// Description: 
#////////////////////////////////////////////////////////////////////////////////
ifndef ALKALINE_MK
ALKALINE_MK=1

BLD = $(BLD) -DBATTERY_TYPE_ALKALINE
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DBATTERY_TYPE_ALKALINE -Wc-DBATTERY_TYPE_ALKALINE
# player asm and c files need define above to know if ALKALINE levels for LRADC Vbat check
# which is used in at least 4 source files. Also allows for correct ALKALINE brownout levels. 


OBJLIST=    $(OBJLIST)\

MODULELIST = $(MODULELIST)\
    $(OUTPUT_RSRC_PATH)\batterychargecodebank.src

#No battery charge in alkaline players.   
#the following are null sources if using batterychargecodebank. Here to make the resource linker happy.
"$(OUTPUT_RSRC_PATH)\batterychargecodebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL batterychargecodebank.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\batterychargecodebank.src
endif