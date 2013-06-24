OBJLIST =$(OBJLIST)\
		 $(OBJ_PATH)\rtcaccess.obj\
		 $(OBJ_PATH)\setdatemenu.obj\
		 $(OBJ_PATH)\settimemenu.obj\
		 $(OBJ_PATH)\timedatemenu.obj


#all src files must be built regardless of RTC option
#if RTC is disabled then the resources will be built 
#but will be a null resource

#Timedate files
CODEBANKLIST = $(CODEBANKLIST)\
	  $(OUTPUT_RSRC_PATH)\timedatemenu.src\
	  $(OUTPUT_RSRC_PATH)\setdatemenu.src\
	  $(OUTPUT_RSRC_PATH)\settimemenu.src

FUNCLETSRCLIST =    $(FUNCLETSRCLIST)\
      $(OUTPUT_RSRC_PATH)\Funclet_ReadRTC.src\
	  $(OUTPUT_RSRC_PATH)\Funclet_InitRTC.src\
	  $(OUTPUT_RSRC_PATH)\Funclet_SetRTC.src
            
###################################################
# Timedate Text resource list
RESOURCELIST = $(RESOURCELIST)\
	$(OUTPUT_RSRC_PATH)\string_sunday.src\
	$(OUTPUT_RSRC_PATH)\string_monday.src\
	$(OUTPUT_RSRC_PATH)\string_tuesday.src\
	$(OUTPUT_RSRC_PATH)\string_wednesday.src\
	$(OUTPUT_RSRC_PATH)\string_thursday.src\
	$(OUTPUT_RSRC_PATH)\string_friday.src\
	$(OUTPUT_RSRC_PATH)\string_saturday.src\
	$(OUTPUT_RSRC_PATH)\string_am.src\
	$(OUTPUT_RSRC_PATH)\string_pm.src\
	$(OUTPUT_RSRC_PATH)\string_amclear.src\
	$(OUTPUT_RSRC_PATH)\string_slash.src\
	$(OUTPUT_RSRC_PATH)\string_colon.src\
	$(OUTPUT_RSRC_PATH)\string_12hour.src\
	$(OUTPUT_RSRC_PATH)\string_24hour.src\
	$(OUTPUT_RSRC_PATH)\string_mmddyyyy.src\
	$(OUTPUT_RSRC_PATH)\string_ddmmyyyy.src\
	$(OUTPUT_RSRC_PATH)\string_yyyymmdd.src\
	$(OUTPUT_RSRC_PATH)\string_format.src\
	$(OUTPUT_RSRC_PATH)\string_time_date_menu.src\
	$(OUTPUT_RSRC_PATH)\string_set_time_menu.src\
	$(OUTPUT_RSRC_PATH)\string_set_date_menu.src\
	$(OUTPUT_RSRC_PATH)\string_ok.src\
	$(OUTPUT_RSRC_PATH)\string_cancel.src

#######################################################
# Timedate Bitmap resource list    
RESOURCELIST = $(RESOURCELIST)\
	$(OUTPUT_RSRC_PATH)\time_date_title.src\
	$(OUTPUT_RSRC_PATH)\set_time_title.src\
	$(OUTPUT_RSRC_PATH)\set_date_title.src\
	$(OUTPUT_RSRC_PATH)\save_changes_yes.src\
	$(OUTPUT_RSRC_PATH)\save_changes_no.src\
	$(OUTPUT_RSRC_PATH)\save_changes_cancel.src\
	$(OUTPUT_RSRC_PATH)\save_changes_clear.src

#########################################################
# Build dependencies                                               
INC_ALL_C = $(INC_ALL_C) -Wc-I$(SYS_COMMON_PATH)\rtcaccess

"$(OBJ_PATH)\rtcaccess.obj"     : $(SYS_COMMON_PATH)\rtcaccess.asm
"$(OBJ_PATH)\timedatemenu.obj"  : $(PROJMENUS_PATH)\timedatemenu.c
"$(OBJ_PATH)\timedatemenu.obj"  : $(PROJMENUS_PATH)\timedatemenu.h
"$(OBJ_PATH)\settimemenu.obj"	: $(PROJMENUS_PATH)\settimemenu.c
"$(OBJ_PATH)\settimemenu.obj"	: $(PROJMENUS_PATH)\settimemenu.h
"$(OBJ_PATH)\setdatemenu.obj"	: $(PROJMENUS_PATH)\setdatemenu.c
"$(OBJ_PATH)\setdatemenu.obj"	: $(PROJMENUS_PATH)\setdatemenu.h

