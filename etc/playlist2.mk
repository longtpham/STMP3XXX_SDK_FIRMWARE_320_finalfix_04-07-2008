#This is the makefile for the SDK example playlist
#


OBJLIST=    $(OBJLIST)\
            $(OBJ_PATH)\playlist2.obj\
            $(OBJ_PATH)\playlist2_static.obj\
            $(OBJ_PATH)\playlist2mem.obj\
            $(OBJ_PATH)\shuffletrackorder.obj\
			$(OBJ_PATH)\playlist2init.obj\
			$(OBJ_PATH)\playlist2traverse.obj\
			$(OBJ_PATH)\playlist2sortmem.obj\
            $(OBJ_PATH)\playlist2mem2.obj\




MODULELIST = $(MODULELIST)\
    $(OUTPUT_RSRC_PATH)\playlist_codebank.src\
    $(OUTPUT_RSRC_PATH)\playlist2init_codebank.src\
    $(OUTPUT_RSRC_PATH)\playlist2traverse_codebank.src\
	\
	$(OUTPUT_RSRC_PATH)\MusicLibBuildModule.src\
	$(OUTPUT_RSRC_PATH)\MusicLibBuildGenreTable.src\
	$(OUTPUT_RSRC_PATH)\MusicLibBuildFlushMem.src\
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

INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\playlist2

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
		$(OUTPUT_RSRC_PATH)\playlist5_browse_module.src\
		$(OUTPUT_RSRC_PATH)\playlist5_playback_module.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browse_submenu.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browsemenu.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browsemenu_extra.src\
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
        $(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src\
        $(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushBack.src\
        $(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushFront.src\
        $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousSong.src\
        $(OUTPUT_RSRC_PATH)\Funclet_BuildVoiceFilePath.src\
        $(OUTPUT_RSRC_PATH)\Funclet_BuildFMFilePath.src\
        $(OUTPUT_RSRC_PATH)\Funclet_BuildLINEINFilePath.src\
        $(OUTPUT_RSRC_PATH)\Funclet_GetVoiceFastKey.src\

endif

$(OBJ_PATH)\playlist2.obj :$(SYS_COMMON_PATH)\playlist2\playlist2.c
$(OBJ_PATH)\playlist2.obj :$(SYS_COMMON_PATH)\playlist2\playlist2internal.h
$(OBJ_PATH)\playlist2.obj :$(OUTPUT_INCLUDE_PATH)\sysspeed.h
$(OBJ_PATH)\playlist2.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\playlist2init.obj :$(SYS_COMMON_PATH)\playlist2\playlist2init.c
$(OBJ_PATH)\playlist2traverse.obj :$(SYS_COMMON_PATH)\playlist2\playlist2traverse.c
$(OBJ_PATH)\shuffletrackorder.obj : $(SYS_COMMON_PATH)\playlist2\shuffletrackorder.c
$(OBJ_PATH)\playlist2mem.obj :$(SYS_COMMON_PATH)\playlist2\playlist2mem.c $(SYS_COMMON_PATH)\playlist2\playlist2internal.h
$(OBJ_PATH)\playlist2sortmem.obj :$(SYS_COMMON_PATH)\playlist2\playlist2sortmem.c
$(OBJ_PATH)\playlist2mem2.obj :$(SYS_COMMON_PATH)\playlist2\playlist2mem2.c $(SYS_COMMON_PATH)\playlist2\playlist2internal.h

$(OBJ_PATH)\playlist2_static.obj :$(SYS_COMMON_PATH)\playlist2\playlist2_static.c


"$(OUTPUT_RSRC_PATH)\playlist2init_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract playlist_codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist2init,.ptextfindfirst,.ptextfindnext -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\playlist_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract playlist_codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist2,.ptextshuffletrackorder -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\playlist2traverse_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract playlist_codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist2traverse -f$(OUTPUT_PATH)\$(PROJ).map

### Resource Extraction Dependencies
### These resources are made from null.src to make the resource linker happy
"$(OUTPUT_RSRC_PATH)\MusicLibBuildModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibBuildModule.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibBuildModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibBuildGenreTable.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibBuildGenreTable.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibBuildGenreTable.src
"$(OUTPUT_RSRC_PATH)\MusicLibBuildFlushMem.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Copying null.src to MusicLibBuildFlushMem.src
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibBuildFlushMem.src
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

$(OUTPUT_RSRC_PATH)\string_play_all.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_play_all.src
$(OUTPUT_RSRC_PATH)\string_play.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_play.src
$(OUTPUT_RSRC_PATH)\playlist5_browsemenu_extra.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_browsemenu_extra.src		
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
$(OUTPUT_RSRC_PATH)\string_fm_rec.src		: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_fm_rec.src
$(OUTPUT_RSRC_PATH)\string_linein_rec.src	: $(PROJRSRC_PATH)\null.src
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_linein_rec.src
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

"$(OUTPUT_RSRC_PATH)\playlist5_browsemenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_browsemenu.src		
	
"$(OUTPUT_RSRC_PATH)\playlist5_browse_submenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_browse_submenu.src		
		
"$(OUTPUT_RSRC_PATH)\playlist5_playback_module.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_playback_module.src		
		
"$(OUTPUT_RSRC_PATH)\playlist5_browse_module.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_browse_module.src	
	
"$(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushBack.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushBack.src	
		
"$(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushFront.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushFront.src	
			
ifndef SYNC_LYRICS
"$(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src
endif     