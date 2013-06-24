######################################
#include the audiblemetadata makefile
include $(RELBASE_PATH)\etc\audiblemetadata.mk
######################################

MODULELIST = $(MODULELIST)\
    $(OUTPUT_RSRC_PATH)\metadata_codebank.src\
    $(OUTPUT_RSRC_PATH)\mp3metadata_codebank.src\
    $(OUTPUT_RSRC_PATH)\wmametadata_codebank.src\
    $(OUTPUT_RSRC_PATH)\wavmetadata_codebank.src\
    $(OUTPUT_RSRC_PATH)\smvmetadata_codebank.src\
    $(OUTPUT_RSRC_PATH)\vbr_codebank.src\
    $(OUTPUT_RSRC_PATH)\id3v2_codebank.src\
    $(OUTPUT_RSRC_PATH)\apicframe_codebank.src


OBJLIST =$(OBJLIST)\
    $(OBJ_PATH)\metadata.obj\
    $(OBJ_PATH)\mp3metadata.obj\
    $(OBJ_PATH)\wmametadata.obj\
    $(OBJ_PATH)\wavmetadata.obj\
    $(OBJ_PATH)\id3v2.obj\
    $(OBJ_PATH)\vbr.obj
    
ifdef JPEG_ALBUM_ART
OBJLIST =$(OBJLIST)\
	$(OBJ_PATH)\attachedpictureframe.obj

"$(OUTPUT_RSRC_PATH)\apicframe_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Attached picture frame codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextattachedpictureframe -F$(OUTPUT_PATH)\$(PROJ).map

$(OBJ_PATH)\attachedpictureframe.obj	: $(SYS_COMMON_PATH)\metadata\attachedpictureframe.c
$(OBJ_PATH)\attachedpictureframe.obj	: $(SYS_COMMON_PATH)\metadata\attachedpictureframe.h
else
$(OUTPUT_RSRC_PATH)\apicframe_codebank.src		: $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL apicframe_codebank.src 
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\apicframe_codebank.src
endif

ifdef SMV_ADPCM
OBJLIST =$(OBJLIST)\
    $(OBJ_PATH)\smvmetadata.obj

$(OUTPUT_RSRC_PATH)\smvmetadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract smvmetadata codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextsmvmetadata -F$(OUTPUT_PATH)\$(PROJ).map

$(OBJ_PATH)\smvmetadata.obj : $(SYS_COMMON_PATH)\metadata\smvmetadata.c
else
$(OUTPUT_RSRC_PATH)\smvmetadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL smvmetadata_codebank.src 
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\smvmetadata_codebank.src
endif

$(OUTPUT_RSRC_PATH)\metadata_codebank.src       : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\metadata.loc
$(OUTPUT_RSRC_PATH)\vbr_codebank.src            : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\metadata.loc
        @echo Extract MetaData codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

$(OUTPUT_RSRC_PATH)\id3v2_codebank.src       : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL id3v2_codebank.src 
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\id3v2_codebank.src


ifdef USE_PLAYLIST3

$(OUTPUT_RSRC_PATH)\mp3metadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract mp3metadata codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist3init_metadata,.ptextmp3metadata,.ptextid3v2 -F$(OUTPUT_PATH)\$(PROJ).map

$(OUTPUT_RSRC_PATH)\wmametadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract wavmetadata_codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist3init_metadata,.ptextwmametadata -F$(OUTPUT_PATH)\$(PROJ).map

$(OUTPUT_RSRC_PATH)\wavmetadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract wavmetadata_codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist3init_metadata,.ptextwavmetadata -F$(OUTPUT_PATH)\$(PROJ).map

else

$(OUTPUT_RSRC_PATH)\mp3metadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract mp3metadata codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextmp3metadata,.ptextid3v2 -F$(OUTPUT_PATH)\$(PROJ).map

$(OUTPUT_RSRC_PATH)\wmametadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract wavmetadata_codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextwmametadata -F$(OUTPUT_PATH)\$(PROJ).map

$(OUTPUT_RSRC_PATH)\wavmetadata_codebank.src    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract wavmetadata_codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextwavmetadata -F$(OUTPUT_PATH)\$(PROJ).map

endif

$(OBJ_PATH)\metadata.obj : $(SYS_COMMON_PATH)\metadata\metadata.c
$(OBJ_PATH)\metadata.obj : $(OUTPUT_INCLUDE_PATH)\sysspeed.h
$(OBJ_PATH)\metadata.obj : $(OUTPUT_INCLUDE_PATH)\sysresources.h
$(OBJ_PATH)\metadata.obj : $(OUTPUT_INCLUDE_PATH)\sysequ.h
ifdef USE_PLAYLIST3
$(OBJ_PATH)\metadata.obj : $(SW_MSGMODULES_PATH)\musiclib\ghdr\musiclib_ghdr.h
endif

$(OBJ_PATH)\mp3metadata.obj : $(SYS_COMMON_PATH)\metadata\mp3metadata.c

$(OBJ_PATH)\wmametadata.obj : $(SYS_COMMON_PATH)\metadata\wmametadata.c
$(OBJ_PATH)\wmametadata.obj : $(SYS_COMMON_PATH)\metadata\wmametadata.h

$(OBJ_PATH)\wavmetadata.obj : $(SYS_COMMON_PATH)\metadata\wavmetadata.c

$(OBJ_PATH)\smvmetadata.obj : $(SYS_COMMON_PATH)\metadata\smvmetadata.c

$(OBJ_PATH)\id3v2.obj       : $(SYS_COMMON_PATH)\metadata\id3v2.c
$(OBJ_PATH)\id3v2.obj       : $(SYS_COMMON_PATH)\metadata\id3v2_internal.h
$(OBJ_PATH)\id3v2.obj       : $(SYS_COMMON_PATH)\metadata\id3v2.h

$(OBJ_PATH)\vbr.obj         : $(SYS_COMMON_PATH)\metadata\vbr.c