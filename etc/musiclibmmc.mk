#This is the makefile for the SDK playlist3 and music library rebuild functions
#
# Note:
# We should use "playlist3init_codeback.src", but we use
# "playlist2init_codebank.src" to make the resource linker happy
# We are generating NULL "playlist2traverse_codebank.src" to make the resource linker happy

MUSICLIBMMCNAME = musiclibmmc.a

ifdef D_PLIBS
MLMMCOBJ_PATH = $(ROOT)\libsource\feature_libs\output_3500\obj
MUSICLIBMMC = $(MLMMCOBJ_PATH)\$(MUSICLIBMMCNAME)
MUSICLIBMMC_PATH = $(SW_MSGMODULES_PATH)\musiclibmmc
else
MUSICLIBMMC = $(LIB_PATH)\$(MUSICLIBMMCNAME)
endif

INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\playlist3

SYSTEMLIBS =	$(SYSTEMLIBS)\
                $(MUSICLIBMMC)

ifndef D_PLIBS

MODULELIST=	$(MODULELIST)\
		$(OUTPUT_RSRC_PATH)\MusicLibBuildModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibBuildGenreTable.src\
		$(OUTPUT_RSRC_PATH)\MusicLibBrowseApp.src\
		$(OUTPUT_RSRC_PATH)\MusicLibBrowseModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibPlayModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibPlayAllNextModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibPlayAllPrevModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibShuffleModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibVoiceApp.src\
		$(OUTPUT_RSRC_PATH)\MusicLibVoiceModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibVoicePlayModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src\
		$(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src

ifdef MAKE_FUNCLET_RSRC
FUNCLETSRCLIST =    $(FUNCLETSRCLIST)\
        $(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetLvlOneItem_info.src\
        $(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetFlashAssociatedList_info.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_ResumePlayState.src
endif

endif

ifdef D_PLIBS
MUSICLIBMMCLIST= 	$(MLMMCOBJ_PATH)\mlmmc_build_function.obj\
              	    $(MLMMCOBJ_PATH)\mlmmc_build_genre_table.obj\
              	    $(MLMMCOBJ_PATH)\mlmmc_browse_app.obj\
	      	        $(MLMMCOBJ_PATH)\mlmmc_browse_module.obj\
	      	        $(MLMMCOBJ_PATH)\mlmmc_play_module.obj\
	      	        $(MLMMCOBJ_PATH)\mlmmc_playallnext_module.obj\
	      	        $(MLMMCOBJ_PATH)\mlmmc_playallprev_module.obj\
		            $(MLMMCOBJ_PATH)\mlmmc_shuffle_module.obj\
		            $(MLMMCOBJ_PATH)\mlmmc_voice_function.obj\
		            $(MLMMCOBJ_PATH)\mlmmc_voice_module.obj\
		            $(MLMMCOBJ_PATH)\mlmmc_voiceplay_module.obj\
		            $(MLMMCOBJ_PATH)\mlmmc_merge_function.obj\
				    $(MLMMCOBJ_PATH)\mlmmc_merge_module.obj\
				    $(MLMMCOBJ_PATH)\mlmmc_browse_funclet.obj



OBJLIST=	$(OBJLIST)\
		    $(MUSICLIBMMCLIST)

#///////////////////////////////////////////////////////////////////////////////
# Musiclib Control
#///////////////////////////////////////////////////////////////////////////////
$(MUSICLIBMMC) : $(MUSICLIBMMCLIST)
    @echo Creating IEEE archive library file for MusicLibMMC (Rebuild) Module $@
        @$(AR_EXE) -r $(MUSICLIBMMC) $(MUSICLIBMMCLIST)

### Music Library compilation dependencies
"$(MLMMCOBJ_PATH)\mlmmc_build_function.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_build_function.c
"$(MLMMCOBJ_PATH)\mlmmc_build_function.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_basic_function.c
"$(MLMMCOBJ_PATH)\mlmmc_build_function.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_flash_function.c
"$(MLMMCOBJ_PATH)\mlmmc_build_function.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_build_function.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h
"$(MLMMCOBJ_PATH)\mlmmc_build_function.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h

"$(MLMMCOBJ_PATH)\mlmmc_build_genre_table.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_build_genre_table.c
"$(MLMMCOBJ_PATH)\mlmmc_build_genre_table.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h

"$(MLMMCOBJ_PATH)\mlmmc_browse_app.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_browse_app.c
"$(MLMMCOBJ_PATH)\mlmmc_browse_app.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_flash_function.c
"$(MLMMCOBJ_PATH)\mlmmc_browse_app.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_browse_app.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_browse_module.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_browse_module.c
"$(MLMMCOBJ_PATH)\mlmmc_browse_module.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_browse_function.c
"$(MLMMCOBJ_PATH)\mlmmc_browse_module.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_browse_module.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_play_module.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_play_module.c
"$(MLMMCOBJ_PATH)\mlmmc_play_module.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_browse_function.c
"$(MLMMCOBJ_PATH)\mlmmc_play_module.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_play_module.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_playallnext_module.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_playallnext_module.c
"$(MLMMCOBJ_PATH)\mlmmc_playallnext_module.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_browse_function.c
"$(MLMMCOBJ_PATH)\mlmmc_playallnext_module.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_playallnext_module.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_playallprev_module.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_playallprev_module.c
"$(MLMMCOBJ_PATH)\mlmmc_playallprev_module.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_browse_function.c
"$(MLMMCOBJ_PATH)\mlmmc_playallprev_module.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_playallprev_module.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_shuffle_module.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_shuffle_module.c
"$(MLMMCOBJ_PATH)\mlmmc_shuffle_module.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_browse_function.c
"$(MLMMCOBJ_PATH)\mlmmc_shuffle_module.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_shuffle_module.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_voice_function.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_voice_function.c
"$(MLMMCOBJ_PATH)\mlmmc_voice_function.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_basic_function.c
"$(MLMMCOBJ_PATH)\mlmmc_voice_function.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_flash_function.c
"$(MLMMCOBJ_PATH)\mlmmc_voice_function.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_flash_function.h
"$(MLMMCOBJ_PATH)\mlmmc_voice_function.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_voice_function.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_voice_module.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_voice_module.c
"$(MLMMCOBJ_PATH)\mlmmc_voice_module.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_browse_function.c
"$(MLMMCOBJ_PATH)\mlmmc_voice_module.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_voice_module.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_voiceplay_module.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_voiceplay_module.c
"$(MLMMCOBJ_PATH)\mlmmc_voiceplay_module.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_browse_function.c
"$(MLMMCOBJ_PATH)\mlmmc_voiceplay_module.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_voiceplay_module.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_merge_function.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_merge_function.c
"$(MLMMCOBJ_PATH)\mlmmc_merge_function.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_basic_function.c
"$(MLMMCOBJ_PATH)\mlmmc_merge_function.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_flash_function.c
"$(MLMMCOBJ_PATH)\mlmmc_merge_function.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_merge_function.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_merge_module.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_merge_module.c
"$(MLMMCOBJ_PATH)\mlmmc_merge_module.obj" : $(MUSICLIBMMC_PATH)\src\musiclib_flash_function.c
"$(MLMMCOBJ_PATH)\mlmmc_merge_module.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_hdr.h
"$(MLMMCOBJ_PATH)\mlmmc_merge_module.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h

"$(MLMMCOBJ_PATH)\mlmmc_browse_funclet.obj" : $(MUSICLIBMMC_PATH)\src\mlmmc_browse_funclet.c
"$(MLMMCOBJ_PATH)\mlmmc_browse_funclet.obj" : $(MUSICLIBMMC_PATH)\ghdr\musiclib_ghdr.h
"$(MLMMCOBJ_PATH)\mlmmc_browse_funclet.obj" : $(MUSICLIBMMC_PATH)\hdr\musiclib_flash_function.h

else

### Music Library extractions
"$(OUTPUT_RSRC_PATH)\MusicLibBuildModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibBuildGenreTable.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibBrowseApp.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibBrowseModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibPlayModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibPlayAllNextModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibPlayAllPrevModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibShuffleModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibVoiceApp.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibVoiceModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibVoicePlayModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\musiclibmmc.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

ifdef MAKE_FUNCLET_RSRC
"$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetLvlOneItem_info.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetFlashAssociatedList_info.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ResumePlayState.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_RESUMEPLAYSTATE_P -F$(OUTPUT_PATH)\$(PROJ).map
endif

endif
