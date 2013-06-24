ifndef DDILDL_MK
DDILDL_MK=TRUE
#This is the makefile for the DDI LDL API.
#

LIBNAME     =   ddildl.a

INC_ALL=$(INC_ALL);$(DDILDL_PATH);$(DDILDL_INCLUDE_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(DDILDL_PATH) -Wc-I$(DDILDL_INCLUDE_PATH)

DRIVERLIBS  =   $(DRIVERLIBS) \
                $(DDILDLLIB)
         
ifdef D_PLIBS
DDILDLLIB   =   $(OBJ_PATH)\$(LIBNAME)
else                
DDILDLLIB   =   $(LIB_PATH)\$(LIBNAME)
endif

DDILDLLIST  =   $(OBJ_PATH)\ddildl_asm.obj \
                $(OBJ_PATH)\ddildl_write_asm.obj \
                $(OBJ_PATH)\ddildl_utils_asm.obj\

ifdef PLAYER
DDILDL_RECOVER_LIST  = $(OBJ_PATH)\restoredrivefrombackup.obj\
				    $(OBJ_PATH)\partialrestoredrivefrombackup.obj
endif

ifdef USBMTP
DDILDL_RECOVER_LIST = $(OBJ_PATH)\restoredrivefrombackupMTP.obj
endif

OBJLIST = $(OBJLIST) $(OBJ_PATH)\ddiproject.obj

#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
DDILDL_DEPENDENCIES= \
    "$(INC_PATH)\ddildl.h" \
    "$(OUTPUT_INCLUDE_PATH)\errors.h"
    
# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifdef D_PLIBS

$(DDILDLLIB) : $(DDILDLLIST) $(DDILDL_RECOVER_LIST)
    @echo Creating IEEE archive library file for DDI LDL API $@
        @$(AR_EXE) -r $(DDILDLLIB) $(DDILDLLIST) $(DDILDL_RECOVER_LIST)



#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\ddildl_asm.obj"        : "$(DDILDL_PATH)\ddildl_asm.asm" $(DDILDL_DEPENDENCIES)
"$(OBJ_PATH)\ddildl_write_asm.obj"  : "$(DDILDL_PATH)\ddildl_write_asm.asm" $(DDILDL_DEPENDENCIES)
"$(OBJ_PATH)\ddildl_utils_asm.obj"  : "$(DDILDL_PATH)\ddildl_utils_asm.asm" $(DDILDL_DEPENDENCIES)




endif 

#///////////////////////////////////////////////////////////////////////////////
# Device driver files
#///////////////////////////////////////////////////////////////////////////////
# create regsrtc.h from regsrtc.inc for use by build target obj below

$(ROOT)\inc\regsrtc.h : $(ROOT)\inc\3500\regsrtc.inc $(ROOT)\inc\persist_bits.h
    @echo Converting Header File $!
    $(INCTOC_EXE) $(ROOT)\inc\3500\regsrtc.inc  $(ROOT)\inc\regsrtc.h


ifdef PLAYER
"$(OBJ_PATH)\restoredrivefrombackup.obj" : $(INC_PATH)\persist_bits.h
"$(OBJ_PATH)\restoredrivefrombackup.obj" : $(ROOT)\filesystem\fat32\utility\restoredrivefrombackup.c "$(ROOT)\inc\regsrtc.h" "$(INC_PATH)\persist_bits.h" $(DDILDL_DEPENDENCIES) 
"$(OBJ_PATH)\restoredrivefrombackup.obj" : $(INC_PATH)\restoredrivefrombackup.h

"$(OBJ_PATH)\partialrestoredrivefrombackup.obj" : $(INC_PATH)\persist_bits.h
"$(OBJ_PATH)\partialrestoredrivefrombackup.obj" : $(ROOT)\filesystem\fat32\utility\partialrestoredrivefrombackup.c "$(ROOT)\inc\regsrtc.h" "$(INC_PATH)\persist_bits.h" $(DDILDL_DEPENDENCIES) 
"$(OBJ_PATH)\partialrestoredrivefrombackup.obj" : $(INC_PATH)\restoredrivefrombackup.h
endif
ifdef USBMTP

"$(OBJ_PATH)\restoredrivefrombackupMTP.obj" : $(INC_PATH)\persist_bits.h
"$(OBJ_PATH)\restoredrivefrombackupMTP.obj" : $(ROOT)\filesystem\fat32\utility\restoredrivefrombackupMTP.c "$(ROOT)\inc\regsrtc.h" "$(INC_PATH)\persist_bits.h" $(DDILDL_DEPENDENCIES) 
"$(OBJ_PATH)\restoredrivefrombackupMTP.obj" : $(INC_PATH)\restoredrivefrombackupMTP.h
endif

$(OBJ_PATH)\ddiproject.obj  : $(PROJ_PATH)\ddiproject.c  $(OUTPUT_INCLUDE_PATH)\errors.h

endif