##################################################
#Timedate Text resources
$(OUTPUT_RSRC_PATH)\string_sunday.src     	: $(PROJRSRC_PATH)\strings\string_sunday.txt
$(OUTPUT_RSRC_PATH)\string_monday.src     	: $(PROJRSRC_PATH)\strings\string_monday.txt
$(OUTPUT_RSRC_PATH)\string_tuesday.src     	: $(PROJRSRC_PATH)\strings\string_tuesday.txt
$(OUTPUT_RSRC_PATH)\string_wednesday.src    : $(PROJRSRC_PATH)\strings\string_wednesday.txt
$(OUTPUT_RSRC_PATH)\string_thursday.src     : $(PROJRSRC_PATH)\strings\string_thursday.txt
$(OUTPUT_RSRC_PATH)\string_friday.src     	: $(PROJRSRC_PATH)\strings\string_friday.txt
$(OUTPUT_RSRC_PATH)\string_saturday.src     : $(PROJRSRC_PATH)\strings\string_saturday.txt
$(OUTPUT_RSRC_PATH)\string_slash.src		: $(PROJRSRC_PATH)\strings\string_slash.txt
$(OUTPUT_RSRC_PATH)\string_colon.src		: $(PROJRSRC_PATH)\strings\string_colon.txt
$(OUTPUT_RSRC_PATH)\string_am.src			: $(PROJRSRC_PATH)\strings\string_am.txt
$(OUTPUT_RSRC_PATH)\string_pm.src			: $(PROJRSRC_PATH)\strings\string_pm.txt
$(OUTPUT_RSRC_PATH)\string_amclear.src		: $(PROJRSRC_PATH)\strings\string_amclear.txt
$(OUTPUT_RSRC_PATH)\string_12hour.src		: $(PROJRSRC_PATH)\strings\string_12hour.txt			   
$(OUTPUT_RSRC_PATH)\string_24hour.src		: $(PROJRSRC_PATH)\strings\string_24hour.txt
$(OUTPUT_RSRC_PATH)\string_format.src		: $(PROJRSRC_PATH)\strings\string_format.txt
$(OUTPUT_RSRC_PATH)\string_mmddyyyy.src		: $(PROJRSRC_PATH)\strings\string_mmddyyyy.txt
$(OUTPUT_RSRC_PATH)\string_ddmmyyyy.src		: $(PROJRSRC_PATH)\strings\string_ddmmyyyy.txt
$(OUTPUT_RSRC_PATH)\string_yyyymmdd.src		: $(PROJRSRC_PATH)\strings\string_yyyymmdd.txt
$(OUTPUT_RSRC_PATH)\string_time_date_menu.src	: $(PROJRSRC_PATH)\strings\string_time_date_menu.src
$(OUTPUT_RSRC_PATH)\string_set_time_menu.src	: $(PROJRSRC_PATH)\strings\string_set_time_menu.src
$(OUTPUT_RSRC_PATH)\string_set_date_menu.src	: $(PROJRSRC_PATH)\strings\string_set_date_menu.src
$(OUTPUT_RSRC_PATH)\string_ok.src			: $(PROJRSRC_PATH)\strings\string_ok.txt
$(OUTPUT_RSRC_PATH)\string_cancel.src		: $(PROJRSRC_PATH)\strings\string_cancel.txt

#####################################################
#Timedate bitmaps
$(OUTPUT_RSRC_PATH)\time_date_title.src     : $(PROJRSRC_PATH)\bitmaps\time_date_title.bmp
$(OUTPUT_RSRC_PATH)\set_time_title.src		: $(PROJRSRC_PATH)\bitmaps\set_time_title.bmp
$(OUTPUT_RSRC_PATH)\set_date_title.src		: $(PROJRSRC_PATH)\bitmaps\set_date_title.bmp
$(OUTPUT_RSRC_PATH)\save_changes_yes.src	: $(PROJRSRC_PATH)\bitmaps\save_changes_yes.bmp
$(OUTPUT_RSRC_PATH)\save_changes_no.src		: $(PROJRSRC_PATH)\bitmaps\save_changes_no.bmp
$(OUTPUT_RSRC_PATH)\save_changes_cancel.src	: $(PROJRSRC_PATH)\bitmaps\save_changes_cancel.bmp
$(OUTPUT_RSRC_PATH)\save_changes_clear.src	: $(PROJRSRC_PATH)\bitmaps\save_changes_clear.bmp

#############################################
# For SDK's before 3.05
"$(OUTPUT_RSRC_PATH)\timedatemenu.src"          : $(OUTPUT_PATH)\$(PROJ).abs
	@$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptexttimedatemenu -F$(OUTPUT_PATH)\$(PROJ).map
	        
"$(OUTPUT_RSRC_PATH)\settimemenu.src"           : $(OUTPUT_PATH)\$(PROJ).abs	        
	@$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextsettimemenu -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\setdatemenu.src"           : $(OUTPUT_PATH)\$(PROJ).abs	
	@$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextsetdatemenu -F$(OUTPUT_PATH)\$(PROJ).map
  
 
###############################################
#funclets
"$(OUTPUT_RSRC_PATH)\Funclet_InitRTC.src"		 : $(OUTPUT_PATH)\$(PROJ).abs
	@$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_InitRTC_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_SetRTC.src"		 : $(OUTPUT_PATH)\$(PROJ).abs
	@$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_SetRTC_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_ReadRTC.src"		 : $(OUTPUT_PATH)\$(PROJ).abs
	@$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_ReadRTC_P -F$(OUTPUT_PATH)\$(PROJ).map

