#This is the makefile for the Device Control Class (DCC) common files
#All DCC projects should include this makefile
#
#To include the common DCC files in a DCC project
#   1. Include this makefile in the project makefile
#   2. Add $(DCCCOMLIST) to the list of linker dependencies


include $(ROOT)\etc\stringlib.mk


DCCCOMLIST = \
            $(OBJ_PATH)\dcc.obj\
            $(OBJ_PATH)\dccui.obj\
            $(OBJ_PATH)\dctldrv.obj\
            $(OBJ_PATH)\dctlsector.obj\
            $(OBJ_PATH)\dctlmem.obj\
            $(OBJ_PATH)\dctlusb.obj\
            $(OBJ_PATH)\dctlusbmem.obj\
            $(OBJ_PATH)\usbmem.obj\
            $(OBJ_PATH)\mediadetection.obj\
            $(OBJ_PATH)\usbproject.obj\
			$(OBJ_PATH)\dccmiscinit.obj\

######################################
#include the System Serial Number makefile
include $(RELBASE_PATH)\etc\sysserialnumber.mk
######################################

#///////////////////////////////////////////////////////////////////////////////
# Common files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\dcc.obj" : $(DCC_COMMON_PATH)\dcc.asm

"$(OBJ_PATH)\dccui.obj" : $(DCC_COMMON_PATH)\dccui.asm

"$(OBJ_PATH)\dctldrv.obj" : $(DCC_COMMON_PATH)\dctldrv.asm

"$(OBJ_PATH)\dctlmem.obj" : $(DCC_COMMON_PATH)\dctlmem.asm

"$(OBJ_PATH)\dctlsector.obj" : $(DCC_COMMON_PATH)\dctlsector.asm

$(OBJ_PATH)\mediadetection.obj : $(SYS_COMMON_PATH)\mediadetection.c

"$(OBJ_PATH)\dccmiscinit.obj" : $(PROJ_PATH)\dccmiscinit.c


#///////////////////////////////////////////////////////////////////////////////
# USB command input/output driver
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\dctlusb.obj" : $(DCCDEVUSB_PATH)\dctlusb.asm

"$(OBJ_PATH)\dctlusbmem.obj" : $(DCCDEVUSB_PATH)\dctlusbmem.asm

"$(OBJ_PATH)\usbmem.obj" : $(DCCDEVUSB_PATH)\usbmem.asm


$(OBJ_PATH)\usbproject.obj : $(PROJ_PATH)\usbproject.c
$(OBJ_PATH)\usbproject.obj : $(PROJECT_H)
