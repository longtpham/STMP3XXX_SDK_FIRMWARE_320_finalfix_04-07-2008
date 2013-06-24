#This is needed to force the SmartMedia DDI MediaInit routine to count the number
#   of hidden system blocks on the boot media
BLD=$(BLD) -DMEDIA_DDI_COUNT_HIDDEN_SYSTEM_BLOCKS
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DMEDIA_DDI_COUNT_HIDDEN_SYSTEM_BLOCKS

#Disable SmartMedia ECC on reads
BLD=$(BLD) -DSMNOECCR -DFAT32TEST
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSMNOECCR -Wa-DFAT32TEST
D_SMNOECCR=TRUE
MAKE_FAT32TEST=TRUE

# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
PROJ                = fat32test
# relative pointer to SDK root directory \Mp3Firmware
RELBASE_PATH        = ..\..\..
ROOT                = $(RELBASE_PATH)

# these macros are project dependent
PROJ_PATH           = $(ROOT)\test\$(PROJ)
PROJSUPPORT_PATH    = $(ROOT)\test\testsupport
PROJSYS_PATH        = $(PROJSUPPORT_PATH)\system
PROJRSRC_PATH       = $(PROJSUPPORT_PATH)\rsrc
SDK_INC             = $(ROOT)\INC
PROJECT_H           = $(OUTPUT_INCLUDE_PATH)\project.h
HWEQU_H           = $(OUTPUT_INCLUDE_PATH)\hwequ.h

PROFILE_PATH      = $(ROOT)\Projects\SDK\lcdexample\mtp

INC_ALL_C =\
  $(INC_ALL_C)\
  -Wc-I$(PROFILE_PATH)
#INC_ALL_C = $(INC_ALL_C) -Wc-I$(INC_FS)
################################################################################
CMP_PATH = $(FAT32_PATH)\cmp
CMP_STDTYPE_PATH = $(CMP_PATH)\cmp_stdtype
CMP_DEBUG_PATH = $(CMP_PATH)\cmp_debug
CMP_FILE_SYSTEM_PATH = $(CMP_PATH)\cmp_file_system
CMP_MEDIA_NAND_PATH = $(ROOT)\DeviceDriver\Media\cmp\cmp_media_nand

INC_ALL_C =\
  $(INC_ALL_C)\
  -Wc-I$(INC_FS)\
  -Wc-I$(CMP_STDTYPE_PATH)\
  -Wc-I$(CMP_DEBUG_PATH)\
  -Wc-I$(CMP_FILE_SYSTEM_PATH)\
  -Wc-I$(CMP_MEDIA_NAND_PATH)

C_ASM_OPTS=$(C_ASM_OPTS) -DFAT32TEST -Wa-DFAT32TEST
# I notice -DFAT32TEST on line above is different than typical -Wa for asm def or -Wc for C def. Passes ok to compiler. 

################################################################################

#########################################
include $(ROOT)\etc\standard.mk
#########################################
include $(ROOT)\etc\registers.mk
######################################
include $(RELBASE_PATH)\etc\stringlib.mk
######################################

include $(RELBASE_PATH)\etc\interrupt.mk

#########################################
include $(ROOT)\etc\ddildl.mk
#########################################

###################################### 
#removed and used stub var in main test prog to free p space. other utils do this too. 
#include $(RELBASE_PATH)\etc\sysserialnumber.mk
######################################

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
######################################

#########################################
include $(ROOT)\etc\MMCMedia.mk
#########################################

#########################################
include $(ROOT)\etc\MMCDataDrive.mk
#########################################

######################################
include $(ETC_PATH)\resourcemanager.mk
######################################

include $(ETC_PATH)\stdlib.mk

#########################################
include $(ROOT)\etc\fat32.mk
#########################################

######################################
include $(RELBASE_PATH)\etc\display.mk
include $(RELBASE_PATH)\etc\epson_sed15xx.mk
include $(RELBASE_PATH)\etc\framebuffer.mk
######################################

# must follow standard.mk, where base INC_ALL gets defined
INC_ALL=  -I$(PROJ_PATH);$(PROJRSRC_PATH);$(PROJSYS_PATH);$(OUTPUT_PATH);$(INC_ALL);$(PROJSUPPORT_PATH) 
INC_ALL_C = $(INC_ALL_C) -Wc-I$(PROJ_PATH) -Wc-I$(OUTPUT_INCLUDE_PATH) -Wc-I$(PROJSYS_PATH) -Wc-I$(INC_PATH) -Wc-I$(SYS_COMMON_PATH) -Wc-I$(PROJSUPPORT_PATH)


