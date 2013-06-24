ifndef D_AUDIBLEDEC

OBJLIST=    $(OBJLIST)\

MODULELIST = $(MODULELIST)\
	$(OUTPUT_RSRC_PATH)\audiblemetadata_p.src\
	$(OUTPUT_RSRC_PATH)\audiblemetadata_y.src\
	$(OUTPUT_RSRC_PATH)\audiblemetastrings_p.src\
	$(OUTPUT_RSRC_PATH)\audiblefillmetadata_p.src\
	$(OUTPUT_RSRC_PATH)\audiblesongposition_p.src\
	$(OUTPUT_RSRC_PATH)\audibletargetcheck_p.src\
	$(OUTPUT_RSRC_PATH)\audiblefillfileinfo_p.src\
    $(OUTPUT_RSRC_PATH)\audibledsa_p.src\
	$(OUTPUT_RSRC_PATH)\aaactivationrecords_p.src

RESOURCELIST = $(RESOURCELIST)\
	$(OUTPUT_RSRC_PATH)\audible_secnav.src

"$(OUTPUT_RSRC_PATH)\audible_secnav.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL audible_secnav.src.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audible_secnav.src

"$(OUTPUT_RSRC_PATH)\audiblemetadata_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL audiblemetadata_p.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblemetadata_p.src

"$(OUTPUT_RSRC_PATH)\audibledsa_p.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL audibledsa_p.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audibledsa_p.src

$(OUTPUT_RSRC_PATH)\audiblemetadata_y.src : $(OUTPUT_PATH)\$(PROJ).abs
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblemetadata_y.src

$(OUTPUT_RSRC_PATH)\audiblemetastrings_p.src : $(OUTPUT_PATH)\$(PROJ).abs
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblemetastrings_p.src

$(OUTPUT_RSRC_PATH)\audiblefillmetadata_p.src : $(OUTPUT_PATH)\$(PROJ).abs
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblefillmetadata_p.src        

$(OUTPUT_RSRC_PATH)\audiblesongposition_p.src : $(OUTPUT_PATH)\$(PROJ).abs
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblesongposition_p.src

$(OUTPUT_RSRC_PATH)\audibletargetcheck_p.src : $(OUTPUT_PATH)\$(PROJ).abs
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audibletargetcheck_p.src

$(OUTPUT_RSRC_PATH)\audiblefillfileinfo_p.src : $(OUTPUT_PATH)\$(PROJ).abs
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblefillfileinfo_p.src

$(OUTPUT_RSRC_PATH)\aaactivationrecords_p.src : $(OUTPUT_PATH)\$(PROJ).abs
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\aaactivationrecords_p.src

else
OBJLIST =$(OBJLIST)\
	$(OBJ_PATH)\audiblemetadata.obj\
	$(OBJ_PATH)\audiblefillmetadata.obj\
	$(OBJ_PATH)\AAMetaStrings.obj\
	$(OBJ_PATH)\audiblesongposition.obj\
	$(OBJ_PATH)\activationrecords.obj\
	$(OBJ_PATH)\checktargeting.obj\
	$(OBJ_PATH)\dsaverify.obj\
	$(OBJ_PATH)\keys.obj\
	$(OBJ_PATH)\ronsdsa.obj\
	$(OBJ_PATH)\sha.obj\
	$(OBJ_PATH)\metafillaudiblefileinfo.obj\
	$(OBJ_PATH)\aaactivationrecords.obj\
	$(OBJ_PATH)\aaactivationfilename.obj
        
MODULELIST = $(MODULELIST)\
	$(OUTPUT_RSRC_PATH)\audiblemetadata_p.src\
	$(OUTPUT_RSRC_PATH)\audiblemetadata_y.src\
	$(OUTPUT_RSRC_PATH)\audiblemetastrings_p.src\
	$(OUTPUT_RSRC_PATH)\audiblefillmetadata_p.src\
	$(OUTPUT_RSRC_PATH)\audiblesongposition_p.src\
	$(OUTPUT_RSRC_PATH)\audibletargetcheck_p.src\
	$(OUTPUT_RSRC_PATH)\audiblefillfileinfo_p.src\
	$(OUTPUT_RSRC_PATH)\aaactivationrecords_p.src

RESOURCELIST = $(RESOURCELIST)\
	$(OUTPUT_RSRC_PATH)\audible_secnav.src
        
$(OUTPUT_RSRC_PATH)\audiblemetadata_p.src				: $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audiblemetadata.loc
$(OUTPUT_RSRC_PATH)\audiblemetadata_y.src				: $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audiblemetadata.loc
$(OUTPUT_RSRC_PATH)\audiblemetastrings_p.src			: $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audiblemetadata.loc
$(OUTPUT_RSRC_PATH)\audiblefillmetadata_p.src			: $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audiblemetadata.loc
$(OUTPUT_RSRC_PATH)\audiblesongposition_p.src			: $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audiblemetadata.loc
$(OUTPUT_RSRC_PATH)\audibletargetcheck_p.src			: $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audiblemetadata.loc
$(OUTPUT_RSRC_PATH)\audiblefillfileinfo_p.src			: $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audiblemetadata.loc
$(OUTPUT_RSRC_PATH)\aaactivationrecords_p.src			: $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audiblemetadata.loc
        @echo Extract MetaData codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\


#Bitmap Resources
$(OUTPUT_RSRC_PATH)\audible_secnav.src		: $(PROJRSRC_PATH)\bitmaps\audible_secnav.bmp

#Audible Objects
$(OBJ_PATH)\audiblemetadata.obj		: $(SYS_COMMON_PATH)\metadata\AudibleMetadata.c
$(OBJ_PATH)\audiblefillmetadata.obj	: $(SYS_COMMON_PATH)\metadata\audiblefillmetadata.c
$(OBJ_PATH)\aametastrings.obj		: $(SYS_COMMON_PATH)\metadata\AAMetaStrings.c
$(OBJ_PATH)\audiblesongposition.obj : $(SYS_COMMON_PATH)\metadata\audiblesongposition.c
"$(OBJ_PATH)\activationrecords.obj"	: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\activationrecords.c
"$(OBJ_PATH)\activationrecords.obj"	: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\activationrecords.h
"$(OBJ_PATH)\checktargeting.obj"	: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\checktargeting.c
"$(OBJ_PATH)\dsaverify.obj"			: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\dsaverify.c
"$(OBJ_PATH)\keys.obj"				: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\keys.c
"$(OBJ_PATH)\ronsdsa.obj"			: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\ronsdsa.c
"$(OBJ_PATH)\sha.obj"				: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\sha.c
"$(OBJ_PATH)\metafillaudiblefileinfo.obj"	: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\metafillaudiblefileinfo.c
"$(OBJ_PATH)\aaactivationrecords.obj"		: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\aaactivationrecords.c
"$(OBJ_PATH)\aaactivationfilename.obj"		: $(SYS_COMMON_PATH)\metadata\AudibleTargetCheck\aaactivationfilename.c

endif
