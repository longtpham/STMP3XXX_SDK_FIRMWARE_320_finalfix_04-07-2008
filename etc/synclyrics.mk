LYRICS_PATH = $(SYS_COMMON_PATH)\lyrics
ifndef D_PLIBS
LYRICS_LIB_PATH = $(LYRICS_PATH)\lib
else
LYRICS_LIB_PATH = $(OUTPUT_PATH)
endif
LYRICS_SOURCE_PATH = $(ROOT)\lyrics

MODULELIST = $(MODULELIST)\
    $(OUTPUT_RSRC_PATH)\id3v2lyrics_codebank.src\
    $(OUTPUT_RSRC_PATH)\lyrics3_codebank.src\
    $(OUTPUT_RSRC_PATH)\lrc_codebank.src\
    $(OUTPUT_RSRC_PATH)\lyrics_api_codebank.src\
    $(OUTPUT_RSRC_PATH)\wmalyrics_codebank.src

ifdef USE_PLAYLIST5    
FUNCLETSRCLIST=$(FUNCLETSRCLIST)\
            $(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src
endif            
            
ifdef SYNC_LYRICS

ALLLIBLIST = $(ALLLIBLIST)\
			$(SYNCLYRICSLIB)

ifdef SONG_ENCRYPT
SYNCLYRICSLIB = $(LYRICS_LIB_PATH)\synclyricslib_v1-01e.a
else
SYNCLYRICSLIB = $(LYRICS_LIB_PATH)\synclyricslib.a
endif

SYNCLYRICSLIST = $(SYNC_LYRICS_LIB)

OBJLIST =$(OBJLIST)\
    $(OBJ_PATH)\sync_lyrics.obj

ifdef D_PLIBS
SYNC_LYRICS_LIB = $(OBJ_PATH)\id3v2_lyrics_core.obj\
					$(OBJ_PATH)\lyrics_static.obj\
					$(OBJ_PATH)\lrc_core.obj\
					$(OBJ_PATH)\lyrics3_core.obj\
					$(OBJ_PATH)\lyrics_api.obj\
					$(OBJ_PATH)\wma_lyrics_core.obj

"$(SYNCLYRICSLIB)" : $(HWEQUCOMMON) $(HWEQULIST) $(SYNCLYRICSLIST) $(ETC_PATH)\synclyrics.mk
    @echo Creating IEEE archive library file for Sync Lyrics $@
        @$(AR_EXE) -r $@ $(match .obj $!) $(SYNC_LYRICS_LIB)

$(OBJ_PATH)\id3v2_lyrics_core.obj : $(LYRICS_SOURCE_PATH)\id3v2_lyrics_core.c
$(OBJ_PATH)\id3v2_lyrics_core.obj : $(LYRICS_SOURCE_PATH)\lyrics_internal.h
$(OBJ_PATH)\id3v2_lyrics_core.obj : $(INC_PATH)\synclyrics.h
$(OBJ_PATH)\id3v2_lyrics_core.obj : $(OUTPUT_INCLUDE_PATH)\messages.h

$(OBJ_PATH)\lyrics_static.obj : $(LYRICS_SOURCE_PATH)\lyrics_static.c
$(OBJ_PATH)\lyrics_static.obj : $(LYRICS_SOURCE_PATH)\lyrics_internal.h
$(OBJ_PATH)\lyrics_static.obj : $(INC_PATH)\synclyrics.h

$(OBJ_PATH)\lyrics3_core.obj : $(LYRICS_SOURCE_PATH)\lyrics3_core.c
$(OBJ_PATH)\lyrics3_core.obj : $(LYRICS_SOURCE_PATH)\lyrics_internal.h
$(OBJ_PATH)\lyrics3_core.obj : $(INC_PATH)\synclyrics.h

$(OBJ_PATH)\lrc_core.obj : $(LYRICS_SOURCE_PATH)\lrc_core.c
$(OBJ_PATH)\lrc_core.obj : $(LYRICS_SOURCE_PATH)\lyrics_internal.h
$(OBJ_PATH)\lrc_core.obj : $(INC_PATH)\synclyrics.h

$(OBJ_PATH)\lyrics_api.obj : $(LYRICS_SOURCE_PATH)\lyrics_api.c
$(OBJ_PATH)\lyrics_api.obj : $(LYRICS_SOURCE_PATH)\lyrics_internal.h
$(OBJ_PATH)\lyrics_api.obj : $(INC_PATH)\synclyrics.h

$(OBJ_PATH)\wma_lyrics_core.obj : $(LYRICS_SOURCE_PATH)\wma_lyrics_core.c
$(OBJ_PATH)\wma_lyrics_core.obj : $(LYRICS_SOURCE_PATH)\lyrics_internal.h
$(OBJ_PATH)\wma_lyrics_core.obj : $(INC_PATH)\synclyrics.h

endif

$(OUTPUT_RSRC_PATH)\id3v2lyrics_codebank.src       : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\synclyrics.loc
        @echo Extract ID3V2 Sync Lyrics codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

$(OUTPUT_RSRC_PATH)\lyrics3_codebank.src       : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\synclyrics.loc
        @echo Extract Lyrics3 Sync Lyrics codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

$(OUTPUT_RSRC_PATH)\lrc_codebank.src       : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\synclyrics.loc
        @echo Extract LRC Sync Lyrics codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

$(OUTPUT_RSRC_PATH)\lyrics_api_codebank.src       : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\synclyrics.loc
        @echo Extract Sync Lyrics APIs codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

$(OUTPUT_RSRC_PATH)\wmalyrics_codebank.src       : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\synclyrics.loc
        @echo Extract WMA Sync Lyrics codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

$(OBJ_PATH)\sync_lyrics.obj : $(LYRICS_PATH)\sync_lyrics.c
$(OBJ_PATH)\sync_lyrics.obj : $(INC_PATH)\synclyrics.h
$(OBJ_PATH)\sync_lyrics.obj : $(OUTPUT_INCLUDE_PATH)\resource.h

ifdef USE_PLAYLIST5 
ifdef SYNC_LYRICS
"$(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_Get_LRCDirName_PL5.src 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_GET_LRC_DIR_NAME_PL5_P -f$(OUTPUT_PATH)\$(PROJ).map
else
"$(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src 
endif	        
else
"$(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src        
endif

else
#### create dummy resources so there are no changes to resource.inc
"$(OUTPUT_RSRC_PATH)\id3v2lyrics_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\id3v2lyrics_codebank.src

"$(OUTPUT_RSRC_PATH)\lyrics3_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\lyrics3_codebank.src

"$(OUTPUT_RSRC_PATH)\lrc_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\lrc_codebank.src

"$(OUTPUT_RSRC_PATH)\lyrics_api_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\lyrics_api_codebank.src

"$(OUTPUT_RSRC_PATH)\wmalyrics_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wmalyrics_codebank.src

ifdef USE_PLAYLIST5 
"$(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src
endif    
    
endif

