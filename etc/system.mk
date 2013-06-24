#This is the makefile for the system files.
#
#This file should be included into the project makefile and 
# $(SYSTEMLIB) must be included in the 
# dependency list of the .out file.

include $(ETC_PATH)\stdlib.mk
include $(ETC_PATH)\metadata.mk
include $(ETC_PATH)\resourcemanager.mk
#########################################
#
include $(ROOT)\etc\NANDDataDrive.mk
#########################################
#
include $(ROOT)\etc\NANDCommonDdi.mk
#########################################
#
include $(ROOT)\etc\NANDSystemDrive.mk
#########################################
#
include $(ROOT)\etc\NANDMedia.mk
#########################################
#
include $(ROOT)\etc\NANDHal.mk
#########################################
include $(ROOT)\etc\PortHil.mk

include $(ETC_PATH)\ddildl.mk

LOC_FILE_LIST = $(LOC_FILE_LIST)\
    $(ETC_PATH)\system.loc

OBJLIST =$(OBJLIST)\
    $(OBJ_PATH)\dbcs.obj\
    $(OBJ_PATH)\const.obj\
    $(OBJ_PATH)\mixer.obj\
    $(OBJ_PATH)\systemapi.obj\
    $(OBJ_PATH)\systemmodule.obj \
    $(OBJ_PATH)\sysusb.obj\
    $(OBJ_PATH)\syswaitonevent.obj\
    $(OBJ_PATH)\userkernel.obj\
    $(OBJ_PATH)\syscallfunction.obj\
    $(OBJ_PATH)\2400_syscallfunction.obj\
    $(OBJ_PATH)\sysmem.obj\
    $(OBJ_PATH)\sysmisc.obj\
    $(OBJ_PATH)\syscoder.obj\
    $(OBJ_PATH)\sysspeed.obj\
    $(OBJ_PATH)\steppingvoltageset.obj\
    $(OBJ_PATH)\sysirq.obj\
    $(OBJ_PATH)\syserror.obj\
    $(OBJ_PATH)\systimers.obj\
    $(OBJ_PATH)\SysExecSpeedClient.obj\
    $(OBJ_PATH)\bufdesc.obj\
    $(OBJ_PATH)\stack.obj\
    $(OBJ_PATH)\stackmem.obj\
    $(OBJ_PATH)\dacdrv.obj\
    $(OBJ_PATH)\dacisr.obj\
    $(OBJ_PATH)\copybuffer.obj\
    $(OBJ_PATH)\dacmemy.obj\
    $(OBJ_PATH)\copybufmem.obj\
    $(OBJ_PATH)\daccoefs.obj\
    $(OBJ_PATH)\dacmem.obj\
    $(OBJ_PATH)\fgndexec.obj\
    $(OBJ_PATH)\moduletable.obj\
    $(OBJ_PATH)\syssettings.obj\
    $(OBJ_PATH)\button.obj\
    $(OBJ_PATH)\lowresolutionadc.obj\
    $(OBJ_PATH)\lowresolutionadcinit.obj\
    $(OBJ_PATH)\playerlib.obj\
    $(OBJ_PATH)\playerlib_extra.obj\
    $(OBJ_PATH)\battery.obj\
    $(OBJ_PATH)\softtimers.obj\
    $(OBJ_PATH)\geq_overlay.obj\
    $(OBJ_PATH)\geq.obj\
    $(OBJ_PATH)\geqmem.obj\
    $(OBJ_PATH)\sysvolume.obj\
    $(OBJ_PATH)\memorysetup.obj\    
    $(OBJ_PATH)\syshwinit.obj\
    $(OBJ_PATH)\sysheadphones.obj\
    $(OBJ_PATH)\sysbrownout.obj\
    $(OBJ_PATH)\adcdrv.obj\
	$(OBJ_PATH)\adcmemx.obj\
	$(OBJ_PATH)\adcmemy.obj\
    $(OBJ_PATH)\sysheadphoneshort.obj 	
ifdef D_D3500
OBJLIST =$(OBJLIST)\
    $(OBJ_PATH)\persistbits.obj
endif

ifdef SPECTRUM_ANAL
OBJLIST =$(OBJLIST)\
	$(OBJ_PATH)\spectralanal.obj\
	$(OBJ_PATH)\spectralanalmem.obj
endif

