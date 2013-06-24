#########################################
# SDK ButtonTest makefile
# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
#########################################

PROJ                = buttontest

# relative pointer to SDK root directory \Mp3Firmware
RELBASE_PATH        = ..\..\..
ROOT                = $(RELBASE_PATH)

# these macros are project dependent
PROJ_PATH           = $(ROOT)\test\$(PROJ)
PROJSUPPORT_PATH    = $(ROOT)\test\testsupport
PROJSYS_PATH        = $(ROOT)\test\testsupport\system
PROJRSRC_PATH       = $(ROOT)\test\testsupport\rsrc
PROJECT_H           = $(OUTPUT_INCLUDE_PATH)\project.h
HWEQU_H             = $(OUTPUT_INCLUDE_PATH)\hwequ.h

## PLAYER ###############################
# Distinguish between Player and MTP/hostlink 
BLD=$(BLD) -DSTMP_BUILD_PLAYER
STMP_BUILD_PLAYER=TRUE
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSTMP_BUILD_PLAYER
PLAYER=TRUE
ifdef PLAYER
BLD=$(BLD) -DPLAYER
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DPLAYER
endif
#########################################

#########################################
include $(ROOT)\etc\standard.mk
#########################################

#########################################
include $(ROOT)\etc\registers.mk
#########################################

#########################################
include $(RELBASE_PATH)\etc\stringlib.mk
#########################################

#########################################
include $(ROOT)\etc\ddildl.mk
#########################################

#########################################
include $(RELBASE_PATH)\etc\sysserialnumber.mk
#########################################

#########################################
include $(ROOT)\etc\NANDMedia.mk
#########################################

#########################################
include $(ROOT)\etc\NANDDataDrive.mk
#########################################

#########################################
include $(ROOT)\etc\NANDCommonDdi.mk
#########################################

#########################################
include $(ROOT)\etc\NANDSystemDrive.mk
#########################################

#########################################
include $(ROOT)\etc\NANDHal.mk
#########################################

#########################################
include $(ROOT)\etc\PortHil.mk
#########################################

#########################################
include $(ROOT)\etc\RAMDataDrive.mk
#########################################

#########################################
include $(ROOT)\etc\RAMSystemDrive.mk
#########################################

#########################################
include $(ROOT)\etc\RAMMedia.mk
#########################################

#########################################
include $(ROOT)\etc\RAMHal.mk
#########################################

#########################################
# This is needed by resouremanager. 
$(OBJ_PATH)\reentrant.obj : $(ROOT)\FileSystem\reentrant.asm 

include $(ETC_PATH)\resourcemanager.mk
#########################################

#########################################
include $(ETC_PATH)\stdlib.mk
#########################################

#########################################
#Display module
include $(RELBASE_PATH)\etc\display.mk
include $(RELBASE_PATH)\etc\epson_sed15xx.mk
include $(RELBASE_PATH)\etc\framebuffer.mk
#include $(RELBASE_PATH)\etc\directtolcd.mk
#########################################

# must follow standard.mk, where base INC_ALL gets defined
INC_ALL=-I$(PROJRSRC_PATH);$(PROJ_PATH);$(PROJSYS_PATH);$(OUTPUT_PATH);$(INC_ALL);$(PROJSUPPORT_PATH)
INC_ALL_C = $(INC_ALL_C) -Wc-I$(PROJ_PATH) -Wc-I$(OUTPUT_INCLUDE_PATH) -Wc-I$(INC_PATH) -Wc-I$(SYS_COMMON_PATH) -Wc-I$(PROJSUPPORT_PATH)

### $(BLD) is environment variable; set in batch file
AS_OPTS=$(INC_ALL) -OJMPS  -l -LCeIn -DTASKING $(BLD) 
LK_OPTS= -L$(LIB_PATH) -L$(MP3_PATH)\tasking -L$(BIN_DRIVE)\2410\lib\2410 -w8
CC_OPTS= -Wc-si $(BLD) -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL)

LIBLIST = $(BIN_DRIVE)\c2410\lib\2410\librt.a\
		  $(WMA_LIB_PATH)\wmalibNoDRM.a

