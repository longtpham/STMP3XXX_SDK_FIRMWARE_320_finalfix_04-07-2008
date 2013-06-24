AUDIBLE_MTP_PATH      =  $(ROOT)\system\common\metadata\AudibleMtp

AUD_PATH			= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\AASource
AUD_PATH2			= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\AADevProject
AUD_INC1			= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\IncludePublic
ACELP_LIB_PATH	= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp\Audible_Acelp_Src\ACELP
AUD_ACELP_PATH		= ..\..\..\..\..\Algorithms\Decoders\AudibleAcelp
AUD_ACELP_WRAPPER_PATH = ..\..\..\..\..\System\MsgModules\Software\Decoder\AudibleAcelp

INC_ALL =       $(INC_ALL);$(AUDIBLE_MTP_PATH);$(AUD_PATH);$(AUD_PATH2);$(AUD_INC1);$(ACELP_LIB_PATH);$(AUD_ACELP_WRAPPER_PATH)
INC_ALL_C =     $(INC_ALL_C) -Wc-I$(AUDIBLE_MTP_PATH) -Wc-I$(AUD_PATH) -Wc-I$(AUD_PATH2) -Wc-I$(AUD_INC1) -Wc-I$(ACELP_LIB_PATH) -Wc-I$(AUD_ACELP_WRAPPER_PATH)

####### If a non-audible build only add the handler files ######
ifndef D_AUDIBLEDEC
OBJLIST =$(OBJLIST)\
    $(OBJ_PATH)\AudibleObjectPropHandler.obj\
    $(OBJ_PATH)\AudibleDevicePropertyHandler.obj\
    $(OBJ_PATH)\audible_resource.obj

RSRCLIST = $(RSRCLIST)\
	$(OUTPUT_RSRC_PATH)\audiblemtpmetadata.src\
    $(OUTPUT_RSRC_PATH)\audibleobjecthandler.src\
    $(OUTPUT_RSRC_PATH)\audibledevicehandler.src

"$(OUTPUT_RSRC_PATH)\audiblemtpmetadata.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract NULL audiblemtpmetadata.src 
	    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\audiblemtpmetadata.src

"$(OUTPUT_RSRC_PATH)\audibleobjecthandler.src"		 : $(OUTPUT_PATH)\$(PROJ).abs
	@$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextaudibleobjectprophandler -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\audibledevicehandler.src"		 : $(OUTPUT_PATH)\$(PROJ).abs
	@$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=.ptextaudibledevicepropertyhandler -F$(OUTPUT_PATH)\$(PROJ).map

"$(OBJ_PATH)\AudibleDevicePropertyHandler.obj" : "$(AUDIBLE_MTP_PATH)\AudibleDevicePropertyHandler.c" 
"$(OBJ_PATH)\AudibleObjectPropHandler.obj"  : "$(AUDIBLE_MTP_PATH)\AudibleObjectPropHandler.c"
$(OBJ_PATH)\audible_resource.obj : $(SYMBOLS_PATH)\audible_resource.asm

else
############## Add all Audible files #####################
RSRCLIST = $(RSRCLIST)\
	$(OUTPUT_RSRC_PATH)\audiblemtpmetadata.src\
    $(OUTPUT_RSRC_PATH)\audibleobjecthandler.src\
    $(OUTPUT_RSRC_PATH)\audibledevicehandler.src			


OBJLIST =$(OBJLIST)\
	$(OBJ_PATH)\aamtpaccess.obj\
    $(OBJ_PATH)\aamtpsongposition.obj\
    $(OBJ_PATH)\mtp_audiblemetadata.obj\
    $(OBJ_PATH)\AudibleDevicePropertyHandler.obj\
	$(OBJ_PATH)\AAActivationRecords.obj\
	$(OBJ_PATH)\AudibleSetDevicePropValue.obj\
    $(OBJ_PATH)\AudibleObjectPropHandler.obj\
    $(OBJ_PATH)\audible_resource.obj
    
$(OUTPUT_RSRC_PATH)\audiblemtpmetadata.src      : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audible_mtp.loc
$(OUTPUT_RSRC_PATH)\audibleobjecthandler.src    : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audible_mtp.loc
$(OUTPUT_RSRC_PATH)\audibledevicehandler.src    : $(OUTPUT_PATH)\$(PROJ).abs $(ETC_PATH)\audible_mtp.loc
 
        @echo Extract MetaData codebanks
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\


$(OBJ_PATH)\aamtpaccess.obj      : $(AUDIBLE_MTP_PATH)\aamtpaccess.c
$(OBJ_PATH)\aamtpsongposition.obj  :  $(AUDIBLE_MTP_PATH)\aamtpsongposition.c
$(OBJ_PATH)\mtp_audiblemetadata.obj      : $(AUDIBLE_MTP_PATH)\mtp_audiblemetadata.c  
"$(OBJ_PATH)\AudibleDevicePropertyHandler.obj" : "$(AUDIBLE_MTP_PATH)\AudibleDevicePropertyHandler.c" 
"$(OBJ_PATH)\AAActivationRecords.obj" : "$(AUDIBLE_MTP_PATH)\AAActivationRecords.c" 
"$(OBJ_PATH)\AudibleSetDevicePropValue.obj" : "$(AUDIBLE_MTP_PATH)\AudibleSetDevicePropValue.c" 
"$(OBJ_PATH)\AudibleObjectPropHandler.obj"  : "$(AUDIBLE_MTP_PATH)\AudibleObjectPropHandler.c"
$(OBJ_PATH)\audible_resource.obj : $(SYMBOLS_PATH)\audible_resource.asm 

endif