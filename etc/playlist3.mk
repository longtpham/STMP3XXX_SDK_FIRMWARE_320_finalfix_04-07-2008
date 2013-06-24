#This is the makefile for the SDK playlist3 and music library
#
ifdef D_PL3_FB
CC_OPTS= $(CC_OPTS) -Wc-DPL3_FB
MUSICLIBNAME = musiclib_fb.a
else
MUSICLIBNAME = musiclib.a
endif

ifdef D_PLIBS
MUSICLIB = $(OBJ_PATH)\$(MUSICLIBNAME)
MUSICLIB_PATH = $(SW_MSGMODULES_PATH)\musiclib
else
MUSICLIB = $(LIB_PATH)\$(MUSICLIBNAME)
endif

MUSICLIB_INCPATH=$(SW_MSGMODULES_PATH)\musiclib\ghdr

# Note:
# We should use "playlist3init_codeback.src", but we use
# "playlist2init_codebank.src" to make the resource linker happy
# We are generating NULL "playlist2traverse_codebank.src" to make the resource linker happy

MODULELIST=	$(MODULELIST)\
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
		\
        $(OUTPUT_RSRC_PATH)\playlist5_browsemenu.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browse_submenu.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browse_module.src\
		$(OUTPUT_RSRC_PATH)\playlist5_playback_module.src


