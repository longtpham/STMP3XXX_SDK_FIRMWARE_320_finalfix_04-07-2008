ifndef USB11_MK
USB11_MK=TRUE
#This is the makefile for the usb 1.1 driver.
#
include $(ETC_PATH)\stringlib.mk


INC_ALL=$(INC_ALL);$(USB_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(USB_PATH)\Ch9                  

OBJLIST = $(OBJLIST)\
    $(OBJ_PATH)\powerontable.obj\
    $(OBJ_PATH)\usb11dd_isr.obj\
    $(OBJ_PATH)\usbrequests.obj\
    $(OBJ_PATH)\usbcopy.obj\
    $(OBJ_PATH)\usb11dd.obj

 

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
    
   
#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\powerontable.obj" : $(USB_PATH)\usb11\powerontable.c
"$(OBJ_PATH)\powerontable.obj" : $(OUTPUT_INCLUDE_PATH)\project.h

"$(OBJ_PATH)\usb11dd.obj" : "$(USB_PATH)\usb11\usb11dd.c" 
"$(OBJ_PATH)\usb11dd.obj" : $(USB_DEPENDENCIES) 
"$(OBJ_PATH)\usb11dd.obj" : $(OUTPUT_INCLUDE_PATH)\regsusb.h
"$(OBJ_PATH)\usb11dd.obj" : $(OUTPUT_INCLUDE_PATH)\regsspare.h

"$(OBJ_PATH)\usb11dd_isr.obj" : "$(USB_PATH)\usb11\usb11dd_isr.asm" 

"$(OBJ_PATH)\usb11ddmem.obj" : "$(USB_PATH)\DevCtrlInt\usb11ddmem.asm" $(USB_DEPENDENCIES)

$(OBJ_PATH)\usbcopy.obj : $(USB_PATH)\usb11\usbcopy.c

$(OBJ_PATH)\usbrequests.obj : $(USB_PATH)\usb11\usbrequests.c
endif