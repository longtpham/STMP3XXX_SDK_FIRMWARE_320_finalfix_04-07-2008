#INCLUDE_CHKDSK=Yes
#ifdef INCLUDE_CHKDSK
# only include checkdisk in the 3410 USBMSC
#ifdef D_D3410		
#BLD=$(BLD) -DCHKDSK
#C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DCHKDSK
#endif
#endif

################# BUILD OPTIONS #########################

#### STEELY DAN FM TUNER #####
STFM_BUILD=TRUE
ifdef STFM_BUILD
STFM1000_LCD=TRUE
BLD=$(BLD) -DTUNER_STFM1000
BLD=$(BLD) -DSTFM1000_LCD=TRUE
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DTUNER_STFM1000
endif

####################################
# Enable manufacturing tool. 
####################################
ifdef D_MFGTOOL
AS_OPTS=$(AS_OPTS) -DMFG_TOOL
CC_OPTS=$(CC_OPTS) -DMFG_TOOL
endif 

####################################
# Enable UPDATER.SB build. 
####################################
ifdef D_UPDATER
# no asm code uses this currently.
CC_OPTS=$(CC_OPTS) -DUPDATER
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DUPDATER
endif 


#################################### 
# IN SDK3 USB MODE THESE TUNER MACROS ARE UNTESTED. Purpose was to reduce current by powering off tuner IC.
# Please uncomment the following two lines if using Philips 5757 FM Tuner
#AS_OPTS=$(AS_OPTS) -DTUNER_5757
#CC_OPTS=$(CC_OPTS) -DTUNER_5757
####################################
# Please uncomment the following two lines if using Philips 5767 FM Tuner
#AS_OPTS=$(AS_OPTS) -DTUNER_5767
#CC_OPTS=$(CC_OPTS) -DTUNER_5767

####################################################
# To detect if MMC and C_LCD_16BIT are use together.
ifdef D_MMC
ifdef D_C_LCD_16BIT
BUILD_OPTION_ERROR=1
endif
endif
####################################################

####################################
include ../../config.mk
####################################

# Put here to force a default 3500 build.  
D_D3500=TRUE

#Uncomment the following line to enable the USB throughput to be shown while connected to the PC.
#BLD=$(BLD) -DUSB_THROUGHPUT

#Uncomment the following line to Customize the USB Manufacturer and Product strings.
# Using Unicode characters.
#BLD=$(BLD) -DCUSTOMIZE_UNICODE

# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
PROJ    = usbmsc
# points to \Mp3Firmware
RELBASE_PATH=..\..\..\..\..
ROOT    = $(RELBASE_PATH)

# these macros are project dependent
MAIN_PATH           = $(ROOT)\Projects\sdk\lcdexample
PROJ_PATH           = $(ROOT)\Projects\sdk\lcdexample\usbmsc
PROJSYS_PATH        = $(PROJ_PATH)\System
PROJRSRC_PATH       = $(PROJ_PATH)\rsrc
PROJMAKE_PATH       = $(PROJ_PATH)\make

USB_PATH            = $(ROOT)\DeviceDriver\Media\usb
USBMSC_PATH         = $(ROOT)\DeviceDriver\Media\usbmsc
SCSI_PATH           = $(ROOT)\DeviceDriver\Media\scsi

BITMAP_PATH	    = $(ROOT)\Bitmaps

#########################################
#
include $(ROOT)\etc\standard.mk
#########################################
include $(ROOT)\etc\registers.mk
include $(ROOT)\etc\kernel.mk
include $(ETC_PATH)\includes.mk
#///////////////////////////////////////////////////////////////////////////////
# Dependencies
#///////////////////////////////////////////////////////////////////////////////

PROJECT_H = $(OUTPUT_INCLUDE_PATH)\project.h
HWEQU_H   = $(OUTPUT_INCLUDE_PATH)\hwequ.h
    

MAIN_DEPENDENCIES= \
    $(USB_DEPENDENCIES) \
    $(USBMSC_DEPENDENCIES) \
    $(SCSI_DEPENDENCIES)\
    $(PROJECT_H) \
    $(HWEQU_H) \
    $(OUTPUT_INCLUDE_PATH)\resource.h \
    $(OUTPUT_INCLUDE_PATH)\errors.h \