CCFLAGS= $(CCFLAGS) -Wc-I$(MUSICLIB_INCPATH)
INC_ALL = $(INC_ALL);$(MUSICLIB_INCPATH)
INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\playlist3

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
		$(OUTPUT_RSRC_PATH)\playlist5_browsemenu.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browse_submenu.src\
		$(OUTPUT_RSRC_PATH)\playlist5_browsemenu_extra.src\
		$(OUTPUT_RSRC_PATH)\string_unknown_year.src\
		$(OUTPUT_RSRC_PATH)\string_year_width.src\
		$(OUTPUT_RSRC_PATH)\string_artist.src\
		$(OUTPUT_RSRC_PATH)\string_album.src\
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
        $(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousSong.src\
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
        $(OUTPUT_RSRC_PATH)\Funclet_BuildVoiceFilePath.src\
        $(OUTPUT_RSRC_PATH)\Funclet_BuildFMFilePath.src\
        $(OUTPUT_RSRC_PATH)\Funclet_BuildLINEINFilePath.src\
        $(OUTPUT_RSRC_PATH)\Funclet_GetVoiceFastKey.src

endif


OBJLIST=	$(OBJLIST)\
		$(OBJ_PATH)\playlist3.obj\
		$(OBJ_PATH)\playlist3_static.obj\
		$(OBJ_PATH)\playlist3mem.obj\
		$(OBJ_PATH)\playlist3init.obj\
		$(OBJ_PATH)\playlist3init_static.obj\
		$(OBJ_PATH)\playlist3init_metadata.obj\
		\
		$(OBJ_PATH)\playmusicmenu.obj\
		$(OBJ_PATH)\newmusicmenu.obj\
		$(OBJ_PATH)\browsemenu.obj\
		$(OBJ_PATH)\browsemenu_extra.obj\
		$(OBJ_PATH)\sdmd.obj

SYSTEMLIBS =	$(SYSTEMLIBS)\
		$(MUSICLIB)

ifdef D_PLIBS
MUSICLIBLIST= 	$(OBJ_PATH)\musiclib_build_function.obj\
              	$(OBJ_PATH)\musiclib_build_genre_table.obj\
              	$(OBJ_PATH)\musiclib_browse_app.obj\
    	      	$(OBJ_PATH)\musiclib_browse_module.obj\
    	      	$(OBJ_PATH)\musiclib_play_module.obj\
    	      	$(OBJ_PATH)\musiclib_playallnext_module.obj\
    	      	$(OBJ_PATH)\musiclib_playallprev_module.obj\
        		$(OBJ_PATH)\musiclib_shuffle_module.obj\
        		$(OBJ_PATH)\musiclib_voice_function.obj\
        		$(OBJ_PATH)\musiclib_voice_module.obj\
        		$(OBJ_PATH)\musiclib_voiceplay_module.obj\
        		$(OBJ_PATH)\musiclib_merge_function.obj\
				$(OBJ_PATH)\musiclib_merge_module.obj\
                $(OBJ_PATH)\musiclib_basic_function_common.obj\
                $(OBJ_PATH)\musiclib_browse_app_basic.obj\
        		$(OBJ_PATH)\musiclib_voice_function_basic.obj\
        		$(OBJ_PATH)\musiclib_build_function_flash.obj\
        		$(OBJ_PATH)\musiclib_voice_function_flash.obj\
        		$(OBJ_PATH)\musiclib_merge_function_flash.obj\
        		$(OBJ_PATH)\musiclib_browse_app_flash.obj\
        		$(OBJ_PATH)\musiclib_merge_module_flash.obj\
        		$(OBJ_PATH)\musiclib_browse_extra.obj\
        		$(OBJ_PATH)\musiclib_build_function_sec.obj\
        		$(OBJ_PATH)\musiclib_build_function_flush.obj\
        		$(OBJ_PATH)\musiclib_browse_function_common.obj\
        		$(OBJ_PATH)\musiclib_browse_play.obj\
        		$(OBJ_PATH)\musiclib_browse_play_extra.obj\
        		$(OBJ_PATH)\musiclib_browse_play_extra2.obj

OBJLIST=	$(OBJLIST)\
		$(MUSICLIBLIST)

#///////////////////////////////////////////////////////////////////////////////
# Musiclib Control
#///////////////////////////////////////////////////////////////////////////////
$(MUSICLIB) : $(MUSICLIBLIST)
    @echo Creating IEEE archive library file for MusicLib Module $@
        @$(AR_EXE) -r $(MUSICLIB) $(MUSICLIBLIST)

### Music Library compilation dependencies
"$(OBJ_PATH)\musiclib_build_function.obj" : $(MUSICLIB_PATH)\src\musiclib_build_function.c
"$(OBJ_PATH)\musiclib_build_function.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_build_function.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_build_function.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_build_function.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h

"$(OBJ_PATH)\musiclib_build_genre_table.obj" : $(MUSICLIB_PATH)\src\musiclib_build_genre_table.c
"$(OBJ_PATH)\musiclib_build_genre_table.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h

"$(OBJ_PATH)\musiclib_browse_app.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_app.c
"$(OBJ_PATH)\musiclib_browse_app.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_browse_app.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_browse_app.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_browse_app.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h

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

"$(OBJ_PATH)\musiclib_voice_function.obj" : $(MUSICLIB_PATH)\src\musiclib_voice_function.c
"$(OBJ_PATH)\musiclib_voice_function.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_voice_function.obj" : $(MUSICLIB_PATH)\hdr\musiclib_hdr.h
"$(OBJ_PATH)\musiclib_voice_function.obj" : $(MUSICLIB_PATH)\ghdr\musiclib_ghdr.h
"$(OBJ_PATH)\musiclib_voice_function.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
"$(OBJ_PATH)\musiclib_voice_function.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h

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

"$(OBJ_PATH)\musiclib_browse_extra.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_extra.c
"$(OBJ_PATH)\musiclib_browse_extra.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_browse_extra.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_browse_extra.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_browse_extra.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

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

"$(OBJ_PATH)\musiclib_browse_function_common.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.c
"$(OBJ_PATH)\musiclib_browse_function_common.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_browse_function_common.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_browse_function_common.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_browse_function_common.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_browse_play.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_play.c
"$(OBJ_PATH)\musiclib_browse_play.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_browse_play.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_browse_play.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_browse_play.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_browse_play_extra.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_play_extra.c
"$(OBJ_PATH)\musiclib_browse_play_extra.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_browse_play_extra.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_browse_play_extra.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_browse_play_extra.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h

"$(OBJ_PATH)\musiclib_browse_play_extra2.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_play_extra2.c
"$(OBJ_PATH)\musiclib_browse_play_extra2.obj" : $(MUSICLIB_PATH)\hdr\musiclib_flash_function.h
"$(OBJ_PATH)\musiclib_browse_play_extra2.obj" : $(MUSICLIB_PATH)\src\musiclib_browse_function_common.h
"$(OBJ_PATH)\musiclib_browse_play_extra2.obj" : $(MUSICLIB_PATH)\src\musiclib_flash_function_common.h
"$(OBJ_PATH)\musiclib_browse_play_extra2.obj" : $(MUSICLIB_PATH)\src\musiclib_basic_function_common.h
endif

### PLAYLIST3 compilation dependencies
$(OBJ_PATH)\playlist3.obj :$(SYS_COMMON_PATH)\playlist3\playlist3.c
$(OBJ_PATH)\playlist3.obj :$(SYS_COMMON_PATH)\playlist3\playlist3internal.h
$(OBJ_PATH)\playlist3.obj :$(OUTPUT_INCLUDE_PATH)\sysspeed.h
$(OBJ_PATH)\playlist3.obj :$(OUTPUT_INCLUDE_PATH)\messages.h

$(OBJ_PATH)\playlist3init.obj :$(SYS_COMMON_PATH)\playlist3\playlist3init.c
$(OBJ_PATH)\playlist3init.obj :$(MUSICLIB_INCPATH)\musiclib_ghdr.h

$(OBJ_PATH)\playlist3init_static.obj :$(SYS_COMMON_PATH)\playlist3\playlist3init_static.c
$(OBJ_PATH)\playlist3init_static.obj :$(MUSICLIB_INCPATH)\musiclib_ghdr.h

$(OBJ_PATH)\playlist3init_metadata.obj :$(SYS_COMMON_PATH)\playlist3\playlist3init_metadata.c
$(OBJ_PATH)\playlist3init_metadata.obj :$(MUSICLIB_INCPATH)\musiclib_ghdr.h

$(OBJ_PATH)\playlist3mem.obj :$(SYS_COMMON_PATH)\playlist3\playlist3mem.c

$(OBJ_PATH)\playlist3_static.obj :$(SYS_COMMON_PATH)\playlist3\playlist3_static.c

### Menu Codebank Compilation dependencies
$(OBJ_PATH)\sdmd.obj : $(PROJMENUS_PATH)\sdmd.c
$(OBJ_PATH)\sdmd.obj : $(PROJMENUS_PATH)\sdmd.h
$(OBJ_PATH)\sdmd.obj : $(OUTPUT_INCLUDE_PATH)\resource.h

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

### Resource Extraction Dependencies
$(OUTPUT_RSRC_PATH)\string_play_all.src			: $(PROJRSRC_PATH)\strings\string_play_all.txt
$(OUTPUT_RSRC_PATH)\string_play.src			: $(PROJRSRC_PATH)\strings\string_play.txt
$(OUTPUT_RSRC_PATH)\string_unknown_year.src			: $(PROJRSRC_PATH)\strings\string_unknown_year.txt
$(OUTPUT_RSRC_PATH)\string_year_width.src			: $(PROJRSRC_PATH)\strings\string_year_width.txt
$(OUTPUT_RSRC_PATH)\string_artist.src			: $(PROJRSRC_PATH)\strings\string_artist.txt
$(OUTPUT_RSRC_PATH)\string_album.src			: $(PROJRSRC_PATH)\strings\string_album.txt
$(OUTPUT_RSRC_PATH)\string_songs.src			: $(PROJRSRC_PATH)\strings\string_songs.txt
$(OUTPUT_RSRC_PATH)\string_on_the_fly.src			: $(PROJRSRC_PATH)\strings\string_on_the_fly.txt
$(OUTPUT_RSRC_PATH)\string_new_music.src			: $(PROJRSRC_PATH)\strings\string_new_music.txt
$(OUTPUT_RSRC_PATH)\string_genre.src			: $(PROJRSRC_PATH)\strings\string_genre.txt
$(OUTPUT_RSRC_PATH)\string_year.src			: $(PROJRSRC_PATH)\strings\string_year.txt
$(OUTPUT_RSRC_PATH)\string_playlist.src			: $(PROJRSRC_PATH)\strings\string_playlist.txt
$(OUTPUT_RSRC_PATH)\string_fm_rec.src			: $(PROJRSRC_PATH)\strings\string_fm_rec.txt
$(OUTPUT_RSRC_PATH)\string_linein_rec.src			: $(PROJRSRC_PATH)\strings\string_linein_rec.txt
$(OUTPUT_RSRC_PATH)\string_play_music.src			: $(PROJRSRC_PATH)\strings\string_play_music.txt
$(OUTPUT_RSRC_PATH)\string_1_day.src			: $(PROJRSRC_PATH)\strings\string_1_day.txt
$(OUTPUT_RSRC_PATH)\string_1_week.src			: $(PROJRSRC_PATH)\strings\string_1_week.txt
$(OUTPUT_RSRC_PATH)\string_1_month.src			: $(PROJRSRC_PATH)\strings\string_1_month.txt

$(OUTPUT_RSRC_PATH)\build_ml.src  : $(PROJRSRC_PATH)\bitmaps\build_ml.bmp
$(OUTPUT_RSRC_PATH)\build_ml_warning.src  : $(PROJRSRC_PATH)\bitmaps\build_ml_warning.bmp
$(OUTPUT_RSRC_PATH)\build_ml_warning2.src  : $(PROJRSRC_PATH)\bitmaps\build_ml_warning2.bmp
$(OUTPUT_RSRC_PATH)\build_flash1.src  : $(PROJRSRC_PATH)\bitmaps\build_flash1.bmp
$(OUTPUT_RSRC_PATH)\build_flash2.src  : $(PROJRSRC_PATH)\bitmaps\build_flash2.bmp
$(OUTPUT_RSRC_PATH)\build_flash3.src  : $(PROJRSRC_PATH)\bitmaps\build_flash3.bmp
$(OUTPUT_RSRC_PATH)\build_sd1.src  : $(PROJRSRC_PATH)\bitmaps\build_sd1.bmp
$(OUTPUT_RSRC_PATH)\build_sd2.src  : $(PROJRSRC_PATH)\bitmaps\build_sd2.bmp
$(OUTPUT_RSRC_PATH)\build_sd3.src  : $(PROJRSRC_PATH)\bitmaps\build_sd3.bmp
$(OUTPUT_RSRC_PATH)\sd_remove.src  : $(PROJRSRC_PATH)\bitmaps\sd_remove.bmp
$(OUTPUT_RSRC_PATH)\sd_insert.src  : $(PROJRSRC_PATH)\bitmaps\sd_insert.bmp
$(OUTPUT_RSRC_PATH)\build_newmusic.src  : $(PROJRSRC_PATH)\bitmaps\build_newmusic.bmp
$(OUTPUT_RSRC_PATH)\highlight_back.src  : $(PROJRSRC_PATH)\bitmaps\highlight_back.bmp
$(OUTPUT_RSRC_PATH)\empty_favourite.src  : $(PROJRSRC_PATH)\bitmaps\empty_favourite.bmp
$(OUTPUT_RSRC_PATH)\on_the_fly_full.src  : $(PROJRSRC_PATH)\bitmaps\on_the_fly_full.bmp
$(OUTPUT_RSRC_PATH)\on_the_fly_free1.src  : $(PROJRSRC_PATH)\bitmaps\on_the_fly_free1.bmp
$(OUTPUT_RSRC_PATH)\on_the_fly_free2.src  : $(PROJRSRC_PATH)\bitmaps\on_the_fly_free2.bmp
$(OUTPUT_RSRC_PATH)\on_the_fly_delete1.src  : $(PROJRSRC_PATH)\bitmaps\on_the_fly_delete1.bmp
$(OUTPUT_RSRC_PATH)\on_the_fly_delete2.src  : $(PROJRSRC_PATH)\bitmaps\on_the_fly_delete2.bmp
$(OUTPUT_RSRC_PATH)\check_disk_1.src  : $(PROJRSRC_PATH)\bitmaps\check_disk_1.bmp
$(OUTPUT_RSRC_PATH)\check_disk_2.src  : $(PROJRSRC_PATH)\bitmaps\check_disk_2.bmp
$(OUTPUT_RSRC_PATH)\check_disk_3.src  : $(PROJRSRC_PATH)\bitmaps\check_disk_3.bmp
$(OUTPUT_RSRC_PATH)\flash_error.src  : $(PROJRSRC_PATH)\bitmaps\flash_error.bmp

### PLAYLIST3 extractions
"$(OUTPUT_RSRC_PATH)\playlist_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract playlist_codebank
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist3,.ptextfindfirst,.ptextfindnext -f$(OUTPUT_PATH)\$(PROJ).map

# We should use "playlist3init_codeback.src", but we use "playlist2init_codebank.src" to make the resource linker happy
"$(OUTPUT_RSRC_PATH)\playlist2init_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract playlist3init_codebank to playlist2init_codebank.src
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextplaylist3init,.ptextfindfirst,.ptextfindnext -f$(OUTPUT_PATH)\$(PROJ).map

#the following are null sources if using playlist3. Here to make the resource linker happy.
"$(OUTPUT_RSRC_PATH)\playlist2traverse_codebank.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL playlist2traverse_codebank
		copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist2traverse_codebank.src

### Music Library extractions
"$(OUTPUT_RSRC_PATH)\MusicLibBuildModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibBuildGenreTable.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibBuildFlushMem.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibBrowseApp.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibBrowseModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibPlayModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibPlayAllNextModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibPlayAllPrevModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibShuffleModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibVoiceApp.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibVoiceModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\MusicLibVoicePlayModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\playlist3.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
ifdef D_MMC
"$(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextmusiclib_merge_module,.ptextmusiclib_merge_module_flash -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextmusiclib_merge_function,.ptextmusiclib_basic_function_common,.ptextmusiclib_merge_function_flash -F$(OUTPUT_PATH)\$(PROJ).map

else
"$(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibMergeModule.src
"$(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\MusicLibMergeFunction.src
endif

"$(OUTPUT_RSRC_PATH)\Funclet_write_detection_access.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_WRITE_DETECTION_ACCESS_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\playmusicmenu.src"	: $(OUTPUT_PATH)\$(PROJ).abs playlist3menus.loc
"$(OUTPUT_RSRC_PATH)\browsemenu.src"	: $(OUTPUT_PATH)\$(PROJ).abs playlist3menus.loc
"$(OUTPUT_RSRC_PATH)\browsemenu_extra.src"	: $(OUTPUT_PATH)\$(PROJ).abs playlist3menus.loc
"$(OUTPUT_RSRC_PATH)\newmusicmenu.src"	: $(OUTPUT_PATH)\$(PROJ).abs playlist3menus.loc
"$(OUTPUT_RSRC_PATH)\sdmd.src" 		: $(OUTPUT_PATH)\$(PROJ).abs playlist3menus.loc
"$(OUTPUT_RSRC_PATH)\musiclib_build_function_sec.src" 		: $(OUTPUT_PATH)\$(PROJ).abs playlist3menus.loc
"$(OUTPUT_RSRC_PATH)\musiclib_build_function_flush.src"     : $(OUTPUT_PATH)\$(PROJ).abs playlist3menus.loc
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetLvlOneItem_info.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_LOCAL_GET_LVL_ONE_ITEM_INFO_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_LOCAL_GetFlashAssociatedList_info.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_LOCAL_GET_FLASH_ASSOCIATED_LIST_INFO_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ResumePlayState.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_RESUMEPLAYSTATE_P -F$(OUTPUT_PATH)\$(PROJ).map

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

"$(OUTPUT_RSRC_PATH)\Funclet_ML_movePointerBackward.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_MOVE_POINTER_BACKWARD_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_findLastItemPointer.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_FIND_LAST_ITEM_POINTER_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_startShuffle.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_START_SHUFFLE_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_BrowseMenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_BROWSE_MENU_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_StartPlayAllSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_START_PLAY_ALL_SONG_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayAllNextSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_PLAY_ALL_NEXT_SONG_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayAllPreviousSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_PLAY_ALL_PREVIOUS_SONG_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ShuffleNextSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_SHUFFLE_NEXT_SONG_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayNextSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_PLAY_NEXT_SONG_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_PLAY_PREVIOUS_SONG_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_BuildVoiceFilePath.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_BUILDVOICEFILEPATH_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_BuildFMFilePath.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_BUILDFMFILEPATH_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_BuildLINEINFilePath.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_BUILDLINEINFILEPATH_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_GetVoiceFastKey.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_GETVOICEFASTKEY_P -F$(OUTPUT_PATH)\$(PROJ).map

ifdef D_PL3_FB

"$(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListId.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_ID_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListRootId.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_ROOT_ID_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_GetFlashDirAssocListSize.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_GET_FLASH_DIR_ASSOC_LIST_SIZE_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_GetIdByStateAndPosition.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_GET_ID_BY_STATE_AND_POSITION_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_StartPlayDirSongInit.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_START_PLAY_DIR_SONG_INIT_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_ShuffleNextDirSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_SHUFFLE_NEXT_DIR_SONG_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ML_PlayPreviousDirSong.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ML_PLAY_PREVIOUS_DIR_SONG_P -F$(OUTPUT_PATH)\$(PROJ).map

else

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

endif

"$(OUTPUT_RSRC_PATH)\playlist5_browsemenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_browsemenu.src

"$(OUTPUT_RSRC_PATH)\playlist5_browse_submenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_browse_submenu.src

"$(OUTPUT_RSRC_PATH)\playlist5_browse_module.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_browse_module.src

"$(OUTPUT_RSRC_PATH)\playlist5_playback_module.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_playback_module.src
        
"$(OUTPUT_RSRC_PATH)\playlist5_browsemenu_extra.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\playlist5_browsemenu_extra.src
	
"$(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushBack.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushBack.src	
		
"$(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushFront.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_PL5_PL_QueuePushFront.src	
	
ifndef SYNC_LYRICS
"$(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_Get_LRCDirName_PL5.src
endif        

