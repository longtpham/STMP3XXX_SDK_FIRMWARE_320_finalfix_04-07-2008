#This is the makefile for the Compact Flash Port HIL API.

ifdef D_FUNCLETS
LIBNAME_GPPORTHIL            =   porthil.a
else
LIBNAME_GPPORTHIL            =   porthil.a
endif

GPPORTHILLIB_PATH  =   $(LIB_PATH)\$(DEVICE)

ifdef D_PLIBS
GPPORTHILLIB       =   $(OBJ_PATH)\$(LIBNAME_GPPORTHIL)
else
GPPORTHILLIB       =   $(GPPORTHILLIB_PATH)\$(LIBNAME_GPPORTHIL)
endif


DRIVERLIBS     =   $(DRIVERLIBS) \
                $(GPPORTHILLIB) \

GPPORTHIL_PATH    = $(ROOT)\DeviceDriver\Media\PortHIL\GPFlash
GPPORTHIL_INCPATH = $(ROOT)\DeviceDriver\Media\PortHIL\GPFlash\include

CFGPCOMMON_INCPATH = $(ROOT)\DeviceDriver\Media\include
PORTHILCOMMON_PATH = $(ROOT)\DeviceDriver\Media\PortHIL\common
CFGPHALINC_PATH      = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL\include

INC_ALL=$(INC_ALL);$(CFGPCOMMON_INCPATH);$(CFGPHALINC_PATH);$(GPPORTHIL_PATH);$(GPPORTHIL_INCPATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(CFGPCOMMON_INCPATH) -Wc-I$(GPPORTHIL_INCPATH) -Wc-I$(GPPORTHIL_PATH)

GPPORTHILLIST = 	$(OBJ_PATH)\CFGPPortHilCommonPads_asm.obj \
				  	$(OBJ_PATH)\GPPortHilCommands_asm.obj

GPPORTHILWRITES = 	$(OBJ_PATH)\GPPortHilWriteCommands_asm.obj

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
GPPORTHIL_DEPENDENCIES= \
    "$(CFGPCOMMON_INCPATH)\NANDPortHil.h" \

# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

"$(GPPORTHILLIB)" : $(GPPORTHILLIST) $(GPPORTHILWRITES)
    @echo Creating IEEE archive library file for Port HIL API $@
        @$(AR_EXE) -r $(GPPORTHILLIB) $(GPPORTHILLIST) $(GPPORTHILWRITES)


#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\CFGPPortHilCommonPads_asm.obj"   : "$(PORTHILCOMMON_PATH)\CFGPPortHilCommonPads_asm.asm" $(GPPORTHIL_DEPENDENCIES)
"$(OBJ_PATH)\GPPortHilCommands_asm.obj"		  : "$(GPPORTHIL_PATH)\GPPortHilCommands_asm.asm"  $(GPPORTHIL_DEPENDENCIES)
"$(OBJ_PATH)\GPPortHilWriteCommands_asm.obj"  : "$(GPPORTHIL_PATH)\GPPortHilWriteCommands_asm.asm"  $(GPPORTHIL_DEPENDENCIES)

endif