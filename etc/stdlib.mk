ifndef STDLIB_INCLUDED
define STDLIB_INCLUDED=1

STDLIBLIST = \
            $(OBJ_PATH)\srand.obj\
            $(OBJ_PATH)\rand.obj\
            $(OBJ_PATH)\gettime.obj\


STANDARDLIB = $(STANDARDLIB)\
          $(OBJ_PATH)\stdlib.a


#///////////////////////////////////////////////////////////////////////////////
# Archive build : libraries (*.a) from object modules (*.obj)
#///////////////////////////////////////////////////////////////////////////////
$(OBJ_PATH)\stdlib.a	: $(ETC_PATH)\stdlib.mk $(STDLIBLIST)
	@echo Creating IEEE archive library file for stdlib library $@
		@$(AR_EXE) -r $@ $(match .obj $!)

$(OBJ_PATH)\rand.obj:                   $(SYS_COMMON_PATH)\stdlib\rand.c

$(OBJ_PATH)\srand.obj:                   $(SYS_COMMON_PATH)\stdlib\srand.c

$(OBJ_PATH)\gettime.obj:                   $(SYS_COMMON_PATH)\stdlib\gettime.asm

endif