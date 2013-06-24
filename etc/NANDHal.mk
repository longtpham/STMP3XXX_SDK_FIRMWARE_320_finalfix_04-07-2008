#This is the makefile for the NAND HAL API.

ifdef D_FUNCLETS
LIBNAME_NANDHAL          =   NANDHal.a
else
LIBNAME_NANDHAL          =   NANDHal.a
endif

NANDHAL_ECCUTIL_PATH = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL\ecc

ifdef D_D3410
NANDHAL_ECCPATH = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL\common\D3410
else
ifdef D_D3500
NANDHAL_ECCPATH = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL\common\D3500
else
NANDHAL_ECCPATH = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL\common
endif
endif

NANDHAL_LOCKPATH = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL\common\sdk2400

ifdef D_PLIBS
NANDHALLIB       =   $(OBJ_PATH)\$(LIBNAME_NANDHAL)
else
NANDHALLIB       =   $(LIB_PATH)\$(DEVICE)\$(LIBNAME_NANDHAL)
endif

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(NANDHALLIB) \

DDILDL_PATH				 = $(ROOT)\devicedriver\media\ddildl
DDILDL_INCLUDE_PATH      = $(DDILDL_PATH)\include
MEDIATYPENAND_PATH       = $(ROOT)\devicedriver\media\MediaTypeNAND
DDINAND_PATH             = $(MEDIATYPENAND_PATH)\DDI
DDINANDCOMMONDDI_PATH    = $(DDINAND_PATH)\Common
DDINAND_INCLUDEPATH              = $(DDINAND_PATH)\include
DDINANDCOMMONDDI_INCLUDEPATH     = $(DDINANDCOMMONDDI_PATH)\include
NANDHAL_PATH = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL
NANDHAL_INCPATH = $(ROOT)\DeviceDriver\Media\MediaTypeNAND\HAL\include

