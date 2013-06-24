#This is the makefile for the SDK playlist5
#

OBJLIST=	$(OBJLIST)\
        $(OBJ_PATH)\playlist5.obj\
        $(OBJ_PATH)\playlist5init.obj\
        $(OBJ_PATH)\playlist5_static.obj\
		$(OBJ_PATH)\playlist5_browse_common.obj\
		$(OBJ_PATH)\playlist5_browsemenu.obj\
		$(OBJ_PATH)\pl5_browsemenu_extra.obj\
		$(OBJ_PATH)\playlist5_browse_submenu.obj\
		$(OBJ_PATH)\playlist5_browse_module.obj\
		$(OBJ_PATH)\playlist5_playback_module.obj\
		$(OBJ_PATH)\playlist5_playback_common.obj




MODULELIST = $(MODULELIST)\
		$(OUTPUT_RSRC_PATH)\playlist_codebank.src\
		$(OUTPUT_RSRC_PATH)\playlist2init_codebank.src\
		$(OUTPUT_RSRC_PATH)\playlist2traverse_codebank.src\
		\
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
		$(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src\
		\
		$(OUTPUT_RSRC_PATH)\playlist5_browsemenu.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browsemenu_extra.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browse_submenu.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browse_module.src\
		$(OUTPUT_RSRC_PATH)\playlist5_playback_module.src


INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\playlist5

### These resources are made from null.src to make the resource linker happy
CODEBANKLIST = $(CODEBANKLIST)\
		$(OUTPUT_RSRC_PATH)\playmusicmenu.src\
		$(OUTPUT_RSRC_PATH)\newmusicmenu.src\
		$(OUTPUT_RSRC_PATH)\browsemenu.src\
		$(OUTPUT_RSRC_PATH)\browsemenu_extra.src\
		$(OUTPUT_RSRC_PATH)\sdmd.src\
        $(OUTPUT_RSRC_PATH)\musiclib_build_function_sec.src\
        $(OUTPUT_RSRC_PATH)\musiclib_build_function_flush.src

RESOURCELIST =	$(RESOURCELIST)\
		$(OUTPUT_RSRC_PATH)\string_play_all.src\
		$(OUTPUT_RSRC_PATH)\string_play.src\
		$(OUTPUT_RSRC_PATH)\string_unknown_year.src\
		$(OUTPUT_RSRC_PATH)\string_year_width.src\
		$(OUTPUT_RSRC_PATH)\string_artist.src\
		$(OUTPUT_RSRC_PATH)\string_songs.src\
		$(OUTPUT_RSRC_PATH)\string_on_the_fly.src\
		$(OUTPUT_RSRC_PATH)\string_new_music.src\
		$(OUTPUT_RSRC_PATH)\string_genre.src\
		$(OUTPUT_RSRC_PATH)\string_year.src\
		$(OUTPUT_RSRC_PATH)\string_playlist.src\
		$(OUTPUT_RSRC_PATH)\string_fm_rec.src\
		$(OUTPUT_RSRC_PATH)\string_linein_rec.src\
		$(OUTPUT_RSRC_PATH)\string_play_music.src\
		$(OUTPUT_RSRC_PATH)\string_1_day.src\
		$(OUTPUT_RSRC_PATH)\string_1_week.src\
		$(OUTPUT_RSRC_PATH)\string_1_month.src\
		$(OUTPUT_RSRC_PATH)\build_ml.src \
		$(OUTPUT_RSRC_PATH)\build_ml_warning.src \
		$(OUTPUT_RSRC_PATH)\build_ml_warning2.src \
		$(OUTPUT_RSRC_PATH)\build_flash1.src \
		$(OUTPUT_RSRC_PATH)\build_flash2.src \
		$(OUTPUT_RSRC_PATH)\build_flash3.src \
		$(OUTPUT_RSRC_PATH)\build_sd1.src \
		$(OUTPUT_RSRC_PATH)\build_sd2.src \
		$(OUTPUT_RSRC_PATH)\build_sd3.src \
		$(OUTPUT_RSRC_PATH)\sd_remove.src \
		$(OUTPUT_RSRC_PATH)\sd_insert.src \
		$(OUTPUT_RSRC_PATH)\build_newmusic.src \
		$(OUTPUT_RSRC_PATH)\highlight_back.src \
		$(OUTPUT_RSRC_PATH)\empty_favourite.src \
		$(OUTPUT_RSRC_PATH)\on_the_fly_full.src \
		$(OUTPUT_RSRC_PATH)\on_the_fly_free1.src \
		$(OUTPUT_RSRC_PATH)\on_the_fly_free2.src \
		$(OUTPUT_RSRC_PATH)\on_the_fly_delete1.src \
		$(OUTPUT_RSRC_PATH)\on_the_fly_delete2.src \
		$(OUTPUT_RSRC_PATH)\check_disk_1.src \
		$(OUTPUT_RSRC_PATH)\check_disk_2.src \
		$(OUTPUT_RSRC_PATH)\check_disk_3.src \
		$(OUTPUT_RSRC_PATH)\MusicLibBuildFlushMem.src \
		$(OUTPUT_RSRC_PATH)\flash_error.src


ifdef MAKE_FUNCLET_RSRC
FUNCLETSRCLIST =    $(FUNCLETSRCLIST)\
        $(OUTPUT_RSRC_PATH)\Funclet_write_detection_access.src\
        \
		$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetLvlOneItem_info.src\
		$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetFlashAssociatedList_info.src\
		$(OUTPUT_RSRC_PATH)\Funclet_ML_ResumePlayState.src\
		$(OUTPUT_RSRC_PATH)\Funclet_ML_UpdateOnTheGo.src\
		$(OUTPUT_RSRC_PATH)\Funclet_ML_ChOnTheGo.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_LoadOnTheGo.src\
        $(OUTPUT_RSRC_PATH)\Funclet_Build_InitandUseCache.src\
        $(OUTPUT_RSRC_PATH)\Funclet_Build_FlushAndCloseCache.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_movePointerBackward.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_findLastItemPointer.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_startShuffle.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_BrowseMenu.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_StartPlayAllSong.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayAllNextSong.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayAllPreviousSong.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_ShuffleNextSong.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayNextSong.src\
        \
        $(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListId.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListRootId.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListSize.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_GetIdByStateAndPosition.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_StartPlayDirSongInit.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_ShuffleNextDirSong.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousDirSong.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousSong.src\
        $(OUTPUT_RSRC_PATH)\Funclet_BuildVoiceFilePath.src\
        $(OUTPUT_RSRC_PATH)\Funclet_BuildFMFilePath.src\
        $(OUTPUT_RSRC_PATH)\Funclet_BuildLINEINFilePath.src\
        $(OUTPUT_RSRC_PATH)\Funclet_GetVoiceFastKey.src\
				$(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushBack.src\
				$(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushFront.src\

endif

$(OBJ_PATH)\playlist5.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.c
$(OBJ_PATH)\playlist5.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h
$(OBJ_PATH)\playlist5.obj : $(SYS_COMMON_PATH)\playlist5\playlist5internal.h

$(OBJ_PATH)\playlist5init.obj : $(SYS_COMMON_PATH)\playlist5\playlist5init.c
$(OBJ_PATH)\playlist5init.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h
$(OBJ_PATH)\playlist5init.obj : $(SYS_COMMON_PATH)\playlist5\playlist5internal.h

$(OBJ_PATH)\playlist5_static.obj : $(SYS_COMMON_PATH)\playlist5\playlist5_static.c
$(OBJ_PATH)\playlist5_static.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h
$(OBJ_PATH)\playlist5_static.obj : $(SYS_COMMON_PATH)\playlist5\playlist5internal.h

$(OBJ_PATH)\playlist5_browse_common.obj : $(SYS_COMMON_PATH)\playlist5\playlist5_browse_common.c
$(OBJ_PATH)\playlist5_browse_common.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h
$(OBJ_PATH)\playlist5_browse_common.obj : $(SYS_COMMON_PATH)\playlist5\playlist5internal.h

$(OBJ_PATH)\playlist5_browsemenu.obj : $(PROJMENUS_PATH)\playlist5_browsemenu.c
$(OBJ_PATH)\playlist5_browsemenu.obj : $(PROJMENUS_PATH)\playlist5_browsemenu.h
$(OBJ_PATH)\playlist5_browsemenu.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h

$(OBJ_PATH)\pl5_browsemenu_extra.obj : $(PROJMENUS_PATH)\pl5_browsemenu_extra.c
$(OBJ_PATH)\pl5_browsemenu_extra.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h

$(OBJ_PATH)\playlist5_browse_submenu.obj : $(PROJMENUS_PATH)\playlist5_browse_submenu.c
$(OBJ_PATH)\playlist5_browse_submenu.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h

$(OBJ_PATH)\playlist5_browse_module.obj : $(SYS_COMMON_PATH)\playlist5\playlist5_browse_module.c
$(OBJ_PATH)\playlist5_browse_module.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h

$(OBJ_PATH)\playlist5_playback_module.obj : $(SYS_COMMON_PATH)\playlist5\playlist5_playback_module.c
$(OBJ_PATH)\playlist5_playback_module.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h

$(OBJ_PATH)\playlist5_playback_common.obj : $(SYS_COMMON_PATH)\playlist5\playlist5_playback_common.c
$(OBJ_PATH)\playlist5_playback_common.obj : $(SYS_COMMON_PATH)\playlist5\playlist5.h


"$(OUTPUT_RSRC_PATH)\playlist2init_codebank.src"    : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist5init -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\playlist_codebank.src"         : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist5 -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\playlist5_browsemenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist5_browsemenu -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\playlist5_browsemenu_extra.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextpl5_browsemenu_extra -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\playlist5_browse_submenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist5_browse_submenu -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\playlist5_browse_module.src" : $(OUTPUT_PATH)\$(PROJ).abs
#        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextplaylist5_browse_module,.ptextplaylist5_browse_common,.ptextfindfirst,.ptextfindnext -f$(OUTPUT_PATH)\$(PROJ).map
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextplaylist5_browse_module,.ptextplaylist5_browse_common -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\playlist5_playback_module.src" : $(OUTPUT_PATH)\$(PROJ).abs
#        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextplaylist5_playback_module,.ptextfindfirst,.ptextfindnext -f$(OUTPUT_PATH)\$(PROJ).map
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextplaylist5_playback_module,.ptextplaylist5_playback_common -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushBack.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_PL5_PL_QueuePushBack.src 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_PL5_PL_QUEUEPUSHBACK_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushFront.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_PL5_PL_QueuePushFront.src 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_PL5_PL_QUEUEPUSHFRONT_P -f$(OUTPUT_PATH)\$(PROJ).map

### Resource Extraction Dependencies
$(OUTPUT_RSRC_PATH)\string_play_all.src			: $(PROJRSRC_PATH)\strings\string_play_all.txt
$(OUTPUT_RSRC_PATH)\string_play.src			: $(PROJRSRC_PATH)\strings\string_play.txt
$(OUTPUT_RSRC_PATH)\string_fm_rec.src		    : $(PROJRSRC_PATH)\strings\string_fm_rec.txt
$(OUTPUT_RSRC_PATH)\string_linein_rec.src	    : $(PROJRSRC_PATH)\strings\string_linein_rec.txt

### These resources are made from null.src to make the resource linker happy
# playlist2 sources
"$(OUTPUT_RSRC_PATH)\playlist2traverse_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to playlist2traverse_codebank.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist2traverse_codebank.src

# playlist3 sources
"$(OUTPUT_RSRC_PATH)\MusicLibBuildModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibBuildModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibBuildModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibBuildGenreTable.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibBuildGenreTable.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibBuildGenreTable.src
"$(OUTPUT_RSRC_PATH)\MusicLibBrowseApp.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibBrowseApp.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibBrowseApp.src
"$(OUTPUT_RSRC_PATH)\MusicLibBrowseModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibBrowseModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibBrowseModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibPlayModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibPlayModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibPlayModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibPlayAllNextModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibPlayAllNextModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibPlayAllNextModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibPlayAllPrevModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibPlayAllPrevModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibPlayAllPrevModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibShuffleModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibShuffleModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibShuffleModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibVoiceApp.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibVoiceApp.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibVoiceApp.src
"$(OUTPUT_RSRC_PATH)\MusicLibVoiceModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibVoiceModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibVoiceModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibVoicePlayModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibVoicePlayModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibVoicePlayModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibMergeFunction.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src
"$(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibMergeModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src

#$(OUTPUT_RSRC_PATH)\string_play_all.src		: $(PROJRSRC_PATH)\null.src
#		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_play_all.src
$(OUTPUT_RSRC_PATH)\string_unknown_year.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_unknown_year.src
$(OUTPUT_RSRC_PATH)\string_year_width.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_year_width.src
$(OUTPUT_RSRC_PATH)\string_artist.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_artist.src
$(OUTPUT_RSRC_PATH)\string_songs.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_songs.src
$(OUTPUT_RSRC_PATH)\string_on_the_fly.src	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_on_the_fly.src
$(OUTPUT_RSRC_PATH)\string_new_music.src	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_new_music.src
$(OUTPUT_RSRC_PATH)\string_genre.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_genre.src
$(OUTPUT_RSRC_PATH)\string_year.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_year.src
$(OUTPUT_RSRC_PATH)\string_playlist.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_playlist.src
$(OUTPUT_RSRC_PATH)\string_play_music.src	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_play_music.src
$(OUTPUT_RSRC_PATH)\string_1_day.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_1_day.src
$(OUTPUT_RSRC_PATH)\string_1_week.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_1_week.src
$(OUTPUT_RSRC_PATH)\string_1_month.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_1_month.src
$(OUTPUT_RSRC_PATH)\build_ml.src  		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_ml.src
$(OUTPUT_RSRC_PATH)\build_ml_warning.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_ml_warning.src
$(OUTPUT_RSRC_PATH)\build_ml_warning2.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_ml_warning2.src
$(OUTPUT_RSRC_PATH)\build_flash1.src  		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_flash1.src
$(OUTPUT_RSRC_PATH)\build_flash2.src  		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_flash2.src
$(OUTPUT_RSRC_PATH)\build_flash3.src  		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_flash3.src
$(OUTPUT_RSRC_PATH)\build_sd1.src  		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_sd1.src
$(OUTPUT_RSRC_PATH)\build_sd2.src  		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_sd2.src
$(OUTPUT_RSRC_PATH)\build_sd3.src  		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_sd3.src
$(OUTPUT_RSRC_PATH)\sd_remove.src  		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\sd_remove.src
$(OUTPUT_RSRC_PATH)\sd_insert.src  		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\sd_insert.src
$(OUTPUT_RSRC_PATH)\build_newmusic.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\build_newmusic.src
$(OUTPUT_RSRC_PATH)\highlight_back.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\highlight_back.src
$(OUTPUT_RSRC_PATH)\empty_favourite.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\empty_favourite.src
$(OUTPUT_RSRC_PATH)\on_the_fly_full.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\on_the_fly_full.src
$(OUTPUT_RSRC_PATH)\on_the_fly_free1.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\on_the_fly_free1.src
$(OUTPUT_RSRC_PATH)\on_the_fly_free2.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\on_the_fly_free2.src
$(OUTPUT_RSRC_PATH)\on_the_fly_delete1.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\on_the_fly_delete1.src
$(OUTPUT_RSRC_PATH)\on_the_fly_delete2.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\on_the_fly_delete2.src
$(OUTPUT_RSRC_PATH)\check_disk_1.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\check_disk_1.src
$(OUTPUT_RSRC_PATH)\check_disk_2.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\check_disk_2.src
$(OUTPUT_RSRC_PATH)\check_disk_3.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\check_disk_3.src
$(OUTPUT_RSRC_PATH)\flash_error.src  	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\flash_error.src

"$(OUTPUT_RSRC_PATH)\playmusicmenu.src"	: $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to playmusicmenu.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playmusicmenu.src

"$(OUTPUT_RSRC_PATH)\browsemenu.src"	: $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to browsemenu.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\browsemenu.src

"$(OUTPUT_RSRC_PATH)\browsemenu_extra.src"	: $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to browsemenu_extra.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\browsemenu_extra.src

"$(OUTPUT_RSRC_PATH)\newmusicmenu.src"	: $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to newmusicmenu.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\newmusicmenu.src

"$(OUTPUT_RSRC_PATH)\sdmd.src" 		: $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to sdmd.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\sdmd.src

"$(OUTPUT_RSRC_PATH)\musiclib_build_function_sec.src" 		: $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to sdmd.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\musiclib_build_function_sec.src

"$(OUTPUT_RSRC_PATH)\musiclib_build_function_flush.src" 		: $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to sdmd.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\musiclib_build_function_flush.src

"$(OUTPUT_RSRC_PATH)\Funclet_write_detection_access.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_write_detection_access.src

"$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetLvlOneItem_info.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetLvlOneItem_info.src

"$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetFlashAssociatedList_info.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetFlashAssociatedList_info.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ResumePlayState.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_ResumePlayState.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_UpdateOnTheGo.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_UpdateOnTheGo.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ChOnTheGo.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_ChOnTheGo.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_LoadOnTheGo.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_LoadOnTheGo.src

"$(OUTPUT_RSRC_PATH)\Funclet_Build_InitandUseCache.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_Build_InitandUseCache.src

"$(OUTPUT_RSRC_PATH)\Funclet_Build_FlushAndCloseCache.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_Build_FlushAndCloseCache.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_movePointerBackward.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_movePointerBackward.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_findLastItemPointer.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_findLastItemPointer.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_startShuffle.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_startShuffle.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_BrowseMenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_BrowseMenu.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_StartPlayAllSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_StartPlayAllSong.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayAllNextSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayAllNextSong.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayAllPreviousSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayAllPreviousSong.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ShuffleNextSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_ShuffleNextSong.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayNextSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayNextSong.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousSong.src

"$(OUTPUT_RSRC_PATH)\Funclet_BuildVoiceFilePath.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_BuildVoiceFilePath.src

"$(OUTPUT_RSRC_PATH)\Funclet_BuildFMFilePath.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_BuildFMFilePath.src

"$(OUTPUT_RSRC_PATH)\Funclet_BuildLINEINFilePath.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_BuildLINEINFilePath.src

"$(OUTPUT_RSRC_PATH)\Funclet_GetVoiceFastKey.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_GetVoiceFastKey.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListId.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListId.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListRootId.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListRootId.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListSize.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListSize.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_GetIdByStateAndPosition.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_GetIdByStateAndPosition.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_StartPlayDirSongInit.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_StartPlayDirSongInit.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ShuffleNextDirSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_ShuffleNextDirSong.src

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousDirSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousDirSong.src
	
"$(OUTPUT_RSRC_PATH)\MusicLibBuildFlushMem.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibBuildFlushMem.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibBuildFlushMem.src	

