#This is the makefile for the MediaTypeNAND DDI.
#

ifdef D_FUNCLETS
DDINANDCOMMONDDI_LIBNAME     =   dd_nand_ddi_common.a
else
DDINANDCOMMONDDI_LIBNAME     =   dd_nand_ddi_common.a
endif

MEDIATYPENAND_PATH       = $(ROOT)\devicedriver\media\MediaTypeNAND
DDINAND_PATH             = $(MEDIATYPENAND_PATH)\DDI
DDINANDCOMMONDDI_PATH    = $(DDINAND_PATH)\Common
HALNAND_PATH             = $(MEDIATYPENAND_PATH)\HAL

MEDIATYPENAND_INCLUDEPATH        = $(MEDIATYPENAND_PATH)\include
DDINAND_INCLUDEPATH              = $(DDINAND_PATH)\include
DDINANDCOMMONDDI_INCLUDEPATH     = $(DDINANDCOMMONDDI_PATH)\include
HALNAND_INCLUDEPATH              = $(HALNAND_PATH)\include

INC_ALL=$(INC_ALL);$(DDINAND_INCLUDEPATH);$(MEDIATYPENAND_INCLUDEPATH);$(DDINANDCOMMONDDI_INCLUDEPATH);$(HALNAND_INCLUDEPATH);$(DDINANDCOMMONDDI_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(MEDIATYPENAND_INCLUDEPATH) -Wc-I$(DDINANDCOMMONDDI_INCLUDEPATH) -Wc-I$(DDINAND_INCLUDEPATH) -Wc-I$(HALNAND_INCLUDEPATH) -Wc-I$(DDINANDCOMMONDDI_PATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDINANDCOMMONDDILIB)

ifdef D_PLIBS
DDINANDCOMMONDDILIB   =   $(OBJ_PATH)\$(DDINANDCOMMONDDI_LIBNAME)
else                
DDINANDCOMMONDDILIB   =   $(LIB_PATH)\$(DEVICE)\$(DDINANDCOMMONDDI_LIBNAME)
endif

DDINANDCOMMONDDILIST  = \
                $(OBJ_PATH)\NANDCalculateEccSync.obj \
                $(OBJ_PATH)\NANDWriteSync.obj \


#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDINANDCOMMONDDI_DEPENDENCIES= \
    "$(DDINAND_INCLUDEPATH)\NANDDdi.h"  \
    "$(OUTPUT_INCLUDE_PATH)\errors.h"
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

$(DDINANDCOMMONDDILIB) : $(DDINANDCOMMONDDILIST)
    @echo Creating IEEE archive library file for DriveTypeNAND Common DDI files $@
        @$(AR_EXE) -r $(DDINANDCOMMONDDILIB) $(DDINANDCOMMONDDILIST)

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\NANDCalculateEccSync.obj"          : "$(DDINANDCOMMONDDI_PATH)\NANDCalculateEccSync.c" $(DDINANDCOMMONDDI_DEPENDENCIES)
"$(OBJ_PATH)\NANDWriteSync.obj"                 : "$(DDINANDCOMMONDDI_PATH)\NANDWriteSync.c" $(DDINANDCOMMONDDI_DEPENDENCIES)


endif