MODULELIST = $(MODULELIST)\
    $(OUTPUT_RSRC_PATH)\Funclet_encodercommon.src\
    $(OUTPUT_RSRC_PATH)\Funclet_DBCSToUnicode.src\
    $(OUTPUT_RSRC_PATH)\Funclet_DBCSToUnicodeDstXSrcY.src\
    $(OUTPUT_RSRC_PATH)\Funclet_SteppingVoltageSet.src\
    $(OUTPUT_RSRC_PATH)\mixmod.src\
    $(OUTPUT_RSRC_PATH)\settings.src\
    $(OUTPUT_RSRC_PATH)\sysmod.src\
    $(OUTPUT_RSRC_PATH)\EncCommonp.src\
    $(OUTPUT_RSRC_PATH)\adc_adcx.src\
    $(OUTPUT_RSRC_PATH)\adc_adcy.src\
    $(OUTPUT_RSRC_PATH)\SoftTimerMod.src\
    $(OUTPUT_RSRC_PATH)\geqoverlay.src\
    $(OUTPUT_RSRC_PATH)\effectsmodules.src


######################################
#include the System Serial Number makefile
include $(RELBASE_PATH)\etc\sysserialnumber.mk
######################################

######################################
#include the System Serial Number makefile
include $(RELBASE_PATH)\etc\stackcheck.mk
######################################


#///////////////////////////////////////////////////////////////////////////////
# System Files
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\playerlib.obj : $(INC_PATH)\sysmem.h
$(OBJ_PATH)\playerlib.obj : $(INC_PATH)\types.h
$(OBJ_PATH)\playerlib.obj : $(INC_PATH)\playerlib.h
$(OBJ_PATH)\playerlib.obj : $(SYS_COMMON_PATH)\playerlib\playerlib.c
$(OBJ_PATH)\playerlib.obj : $(OUTPUT_INCLUDE_PATH)\sysspeed.h

$(OBJ_PATH)\playerlib_extra.obj : $(INC_PATH)\playerlib.h
$(OBJ_PATH)\playerlib_extra.obj : $(SYS_COMMON_PATH)\playerlib\playerlib_extra.c



ifndef SYSSPEED_H
SYSSPEED_H=$(HW_REGS_INC_PATH)\sysspeed.inc
endif

$(OUTPUT_INCLUDE_PATH)\sysspeed.h : $(SYSSPEED_H)
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OBJ_PATH)\sysheadphones.obj   : $(SYS_DEVICE_PATH)\sysheadphones.c

$(OBJ_PATH)\syshwinit.obj : $(SYS_DEVICE_PATH)\syshwinit.asm

$(OBJ_PATH)\sysbrownout.obj : $(SYS_DEVICE_PATH)\sysbrownout.asm 
$(OBJ_PATH)\sysbrownout.obj : $(COMMON_PROJ_PATH)\project.inc

$(OBJ_PATH)\sysheadphoneshort.obj : $(SYS_DEVICE_PATH)\sysheadphoneshort.asm

$(OBJ_PATH)\button.obj : $(DD_UI_PATH)\buttons\button.asm $(PROJ_PATH)\resource.inc

$(OBJ_PATH)\mixer.obj : $(HW_MSGMODULES_PATH)\mixer\mixer.asm $(PROJ_PATH)\resource.inc
  
$(OBJ_PATH)\sysresource_nand_dd.obj : $(DD_SM_SYS)\sysresource_nand_dd.asm

$(OBJ_PATH)\sysusb.obj : $(SYS_COMMON_PATH)\sysusb.asm

$(OBJ_PATH)\sysmem.obj : $(SYS_COMMON_PATH)\sysmem.asm

$(OBJ_PATH)\sysmisc.obj : $(SYS_COMMON_PATH)\sysmisc.asm

$(OBJ_PATH)\syscoder.obj : $(SYS_COMMON_PATH)\syscoder.asm

$(OBJ_PATH)\syserror.obj : $(SYS_COMMON_PATH)\syserror.asm

$(OBJ_PATH)\sysspeed.obj : $(SYS_DEVICE_PATH)\sysspeed.asm $(HW_REGS_INC_PATH)\sysspeed.inc $(SYS_DEVICE_PATH)\steppingvoltageset.c 

$(OBJ_PATH)\steppingvoltageset.obj : $(SYS_DEVICE_PATH)\steppingvoltageset.c $(INC_PATH)\steppingvoltageset.h $(INC_PATH)\3500\regsdcdc.inc $(SYS_COMMON_PATH)\systime.asm $(SYS_COMMON_PATH)\systimers.asm

$(OBJ_PATH)\systime.obj : $(SYS_COMMON_PATH)\systime.asm

$(OBJ_PATH)\sysirq.obj : $(SYS_COMMON_PATH)\sysirq.asm 

$(OBJ_PATH)\systimers.obj : $(SYS_COMMON_PATH)\systimers.asm

