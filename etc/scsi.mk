ifndef SCSI_MK
SCSI_MK=TRUE
#This is the makefile for the scsi driver.
#
include $(ETC_PATH)\stdlib.mk
include $(ETC_PATH)\registers.mk

#Uncomment the following line to trap truly unknown SCSI commands
#BLD=$(BLD) -DSCSI_TRAP_UNKNOWN_OPCODES

#Uncomment the following line to keep tabs on the SCSI command history
#BLD=$(BLD) -DSCSI_COMMAND_HISTORY

#Uncomment the following line to keep tabs on the SCSI read history
#BLD=$(BLD) -DSCSI_DEBUG_READ_COMMAND_HISTORY

#Uncomment the following line to keep tabs on the SCSI write history
#BLD=$(BLD) -DSCSI_DEBUG_WRITE_COMMAND_HISTORY

#Uncomment the following line to keep tabs on the SCSI read time
#BLD=$(BLD) -DSCSI_DEBUG_READ_TIME_HISTORY

#Uncomment the following line to keep tabs on the SCSI write time
#BLD=$(BLD) -DSCSI_DEBUG_WRITE_TIME_HISTORY

SCSI_PATH   =   $(ROOT)\DeviceDriver\Media\scsi

INC_ALL=$(INC_ALL);$(SCSI_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(SCSI_PATH)

FILESYSLIB  =   $(FILESYSLIB) \
                $(SCSILIB)


LK_OPTS = $(LK_OPTS) -u FGetVendorSpecificSCSIHandler

ifndef D_PLIBS
SCSILIB     =   $(CHIP_LIB_PATH)\scsi.a
else                
SCSILIB     =   $(OBJ_PATH)\scsi.a

SCSILIST    =   $(OBJ_PATH)\scsi.obj\
                $(OBJ_PATH)\scsich.obj\
                $(OBJ_PATH)\sectorarrayhandler.obj\
                $(OBJ_PATH)\scsi_vendor_specific_default.obj

CC_OPTS = $(CC_OPTS) -Wc-DSCSI_INQUIRY_FORCE_REMOVABLE -Wc-DMULTI_LUN_BUILD
#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
SCSI_DEPENDENCIES= \
    "$(INC_PATH)\scsi.h" \
    "$(SCSI_PATH)\scsich.h" \
    $(USBMSC_DEPENDENCIES)
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above

"$(SCSILIB)" : $(SCSILIST)
    @echo Creating IEEE archive library file for SCSI Driver$@
        @$(AR_EXE) -r $(SCSILIB) $(SCSILIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\scsi.obj" : "$(SCSI_PATH)\scsi.c" $(SCSI_DEPENDENCIES)

"$(OBJ_PATH)\scsiisr.obj" : "$(SCSI_PATH)\scsiisr.c" $(SCSI_DEPENDENCIES)

"$(OBJ_PATH)\scsich.obj" : "$(SCSI_PATH)\scsich.c" $(SCSI_DEPENDENCIES)
"$(OBJ_PATH)\scsich.obj" : $(OUTPUT_INCLUDE_PATH)\regsswizzle.h
# the following is needed because of the persistent bits in 3500
"$(OBJ_PATH)\scsich.obj" : $(OUTPUT_INCLUDE_PATH)\regsrtc.h

$(OBJ_PATH)\sectorarrayhandler.obj : $(SCSI_PATH)\sectorarrayhandler.c

$(OBJ_PATH)\scsi_vendor_specific_default.obj : $(SCSI_PATH)\scsi_vendor_specific_default.c
endif

endif