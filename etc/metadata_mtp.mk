include $(ETC_PATH)\audible_mtp.mk

RSRCLIST = $(RSRCLIST)\
    $(OUTPUT_RSRC_PATH)\metadata_codebank.src\
    $(OUTPUT_RSRC_PATH)\mp3metadata_codebank.src\
    $(OUTPUT_RSRC_PATH)\wmametadata_codebank.src\
    $(OUTPUT_RSRC_PATH)\wavmetadata_codebank.src\
    $(OUTPUT_RSRC_PATH)\vbr_codebank.src


OBJLIST =$(OBJLIST)\
    $(OBJ_PATH)\mtp_mp3metadata.obj\
    $(OBJ_PATH)\mtp_wmametadata.obj\
    $(OBJ_PATH)\mtp_wavmetadata.obj\
    $(OBJ_PATH)\mtp_metadata.obj\
    $(OBJ_PATH)\id3v2.obj\
    $(OBJ_PATH)\mtp_vbr.obj
    
$(OUTPUT_RSRC_PATH)\metadata_codebank.src       : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\metadata_mtp.loc
$(OUTPUT_RSRC_PATH)\mp3metadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\metadata_mtp.loc
$(OUTPUT_RSRC_PATH)\wmametadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\metadata_mtp.loc
$(OUTPUT_RSRC_PATH)\wavmetadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\metadata_mtp.loc
$(OUTPUT_RSRC_PATH)\vbr_codebank.src            : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\metadata_mtp.loc
        @echo Extract MetaData codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

$(OBJ_PATH)\mtp_metadata.obj : $(SYS_COMMON_PATH)\metadata\mtp_metadata.c
$(OBJ_PATH)\mtp_metadata.obj : $(OUTPUT_INCLUDE_PATH)\sysspeed.h


$(OBJ_PATH)\mtp_mp3metadata.obj : $(SYS_COMMON_PATH)\metadata\mtp_mp3metadata.c

$(OBJ_PATH)\mtp_wmametadata.obj : $(SYS_COMMON_PATH)\metadata\mtp_wmametadata.c
$(OBJ_PATH)\mtp_wmametadata.obj : $(SYS_COMMON_PATH)\metadata\wmametadata.h

$(OBJ_PATH)\mtp_wavmetadata.obj : $(SYS_COMMON_PATH)\metadata\mtp_wavmetadata.c

$(OBJ_PATH)\id3v2.obj       : $(SYS_COMMON_PATH)\metadata\id3v2.c
$(OBJ_PATH)\id3v2.obj       : $(SYS_COMMON_PATH)\metadata\id3v2_internal.h
$(OBJ_PATH)\id3v2.obj       : $(SYS_COMMON_PATH)\metadata\id3v2.h

$(OBJ_PATH)\mtp_vbr.obj         : $(SYS_COMMON_PATH)\metadata\mtp_vbr.c