$(HWEQU_H) : $(HW_REGS_INC_PATH)\hwequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@


FAT12_PATH= $(ROOT)\FileSystem\Fat12

# must follow standard.mk, where base INC_ALL gets defined
INC_ALL= -I$(MAIN_PATH);$(PROJRSRC_PATH);$(PROJSYS_PATH);$(INC_ALL);$(OUTPUT_PATH)
INC_ALL_C = $(INC_ALL_C)

################################################
# The second definition is needed because of 
# battery related code which is added by 2.470
# merge.  Since 2.470 merge was done after
# 3.070 merge, defining BATTERY_TYPE_ALKALINE
# is probably o.k. since 3.xxx branch does the
# charging
#BLD=$(BLD) -DBATTERY_TYPE_ALKALINE
################################################

BLD=$(BLD) -DUSBMSC_BUILD

### $(BLD) is environment variable; set in batch file
#AS_OPTS=$(INC_ALL) -OJMNPRS -RDRS -gs -l -LCeIn -DTASKING $(BLD) 
AS_OPTS=$(INC_ALL) -OJMPS -l -LCeIn -DBRD=REVB -DDPLUS=TRUE -DTASKING $(BLD)
LK_OPTS=-L$(PRODLIB_DIR) -L$(MP3_PATH)\tasking -L$(BIN_DRIVE)\2410\lib\2410 -w8 $(LK_OPTS)
CC_OPTS= -Wc-si -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL) $(BLD) -DASCII_ONLY $(CC_OPTS)

# any additional C defines 
#CC_OPTS=$(CC_OPTS) -Wc-DyourDefine

### Avoid to build abs files with symbols
ifdef D_NO_SYMBOLS
LC_OPTS = -s $(LC_OPTS)
endif

ALLLIBLIST = $(ALLLIBLIST) $(STANDARD_LIB_PATH)\ccr.a

OBJLIST =   $(OBJLIST)\
            $(OBJ_PATH)\main.obj\
            $(OBJ_PATH)\usbmscmisc.obj\
            $(OBJ_PATH)\version.obj\
            $(OBJ_PATH)\componentversion.obj\
            $(OBJ_PATH)\miscproject.obj\
            $(OBJ_PATH)\usbmscproject.obj\
            $(OBJ_PATH)\system.obj\
            $(OBJ_PATH)\usbmsc_resource.obj\
            $(OBJ_PATH)\usbmsc_lcd.obj\
            $(OBJ_PATH)\usbmsc_lcd_battery.obj\
            $(OBJ_PATH)\usbmsc_ids.obj\
            $(OBJ_PATH)\lowresolutionadc.obj\
            $(OBJ_PATH)\lowresolutionadcinit.obj\
            $(OBJ_PATH)\usbbattery.obj\
            $(OBJ_PATH)\usbbrownout.obj\
            $(OBJ_PATH)\persistbits.obj\
            $(OBJ_PATH)\usbinsertion.obj\
            $(OBJ_PATH)\syserror.obj\
            $(OBJ_PATH)\STFM1000TestPresence.obj 



