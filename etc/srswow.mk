INC_ALL_C = $(INC_ALL_C) -Wc-I$(SW_MSGMODULES_PATH)\Effects\srswow

#all paths are defined either in player.mk or standard.mk

CODEBANKLIST = $(CODEBANKLIST)\
			 $(OUTPUT_RSRC_PATH)\wowmenu.src\

RESOURCELIST = $(RESOURCELIST)\
    $(OUTPUT_RSRC_PATH)\string_wow_menu.src\
    $(OUTPUT_RSRC_PATH)\string_wowbrightness_menu.src\
    $(OUTPUT_RSRC_PATH)\string_wowbrightness_low_menu.src\
    $(OUTPUT_RSRC_PATH)\string_wowbrightness_mid_menu.src\
    $(OUTPUT_RSRC_PATH)\string_wowbrightness_high_menu.src\
    $(OUTPUT_RSRC_PATH)\string_wowtrubass_menu.src\
    $(OUTPUT_RSRC_PATH)\string_wowvolume_menu.src\
	$(OUTPUT_RSRC_PATH)\wow_icon.src\
	$(OUTPUT_RSRC_PATH)\wowctrl.src\
	$(OUTPUT_RSRC_PATH)\wow16k.src\
	$(OUTPUT_RSRC_PATH)\wow32k.src\
	$(OUTPUT_RSRC_PATH)\wow8k.src\
	$(OUTPUT_RSRC_PATH)\wow11k.src\
	$(OUTPUT_RSRC_PATH)\wow22k.src\
	$(OUTPUT_RSRC_PATH)\wow24k.src\
	$(OUTPUT_RSRC_PATH)\wow44k.src\
	$(OUTPUT_RSRC_PATH)\wow48k.src\
	$(OUTPUT_RSRC_PATH)\wow16k_Y.src\
	$(OUTPUT_RSRC_PATH)\wow32k_Y.src\
	$(OUTPUT_RSRC_PATH)\wow8k_Y.src\
	$(OUTPUT_RSRC_PATH)\wow11k_Y.src\
	$(OUTPUT_RSRC_PATH)\wow22k_Y.src\
	$(OUTPUT_RSRC_PATH)\wow24k_Y.src\
	$(OUTPUT_RSRC_PATH)\wow44k_Y.src\
	$(OUTPUT_RSRC_PATH)\wow48k_Y.src\
    $(OUTPUT_RSRC_PATH)\srs_effectsmodules_p.src\
    $(OUTPUT_RSRC_PATH)\srs_effectsmodules_x.src

#############################################################################################################################################
ifdef D_WOW_ENABLE

GENERALLIB = $(GENERALLIB)\
          $(SW_MSGMODULES_PATH)\Effects\srswow\wowlib.a

OBJLIST = $(OBJLIST)\
          $(OBJ_PATH)\wow_init.obj\
          $(OBJ_PATH)\wow_process.obj\
		  $(OBJ_PATH)\wowmenu.obj\


$(OBJ_PATH)\wow_init.obj : $(SW_MSGMODULES_PATH)\Effects\srswow\wow_init.c
$(OBJ_PATH)\wow_init.obj : $(SW_MSGMODULES_PATH)\Effects\srswow\wow_ed.h
$(OBJ_PATH)\wow_process.obj : $(SW_MSGMODULES_PATH)\Effects\srswow\wow_process.asm

$(OBJ_PATH)\wowmenu.obj         : $(PROJMENUS_PATH)\wowmenu.c 
$(OBJ_PATH)\wowmenu.obj         : $(PROJMENUS_PATH)\wowmenu.h
$(OBJ_PATH)\wowmenu.obj         : $(OUTPUT_INCLUDE_PATH)\resource.h  
$(OBJ_PATH)\wowmenu.obj         : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\wowmenu.obj         : $(PROJMENUS_PATH)\menus.h

#extract resources
#Text resources
$(OUTPUT_RSRC_PATH)\string_wow_menu.src     : $(PROJRSRC_PATH)\strings\string_wow_menu.txt
$(OUTPUT_RSRC_PATH)\string_wowbrightness_menu.src : $(PROJRSRC_PATH)\strings\string_wowbrightness_menu.txt
$(OUTPUT_RSRC_PATH)\string_wowtrubass_menu.src : $(PROJRSRC_PATH)\strings\string_wowtrubass_menu.txt
$(OUTPUT_RSRC_PATH)\string_wowvolume_menu.src    : $(PROJRSRC_PATH)\strings\string_wowvolume_menu.txt
$(OUTPUT_RSRC_PATH)\string_wowbrightness_low_menu.src : $(PROJRSRC_PATH)\strings\string_wowbrightness_low_menu.txt
$(OUTPUT_RSRC_PATH)\string_wowbrightness_mid_menu.src : $(PROJRSRC_PATH)\strings\string_wowbrightness_mid_menu.txt
$(OUTPUT_RSRC_PATH)\string_wowbrightness_high_menu.src : $(PROJRSRC_PATH)\strings\string_wowbrightness_high_menu.txt

