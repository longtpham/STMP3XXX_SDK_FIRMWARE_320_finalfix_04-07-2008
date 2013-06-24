ifndef USBMSC_MK
USBMSC_MK = TRUE
#This is the makefile for the USB MSC driver.
#

include $(ETC_PATH)\usb.mk
include $(ETC_PATH)\scsi.mk

USBMSC_PATH =   $(ROOT)\DeviceDriver\Media\usbmsc
MTP_PATH    =   $(ROOT)\DeviceDriver\Media\mtp

INC_ALL     =   $(INC_ALL);$(USBMSC_PATH);$(MTP_PATH)
INC_ALL_C   =   $(INC_ALL_C) -Wc-I$(USBMSC_PATH) -Wc-I$(MTP_PATH)

GENERALLIB =   $(USBMSCLIB) \
                $(GENERALLIB)

ifdef D_PLIBS
USBMSCLIB   =   $(OBJ_PATH)\usbmsc.a
else                
USBMSCLIB   =   $(CHIP_LIB_PATH)\usbmsc.a
endif

USB_MSCDESCRIPTORS = $(OBJ_PATH)\descriptors_3500.obj

USBMSCLIST  =   \
                $(OBJ_PATH)\usbmsc.obj\
                $(OBJ_PATH)\usbmscrequests.obj\

ifdef MTP_BUILD
OBJLIST = $(OBJLIST)\
                $(OBJ_PATH)\usbmsc_services.obj\
                $(OBJ_PATH)\usbmscMtpSwitch.obj

else
OBJLIST = $(OBJLIST)\
                $(USB_MSCDESCRIPTORS)\
                $(OBJ_PATH)\usbmsc_services.obj\
                $(OBJ_PATH)\usbmscMtpSwitch.obj
endif


#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
USBMSC_DEPENDENCIES= \
    "$(INC_PATH)\usbmsc.h" \
    "$(USBMSC_PATH)\usbmscrequests.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsdcdc.h" \
    "$(OUTPUT_INCLUDE_PATH)\regscodec.h" \
    "$(OUTPUT_INCLUDE_PATH)\regslradc.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsicoll.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsemc.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsgpflash.h" \
    "$(OUTPUT_INCLUDE_PATH)\regsusb20.h" \
    $(USB_DEPENDENCIES)
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

"$(USBMSCLIB)" : $(USBMSCLIST)
    @echo Creating IEEE archive library file for USB Driver$@
        @$(AR_EXE) -r $(USBMSCLIB) $(USBMSCLIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\usbmsc.obj" : "$(USBMSC_PATH)\usbmsc.c" $(USBMSC_DEPENDENCIES)

"$(OBJ_PATH)\usbmscrequests.obj" : "$(USBMSC_PATH)\usbmscrequests.c" $(USBMSC_DEPENDENCIES)


endif

$(OBJ_PATH)\usbmsc_services.obj : "$(USBMSC_PATH)\usbmsc_services.c" $(USBMSC_DEPENDENCIES)

"$(OBJ_PATH)\usbmscMtpSwitch.obj" : "$(USBMSC_PATH)\usbmscMtpSwitch.c" $(USBMSC_DEPENDENCIES)

endif 
