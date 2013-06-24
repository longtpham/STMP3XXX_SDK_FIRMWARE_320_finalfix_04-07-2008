ifndef USB_MK
USB_MK = TRUE
#This is the makefile for the usb driver.
#

USB_PATH = $(ROOT)\DeviceDriver\Media\usb\


include $(ETC_PATH)\usb20.mk

OBJLIST = $(OBJ_PATH)\usb_api.obj\
          $(OBJ_PATH)\usb_descriptor.obj\
          $(OBJLIST)\
       


$(OBJ_PATH)\usb_api.obj : $(USB_PATH)\usb_api.c
$(OBJ_PATH)\usb_api.obj : $(OUTPUT_INCLUDE_PATH)\project.h

$(OBJ_PATH)\usb_descriptor.obj : $(USB_PATH)\usb_descriptor.c
$(OBJ_PATH)\usb_descriptor.obj : $(OUTPUT_INCLUDE_PATH)\project.h

endif