#### REMAP PROJECT.INC for system makefiles #####
COMMON_PROJ_PATH = ..\

### $(BLD) is environment variable; set in batch file
AS_OPTS=$(INC_ALL) -OJMPS  -l -LCeIn -DTASKING $(BLD) 
LK_OPTS= -L$(LIB_PATH) -L$(MP3_PATH)\tasking -L$(BIN_DRIVE)\2410\lib\2410 -w8


CC_OPTS= -Wc-si $(BLD) -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL) $(C_ASM_OPTS)
# this was only for MMC/SD
#       -Wc-Dk_opt_dynamic_cache

                                                        
LIB_OPT_LIST =  $(LIB_OPT_LIST)

LIBLIST = $(LIBLIST)\
           $(BIN_DRIVE)\c2410\lib\2410\librt.a\
           $(WMA_LIB_PATH)\wmalibNoDRM.a

ifdef MMC
LIBLIST = $(LIBLIST)\
           $(CHIP_LIB_PATH)\dd_mmc_media.a
endif

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


HWEQULIST = $(HWEQULIST)\
            $(HWEQUCOMMON)\
            $(OUTPUT_INCLUDE_PATH)\regsecc.h\
            $(OUTPUT_INCLUDE_PATH)\regsgpflash.h\
            $(OUTPUT_INCLUDE_PATH)\regsfilco.h\
            $(OUTPUT_INCLUDE_PATH)\regspwm.h\
            $(OUTPUT_INCLUDE_PATH)\regsusb20.h\
            $(OUTPUT_INCLUDE_PATH)\regsusb20phy.h

OBJLIST =   $(OBJLIST)\
            $(OBJ_PATH)\overlay_syscallfunction.obj\
            $(OBJ_PATH)\overlaymanager.obj\
            $(OBJ_PATH)\overlay_istaskready.obj\
            $(OBJ_PATH)\kernel_syswaitonsignal.obj\
            $(OBJ_PATH)\kernel_syswaitonmessage.obj\
            $(OBJ_PATH)\queues.obj\
            $(OBJ_PATH)\kernel_syssignal.obj\
            $(OBJ_PATH)\kernel_getregion.obj\
            $(OBJ_PATH)\sysmem.obj\
            $(OBJ_PATH)\sysmisc.obj\
            $(OBJ_PATH)\sysspeed.obj\
            $(OBJ_PATH)\steppingvoltageset.obj\
            $(OBJ_PATH)\bufdesc.obj\
            $(OBJ_PATH)\stack.obj\
            $(OBJ_PATH)\stackmem.obj\
            $(OBJ_PATH)\getmessage.obj\
            $(OBJ_PATH)\syspostmsg.obj\
            $(OBJ_PATH)\fattest.obj\
        $(OBJ_PATH)\systime.obj\
        $(OBJ_PATH)\systimers.obj\
            $(OBJ_PATH)\fsproj.obj\
            $(OBJ_PATH)\const.obj\
            $(OBJ_PATH)\dbcs.obj\
            $(OBJ_PATH)\modulemsg.obj\
            $(OBJ_PATH)\lcdsupport.obj\
            $(OBJ_PATH)\miscproject.obj\
            $(OBJ_PATH)\utilasm.obj\
            $(OBJ_PATH)\fsunicode.obj\
            $(OBJ_PATH)\Reentrant.obj\
            $(OBJ_PATH)\memorysetup.obj\
            $(OBJ_PATH)\divide.obj\
      $(OBJ_PATH)\syshwinit.obj

ifdef D_MMC
# mmc datadrive mk file compiles this object only if MAKE_FAT32TEST make var or PLIBS.
OBJLIST =   $(OBJLIST)\
      $(OBJ_PATH)\MMCEraseFunctions.obj
endif

ifdef FATTEST_EXTENDED
EXTOBJLIST = $(OBJ_PATH)\extended.obj
endif

RESOURCELIST = $(RESOURCELIST)\
    $(OUTPUT_RSRC_PATH)\mediastartup.src\
    $(OUTPUT_RSRC_PATH)\nanddatadriveinit.src     


RESOURCELIST = $(RESOURCELIST)\
    $(OUTPUT_RSRC_PATH)\mmcmediastartup.src\
    $(OUTPUT_RSRC_PATH)\mmcerase.src \
    $(OUTPUT_RSRC_PATH)\mmcinfo.src \
    $(OUTPUT_RSRC_PATH)\mmcreadwrite.src \
    $(OUTPUT_RSRC_PATH)\mmcenumerate.src \
    $(OUTPUT_RSRC_PATH)\mmcresetdevice.src \
    $(OUTPUT_RSRC_PATH)\mmcprocesscsd.src \
    $(OUTPUT_RSRC_PATH)\mmcprocesscid.src \
    $(OUTPUT_RSRC_PATH)\mmcdetectpresence.src 

