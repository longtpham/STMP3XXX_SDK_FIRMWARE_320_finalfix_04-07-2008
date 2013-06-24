#This is the makefile for the SCMOS.
#

SCMOS_PATH          = $(ROOT)\system\scmos

INC_ALL=$(INC_ALL);$(SCMOS_PATH)
INC_ALL_C=$(INC_ALL_C) -Wc-I$(SCMOS_PATH)

OBJLIST     =   $(OBJLIST)\
                $(SCMOSLIST)

SCMOSLIST   =   $(OBJ_PATH)\scmosasm.obj\
                $(OBJ_PATH)\scmos.obj\
                $(OBJ_PATH)\scmosproject.obj
                
#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////
SCMOS_DEPENDENCIES= \
    "$(INC_PATH)\scmos.h" \
    "$(INC_PATH)\scmos.inc" \
    "$(PROJSYS_PATH)\scmosproject.c"
    
#///////////////////////////////////////////////////////////////////////////////
# Files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\scmosasm.obj" : $(SCMOS_PATH)\scmosasm.asm $(SCMOS_DEPENDENCIES)
"$(OBJ_PATH)\scmos.obj" : $(SCMOS_PATH)\scmos.c $(SCMOS_DEPENDENCIES)
"$(OBJ_PATH)\scmosproject.obj" : $(PROJSYS_PATH)\scmosproject.c $(SCMOS_DEPENDENCIES)


