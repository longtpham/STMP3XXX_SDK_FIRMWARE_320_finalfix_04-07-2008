
ifdef D_LIBS
STACKCHECK_LIB = $(OBJ_PATH)\stackcheck.a
else
STACKCHECK_LIB = $(LIB_PATH)\stackcheck.a
endif


GENERALLIB = $(GENERALLIB)\
    $(STACKCHECK_LIB)
    
$(OBJ_PATH)\stackcheck.a  : $(OBJ_PATH)\stackcheck.obj
    @echo Creating IEEE archive library file for Stack Check $@
        @$(AR_EXE) -r $@ $(OBJ_PATH)\stackcheck.obj

$(OBJ_PATH)\stackcheck.obj	   :   $(SYS_PATH)\data_management\stack\stackcheck.asm