HWEQUCOMMON = $(HWEQUCOMMON)\
			$(OUTPUT_INCLUDE_PATH)\hwequ.h\
			$(OUTPUT_INCLUDE_PATH)\regsclkctrl.h\
			$(OUTPUT_INCLUDE_PATH)\regscore.h\
  			$(OUTPUT_INCLUDE_PATH)\regscodec.h\
  			$(OUTPUT_INCLUDE_PATH)\regsdcdc.h\
  			$(OUTPUT_INCLUDE_PATH)\regsemc.h\
  			$(OUTPUT_INCLUDE_PATH)\regsgpio.h\
  			$(OUTPUT_INCLUDE_PATH)\regsi2c.h\
  			$(OUTPUT_INCLUDE_PATH)\regsi2s.h\
  			$(OUTPUT_INCLUDE_PATH)\regsicoll.h\
  			$(OUTPUT_INCLUDE_PATH)\regslradc.h\
  			$(OUTPUT_INCLUDE_PATH)\regsrevision.h\
  			$(OUTPUT_INCLUDE_PATH)\regsrtc.h\
  			$(OUTPUT_INCLUDE_PATH)\regssdram.h\
  			$(OUTPUT_INCLUDE_PATH)\regsspare.h\
  			$(OUTPUT_INCLUDE_PATH)\regsspi.h\
  			$(OUTPUT_INCLUDE_PATH)\regsswizzle.h\
  			$(OUTPUT_INCLUDE_PATH)\regstb.h\
			$(OUTPUT_INCLUDE_PATH)\regstimer.h\

#Create hwequ list based on device
ifdef D_D3500
HWEQULIST = $(HWEQULIST)\
			$(HWEQUCOMMON)\
  			$(OUTPUT_INCLUDE_PATH)\regsecc.h\
  			$(OUTPUT_INCLUDE_PATH)\regsgpflash.h\
  			$(OUTPUT_INCLUDE_PATH)\regsfilco.h\
  			$(OUTPUT_INCLUDE_PATH)\regspwm.h\
  			$(OUTPUT_INCLUDE_PATH)\regsusb20.h\
	  		$(OUTPUT_INCLUDE_PATH)\regsusb20phy.h
else
#3410 list
HWEQULIST = $(HWEQULIST)\
			$(HWEQUCOMMON)\
			$(OUTPUT_INCLUDE_PATH)\regsusb.h
endif

OBJLIST =   $(OBJLIST) \
            $(OBJ_PATH)\sysmem.obj\
            $(OBJ_PATH)\syserror.obj\
            $(OBJ_PATH)\sysmisc.obj\
            $(OBJ_PATH)\systime.obj\
            $(OBJ_PATH)\sysirq.obj\
            $(OBJ_PATH)\systimers.obj\
            $(OBJ_PATH)\sysspeed.obj\
            $(OBJ_PATH)\steppingvoltageset.obj\
            $(OBJ_PATH)\bufdesc.obj\
            $(OBJ_PATH)\stack.obj\
            $(OBJ_PATH)\stackmem.obj\
            $(OBJ_PATH)\syspostmsg.obj\
            $(OBJ_PATH)\buttontest.obj\
            $(OBJ_PATH)\const.obj\
            $(OBJ_PATH)\button.obj\
            $(OBJ_PATH)\modulemsg.obj\
            $(OBJ_PATH)\utilasm.obj\
            $(OBJ_PATH)\sysmodule4test.obj\
            $(OBJ_PATH)\lcdsupport.obj\
            $(OBJ_PATH)\dbcs.obj\
            $(OBJ_PATH)\divide.obj\
            $(OBJ_PATH)\incr.obj\
            $(OBJ_PATH)\packed_get.obj\
            $(OBJ_PATH)\bytearray.obj\
            $(OBJ_PATH)\kernel_syswaitonobject.obj\
            $(OBJ_PATH)\getbytearrayy.obj\
            $(OBJ_PATH)\getmessage.obj\
            $(OBJ_PATH)\putbytearrayy.obj\
            $(OBJ_PATH)\packed_strlen.obj\
            $(OBJ_PATH)\resourcey.obj\
            $(OBJ_PATH)\lowresolutionadc.obj\
            $(OBJ_PATH)\lowresolutionadcinit.obj\
            $(OBJ_PATH)\kernel_getregion.obj\
            $(OBJ_PATH)\kernel_contextswitch.obj\
            $(OBJ_PATH)\kernel_settimeout.obj \
            $(OBJ_PATH)\overlay_istaskloaded.obj\
            $(OBJ_PATH)\kernel_sysreleaseobject.obj\
            $(OBJ_PATH)\miscproject.obj\
            $(OBJ_PATH)\memorysetup.obj\
			$(OBJ_PATH)\2400_syscallfunction.obj\
            $(OBJ_PATH)\persistbits.obj\
            $(OBJ_PATH)\reentrant.obj 