$(OBJ_PATH)\bufdesc.obj : $(SYS_COMMON_PATH)\bufdesc.asm

$(OBJ_PATH)\stack.obj : $(SYS_PATH)\data_management\stack\stack.asm
																

$(OBJ_PATH)\stackmem.obj : $(SYS_COMMON_PATH)\stackmem.asm $(COMMON_PROJ_PATH)\project.inc

$(OBJ_PATH)\const.obj : $(SYS_PATH)\data_management\const.asm

$(OBJ_PATH)\sysexecspeedclient.obj : $(SYS_EXEC_PATH)\sysexecspeedclient.asm

$(OBJ_PATH)\mediadetection.obj : $(SYS_COMMON_PATH)\mediadetection.c   
$(OBJ_PATH)\mediadetection.obj : $(COMMON_PROJ_PATH)\project.inc

$(OBJ_PATH)\dacmemy.obj        :   $(SYS_PATH)\fgndmodules\dacmemy.asm

$(OBJ_PATH)\copybuffer.obj     :   $(SYS_PATH)\fgndmodules\copybuffer.asm

$(OBJ_PATH)\copybufmem.obj     :   $(SYS_PATH)\fgndmodules\copybufmem.asm

$(OBJ_PATH)\daccoefs.obj       :   $(SYS_PATH)\fgndmodules\daccoefs.asm

$(OBJ_PATH)\dacdrv.obj         :   $(SYS_PATH)\fgndmodules\dacdrv.asm

$(OBJ_PATH)\dacisr.obj         :   $(SYS_PATH)\fgndmodules\dacisr.asm

$(OBJ_PATH)\dacmem.obj         :   $(SYS_PATH)\fgndmodules\dacmem.asm

$(OBJ_PATH)\fgndexec.obj       :   $(SYS_PATH)\fgndmodules\fgndexec.asm

$(OBJ_PATH)\moduletable.obj    :   $(SYS_PATH)\fgndmodules\moduletable.asm

$(OBJ_PATH)\adcdrv.obj         :   $(ROOT)\DeviceDriver\Analog\Adc\adcdrv.asm
$(OBJ_PATH)\adcmemx.obj : 	$(ADC_PATH)\adcmemx.asm 
$(OBJ_PATH)\adcmemy.obj :   $(ADC_PATH)\adcmemy.asm

$(OBJ_PATH)\syssettings.obj : $(SYS_COMMON_PATH)\syssettings.asm 
$(OBJ_PATH)\syssettings.obj : $(PROJ_PATH)\resource.inc 
$(OBJ_PATH)\syssettings.obj : $(COMMON_PROJ_PATH)\project.inc

$(OBJ_PATH)\systemapi.obj : $(SW_MSGMODULES_PATH)\system\systemapi.asm 
$(OBJ_PATH)\systemapi.obj : $(PROJ_PATH)\resource.inc

$(OBJ_PATH)\systemmodule.obj : $(SW_MSGMODULES_PATH)\system\systemmodule.c

$(OBJ_PATH)\lowresolutionadcinit.obj : $(SYS_DEVICE_PATH)\lowresolutionadcinit.c
$(OBJ_PATH)\lowresolutionadc.obj : $(SYS_DEVICE_PATH)\lowresolutionadc.c

$(OBJ_PATH)\battery.obj : $(SYS_DEVICE_PATH)\battery.c  
$(OBJ_PATH)\battery.obj : $(PROJECT_H)

$(OBJ_PATH)\sysvolume.obj : $(SYS_COMMON_PATH)\sysvolume.c 
$(OBJ_PATH)\sysvolume.obj : $(PROJECT_H)

$(OBJ_PATH)\dbcs.obj: $(SYS_COMMON_PATH)\dbcs.c 
$(OBJ_PATH)\dbcs.obj: $(OUTPUT_INCLUDE_PATH)\resource.h 
$(OBJ_PATH)\dbcs.obj: $(OUTPUT_INCLUDE_PATH)\sysequ.h 
$(OBJ_PATH)\dbcs.obj: $(OUTPUT_INCLUDE_PATH)\sysresources.h

$(OBJ_PATH)\geq.obj             :   $(SYS_PATH)\MsgModules\Software\Effects\GraphicEq\geq.asm
$(OBJ_PATH)\geqmem.obj          :   $(SYS_PATH)\MsgModules\Software\Effects\GraphicEq\geqmem.asm
$(OBJ_PATH)\geq_overlay.obj     :   $(SYS_PATH)\MsgModules\Software\Effects\GraphicEq\geq_overlay.asm  $(COMMON_PROJ_PATH)\project.inc

