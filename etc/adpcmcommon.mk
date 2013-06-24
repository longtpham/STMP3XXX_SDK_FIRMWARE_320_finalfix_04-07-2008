#This is the common makefile for the ADPCM Encoder and Decoder
#
#This file should be included into the project makefile and 


#LIB_OPT_LIST =  $(LIB_OPT_LIST)\
#                $(ADPCMLIB_OPTS)


#LIB_LIST = $(LIB_LIST)\
#           $(ADPCMLIB)


#ADPCMLIB			= $(LIB_PATH)\adpcmlib.a

OBJLIST=$(OBJLIST)\
        $(ADPCMLIST)

ADPCMLIST = \
            $(OBJ_PATH)\msadpcm_mem.obj\
            $(OBJ_PATH)\imaadpcm_mem.obj\
            $(OBJ_PATH)\wave.obj\
            $(OBJ_PATH)\bitstreamwave.obj    
ifdef SMV_ADPCM    
ADPCMLIST = \
			$(ADPCMLIST)\
            $(OBJ_PATH)\smvadpcm_mem.obj\
            $(OBJ_PATH)\smvwave.obj\
            $(OBJ_PATH)\smvbitstreamwave.obj
endif


# The SDK projects do not include sources for the libs, so the ifndef
# below allows us to exclude the dependancies below, but use the make
# variables above
ifndef NO_LIB_DEPENDANCIES
#///////////////////////////////////////////////////////////////////////////////
# Archive build : libraries (*.a) from object modules (*.obj)
#///////////////////////////////////////////////////////////////////////////////
"$(LIB_PATH)\adpcmlib.a"			: $(ADPCMLIST)
	@echo Creating IEEE archive library file for ADPCM $@
		@$(AR_EXE) -r $(ADPCMLIB) $(ADPCMLIST)

endif

#///////////////////////////////////////////////////////////////////////////////
# ADPCM Common
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\msadpcm_mem.obj         : $(ALGO_COMMON_PATH)\msadpcm_mem.asm
$(OBJ_PATH)\smvadpcm_mem.obj         : $(ALGO_COMMON_PATH)\smvadpcm_mem.asm
$(OBJ_PATH)\imaadpcm_mem.obj        : $(ALGO_COMMON_PATH)\imaadpcm_mem.asm
$(OBJ_PATH)\wave.obj                : $(ALGO_COMMON_PATH)\wave.c
$(OBJ_PATH)\bitstreamwave.obj       : $(ALGO_COMMON_PATH)\bitstreamwave.asm 
$(OBJ_PATH)\smvbitstreamwave.obj       : $(ALGO_COMMON_PATH)\smvbitstreamwave.asm 
$(OBJ_PATH)\smvwave.obj                : $(ALGO_COMMON_PATH)\smvwave.c