ALL :  $(OUTPUT_INCLUDE_PATH)\project.h $(OUTPUT_PATH)\resource.bin $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).s  

$(OUTPUT_RSRC_PATH)\nanddatadriveinit.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mediastartup.loc 
$(OUTPUT_RSRC_PATH)\mediastartup.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mediastartup.loc
        @echo Extract $@
        $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)


ifdef D_MMC
$(OUTPUT_RSRC_PATH)\mmcmediastartup.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\fattest.loc
$(OUTPUT_RSRC_PATH)\mmcerase.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\fattest.loc
$(OUTPUT_RSRC_PATH)\mmcinfo.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\fattest.loc
$(OUTPUT_RSRC_PATH)\mmcreadwrite.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\fattest.loc
$(OUTPUT_RSRC_PATH)\mmcenumerate.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\fattest.loc
$(OUTPUT_RSRC_PATH)\mmcresetdevice.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\fattest.loc
$(OUTPUT_RSRC_PATH)\mmcprocesscsd.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\fattest.loc
$(OUTPUT_RSRC_PATH)\mmcprocesscid.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\fattest.loc
$(OUTPUT_RSRC_PATH)\mmcdetectpresence.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\fattest.loc
        @echo Extract $@
        $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH) 
else
#### create dummy resources so there are no changes to resource.inc
$(OUTPUT_RSRC_PATH)\mmcmediastartup.src : $(OUTPUT_PATH)\$(PROJ).abs 
     copy $(PROJRSRC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\mmcmediastartup.src

$(OUTPUT_RSRC_PATH)\mmcerase.src : $(OUTPUT_PATH)\$(PROJ).abs 
    copy $(PROJRSRC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\mmcerase.src

$(OUTPUT_RSRC_PATH)\mmcinfo.src : $(OUTPUT_PATH)\$(PROJ).abs 
    copy $(PROJRSRC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\mmcinfo.src

$(OUTPUT_RSRC_PATH)\mmcreadwrite.src : $(OUTPUT_PATH)\$(PROJ).abs 
    copy $(PROJRSRC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\mmcreadwrite.src

$(OUTPUT_RSRC_PATH)\mmcenumerate.src : $(OUTPUT_PATH)\$(PROJ).abs 
    copy $(PROJRSRC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\mmcenumerate.src

$(OUTPUT_RSRC_PATH)\mmcresetdevice.src : $(OUTPUT_PATH)\$(PROJ).abs 
    copy $(PROJRSRC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\mmcresetdevice.src

$(OUTPUT_RSRC_PATH)\mmcprocesscsd.src : $(OUTPUT_PATH)\$(PROJ).abs 
    copy $(PROJRSRC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\mmcprocesscsd.src

$(OUTPUT_RSRC_PATH)\mmcprocesscid.src : $(OUTPUT_PATH)\$(PROJ).abs 
    copy $(PROJRSRC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\mmcprocesscid.src   

$(OUTPUT_RSRC_PATH)\mmcdetectpresence.src : $(OUTPUT_PATH)\$(PROJ).abs 
    copy $(PROJRSRC_PATH)\null_data.src $(OUTPUT_RSRC_PATH)\mmcdetectpresence.src
endif      #ifdef MMC

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////


ifdef FATTEST_EXTENDED
"$(OUTPUT_PATH)\$(PROJ).out" : $(PROJ).mk testdflt.dsc $(HWEQULIST) $(MODLIST) $(OBJLIST) $(EXTOBJLIST) $(ALLLIBLIST) $(FAT32LIB) $(LIBLIST)

else
"$(OUTPUT_PATH)\$(PROJ).out" : $(PROJ).mk testdflt.dsc $(HWEQULIST) $(MODLIST) $(OBJLIST) $(ALLLIBLIST) $(FAT32LIB) $(LIBLIST)
endif
    @echo Linking $(PROJ).out
        @$(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(LIB_OPT_LIST) $(match .a $!) $(match .sym $!)

"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" testdflt.dsc
    @echo Creating IEEE-695 absolute file $@
        @$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out



#///////////////////////////////////////////////////////////////////////////////
# Project-specific files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\profiler.obj" : "$(ROOT)\Algorithms\DRM\janus\src\asm\utilities\profiler.asm"

"$(OBJ_PATH)\Profile_tools.obj" : "$(PROFILE_PATH)\Profile_tools.c" "$(PROFILE_PATH)\Profile_Tools.h"

"$(OBJ_PATH)\mmcsalstructs.obj" : $(DDI_MMC_PATH)\mmcsalstructs.c

"$(OBJ_PATH)\mmcerase.obj" : $(DDI_MMC_PATH)\mmcerase.c

"$(OBJ_PATH)\fattest.obj" : "$(PROJ_PATH)\fattest.c" "$(OUTPUT_INCLUDE_PATH)\messages.h" "$(OUTPUT_INCLUDE_PATH)\project.h"
"$(OBJ_PATH)\fattest.obj" : "$(SYS_COMMON_PATH)\sysmacro.asm" "$(INC_PATH)\sysequ.inc" "$(INC_PATH)\sysresources.inc"
"$(OBJ_PATH)\fattest.obj" : "$(XREF_PATH)\sysmisc.xref" "$(XREF_PATH)\sysresource.xref" 

"$(OBJ_PATH)\fsproj.obj" : "$(PROJ_PATH)\fsproj.c" "$(PROJ_PATH)\fsproj.h"

$(OBJ_PATH)\systime.obj     : $(SYS_COMMON_PATH)\systime.asm

$(OBJ_PATH)\systimers.obj     : $(SYS_COMMON_PATH)\systimers.asm

"$(OBJ_PATH)\extended.obj" : "$(PROJ_PATH)\extended.c"

"$(OBJ_PATH)\lcdsupport.obj" : "$(PROJSUPPORT_PATH)\lcdsupport.c"  

#"$(OBJ_PATH)\startup.obj" : "$(PROJ_PATH)\startup.asm"

"$(OBJ_PATH)\utilasm.obj" : "$(PROJSYS_PATH)\utilasm.asm"

"$(OBJ_PATH)\const.obj" : "$(SYS_PATH)\data_management\const.asm"

"$(OBJ_PATH)\bufdesc.obj" : "$(SYS_COMMON_PATH)\bufdesc.asm" "$(XREF_PATH)\const.xref"
"$(OBJ_PATH)\bufdesc.obj" : "$(INC_PATH)\sysequ.inc"          

"$(OBJ_PATH)\stack.obj" : "$(SYS_PATH)\data_management\stack\stack.asm" "$(SYS_COMMON_PATH)\sysmacro.asm"
"$(OBJ_PATH)\stack.obj" : "$(XREF_PATH)\stackmem.xref" "$(XREF_PATH)\const.xref"

"$(OBJ_PATH)\stackmem.obj" : "$(SYS_COMMON_PATH)\stackmem.asm" 

"$(OBJ_PATH)\syspostmsg.obj" : "$(SYS_COMMON_PATH)\syspostmsg.asm"   "$(SYS_COMMON_PATH)\sysmacro.asm"
"$(OBJ_PATH)\syspostmsg.obj" : "$(INC_PATH)\sysequ.inc"                "$(INC_PATH)\msgequ.inc" 
"$(OBJ_PATH)\syspostmsg.obj" : "$(XREF_PATH)\bufdesc.xref"            "$(XREF_PATH)\const.xref"

"$(OBJ_PATH)\sysspeed.obj" : "$(SYS_DEVICE_PATH)\sysspeed.asm"

"$(OBJ_PATH)\steppingvoltageset.obj" : "$(SYS_DEVICE_PATH)\steppingvoltageset.c" "$(SYS_COMMON_PATH)\systime.asm" "$(SYS_COMMON_PATH)\systimers.asm"

"$(OBJ_PATH)\modulemsg.obj": $(PROJ_PATH)\modulemsg.asm

#"$(OBJ_PATH)\sysmodule4test.obj" : "$(SYS_EXEC_PATH)\sysmodule4test.asm"

"$(OBJ_PATH)\sysmem.obj" : "$(SYS_COMMON_PATH)\sysmem.asm" "$(XREF_PATH)\sysmisc.xref"
"$(OBJ_PATH)\sysmem.obj" : "$(INC_PATH)\sysequ.inc" "$(XREF_PATH)\bufdesc.xref" 

$(OBJ_PATH)\getmessage.obj : $(SYS_COMMON_PATH)\getmessage.asm

"$(OBJ_PATH)\sysmisc.obj" : "$(SYS_COMMON_PATH)\sysmisc.asm" 
"$(OBJ_PATH)\sysmisc.obj" :  "$(XREF_PATH)\const.xref"
"$(OBJ_PATH)\syshwinit.obj" : "$(SYS_DEVICE_PATH)\syshwinit.asm"

"$(OBJ_PATH)\miscproject.obj" : $(PROJ_PATH)\miscproject.c

"$(OBJ_PATH)\fsunicode.obj" : "$(RELBASE_PATH)\FileSystem\fsunicode.c"
"$(OBJ_PATH)\Reentrant.obj" : "$(RELBASE_PATH)\FileSystem\Reentrant.asm"
"$(OBJ_PATH)\divide.obj" : "$(PROJSYS_PATH)\divide.asm"

$(OBJ_PATH)\overlay_syscallfunction.obj  : $(SYS_PATH)\kernel\overlay_syscallfunction.asm
$(OBJ_PATH)\overlay_syscallfunction.obj  : $(SYS_PATH)\common\sysmacro.asm 

$(OBJ_PATH)\overlaymanager.obj : $(SYS_PATH)\kernel\overlaymanager.c
$(OBJ_PATH)\overlay_istaskready.obj : $(SYS_PATH)\kernel\overlay_istaskready.c
$(OBJ_PATH)\kernel_syswaitonsignal.obj : $(SYS_PATH)\kernel\kernel_syswaitonsignal.asm
$(OBJ_PATH)\kernel_syswaitonmessage.obj : $(SYS_PATH)\kernel\kernel_syswaitonmessage.asm
$(OBJ_PATH)\queues.obj : $(SYS_PATH)\data_management\queues.asm
$(OBJ_PATH)\kernel_syssignal.obj : $(SYS_PATH)\kernel\kernel_syssignal.c

$(OBJ_PATH)\kernel_getregion.obj        : $(SYS_PATH)\kernel\kernel_getregion.asm
$(OBJ_PATH)\kernel_getregion.obj        : $(SYS_PATH)\common\sysmacro.asm 

"$(OUTPUT_INCLUDE_PATH)\messages.h":$(INC_PATH)\msgequ.inc 
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 


$(OBJ_PATH)\dbcs.obj: $(SYS_COMMON_PATH)\dbcs.c $(OUTPUT_INCLUDE_PATH)\resource.h $(OUTPUT_INCLUDE_PATH)\sysequ.h $(OUTPUT_INCLUDE_PATH)\sysresources.h


$(OBJ_PATH)\memorysetup.obj : $(PROJ_PATH)\memorysetup.c

$(OUTPUT_INCLUDE_PATH)\resource.h:$(PROJ_PATH)\resource.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_INCLUDE_PATH)\sysequ.h:$(INC_PATH)\sysequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_INCLUDE_PATH)\sysresources.h:$(INC_PATH)\sysresources.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_INCLUDE_PATH)\project.h:..\project.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(OUTPUT_INCLUDE_PATH)\errors.h :$(INC_PATH)\errors.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 

$(HWEQU_H) : $(HW_REGS_INC_PATH)\hwequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@ 


#///////////////////////////////////////////////////////////////////////////////
# Module code extraction
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).s" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System S-rec for $(DEVICE) device family
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -S$@=P_vector,P_Media_DD_Read_cluster,P_clstr,X_clstr,Y_clstr,Y_copyClstr -V$(VERSION) -OFFE8=0828 -OFFE9=1818 -OFFED=380000
        copy /y $(OUTPUT_PATH)\$(PROJ).s $(OUTPUT_PATH)\stmpsys.s
        @$(BIN_PATH)\stosbcon $(OUTPUT_PATH)\stmpsys.s $(OUTPUT_PATH)\stmpsys.sb
        
#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////

$(OUTPUT_PATH)\resource.bin : $(RESOURCELIST) $(MODULELIST)
    @echo Creating $@
    $(BIN_PATH)\rsclink.EXE -h$(PROJ_PATH)\resource.inc \
    -i$(OUTPUT_RSRC_PATH) \
    -i$(PROJRSRC_PATH) \
    -i$(RESOURCE_PATH)\ShingYih \
    -i$(ROOT)\fonts\iso8859\
    -i$(ROOT)\fonts\iso8859\sigmatel_8 \
    -i$(BITMAP_PATH)\icons\status\16 \
    -i$(BITMAP_PATH)\icons\battery\8x8 \
    -i$(BITMAP_PATH)\strings\arial_8 \
    -i$(BITMAP_PATH)\icons\volume\8x8 \
    -o$(OUTPUT_PATH)\resource.bin \
    -v$(VERSION) -a 
    @$(BIN_PATH)\resparse $(OUTPUT_PATH)\resource.bin $(OUTPUT_PATH)\resource.txt