RSRCLIST =  $(PROJRSRC_PATH)\connectleft.src\
            $(PROJRSRC_PATH)\connectright.src\
            $(PROJRSRC_PATH)\font_table.src\
            $(PROJRSRC_PATH)\notes.src\
            $(PROJRSRC_PATH)\reading.src\
            $(PROJRSRC_PATH)\ready.src\
            $(PROJRSRC_PATH)\writing.src\
            $(PROJRSRC_PATH)\period.src\
            $(PROJRSRC_PATH)\version.src\
            $(PROJRSRC_PATH)\lowbat.src\
            $(PROJRSRC_PATH)\fullspeed.src\
            $(PROJRSRC_PATH)\highspeed.src\
            $(OUTPUT_RSRC_PATH)\battery_00.src\
            $(OUTPUT_RSRC_PATH)\battery_01.src\
            $(OUTPUT_RSRC_PATH)\battery_02.src\
            $(OUTPUT_RSRC_PATH)\battery_03.src\
            $(OUTPUT_RSRC_PATH)\battery_04.src\
            $(OUTPUT_RSRC_PATH)\battery_05.src\
            $(OUTPUT_RSRC_PATH)\battery_06.src\
            $(OUTPUT_RSRC_PATH)\battery_07.src\
            $(OUTPUT_RSRC_PATH)\battery_08.src\
            $(OUTPUT_RSRC_PATH)\battery_09.src\
            $(OUTPUT_RSRC_PATH)\battery_10.src\
            $(OUTPUT_RSRC_PATH)\lcd_controller_init.src\
            $(OUTPUT_RSRC_PATH)\charging.src\
            $(OUTPUT_RSRC_PATH)\charge_complete.src\
			$(OUTPUT_RSRC_PATH)\font_PGM.src\
			$(OUTPUT_RSRC_PATH)\font_SGMs.src\
			$(OUTPUT_RSRC_PATH)\font_script_00.src\
			$(OUTPUT_RSRC_PATH)\font_scripts.src\
			$(OUTPUT_RSRC_PATH)\font_PDM.src\
			$(OUTPUT_RSRC_PATH)\font_SDMs.src



HEADERLIST = $(OUTPUT_INCLUDE_PATH)\regsrevision.h

MAKELIST =  $(PSUILIST) $(OBJLIST) $(ALLLIBLIST) $(SYMBOLLIST) $(HEADERLIST)
            
ifdef BUILD_OPTION_ERROR

ALL :
    @echo         *********************************************************
    @echo         ****                                                 ****
    @echo         ****    C_LCD_16BIT can not use together with MMC    ****
    @echo         ****                                                 ****
    @echo         *********************************************************
    
else

ALL : $(OUTPUT_PATH)\$(PROJ).s
#ALL : $(OUTPUT_PATH)\$(PROJ).abs

endif

#########################################
#
include $(ROOT)\etc\ddildl.mk
#########################################
#

ifdef D_MMC
include $(ROOT)\etc\MMCDataDrive.mk
#########################################
#
include $(ROOT)\etc\MMCMedia.mk
#########################################
endif

include $(ROOT)\etc\RAMDataDrive.mk
#########################################
#
include $(ROOT)\etc\RAMSystemDrive.mk
#########################################
#
include $(ROOT)\etc\RAMMedia.mk
#########################################
#
include $(ROOT)\etc\RAMHal.mk
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


#########################################
#
include $(ROOT)\etc\ResourceManager.mk
#########################################



######################################
#include the USB MSC makefile
include $(RELBASE_PATH)\etc\usbmsc.mk
######################################


######################################
#include the updater
include $(RELBASE_PATH)\etc\updater.mk
######################################

######################################
#include the System Serial Number makefile
include $(RELBASE_PATH)\etc\sysserialnumber.mk
######################################

######################################
#Display module
ifdef D_CLCD
include $(RELBASE_PATH)\etc\display_task_color.mk
include $(RELBASE_PATH)\etc\samsung_s6b33b0a.mk
include $(RELBASE_PATH)\etc\colorlcdsal.mk
else
include $(RELBASE_PATH)\etc\display_task.mk
include $(RELBASE_PATH)\etc\epson_sed15xx.mk

#To enable writing directly to the LCD, comment the next
#next line and uncomment the "directtolcd.mk" line. 
include $(RELBASE_PATH)\etc\framebuffer.mk
#include $(RELBASE_PATH)\etc\directtolcd.mk
endif
######################################

######################################
include $(ROOT)\etc\stringlib.mk
######################################

######################################
include $(ROOT)\etc\instrument.mk
######################################

######################################
include $(ROOT)\etc\battery.mk
######################################

######################################
#Define USE_PLAYLIST3_HOST for music library stick bit
USE_PLAYLIST3_HOST = TRUE
ifdef USE_PLAYLIST3_HOST
BLD=$(BLD) -DUSE_PLAYLIST3_HOST
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DUSE_PLAYLIST3_HOST
endif
######################################

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////


