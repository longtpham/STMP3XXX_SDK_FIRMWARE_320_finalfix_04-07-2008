#instrument.mk

ifndef INSTRUMENT_MK
INSTRUMENT_MK= TRUE

ifdef D_INSTRUMENT
CC_OPTS = $(CC_OPTS) -Wc-DUSE_INSTRUMENTATION -Wa-DUSE_INSTRUMENTATION
AS_OPTS = $(AS_OPTS) -DUSE_INSTRUMENTATION

include $(ETC_PATH)\registers.mk

SYSTEMLIBS = $(SYSTEMLIBS)\
    $(OBJ_PATH)\instrument.a

INSTRUMENT_OBJ_LIST = $(INSTRUMENT_OBJ_LIST)\
    $(OBJ_PATH)\instrument.obj \
    $(OBJ_PATH)\instrumenthelper.obj 

$(OBJ_PATH)\instrument.a : $(INSTRUMENT_OBJ_LIST) 
    @echo Creating IEEE archive library file for instrumentation library $@
        @$(AR_EXE) -r $@ $!

$(OBJ_PATH)\instrument.obj : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
$(OBJ_PATH)\instrument.obj : $(SYS_COMMON_PATH)\instrumentation\instrument.c
$(OBJ_PATH)\instrument.obj : $(INC_PATH)\instrument_consts.h
$(OBJ_PATH)\instrument.obj : $(INC_PATH)\instrumentation.h

$(OBJ_PATH)\instrumenthelper.obj : $(SYS_COMMON_PATH)\instrumentation\instrumenthelper.asm

endif


endif