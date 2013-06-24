#This is the makefile for the usb driver.
#

SYSSERIALNUMBER_PATH    = $(ROOT)\libsource\sysserialnumber

INC_ALL=$(INC_ALL);$(SYSSERIALNUMBER_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(SYSSERIALNUMBER_PATH)

STANDARDLIB  =   $(STANDARDLIB) \
                $(SYSSERIALNUMBERLIB)

ifdef D_PLIBS
ifdef D_FUNCLETS
SYSSERIALNUMBERLIB      =    $(OBJ_PATH)\sysserialnumber_funclets.a
else
SYSSERIALNUMBERLIB      =    $(OBJ_PATH)\sysserialnumber.a
endif
else
ifdef D_FUNCLETS
SYSSERIALNUMBERLIB      =    $(CHIP_LIB_PATH)\sysserialnumber_funclets.a
else
SYSSERIALNUMBERLIB      =    $(CHIP_LIB_PATH)\sysserialnumber.a
endif
endif

SYSSERIALNUMBERLIST     =   $(OBJ_PATH)\ChipSerialNumberInit.obj\
    $(OBJ_PATH)\IntMediaSerialNumInit.obj\
    $(OBJ_PATH)\ChipGetSerialNumber.obj\
    $(OBJ_PATH)\CreateNullSerialNumber.obj\   
    $(OBJ_PATH)\ConvertHexToASCII.obj
    
OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\serialnumber_resource.obj

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
USB_DEPENDENCIES= \
    "$(INC_PATH)\sysserialnumber.h" 
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

$(SYSSERIALNUMBERLIB) : $(SYSSERIALNUMBERLIST)
    @echo Creating IEEE archive library file for USB Driver$@
        @$(AR_EXE) -r $(SYSSERIALNUMBERLIB) $(SYSSERIALNUMBERLIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\ChipSerialNumberInit.obj" : $(SYSSERIALNUMBER_PATH)\ChipSerialNumberInit.c $(USB_DEPENDENCIES)
"$(OBJ_PATH)\IntMediaSerialNumInit.obj" : $(SYSSERIALNUMBER_PATH)\IntMediaSerialNumInit.c $(USB_DEPENDENCIES)
"$(OBJ_PATH)\ChipGetSerialNumber.obj" : $(SYSSERIALNUMBER_PATH)\$(DEVICE)\ChipGetSerialNumber.c $(USB_DEPENDENCIES)
"$(OBJ_PATH)\CreateNullSerialNumber.obj" : $(SYSSERIALNUMBER_PATH)\CreateNullSerialNumber.c $(USB_DEPENDENCIES)
"$(OBJ_PATH)\ConvertHexToASCII.obj" : $(SYSSERIALNUMBER_PATH)\ConvertHexToASCII.c $(USB_DEPENDENCIES)
endif

"$(OBJ_PATH)\serialnumber_resource.obj" : $(SYS_COMMON_PATH)\symbols\serialnumber_resource.asm $(USB_DEPENDENCIES)