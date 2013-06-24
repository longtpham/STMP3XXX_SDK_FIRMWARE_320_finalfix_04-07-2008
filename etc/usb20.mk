ifndef USB20_MK
USB20_MK=TRUE
#This is the makefile for the usb 2.0 driver.
#

include $(ETC_PATH)\registers.mk

INC_ALL=$(INC_ALL);$(USB_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(USB_PATH)\Ch9                  

OBJLIST = $(OBJLIST)\
    "$(OBJ_PATH)\usb20dd.obj" \
    "$(OBJ_PATH)\usb20ddmem.obj" \
    "$(OBJ_PATH)\usb20ch9.obj" \
    "$(OBJ_PATH)\packdesc.obj"\
    "$(OBJ_PATH)\phy.obj"\
    "$(OBJ_PATH)\phy_asm.obj"
    
 

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
USB20_DEPENDENCIES = $(USB20_DEPENDENCIES) \
    $(OUTPUT_INCLUDE_PATH)\regsusb20.h \
	$(OUTPUT_INCLUDE_PATH)\regsusb20phy.h \
    $(INC_PATH)\usb_api.h \
    "$(OUTPUT_INCLUDE_PATH)\regsrevision.h" \ 
    "$(OUTPUT_INCLUDE_PATH)\regsdcdc.h" \ 
    "$(OUTPUT_INCLUDE_PATH)\regsclkctrl.h"
    
    
    
#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\usb20dd.obj" : "$(USB_PATH)\usb20\DevCtrlInt\usb20dd.c" $(USB20_DEPENDENCIES)

"$(OBJ_PATH)\usb20ddmem.obj" : "$(USB_PATH)\usb20\DevCtrlInt\usb20ddmem.asm" $(USB20_DEPENDENCIES)

"$(OBJ_PATH)\usb20ch9.obj" : "$(USB_PATH)\usb20\Ch9\usb20ch9.c" $(USB20_DEPENDENCIES)

"$(OBJ_PATH)\packdesc.obj" : "$(USB_PATH)\usb20\Ch9\packdesc.c" "$(OUTPUT_INCLUDE_PATH)\regsrevision.h" $(USB20_DEPENDENCIES)

"$(OBJ_PATH)\phy.obj"       : "$(USB_PATH)\usb20\phy.c" 

"$(OBJ_PATH)\phy_asm.obj"   : "$(USB_PATH)\usb20\phy_asm.asm" 
 

endif