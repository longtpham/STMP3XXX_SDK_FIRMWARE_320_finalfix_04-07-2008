#iolib.mk


IOLIBLIST = \
            $(OBJ_PATH)\_find.obj\
            $(OBJ_PATH)\getdiskfreespace.obj

ifdef D_LIBS
ifdef D_FUNCLETS
IOLIB = $(OBJ_PATH)\iolib_funclets.a
else
IOLIB = $(OBJ_PATH)\iolib.a
endif
else
ifdef D_FUNCLETS
IOLIB = $(LIB_PATH)\iolib_funclets.a
else
IOLIB = $(LIB_PATH)\iolib.a
endif
endif

FILESYSLIB = $(FILESYSLIB)\
          $(IOLIB)



LK_OPTS = $(LK_OPTS) -u unpack_string -u strchr -upack_string

FUNCLETSRCLIST =    $(FUNCLETSRCLIST)\
		            $(OUTPUT_RSRC_PATH)\Funclet_findfirst.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_findnext.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_findclose.src\

OBJLIST  = $(OBJLIST) \
                $(OBJ_PATH)\iolib_resource.obj
#///////////////////////////////////////////////////////////////////////////////
# Archive build : libraries (*.a) from object modules (*.obj)
#///////////////////////////////////////////////////////////////////////////////
ifdef D_LIBS
$(IOLIB) : $(IOLIBLIST)
	@echo Creating IEEE archive library file for io library $@
		@$(AR_EXE) -r $@ $!


$(OBJ_PATH)\_find.obj:            $(SYS_COMMON_PATH)\io\_find.c

$(OBJ_PATH)\getdiskfreespace.obj: $(SYS_COMMON_PATH)\io\getdiskfreespace.asm

endif

$(OUTPUT_RSRC_PATH)\funclet_findfirst.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
$(OUTPUT_RSRC_PATH)\funclet_findnext.src:   $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
$(OUTPUT_RSRC_PATH)\funclet_findclose.src:  $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
        @echo Extract Funclets
        $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(ETC_PATH)\iolib.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\


$(OBJ_PATH)\iolib_resource.obj : $(SYS_COMMON_PATH)\symbols\iolib_resource.asm