ALL : $(OUTPUT_PATH)\resource.asm $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).s

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).out" : .\stmp3500.dsc $(HWEQULIST) $(OBJLIST) $(ALLLIBLIST) $(PROJ).mk $(PROJECT_H) $(LIBLIST)
    @echo Linking $(PROJ).out
         $(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(match .a $!) $(match .sym $!)

"$(OUTPUT_PATH)\$(PROJ).abs"            : "$(OUTPUT_PATH)\$(PROJ).out" .\stmp3500.dsc 
    @echo Creating IEEE-695 absolute file $@
        @$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out

#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////
$(OUTPUT_PATH)\resource.asm : $(PROJ_PATH)\resource.inc
    @echo Creating $@
    @$(BIN_PATH)\rsclink.EXE -h$(PROJ_PATH)\resource.inc \
    -i$(ROOT)\fonts\iso8859\
    -i$(RESOURCE_PATH)\shingyih\New_Type_LCD \
    -i$(ROOT)\fonts\iso8859\sigmatel_8 \
    -i$(OUTPUT_RSRC_PATH) -i$(PROJRSRC_PATH) \
    -o$(OUTPUT_PATH)\resource.bin \
    -v$(VERSION) -a 
    @$(BIN_PATH)\resparse $(OUTPUT_PATH)\resource.bin $(OUTPUT_PATH)\resource.txt

#///////////////////////////////////////////////////////////////////////////////
# Project-specific files
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\buttontest.obj" : $(PROJ_PATH)\buttontest.c  $(OUTPUT_PATH)\resource.asm $(OUTPUT_INCLUDE_PATH)\messages.h 
$(OBJ_PATH)\buttontest.obj  :  $(SYS_COMMON_PATH)\syserror.asm
$(OBJ_PATH)\syserror.obj    :  $(SYS_COMMON_PATH)\syserror.asm


"$(OBJ_PATH)\modulemsg.obj" : $(PROJ_PATH)\modulemsg.asm

$(OBJ_PATH)\button.obj : $(DD_UI_PATH)\buttons\button.asm

$(OBJ_PATH)\lowresolutionadc.obj:  $(SYS_DEVICE_PATH)\lowresolutionadc.c
$(OBJ_PATH)\lowresolutionadcinit.obj : $(SYS_DEVICE_PATH)\lowresolutionadcinit.c

$(OBJ_PATH)\overlay_syscallfunction.obj  : $(SYS_PATH)\kernel\overlay_syscallfunction.asm

$(OBJ_PATH)\kernel_getregion.obj        : $(SYS_PATH)\kernel\kernel_getregion.asm 

$(OBJ_PATH)\kernel_contextswitch.obj    : $(SYS_PATH)\kernel\kernel_contextswitch.asm

$(OBJ_PATH)\kernel_settimeout.obj       : $(SYS_PATH)\kernel\kernel_settimeout.c

$(OBJ_PATH)\overlay_istaskloaded.obj  : $(SYS_PATH)\kernel\overlay_istaskloaded.c

$(OBJ_PATH)\kernel_sysreleaseobject.obj : $(SYS_PATH)\kernel\kernel_sysreleaseobject.c

"$(OBJ_PATH)\getmessage.obj" : $(SYS_COMMON_PATH)\getmessage.asm

$(OBJ_PATH)\sysmodule4test.obj : $(SYS_EXEC_PATH)\sysmodule4test.asm

$(OBJ_PATH)\syspostmsg.obj : $(SYS_COMMON_PATH)\syspostmsg.asm

$(OBJ_PATH)\systime.obj : $(SYS_COMMON_PATH)\systime.asm
$(OBJ_PATH)\systime.obj : $(PROJSUPPORT_PATH)\systimers.inc

$(OBJ_PATH)\sysspeed.obj : $(SYS_DEVICE_PATH)\sysspeed.asm $(PROJ_PATH)\project.inc

$(OBJ_PATH)\steppingvoltageset.obj : $(SYS_DEVICE_PATH)\steppingvoltageset.c

$(OBJ_PATH)\const.obj : $(SYS_PATH)\data_management\const.asm

$(OBJ_PATH)\bufdesc.obj : $(SYS_COMMON_PATH)\bufdesc.asm

$(OBJ_PATH)\utilasm.obj : $(PROJSYS_PATH)\utilasm.asm

$(OBJ_PATH)\resourcey.obj : $(PROJSYS_PATH)\resourcey.asm

$(OBJ_PATH)\lcdsupport.obj : $(PROJSUPPORT_PATH)\lcdsupport.c

$(OBJ_PATH)\sysmem.obj : $(SYS_COMMON_PATH)\sysmem.asm

$(OBJ_PATH)\sysmisc.obj : $(SYS_COMMON_PATH)\sysmisc.asm

$(OBJ_PATH)\stack.obj : $(SYS_PATH)\data_management\stack\stack.asm

$(OBJ_PATH)\packed_get.obj : $(SYS_COMMON_PATH)\stringlib\packed_get.asm

$(OBJ_PATH)\bytearray.obj  : $(SYS_COMMON_PATH)\stringlib\bytearray.asm

$(OBJ_PATH)\kernel_syswaitonobject.obj  : $(SYS_PATH)\kernel\kernel_syswaitonobject.asm

$(OBJ_PATH)\getbytearrayy.obj:          $(SYS_COMMON_PATH)\stringlib\getbytearrayy.asm
$(OBJ_PATH)\putbytearrayy.obj:          $(SYS_COMMON_PATH)\stringlib\putbytearrayy.asm
$(OBJ_PATH)\packed_strlen.obj:          $(SYS_COMMON_PATH)\stringlib\packed_strlen.asm

$(OBJ_PATH)\divide.obj : "$(PROJSYS_PATH)\divide.asm"

$(OBJ_PATH)\incr.obj : "$(PROJSYS_PATH)\incr.asm"

$(OBJ_PATH)\stackmem.obj : $(SYS_COMMON_PATH)\stackmem.asm

$(OBJ_PATH)\sysirq.obj : $(SYS_COMMON_PATH)\sysirq.asm

$(OBJ_PATH)\systimers.obj : $(SYS_COMMON_PATH)\systimers.asm

$(OBJ_PATH)\dbcs.obj: $(SYS_COMMON_PATH)\dbcs.c $(OUTPUT_INCLUDE_PATH)\resource.h $(OUTPUT_INCLUDE_PATH)\sysequ.h $(OUTPUT_INCLUDE_PATH)\sysresources.h

$(OBJ_PATH)\2400_syscallfunction.obj : "$(SYS_COMMON_PATH)\2400_syscallfunction.asm"

"$(OBJ_PATH)\miscproject.obj" : $(PROJ_PATH)\miscproject.c  $(OUTPUT_INCLUDE_PATH)\errors.h

$(OBJ_PATH)\memorysetup.obj : $(PROJ_PATH)\memorysetup.c

$(OBJ_PATH)\cfporthil_resource.obj : $(SYMBOLS_PATH)\cfporthil_resource.asm

$(OBJ_PATH)\persistbits.obj :  $(SYS_DEVICE_PATH)\persistbits.c
$(OBJ_PATH)\persistbits.obj :  $(INC_PATH)\persist_bits.h

"$(OUTPUT_INCLUDE_PATH)\messages.h":$(INC_PATH)\msgequ.inc 
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_INCLUDE_PATH)\resource.h:$(PROJ_PATH)\resource.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_INCLUDE_PATH)\sysequ.h:$(INC_PATH)\sysequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_INCLUDE_PATH)\sysresources.h:$(INC_PATH)\sysresources.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(PROJECT_H) :$(ROOT)\inc\project.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

$(HWEQU_H) : $(HW_REGS_INC_PATH)\hwequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

$(OUTPUT_INCLUDE_PATH)\errors.h :$(INC_PATH)\errors.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 


#///////////////////////////////////////////////////////////////////////////////
# Module code extraction
#///////////////////////////////////////////////////////////////////////////////

"$(OUTPUT_PATH)\$(PROJ).s" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System S-rec for $(DEVICE) device family
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -S$@=P_clstr,X_clstr,Y_clstr -V$(VERSION) -OFFE8=1818 -OFFE9=1818 
        @$(BIN_PATH)\stosbcon $(OUTPUT_PATH)\buttontest.s $(OUTPUT_PATH)\stmpsys.sb
        