"$(OUTPUT_PATH)\$(PROJ).out" :  $(PROJMAKE_PATH)\stmp3410.dsc $(MAKELIST) $(STANDARDLIBLIST) 
    @echo Linking $(PROJ).out
        $(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(match .a $!) $(match .sym $!) 


"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" $(PROJMAKE_PATH)\stmp3500.dsc $(CPU_PATH)\stmp3400.cpu
    @echo Creating IEEE-695 absolute file $@
        $(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out

#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////
ifdef D_CLCD
"$(OUTPUT_PATH)\resource.asm" : $(RSRCLIST) $(OUTPUT_INCLUDE_PATH)\resource.inc 
    @echo Creating $@
    @$(BIN_PATH)\rsclink.EXE -h$(match .inc $!) \
    -i$(RESOURCE_PATH)\misc \
    -i$(OUTPUT_RSRC_PATH) \
    -i$(PROJRSRC_PATH) \
    -o$(OUTPUT_PATH)\resource.bin \
    -v$(VERSION) -a
    @$(BIN_PATH)\resparse $(OUTPUT_PATH)\resource.bin $(OUTPUT_PATH)\resource.txt
else
"$(OUTPUT_PATH)\resource.asm" : $(RSRCLIST) $(OUTPUT_INCLUDE_PATH)\resource.inc 
    @echo Creating $@
    @$(BIN_PATH)\rsclink.EXE -h$(match .inc $!) \
    -i$(ROOT)\fonts\iso8859\sigmatel_8 \
    -i$(ROOT)\fonts\iso8859\ \
    -i$(RESOURCE_PATH)\shingyih \
    -i$(RESOURCE_PATH)\misc \
    -i$(OUTPUT_RSRC_PATH) \
    -i$(PROJRSRC_PATH) \
    -o$(OUTPUT_PATH)\resource.bin \
    -v$(VERSION) -a
    @$(BIN_PATH)\resparse $(OUTPUT_PATH)\resource.bin $(OUTPUT_PATH)\resource.txt
endif    

$(OUTPUT_RSRC_PATH)\version.src     :$(PROJRSRC_PATH)\version.bmp
$(OUTPUT_RSRC_PATH)\connectleft.src :$(PROJRSRC_PATH)\connectleft.bmp
$(OUTPUT_RSRC_PATH)\connectright.src:$(PROJRSRC_PATH)\connectright.bmp
$(OUTPUT_RSRC_PATH)\notes.src       :$(PROJRSRC_PATH)\notes.bmp
$(OUTPUT_RSRC_PATH)\reading.src     :$(PROJRSRC_PATH)\reading.bmp
$(OUTPUT_RSRC_PATH)\ready.src       :$(PROJRSRC_PATH)\ready.bmp
$(OUTPUT_RSRC_PATH)\writing.src     :$(PROJRSRC_PATH)\writing.bmp
$(OUTPUT_RSRC_PATH)\period.src     :$(PROJRSRC_PATH)\period.bmp
$(OUTPUT_RSRC_PATH)\lowbat.src     :$(PROJRSRC_PATH)\lowbat.bmp
$(OUTPUT_RSRC_PATH)\charging.src     :$(PROJRSRC_PATH)\charging.bmp
$(OUTPUT_RSRC_PATH)\charge_complete.src     :$(PROJRSRC_PATH)\charge_complete.bmp

ifdef D_CLCD
$(OUTPUT_RSRC_PATH)\fullspeed.src      :$(PROJRSRC_PATH)\color_rsrc\fullspeed.bmp
$(OUTPUT_RSRC_PATH)\highspeed.src      :$(PROJRSRC_PATH)\color_rsrc\highspeed.bmp
else
$(OUTPUT_RSRC_PATH)\fullspeed.src      :$(PROJRSRC_PATH)\fullspeed.bmp
$(OUTPUT_RSRC_PATH)\highspeed.src      :$(PROJRSRC_PATH)\highspeed.bmp
endif
$(OUTPUT_RSRC_PATH)\battery_00.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_00.bmp
$(OUTPUT_RSRC_PATH)\battery_01.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_01.bmp
$(OUTPUT_RSRC_PATH)\battery_02.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_02.bmp
$(OUTPUT_RSRC_PATH)\battery_03.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_03.bmp
$(OUTPUT_RSRC_PATH)\battery_04.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_04.bmp
$(OUTPUT_RSRC_PATH)\battery_05.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_05.bmp
$(OUTPUT_RSRC_PATH)\battery_06.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_06.bmp
$(OUTPUT_RSRC_PATH)\battery_07.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_07.bmp
$(OUTPUT_RSRC_PATH)\battery_08.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_08.bmp
$(OUTPUT_RSRC_PATH)\battery_09.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_09.bmp
$(OUTPUT_RSRC_PATH)\battery_10.src     :$(BITMAP_PATH)\icons\battery\8x8\battery_10.bmp

ifdef D_CLCD
"$(OUTPUT_RSRC_PATH)\font_PGM.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\mono_PGM.src $(OUTPUT_RSRC_PATH)\font_PGM.src

"$(OUTPUT_RSRC_PATH)\font_SGMs.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\mono_SGMs.src $(OUTPUT_RSRC_PATH)\font_SGMs.src

"$(OUTPUT_RSRC_PATH)\font_script_00.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\mono_script_00.src $(OUTPUT_RSRC_PATH)\font_script_00.src

"$(OUTPUT_RSRC_PATH)\font_scripts.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\mono_scripts.src $(OUTPUT_RSRC_PATH)\font_scripts.src

"$(OUTPUT_RSRC_PATH)\font_PDM.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\color_PDM.src $(OUTPUT_RSRC_PATH)\font_PDM.src

"$(OUTPUT_RSRC_PATH)\font_SDMs.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\color_SDMs.src $(OUTPUT_RSRC_PATH)\font_SDMs.src

else #D_CLCD
"$(OUTPUT_RSRC_PATH)\font_PGM.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\sigmatel_8\sigmatel_8_PGM.src $(OUTPUT_RSRC_PATH)\font_PGM.src

"$(OUTPUT_RSRC_PATH)\font_SGMs.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\sigmatel_8\sigmatel_8_SGMs.src $(OUTPUT_RSRC_PATH)\font_SGMs.src

"$(OUTPUT_RSRC_PATH)\font_script_00.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\sigmatel_8\sigmatel_8_script_00.src $(OUTPUT_RSRC_PATH)\font_script_00.src

"$(OUTPUT_RSRC_PATH)\font_scripts.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\sigmatel_8\sigmatel_8_scripts.src $(OUTPUT_RSRC_PATH)\font_scripts.src

"$(OUTPUT_RSRC_PATH)\font_PDM.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\iso8859_PDM.src $(OUTPUT_RSRC_PATH)\font_PDM.src

"$(OUTPUT_RSRC_PATH)\font_SDMs.src" : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(ROOT)\fonts\iso8859\iso8859_SDMs.src $(OUTPUT_RSRC_PATH)\font_SDMs.src
endif #D_CLCD

ifdef S6B33B0A_LCD
$(OUTPUT_RSRC_PATH)\lcd_controller_init.src : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(PROJRSRC_PATH)\truly_lcd_init_seq_65k.src $(OUTPUT_RSRC_PATH)\lcd_controller_init.src
endif

ifdef SED15XX_LCD
$(OUTPUT_RSRC_PATH)\lcd_controller_init.src : $(OUTPUT_PATH)\$(PROJ).abs
	copy $(RESOURCE_PATH)\shingyih\system_lcd_init_seq.src $(OUTPUT_RSRC_PATH)\lcd_controller_init.src
endif

#///////////////////////////////////////////////////////////////////////////////
# Project-specific project files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\descriptors_3500.obj" : "$(USBMSC_PATH)\descriptors_3500.c" $(USBMSC_DEPENDENCIES)
$(OBJ_PATH)\usbinsertion.obj: "$(USBMSC_PATH)\usbinsertion.c" $(USBMSC_DEPENDENCIES)

$(OBJ_PATH)\system.obj      :   $(PROJ_PATH)\system\system.c
$(OBJ_PATH)\system.obj      :   $(OUTPUT_INCLUDE_PATH)\messages.h

"$(OBJ_PATH)\timestamp.obj" : $(SYS_PATH)\debug\timestamp.asm

"$(OBJ_PATH)\startup.obj" : $(SYS_COMMON_PATH)\startup.asm

"$(OBJ_PATH)\main.obj" : "$(PROJ_PATH)\main.c" $(OUTPUT_INCLUDE_PATH)\regsgpio.h $(MAIN_DEPENDENCIES)
"$(OBJ_PATH)\main.obj" : $(OUTPUT_INCLUDE_PATH)\messages.h
"$(OBJ_PATH)\main.obj" : $(OUTPUT_INCLUDE_PATH)\regsrtc.h

$(OBJ_PATH)\usbmsc_ids.obj : $(PROJ_PATH)\usbmsc_ids.c

"$(OBJ_PATH)\usbmscmisc.obj" : "$(SYS_COMMON_PATH)\usbmscmisc.c"
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpio.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regscore.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsrtc.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsemc.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regscodec.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regslradc.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsclkctrl.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsdcdc.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsswizzle.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regstb.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regssdram.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regstimer.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regspwm.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsrevision.h

ifdef D_D3500
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsgpflash.h
"$(OBJ_PATH)\usbmscmisc.obj" : $(OUTPUT_INCLUDE_PATH)\regsfilco.h
endif

"$(OBJ_PATH)\mediadetection.obj" : $(SYS_COMMON_PATH)\mediadetection.c

"$(OBJ_PATH)\MediaMap.obj": $(PROJ_PATH)\system\MediaMap.asm

"$(OBJ_PATH)\mediaroutines.obj" : $(SYS_COMMON_PATH)\mediaroutines.c

"$(OBJ_PATH)\copy.obj" : $(PROJ_PATH)\copy.c

"$(OBJ_PATH)\version.obj" : $(SYS_COMMON_PATH)\version.c

"$(OBJ_PATH)\componentversion.obj" : $(SYS_COMMON_PATH)\componentversion.asm

"$(OBJ_PATH)\miscproject.obj" : $(PROJSYS_PATH)\miscproject.c

"$(OBJ_PATH)\miscproject.obj" : $(OUTPUT_INCLUDE_PATH)\regsrevision.h

"$(OBJ_PATH)\usbproject.obj" : "$(PROJSYS_PATH)\usbproject.c" $(USB_DEPENDENCIES)

"$(OBJ_PATH)\usbmscproject.obj" : "$(PROJSYS_PATH)\usbmscproject.c" $(USBMSC_DEPENDENCIES)

"$(OBJ_PATH)\modulemsg.obj" : $(MAIN_PATH)\dcc\system\modulemsg.asm

"$(OBJ_PATH)\lcd.obj" : $(HW_MSGMODULES_PATH)\graphicallcd\lcd.asm 

$(OBJ_PATH)\bufdesc.obj : $(SYS_COMMON_PATH)\bufdesc.asm

$(OBJ_PATH)\syspostmsg.obj : $(SYS_COMMON_PATH)\syspostmsg.asm
###$(OBJ_PATH)\syspostmsg.obj : $(SYS_COMMON_PATH)\syspostmsg_kernel.asm

$(OBJ_PATH)\sysmodule.obj : $(SYS_EXEC_PATH)\sysmodule.asm

$(OBJ_PATH)\dbcs.obj: $(SYS_COMMON_PATH)\dbcs.c 

$(OBJ_PATH)\usbmsc_resource.obj : $(PROJ_PATH)\usbmsc_resource.asm $(OUTPUT_PATH)\resource.asm

$(OBJ_PATH)\usbmsc_lcd.obj      :   $(PROJ_PATH)\usbmsc_lcd.c
$(OBJ_PATH)\usbmsc_lcd.obj      :   $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\usbmsc_lcd.obj      :   $(MAIN_PATH)\battery_config.h

$(OBJ_PATH)\usbmsc_lcd_battery.obj      :   $(PROJ_PATH)\usbmsc_lcd_battery.c
$(OBJ_PATH)\usbmsc_lcd_battery.obj      :   $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\usbmsc_lcd_battery.obj      :   $(MAIN_PATH)\battery_config.h

"$(OBJ_PATH)\updater_scsi_commands.obj" : "$(SYS_COMMON_PATH)\Updater\updater_scsi_commands.c"
"$(OBJ_PATH)\updater_scsi_commands.obj" : "$(STANDARD_LIB_PATH)\ccr.a"

$(OBJ_PATH)\usbbattery.obj      :   $(SYS_DEVICE_PATH)\usbbattery.c
$(OBJ_PATH)\usbbattery.obj      :   $(MAIN_PATH)\battery_config.h

$(OBJ_PATH)\lowresolutionadc.obj :  $(SYS_DEVICE_PATH)\lowresolutionadc.c $(MAIN_DEPENDENCIES)

$(OBJ_PATH)\lowresolutionadcinit.obj :  $(SYS_DEVICE_PATH)\lowresolutionadcinit.c $(MAIN_DEPENDENCIES)

$(OBJ_PATH)\persistbits.obj :  $(SYS_DEVICE_PATH)\persistbits.c
$(OBJ_PATH)\persistbits.obj :  $(INC_PATH)\persist_bits.h

$(OBJ_PATH)\usbbrownout.obj : $(SYS_COMMON_PATH)\usbbrownout.asm

# SysDefaultAllVectors to a debug instruction in DEBUG builds or default vectors to 'jmp FatalError' for RETAIL build. 
# SystemHalt aka "error" macro in sysmacro.asm in a RETAIL build will jmp to vector P:00000C which
# has "jmp FatalError" installed (FatalError routine in this module resets DSP)
$(OBJ_PATH)\syserror.obj : $(SYS_COMMON_PATH)\syserror.asm

#TOVERIFY sdk3.0 had the next line but 2.6 proj & base ver did not. 
$(OBJ_PATH)\platform.obj : $(ROOT)\FileSystem\fat32\platform\platform.asm


# Remap LCD pins if STFM1000 chip present or not
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(HW_MSGMODULES_PATH)\Tuner\STFM1000\STFM1000TestPresence.c
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(HW_MSGMODULES_PATH)\Tuner\STFM1000\STFM1000.h
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsi2c.h
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsi2s.h 
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsspare.h 
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsspi.h 
"$(OBJ_PATH)\STFM1000TestPresence.obj" : $(OUTPUT_INCLUDE_PATH)\regsusb20phy.h

#///////////////////////////////////////////////////////////////////////////////
# General System
#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
# Module code extraction & encryption from s to sb file format
# In sdk2.6XX hostrsc.bin below is just a placeholder file. 
# Updater.sb is from UPDATER build option, and hostlink.sb is from non-UPDATER build. 
# These are same except for scsi strings and usb id's. 
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).s" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System S-rec         
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -S$@=all -V$(VERSION) -OFFE8=0828 -OFFE9=1818 -OFFED=380000
ifdef D_MFGTOOL
        @echo Encrypt $(PROJ).s to stmfgmsc.sb for manufacturing tool
        @$(BIN_PATH)\stosbcon "$(OUTPUT_PATH)\$(PROJ).s" $(OUTPUT_PATH)\stmfgmsc.sb
else
ifdef D_UPDATER
        @echo Encrypt $(PROJ).s to updater.sb
        @$(BIN_PATH)\stosbcon "$(OUTPUT_PATH)\$(PROJ).s" $(OUTPUT_PATH)\updater.sb
else
        @echo Encrypt $(PROJ).s to hostlink.sb
        @$(BIN_PATH)\stosbcon "$(OUTPUT_PATH)\$(PROJ).s" $(OUTPUT_PATH)\hostlink.sb
        copy "$(OUTPUT_PATH)\resource.bin" $(OUTPUT_PATH)\hostrsc.bin
endif
endif   

#ifndef D_UPDATER
# For USB test "USBCV 1.3".  This macor is only for USBMSC, not for MTP
#BLD=$(BLD) -DUSBTEST_FOR_USBCV_1_3
#C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DUSBTEST_FOR_USBCV_1_3
#endif
