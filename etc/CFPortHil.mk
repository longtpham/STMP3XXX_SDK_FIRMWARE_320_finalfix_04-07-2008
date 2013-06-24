#This is the makefile for the Compact Flash Port HIL API.
LIBNAME_CFPORTHIL            =   porthil.a

# MSC and Player have different library sets
ifdef STMP_BUILD_PLAYER 
CFPORTHILLIB_PATH  =   $(LIB_PATH)\$(DEVICE)
else
CFPORTHILLIB_PATH  =   $(LIB_PATH)\$(DEVICE)
endif

ifdef D_PLIBS
CFPORTHILLIB       =   $(OBJ_PATH)\$(LIBNAME_CFPORTHIL)
else
CFPORTHILLIB       =   $(CFPORTHILLIB_PATH)\$(LIBNAME_CFPORTHIL)
endif

DRIVERLIBS     =   $(DRIVERLIBS) \
                $(CFPORTHILLIB) \

CFPORTHIL_PATH     = $(ROOT)\DeviceDriver\Media\PortHIL\CompactFlash
CFPORTHIL_INCPATH  = $(ROOT)\DeviceDriver\Media\PortHIL\CompactFlash\include
CFGPCOMMON_INCPATH = $(ROOT)\DeviceDriver\Media\include
PORTHILCOMMON_PATH = $(ROOT)\DeviceDriver\Media\PortHIL\common
CFHALINC_PATH      = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL\include

INC_ALL=$(INC_ALL);$(CFPORTHIL_PATH);$(CFPORTHIL_INCPATH);$(CFGPCOMMON_INCPATH);$(CFHALINC_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(CFPORTHIL_PATH) -Wc-I$(CFPORTHIL_INCPATH) -Wc-I$(CFGPCOMMON_INCPATH)

CFPORTHILLIST = 	$(OBJ_PATH)\CFGPPortHilCommonPads_asm.obj \
				  	$(OBJ_PATH)\CFPortHilCommands_asm.obj

CFPORTHILWRITES = 	$(OBJ_PATH)\CFPortHilWriteCommands_asm.obj

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
CFPORTHIL_DEPENDENCIES= \
    "$(CFGPCOMMON_INCPATH)\NANDPortHil.h" \

# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

"$(CFPORTHILLIB)" : $(CFPORTHILLIST) $(CFPORTHILWRITES)
    @echo Creating IEEE archive library file for Port HIL API $@
        @$(AR_EXE) -r $(CFPORTHILLIB) $(CFPORTHILLIST) $(CFPORTHILWRITES)


#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\CFGPPortHilCommonPads_asm.obj"   : "$(PORTHILCOMMON_PATH)\CFGPPortHilCommonPads_asm.asm" $(CFPORTHIL_DEPENDENCIES)
"$(OBJ_PATH)\CFPortHilCommands_asm.obj"		  : "$(CFPORTHIL_PATH)\CFPortHilCommands_asm.asm"  $(CFPORTHIL_DEPENDENCIES)
"$(OBJ_PATH)\CFPortHilWriteCommands_asm.obj"  : "$(CFPORTHIL_PATH)\CFPortHilWriteCommands_asm.asm"  $(CFPORTHIL_DEPENDENCIES)

endif