ifdef SPECTRUM_ANAL
$(OBJ_PATH)\spectralanal.obj	:   $(SYS_PATH)\MsgModules\Software\effects\spectralanal\spectralanal.asm
$(OBJ_PATH)\spectralanalmem.obj	:   $(SYS_PATH)\MsgModules\Software\effects\spectralanal\spectralanalmem.asm
endif

$(OBJ_PATH)\softtimers.obj  : $(SW_MSGMODULES_PATH)\softtimer\softtimers.c $(PROJ_PATH)\resource.inc 
$(OBJ_PATH)\softtimers.obj  : $(OUTPUT_INCLUDE_PATH)\messages.h 
$(OBJ_PATH)\softtimers.obj  : $(PROJECT_H)

$(OBJ_PATH)\persistbits.obj 	:   $(SYS_DEVICE_PATH)\persistbits.c
$(OBJ_PATH)\persistbits.obj 	:   $(INC_PATH)\persist_bits.h 
$(OBJ_PATH)\persistbits.obj 	:   $(PROJECT_H)


$(OUTPUT_INCLUDE_PATH)\adpcmequ.h:$(INC_PATH)\adpcmequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_INCLUDE_PATH)\smvadpcmequ.h:$(INC_PATH)\smvadpcmequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 


#///////////////////////////////////////////////////////////////////////////////
# Menu Kernel
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\syswaitonevent.obj    : $(SYS_EXEC_PATH)\c_menu_kernel\syswaitonevent.asm 
$(OBJ_PATH)\syswaitonevent.obj    : $(PROJ_PATH)\resource.inc
$(OBJ_PATH)\syswaitonevent.obj    : $(INC_PATH)\msgequ.inc
$(OBJ_PATH)\syswaitonevent.obj    : $(SYS_COMMON_PATH)\sysmacro.asm

$(OBJ_PATH)\userkernel.obj        : $(SYS_EXEC_PATH)\c_menu_kernel\userkernel.asm 
$(OBJ_PATH)\userkernel.obj        : $(PROJ_PATH)\resource.inc
$(OBJ_PATH)\userkernel.obj        : $(INC_PATH)\msgequ.inc

$(OBJ_PATH)\syscallfunction.obj   : $(SYS_EXEC_PATH)\c_menu_kernel\syscallfunction.asm 
$(OBJ_PATH)\syscallfunction.obj   : $(SYS_COMMON_PATH)\sysmacro.asm
$(OBJ_PATH)\syscallfunction.obj   : $(INC_PATH)\msgequ.inc
$(OBJ_PATH)\syscallfunction.obj   : $(PROJ_PATH)\resource.inc

$(OBJ_PATH)\memorysetup.obj         : $(PROJ_PATH)\memorysetup.c

$(OBJ_PATH)\2400_syscallfunction.obj : $(SYS_COMMON_PATH)\2400_syscallfunction.asm
$(OBJ_PATH)\2400_syscallfunction.obj      : $(SYS_COMMON_PATH)\sysmacro.asm

#$(OBJ_PATH)\system_resource.obj     : $(SYS_COMMON_PATH)\symbols\system_resource.asm

#///////////////////////////////////////////////////////////////////////////////
# System Overlays -- Resource extraction
#///////////////////////////////////////////////////////////////////////////////

"$(OUTPUT_RSRC_PATH)\EncCommonp.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\adc_adcx.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\adc_adcy.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo "Extract Adc X & Adc Y"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\enccommon.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\


# Geq module & code
"$(OUTPUT_RSRC_PATH)\geqoverlay.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Geq Overlay Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=GeqOverlay_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map


"$(OUTPUT_RSRC_PATH)\mixmod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Mixer Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MIXMOD_Mixer_overlay_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\sysmod.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SystemApi_P_overlay,.ptextsystemmodule -f$(OUTPUT_PATH)\$(PROJ).map
        
"$(OUTPUT_RSRC_PATH)\effectsmodules.src" : $(OUTPUT_PATH)\$(PROJ).abs        
        @echo "Extract Effects Modules"
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\effectsmodules.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\
        

# Settings commands
"$(OUTPUT_RSRC_PATH)\settings.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Settings Module
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -C$@=Settings_Y 

"$(OUTPUT_RSRC_PATH)\Funclet_DBCSToUnicode.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_DBCSToUnicode 
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_DBCS_TO_UNICODE_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_DBCSToUnicodeDstXSrcY.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_DBCSToUnicodeDstXSrcY
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_DBCS_TO_UNICODE_DSTX_SRCY_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_encodercommon.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_EncoderCommon
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_LoadEncoderCommon_P -f$(OUTPUT_PATH)\$(PROJ).map