INC_ALL=$(INC_ALL);$(DDILDL_PATH);$(DDINAND_INCLUDEPATH);$(NANDHAL_PATH);$(NANDHAL_INCPATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(DDILDL_PATH) -Wc-I$(DDILDL_INCLUDE_PATH) -Wc-I$(DDINANDCOMMONDDI_INCLUDEPATH) -Wc-I$(DDINAND_INCLUDEPATH) -Wc-I$(NANDHAL_PATH) -Wc-I$(NANDHAL_INCPATH)

NANDHALLIST = 	  $(OBJ_PATH)\NANDHalReadFunctions.obj \
				  $(OBJ_PATH)\NANDHalType1Descriptors.obj \
				  $(OBJ_PATH)\NANDHalType2Descriptors.obj \
				  $(OBJ_PATH)\NANDHalType5Descriptors.obj \
				  $(OBJ_PATH)\NANDHalInitFunctions.obj

NANDHALWRITES =	  $(OBJ_PATH)\NANDHalType1WriteFunctions.obj \
				  $(OBJ_PATH)\NANDHalType2WriteFunctions.obj \
				  $(OBJ_PATH)\NANDHalType5Functions.obj \
				  $(OBJ_PATH)\NANDHalCommonWriteFunctions.obj \
				  $(OBJ_PATH)\NANDHalMarkBlockBadInRA.obj \
				  $(OBJ_PATH)\NANDRA_utils.obj \
				  $(OBJ_PATH)\NANDRA_utils_write.obj

ifdef D_D3410
NANDHALCOMMON =   $(OBJ_PATH)\NANDHalCommonFunctions.obj\
				  $(OBJ_PATH)\NANDHalSetNandBusy.obj \
				  $(OBJ_PATH)\NANDHalLockNand.obj \
				  $(OBJ_PATH)\NANDHalLockInterfacePort.obj \
				  $(OBJ_PATH)\NANDHalStartComputeECC.obj \
				  $(OBJ_PATH)\NANDHalCorrectECC.obj \
				  $(OBJ_PATH)\NANDHalComputeECCGetStatus.obj \
				  $(OBJ_PATH)\eccutils.obj \
				  $(OBJ_PATH)\ecctable.obj \
				  $(OBJ_PATH)\pageutils.obj \
				  $(OBJ_PATH)\streamutils.obj
else
ifdef D_D3500
NANDHALCOMMON =   $(OBJ_PATH)\NANDHalCommonFunctions.obj\
				  $(OBJ_PATH)\NANDHalSetNandBusy.obj \
				  $(OBJ_PATH)\NANDHalLockNand.obj \
				  $(OBJ_PATH)\NANDHalLockInterfacePort.obj \
				  $(OBJ_PATH)\NANDHalStartComputeECC.obj \
				  $(OBJ_PATH)\NANDHalCorrectECC.obj \
				  $(OBJ_PATH)\NANDHalComputeECCGetStatus.obj \
				  $(OBJ_PATH)\streamutils.obj
else
NANDHALCOMMON =   $(OBJ_PATH)\NANDHalCommonFunctions.obj\
				  $(OBJ_PATH)\NANDHalSetNandBusy.obj \
				  $(OBJ_PATH)\NANDHalLockNand.obj \
				  $(OBJ_PATH)\NANDHalLockInterfacePort.obj \
				  $(OBJ_PATH)\NANDHalStartComputeECC.obj \
				  $(OBJ_PATH)\NANDHalCorrectECC.obj \
				  $(OBJ_PATH)\NANDHalComputeECCGetStatus.obj \
				  $(OBJ_PATH)\eccutils.obj \
				  $(OBJ_PATH)\ecctable.obj \
				  $(OBJ_PATH)\pageutils.obj \
				  $(OBJ_PATH)\streamutils.obj
endif
endif


NANDHALCOMMON = $(NANDHALCOMMON) $(OBJ_PATH)\nandhalproject.obj



#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
NANDHAL_DEPENDENCIES= \
    "$(NANDHAL_INCPATH)\NANDHal.h" \
    "$(NANDHAL_PATH)\common\nandtables.h" \
    "$(OUTPUT_INCLUDE_PATH)\errors.h"

# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

"$(NANDHALLIB)" : $(NANDHALLIST) $(NANDHALWRITES) $(NANDHALCOMMON)
    @echo Creating IEEE archive library file for NAND HAL API $@
        @$(AR_EXE) -r $(NANDHALLIB) $(NANDHALLIST) $(NANDHALWRITES) $(NANDHALCOMMON)

#///////////////////////////////////////////////////////////////////////////////
# Convert Inc files to C header files
#///////////////////////////////////////////////////////////////////////////////

ifndef __CONVERT_REGSGPFLASH
__CONVERT_REGSGPFLASH=TRUE
$(OUTPUT_INCLUDE_PATH)\regsgpflash.h :$(HW_REGS_INC_PATH)\regsgpflash.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $(HW_REGS_INC_PATH)\regsgpflash.inc $(OUTPUT_INCLUDE_PATH)\regsgpflash.h
endif

ifndef __CONVERT_REGSGPIO
__CONVERT_REGSGPIO=TRUE
$(OUTPUT_INCLUDE_PATH)\regsgpio.h :$(HW_REGS_INC_PATH)\regsgpio.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $(HW_REGS_INC_PATH)\regsgpio.inc $(OUTPUT_INCLUDE_PATH)\regsgpio.h
endif

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\NANDHalReadFunctions.obj"        : "$(NANDHAL_PATH)\common\NANDHalReadFunctions.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalInitFunctions.obj"        : "$(NANDHAL_PATH)\common\NANDHalInitFunctions.c" $(NANDHAL_DEPENDENCIES) $(OUTPUT_INCLUDE_PATH)\regsgpio.h
"$(OBJ_PATH)\NANDHalInitFunctions.obj"        : "$(OUTPUT_INCLUDE_PATH)\regsgpflash.h"
"$(OBJ_PATH)\NANDHalType1Descriptors.obj"     : "$(NANDHAL_PATH)\Type1\NANDHalType1Descriptors.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalType2Descriptors.obj"     : "$(NANDHAL_PATH)\Type2\NANDHalType2Descriptors.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalType5Descriptors.obj"     : "$(NANDHAL_PATH)\Type5\NANDHalType5Descriptors.c" $(NANDHAL_DEPENDENCIES)

"$(OBJ_PATH)\NANDHalType1WriteFunctions.obj"  : "$(NANDHAL_PATH)\Type1\NANDHalType1WriteFunctions.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalType2WriteFunctions.obj"  : "$(NANDHAL_PATH)\Type2\NANDHalType2WriteFunctions.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalType5Functions.obj"       : "$(NANDHAL_PATH)\Type5\NANDHalType5Functions.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalCommonWriteFunctions.obj" : "$(NANDHAL_PATH)\common\NANDHalCommonWriteFunctions.c" $(NANDHAL_DEPENDENCIES)

"$(OBJ_PATH)\NANDHalCommonFunctions.obj"      : "$(NANDHAL_PATH)\common\NANDHalCommonFunctions.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalSetNandBusy.obj"          : "$(NANDHAL_PATH)\common\NANDHalSetNandBusy.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalLockNand.obj"             : "$(NANDHAL_LOCKPATH)\NANDHalLockNand.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalLockInterfacePort.obj"    : "$(NANDHAL_LOCKPATH)\NANDHalLockInterfacePort.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalStartComputeECC.obj"      : "$(NANDHAL_ECCPATH)\NANDHalStartComputeECC.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalCorrectECC.obj"           : "$(NANDHAL_ECCPATH)\NANDHalCorrectECC.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalComputeECCGetStatus.obj"  : "$(NANDHAL_ECCPATH)\NANDHalComputeECCGetStatus.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\eccutils.obj"                    : "$(NANDHAL_ECCUTIL_PATH)\eccutils.asm" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\ecctable.obj"                    : "$(NANDHAL_ECCUTIL_PATH)\ecctable.asm" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\pageutils.obj"                   : "$(NANDHAL_ECCUTIL_PATH)\pageutils.asm" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\streamutils.obj"                 : "$(NANDHAL_ECCUTIL_PATH)\streamutils.asm" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDHalMarkBlockBadInRA.obj"     : "$(NANDHAL_PATH)\common\NANDHalMarkBlockBadInRA.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDRA_utils.obj"                : "$(NANDHAL_PATH)\common\NANDRA_utils.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\NANDRA_utils_write.obj"          : "$(NANDHAL_PATH)\common\NANDRA_utils_write.c" $(NANDHAL_DEPENDENCIES)
"$(OBJ_PATH)\nandhalproject.obj"              : "$(NANDHAL_PATH)\common\nandhalproject.c"

"$(NANDHAL_ECCPATH)\NANDHalComputeECCGetStatus.c" : "$(OUTPUT_INCLUDE_PATH)\regsecc.h"
"$(NANDHAL_ECCPATH)\NANDHalCorrectECC.c"          : "$(OUTPUT_INCLUDE_PATH)\regsecc.h"
"$(NANDHAL_ECCPATH)\NANDHalStartComputeECC.c"     : "$(OUTPUT_INCLUDE_PATH)\regsecc.h"

"$(OUTPUT_INCLUDE_PATH)\regsgpflash.h" : "$(HW_REGS_INC_PATH)\regsgpflash.inc"

endif
