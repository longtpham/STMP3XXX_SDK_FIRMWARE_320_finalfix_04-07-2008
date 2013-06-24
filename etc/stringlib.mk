ifndef STRINGLIB_MK
STRINGLIB_MK = TRUE

LK_OPTS = $(LK_OPTS) -ustrrchr -ustrncmp 


ifdef D_LIBS
STRINGLIB=$(OBJ_PATH)\stringlib.a
else
STRINGLIB=$(LIB_PATH)\stringlib.a
endif
STRINGLIBLIST = \
            $(OBJ_PATH)\pack_string.obj\
            $(OBJ_PATH)\packed_get.obj\
            $(OBJ_PATH)\packed_set.obj\
            $(OBJ_PATH)\packed_strlen.obj\
            $(OBJ_PATH)\packed_strcpy.obj\
			$(OBJ_PATH)\packed_strncpy.obj\
            $(OBJ_PATH)\packed_strcmp.obj\
            $(OBJ_PATH)\packed_strncmp.obj\
            $(OBJ_PATH)\packed_strncmp.obj\
            $(OBJ_PATH)\packed_strncat.obj\
            $(OBJ_PATH)\packed_strcat.obj\
            $(OBJ_PATH)\strcat.obj\
            $(OBJ_PATH)\strcmp.obj\
            $(OBJ_PATH)\strcpy.obj\
            $(OBJ_PATH)\strlen.obj\
            $(OBJ_PATH)\strncat.obj\
            $(OBJ_PATH)\strncmp.obj\
            $(OBJ_PATH)\strncpy.obj\
            $(OBJ_PATH)\strchr.obj\
            $(OBJ_PATH)\strrchr.obj\
            $(OBJ_PATH)\unpack_string.obj\
            $(OBJ_PATH)\unpack_data.obj\
            $(OBJ_PATH)\getbytearrayx.obj\
            $(OBJ_PATH)\getbytearrayy.obj\
            $(OBJ_PATH)\putbytearrayx.obj\
            $(OBJ_PATH)\putbytearrayy.obj\

#we want this library first among all.
STANDARDLIB= $(STANDARDLIB) $(STRINGLIB) \
            

#///////////////////////////////////////////////////////////////////////////////
# Archive build : libraries (*.a) from object modules (*.obj)
#///////////////////////////////////////////////////////////////////////////////
ifdef D_LIBS
$(OBJ_PATH)\stringlib.a	: $(ETC_PATH)\stringlib.mk $(STRINGLIBLIST)
	@echo Creating IEEE archive library file for string library $@
		@$(AR_EXE) -r $@ $(match .obj $!)

$(OBJ_PATH)\pack_string.obj:            $(SYS_COMMON_PATH)\stringlib\pack_string.asm
$(OBJ_PATH)\packed_get.obj:             $(SYS_COMMON_PATH)\stringlib\packed_get.asm
$(OBJ_PATH)\packed_set.obj:             $(SYS_COMMON_PATH)\stringlib\packed_set.asm
$(OBJ_PATH)\packed_strlen.obj:          $(SYS_COMMON_PATH)\stringlib\packed_strlen.asm
$(OBJ_PATH)\packed_strcpy.obj:          $(SYS_COMMON_PATH)\stringlib\packed_strcpy.asm
$(OBJ_PATH)\packed_strncpy.obj:          $(SYS_COMMON_PATH)\stringlib\packed_strncpy.asm
$(OBJ_PATH)\packed_strcmp.obj:          $(SYS_COMMON_PATH)\stringlib\packed_strcmp.asm
$(OBJ_PATH)\packed_strncmp.obj:         $(SYS_COMMON_PATH)\stringlib\packed_strncmp.asm
$(OBJ_PATH)\packed_strncat.obj:         $(SYS_COMMON_PATH)\stringlib\packed_strncat.asm
$(OBJ_PATH)\packed_strcat.obj:          $(SYS_COMMON_PATH)\stringlib\packed_strcat.asm
$(OBJ_PATH)\strcat.obj:                 $(SYS_COMMON_PATH)\stringlib\strcat.asm
$(OBJ_PATH)\strcmp.obj:                 $(SYS_COMMON_PATH)\stringlib\strcmp.asm
$(OBJ_PATH)\strcpy.obj:                 $(SYS_COMMON_PATH)\stringlib\strcpy.asm
$(OBJ_PATH)\strlen.obj:                 $(SYS_COMMON_PATH)\stringlib\strlen.asm
$(OBJ_PATH)\strncat.obj:                $(SYS_COMMON_PATH)\stringlib\strncat.asm
$(OBJ_PATH)\strncmp.obj:                $(SYS_COMMON_PATH)\stringlib\strncmp.asm
$(OBJ_PATH)\strncpy.obj:                $(SYS_COMMON_PATH)\stringlib\strncpy.asm
$(OBJ_PATH)\strchr.obj:                 $(SYS_COMMON_PATH)\stringlib\strchr.asm
$(OBJ_PATH)\strrchr.obj:                $(SYS_COMMON_PATH)\stringlib\strrchr.asm
$(OBJ_PATH)\unpack_string.obj:          $(SYS_COMMON_PATH)\stringlib\unpack_string.asm $(SYS_COMMON_PATH)\stringlib\bytearray.asm
$(OBJ_PATH)\unpack_data.obj:            $(SYS_COMMON_PATH)\stringlib\unpack_data.asm $(SYS_COMMON_PATH)\stringlib\bytearray.asm
$(OBJ_PATH)\getbytearrayx.obj:          $(SYS_COMMON_PATH)\stringlib\getbytearrayX.asm $(SYS_COMMON_PATH)\stringlib\bytearray.asm
$(OBJ_PATH)\getbytearrayy.obj:          $(SYS_COMMON_PATH)\stringlib\getbytearrayy.asm $(SYS_COMMON_PATH)\stringlib\bytearray.asm
$(OBJ_PATH)\putbytearrayx.obj:          $(SYS_COMMON_PATH)\stringlib\putBytearrayx.asm $(SYS_COMMON_PATH)\stringlib\bytearray.asm
$(OBJ_PATH)\putbytearrayy.obj:          $(SYS_COMMON_PATH)\stringlib\putByteArrayY.asm $(SYS_COMMON_PATH)\stringlib\bytearray.asm

endif

endif