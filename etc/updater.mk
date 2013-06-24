ifndef UPDATER_MK
UPDATER_MK = TRUE
#This is the makefile for the USB MSC driver.
#
#include the usbmsc make file to make sure its already in the list.

include $(RELBASE_PATH)\etc\usbmsc.mk



UPDATER_PATH =   $(ROOT)\System\Common\updater

INC_ALL     =   $(INC_ALL);$(UPDATER_PATH)
INC_ALL_C   =   $(INC_ALL_C) -Wc-I$(UPDATER_PATH)

LK_OPTS     =   $(LK_OPTS) -u FGetCustomerExtentionSCSIHandler


ifdef D_LIBS
UPDATERLIB  =   $(OBJ_PATH)\updater.a
else
UPDATERLIB  =   $(CHIP_LIB_PATH)\updater.a
endif

GENERALLIB  =   $(UPDATERLIB) \
                $(GENERALLIB)

UPDATERLIST =   $(UPDATERLIST)\
                $(OBJ_PATH)\updater_scsi_commands.obj\
                $(OBJ_PATH)\getcustomerscsihandler.obj\

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_LIBS

$(UPDATERLIB) : $(UPDATERLIST)
    @echo Creating IEEE archive library file for USB Driver$@
        @$(AR_EXE) -r $@ $!

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\updater_scsi_commands.obj : $(UPDATER_PATH)\updater_scsi_commands.c

$(OBJ_PATH)\getcustomerscsihandler.obj : $(UPDATER_PATH)\getcustomerscsihandler.c
endif

endif