#This is the makefile for recording test files
#
#To include the function in the sdk, just use TEST build option
#when build sdk

CODEBANKLIST = $(CODEBANKLIST)\
			 $(OUTPUT_RSRC_PATH)\testmenu.src\
			\

RESOURCELIST = $(RESOURCELIST)\
				$(OUTPUT_RSRC_PATH)\string_recordtest_menu.src\
				$(OUTPUT_RSRC_PATH)\string_recordtest_duration.src\
				$(OUTPUT_RSRC_PATH)\string_all.src\
				$(OUTPUT_RSRC_PATH)\string_recordtest_time5.src\
				$(OUTPUT_RSRC_PATH)\string_recordtest_time10.src\
				$(OUTPUT_RSRC_PATH)\string_recordtest_time30.src\
				$(OUTPUT_RSRC_PATH)\string_recordtest_time60.src\
				$(OUTPUT_RSRC_PATH)\string_recordtest_time300.src\
				$(OUTPUT_RSRC_PATH)\string_recordtest_time600.src\
				$(OUTPUT_RSRC_PATH)\test_title.src


ifdef D_TEST

OBJLIST    = $(OBJLIST)\
            $(OBJ_PATH)\testmenu.obj


#Text resources
$(OUTPUT_RSRC_PATH)\string_recordtest_menu.src  : $(PROJRSRC_PATH)\strings\string_recordtest_menu.txt
$(OUTPUT_RSRC_PATH)\string_recordtest_duration.src  : $(PROJRSRC_PATH)\strings\string_recordtest_duration.txt
$(OUTPUT_RSRC_PATH)\string_recordtest_time5.src  : $(PROJRSRC_PATH)\strings\string_recordtest_time5.txt
$(OUTPUT_RSRC_PATH)\string_recordtest_time10.src  : $(PROJRSRC_PATH)\strings\string_recordtest_time10.txt
$(OUTPUT_RSRC_PATH)\string_recordtest_time30.src  : $(PROJRSRC_PATH)\strings\string_recordtest_time30.txt
$(OUTPUT_RSRC_PATH)\string_recordtest_time60.src  : $(PROJRSRC_PATH)\strings\string_recordtest_time60.txt
$(OUTPUT_RSRC_PATH)\string_recordtest_time300.src  : $(PROJRSRC_PATH)\strings\string_recordtest_time300.txt
$(OUTPUT_RSRC_PATH)\string_recordtest_time600.src  : $(PROJRSRC_PATH)\strings\string_recordtest_time600.txt
$(OUTPUT_RSRC_PATH)\string_all.src  : $(PROJRSRC_PATH)\strings\string_all.txt
#bitmap resources
$(OUTPUT_RSRC_PATH)\test_title.src            :$(PROJRSRC_PATH)\bitmaps\test_title.bmp

#///////////////////////////////////////////////////////////////////////////////
# Record test file Common
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\testmenu.obj :$(PROJMENUS_PATH)\testmenu.c

"$(OUTPUT_RSRC_PATH)\testmenu.src"        : $(OUTPUT_PATH)\$(PROJ).abs
    @echo Extract TestMenu
    @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptexttestmenu -f$(OUTPUT_PATH)\$(PROJ).map

else

"$(OUTPUT_RSRC_PATH)\string_recordtest_menu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\string_recordtest_menu.src
"$(OUTPUT_RSRC_PATH)\string_recordtest_duration.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\string_recordtest_duration.src
"$(OUTPUT_RSRC_PATH)\string_recordtest_time5.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\string_recordtest_time5.src
"$(OUTPUT_RSRC_PATH)\string_recordtest_time10.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\string_recordtest_time10.src
"$(OUTPUT_RSRC_PATH)\string_recordtest_time30.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\string_recordtest_time30.src
"$(OUTPUT_RSRC_PATH)\string_recordtest_time60.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\string_recordtest_time60.src
"$(OUTPUT_RSRC_PATH)\string_recordtest_time300.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\string_recordtest_time300.src
"$(OUTPUT_RSRC_PATH)\string_recordtest_time600.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\string_recordtest_time600.src
"$(OUTPUT_RSRC_PATH)\string_all.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\string_all.src

"$(OUTPUT_RSRC_PATH)\test_title.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\test_title.src
"$(OUTPUT_RSRC_PATH)\testmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(RELBASE_PATH)\resources\misc\null_data.src $(OUTPUT_RSRC_PATH)\testmenu.src

endif