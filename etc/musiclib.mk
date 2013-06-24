#This is the makefile for the SDK music library browse engine
#

MUSICLIBNAME = musiclib.a

ifdef D_PLIBS
MUSICLIB = $(OBJ_PATH)\$(MUSICLIBNAME)
MUSICLIB_PATH = $(SW_MSGMODULES_PATH)\musiclib
else
MUSICLIB = $(LIB_PATH)\$(MUSICLIBNAME)
endif

ifndef MUSICLIB_INCPATH
MUSICLIB_INCPATH=$(MUSICLIB_PATH)\ghdr
CCFLAGS= $(CCFLAGS) -Wc-I$(MUSICLIB_INCPATH)
INC_ALL = $(INC_ALL);$(MUSICLIB_INCPATH)
endif

MODULELIST=	$(MODULELIST)\
		$(OUTPUT_RSRC_PATH)\MusicLibBrowseModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibPlayModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibPlayAllNextModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibPlayAllPrevModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibShuffleModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibVoiceModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibVoicePlayModule.src\
		$(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src\
        $(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src


CODEBANKLIST = $(CODEBANKLIST)\
		$(OUTPUT_RSRC_PATH)\playmusicmenu.src\
		$(OUTPUT_RSRC_PATH)\newmusicmenu.src\
		$(OUTPUT_RSRC_PATH)\browsemenu.src\
		$(OUTPUT_RSRC_PATH)\browsemenu_extra.src\
        $(OUTPUT_RSRC_PATH)\musiclib_build_function_sec.src\
        $(OUTPUT_RSRC_PATH)\musiclib_build_function_flush.src


ifdef MAKE_FUNCLET_RSRC
FUNCLETSRCLIST =    $(FUNCLETSRCLIST)\
		$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetLvlOneItem_info.src\
		$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetFlashAssociatedList_info.src\
		$(OUTPUT_RSRC_PATH)\Funclet_ML_UpdateOnTheGo.src\
		$(OUTPUT_RSRC_PATH)\Funclet_ML_ChOnTheGo.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_ResumePlayState.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_LoadOnTheGo.src\
        $(OUTPUT_RSRC_PATH)\Funclet_Build_InitandUseCache.src\
        $(OUTPUT_RSRC_PATH)\Funclet_Build_FlushAndCloseCache.src

endif

OBJLIST=	$(OBJLIST)\
		$(OBJ_PATH)\playmusicmenu.obj\
		$(OBJ_PATH)\newmusicmenu.obj\
		$(OBJ_PATH)\browsemenu.obj\
		$(OBJ_PATH)\browsemenu_extra.obj\

SYSTEMLIBS =	$(SYSTEMLIBS)\
		$(MUSICLIB)

ifdef D_PLIBS
MUSICLIBLIST= 	$(OBJ_PATH)\musiclib_browse_module.obj\
		      	$(OBJ_PATH)\musiclib_play_module.obj\
		      	$(OBJ_PATH)\musiclib_playallnext_module.obj\
		      	$(OBJ_PATH)\musiclib_playallprev_module.obj\
				$(OBJ_PATH)\musiclib_shuffle_module.obj\
				$(OBJ_PATH)\musiclib_voice_module.obj\
				$(OBJ_PATH)\musiclib_voiceplay_module.obj\
				$(OBJ_PATH)\musiclib_browse_funclet.obj\
                $(OBJ_PATH)\musiclib_basic_function_common.obj\
                $(OBJ_PATH)\musiclib_browse_app_basic.obj\
        		$(OBJ_PATH)\musiclib_voice_function_basic.obj\
        		$(OBJ_PATH)\musiclib_build_function_flash.obj\
        		$(OBJ_PATH)\musiclib_voice_function_flash.obj\
        		$(OBJ_PATH)\musiclib_merge_function.obj\
        		$(OBJ_PATH)\musiclib_merge_function_flash.obj\
        		$(OBJ_PATH)\musiclib_browse_app_flash.obj\
        		$(OBJ_PATH)\musiclib_merge_module.obj\
        		$(OBJ_PATH)\musiclib_merge_module_flash.obj\
        		$(OBJ_PATH)\musiclib_browse_module_browse.obj\
        		$(OBJ_PATH)\musiclib_play_module_browse.obj\
        		$(OBJ_PATH)\musiclib_playallnext_module_browse.obj\
        		$(OBJ_PATH)\musiclib_playallprev_module_browse.obj\
        		$(OBJ_PATH)\musiclib_shuffle_module_browse.obj\
        		$(OBJ_PATH)\musiclib_voice_module_browse.obj\
        		$(OBJ_PATH)\musiclib_voiceplay_module_browse.obj\
        		$(OBJ_PATH)\musiclib_build_function_sec.obj\
        		$(OBJ_PATH)\musiclib_build_function_flush.obj


OBJLIST=	$(OBJLIST)\
		$(MUSICLIBLIST)

#///////////////////////////////////////////////////////////////////////////////
# Musiclib Control
#///////////////////////////////////////////////////////////////////////////////
$(MUSICLIB) : $(MUSICLIBLIST)
    @echo Creating IEEE archive library file for MusicLib Module $@
        @$(AR_EXE) -r $(MUSICLIB) $(MUSICLIBLIST)

### Music Library compilation dependencies
"$(OBJ_PATH)\musiclib_browse_module.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_module.c
"$(OBJ_PATH)\musiclib_browse_module.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_browse_module.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_browse_module.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_browse_module.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h

"$(OBJ_PATH)\musiclib_play_module.obj" : $(MUSICLIB_PATH)\src\musiclib_play_module.c
"$(OBJ_PATH)\musiclib_play_module.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_play_module.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_play_module.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_play_module.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h

"$(OBJ_PATH)\musiclib_playallnext_module.obj" : $(MUSICLIB_PATH)\src\musiclib_playallnext_module.c
"$(OBJ_PATH)\musiclib_playallnext_module.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_playallnext_module.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_playallnext_module.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_playallnext_module.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h

"$(OBJ_PATH)\musiclib_playallprev_module.obj" : $(MUSICLIB_PATH)\src\musiclib_playallprev_module.c
"$(OBJ_PATH)\musiclib_playallprev_module.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_playallprev_module.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_playallprev_module.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_playallprev_module.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h

"$(OBJ_PATH)\musiclib_shuffle_module.obj" : $(MUSICLIB_PATH)\src\musiclib_shuffle_module.c
"$(OBJ_PATH)\musiclib_shuffle_module.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_shuffle_module.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_shuffle_module.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_shuffle_module.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h

"$(OBJ_PATH)\musiclib_voice_module.obj" : $(MUSICLIB_PATH)\src\musiclib_voice_module.c
"$(OBJ_PATH)\musiclib_voice_module.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_voice_module.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_voice_module.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_voice_module.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h

"$(OBJ_PATH)\musiclib_voiceplay_module.obj" : $(MUSICLIB_PATH)\src\musiclib_voiceplay_module.c
"$(OBJ_PATH)\musiclib_voiceplay_module.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_voiceplay_module.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_voiceplay_module.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_voiceplay_module.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h

"$(OBJ_PATH)\musiclib_merge_function.obj" : $(MUSICLIB_PATH)\src\musiclib_merge_function.c
"$(OBJ_PATH)\musiclib_merge_function.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_merge_function.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_merge_function.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_merge_function.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h

"$(OBJ_PATH)\musiclib_merge_module.obj" : $(MUSICLIB_PATH)\src\musiclib_merge_module.c
"$(OBJ_PATH)\musiclib_merge_module.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_merge_module.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_merge_module.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_merge_module.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h

"$(OBJ_PATH)\musiclib_browse_funclet.obj" : $(MLREBLD_PATH)\src\musiclib_browse_funclet.c
"$(OBJ_PATH)\musiclib_browse_funclet.obj" : $(MLREBLD_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_browse_funclet.obj" : $(MLREBLD_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_browse_funclet.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h

"$(OBJ_PATH)\musiclib_basic_function_common.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.c
"$(OBJ_PATH)\musiclib_basic_function_common.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_basic_function_common.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_browse_app_basic.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_app_basic.c
"$(OBJ_PATH)\musiclib_browse_app_basic.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_browse_app_basic.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_voice_function_basic.obj" : $(MUSICLIB_PATH)\src\musiclib_voice_function_basic.c
"$(OBJ_PATH)\musiclib_voice_function_basic.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_voice_function_basic.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_build_function_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_build_function_flash.c
"$(OBJ_PATH)\musiclib_build_function_flash.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_build_function_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_build_function_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_voice_function_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_voice_function_flash.c
"$(OBJ_PATH)\musiclib_voice_function_flash.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_voice_function_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_voice_function_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_merge_function_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_merge_function_flash.c
"$(OBJ_PATH)\musiclib_merge_function_flash.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_merge_function_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_merge_function_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_browse_app_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_app_flash.c
"$(OBJ_PATH)\musiclib_browse_app_flash.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_browse_app_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_browse_app_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_merge_module_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_merge_module_flash.c
"$(OBJ_PATH)\musiclib_merge_module_flash.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_merge_module_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_merge_module_flash.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_browse_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_module_browse.c
"$(OBJ_PATH)\musiclib_browse_module_browse.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_browse_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_browse_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_browse_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_play_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_play_module_browse.c
"$(OBJ_PATH)\musiclib_play_module_browse.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_play_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_play_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_play_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_playallnext_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_playallnext_module_browse.c
"$(OBJ_PATH)\musiclib_playallnext_module_browse.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_playallnext_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_playallnext_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_playallnext_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_playallprev_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_playallprev_module_browse.c
"$(OBJ_PATH)\musiclib_playallprev_module_browse.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_playallprev_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_playallprev_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_playallprev_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_shuffle_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_shuffle_module_browse.c
"$(OBJ_PATH)\musiclib_shuffle_module_browse.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_shuffle_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_shuffle_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_shuffle_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_voice_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_voice_module_browse.c
"$(OBJ_PATH)\musiclib_voice_module_browse.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_voice_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_voice_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_voice_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_voiceplay_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_voiceplay_module_browse.c
"$(OBJ_PATH)\musiclib_voiceplay_module_browse.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_voiceplay_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_voiceplay_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_voiceplay_module_browse.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_build_function_sec.obj" : $(MUSICLIB_PATH)\src\musiclib_build_function_sec.c
"$(OBJ_PATH)\musiclib_build_function_sec.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_build_function_sec.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_build_function_sec.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_build_function_sec.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h

"$(OBJ_PATH)\musiclib_build_function_flush.obj" : $(MUSICLIB_PATH)\src\musiclib_build_function_flush.c
"$(OBJ_PATH)\musiclib_build_function_flush.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_build_function_flush.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_build_function_flush.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_build_function_flush.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
endif

### Menu Codebank Compilation dependencies
$(OBJ_PATH)\playmusicmenu.obj :$(PROJMENUS_PATH)\playmusicmenu.c
$(OBJ_PATH)\playmusicmenu.obj :$(PROJMENUS_PATH)\playmusicmenu.h
$(OBJ_PATH)\playmusicmenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\playmusicmenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\playmusicmenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\playmusicmenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\playmusicmenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\playmusicmenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\playmusicmenu.obj :$(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\newmusicmenu.obj :$(PROJMENUS_PATH)\newmusicmenu.c
$(OBJ_PATH)\newmusicmenu.obj :$(PROJMENUS_PATH)\newmusicmenu.h
$(OBJ_PATH)\newmusicmenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\newmusicmenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\newmusicmenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\newmusicmenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\newmusicmenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\newmusicmenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\newmusicmenu.obj :$(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\browsemenu.obj :$(PROJMENUS_PATH)\browsemenu.c
$(OBJ_PATH)\browsemenu.obj :$(PROJMENUS_PATH)\browsemenu.h
$(OBJ_PATH)\browsemenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\browsemenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\browsemenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\browsemenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\browsemenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\browsemenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\browsemenu.obj :$(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\browsemenu_extra.obj :$(PROJMENUS_PATH)\browsemenu_extra.c
$(OBJ_PATH)\browsemenu_extra.obj :$(PROJMENUS_PATH)\browsemenu.h

### Music Library extractions
"$(OUTPUT_RSRC_PATH)\MusicLibBrowseModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibPlayModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibPlayAllNextModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibPlayAllPrevModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibShuffleModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibVoiceModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibVoicePlayModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\musiclib.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

#"$(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
#        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src
#"$(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src" : $(OUTPUT_PATH)\$(PROJ).abs
#        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src

"$(OUTPUT_RSRC_PATH)\playmusicmenu.src"	: $(OUTPUT_PATH)\$(PROJ).abs musiclibmenus.loc
"$(OUTPUT_RSRC_PATH)\browsemenu.src"	: $(OUTPUT_PATH)\$(PROJ).abs musiclibmenus.loc
"$(OUTPUT_RSRC_PATH)\browsemenu_extra.src"	: $(OUTPUT_PATH)\$(PROJ).abs musiclibmenus.loc
"$(OUTPUT_RSRC_PATH)\newmusicmenu.src"	: $(OUTPUT_PATH)\$(PROJ).abs musiclibmenus.loc
"$(OUTPUT_RSRC_PATH)\musiclib_build_function_sec.src" 		: $(OUTPUT_PATH)\$(PROJ).abs musiclibmenus.loc
"$(OUTPUT_RSRC_PATH)\musiclib_build_function_flush.src"     : $(OUTPUT_PATH)\$(PROJ).abs musiclibmenus.loc
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

ifdef MAKE_FUNCLET_RSRC
"$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetLvlOneItem_info.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetFlashAssociatedList_info.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_UpdateOnTheGo.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_UPDATE_ON_THE_GO_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ChOnTheGo.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_CH_ON_THE_GO_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_LoadOnTheGo.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_LOAD_ON_THE_GO_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_Build_InitandUseCache.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_BUILD_INIT_AND_USE_CACHE_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_Build_FlushAndCloseCache.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_BUILD_FLUSH_AND_CLOSE_CACHE_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ResumePlayState.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_RESUMEPLAYSTATE_P -F$(OUTPUT_PATH)\$(PROJ).map

endif