#bitmap resources
$(OUTPUT_RSRC_PATH)\wow_icon.src     : $(BITMAP_PATH)\icons\wow\wow_icon.bmp

#menu resources

"$(OUTPUT_RSRC_PATH)\wowmenu.src"       : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Wow Menu
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextwowmenu -f$(OUTPUT_PATH)\$(PROJ).map


"$(OUTPUT_RSRC_PATH)\wowctrl.src" : $(OUTPUT_PATH)\$(PROJ).abs 
"$(OUTPUT_RSRC_PATH)\wow16k.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wow32k.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wow8k.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wow11k.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wow22k.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wow24k.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wow44k.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wow48k.src" : $(OUTPUT_PATH)\$(PROJ).abs	
"$(OUTPUT_RSRC_PATH)\wow11k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs 
"$(OUTPUT_RSRC_PATH)\wow16k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs 
"$(OUTPUT_RSRC_PATH)\wow24k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs 
"$(OUTPUT_RSRC_PATH)\wow44k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs 
"$(OUTPUT_RSRC_PATH)\wow48k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs 
"$(OUTPUT_RSRC_PATH)\wow8k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs	 
"$(OUTPUT_RSRC_PATH)\wow22k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs 
"$(OUTPUT_RSRC_PATH)\wow32k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\srs_effectsmodules_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\srs_effectsmodules_x.src" : $(OUTPUT_PATH)\$(PROJ).abs 
        @echo "Extract SRS-WOW program code and data sections"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\srswow.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

else
#########################################################################################################################
#### create dummy resources so there are no changes to resource.inc######################################################
"$(OUTPUT_RSRC_PATH)\wowmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wowmenu.src

"$(OUTPUT_RSRC_PATH)\string_wow_menu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_wow_menu.src

"$(OUTPUT_RSRC_PATH)\string_wowbrightness_menu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_wowbrightness_menu.src

"$(OUTPUT_RSRC_PATH)\string_wowbrightness_low_menu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_wowbrightness_low_menu.src

"$(OUTPUT_RSRC_PATH)\string_wowbrightness_mid_menu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_wowbrightness_mid_menu.src

"$(OUTPUT_RSRC_PATH)\string_wowbrightness_high_menu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_wowbrightness_high_menu.src

"$(OUTPUT_RSRC_PATH)\string_wowtrubass_menu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_wowtrubass_menu.src

"$(OUTPUT_RSRC_PATH)\string_wowvolume_menu.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_wowvolume_menu.src

"$(OUTPUT_RSRC_PATH)\wow_icon.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow_icon.src

"$(OUTPUT_RSRC_PATH)\wowctrl.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wowctrl.src

"$(OUTPUT_RSRC_PATH)\wow16k.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow16k.src

"$(OUTPUT_RSRC_PATH)\wow32k.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow32k.src

"$(OUTPUT_RSRC_PATH)\wow8k.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow8k.src

"$(OUTPUT_RSRC_PATH)\wow11k.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow11k.src

"$(OUTPUT_RSRC_PATH)\wow22k.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow22k.src

"$(OUTPUT_RSRC_PATH)\wow24k.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow24k.src

"$(OUTPUT_RSRC_PATH)\wow44k.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow44k.src

"$(OUTPUT_RSRC_PATH)\wow48k.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow48k.src

"$(OUTPUT_RSRC_PATH)\wow11k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow11k_Y.src

"$(OUTPUT_RSRC_PATH)\wow16k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow16k_Y.src

"$(OUTPUT_RSRC_PATH)\wow24k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow24k_Y.src

"$(OUTPUT_RSRC_PATH)\wow44k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow44k_Y.src

"$(OUTPUT_RSRC_PATH)\wow48k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow48k_Y.src

"$(OUTPUT_RSRC_PATH)\wow8k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow8k_Y.src

"$(OUTPUT_RSRC_PATH)\wow22k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow22k_Y.src

"$(OUTPUT_RSRC_PATH)\wow32k_Y.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\wow32k_Y.src
    
"$(OUTPUT_RSRC_PATH)\srs_effectsmodules_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\srs_effectsmodules_p.src

"$(OUTPUT_RSRC_PATH)\srs_effectsmodules_x.src" : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\srs_effectsmodules_x.src

endif
