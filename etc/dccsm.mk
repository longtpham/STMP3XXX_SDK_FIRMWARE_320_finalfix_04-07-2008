#This is the makefile for the DCC SmartMedia command handler
#
#To include the SmartMedia command handler in a DCC project
#   1. Include this makefile in the project makefile
#   2. Add $(DCCSMLIST) to the list of linker dependencies

DCCSMLIST   = \
            $(OBJ_PATH)\dctlsm.obj\
            $(OBJ_PATH)\dctlsmmem.obj\
            $(OBJ_PATH)\smedia.obj\
            $(OBJ_PATH)\smmem.obj

ifndef INCLUDE_CHKDSK
DCCSMLIST   = $(DCCSMLIST)\
			$(OBJ_PATH)\convertcs.obj\
            "$(OBJ_PATH)\smartmediadetection.obj"\
			$(OBJ_PATH)\smartmediaserialnumber.obj\
			$(OBJ_PATH)\smdd_gpio.obj
endif



#///////////////////////////////////////////////////////////////////////////////
# SmartMedia command handler files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\dctlsm.obj" : $(DCCDEVSM_PATH)\dctlsm.asm

"$(OBJ_PATH)\dctlsmmem.obj" : $(DCCDEVSM_PATH)\dctlsmmem.asm

"$(OBJ_PATH)\smedia.obj" : $(DCCDEVSM_PATH)\smedia.asm

"$(OBJ_PATH)\smmem.obj" : $(DCCDEVSM_PATH)\smmem.asm

"$(OBJ_PATH)\smartmediadetection.obj"   :"$(DDI_SM_PATH)\smartmediadetection.asm" $(COMMON_PROJ_PATH)\project.inc

$(OBJ_PATH)\smdd_gpio.obj           : $(DDI_SM_PATH)\fat\hal\smdd_gpio.asm

"$(OBJ_PATH)\smartmediaserialnumber.obj"   :"$(DDI_SM_PATH)\smartmediaserialnumber.c" 

$(OBJ_PATH)\convertcs.obj : $(DDI_SM_PATH)\fat\ddi\convertcs.asm

