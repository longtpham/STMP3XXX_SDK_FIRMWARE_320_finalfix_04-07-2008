##### PLAYER ####### segment moved to top of file for 3.120.
# Distinguish between Player, and USBMSC/MTP
BLD=$(BLD) -DSTMP_BUILD_PLAYER -DPLAYER_BUILD
STMP_BUILD_PLAYER=TRUE
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSTMP_BUILD_PLAYER
PLAYER=TRUE
# 3.120review next line and related
FULL_PLAYER_KERNEL=TRUE
STFM1000_LCD=TRUE
ifdef PLAYER
BLD=$(BLD) -DPLAYER
BLD=$(BLD) -DSTFM1000_LCD
BLD=$(BLD) -DFULL_PLAYER_KERNEL
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DPLAYER
C_ASM_OPTS=$(C_ASM_OPTS) -DFULL_PLAYER_KERNEL
endif
####################

include ../../config.mk
# Copyright (c) 2003-2005   SigmaTel, Inc.
# Filename: player.mk (LCD example audio player)

# Note: Environment defines made in this file are best checked with ifdef
#       rather than for strict value.

# Put here to force a default 3500 build.  D_D3501 will disable the FM tuner
# and the FM Tuner on linein2 (see below).
D_D3500=TRUE

# Uncomment the following line to build for 3501 device.
# WARNING: Do not turn off D_D3500 when you uncomment this line.
#D_D3501=TRUE

#All the following four codecs can be enabled at once
#Uncomment the following line, if you want MS ADPCM Codec
MS_ADPCM=TRUE

#Uncomment the following line, if you want IMA ADPCM Codec
IMA_ADPCM=TRUE

#Uncomment the following line, if you want plain vanilla PCM
WINDOWS_PCM=TRUE

#Uncomment the following line to enable Audible Acelp decoder
#D_AUDIBLEDEC=TRUE
ifdef D_AUDIBLEDEC
BLD=$(BLD) -DAUDIBLE
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DAUDIBLE
endif

################# BUILD OPTIONS #########################
# Default build is STFM1000 FM device.
# Pass in option REMOVE_FM from player.bat to save RAM if no FM is present

ifdef D_REMOVE_FM
BLD=$(BLD) -DREMOVE_FM
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DREMOVE_FM

ifdef STFM1000_BUILD
undef STFM1000_BUILD
endif

ifdef D_FM_PHILIPS
undef D_FM_PHILIPS
endif

else
ifndef D_FM_PHILIPS
STFM1000_BUILD=TRUE
endif
endif

### FM Philips support
ifdef D_FM_PHILIPS
BLD=$(BLD) -DFM_PHILIPS
endif

#### STFM1000 A2 and B2 FM TUNER IC support #####
ifdef STFM1000_BUILD
BLD=$(BLD) -DTUNER_STFM1000
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DTUNER_STFM1000
endif

#### USA is  200KHz grid, 75uS de-emphasis;  ####
     # Japan is 100KHz grid, 50uS de-emphasis;  #
     # Europe and China is 100KHz grid, 50uS de-emphasis #
#ifdef STFM1000_BUILD
ifdef D_FM_USA
    ###In USA,
BLD=$(BLD) -DFM_USA_REGION
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFM_USA_REGION
    ### de-emphasis is 75uS
BLD=$(BLD) -DFM_TUNER_US_DEEMPHASIS
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFM_TUNER_US_DEEMPHASIS
    ### 200KHz
BLD=$(BLD) -DSD_USE_200KHZ_TUNING_GRID
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSD_USE_200KHZ_TUNING_GRID

else
ifdef D_FM_JAPAN
    ### In JAPAN,
    ###de-emphasis is 50 uS
BLD=$(BLD) -DFM_JAPAN_REGION
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFM_JAPAN_REGION
    ### 100KHz
BLD=$(BLD) -DSD_USE_100KHZ_TUNING_GRID
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSD_USE_100KHZ_TUNING_GRID
else
    #ifdef D_FM_EU
    ### In Europe and China
    ### de-emphasis is 50uS
BLD=$(BLD) -DFM_EUROPE_REGION
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFM_EUROPE_REGION
    ### 100 KHz
BLD=$(BLD) -DSD_USE_100KHZ_TUNING_GRID
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSD_USE_100KHZ_TUNING_GRID
endif
endif
#endif

######## NEWSHINGYIH LCD #########
#uncomment the following line if you want to use the new Shing Yih LCD
NEWSHINGYIH=TRUE

ifdef NEWSHINGYIH
BLD=$(BLD) -DNEWSHINGYIH
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DNEWSHINGYIH
endif
################################

ifdef STFM1000_BUILD
BLD=$(BLD) -DREAL_I2S_DATA
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DREAL_I2S_DATA
endif

##### FUNCLETS #############
#Uncomment next four lines if using funclet
BLD=$(BLD) -DFUNCLET
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFUNCLET
D_FUNCLETS=TRUE
MAKE_FUNCLET_RSRC=TRUE
############################

#This is needed to force the SmartMedia DDI MediaInit routine to count the number
#   of hidden system blocks on the boot media
BLD=$(BLD) -DMEDIA_DDI_COUNT_HIDDEN_SYSTEM_BLOCKS
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DMEDIA_DDI_COUNT_HIDDEN_SYSTEM_BLOCKS

# Note: To enable filesystem performance for player, see fat32.mk and nanddatadrive.mk
# which currently exclude this define for all player cases due to memory limits.


# #### DEVICE ###############
# Set in standard.mk
# ###########################


### CAPLESS HEADPHONE MODE ###
# For FM audio, keep this block commented if you have the 100 pin 35xx and you want FM
# audio at pins Line-in-left/right, and ensure that you have external headphone capacitors in layout.
# The 144 pin BGA allows both capless HP and FM since less pins are shared.
# Uncomment this block to enable capless headphone mode for STMP35xx builds.
ifdef D_D3500
ifdef D_CAPLESSPHONES
BLD=$(BLD) -DCAPLESS_HP
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DCAPLESS_HP
endif
endif
##############################

### PLAYER STRESS TEST ###
ifdef D_PLAYER_STRESS
BLD=$(BLD) -DPLAYER_STRESS
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DPLAYER_STRESS
endif
##############################


###### FM TUNER ############
ifndef D_D3501
#uncomment the next line to include the FMTUNER in the build -- currently will not timeout if no response from hw, so don't unless wking hw
BLD=$(BLD) -DFMTUNER
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFMTUNER
# Use PWM0 for measurements if not STMP3502        TOVERIFY2   This is enabled for 3502 now isn't. Comment says it should not be. See JeffD.
#BLD=$(BLD) -DFM_TUNER_USE_PWM0
#C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFM_TUNER_USE_PWM0
endif

ifndef D_D3501
#uncomment the next line to use minimum filter, else the optimized low pass filter will build
#MIN_FILTER=TRUE
ifdef MIN_FILTER
BLD=$(BLD) -DMIN_FILTER
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DMIN_FILTER
endif
endif
############################



#####  144-Pin Package #####
ifndef  D_D3501
#uncomment the next line for 144 pin packages specific build options such as FM Tuner as LINE2
#144PIN_PACKAGE=TRUE
ifdef 144PIN_PACKAGE
BLD=$(BLD) -DFMTUNER_ON_LINE2_IN
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DFMTUNER_ON_LINE2_IN
endif
endif
############################

### SOFT TRACEBUF DEBUG TOOL ###  Currently does not build when enabling (SDK 2.400).
#  Not enabled by default to conserve memory
#  See SysTrace.asm and TracePointer->TraceBuffer on writing to circular buffer to trace system execution
#  Note: -Wc-DMACRO passes Define MACRO to C compiler (a CC2410 tool control program option).
#  Uncomment the two lines below to allocate the TraceBuffer and un-stub the trace macro.
#BLD=$(BLD) -DTRACEBUF_EN
#C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DTRACEBUF_EN -Wc-DTRACEBUF_EN
################################


###### 3RD PARTY IP #######################################
# requires agreement and code drop from 3rd party

###### STMP3420 PD-MP3 ENCODE ######
#### This encoder will only work on STMP3420 devices
#uncomment the next line to enable MP3 Encode SUPPORT
MP3_ENCODE=TRUE
ifdef MP3_ENCODE
BLD=$(BLD) -DMP3_ENCODE
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DMP3_ENCODE -Wc-DMP3_ENCODE
endif

###### STMP35xx Checkdisk ######
# only include checkdisk in the 35xx USBMSC
#ifdef D_D3500
#uncomment the next line if this is a STMP3501
#BLD=$(BLD) -DD3501
#uncomment the next line to enable Checkdisk
INCLUDE_CHKDSK=Yes
ifdef INCLUDE_CHKDSK
BLD=$(BLD) -DCHKDSK
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DCHKDSK
endif
#endif
############################

###### SMALL_PLAYLIST ######
# Reduce size of playlist for largest build only
ifdef D_SMALL_PLAYLIST
BLD=$(BLD) -DSMALL_PLAYLIST
endif
############################

###### SRS WOW #############
# uncomment next line to enable WOW
#D_WOW_ENABLE=TRUE
ifdef D_WOW_ENABLE
BLD=$(BLD) -DWOW
endif
############################

#########################################################

OUTPUT_RESOURCE_BIN=$(OUTPUT_PATH)\resource.bin
OUTPUT_STMPSYS_S=$(OUTPUT_PATH)\stmpsys.s
ifdef D_JANUS
OUTPUT_OEM_DATA=$(OUTPUT_RSRC_PATH)\oem_data.c
BLD=$(BLD) -DJANUS_BUILD
endif

# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
PROJ    = player
# relative pointer to SDK root directory \Mp3Firmware
RELBASE_PATH        =..\..\..\..\..
ROOT                = $(RELBASE_PATH)

# these macros are project dependent
PROJ_PATH           = $(ROOT)\Projects\sdk\lcdexample\Player
MAIN_PATH           = $(ROOT)\Projects\sdk\lcdexample
PROJSYS_PATH        = $(PROJ_PATH)\System
PROJRSRC_PATH       = $(PROJ_PATH)\Rsrc
PROJMENUS_PATH      = $(PROJ_PATH)\Menus
PROJDISP_PATH       = $(PROJ_PATH)\Display
DOC_DIR             = $(OUTPUT_PATH)\HTML

PROJECT_H           = $(OUTPUT_INCLUDE_PATH)\project.h
HWEQU_H           = $(OUTPUT_INCLUDE_PATH)\hwequ.h

#########################################
#
include $(ROOT)\etc\standard.mk
#########################################
include $(ROOT)\etc\registers.mk

# these definitions must follow standard.mk, where base INC_ALL gets defined
# project paths MUST be before system paths in order to select modified headers in project space
INC_ALL=-I$(PROJRSRC_PATH);$(PROJ_PATH);$(PROJSYS_PATH);$(PROJMENUS_PATH);$(COMMON_PROJ_PATH);$(OUTPUT_INCLUDE_PATH);$(INC_ALL);$(PROJDISP_PATH)
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
  -Wc-I$(CMP_MEDIA_NAND_PATH)\
  -Wc-I$(JPEG_ALG_PATH)\
  -Wc-I$(SMVJPEG_ALG_PATH)

#C_ASM_OPTS=$(C_ASM_OPTS)

################################################################################

# ASSEMBLER options
# -l     : generate listing file
# -LCeIn : Controls listing content
# -OJMPS : disable some optimizations
#AS_OPTS=$(INC_ALL) -OJMPS  -l -LCeIn -DTASKING $(BLD) -DTRACEBUFFER
AS_OPTS=$(INC_ALL) -OJMPS  -l -LCeIn -DTASKING $(BLD)

# COMPILER OPTIONS (precede with -Wc to make sure it gets passed to the compiler from the control program)
# -Wc-si : merge C source and included files into assembly files  ** can change to -Wc-s to remove included files from listing
# -tmp   : add this option to leave the .src when get an error in the assembler part of the .c to .obj
#          WANRING: must remove option and rebuild because the extension .src confuses the resource linker utility

# TOVERIFY TODO: move dyn cache & single fat option defs out of here, into fat32.mk
CC_OPTS= -Wc-si -DPLAYER $(BLD) -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL) $(C_ASM_OPTS)
#         -Wc-Dk_opt_single_fat   we probably can't use this until related code is designed for it. Ok for 2.6 but maybe not yet for merge to 3.0beta

# removed since it was only for MMC/SD -Wc-Dk_opt_dynamic_cache


# LINKER options
LK_OPTS=-L$(LIB_PATH) -L$(MP3_PATH)\tasking -L$(WMA_PATH)\tasking -L$(BIN_DRIVE)\2410\lib\2410 -w8 $(LIB_OPT_LIST)

### Avoid to build abs files with symbols
ifdef D_NO_SYMBOLS
LC_OPTS = -s $(LC_OPTS)
endif

LIB_OPT_LIST =  $(LIB_OPT_LIST)

LIBLIST = $(LIBLIST)\
          $(BIN_DRIVE)\c2410\lib\2410\librt.a

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

OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\main.obj\
    $(OBJ_PATH)\msgs.obj\
    $(OBJ_PATH)\changeplayset.obj\
    $(OBJ_PATH)\updateautoshutdowntimer.obj\
    $(OBJ_PATH)\sysdisplay.obj\
    $(OBJ_PATH)\displaysplashscreen.obj\
    $(OBJ_PATH)\display.obj\
    $(OBJ_PATH)\mainmenu.obj\
     $(OBJ_PATH)\playerstatemachine.obj\
    $(OBJ_PATH)\musicmenu.obj\
    $(OBJ_PATH)\musicmenu_extra.obj\
    $(OBJ_PATH)\fmtunermenu.obj\
    $(OBJ_PATH)\voicemenu.obj\
    $(OBJ_PATH)\recorderstatemachine.obj\
    $(OBJ_PATH)\micbias.obj\
    $(OBJ_PATH)\displaylists.obj\
    $(OBJ_PATH)\contrastmenu.obj\
    $(OBJ_PATH)\eqmenu.obj\
    $(OBJ_PATH)\playmodemenu.obj\
    $(OBJ_PATH)\pwrsettingsmenu.obj\
    $(OBJ_PATH)\settingsmenu.obj\
    $(OBJ_PATH)\saveusertime.obj\
    $(OBJ_PATH)\deletemenu.obj\
    $(OBJ_PATH)\aboutmenu.obj\
    $(OBJ_PATH)\backlightmenu.obj\
    $(OBJ_PATH)\miscproject.obj\
    $(OBJ_PATH)\fsproj.obj\
    $(OBJ_PATH)\fsutilsc.obj\
    $(OBJ_PATH)\fsutils.obj\
    $(OBJ_PATH)\fsunicode.obj\
    $(OBJ_PATH)\reentrant.obj\
    $(OBJ_PATH)\recordsettingsmenu.obj\
    $(OBJ_PATH)\sysfatwrite.obj\
    $(OBJ_PATH)\STFM1000TestPresence.obj\
	$(OBJ_PATH)\GetResourceTag.obj

ifdef JPEG_APP
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\jpegdisplaymenu.obj\
    $(OBJ_PATH)\jpegmanualmenu.obj\
    $(OBJ_PATH)\jpegslideshowmenu.obj\
    $(OBJ_PATH)\jpegthumbnailmenu.obj\
    $(OBJ_PATH)\jpegdisplaycommon.obj
else
ifdef JPEG_ALBUM_ART
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\jpegdisplaycommon.obj
endif
endif

############################################
ifdef JPEG_DECODER
JPEGDECODERS=TRUE
endif
ifdef SMVJPEG_DECODER
JPEGDECODERS=TRUE
endif
############################################
############################################
ifdef JPEGDECODERS

ifndef JPEG_APP
ifndef MOTION_VIDEO
NO_JPEG_FILEUTIL=TRUE
endif
endif

ifndef NO_JPEG_FILEUTIL
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\jpegfileutil.obj\
    $(OBJ_PATH)\jpegfileutilextra.obj
endif

endif #ifdef JPEGDECODERS
############################################

ifdef MOTION_VIDEO
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\motionvideomenu.obj\
    $(OBJ_PATH)\motionvideomenuinitstate.obj
endif


ifdef SPECTRUM_ANAL
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\spectrogram.obj
endif

ifdef D_D3410
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\cfporthil_resource.obj
endif

ifdef D_MMC
OBJLIST =   $(OBJLIST)\
    $(OBJ_PATH)\checkremovablemedia.obj

HWEQULIST = $(HWEQULIST)\
    $(OUTPUT_INCLUDE_PATH)\regsspi.h
endif

CODEBANKLIST = $(CODEBANKLIST)\
             $(OUTPUT_RSRC_PATH)\spectrogram.src\
             $(OUTPUT_RSRC_PATH)\mainmenu.src\
             $(OUTPUT_RSRC_PATH)\display.src\
             $(OUTPUT_RSRC_PATH)\playerstatemachine.src\
             $(OUTPUT_RSRC_PATH)\musicmenu.src\
             $(OUTPUT_RSRC_PATH)\motionvideomenu.src\
             $(OUTPUT_RSRC_PATH)\motionvideomenuinitstate.src\
             $(OUTPUT_RSRC_PATH)\jpegdisplaymenu.src\
             $(OUTPUT_RSRC_PATH)\jpegmanualmenu.src\
             $(OUTPUT_RSRC_PATH)\jpegslideshowmenu.src\
             $(OUTPUT_RSRC_PATH)\jpegthumbnailmenu.src\
             $(OUTPUT_RSRC_PATH)\albumartmenu.src\
             $(OUTPUT_RSRC_PATH)\jpegfileutilextra.src\
             $(OUTPUT_RSRC_PATH)\fmtunermenu.src\
             $(OUTPUT_RSRC_PATH)\voicemenu.src\
             $(OUTPUT_RSRC_PATH)\recorderstatemachine.src\
             $(OUTPUT_RSRC_PATH)\displaylists.src\
             $(OUTPUT_RSRC_PATH)\contrastmenu.src\
             $(OUTPUT_RSRC_PATH)\eqmenu.src\
             $(OUTPUT_RSRC_PATH)\playmodemenu.src\
             $(OUTPUT_RSRC_PATH)\pwrsettingsmenu.src\
             $(OUTPUT_RSRC_PATH)\settingsmenu.src\
             $(OUTPUT_RSRC_PATH)\deletemenu.src\
             $(OUTPUT_RSRC_PATH)\aboutmenu.src\
             $(OUTPUT_RSRC_PATH)\backlightmenu.src\
             $(OUTPUT_RSRC_PATH)\record_settings_menu_code_bank.src\
             $(OUTPUT_RSRC_PATH)\restoresysdrive.src\
         $(OUTPUT_RSRC_PATH)\softtimermod.src

ifdef MAKE_FUNCLET_RSRC

FUNCLETSRCLIST =    $(FUNCLETSRCLIST)\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysResetAllInt.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysSetupDecVec.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysSetupEncVec.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysTimeInit.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_AnalogInit.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_UsbConnected.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_ButtonInit.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_PowerUpHeadPhones.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_PowerDownHeadPhones.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_StartProject.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_ModuleSetTimer.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysStopDecoder.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysStopEncoder.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysSetDecoder.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysSetEncoder.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_ProcessPT.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysSetSpeed.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SysGetVolume.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_DisplaySplashScreen.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_MixerMasterFadeOut.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_MixerMasterFadeIn.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_DisableStmpGeneratedMicBias.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_EnableStmpGeneratedMicBias.src\
            $(OUTPUT_RSRC_PATH)\arrangefilename.src\
            $(OUTPUT_RSRC_PATH)\changetorootdirectory.src\
            $(OUTPUT_RSRC_PATH)\clearcluster.src\
            $(OUTPUT_RSRC_PATH)\createdirectory.src\
            $(OUTPUT_RSRC_PATH)\deletecontent.src\
            $(OUTPUT_RSRC_PATH)\deleterecord.src\
            $(OUTPUT_RSRC_PATH)\fastopen.src\
            $(OUTPUT_RSRC_PATH)\fcreate.src\
            $(OUTPUT_RSRC_PATH)\filegetattrib.src\
            $(OUTPUT_RSRC_PATH)\filegetdate.src\
            $(OUTPUT_RSRC_PATH)\filesetattrib.src\
            $(OUTPUT_RSRC_PATH)\filesetdate.src\
            $(OUTPUT_RSRC_PATH)\readrootdirrecord.src\
            $(OUTPUT_RSRC_PATH)\setcurrentpos.src\
            $(OUTPUT_RSRC_PATH)\fsinit.src\
            $(OUTPUT_RSRC_PATH)\fsdriveinit.src\
            $(OUTPUT_RSRC_PATH)\setcwdhandle.src\
            $(OUTPUT_RSRC_PATH)\fsclearBuf.src\
            $(OUTPUT_RSRC_PATH)\fsshutdown.src\
            $(OUTPUT_RSRC_PATH)\readdevicerecord.src\
            $(OUTPUT_RSRC_PATH)\checkspaceinrootdir.src\
            $(OUTPUT_RSRC_PATH)\fgetfasthandle.src\
            $(OUTPUT_RSRC_PATH)\freehandle.src\
            $(OUTPUT_RSRC_PATH)\updatehandlemode.src\
            $(OUTPUT_RSRC_PATH)\ishandlewriteallocated.src\
            $(OUTPUT_RSRC_PATH)\searchfreehandleallocate.src\
            $(OUTPUT_RSRC_PATH)\isfileopen.src\
            $(OUTPUT_RSRC_PATH)\iscurrworkdir.src\
            $(OUTPUT_RSRC_PATH)\changepath.src\
            $(OUTPUT_RSRC_PATH)\chdir.src\
            $(OUTPUT_RSRC_PATH)\chdirFromOffset.src\
            $(OUTPUT_RSRC_PATH)\deletetree.src\
            $(OUTPUT_RSRC_PATH)\deleteallrecords.src\
            $(OUTPUT_RSRC_PATH)\cleardata.src\
            $(OUTPUT_RSRC_PATH)\changetolowleveldir.src\
            $(OUTPUT_RSRC_PATH)\getrecordnumber.src\
            $(OUTPUT_RSRC_PATH)\fileremove.src\
            $(OUTPUT_RSRC_PATH)\charactersearch.src\
            $(OUTPUT_RSRC_PATH)\stringcompare.src\
            $(OUTPUT_RSRC_PATH)\fopen.src\
            $(OUTPUT_RSRC_PATH)\fopenw.src\
            $(OUTPUT_RSRC_PATH)\fremove.src\
            $(OUTPUT_RSRC_PATH)\fremovew.src\
            $(OUTPUT_RSRC_PATH)\mkdir.src\
            $(OUTPUT_RSRC_PATH)\mkdirw.src\
            $(OUTPUT_RSRC_PATH)\rmdir.src\
            $(OUTPUT_RSRC_PATH)\rmdirw.src\
            $(OUTPUT_RSRC_PATH)\searchdirectory.src\
            $(OUTPUT_RSRC_PATH)\sethandleforsearch.src\
            $(OUTPUT_RSRC_PATH)\fclose.src\
            $(OUTPUT_RSRC_PATH)\fgetc.src\
            $(OUTPUT_RSRC_PATH)\fgets.src\
            $(OUTPUT_RSRC_PATH)\fputc.src\
            $(OUTPUT_RSRC_PATH)\fputs.src\
            $(OUTPUT_RSRC_PATH)\arrangelongfilename.src\
            $(OUTPUT_RSRC_PATH)\convert_itoa.src\
            $(OUTPUT_RSRC_PATH)\createdirrecord.src\
            $(OUTPUT_RSRC_PATH)\chksum.src\
            $(OUTPUT_RSRC_PATH)\createshortdirrecord.src\
            $(OUTPUT_RSRC_PATH)\discardtrailigperiods.src\
            $(OUTPUT_RSRC_PATH)\extractfilename.src\
            $(OUTPUT_RSRC_PATH)\extractfilenamew.src\
            $(OUTPUT_RSRC_PATH)\extractpathw.src\
            $(OUTPUT_RSRC_PATH)\extractpath.src\
            $(OUTPUT_RSRC_PATH)\findfreerecord.src\
            $(OUTPUT_RSRC_PATH)\getnamew.src\
            $(OUTPUT_RSRC_PATH)\getname.src\
            $(OUTPUT_RSRC_PATH)\isdirectoryempty.src\
            $(OUTPUT_RSRC_PATH)\isshortnamevalid.src\
            $(OUTPUT_RSRC_PATH)\longdirmatch.src\
            $(OUTPUT_RSRC_PATH)\changecase.src\
            $(OUTPUT_RSRC_PATH)\unicodetooem.src\
            $(OUTPUT_RSRC_PATH)\matchdirrecord.src\
            $(OUTPUT_RSRC_PATH)\matchdirrecordw.src\
            $(OUTPUT_RSRC_PATH)\setcwd.src\
            $(OUTPUT_RSRC_PATH)\setshortfilename.src\
            $(OUTPUT_RSRC_PATH)\generatefilenametail.src\
            $(OUTPUT_RSRC_PATH)\dbcstounicode.src\
            $(OUTPUT_RSRC_PATH)\shortdirmatch.src\
            $(OUTPUT_RSRC_PATH)\uppercase.src\
            $(OUTPUT_RSRC_PATH)\setfilename.src\
            $(OUTPUT_RSRC_PATH)\convertoshortname.src\
            $(OUTPUT_RSRC_PATH)\strcpy.src\
            $(OUTPUT_RSRC_PATH)\strcpyw.src\
            $(OUTPUT_RSRC_PATH)\strlengthw.src\
            $(OUTPUT_RSRC_PATH)\strlength.src\
            $(OUTPUT_RSRC_PATH)\discardtrailigperiodsucs3.src\
            $(OUTPUT_RSRC_PATH)\discardtrailigperiodsw.src\
            $(OUTPUT_RSRC_PATH)\strcpyucs3_2.src\
            $(OUTPUT_RSRC_PATH)\getvolumelabel.src\
            $(OUTPUT_RSRC_PATH)\Funclet_loadusertime.src\
            $(OUTPUT_RSRC_PATH)\Funclet_saveusertime.src\
            $(OUTPUT_RSRC_PATH)\Funclet_SysLRADCBrownoutInit.src\
            $(OUTPUT_RSRC_PATH)\Funclet_SysBatteryGetLevel.src\
            $(OUTPUT_RSRC_PATH)\setvolumelabel.src\
            $(OUTPUT_RSRC_PATH)\Funclet_Init5VSense.src\
            $(OUTPUT_RSRC_PATH)\Funclet_UpdateDCDCDutyCycle.src\
            $(OUTPUT_RSRC_PATH)\Funclet_ChangePlaySet.src\
                    $(OUTPUT_RSRC_PATH)\two_adic_inverse.src\
                    $(OUTPUT_RSRC_PATH)\mp_shift.src\
                    $(OUTPUT_RSRC_PATH)\mp_significant_bit_count.src\
                    $(OUTPUT_RSRC_PATH)\set_immediate.src\
                    $(OUTPUT_RSRC_PATH)\multiply_immediate.src\
                    $(OUTPUT_RSRC_PATH)\multiply.src\
                    $(OUTPUT_RSRC_PATH)\divide_precondition_1.src\
                    $(OUTPUT_RSRC_PATH)\divide_immediate.src\
                    $(OUTPUT_RSRC_PATH)\ecaffine_exponentiation_tabular.src\
                    $(OUTPUT_RSRC_PATH)\ecaffine_table_construction.src\
                    $(OUTPUT_RSRC_PATH)\ecproj5_to_ecaffine.src\
                    $(OUTPUT_RSRC_PATH)\ecproj5_set_infinite.src\
                    $(OUTPUT_RSRC_PATH)\ecaffine_on_curve.src\
                    $(OUTPUT_RSRC_PATH)\ecaffine_addition.src\
                    $(OUTPUT_RSRC_PATH)\ecaffine_addition_subtraction.src\
                    $(OUTPUT_RSRC_PATH)\ecaffine_attributes2.src\
                    $(OUTPUT_RSRC_PATH)\kfdesc_initialize.src\
                    $(OUTPUT_RSRC_PATH)\kimmediate.src\
                    $(OUTPUT_RSRC_PATH)\kprime_immediater.src\
                    $(OUTPUT_RSRC_PATH)\kprime_sqrter.src\
                    $(OUTPUT_RSRC_PATH)\kinitialize_prime.src\
                    $(OUTPUT_RSRC_PATH)\mod_lucasuv.src\
                    $(OUTPUT_RSRC_PATH)\mod_lucas.src\
                    $(OUTPUT_RSRC_PATH)\bucket_multiply.src\
                    $(OUTPUT_RSRC_PATH)\mod_exp2000.src\
                    $(OUTPUT_RSRC_PATH)\mod_exp.src\
                    $(OUTPUT_RSRC_PATH)\modmul_choices1.src\
                    $(OUTPUT_RSRC_PATH)\mod_sqrt.src\
                    $(OUTPUT_RSRC_PATH)\create_modulus.src\
                    $(OUTPUT_RSRC_PATH)\from_modular.src\
                    $(OUTPUT_RSRC_PATH)\add_immediate.src\
                    $(OUTPUT_RSRC_PATH)\add_diff.src\
                    $(OUTPUT_RSRC_PATH)\add_full.src\
                    $(OUTPUT_RSRC_PATH)\compare_sum_same.src\
                    $(OUTPUT_RSRC_PATH)\sub_immediate.src\
                    $(OUTPUT_RSRC_PATH)\mp_initialization.src\
                    $(OUTPUT_RSRC_PATH)\new_random_bytes.src\
                    $(OUTPUT_RSRC_PATH)\new_random_dword_interval.src\
                    $(OUTPUT_RSRC_PATH)\mp_gcdex.src\
                    $(OUTPUT_RSRC_PATH)\mp_gcdex_split1.src\
                    $(OUTPUT_RSRC_PATH)\new_random_digit_interval.src\
                    $(OUTPUT_RSRC_PATH)\new_random_mod.src\
                    $(OUTPUT_RSRC_PATH)\new_random_mod_nonzero.src\
                    $(OUTPUT_RSRC_PATH)\new_random_digits.src\
                    $(OUTPUT_RSRC_PATH)\words_to_ecaffine.src\
                    $(OUTPUT_RSRC_PATH)\ecaffine_to_dwords.src\
                    $(OUTPUT_RSRC_PATH)\_threadunsafepkinit.src\
                    $(OUTPUT_RSRC_PATH)\pkinit.src\
                    $(OUTPUT_RSRC_PATH)\drm_pk_genkeypair.src\
                    $(OUTPUT_RSRC_PATH)\drm_pk_encrypt.src\
                    $(OUTPUT_RSRC_PATH)\drm_pk_decrypt.src\
                    $(OUTPUT_RSRC_PATH)\byte_array_mod_bignum.src\
                    $(OUTPUT_RSRC_PATH)\fe2ipmod.src\
                    $(OUTPUT_RSRC_PATH)\drm_pk_sign.src\
                    $(OUTPUT_RSRC_PATH)\drm_pk_verify.src\
                    $(OUTPUT_RSRC_PATH)\random_bytes.src\
                    $(OUTPUT_RSRC_PATH)\legacy_decryptcontentkey.src\
                    $(OUTPUT_RSRC_PATH)\drm_bbx_symmetricverify.src\
                    $(OUTPUT_RSRC_PATH)\drm_bbx_legacycipherkeysetup.src\
                    $(OUTPUT_RSRC_PATH)\drm_bbx_initialize.src\
                    $(OUTPUT_RSRC_PATH)\drm_bbx_canbind.src\
                    $(OUTPUT_RSRC_PATH)\_decryptcontentkey.src\
                    $(OUTPUT_RSRC_PATH)\verifychecksum.src\
                    $(OUTPUT_RSRC_PATH)\drm_bbx_hashvalue.src\
                    $(OUTPUT_RSRC_PATH)\drm_bbx_cipherkeysetup.src\
                    $(OUTPUT_RSRC_PATH)\_findkeypair.src\
                    $(OUTPUT_RSRC_PATH)\drm_b64_decodew.src\
                    $(OUTPUT_RSRC_PATH)\drm_b64_encodew.src\
                    $(OUTPUT_RSRC_PATH)\drm_mac_inv32.src\
                    $(OUTPUT_RSRC_PATH)\_parselicenseattributes.src\
                    $(OUTPUT_RSRC_PATH)\_loadlicenseattributes.src\
                    $(OUTPUT_RSRC_PATH)\_performactions.src\
                    $(OUTPUT_RSRC_PATH)\_getlicenseinfoandsetup.src\
                    $(OUTPUT_RSRC_PATH)\_processendofchain.src\
                    $(OUTPUT_RSRC_PATH)\drm_lic_completelicensechain.src\
                    $(OUTPUT_RSRC_PATH)\drm_lic_reportactions.src\
                    $(OUTPUT_RSRC_PATH)\drm_lic_checkclockrollback.src\
                    $(OUTPUT_RSRC_PATH)\drm_cphr_init.src\
                    $(OUTPUT_RSRC_PATH)\drmcrt_iswxdigit.src\
                    $(OUTPUT_RSRC_PATH)\drmcrt_towlower.src\
                    $(OUTPUT_RSRC_PATH)\drmcrt_wcsntol.src\
                    $(OUTPUT_RSRC_PATH)\drmcrt_wcslen.src\
                    $(OUTPUT_RSRC_PATH)\drmcrt_wcsncpy.src\
                    $(OUTPUT_RSRC_PATH)\drmcrt_memmove.src\
                    $(OUTPUT_RSRC_PATH)\performoperation_part1.src\
                    $(OUTPUT_RSRC_PATH)\performoperation_part2.src\
                    $(OUTPUT_RSRC_PATH)\performoperation_part3.src\
                    $(OUTPUT_RSRC_PATH)\performoperation_part4.src\
                    $(OUTPUT_RSRC_PATH)\performoperation_part5.src\
                    $(OUTPUT_RSRC_PATH)\performoperation_part6.src\
                    $(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression.src\
                    $(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression_no_more_tokens.src\
                    $(OUTPUT_RSRC_PATH)\_iscachedevent.src\
                    $(OUTPUT_RSRC_PATH)\isvalidfunction.src\
                    $(OUTPUT_RSRC_PATH)\functiongetvalue.src\
                    $(OUTPUT_RSRC_PATH)\globalsetvariable.src\
                    $(OUTPUT_RSRC_PATH)\variabledrmkgetorset.src\
                    $(OUTPUT_RSRC_PATH)\variabledrmgetorset.src\
                    $(OUTPUT_RSRC_PATH)\variablemachinegetorset.src\
                    $(OUTPUT_RSRC_PATH)\variableappgetorset.src\
                    $(OUTPUT_RSRC_PATH)\variablesecstategetorset.src\
                    $(OUTPUT_RSRC_PATH)\variablelicensegetorset.src\
                    $(OUTPUT_RSRC_PATH)\variablecontentgetorset.src\
                    $(OUTPUT_RSRC_PATH)\variabledevicegetorset.src\
                    $(OUTPUT_RSRC_PATH)\variablepmlicensegetorset.src\
                    $(OUTPUT_RSRC_PATH)\_basicheaderchecks.src\
                    $(OUTPUT_RSRC_PATH)\drm_hdr_getattribute.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_createstore.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_init.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_uninit.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_openstore.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_closestore.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_deleteslot.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_slotseek.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_slotwrite.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_slotresize.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_initslotenum.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_slotenumnext.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_cleanupstore.src\
                    $(OUTPUT_RSRC_PATH)\_hdscopyslot_child2child.src\
                    $(OUTPUT_RSRC_PATH)\_hdscopychildpayload.src\
                    $(OUTPUT_RSRC_PATH)\_hdsdefragmentfile.src\
                    $(OUTPUT_RSRC_PATH)\_hdscleanupnamespace.src\
                    $(OUTPUT_RSRC_PATH)\_hdscleanupstore.src\
                    $(OUTPUT_RSRC_PATH)\drm_lst_clean.src\
                    $(OUTPUT_RSRC_PATH)\_hdslocatefreeblockforslot.src\
                    $(OUTPUT_RSRC_PATH)\_hdslockblock2deleteslot.src\
                    $(OUTPUT_RSRC_PATH)\_hdsunlockblock2deleteslot.src\
                    $(OUTPUT_RSRC_PATH)\_hdscreatenamespace.src\
                    $(OUTPUT_RSRC_PATH)\_hdsfilepos2blocknum.src\
                    $(OUTPUT_RSRC_PATH)\_writesrn.src\
                    $(OUTPUT_RSRC_PATH)\_writecommonblockheader.src\
                    $(OUTPUT_RSRC_PATH)\_writechildblockheader.src\
                    $(OUTPUT_RSRC_PATH)\_readdatablockheader.src\
                    $(OUTPUT_RSRC_PATH)\_writedatablockheader.src\
                    $(OUTPUT_RSRC_PATH)\_hdsexpandstore.src\
                    $(OUTPUT_RSRC_PATH)\_hdsallocblock.src\
                    $(OUTPUT_RSRC_PATH)\_hdsfreeblock.src\
                    $(OUTPUT_RSRC_PATH)\_hdscreateoversizedslot.src\
                    $(OUTPUT_RSRC_PATH)\_hdsallocslotinfile.src\
                    $(OUTPUT_RSRC_PATH)\_hdswriteblockhdr.src\
                    $(OUTPUT_RSRC_PATH)\_hdsadjustchildpayload.src\
                    $(OUTPUT_RSRC_PATH)\_hdsfindleftmostleafblock.src\
                    $(OUTPUT_RSRC_PATH)\_hdscreateandopenslot.src\
                    $(OUTPUT_RSRC_PATH)\_hdsremoveslot.src\
                    $(OUTPUT_RSRC_PATH)\_hdscopyandlocknewslot.src\
                    $(OUTPUT_RSRC_PATH)\_hdsrelocateslotandkeepcurrlock.src\
                    $(OUTPUT_RSRC_PATH)\_hdstraversenextrightsiblingblock.src\
                    $(OUTPUT_RSRC_PATH)\_hdstraverseblocksinpostorder.src\
                    $(OUTPUT_RSRC_PATH)\_hdsslotresize.src\
                    $(OUTPUT_RSRC_PATH)\_isnull.src\
                    $(OUTPUT_RSRC_PATH)\_hdsgensrnhash.src\
                    $(OUTPUT_RSRC_PATH)\_hdsallocblockbuffer.src\
                    $(OUTPUT_RSRC_PATH)\_readsrn.src\
                    $(OUTPUT_RSRC_PATH)\_hdsgetputchildblocknum.src\
                    $(OUTPUT_RSRC_PATH)\_hdshashkeytoindex.src\
                    $(OUTPUT_RSRC_PATH)\_hdsloadsrn.src\
                    $(OUTPUT_RSRC_PATH)\_hdsslotwrite.src\
                    $(OUTPUT_RSRC_PATH)\_hdsinitslotenum.src\
                    $(OUTPUT_RSRC_PATH)\_hdsslotenumnext.src\
                    $(OUTPUT_RSRC_PATH)\drm_levl_evaluateexpression.src\
                    $(OUTPUT_RSRC_PATH)\drm_levl_performoperations.src\
                    $(OUTPUT_RSRC_PATH)\drm_lic_getattribute.src\
                    $(OUTPUT_RSRC_PATH)\drm_lic_getenablingbits.src\
                    $(OUTPUT_RSRC_PATH)\getlspubkey.src\
                    $(OUTPUT_RSRC_PATH)\drm_lic_verifysignature.src\
                    $(OUTPUT_RSRC_PATH)\_loadproritizedlist.src\
                    $(OUTPUT_RSRC_PATH)\drm_lst_open.src\
                    $(OUTPUT_RSRC_PATH)\drm_lst_close.src\
                    $(OUTPUT_RSRC_PATH)\drm_lst_getlicense.src\
                    $(OUTPUT_RSRC_PATH)\drm_lst_initenum.src\
                    $(OUTPUT_RSRC_PATH)\drm_lst_enumnext.src\
                    $(OUTPUT_RSRC_PATH)\drm_lst_enumdelete.src\
                    $(OUTPUT_RSRC_PATH)\_processextensions.src\
                    $(OUTPUT_RSRC_PATH)\_processidlist.src\
                    $(OUTPUT_RSRC_PATH)\_processexclusions.src\
                    $(OUTPUT_RSRC_PATH)\_processinclusions.src\
                    $(OUTPUT_RSRC_PATH)\drm_opl_processcopyoutputleveldata.src\
                    $(OUTPUT_RSRC_PATH)\_getopllevel.src\
                    $(OUTPUT_RSRC_PATH)\drm_opl_processplayoutputleveldata.src\
                    $(OUTPUT_RSRC_PATH)\_loadattributesintocache.src\
                    $(OUTPUT_RSRC_PATH)\_openandverifyslot.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_openkeytokens.src\
                    $(OUTPUT_RSRC_PATH)\_updateattributetokeninslot.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_closekey.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_gettokenvalue.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_settokenvalue.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_createlicensestatepassword.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_createglobalstorepassword.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_promoteansitounicode.src\
                    $(OUTPUT_RSRC_PATH)\drm_pk_symmetricverify.src\
                    $(OUTPUT_RSRC_PATH)\drm_pk_symmetriccrypt.src\
                    $(OUTPUT_RSRC_PATH)\drm_pk_symmetricsign.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_getversionfromstring.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_stringinsertblanksubstring.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_stringremovesubstring.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_dstrsearch.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_dstrsearchreverse.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_numbertostring.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_getversionasstring.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_stringtoguid.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_decodekid.src\
                    $(OUTPUT_RSRC_PATH)\drm_utl_ensuredataalignment.src\
                    $(OUTPUT_RSRC_PATH)\setcountedexpirycategory.src\
                    $(OUTPUT_RSRC_PATH)\overlappingdates.src\
                    $(OUTPUT_RSRC_PATH)\drm_asd_parsev2license.src\
                    $(OUTPUT_RSRC_PATH)\neginfdate.src\
                    $(OUTPUT_RSRC_PATH)\infdate.src\
                    $(OUTPUT_RSRC_PATH)\isexpired.src\
                    $(OUTPUT_RSRC_PATH)\getsecstateattr.src\
                    $(OUTPUT_RSRC_PATH)\setexpirycategory.src\
                    $(OUTPUT_RSRC_PATH)\getv2licenseinfo.src\
                    $(OUTPUT_RSRC_PATH)\getnextlicense.src\
                    $(OUTPUT_RSRC_PATH)\aggregate.src\
                    $(OUTPUT_RSRC_PATH)\drm_asd_getlicenseaggregatedata.src\
                    $(OUTPUT_RSRC_PATH)\_scannodeforattributew.src\
                    $(OUTPUT_RSRC_PATH)\_getxmlnodecdataw.src\
                    $(OUTPUT_RSRC_PATH)\drm_xml_getnodeattribute.src\
                    $(OUTPUT_RSRC_PATH)\drm_xml_getnodecdata.src\
                    $(OUTPUT_RSRC_PATH)\drm_dcp_getattribute.src\
                    $(OUTPUT_RSRC_PATH)\drm_dcp_loadpropertiescache.src\
                    $(OUTPUT_RSRC_PATH)\_verifysymmerticsignature.src\
                    $(OUTPUT_RSRC_PATH)\drm_ddc_getdevicecertificate.src\
                    $(OUTPUT_RSRC_PATH)\_createdevicestore.src\
                    $(OUTPUT_RSRC_PATH)\_freedrmmanagerinternalcontexts.src\
                    $(OUTPUT_RSRC_PATH)\_setuplicevalobjecttoshare.src\
                    $(OUTPUT_RSRC_PATH)\_mapdrmerror.src\
                    $(OUTPUT_RSRC_PATH)\_comparemachineid.src\
                    $(OUTPUT_RSRC_PATH)\initmgrcontext.src\
                    $(OUTPUT_RSRC_PATH)\drm_mgr_initialize.src\
                    $(OUTPUT_RSRC_PATH)\drm_mgr_uninitialize.src\
                    $(OUTPUT_RSRC_PATH)\drm_mgr_setv2header.src\
                    $(OUTPUT_RSRC_PATH)\drm_mgr_bind.src\
                    $(OUTPUT_RSRC_PATH)\drm_mgr_commit.src\
                    $(OUTPUT_RSRC_PATH)\drm_mgr_cleanuplicensestore.src\
                    $(OUTPUT_RSRC_PATH)\januscleandatastore.src\
                    $(OUTPUT_RSRC_PATH)\drm_mtr_openid.src\
                    $(OUTPUT_RSRC_PATH)\drm_mtr_incrementcount.src\
                    $(OUTPUT_RSRC_PATH)\drm_mtr_closecontext.src\
                    $(OUTPUT_RSRC_PATH)\oem_openfile.src\
                    $(OUTPUT_RSRC_PATH)\oem_setendoffile.src\
                    $(OUTPUT_RSRC_PATH)\oem_writefile.src\
                    $(OUTPUT_RSRC_PATH)\oem_genrandombytes.src\
                    $(OUTPUT_RSRC_PATH)\oem_getfallbacksigningkey.src\
                    $(OUTPUT_RSRC_PATH)\oem_getdevicecerttemplate.src\
                    $(OUTPUT_RSRC_PATH)\oem_setdevicecert.src\
                    $(OUTPUT_RSRC_PATH)\oem_getclockresetstate.src\
                    $(OUTPUT_RSRC_PATH)\oem_setclockresetstate.src\
                    $(OUTPUT_RSRC_PATH)\_getdrmfullpathname.src\
                    $(OUTPUT_RSRC_PATH)\oem_getuniqueid.src\
                    $(OUTPUT_RSRC_PATH)\_getprivatekey.src\
                    $(OUTPUT_RSRC_PATH)\oem_getgroupcertprivatekey.src\
                    $(OUTPUT_RSRC_PATH)\_getdevicecert.src\
                    $(OUTPUT_RSRC_PATH)\oem_getdevicecert.src\
                    $(OUTPUT_RSRC_PATH)\oem_getsecurestoreglobalpasswordseed.src\
                    $(OUTPUT_RSRC_PATH)\drm_snc_openstore.src\
                    $(OUTPUT_RSRC_PATH)\drm_snc_closestore.src\
                    $(OUTPUT_RSRC_PATH)\_setkidstoredata.src\
                    $(OUTPUT_RSRC_PATH)\drm_snc_deletekid.src\
                    $(OUTPUT_RSRC_PATH)\drm_snc_updatekid.src\
                    $(OUTPUT_RSRC_PATH)\drm_mgr_setv1header.src\
                    $(OUTPUT_RSRC_PATH)\functiongetvalue_part1.src\
                    $(OUTPUT_RSRC_PATH)\functiongetvalue_fn_datediff.src\
                    $(OUTPUT_RSRC_PATH)\eval.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_opennamespace.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_openslot.src\
                    $(OUTPUT_RSRC_PATH)\_applydiffstostore.src\
                    $(OUTPUT_RSRC_PATH)\_verifyslothash.src\
                    $(OUTPUT_RSRC_PATH)\_hdscreatenamespacestore.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_createstore2.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src\
                    $(OUTPUT_RSRC_PATH)\_hdsprealloc.src\
                    $(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_STFM1000TestPresence.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_I2CWriteTunerRegister.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_I2CReset.src\
                    $(OUTPUT_RSRC_PATH)\oem_systemtimetofiletime.src\
                    $(OUTPUT_RSRC_PATH)\oem_filetimetosystemtime.src\
                    $(OUTPUT_RSRC_PATH)\gendevicecertificate.src\
                    $(OUTPUT_RSRC_PATH)\drmcrt_memmove_mem.src\
                    $(OUTPUT_RSRC_PATH)\copyhdsdtore.src\
                    $(OUTPUT_RSRC_PATH)\getprivkey.src\
                    $(OUTPUT_RSRC_PATH)\generatedevicecert.src\
                    $(OUTPUT_RSRC_PATH)\drm_b64_decodew_inplace.src\
                    $(OUTPUT_RSRC_PATH)\oem_setsystemtime.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_closelockedslot.src\
                    $(OUTPUT_RSRC_PATH)\oem_getdevicedatetime.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_setdata.src\
                    $(OUTPUT_RSRC_PATH)\_systemtime_to_struct_tm.src\
                    $(OUTPUT_RSRC_PATH)\_struct_tm_to_systemtime.src\
                    $(OUTPUT_RSRC_PATH)\_checksecureclock.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_getdata.src\
                    $(OUTPUT_RSRC_PATH)\localtime.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_getlockeddata.src\
                    $(OUTPUT_RSRC_PATH)\variablesecuretimegetorset.src\
                    $(OUTPUT_RSRC_PATH)\_systemtimetotime_t.src\
                    $(OUTPUT_RSRC_PATH)\drm_mtr_updatedata.src\
                    $(OUTPUT_RSRC_PATH)\gmtime.src\
                    $(OUTPUT_RSRC_PATH)\_hdsupdatesrn.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_openandlockslot.src\
                    $(OUTPUT_RSRC_PATH)\_preparesecureclockdataforwriting.src\
                    $(OUTPUT_RSRC_PATH)\strtol.src\
                    $(OUTPUT_RSRC_PATH)\_preparesecureclockdataforreading.src\
                    $(OUTPUT_RSRC_PATH)\drm_sst_setlockeddata.src\
                    $(OUTPUT_RSRC_PATH)\drm_clk_initchecksecureclock.src\
                    $(OUTPUT_RSRC_PATH)\mktime.src\
                    $(OUTPUT_RSRC_PATH)\getbase64decodedkey.src\
                    $(OUTPUT_RSRC_PATH)\_initslot.src\
                    $(OUTPUT_RSRC_PATH)\_loadslot.src\
					$(OUTPUT_RSRC_PATH)\updatevolume.src\
					$(OUTPUT_RSRC_PATH)\Funclet_HalDisplayInit.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SalDisplayInit.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_SalHandleMessage.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_FixBitmapHeader.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_BitmapReadHeader.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_BitmapReadPalette.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_BitmapReadBitMask.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_BitmapPrintNonPalette.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_BitmapPrintPalette.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_DisplayPrintBitmapFile.src\
                    $(OUTPUT_RSRC_PATH)\Funclet_DisplayPrintBitmapResource.src


endif # Make Funclets

#ifdef D_D3410
#RESOURCELIST = $(RESOURCELIST)\
#       $(OUTPUT_RSRC_PATH)\chkdskstartup.src
#endif

RESOURCELIST = $(RESOURCELIST)\
    	$(ROOT)\resources\shingyih\system_lcd_init_seq.src\
	$(OUTPUT_RSRC_PATH)\lcd_controller_init.src\
    $(OUTPUT_RSRC_PATH)\string_spectrogram_menu.src\
    $(OUTPUT_RSRC_PATH)\string_dec_pt5.src\
    $(OUTPUT_RSRC_PATH)\string_dec_pt0.src\
    $(OUTPUT_RSRC_PATH)\string_db.src\
    $(OUTPUT_RSRC_PATH)\string_hz2.src\
    $(OUTPUT_RSRC_PATH)\string_voice.src\
    $(OUTPUT_RSRC_PATH)\string_song.src\
    $(OUTPUT_RSRC_PATH)\string_music_menu.src\
    $(OUTPUT_RSRC_PATH)\string_mvideo_menu.src\
    $(OUTPUT_RSRC_PATH)\string_jpeg_display_menu.src\
    $(OUTPUT_RSRC_PATH)\string_jpeg_manual_menu.src\
    $(OUTPUT_RSRC_PATH)\string_jpeg_slideshow_menu.src\
    $(OUTPUT_RSRC_PATH)\string_jpeg_thumbnail_menu.src\
    $(OUTPUT_RSRC_PATH)\string_voice_menu.src\
    $(OUTPUT_RSRC_PATH)\string_audible_menu.src\
    $(OUTPUT_RSRC_PATH)\string_music_menu_delete_all.src\
    $(OUTPUT_RSRC_PATH)\string_voice_menu_delete_all.src\
    $(OUTPUT_RSRC_PATH)\string_fmtuner_menu.src\
    $(OUTPUT_RSRC_PATH)\string_settings_menu.src\
    $(OUTPUT_RSRC_PATH)\string_eq_menu.src\
    $(OUTPUT_RSRC_PATH)\string_10min_menu.src\
    $(OUTPUT_RSRC_PATH)\string_1min_menu.src\
    $(OUTPUT_RSRC_PATH)\string_2min_menu.src\
    $(OUTPUT_RSRC_PATH)\string_5min_menu.src\
    $(OUTPUT_RSRC_PATH)\string_10min_menu.src\
    $(OUTPUT_RSRC_PATH)\string_classic_menu.src\
    $(OUTPUT_RSRC_PATH)\string_contrast_menu.src\
    $(OUTPUT_RSRC_PATH)\string_disable_menu.src\
    $(OUTPUT_RSRC_PATH)\string_exit_menu.src\
    $(OUTPUT_RSRC_PATH)\string_jazz_menu.src\
    $(OUTPUT_RSRC_PATH)\string_normal_menu.src\
    $(OUTPUT_RSRC_PATH)\string_playmode_menu.src\
    $(OUTPUT_RSRC_PATH)\string_pop_menu.src\
    $(OUTPUT_RSRC_PATH)\string_pwrsavings_menu.src\
    $(OUTPUT_RSRC_PATH)\string_repeat1_menu.src\
    $(OUTPUT_RSRC_PATH)\string_repeatall_menu.src\
    $(OUTPUT_RSRC_PATH)\string_repeatshuffle_menu.src\
    $(OUTPUT_RSRC_PATH)\string_rock_menu.src\
    $(OUTPUT_RSRC_PATH)\string_shuffle_menu.src\
    $(OUTPUT_RSRC_PATH)\string_system_menu.src\
    $(OUTPUT_RSRC_PATH)\string_about_menu.src\
    $(OUTPUT_RSRC_PATH)\string_backlight_menu.src\
    $(OUTPUT_RSRC_PATH)\string_delete_menu.src\
    $(OUTPUT_RSRC_PATH)\string_record_menu.src\
    $(OUTPUT_RSRC_PATH)\string_mb.src\
    $(OUTPUT_RSRC_PATH)\string_end_of_slide_show.src\
    $(OUTPUT_RSRC_PATH)\settings_title.src\
    $(OUTPUT_RSRC_PATH)\jpeg_display_title.src\
    $(OUTPUT_RSRC_PATH)\erase_title.src\
    $(OUTPUT_RSRC_PATH)\string_record_settings.src\
    $(OUTPUT_RSRC_PATH)\string_sample_rate.src\
    $(OUTPUT_RSRC_PATH)\string_device.src\
    $(OUTPUT_RSRC_PATH)\string_encoder.src\
    $(OUTPUT_RSRC_PATH)\string_internal.src\
    $(OUTPUT_RSRC_PATH)\string_external.src\
    $(OUTPUT_RSRC_PATH)\string_adpcm.src\
    $(OUTPUT_RSRC_PATH)\string_pcm.src\
    $(OUTPUT_RSRC_PATH)\string_msadpcm.src\
    $(OUTPUT_RSRC_PATH)\string_imadpcm.src\
    $(OUTPUT_RSRC_PATH)\string_location.src\
    $(OUTPUT_RSRC_PATH)\string_source.src\
    $(OUTPUT_RSRC_PATH)\string_microphone.src\
    $(OUTPUT_RSRC_PATH)\string_linein.src\
    $(OUTPUT_RSRC_PATH)\string_fm.src\
    $(OUTPUT_RSRC_PATH)\string_source.src\
    $(OUTPUT_RSRC_PATH)\string_channels.src\
    $(OUTPUT_RSRC_PATH)\string_bits.src\
    $(OUTPUT_RSRC_PATH)\string_4.src\
    $(OUTPUT_RSRC_PATH)\string_8.src\
    $(OUTPUT_RSRC_PATH)\string_16.src\
    $(OUTPUT_RSRC_PATH)\string_24.src\
    $(OUTPUT_RSRC_PATH)\string_mono.src\
    $(OUTPUT_RSRC_PATH)\string_stereo.src\
    $(OUTPUT_RSRC_PATH)\string_8000hz.src\
    $(OUTPUT_RSRC_PATH)\string_11025hz.src\
    $(OUTPUT_RSRC_PATH)\string_16000hz.src\
    $(OUTPUT_RSRC_PATH)\string_22050hz.src\
    $(OUTPUT_RSRC_PATH)\string_32000hz.src\
    $(OUTPUT_RSRC_PATH)\string_44100hz.src\
    $(OUTPUT_RSRC_PATH)\string_48000hz.src\
    $(OUTPUT_RSRC_PATH)\string_96000hz.src\
    $(OUTPUT_RSRC_PATH)\string_112000hz.src\
    $(OUTPUT_RSRC_PATH)\string_128000hz.src\
    $(OUTPUT_RSRC_PATH)\string_160000hz.src\
    $(OUTPUT_RSRC_PATH)\string_192000hz.src\
    $(OUTPUT_RSRC_PATH)\string_224000hz.src\
    $(OUTPUT_RSRC_PATH)\string_256000hz.src\
    $(OUTPUT_RSRC_PATH)\string_320000hz.src\
    $(OUTPUT_RSRC_PATH)\string_hz.src\
    $(OUTPUT_RSRC_PATH)\string_bit_rate.src\
    $(OUTPUT_RSRC_PATH)\string_spaces.src\
    $(OUTPUT_RSRC_PATH)\string_album.src\
    $(OUTPUT_RSRC_PATH)\string_encoder_song.src\
    $(OUTPUT_RSRC_PATH)\string_mode.src\
    $(OUTPUT_RSRC_PATH)\string_no_files.src\
    $(OUTPUT_RSRC_PATH)\string_working.src\
    $(OUTPUT_RSRC_PATH)\play_icon_with_border.src\
    $(OUTPUT_RSRC_PATH)\pause_icon_with_border.src\
    $(OUTPUT_RSRC_PATH)\stop_icon_with_border.src\
    $(OUTPUT_RSRC_PATH)\record_icon_with_border.src\
    $(OUTPUT_RSRC_PATH)\paused_record_icon_with_border.src\
    $(OUTPUT_RSRC_PATH)\ffwd_icon_with_border.src\
    $(OUTPUT_RSRC_PATH)\rwnd_icon_with_border.src\
    \
    $(OUTPUT_RSRC_PATH)\del_warning_no.src\
    $(OUTPUT_RSRC_PATH)\del_warning_yes.src\
    $(OUTPUT_RSRC_PATH)\del_warning_line1.src\
    $(OUTPUT_RSRC_PATH)\del_warning_line2.src\
    $(OUTPUT_RSRC_PATH)\del_all_warning_line2.src\
    $(OUTPUT_RSRC_PATH)\del_all_file_star.src \
    $(OUTPUT_RSRC_PATH)\vbr.src\
    $(OUTPUT_RSRC_PATH)\bitmap_warning.src\
    $(OUTPUT_RSRC_PATH)\bitmap_device_full.src\
    $(OUTPUT_RSRC_PATH)\internal_media.src\
    $(OUTPUT_RSRC_PATH)\external_media.src\
    $(OUTPUT_RSRC_PATH)\about_title.src\
    $(OUTPUT_RSRC_PATH)\backlight_title.src\
    $(OUTPUT_RSRC_PATH)\backlight_state_on.src\
    $(OUTPUT_RSRC_PATH)\backlight_state_off.src\
    $(OUTPUT_RSRC_PATH)\player_name.src\
    $(OUTPUT_RSRC_PATH)\repeatsong_icon.src\
    $(OUTPUT_RSRC_PATH)\shuffle_icon.src\
    $(OUTPUT_RSRC_PATH)\random_icon.src\
    $(OUTPUT_RSRC_PATH)\repeatall_icon.src\
    $(OUTPUT_RSRC_PATH)\repeatallclear_icon.src\
    $(OUTPUT_RSRC_PATH)\repeatsongclear_icon.src\
    $(OUTPUT_RSRC_PATH)\shuffleclear_icon.src\
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
    $(OUTPUT_RSRC_PATH)\battery_charging.src\
    $(OUTPUT_RSRC_PATH)\disk_small.src\
    $(OUTPUT_RSRC_PATH)\disk_full.src\
    $(OUTPUT_RSRC_PATH)\lock_small.src\
    $(OUTPUT_RSRC_PATH)\icon_music_mode.src\
    $(OUTPUT_RSRC_PATH)\icon_voice_mode.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_00.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_01.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_02.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_03.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_04.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_05.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_06.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_07.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_08.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_09.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_10.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_11.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_12.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_13.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_14.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_15.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_16.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_17.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_18.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_19.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_20.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_21.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_22.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_23.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_24.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_25.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_26.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_27.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_28.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_29.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_30.src\
    $(OUTPUT_RSRC_PATH)\icon_vol_31.src\
    $(OUTPUT_RSRC_PATH)\contrast_title.src\
    $(OUTPUT_RSRC_PATH)\contrast_frame.src\
    $(OUTPUT_RSRC_PATH)\contrast_level0.src\
    $(OUTPUT_RSRC_PATH)\contrast_level1.src\
    $(OUTPUT_RSRC_PATH)\contrast_level2.src\
    $(OUTPUT_RSRC_PATH)\contrast_level3.src\
    $(OUTPUT_RSRC_PATH)\contrast_level4.src\
    $(OUTPUT_RSRC_PATH)\contrast_level5.src\
    $(OUTPUT_RSRC_PATH)\contrast_level6.src\
    $(OUTPUT_RSRC_PATH)\contrast_level7.src\
    $(OUTPUT_RSRC_PATH)\contrast_level8.src\
    $(OUTPUT_RSRC_PATH)\contrast_level9.src\
    $(OUTPUT_RSRC_PATH)\contrast_level10.src\
    $(OUTPUT_RSRC_PATH)\st_bw1.src\
    $(OUTPUT_RSRC_PATH)\locked.src\
    $(OUTPUT_RSRC_PATH)\eq_clear_icon.src\
    $(OUTPUT_RSRC_PATH)\rock_icon.src\
    $(OUTPUT_RSRC_PATH)\jazz_icon.src\
    $(OUTPUT_RSRC_PATH)\classic_icon.src\
    $(OUTPUT_RSRC_PATH)\pop_icon.src\
    $(OUTPUT_RSRC_PATH)\ab_mark_a.src\
    $(OUTPUT_RSRC_PATH)\ab_mark_b.src\
    $(OUTPUT_RSRC_PATH)\lowbattery.src\
    $(OUTPUT_RSRC_PATH)\string_arial_8_continue_holding_to_power_off.src\
    $(OUTPUT_RSRC_PATH)\string_arial_8_defragmenting_store.src\
    $(OUTPUT_RSRC_PATH)\status_16_6_steps_0.src\
    $(OUTPUT_RSRC_PATH)\status_16_6_steps_1.src\
    $(OUTPUT_RSRC_PATH)\status_16_6_steps_2.src\
    $(OUTPUT_RSRC_PATH)\status_16_6_steps_3.src\
    $(OUTPUT_RSRC_PATH)\status_16_6_steps_4.src\
    $(OUTPUT_RSRC_PATH)\status_16_6_steps_5.src\
    $(OUTPUT_RSRC_PATH)\status_16_6_steps_6.src\
    $(OUTPUT_RSRC_PATH)\delete_successful.src\
    $(OUTPUT_RSRC_PATH)\lic_expired.src\
    $(OUTPUT_RSRC_PATH)\disk_full.src\
    $(OUTPUT_RSRC_PATH)\delete_error.src\
    $(OUTPUT_RSRC_PATH)\exmediaerror1.src\
    $(OUTPUT_RSRC_PATH)\exmediaerror2.src\
    $(OUTPUT_RSRC_PATH)\inmediaerror1.src\
    $(OUTPUT_RSRC_PATH)\mediastartup.src\
    $(OUTPUT_RSRC_PATH)\nanddatadriveinit.src\
    $(OUTPUT_RSRC_PATH)\low_level_pwr_line1.src\
    $(OUTPUT_RSRC_PATH)\low_level_pwr_line2.src\
    $(OUTPUT_RSRC_PATH)\custom_icon.src\
    $(OUTPUT_RSRC_PATH)\negative_sign.src\
    $(OUTPUT_RSRC_PATH)\slider_bar.src\
    $(OUTPUT_RSRC_PATH)\slider_bar_inv.src\
    $(OUTPUT_RSRC_PATH)\slider_track.src\
    $(OUTPUT_RSRC_PATH)\string_searching.src\
    $(OUTPUT_RSRC_PATH)\check_disk_1.src\
    $(OUTPUT_RSRC_PATH)\check_disk_2.src\
    $(OUTPUT_RSRC_PATH)\check_disk_3.src\
    $(OUTPUT_RSRC_PATH)\flash_error.src\
    $(OUTPUT_RSRC_PATH)\siglogo1.src\
    $(OUTPUT_RSRC_PATH)\siglogo2.src\
    $(OUTPUT_RSRC_PATH)\siglogo3.src\
    $(OUTPUT_RSRC_PATH)\siglogo4.src\
    $(OUTPUT_RSRC_PATH)\siglogo5.src\
    $(OUTPUT_RSRC_PATH)\siglogo6.src\
    $(OUTPUT_RSRC_PATH)\siglogo7.src\
    $(OUTPUT_RSRC_PATH)\siglogo8.src\
    $(OUTPUT_RSRC_PATH)\siglogo9.src\
    $(OUTPUT_RSRC_PATH)\siglogo10.src\
    $(OUTPUT_RSRC_PATH)\siglogo11.src\
    $(OUTPUT_RSRC_PATH)\siglogo12.src\
    $(OUTPUT_RSRC_PATH)\siglogo13.src\
    $(OUTPUT_RSRC_PATH)\siglogo.src\
    $(OUTPUT_RSRC_PATH)\font_PGM.src\
    $(OUTPUT_RSRC_PATH)\font_SGMs.src\
    $(OUTPUT_RSRC_PATH)\font_script_00.src\
    $(OUTPUT_RSRC_PATH)\font_scripts.src\
    $(OUTPUT_RSRC_PATH)\font_PDM.src\
    $(OUTPUT_RSRC_PATH)\font_SDMs.src\
    \
    	$(OUTPUT_RSRC_PATH)\icon_folder.src\
    	$(OUTPUT_RSRC_PATH)\icon_song.src\
	$(OUTPUT_RSRC_PATH)\menu_about.src\
    	$(OUTPUT_RSRC_PATH)\menu_clock.src\
    	$(OUTPUT_RSRC_PATH)\menu_exit.src\
    	$(OUTPUT_RSRC_PATH)\menu_fmtuner.src\
    	$(OUTPUT_RSRC_PATH)\menu_music.src\
    	$(OUTPUT_RSRC_PATH)\menu_record.src\
    	$(OUTPUT_RSRC_PATH)\menu_settings.src\
    	$(OUTPUT_RSRC_PATH)\menu_voice.src\
    	$(OUTPUT_RSRC_PATH)\menu_delete.src\
    	$(OUTPUT_RSRC_PATH)\menu_ab.src\
    	$(OUTPUT_RSRC_PATH)\menu_shutdown.src\
    	$(OUTPUT_RSRC_PATH)\music_play_all.src\
    	$(OUTPUT_RSRC_PATH)\music_folder_internal.src\
    	$(OUTPUT_RSRC_PATH)\music_folder_external.src\
    	$(OUTPUT_RSRC_PATH)\music_songs.src\
    	$(OUTPUT_RSRC_PATH)\music_favorites.src\
    	$(OUTPUT_RSRC_PATH)\music_fm_record.src\
    	$(OUTPUT_RSRC_PATH)\music_exit.src\
    	$(OUTPUT_RSRC_PATH)\browse_music_folder_internal.src\
    	$(OUTPUT_RSRC_PATH)\browse_music_folder_external.src\
    	$(OUTPUT_RSRC_PATH)\browse_music_list_songs.src\
    	$(OUTPUT_RSRC_PATH)\browse_music_favourites.src\
    	$(OUTPUT_RSRC_PATH)\browse_music_fm_files.src\
    	$(OUTPUT_RSRC_PATH)\browse_voice.src\
    	$(OUTPUT_RSRC_PATH)\favourites_list_add.src\
	$(OUTPUT_RSRC_PATH)\favourites_list_remove.src\
	$(OUTPUT_RSRC_PATH)\favourites_list_is_full.src\
	$(OUTPUT_RSRC_PATH)\about_screen_1.src\
   	$(OUTPUT_RSRC_PATH)\about_screen_2.src\
   	$(OUTPUT_RSRC_PATH)\about_screen_3.src\
   	$(OUTPUT_RSRC_PATH)\about_screen_4.src\
   	$(OUTPUT_RSRC_PATH)\time_date_exit_title.src \ 
   	$(OUTPUT_RSRC_PATH)\time_clean_desktop.src\
   	$(OUTPUT_RSRC_PATH)\time_dash.src\
   	$(OUTPUT_RSRC_PATH)\time_day_2.src\
   	$(OUTPUT_RSRC_PATH)\time_day_3.src\
   	$(OUTPUT_RSRC_PATH)\time_day_4.src\
   	$(OUTPUT_RSRC_PATH)\time_day_5.src\
   	$(OUTPUT_RSRC_PATH)\time_day_6.src\
   	$(OUTPUT_RSRC_PATH)\time_day_7.src\
   	$(OUTPUT_RSRC_PATH)\time_day_cn.src\
   	$(OUTPUT_RSRC_PATH)\time_month_1.src\
   	$(OUTPUT_RSRC_PATH)\time_month_2.src\
   	$(OUTPUT_RSRC_PATH)\time_month_3.src\
   	$(OUTPUT_RSRC_PATH)\time_month_4.src\
   	$(OUTPUT_RSRC_PATH)\time_month_5.src\
   	$(OUTPUT_RSRC_PATH)\time_month_6.src\
   	$(OUTPUT_RSRC_PATH)\time_month_7.src\
   	$(OUTPUT_RSRC_PATH)\time_month_8.src\
   	$(OUTPUT_RSRC_PATH)\time_month_9.src\
   	$(OUTPUT_RSRC_PATH)\time_month_10.src\
   	$(OUTPUT_RSRC_PATH)\time_month_11.src\
   	$(OUTPUT_RSRC_PATH)\time_month_12.src\
   	$(OUTPUT_RSRC_PATH)\time_num_am.src\
   	$(OUTPUT_RSRC_PATH)\time_num_pm.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_0.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_1.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_2.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_3.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_4.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_5.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_6.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_7.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_8.src\
   	$(OUTPUT_RSRC_PATH)\time_num_large_9.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_0.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_1.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_2.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_3.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_4.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_5.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_6.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_7.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_8.src\
   	$(OUTPUT_RSRC_PATH)\time_num_medium_9.src\
   	$(OUTPUT_RSRC_PATH)\time_colon.src\
   	$(OUTPUT_RSRC_PATH)\fraction_dot.src\
   	$(OUTPUT_RSRC_PATH)\fm_background.src\
   	$(OUTPUT_RSRC_PATH)\searching_please_wait.src\
   	$(OUTPUT_RSRC_PATH)\fm_auto_search.src\
   	\
   	$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_title.src\
    	$(OUTPUT_RSRC_PATH)\settings_backlight_title.src\
    	$(OUTPUT_RSRC_PATH)\settings_contrast_title.src\
    	$(OUTPUT_RSRC_PATH)\settings_eq_title.src\
    	$(OUTPUT_RSRC_PATH)\settings_exit_title.src\
    	$(OUTPUT_RSRC_PATH)\settings_playmode_title.src\
    	$(OUTPUT_RSRC_PATH)\settings_set_date_title.src\
    	$(OUTPUT_RSRC_PATH)\settings_set_time_title.src\
   	$(OUTPUT_RSRC_PATH)\settings_playmode_normal.src\
   	$(OUTPUT_RSRC_PATH)\settings_playmode_repeatone.src\
   	$(OUTPUT_RSRC_PATH)\settings_playmode_repeatall.src\
   	$(OUTPUT_RSRC_PATH)\settings_playmode_shuffle.src\
   	$(OUTPUT_RSRC_PATH)\settings_playmode_shuffle_repeat.src\
   	$(OUTPUT_RSRC_PATH)\settings_backlight_on.src \
   	$(OUTPUT_RSRC_PATH)\settings_backlight_10s.src \
   	$(OUTPUT_RSRC_PATH)\settings_backlight_20s.src \
   	$(OUTPUT_RSRC_PATH)\settings_backlight_30s.src \
   	$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_disable.src \
   	$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_1minute.src \
   	$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_2minutes.src \
   	$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_5minutes.src \
   	$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_10minutes.src \  
   	$(OUTPUT_RSRC_PATH)\settings_languages_eng.src\
   	$(OUTPUT_RSRC_PATH)\settings_languages_vie.src\
   	$(OUTPUT_RSRC_PATH)\settime_format_12h.src\
   	$(OUTPUT_RSRC_PATH)\settime_format_24h.src\
   	$(OUTPUT_RSRC_PATH)\setdate_format_dmy.src\
   	$(OUTPUT_RSRC_PATH)\setdate_format_mdy.src\
   	$(OUTPUT_RSRC_PATH)\setdate_format_ymd.src\
    	$(OUTPUT_RSRC_PATH)\jvj_shutdown_player.src\
    	\
    	$(OUTPUT_RSRC_PATH)\vie_menu_about.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_clock.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_exit.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_fmtuner.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_music.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_record.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_settings.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_voice.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_delete.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_ab.src\
    	$(OUTPUT_RSRC_PATH)\vie_menu_shutdown.src\
    	$(OUTPUT_RSRC_PATH)\vie_music_play_all.src\
    	$(OUTPUT_RSRC_PATH)\vie_music_folder_internal.src\
    	$(OUTPUT_RSRC_PATH)\vie_music_folder_external.src\
    	$(OUTPUT_RSRC_PATH)\vie_music_songs.src\
    	$(OUTPUT_RSRC_PATH)\vie_music_favorites.src\
    	$(OUTPUT_RSRC_PATH)\vie_music_fm_record.src\
    	$(OUTPUT_RSRC_PATH)\vie_music_exit.src\
    	$(OUTPUT_RSRC_PATH)\vie_browse_music_folder_internal.src\
    	$(OUTPUT_RSRC_PATH)\vie_browse_music_folder_external.src\
    	$(OUTPUT_RSRC_PATH)\vie_browse_music_list_songs.src\
    	$(OUTPUT_RSRC_PATH)\vie_browse_music_favourites.src\
    	$(OUTPUT_RSRC_PATH)\vie_browse_music_fm_files.src\
    	$(OUTPUT_RSRC_PATH)\vie_browse_voice.src\
    	$(OUTPUT_RSRC_PATH)\vie_favourites_list_add.src\
	$(OUTPUT_RSRC_PATH)\vie_favourites_list_remove.src\
	$(OUTPUT_RSRC_PATH)\vie_favourites_list_is_full.src\
	$(OUTPUT_RSRC_PATH)\vie_fm_background.src\
	$(OUTPUT_RSRC_PATH)\vie_searching_please_wait.src\
	$(OUTPUT_RSRC_PATH)\vie_fm_auto_search.src\
	$(OUTPUT_RSRC_PATH)\vie_about_screen_1.src\
   	$(OUTPUT_RSRC_PATH)\vie_about_screen_2.src\
   	$(OUTPUT_RSRC_PATH)\vie_about_screen_3.src\
   	$(OUTPUT_RSRC_PATH)\vie_about_screen_4.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_date_exit_title.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_day_2.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_day_3.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_day_4.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_day_5.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_day_6.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_day_7.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_day_cn.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_1.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_2.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_3.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_4.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_5.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_6.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_7.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_8.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_9.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_10.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_11.src\
   	$(OUTPUT_RSRC_PATH)\vie_time_month_12.src\
   	\
   	$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_1min.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_2min.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_5min.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_10min.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_disable.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_backlight_10s.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_backlight_20s.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_backlight_30s.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_backlight_on.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_contrast_title.src\
  	$(OUTPUT_RSRC_PATH)\vie_settings_eq_title.src\
  	$(OUTPUT_RSRC_PATH)\vie_settings_exit_title.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_playmode_normal.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_playmode_repeatall.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_playmode_repeatone.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_playmode_shuffle.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_playmode_shuffle_repeat.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_set_date_title.src\
   	$(OUTPUT_RSRC_PATH)\vie_settings_set_time_title.src\
    	$(OUTPUT_RSRC_PATH)\vie_jvj_shutdown_player.src
    

RESOURCELIST = $(RESOURCELIST)\
    $(OUTPUT_RSRC_PATH)\mmcmediastartup.src\
    $(OUTPUT_RSRC_PATH)\mmcerase.src \
    $(OUTPUT_RSRC_PATH)\mmcinfo.src \
    $(OUTPUT_RSRC_PATH)\mmcread.src \
    $(OUTPUT_RSRC_PATH)\mmcdatadriveerase.src \
    $(OUTPUT_RSRC_PATH)\mmcmediainit.src \
    $(OUTPUT_RSRC_PATH)\mmcdatadriveinfo.src \
    $(OUTPUT_RSRC_PATH)\mmcreadwrite.src \
    $(OUTPUT_RSRC_PATH)\mmcenumerate.src \
    $(OUTPUT_RSRC_PATH)\mmcresetdevice.src \
    $(OUTPUT_RSRC_PATH)\mmcprocesscsd.src \
    $(OUTPUT_RSRC_PATH)\mmcprocesscid.src \
    $(OUTPUT_RSRC_PATH)\mmcprocesscid2.src \
    $(OUTPUT_RSRC_PATH)\mmcdetectpresence.src \
    $(OUTPUT_RSRC_PATH)\mmcserialnumberinit.src \
    $(OUTPUT_RSRC_PATH)\mmccheckwriteprotect.src \
    $(OUTPUT_RSRC_PATH)\Funclet_mmctesterase.src \
    $(OUTPUT_RSRC_PATH)\Funclet_mmchalinit.src

ifdef D_FM_PHILIPS
FUNCLETSRCLIST=$(FUNCLETSRCLIST)\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncUp.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncDown.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverStandyby.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverReplacePreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverErasePreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverTuneToPreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSearchDown.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000SafePowerDown.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000OptimizeChannel.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000I2CInit.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverScanStations.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSetMonoStereoMode.src\
            $(OUTPUT_RSRC_PATH)\Funclet_STFM1000SetLnaBias.src\
            $(OUTPUT_RSRC_PATH)\Funclet_STFM1000LookForPilot.src\
            $(OUTPUT_RSRC_PATH)\Funclet_STFM1000TunerSearchEnded.src\
            $(OUTPUT_RSRC_PATH)\decstfmmod.src\
            $(OUTPUT_RSRC_PATH)\dec_stfmp.src\
            $(OUTPUT_RSRC_PATH)\dec_stfmx.src\
            $(OUTPUT_RSRC_PATH)\dec_stfmy.src\

endif

ifdef D_REMOVE_FM

FUNCLETSRCLIST=$(FUNCLETSRCLIST)\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverInit.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp1.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp2.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000FindRssiThreshold.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000DataPathOff.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000ContinueSearching.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncUp.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncDown.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverStandyby.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverReplacePreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverErasePreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverTuneToPreset.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSearchDown.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000SafePowerDown.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000OptimizeChannel.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000I2CInit.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverScanStations.src\
            $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSetMonoStereoMode.src\
            $(OUTPUT_RSRC_PATH)\Funclet_STFM1000SetLnaBias.src\
            $(OUTPUT_RSRC_PATH)\Funclet_STFM1000LookForPilot.src\
            $(OUTPUT_RSRC_PATH)\Funclet_STFM1000TunerSearchEnded.src\
            $(OUTPUT_RSRC_PATH)\decstfmmod.src\
            $(OUTPUT_RSRC_PATH)\dec_stfmp.src\
            $(OUTPUT_RSRC_PATH)\dec_stfmx.src\
            $(OUTPUT_RSRC_PATH)\dec_stfmy.src\
            $(OUTPUT_RSRC_PATH)\TunerModule.src\
            $(OUTPUT_RSRC_PATH)\fmtunersecondrsrc.src\

endif # D_REMOVE_FM

MMC : $(OUTPUT_PATH) $(OBJ_PATH) $(LIST_PATH) $(OUTPUT_RSRC_PATH) $(OUTPUT_INCLUDE_PATH) $(OUTPUT_RESOURCE_BIN) $(OUTPUT_STMPSYS_S)

SM  : $(OUTPUT_PATH) $(OBJ_PATH) $(LIST_PATH) $(OUTPUT_RSRC_PATH) $(OUTPUT_INCLUDE_PATH) $(OUTPUT_RESOURCE_BIN) $(OUTPUT_STMPSYS_S)
ifdef D_JANUS
ALL : $(OUTPUT_OEM_DATA) $(OUTPUT_RESOURCE_BIN) $(OUTPUT_STMPSYS_S)
else
ALL : $(OUTPUT_RESOURCE_BIN) $(OUTPUT_STMPSYS_S)
endif

######################################
# Note: These 3 are mutually exclusive, use only one out of three at any build of firmware
#USE_PLAYLIST1 = TRUE
#USE_PLAYLIST2 = TRUE
#USE_PLAYLIST3 = TRUE
ifdef D_PL2
USE_PLAYLIST2 = TRUE
else
ifdef D_PL5
USE_PLAYLIST5 = TRUE
else
USE_PLAYLIST3 = TRUE
endif
endif
######################################

######################################
#IO functions
#include $(RELBASE_PATH)\etc\iolib.mk
######################################

######################################
#include the stringlib make file
include $(RELBASE_PATH)\etc\stringlib.mk
######################################

######################################
#include the system makefile
include $(RELBASE_PATH)\etc\system.mk
######################################


ifdef D_MMC
######################################
#include the MMC Data Drive makefile
include $(RELBASE_PATH)\etc\MMCDataDrive.mk
######################################

######################################
#include the MMC Media makefile
include $(RELBASE_PATH)\etc\MMCMedia.mk
######################################
endif


######################################
#include the filesystem makefile
include $(RELBASE_PATH)\etc\fat32.mk
######################################

######################################
#include the STFM decoder makefile
ifdef STFM1000_BUILD
include $(RELBASE_PATH)\etc\stfmdec.mk
endif
######################################

######################################
#include the ADPCM Common makefile
include $(RELBASE_PATH)\etc\adpcmcommon.mk
######################################

######################################
#include the ADPCM decoder makefile
include $(RELBASE_PATH)\etc\adpcmdec.mk
######################################

######################################
#include the ADPCM encoder makefile
include $(RELBASE_PATH)\etc\adpcmenc.mk
######################################

######################################
#include the MP3 decoder makefile
include $(RELBASE_PATH)\etc\mp3dec.mk
######################################

######################################
#include the SMVADPCM decoder makefile
include $(RELBASE_PATH)\etc\smvadpcmdec.mk
######################################

######################################
#include the Jpeg decoder makefile
include $(RELBASE_PATH)\etc\jpegdec.mk

######################################
#include the SmvJpeg decoder makefile
include $(RELBASE_PATH)\etc\smvjpegdec.mk
######################################

######################################

######################################
# Add WMA Build Files
#include the WMA decoder makefile
include $(RELBASE_PATH)\etc\wmadec.mk
######################################

######################################
# Add Audible ACELP Build Files
#include the ACELP decoder makefile
include $(RELBASE_PATH)\etc\AudibleAcelpdec.mk
######################################

######################################
#include the Audible MP3 decoder makefile
include $(RELBASE_PATH)\etc\audiblemp3dec.mk
######################################

######################################
# SRS WOW Processing
include $(RELBASE_PATH)\etc\srswow.mk
######################################

######################################
# Add janus Build Files
#include the Janus decoder makefile
include $(RELBASE_PATH)\etc\janus.mk
######################################

######################################
#include the exec makefile
include $(RELBASE_PATH)\etc\exec.mk
######################################
#include $(RELBASE_PATH)\etc\overlay.mk

###### FM Tuner select ######
#include the FMTuner makefile
#### By default we build with the SigmaTel STFM1000 FM tuner IC support
###  but you can select philips 5767 from player.bat.

ifdef D_FM_PHILIPS
include $(RELBASE_PATH)\etc\Philips5767.mk
#include $(RELBASE_PATH)\etc\Philips5757.mk
endif


#if FM_SANYO   # Was last supported in SDK3.101, not in 3.110 or 3.120
#include $(RELBASE_PATH)\etc\Sanyo24000.mk
#else
ifdef STFM1000_BUILD
include $(RELBASE_PATH)\etc\STFM1000.mk
include $(RELBASE_PATH)\etc\i2s.mk
endif
#endif

######################################
ifdef D_DIS_FASTBOOT
SECOND_FAT_OP = TRUE
endif
######################################

######################################
#Include RTC
include $(RELBASE_PATH)\etc\rtc.mk
######################################

######################################
#Playlist library

ifdef USE_PLAYLIST1
BLD=$(BLD) -DUSE_PLAYLIST1
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DUSE_PLAYLIST1
include $(RELBASE_PATH)\etc\playlist1.mk
endif

ifdef USE_PLAYLIST2
BLD=$(BLD) -DUSE_PLAYLIST2
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DUSE_PLAYLIST2
BLD=$(BLD) -DPERFORM_SORT
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DPERFORM_SORT

#BLD=$(BLD) -DDEBUG_SFN
#C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DDEBUG_SFN

include $(RELBASE_PATH)\etc\playlist2.mk
endif

ifdef USE_PLAYLIST3
BLD=$(BLD) -DUSE_PLAYLIST3
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DUSE_PLAYLIST3
include $(RELBASE_PATH)\etc\playlist3.mk
#include $(RELBASE_PATH)\etc\musiclib.mk
endif

ifdef USE_PLAYLIST5
BLD=$(BLD) -DUSE_PLAYLIST5
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DUSE_PLAYLIST5
include $(RELBASE_PATH)\etc\playlist5.mk
endif
######################################

######################################
#Record implementation
include $(RELBASE_PATH)\etc\sysrecord.mk
######################################

######################################
#Display module
ifdef D_CLCD
include $(RELBASE_PATH)\etc\display_color.mk
include $(RELBASE_PATH)\etc\samsung_s6b33b0a.mk
include $(RELBASE_PATH)\etc\colorlcdsal.mk
else
include $(RELBASE_PATH)\etc\display.mk
include $(RELBASE_PATH)\etc\epson_sed15xx.mk
include $(RELBASE_PATH)\etc\framebuffer.mk
#include $(RELBASE_PATH)\etc\directtolcd.mk
endif
######################################

######################################
#include the MP3 encoder makefile
#build option inside will makefile so null resources are created
include $(RELBASE_PATH)\etc\mp3enc.mk
######################################
######################################
#include the record test makefile
include $(ETC_PATH)\test.mk
######################################

######################################
#include the record instrumentation makefile
include $(ETC_PATH)\instrument.mk
######################################

######################################
#include the battery component
include $(ETC_PATH)\battery.mk
######################################

ifdef INCLUDE_CHKDSK
######################################
#include the checkdisk component
ifdef SECOND_FAT_OP
BLD=$(BLD) -DSECOND_FAT_OP
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSECOND_FAT_OP
endif
include $(ETC_PATH)\chkdsk.mk
######################################
endif

######################################
#include the Sync Lyrics module
include $(ETC_PATH)\synclyrics.mk
######################################

#///////////////////////////////////////////////////////////////////////////////
# Linker links obj's and .a libs to create player.out
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).out" : $(PROJ).mk $(HWEQULIST) $(PROJECT_H) $(MODLIST) $(OBJLIST) $(ALLLIBLIST)
    @echo Linking $(PROJ).out
    $(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(match .a $!) $(match .sym $!)

#///////////////////////////////////////////////////////////////////////////////
# Locate sections from .out file to an .abs file per .dsc spec
# Note: only player.abs clusters contribute to stmpsys.s/sb so make sure your static section is in player.abs
STMP3500_DSC_LIST = $(CPU_PATH)\P_FuncletCluster.dsc            \
                    $(CPU_PATH)\P_MediaDDCluster.dsc            \
                    $(CPU_PATH)\X_AppCluster.dsc                \
                    $(CPU_PATH)\X_LBlock.dsc                    \
                    $(CPU_PATH)\Y_LBlock.dsc                    \
                    $(CPU_PATH)\ycopy.dsc                       \
                    $(CPU_PATH)\copy1.dsc                       \
                    $(CPU_PATH)\copy2.dsc                       \
                    $(CPU_PATH)\copy3.dsc                       \
                    $(CPU_PATH)\copy4.dsc                       \
                    $(CPU_PATH)\copy5.dsc                       \
                    $(CPU_PATH)\copy6.dsc                       \
                    $(CPU_PATH)\copy7.dsc                       \
                    $(CPU_PATH)\copy8.dsc                       \
                    $(CPU_PATH)\copy9.dsc                       \
                    $(CPU_PATH)\copy10.dsc                      \
                    $(CPU_PATH)\copy11.dsc                      \
                    $(CPU_PATH)\copy12.dsc                      \
                    $(CPU_PATH)\copy13.dsc                      \
                    $(CPU_PATH)\copy14.dsc                      \
                    $(CPU_PATH)\copy15.dsc                      \
                    $(CPU_PATH)\copy16.dsc                      \
                    $(CPU_PATH)\copy17.dsc                      \
                    $(CPU_PATH)\copy18.dsc                      \
                    $(CPU_PATH)\copy19.dsc                      \
                    $(CPU_PATH)\copy20.dsc                      \
                    $(CPU_PATH)\copy21.dsc                      \
                    $(CPU_PATH)\copylast.dsc                    \
                    $(CPU_PATH)\LSpace.dsc                      \
                    $(CPU_PATH)\P_Modules.dsc                   \
                    $(CPU_PATH)\SystemCodebanks.dsc             \
                    local_copies.dsc                            \
                    local_copy1.dsc                             \
                    local_copy2.dsc                             

ifdef USE_PLAYLIST2
STMP3500_DSC_LIST = $(STMP3500_DSC_LIST)\
                    $(CPU_PATH)\Y_sys_PL2.dsc\
                    $(CPU_PATH)\Y_AppCluster.dsc\                    
                    $(CPU_PATH)\P_AppCluster.dsc\
                    stmp3500_PL2.dsc\
                    menus.dsc
else
ifdef USE_PLAYLIST5
STMP3500_DSC_LIST = $(STMP3500_DSC_LIST)\
                    $(CPU_PATH)\Y_sys_PL5.dsc\
                    $(CPU_PATH)\Y_AppCluster_PL5.dsc\                    
                    $(CPU_PATH)\P_AppCluster_PL5.dsc\                    
                    stmp3500_PL5.dsc\
                    menus_PL5.dsc
else                   
STMP3500_DSC_LIST = $(STMP3500_DSC_LIST)\
                    $(CPU_PATH)\Y_sys.dsc\
                    $(CPU_PATH)\Y_AppCluster.dsc\                    
                    $(CPU_PATH)\P_AppCluster.dsc\
                    stmp3500.dsc\
                    menus.dsc                    
endif
endif

"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" $(STMP3500_DSC_LIST)
ifdef USE_PLAYLIST2
"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" master_PL2.dsc
    @echo Locator creating IEEE-695 absolute file $(PROJ).abs (all initial sections)
    $(LC_EXE) -v -o $@ $(LC_OPTS) -dmaster_PL2.dsc $(OUTPUT_PATH)\$(PROJ).out
else
ifdef USE_PLAYLIST5
"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" master_PL5.dsc
    @echo Locator creating IEEE-695 absolute file $(PROJ).abs (all initial sections)
    $(LC_EXE) -v -o $@ $(LC_OPTS) -dmaster_PL5.dsc $(OUTPUT_PATH)\$(PROJ).out
else                   
"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" master.dsc
    @echo Locator creating IEEE-695 absolute file $(PROJ).abs (all initial sections)
    $(LC_EXE) -v -o $@ $(LC_OPTS) -dmaster.dsc $(OUTPUT_PATH)\$(PROJ).out                    
endif
endif


#//////////////////////////////////
# Hide OEM Data
#/////////////////////////////////
ifdef D_JANUS
$(OUTPUT_OEM_DATA)  : $(DRM_PATH)\st_pub.bin $(PROJ_PATH)\oemgroupkey.bin
        @$(BIN_PATH)\$(ENCRYPT_EXE) $(DRM_PATH)\st_pub.bin $(PROJ_PATH)\oemgroupkey.bin $(OUTPUT_OEM_DATA)
endif

#///////////////////////////////////////////////////////////////////////////////
# Resource build
#///////////////////////////////////////////////////////////////////////////////

$(OUTPUT_RESOURCE_BIN) : $(MODULELIST) $(CODEBANKLIST) $(WMALIST) $(FUNCLETSRCLIST) $(RESOURCELIST)
    @echo Creating $@
    @$(BIN_PATH)\rsclink.EXE -h$(PROJ_PATH)\resource.inc \
    -i$(ROOT)\fonts\iso8859\ \
    -i$(ROOT)\fonts\iso8859\sigmatel_8 \
    -i$(BITMAP_PATH)\icons\status\16 \
    -i$(BITMAP_PATH)\icons\battery\8x8 \
    -i$(BITMAP_PATH)\strings\arial_8 \
    -i$(RESOURCE_PATH)\shingyih \
    -i$(OUTPUT_RSRC_PATH) -i$(PROJRSRC_PATH) \
    -o$@ -a \
    -v$(VERSION)
    @$(BIN_PATH)\resparse $@ $(OUTPUT_PATH)\resource.txt

#Text resources
$(OUTPUT_RSRC_PATH)\string_searching.src                : $(PROJRSRC_PATH)\strings\string_searching.txt
$(OUTPUT_RSRC_PATH)\string_spectrogram_menu.src         : $(PROJRSRC_PATH)\strings\string_spectrogram_menu.txt
$(OUTPUT_RSRC_PATH)\string_dec_pt5.src                  : $(PROJRSRC_PATH)\strings\string_dec_pt5.txt
$(OUTPUT_RSRC_PATH)\string_dec_pt0.src                  : $(PROJRSRC_PATH)\strings\string_dec_pt0.txt
$(OUTPUT_RSRC_PATH)\string_db.src                       : $(PROJRSRC_PATH)\strings\string_db.txt
$(OUTPUT_RSRC_PATH)\string_hz2.src                      : $(PROJRSRC_PATH)\strings\string_hz2.txt
$(OUTPUT_RSRC_PATH)\string_voice.src                    : $(PROJRSRC_PATH)\strings\string_voice.txt
$(OUTPUT_RSRC_PATH)\string_song.src                     : $(PROJRSRC_PATH)\strings\string_song.txt
$(OUTPUT_RSRC_PATH)\string_music_menu.src               : $(PROJRSRC_PATH)\strings\string_music_menu.txt
$(OUTPUT_RSRC_PATH)\string_voice_menu.src               : $(PROJRSRC_PATH)\strings\string_voice_menu.txt
$(OUTPUT_RSRC_PATH)\string_audible_menu.src             : $(PROJRSRC_PATH)\strings\string_audible_menu.txt
$(OUTPUT_RSRC_PATH)\string_music_menu_delete_all.src    : $(PROJRSRC_PATH)\strings\string_music_menu_delete_all.txt
$(OUTPUT_RSRC_PATH)\string_voice_menu_delete_all.src    : $(PROJRSRC_PATH)\strings\string_voice_menu_delete_all.txt
$(OUTPUT_RSRC_PATH)\string_fmtuner_menu.src             : $(PROJRSRC_PATH)\strings\string_fmtuner_menu.txt
$(OUTPUT_RSRC_PATH)\string_settings_menu.src            : $(PROJRSRC_PATH)\strings\string_settings_menu.txt
$(OUTPUT_RSRC_PATH)\string_eq_menu.src                  : $(PROJRSRC_PATH)\strings\string_eq_menu.txt
$(OUTPUT_RSRC_PATH)\string_10min_menu.src               : $(PROJRSRC_PATH)\strings\string_10min_menu.txt
$(OUTPUT_RSRC_PATH)\string_1min_menu.src                : $(PROJRSRC_PATH)\strings\string_1min_menu.txt
$(OUTPUT_RSRC_PATH)\string_2min_menu.src                : $(PROJRSRC_PATH)\strings\string_2min_menu.txt
$(OUTPUT_RSRC_PATH)\string_5min_menu.src                : $(PROJRSRC_PATH)\strings\string_5min_menu.txt
$(OUTPUT_RSRC_PATH)\string_10min_menu.src               : $(PROJRSRC_PATH)\strings\string_10min_menu.txt
$(OUTPUT_RSRC_PATH)\string_classic_menu.src             : $(PROJRSRC_PATH)\strings\string_classic_menu.txt
$(OUTPUT_RSRC_PATH)\string_contrast_menu.src            : $(PROJRSRC_PATH)\strings\string_contrast_menu.txt
$(OUTPUT_RSRC_PATH)\string_disable_menu.src             : $(PROJRSRC_PATH)\strings\string_disable_menu.txt
$(OUTPUT_RSRC_PATH)\string_exit_menu.src                : $(PROJRSRC_PATH)\strings\string_exit_menu.txt
$(OUTPUT_RSRC_PATH)\string_jazz_menu.src                : $(PROJRSRC_PATH)\strings\string_jazz_menu.txt
$(OUTPUT_RSRC_PATH)\string_normal_menu.src              : $(PROJRSRC_PATH)\strings\string_normal_menu.txt
$(OUTPUT_RSRC_PATH)\string_playmode_menu.src            : $(PROJRSRC_PATH)\strings\string_playmode_menu.txt
$(OUTPUT_RSRC_PATH)\string_pop_menu.src                 : $(PROJRSRC_PATH)\strings\string_pop_menu.txt
$(OUTPUT_RSRC_PATH)\string_pwrsavings_menu.src          : $(PROJRSRC_PATH)\strings\string_pwrsavings_menu.txt
$(OUTPUT_RSRC_PATH)\string_repeat1_menu.src             : $(PROJRSRC_PATH)\strings\string_repeat1_menu.txt
$(OUTPUT_RSRC_PATH)\string_repeatall_menu.src           : $(PROJRSRC_PATH)\strings\string_repeatall_menu.txt
$(OUTPUT_RSRC_PATH)\string_repeatshuffle_menu.src       : $(PROJRSRC_PATH)\strings\string_repeatshuffle_menu.txt
$(OUTPUT_RSRC_PATH)\string_rock_menu.src                : $(PROJRSRC_PATH)\strings\string_rock_menu.txt
$(OUTPUT_RSRC_PATH)\string_shuffle_menu.src             : $(PROJRSRC_PATH)\strings\string_shuffle_menu.txt
$(OUTPUT_RSRC_PATH)\string_about_menu.src               : $(PROJRSRC_PATH)\strings\string_about_menu.txt
$(OUTPUT_RSRC_PATH)\string_backlight_menu.src           : $(PROJRSRC_PATH)\strings\string_backlight_menu.txt
$(OUTPUT_RSRC_PATH)\string_delete_menu.src              : $(PROJRSRC_PATH)\strings\string_delete_menu.txt
$(OUTPUT_RSRC_PATH)\string_system_menu.src              : $(PROJRSRC_PATH)\strings\string_system_menu.txt
$(OUTPUT_RSRC_PATH)\string_record_menu.src              : $(PROJRSRC_PATH)\strings\string_record_menu.txt
$(OUTPUT_RSRC_PATH)\string_mb.src                       : $(PROJRSRC_PATH)\strings\string_mb.txt
$(OUTPUT_RSRC_PATH)\string_record_settings.src          : $(PROJRSRC_PATH)\strings\string_record_settings.txt
$(OUTPUT_RSRC_PATH)\string_sample_rate.src              : $(PROJRSRC_PATH)\strings\string_sample_rate.txt
$(OUTPUT_RSRC_PATH)\string_device.src                   : $(PROJRSRC_PATH)\strings\string_device.txt
$(OUTPUT_RSRC_PATH)\string_encoder.src                  : $(PROJRSRC_PATH)\strings\string_encoder.txt
$(OUTPUT_RSRC_PATH)\string_internal.src                 : $(PROJRSRC_PATH)\strings\string_internal.txt
$(OUTPUT_RSRC_PATH)\string_external.src                 : $(PROJRSRC_PATH)\strings\string_external.txt
$(OUTPUT_RSRC_PATH)\string_adpcm.src                    : $(PROJRSRC_PATH)\strings\string_adpcm.txt
$(OUTPUT_RSRC_PATH)\string_msadpcm.src                  : $(PROJRSRC_PATH)\strings\string_msadpcm.txt
$(OUTPUT_RSRC_PATH)\string_imadpcm.src                  : $(PROJRSRC_PATH)\strings\string_imadpcm.txt
$(OUTPUT_RSRC_PATH)\string_pcm.src                      : $(PROJRSRC_PATH)\strings\string_pcm.txt
$(OUTPUT_RSRC_PATH)\string_location.src                 : $(PROJRSRC_PATH)\strings\string_location.txt
$(OUTPUT_RSRC_PATH)\string_source.src                   : $(PROJRSRC_PATH)\strings\string_source.txt
$(OUTPUT_RSRC_PATH)\string_microphone.src               : $(PROJRSRC_PATH)\strings\string_microphone.txt
$(OUTPUT_RSRC_PATH)\string_linein.src                   : $(PROJRSRC_PATH)\strings\string_linein.txt
$(OUTPUT_RSRC_PATH)\string_fm.src                       : $(PROJRSRC_PATH)\strings\string_fm.txt
$(OUTPUT_RSRC_PATH)\string_channels.src                 : $(PROJRSRC_PATH)\strings\string_channels.txt
$(OUTPUT_RSRC_PATH)\string_bits.src                     : $(PROJRSRC_PATH)\strings\string_bits.txt
$(OUTPUT_RSRC_PATH)\string_4.src                        : $(PROJRSRC_PATH)\strings\string_4.txt
$(OUTPUT_RSRC_PATH)\string_8.src                        : $(PROJRSRC_PATH)\strings\string_8.txt
$(OUTPUT_RSRC_PATH)\string_16.src                       : $(PROJRSRC_PATH)\strings\string_16.txt
$(OUTPUT_RSRC_PATH)\string_24.src                       : $(PROJRSRC_PATH)\strings\string_24.txt
$(OUTPUT_RSRC_PATH)\string_mono.src                     : $(PROJRSRC_PATH)\strings\string_mono.txt
$(OUTPUT_RSRC_PATH)\string_stereo.src                   : $(PROJRSRC_PATH)\strings\string_stereo.txt
$(OUTPUT_RSRC_PATH)\string_8000hz.src                   : $(PROJRSRC_PATH)\strings\string_8000hz.txt
$(OUTPUT_RSRC_PATH)\string_11025hz.src                  : $(PROJRSRC_PATH)\strings\string_11025hz.txt
$(OUTPUT_RSRC_PATH)\string_16000hz.src                  : $(PROJRSRC_PATH)\strings\string_16000hz.txt
$(OUTPUT_RSRC_PATH)\string_22050hz.src                  : $(PROJRSRC_PATH)\strings\string_22050hz.txt
$(OUTPUT_RSRC_PATH)\string_32000hz.src                  : $(PROJRSRC_PATH)\strings\string_32000hz.txt
$(OUTPUT_RSRC_PATH)\string_44100hz.src                  : $(PROJRSRC_PATH)\strings\string_44100hz.txt
$(OUTPUT_RSRC_PATH)\string_48000hz.src                  : $(PROJRSRC_PATH)\strings\string_48000hz.txt
$(OUTPUT_RSRC_PATH)\string_96000hz.src                  : $(PROJRSRC_PATH)\strings\string_96000hz.txt
$(OUTPUT_RSRC_PATH)\string_112000hz.src                 : $(PROJRSRC_PATH)\strings\string_112000hz.txt
$(OUTPUT_RSRC_PATH)\string_128000hz.src                 : $(PROJRSRC_PATH)\strings\string_128000hz.txt
$(OUTPUT_RSRC_PATH)\string_160000hz.src                 : $(PROJRSRC_PATH)\strings\string_160000hz.txt
$(OUTPUT_RSRC_PATH)\string_192000hz.src                 : $(PROJRSRC_PATH)\strings\string_192000hz.txt
$(OUTPUT_RSRC_PATH)\string_224000hz.src                 : $(PROJRSRC_PATH)\strings\string_224000hz.txt
$(OUTPUT_RSRC_PATH)\string_256000hz.src                 : $(PROJRSRC_PATH)\strings\string_256000hz.txt
$(OUTPUT_RSRC_PATH)\string_320000hz.src                 : $(PROJRSRC_PATH)\strings\string_320000hz.txt
$(OUTPUT_RSRC_PATH)\string_album.src                    : $(PROJRSRC_PATH)\strings\string_album.txt
$(OUTPUT_RSRC_PATH)\string_encoder_song.src             : $(PROJRSRC_PATH)\strings\string_encoder_song.txt
$(OUTPUT_RSRC_PATH)\string_mode.src                     : $(PROJRSRC_PATH)\strings\string_mode.txt
$(OUTPUT_RSRC_PATH)\string_hz.src                       : $(PROJRSRC_PATH)\strings\string_hz.txt
$(OUTPUT_RSRC_PATH)\string_bit_rate.src                 : $(PROJRSRC_PATH)\strings\string_bit_rate.txt
$(OUTPUT_RSRC_PATH)\string_spaces.src                   : $(PROJRSRC_PATH)\strings\string_spaces.txt
$(OUTPUT_RSRC_PATH)\string_no_files.src                 : $(PROJRSRC_PATH)\strings\string_no_files.txt
$(OUTPUT_RSRC_PATH)\string_working.src     : $(PROJRSRC_PATH)\strings\string_working.txt

ifdef JPEG_APP
$(OUTPUT_RSRC_PATH)\string_jpeg_display_menu.src   : $(PROJRSRC_PATH)\strings\string_jpeg_display_menu.txt
$(OUTPUT_RSRC_PATH)\string_jpeg_manual_menu.src   : $(PROJRSRC_PATH)\strings\string_jpeg_manual_menu.txt
$(OUTPUT_RSRC_PATH)\string_jpeg_slideshow_menu.src   : $(PROJRSRC_PATH)\strings\string_jpeg_slideshow_menu.txt
$(OUTPUT_RSRC_PATH)\string_jpeg_thumbnail_menu.src   : $(PROJRSRC_PATH)\strings\string_jpeg_thumbnail_menu.txt
$(OUTPUT_RSRC_PATH)\string_end_of_slide_show.src  :$(PROJRSRC_PATH)\strings\string_end_of_slide_show.txt
else
$(OUTPUT_RSRC_PATH)\string_jpeg_display_menu.src   : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\string_jpeg_manual_menu.src   : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\string_jpeg_slideshow_menu.src   : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\string_jpeg_thumbnail_menu.src   : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\string_end_of_slide_show.src  :$(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@
endif

ifdef MOTION_VIDEO
$(OUTPUT_RSRC_PATH)\string_mvideo_menu.src   : $(PROJRSRC_PATH)\strings\string_mvideo_menu.txt
else
$(OUTPUT_RSRC_PATH)\string_mvideo_menu.src   : $(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@
endif

#Bitmap resources
$(OUTPUT_RSRC_PATH)\icon_folder.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\icon_folder.bmp
$(OUTPUT_RSRC_PATH)\icon_song.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\icon_song.bmp
$(OUTPUT_RSRC_PATH)\menu_music.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_music.bmp
$(OUTPUT_RSRC_PATH)\menu_record.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_record.bmp
$(OUTPUT_RSRC_PATH)\menu_voice.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_voice.bmp
$(OUTPUT_RSRC_PATH)\menu_fmtuner.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_fmtuner.bmp
$(OUTPUT_RSRC_PATH)\menu_settings.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_settings.bmp
$(OUTPUT_RSRC_PATH)\menu_clock.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_clock.bmp
$(OUTPUT_RSRC_PATH)\menu_about.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_about.bmp
$(OUTPUT_RSRC_PATH)\menu_exit.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_exit.bmp
$(OUTPUT_RSRC_PATH)\menu_delete.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_delete.bmp
$(OUTPUT_RSRC_PATH)\menu_ab.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_ab.bmp
$(OUTPUT_RSRC_PATH)\menu_shutdown.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\menu_shutdown.bmp
$(OUTPUT_RSRC_PATH)\music_play_all.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\music_play_all.bmp
$(OUTPUT_RSRC_PATH)\music_folder_internal.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\music_folder_internal.bmp
$(OUTPUT_RSRC_PATH)\music_folder_external.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\music_folder_external.bmp
$(OUTPUT_RSRC_PATH)\music_songs.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\music_songs.bmp
$(OUTPUT_RSRC_PATH)\music_favorites.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\music_favorites.bmp
$(OUTPUT_RSRC_PATH)\music_fm_record.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\music_fm_record.bmp
$(OUTPUT_RSRC_PATH)\music_exit.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\music_exit.bmp
$(OUTPUT_RSRC_PATH)\browse_music_folder_internal.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\browse_music_folder_internal.bmp
$(OUTPUT_RSRC_PATH)\browse_music_folder_external.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\browse_music_folder_external.bmp
$(OUTPUT_RSRC_PATH)\browse_music_list_songs.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\browse_music_list_songs.bmp
$(OUTPUT_RSRC_PATH)\browse_music_favourites.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\browse_music_favourites.bmp
$(OUTPUT_RSRC_PATH)\browse_music_fm_files.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\browse_music_fm_files.bmp
$(OUTPUT_RSRC_PATH)\browse_voice.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\browse_voice.bmp
$(OUTPUT_RSRC_PATH)\favourites_list_add.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\favourites_list_add.bmp
$(OUTPUT_RSRC_PATH)\favourites_list_remove.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\favourites_list_remove.bmp
$(OUTPUT_RSRC_PATH)\favourites_list_is_full.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\favourites_list_is_full.bmp
$(OUTPUT_RSRC_PATH)\about_screen_1.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\about_screen_1.bmp
$(OUTPUT_RSRC_PATH)\about_screen_2.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\about_screen_2.bmp
$(OUTPUT_RSRC_PATH)\about_screen_3.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\about_screen_3.bmp
$(OUTPUT_RSRC_PATH)\about_screen_4.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\about_screen_4.bmp
$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_title.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_auto_shutdown_title.bmp
$(OUTPUT_RSRC_PATH)\settings_backlight_title.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_backlight_title.bmp
$(OUTPUT_RSRC_PATH)\settings_contrast_title.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_contrast_title.bmp
$(OUTPUT_RSRC_PATH)\settings_eq_title.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_eq_title.bmp
$(OUTPUT_RSRC_PATH)\settings_exit_title.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_exit_title.bmp
$(OUTPUT_RSRC_PATH)\settings_playmode_title.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_playmode_title.bmp
$(OUTPUT_RSRC_PATH)\settings_set_date_title.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_set_date_title.bmp
$(OUTPUT_RSRC_PATH)\settings_set_time_title.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_set_time_title.bmp

$(OUTPUT_RSRC_PATH)\settings_playmode_normal.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_playmode_normal.bmp
$(OUTPUT_RSRC_PATH)\settings_playmode_repeatone.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_playmode_repeatone.bmp
$(OUTPUT_RSRC_PATH)\settings_playmode_repeatall.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_playmode_repeatall.bmp
$(OUTPUT_RSRC_PATH)\settings_playmode_shuffle.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_playmode_shuffle.bmp
$(OUTPUT_RSRC_PATH)\settings_playmode_shuffle_repeat.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_playmode_shuffle_repeat.bmp

$(OUTPUT_RSRC_PATH)\settings_backlight_on.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_backlight_on.bmp
$(OUTPUT_RSRC_PATH)\settings_backlight_10s.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_backlight_10s.bmp
$(OUTPUT_RSRC_PATH)\settings_backlight_20s.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_backlight_20s.bmp
$(OUTPUT_RSRC_PATH)\settings_backlight_30s.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_backlight_30s.bmp

$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_disable.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_auto_shutdown_disable.bmp
$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_1minute.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_auto_shutdown_1minute.bmp
$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_2minutes.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_auto_shutdown_2minutes.bmp
$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_5minutes.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_auto_shutdown_5minutes.bmp
$(OUTPUT_RSRC_PATH)\settings_auto_shutdown_10minutes.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_auto_shutdown_10minutes.bmp
$(OUTPUT_RSRC_PATH)\time_date_exit_title.src				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_date_exit_title.bmp
$(OUTPUT_RSRC_PATH)\settings_languages_eng.src			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_languages_eng.bmp
$(OUTPUT_RSRC_PATH)\settings_languages_vie.src   			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\settings_languages_vie.bmp

$(OUTPUT_RSRC_PATH)\time_clean_desktop.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_clean_desktop.bmp
$(OUTPUT_RSRC_PATH)\time_dash.src				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_dash.bmp
$(OUTPUT_RSRC_PATH)\fraction_dot.src				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\fraction_dot.bmp
$(OUTPUT_RSRC_PATH)\fm_background.src			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\fm_background.bmp
$(OUTPUT_RSRC_PATH)\searching_please_wait.src	 	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\searching_please_wait.bmp
$(OUTPUT_RSRC_PATH)\fm_auto_search.src	 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\fm_auto_search.bmp

$(OUTPUT_RSRC_PATH)\time_day_2.src 				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_day_2.bmp
$(OUTPUT_RSRC_PATH)\time_day_3.src 				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_day_3.bmp
$(OUTPUT_RSRC_PATH)\time_day_4.src 				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_day_4.bmp
$(OUTPUT_RSRC_PATH)\time_day_5.src 				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_day_5.bmp
$(OUTPUT_RSRC_PATH)\time_day_6.src 				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_day_6.bmp
$(OUTPUT_RSRC_PATH)\time_day_7.src 				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_day_7.bmp
$(OUTPUT_RSRC_PATH)\time_day_cn.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_day_cn.bmp
$(OUTPUT_RSRC_PATH)\time_month_1.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_1.bmp
$(OUTPUT_RSRC_PATH)\time_month_2.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_2.bmp
$(OUTPUT_RSRC_PATH)\time_month_3.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_3.bmp
$(OUTPUT_RSRC_PATH)\time_month_4.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_4.bmp
$(OUTPUT_RSRC_PATH)\time_month_5.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_5.bmp
$(OUTPUT_RSRC_PATH)\time_month_6.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_6.bmp
$(OUTPUT_RSRC_PATH)\time_month_7.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_7.bmp
$(OUTPUT_RSRC_PATH)\time_month_8.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_8.bmp
$(OUTPUT_RSRC_PATH)\time_month_9.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_9.bmp
$(OUTPUT_RSRC_PATH)\time_month_10.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_10.bmp
$(OUTPUT_RSRC_PATH)\time_month_11.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_11.bmp
$(OUTPUT_RSRC_PATH)\time_month_12.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_month_12.bmp
$(OUTPUT_RSRC_PATH)\time_num_am.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_am.bmp
$(OUTPUT_RSRC_PATH)\time_num_pm.src 			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_pm.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_0.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_0.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_1.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_1.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_2.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_2.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_3.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_3.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_4.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_4.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_5.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_5.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_6.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_6.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_7.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_7.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_8.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_8.bmp
$(OUTPUT_RSRC_PATH)\time_num_large_9.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_large_9.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_0.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_0.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_1.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_1.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_2.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_2.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_3.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_3.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_4.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_4.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_5.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_5.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_6.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_6.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_7.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_7.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_8.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_8.bmp
$(OUTPUT_RSRC_PATH)\time_num_medium_9.src 		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_num_medium_9.bmp
$(OUTPUT_RSRC_PATH)\time_colon.src 				: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\time_colon.bmp

$(OUTPUT_RSRC_PATH)\setdate_format_dmy.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\setdate_format_dmy.bmp
$(OUTPUT_RSRC_PATH)\setdate_format_mdy.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\setdate_format_mdy.bmp
$(OUTPUT_RSRC_PATH)\setdate_format_ymd.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\setdate_format_ymd.bmp
$(OUTPUT_RSRC_PATH)\settime_format_12h.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\settime_format_12h.bmp
$(OUTPUT_RSRC_PATH)\settime_format_24h.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\time_number\settime_format_24h.bmp
$(OUTPUT_RSRC_PATH)\jvj_shutdown_player.src      	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\jvj_shutdown_player.bmp

$(OUTPUT_RSRC_PATH)\vie_menu_music.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_music.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_record.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_record.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_voice.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_voice.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_fmtuner.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_fmtuner.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_settings.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_settings.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_clock.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_clock.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_about.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_about.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_exit.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_exit.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_delete.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_delete.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_ab.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_ab.bmp
$(OUTPUT_RSRC_PATH)\vie_menu_shutdown.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_menu_shutdown.bmp
$(OUTPUT_RSRC_PATH)\vie_music_play_all.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_music_play_all.bmp
$(OUTPUT_RSRC_PATH)\vie_music_folder_internal.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_music_folder_internal.bmp
$(OUTPUT_RSRC_PATH)\vie_music_folder_external.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_music_folder_external.bmp
$(OUTPUT_RSRC_PATH)\vie_music_songs.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_music_songs.bmp
$(OUTPUT_RSRC_PATH)\vie_music_favorites.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_music_favorites.bmp
$(OUTPUT_RSRC_PATH)\vie_music_fm_record.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_music_fm_record.bmp
$(OUTPUT_RSRC_PATH)\vie_music_exit.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_music_exit.bmp
$(OUTPUT_RSRC_PATH)\vie_browse_music_folder_internal.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_browse_music_folder_internal.bmp
$(OUTPUT_RSRC_PATH)\vie_browse_music_folder_external.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_browse_music_folder_external.bmp
$(OUTPUT_RSRC_PATH)\vie_browse_music_list_songs.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_browse_music_list_songs.bmp
$(OUTPUT_RSRC_PATH)\vie_browse_music_favourites.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_browse_music_favourites.bmp
$(OUTPUT_RSRC_PATH)\vie_browse_music_fm_files.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_browse_music_fm_files.bmp
$(OUTPUT_RSRC_PATH)\vie_browse_voice.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_browse_voice.bmp
$(OUTPUT_RSRC_PATH)\vie_favourites_list_add.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_favourites_list_add.bmp
$(OUTPUT_RSRC_PATH)\vie_favourites_list_remove.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_favourites_list_remove.bmp
$(OUTPUT_RSRC_PATH)\vie_favourites_list_is_full.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_favourites_list_is_full.bmp
$(OUTPUT_RSRC_PATH)\vie_fm_background.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_fm_background.bmp
$(OUTPUT_RSRC_PATH)\vie_searching_please_wait.src	 : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_searching_please_wait.bmp
$(OUTPUT_RSRC_PATH)\vie_fm_auto_search.src	 : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_fm_auto_search.bmp

$(OUTPUT_RSRC_PATH)\vie_about_screen_1.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_about_screen_1.bmp
$(OUTPUT_RSRC_PATH)\vie_about_screen_2.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_about_screen_2.bmp
$(OUTPUT_RSRC_PATH)\vie_about_screen_3.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_about_screen_3.bmp
$(OUTPUT_RSRC_PATH)\vie_about_screen_4.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_about_screen_4.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_1min.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_auto_shutdown_1min.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_2min.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_auto_shutdown_2min.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_5min.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_auto_shutdown_5min.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_10min.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_auto_shutdown_10min.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_auto_shutdown_disable.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_auto_shutdown_disable.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_backlight_10s.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_backlight_10s.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_backlight_20s.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_backlight_20s.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_backlight_30s.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_backlight_30s.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_backlight_on.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_backlight_on.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_contrast_title.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_contrast_title.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_eq_title.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_eq_title.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_exit_title.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_exit_title.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_playmode_normal.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_playmode_normal.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_playmode_repeatall.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_playmode_repeatall.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_playmode_repeatone.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_playmode_repeatone.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_playmode_shuffle.src		: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_playmode_shuffle.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_playmode_shuffle_repeat.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_playmode_shuffle_repeat.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_set_date_title.src			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_set_date_title.bmp
$(OUTPUT_RSRC_PATH)\vie_settings_set_time_title.src			: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_settings_set_time_title.bmp
$(OUTPUT_RSRC_PATH)\vie_time_date_exit_title.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_date_exit_title.bmp
$(OUTPUT_RSRC_PATH)\vie_time_day_2.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_day_2.bmp
$(OUTPUT_RSRC_PATH)\vie_time_day_3.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_day_3.bmp
$(OUTPUT_RSRC_PATH)\vie_time_day_4.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_day_4.bmp
$(OUTPUT_RSRC_PATH)\vie_time_day_5.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_day_5.bmp
$(OUTPUT_RSRC_PATH)\vie_time_day_6.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_day_6.bmp
$(OUTPUT_RSRC_PATH)\vie_time_day_7.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_day_7.bmp
$(OUTPUT_RSRC_PATH)\vie_time_day_cn.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_day_cn.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_1.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_1.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_2.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_2.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_3.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_3.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_4.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_4.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_5.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_5.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_6.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_6.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_7.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_7.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_8.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_8.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_9.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_9.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_10.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_10.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_11.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_11.bmp
$(OUTPUT_RSRC_PATH)\vie_time_month_12.src	: $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_time_month_12.bmp
$(OUTPUT_RSRC_PATH)\vie_jvj_shutdown_player.src      : $(PROJRSRC_PATH)\bitmaps\bkbitmaps\lang_vie\vie_jvj_shutdown_player.bmp

$(OUTPUT_RSRC_PATH)\negative_sign.src                   :$(PROJRSRC_PATH)\bitmaps\negative_sign.bmp
$(OUTPUT_RSRC_PATH)\slider_bar.src                      :$(PROJRSRC_PATH)\bitmaps\slider_bar.bmp
$(OUTPUT_RSRC_PATH)\slider_bar_inv.src                  :$(PROJRSRC_PATH)\bitmaps\slider_bar_inv.bmp
$(OUTPUT_RSRC_PATH)\slider_track.src                    :$(PROJRSRC_PATH)\bitmaps\slider_track.bmp

$(OUTPUT_RSRC_PATH)\settings_title.src                  :$(PROJRSRC_PATH)\bitmaps\settings_title.bmp
$(OUTPUT_RSRC_PATH)\erase_title.src                     :$(PROJRSRC_PATH)\bitmaps\erase_title.bmp
$(OUTPUT_RSRC_PATH)\st_bw1.src                          :$(BITMAP_PATH)\Logos\st_bw1.bmp
$(OUTPUT_RSRC_PATH)\lowbattery.src                      :$(PROJRSRC_PATH)\bitmaps\lowbattery.bmp
$(OUTPUT_RSRC_PATH)\del_warning_no.src                  :$(PROJRSRC_PATH)\bitmaps\del_warning_no.bmp
$(OUTPUT_RSRC_PATH)\del_warning_yes.src                 :$(PROJRSRC_PATH)\bitmaps\del_warning_yes.bmp
$(OUTPUT_RSRC_PATH)\del_warning_line1.src               :$(PROJRSRC_PATH)\bitmaps\del_warning_line1.bmp
$(OUTPUT_RSRC_PATH)\del_warning_line2.src               :$(PROJRSRC_PATH)\bitmaps\del_warning_line2.bmp
$(OUTPUT_RSRC_PATH)\vbr.src                             :$(PROJRSRC_PATH)\bitmaps\vbr.bmp
$(OUTPUT_RSRC_PATH)\bitmap_warning.src                  :$(PROJRSRC_PATH)\bitmaps\bitmap_warning.bmp
$(OUTPUT_RSRC_PATH)\bitmap_device_full.src              :$(PROJRSRC_PATH)\bitmaps\bitmap_device_full.bmp
$(OUTPUT_RSRC_PATH)\low_level_pwr_line1.src             :$(PROJRSRC_PATH)\bitmaps\low_level_pwr_line1.bmp
$(OUTPUT_RSRC_PATH)\low_level_pwr_line2.src             :$(PROJRSRC_PATH)\bitmaps\low_level_pwr_line2.bmp

$(OUTPUT_RSRC_PATH)\exmediaerror1.src                   : $(PROJRSRC_PATH)\bitmaps\exmediaerror1.bmp
$(OUTPUT_RSRC_PATH)\exmediaerror2.src                   : $(PROJRSRC_PATH)\bitmaps\exmediaerror2.bmp
$(OUTPUT_RSRC_PATH)\inmediaerror1.src                   : $(PROJRSRC_PATH)\bitmaps\inmediaerror1.bmp

$(OUTPUT_RSRC_PATH)\delete_successful.src               :$(PROJRSRC_PATH)\bitmaps\delete_successful.bmp
$(OUTPUT_RSRC_PATH)\disk_full.src                       :$(PROJRSRC_PATH)\bitmaps\disk_full.bmp
$(OUTPUT_RSRC_PATH)\delete_error.src                    :$(PROJRSRC_PATH)\bitmaps\delete_error.bmp
$(OUTPUT_RSRC_PATH)\lic_expired.src            :$(PROJRSRC_PATH)\bitmaps\lic_expired.bmp
$(OUTPUT_RSRC_PATH)\del_all_warning_line2.src           :$(PROJRSRC_PATH)\bitmaps\del_all_warning_line2.bmp
$(OUTPUT_RSRC_PATH)\del_all_file_star.src               :$(PROJRSRC_PATH)\bitmaps\del_all_file_star.bmp

$(OUTPUT_RSRC_PATH)\check_disk_1.src : $(PROJRSRC_PATH)\bitmaps\check_disk_1.bmp
$(OUTPUT_RSRC_PATH)\check_disk_2.src : $(PROJRSRC_PATH)\bitmaps\check_disk_2.bmp
$(OUTPUT_RSRC_PATH)\check_disk_3.src : $(PROJRSRC_PATH)\bitmaps\check_disk_3.bmp
$(OUTPUT_RSRC_PATH)\flash_error.src  : $(PROJRSRC_PATH)\bitmaps\flash_error.bmp

ifdef JPEG_APP
$(OUTPUT_RSRC_PATH)\jpeg_display_title.src  :$(PROJRSRC_PATH)\color_rsrc\jpeg_display_title.bmp
else
$(OUTPUT_RSRC_PATH)\jpeg_display_title.src  :$(OUTPUT_PATH)\$(PROJ).abs
    @copy $(PROJRSRC_PATH)\null.src  $@
endif


# DVRWARN 2 bmp files above

$(OUTPUT_RSRC_PATH)\play_icon_with_border.src           : $(PROJRSRC_PATH)\bitmaps\play_icon_with_border.bmp
$(OUTPUT_RSRC_PATH)\pause_icon_with_border.src          : $(PROJRSRC_PATH)\bitmaps\pause_icon_with_border.bmp
$(OUTPUT_RSRC_PATH)\stop_icon_with_border.src           : $(PROJRSRC_PATH)\bitmaps\stop_icon_with_border.bmp
$(OUTPUT_RSRC_PATH)\record_icon_with_border.src         : $(PROJRSRC_PATH)\bitmaps\record_icon_with_border.bmp
$(OUTPUT_RSRC_PATH)\paused_record_icon_with_border.src  : $(PROJRSRC_PATH)\bitmaps\paused_record_icon_with_border.bmp
$(OUTPUT_RSRC_PATH)\ffwd_icon_with_border.src           : $(PROJRSRC_PATH)\bitmaps\ffwd_icon_with_border.bmp
$(OUTPUT_RSRC_PATH)\rwnd_icon_with_border.src           : $(PROJRSRC_PATH)\bitmaps\rwnd_icon_with_border.bmp

$(OUTPUT_RSRC_PATH)\repeatall_icon.src                  : $(PROJRSRC_PATH)\bitmaps\repeatall_icon.bmp
$(OUTPUT_RSRC_PATH)\repeatsong_icon.src                 : $(PROJRSRC_PATH)\bitmaps\repeatsong_icon.bmp
$(OUTPUT_RSRC_PATH)\shuffle_icon.src                    : $(PROJRSRC_PATH)\bitmaps\shuffle_icon.bmp
$(OUTPUT_RSRC_PATH)\random_icon.src                     : $(PROJRSRC_PATH)\bitmaps\random_icon.bmp
$(OUTPUT_RSRC_PATH)\repeatallclear_icon.src             : $(PROJRSRC_PATH)\bitmaps\repeatallclear_icon.bmp
$(OUTPUT_RSRC_PATH)\repeatsongclear_icon.src            : $(PROJRSRC_PATH)\bitmaps\repeatsongclear_icon.bmp
$(OUTPUT_RSRC_PATH)\shuffleclear_icon.src               : $(PROJRSRC_PATH)\bitmaps\shuffleclear_icon.bmp
$(OUTPUT_RSRC_PATH)\disk_small.src                      : $(PROJRSRC_PATH)\bitmaps\disk_small.bmp
$(OUTPUT_RSRC_PATH)\disk_full.src                       : $(PROJRSRC_PATH)\bitmaps\disk_full.bmp
$(OUTPUT_RSRC_PATH)\lock_small.src                      : $(PROJRSRC_PATH)\bitmaps\lock_small.bmp
$(OUTPUT_RSRC_PATH)\icon_music_mode.src                 : $(PROJRSRC_PATH)\bitmaps\icon_music_mode.bmp
$(OUTPUT_RSRC_PATH)\icon_voice_mode.src                 : $(PROJRSRC_PATH)\bitmaps\icon_voice_mode.bmp

$(OUTPUT_RSRC_PATH)\icon_vol_00.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_00.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_01.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_01.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_02.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_02.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_03.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_03.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_04.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_04.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_05.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_05.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_06.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_06.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_07.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_07.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_08.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_08.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_09.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_09.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_10.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_10.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_11.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_11.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_12.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_12.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_13.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_13.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_14.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_14.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_15.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_15.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_16.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_16.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_17.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_17.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_18.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_18.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_19.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_19.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_20.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_20.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_21.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_21.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_22.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_22.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_23.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_23.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_24.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_24.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_25.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_25.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_26.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_26.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_27.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_27.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_28.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_28.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_29.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_29.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_30.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_30.bmp
$(OUTPUT_RSRC_PATH)\icon_vol_31.src                     : $(PROJRSRC_PATH)\bitmaps\icon_vol_31.bmp

$(OUTPUT_RSRC_PATH)\contrast_title.src                  : $(PROJRSRC_PATH)\bitmaps\contrast_title.bmp
$(OUTPUT_RSRC_PATH)\contrast_frame.src                  : $(PROJRSRC_PATH)\bitmaps\contrast_frame.bmp
$(OUTPUT_RSRC_PATH)\contrast_level0.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level0.bmp
$(OUTPUT_RSRC_PATH)\contrast_level1.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level1.bmp
$(OUTPUT_RSRC_PATH)\contrast_level2.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level2.bmp
$(OUTPUT_RSRC_PATH)\contrast_level3.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level3.bmp
$(OUTPUT_RSRC_PATH)\contrast_level4.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level4.bmp
$(OUTPUT_RSRC_PATH)\contrast_level5.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level5.bmp
$(OUTPUT_RSRC_PATH)\contrast_level6.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level6.bmp
$(OUTPUT_RSRC_PATH)\contrast_level7.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level7.bmp
$(OUTPUT_RSRC_PATH)\contrast_level8.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level8.bmp
$(OUTPUT_RSRC_PATH)\contrast_level9.src                 : $(PROJRSRC_PATH)\bitmaps\contrast_level9.bmp
$(OUTPUT_RSRC_PATH)\contrast_level10.src                : $(PROJRSRC_PATH)\bitmaps\contrast_level10.bmp

$(OUTPUT_RSRC_PATH)\splashscreenupper.src               : $(PROJRSRC_PATH)\bitmaps\splashscreenupper.bmp
$(OUTPUT_RSRC_PATH)\splashscreenmid.src                 : $(PROJRSRC_PATH)\bitmaps\splashscreenmid.bmp
$(OUTPUT_RSRC_PATH)\splashscreenlower.src               : $(PROJRSRC_PATH)\bitmaps\splashscreenlower.bmp
$(OUTPUT_RSRC_PATH)\locked.src                          : $(PROJRSRC_PATH)\bitmaps\locked.bmp
$(OUTPUT_RSRC_PATH)\custom_icon.src                     : $(PROJRSRC_PATH)\bitmaps\custom_icon.bmp

$(OUTPUT_RSRC_PATH)\eq_clear_icon.src                   : $(PROJRSRC_PATH)\bitmaps\eq_clear_icon.bmp
$(OUTPUT_RSRC_PATH)\rock_icon.src                       : $(PROJRSRC_PATH)\bitmaps\rock_icon.bmp
$(OUTPUT_RSRC_PATH)\jazz_icon.src                       : $(PROJRSRC_PATH)\bitmaps\jazz_icon.bmp
$(OUTPUT_RSRC_PATH)\classic_icon.src                    : $(PROJRSRC_PATH)\bitmaps\classic_icon.bmp
$(OUTPUT_RSRC_PATH)\pop_icon.src                        : $(PROJRSRC_PATH)\bitmaps\pop_icon.bmp

$(OUTPUT_RSRC_PATH)\ab_mark_a.src                       : $(PROJRSRC_PATH)\bitmaps\ab_mark_a.bmp
$(OUTPUT_RSRC_PATH)\ab_mark_b.src                       : $(PROJRSRC_PATH)\bitmaps\ab_mark_b.bmp

$(OUTPUT_RSRC_PATH)\internal_media.src                  : $(PROJRSRC_PATH)\bitmaps\internal_media.bmp
$(OUTPUT_RSRC_PATH)\external_media.src                  : $(PROJRSRC_PATH)\bitmaps\external_media.bmp
$(OUTPUT_RSRC_PATH)\about_title.src                     : $(PROJRSRC_PATH)\bitmaps\about_title.bmp
$(OUTPUT_RSRC_PATH)\backlight_title.src                 : $(PROJRSRC_PATH)\bitmaps\backlight_title.bmp
$(OUTPUT_RSRC_PATH)\backlight_state_on.src              : $(PROJRSRC_PATH)\bitmaps\backlight_state_on.bmp
$(OUTPUT_RSRC_PATH)\backlight_state_off.src             : $(PROJRSRC_PATH)\bitmaps\backlight_state_off.bmp
$(OUTPUT_RSRC_PATH)\player_name.src                     : $(PROJRSRC_PATH)\bitmaps\player_name.bmp

$(OUTPUT_RSRC_PATH)\battery_00.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_00.bmp
$(OUTPUT_RSRC_PATH)\battery_01.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_01.bmp
$(OUTPUT_RSRC_PATH)\battery_02.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_02.bmp
$(OUTPUT_RSRC_PATH)\battery_03.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_03.bmp
$(OUTPUT_RSRC_PATH)\battery_04.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_04.bmp
$(OUTPUT_RSRC_PATH)\battery_05.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_05.bmp
$(OUTPUT_RSRC_PATH)\battery_06.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_06.bmp
$(OUTPUT_RSRC_PATH)\battery_07.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_07.bmp
$(OUTPUT_RSRC_PATH)\battery_08.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_08.bmp
$(OUTPUT_RSRC_PATH)\battery_09.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_09.bmp
$(OUTPUT_RSRC_PATH)\battery_10.src                      : $(BITMAP_PATH)\icons\battery\8x8\battery_10.bmp
$(OUTPUT_RSRC_PATH)\battery_charging.src                : $(BITMAP_PATH)\icons\battery\8x8\battery_charging.bmp
$(OUTPUT_RSRC_PATH)\string_arial_8_continue_holding_to_power_off.src : $(BITMAP_PATH)\strings\arial_8\string_arial_8_continue_holding_to_power_off.bmp
$(OUTPUT_RSRC_PATH)\string_arial_8_defragmenting_store.src : $(BITMAP_PATH)\strings\arial_8\string_arial_8_defragmenting_store.bmp
$(OUTPUT_RSRC_PATH)\status_16_6_steps_0.src             : $(BITMAP_PATH)\icons\status\16\status_16_6_steps_0.bmp
$(OUTPUT_RSRC_PATH)\status_16_6_steps_1.src             : $(BITMAP_PATH)\icons\status\16\status_16_6_steps_1.bmp
$(OUTPUT_RSRC_PATH)\status_16_6_steps_2.src             : $(BITMAP_PATH)\icons\status\16\status_16_6_steps_2.bmp
$(OUTPUT_RSRC_PATH)\status_16_6_steps_3.src             : $(BITMAP_PATH)\icons\status\16\status_16_6_steps_3.bmp
$(OUTPUT_RSRC_PATH)\status_16_6_steps_4.src             : $(BITMAP_PATH)\icons\status\16\status_16_6_steps_4.bmp
$(OUTPUT_RSRC_PATH)\status_16_6_steps_5.src             : $(BITMAP_PATH)\icons\status\16\status_16_6_steps_5.bmp
$(OUTPUT_RSRC_PATH)\status_16_6_steps_6.src             : $(BITMAP_PATH)\icons\status\16\status_16_6_steps_6.bmp

ifdef D_CLCD
$(OUTPUT_RSRC_PATH)\siglogo1.src : $(PROJRSRC_PATH)\color_rsrc\siglogo1.bmp
$(OUTPUT_RSRC_PATH)\siglogo2.src : $(PROJRSRC_PATH)\color_rsrc\siglogo2.bmp
$(OUTPUT_RSRC_PATH)\siglogo3.src : $(PROJRSRC_PATH)\color_rsrc\siglogo3.bmp
$(OUTPUT_RSRC_PATH)\siglogo4.src : $(PROJRSRC_PATH)\color_rsrc\siglogo4.bmp
$(OUTPUT_RSRC_PATH)\siglogo5.src : $(PROJRSRC_PATH)\color_rsrc\siglogo5.bmp
$(OUTPUT_RSRC_PATH)\siglogo6.src : $(PROJRSRC_PATH)\color_rsrc\siglogo6.bmp
$(OUTPUT_RSRC_PATH)\siglogo7.src : $(PROJRSRC_PATH)\color_rsrc\siglogo7.bmp
$(OUTPUT_RSRC_PATH)\siglogo8.src : $(PROJRSRC_PATH)\color_rsrc\siglogo8.bmp
$(OUTPUT_RSRC_PATH)\siglogo9.src : $(PROJRSRC_PATH)\color_rsrc\siglogo9.bmp
$(OUTPUT_RSRC_PATH)\siglogo10.src :	$(PROJRSRC_PATH)\color_rsrc\siglogo10.bmp
$(OUTPUT_RSRC_PATH)\siglogo11.src :	$(PROJRSRC_PATH)\color_rsrc\siglogo11.bmp
$(OUTPUT_RSRC_PATH)\siglogo12.src :	$(PROJRSRC_PATH)\color_rsrc\siglogo12.bmp
$(OUTPUT_RSRC_PATH)\siglogo13.src :	$(PROJRSRC_PATH)\color_rsrc\siglogo13.bmp
$(OUTPUT_RSRC_PATH)\siglogo.src :	$(PROJRSRC_PATH)\color_rsrc\siglogo.bmp

else
$(OUTPUT_RSRC_PATH)\siglogo1.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo2.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo3.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo4.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo5.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo6.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo7.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo8.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo9.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo10.src :	$(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo11.src :	$(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo12.src :	$(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo13.src :	$(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

$(OUTPUT_RSRC_PATH)\siglogo.src :	$(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $@

endif


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
	copy $(RESOURCE_PATH)\shingyih\NEW_TYPE_LCD\system_lcd_init_seq.src $(OUTPUT_RSRC_PATH)\lcd_controller_init.src
endif

#///////////////////////////////////////////////////////////////////////////////
# Project-specific files
#///////////////////////////////////////////////////////////////////////////////
"$(OBJ_PATH)\main.obj" : $(PROJ_PATH)\main.asm  $(PROJ_PATH)\resource.inc

"$(OBJ_PATH)\msgs.obj" : $(PROJ_PATH)\msgs.asm $(INC_PATH)\msgequ.inc

$(OBJ_PATH)\mainmenu.obj :$(PROJMENUS_PATH)\mainmenu.c
$(OBJ_PATH)\fsutilsc.obj :$(ROOT)\FileSystem\fsutilsc.c
$(OBJ_PATH)\fsutils.obj :$(ROOT)\FileSystem\fsutils.asm
$(OBJ_PATH)\fsunicode.obj : $(ROOT)\FileSystem\fsunicode.c
$(OBJ_PATH)\reentrant.obj : $(ROOT)\FileSystem\reentrant.asm
$(OBJ_PATH)\mainmenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\mainmenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\mainmenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\mainmenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\mainmenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\mainmenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\mainmenu.obj :$(PROJECT_H)
$(OBJ_PATH)\mainmenu.obj :$(OUTPUT_INCLUDE_PATH)\sysequ.h
$(OBJ_PATH)\mainmenu.obj :$(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\mainmenu.obj : $(OUTPUT_INCLUDE_PATH)\errors.h
$(OBJ_PATH)\mainmenu.obj : $(OUTPUT_INCLUDE_PATH)\sysspeed.h
ifdef USE_PLAYLIST3
$(OBJ_PATH)\mainmenu.obj :$(SW_MSGMODULES_PATH)\musiclib\ghdr\musiclib_ghdr.h
endif

$(OBJ_PATH)\playerstatemachine.obj : $(PROJMENUS_PATH)\playerstatemachine.c
$(OBJ_PATH)\playerstatemachine.obj : $(PROJ_PATH)\resource.inc
$(OBJ_PATH)\playerstatemachine.obj : $(INC_PATH)\menumanager.h
$(OBJ_PATH)\playerstatemachine.obj : $(INC_PATH)\exec.h
$(OBJ_PATH)\playerstatemachine.obj : $(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\playerstatemachine.obj : $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\playerstatemachine.obj : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\playerstatemachine.obj : $(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\playerstatemachine.obj : $(OUTPUT_INCLUDE_PATH)\errors.h

$(OBJ_PATH)\display.obj : $(PROJDISP_PATH)\display.c
$(OBJ_PATH)\display.obj : $(PROJ_PATH)\resource.inc
$(OBJ_PATH)\display.obj : $(INC_PATH)\exec.h
$(OBJ_PATH)\display.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\display.obj : $(PROJDISP_PATH)\display.h
$(OBJ_PATH)\display.obj : $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\display.obj : $(PROJECT_H)
$(OBJ_PATH)\display.obj : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\displaysplashscreen.obj : $(PROJDISP_PATH)\displaysplashscreen.c
$(OBJ_PATH)\displaysplashscreen.obj : $(PROJ_PATH)\resource.inc
$(OBJ_PATH)\displaysplashscreen.obj : $(INC_PATH)\exec.h
$(OBJ_PATH)\displaysplashscreen.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\displaysplashscreen.obj : $(PROJDISP_PATH)\display.h
$(OBJ_PATH)\displaysplashscreen.obj : $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\displaysplashscreen.obj : $(OUTPUT_INCLUDE_PATH)\sysequ.h
$(OBJ_PATH)\displaysplashscreen.obj : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\displaylists.obj :$(INC_PATH)\exec.h
$(OBJ_PATH)\displaylists.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\displaylists.obj :$(PROJDISP_PATH)\displaylists.c
$(OBJ_PATH)\displaylists.obj :$(PROJDISP_PATH)\displaylists.h
$(OBJ_PATH)\displaylists.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\displaylists.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\displaylists.obj : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\musicmenu.obj :$(PROJMENUS_PATH)\musicmenu.c
$(OBJ_PATH)\musicmenu.obj :$(PROJMENUS_PATH)\musicmenu.h
$(OBJ_PATH)\musicmenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\musicmenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\musicmenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\musicmenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\musicmenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\musicmenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\musicmenu.obj :$(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\musicmenu.obj :$(OUTPUT_INCLUDE_PATH)\sysspeed.h
$(OBJ_PATH)\musicmenu.obj :$(INC_PATH)\playerlib.h

$(OBJ_PATH)\musicmenu_extra.obj :$(OBJ_PATH)\musicmenu.obj
$(OBJ_PATH)\musicmenu_extra.obj :$(PROJMENUS_PATH)\musicmenu_extra.c
$(OBJ_PATH)\musicmenu_extra.obj :$(PROJMENUS_PATH)\musicmenu_extra.h

ifdef JPEG_APP
"$(OUTPUT_RSRC_PATH)\jpegdisplaymenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegdisplaymenu -F$(OUTPUT_PATH)\$(PROJ).map
ifdef USE_PLAYLIST3
"$(OUTPUT_RSRC_PATH)\jpegmanualmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegmanualmenu,.ptextjpegdisplaycommon,.ptextjpegfileutil,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\jpegslideshowmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegslideshowmenu,.ptextjpegdisplaycommon,.ptextjpegfileutil,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\jpegthumbnailmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegthumbnailmenu,.ptextjpegdisplaycommon,.ptextjpegfileutil,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map
else
ifdef USE_PLAYLIST5
"$(OUTPUT_RSRC_PATH)\jpegmanualmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegmanualmenu,.ptextjpegdisplaycommon -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\jpegslideshowmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegslideshowmenu,.ptextjpegdisplaycommon -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\jpegthumbnailmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegthumbnailmenu,.ptextjpegdisplaycommon -F$(OUTPUT_PATH)\$(PROJ).map

else
"$(OUTPUT_RSRC_PATH)\jpegmanualmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegmanualmenu,.ptextjpegdisplaycommon,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\jpegslideshowmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegslideshowmenu,.ptextjpegdisplaycommon,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\jpegthumbnailmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegthumbnailmenu,.ptextjpegdisplaycommon,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map
endif
endif
ifdef JPEG_ALBUM_ART
"$(OUTPUT_RSRC_PATH)\albumartmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegdisplaycommon -F$(OUTPUT_PATH)\$(PROJ).map
else
"$(OUTPUT_RSRC_PATH)\albumartmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@
endif #JPEG_ALBUM_ART

$(OBJ_PATH)\jpegdisplaymenu.obj :$(PROJMENUS_PATH)\jpegdisplaymenu.c
$(OBJ_PATH)\jpegdisplaymenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\jpegdisplaymenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\jpegdisplaymenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\jpegdisplaymenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\jpegdisplaymenu.obj :$(OUTPUT_INCLUDE_PATH)\sysspeed.h
$(OBJ_PATH)\jpegdisplaymenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\jpegdisplaymenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\jpegdisplaymenu.obj :$(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\jpegmanualmenu.obj :$(PROJMENUS_PATH)\jpegmanualmenu.c
$(OBJ_PATH)\jpegmanualmenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\jpegmanualmenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\jpegmanualmenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\jpegmanualmenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\jpegmanualmenu.obj :$(OUTPUT_INCLUDE_PATH)\sysspeed.h
$(OBJ_PATH)\jpegmanualmenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\jpegmanualmenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\jpegmanualmenu.obj :$(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\jpegmanualmenu.obj :$(PROJMENUS_PATH)\jpegfileutil.h

$(OBJ_PATH)\jpegslideshowmenu.obj :$(PROJMENUS_PATH)\jpegslideshowmenu.c
$(OBJ_PATH)\jpegslideshowmenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\jpegslideshowmenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\jpegslideshowmenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\jpegslideshowmenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\jpegslideshowmenu.obj :$(OUTPUT_INCLUDE_PATH)\sysspeed.h
$(OBJ_PATH)\jpegslideshowmenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\jpegslideshowmenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\jpegslideshowmenu.obj :$(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\jpegslideshowmenu.obj :$(PROJMENUS_PATH)\jpegfileutil.h

$(OBJ_PATH)\jpegthumbnailmenu.obj :$(PROJMENUS_PATH)\jpegthumbnailmenu.c
$(OBJ_PATH)\jpegthumbnailmenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\jpegthumbnailmenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\jpegthumbnailmenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\jpegthumbnailmenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\jpegthumbnailmenu.obj :$(OUTPUT_INCLUDE_PATH)\sysspeed.h
$(OBJ_PATH)\jpegthumbnailmenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\jpegthumbnailmenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\jpegthumbnailmenu.obj :$(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\jpegthumbnailmenu.obj :$(PROJMENUS_PATH)\jpegfileutil.h

$(OBJ_PATH)\jpegdisplaycommon.obj :$(PROJMENUS_PATH)\jpegdisplaycommon.c
$(OBJ_PATH)\jpegdisplaycommon.obj :$(JPEG_ALG_PATH)\JpegDecodeDisplay.h
$(OBJ_PATH)\jpegdisplaycommon.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\jpegdisplaycommon.obj :$(PROJMENUS_PATH)\jpegfileutil.h

else #JPEG_APP

"$(OUTPUT_RSRC_PATH)\jpegdisplaymenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\jpegmanualmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\jpegslideshowmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\jpegthumbnailmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@

ifdef JPEG_ALBUM_ART
"$(OUTPUT_RSRC_PATH)\albumartmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegdisplaycommon -F$(OUTPUT_PATH)\$(PROJ).map

$(OBJ_PATH)\jpegdisplaycommon.obj :$(PROJMENUS_PATH)\jpegdisplaycommon.c
$(OBJ_PATH)\jpegdisplaycommon.obj :$(JPEG_ALG_PATH)\JpegDecodeDisplay.h
$(OBJ_PATH)\jpegdisplaycommon.obj :$(PROJ_PATH)\resource.inc

else #JPEG_ALBUM_ART
"$(OUTPUT_RSRC_PATH)\albumartmenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@

endif
endif

ifdef JPEG_DECODER
JPEGDECODERS=TRUE
endif
ifdef SMVJPEG_DECODER
JPEGDECODERS=TRUE
endif

ifdef JPEGDECODERS
ifndef JPEG_APP
ifndef MOTION_VIDEO
NO_JPEGFILEUTIL=TRUE
endif
endif
ifndef NO_JPEGFILEUTIL
$(OBJ_PATH)\jpegfileutil.obj      :$(PROJMENUS_PATH)\jpegfileutil.c
$(OBJ_PATH)\jpegfileutil.obj      :$(PROJMENUS_PATH)\jpegfileutil.h

$(OBJ_PATH)\jpegfileutilextra.obj     :$(PROJMENUS_PATH)\jpegfileutilextra.c
$(OBJ_PATH)\jpegfileutilextra.obj     :$(PROJMENUS_PATH)\jpegfileutil.h

ifdef USE_PLAYLIST3
"$(OUTPUT_RSRC_PATH)\jpegfileutilextra.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextjpegfileutilextra,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map
else
"$(OUTPUT_RSRC_PATH)\jpegfileutilextra.src" : $(OUTPUT_PATH)\$(PROJ).abs
		@echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map
endif
else #ifndef NO_JPEGFILEUTIL
"$(OUTPUT_RSRC_PATH)\jpegfileutilextra.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@
endif
else #ifdef JPEGDECODERS
"$(OUTPUT_RSRC_PATH)\jpegfileutilextra.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@
endif



ifdef MOTION_VIDEO
ifdef USE_PLAYLIST3
"$(OUTPUT_RSRC_PATH)\motionvideomenu.src"       : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Menus - $@
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextmotionvideomenu,.ptextjpegfileutil,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map
else
ifdef USE_PLAYLIST5
"$(OUTPUT_RSRC_PATH)\motionvideomenu.src"       : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Menus - $@
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextmotionvideomenu -F$(OUTPUT_PATH)\$(PROJ).map
else
"$(OUTPUT_RSRC_PATH)\motionvideomenu.src"       : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Menus - $@
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextmotionvideomenu,.ptextfindfirst,.ptextfindnext -F$(OUTPUT_PATH)\$(PROJ).map
endif
endif
"$(OUTPUT_RSRC_PATH)\motionvideomenuinitstate.src"       : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Menus - $@
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextmotionvideomenuinitstate -F$(OUTPUT_PATH)\$(PROJ).map

$(OBJ_PATH)\motionvideomenu.obj :$(PROJMENUS_PATH)\motionvideomenu.c
$(OBJ_PATH)\motionvideomenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\motionvideomenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\motionvideomenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\motionvideomenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\motionvideomenu.obj :$(OUTPUT_INCLUDE_PATH)\sysspeed.h
$(OBJ_PATH)\motionvideomenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\motionvideomenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\motionvideomenu.obj :$(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\motionvideomenu.obj :$(OUTPUT_INCLUDE_PATH)\sysequ.h

$(OBJ_PATH)\motionvideomenuinitstate.obj :$(PROJMENUS_PATH)\motionvideomenuinitstate.c
$(OBJ_PATH)\motionvideomenuinitstate.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\motionvideomenuinitstate.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\motionvideomenuinitstate.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\motionvideomenuinitstate.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\motionvideomenuinitstate.obj :$(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\motionvideomenuinitstate.obj :$(OUTPUT_INCLUDE_PATH)\sysequ.h

else

"$(OUTPUT_RSRC_PATH)\motionvideomenu.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\motionvideomenuinitstate.src" : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src  $@

endif

$(OBJ_PATH)\voicemenu.obj :$(PROJMENUS_PATH)\voicemenu.c
$(OBJ_PATH)\voicemenu.obj :$(PROJMENUS_PATH)\voicemenu.h
$(OBJ_PATH)\voicemenu.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\voicemenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\voicemenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\voicemenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\voicemenu.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\voicemenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\voicemenu.obj :$(PROJMENUS_PATH)\recorderstatemachine.h
$(OBJ_PATH)\voicemenu.obj :$(PROJMENUS_PATH)\menus.h
ifdef USE_PLAYLIST3
$(OBJ_PATH)\voicemenu.obj :$(SW_MSGMODULES_PATH)\musiclib\ghdr\musiclib_ghdr.h
endif

$(OBJ_PATH)\fmtunermenu.obj :$(PROJMENUS_PATH)\fmtunermenu.c
$(OBJ_PATH)\fmtunermenu.obj :$(PROJMENUS_PATH)\fmtunermenu.h
$(OBJ_PATH)\fmtunermenu.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\fmtunermenu.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\fmtunermenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\fmtunermenu.obj :$(PROJMENUS_PATH)\recorderstatemachine.h
$(OBJ_PATH)\fmtunermenu.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\fmtunermenu.obj : $(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\fmtunermenu.obj : $(OUTPUT_INCLUDE_PATH)\sysspeed.h

$(OBJ_PATH)\systemmenu.obj : $(PROJMENUS_PATH)\systemmenu.c
$(OBJ_PATH)\systemmenu.obj : $(PROJDISP_PATH)\display.h
$(OBJ_PATH)\systemmenu.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\systemmenu.obj : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\systemmenu.obj : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\deletemenu.obj : $(PROJMENUS_PATH)\deletemenu.c
$(OBJ_PATH)\deletemenu.obj : $(PROJDISP_PATH)\display.h
$(OBJ_PATH)\deletemenu.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\deletemenu.obj : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\deletemenu.obj : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\aboutmenu.obj : $(PROJMENUS_PATH)\aboutmenu.c
$(OBJ_PATH)\aboutmenu.obj : $(PROJDISP_PATH)\display.h
$(OBJ_PATH)\aboutmenu.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\aboutmenu.obj : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\aboutmenu.obj : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\backlightmenu.obj : $(PROJMENUS_PATH)\backlightmenu.c
$(OBJ_PATH)\backlightmenu.obj : $(PROJDISP_PATH)\display.h
$(OBJ_PATH)\backlightmenu.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\backlightmenu.obj : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\backlightmenu.obj : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\contrastmenu.obj : $(PROJMENUS_PATH)\contrastmenu.c
$(OBJ_PATH)\contrastmenu.obj : $(PROJDISP_PATH)\display.h
$(OBJ_PATH)\contrastmenu.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\contrastmenu.obj : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\contrastmenu.obj : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\eqmenu.obj : $(PROJMENUS_PATH)\eqmenu.c
$(OBJ_PATH)\eqmenu.obj : $(PROJMENUS_PATH)\eqmenu.h
$(OBJ_PATH)\eqmenu.obj : $(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\eqmenu.obj : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\eqmenu.obj : $(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\eqmenu.obj : $(PROJDISP_PATH)\display.h
$(OBJ_PATH)\eqmenu.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\eqmenu.obj : $(OUTPUT_INCLUDE_PATH)\messages.h

$(OBJ_PATH)\playmodemenu.obj    : $(PROJMENUS_PATH)\playmodemenu.c
$(OBJ_PATH)\playmodemenu.obj    : $(PROJMENUS_PATH)\playmodemenu.h
$(OBJ_PATH)\playmodemenu.obj    : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\playmodemenu.obj    : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\playmodemenu.obj    : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\pwrsettingsmenu.obj : $(PROJMENUS_PATH)\pwrsettingsmenu.c
$(OBJ_PATH)\pwrsettingsmenu.obj : $(PROJMENUS_PATH)\pwrsettingsmenu.h
$(OBJ_PATH)\pwrsettingsmenu.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\pwrsettingsmenu.obj : $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\pwrsettingsmenu.obj : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\saveusertime.obj  :   $(PROJMENUS_PATH)\saveusertime.c
$(OBJ_PATH)\settingsmenu.obj  :  $(PROJMENUS_PATH)\settingsmenu.c
$(OBJ_PATH)\settingsmenu.obj  :  $(PROJMENUS_PATH)\settingsmenu.h
$(OBJ_PATH)\settingsmenu.obj  :  $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\settingsmenu.obj  :  $(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\settingsmenu.obj  : $(PROJMENUS_PATH)\menus.h

$(OBJ_PATH)\micbias.obj              : $(ADC_PATH)\micbias.c
$(OBJ_PATH)\micbias.obj              : $(INC_PATH)\micbias.h
$(OBJ_PATH)\micbias.obj              : $(OUTPUT_INCLUDE_PATH)\project.h

$(OBJ_PATH)\recorderstatemachine.obj : $(PROJMENUS_PATH)\recorderstatemachine.c
$(OBJ_PATH)\recorderstatemachine.obj : $(PROJ_PATH)\resource.inc
$(OBJ_PATH)\recorderstatemachine.obj : $(INC_PATH)\exec.h
$(OBJ_PATH)\recorderstatemachine.obj : $(INC_PATH)\micbias.h
$(OBJ_PATH)\recorderstatemachine.obj : $(ADC_PATH)\micbias.c
$(OBJ_PATH)\recorderstatemachine.obj : $(INC_PATH)\menumanager.h
$(OBJ_PATH)\recorderstatemachine.obj : $(PROJMENUS_PATH)\recorderstatemachine.h
$(OBJ_PATH)\recorderstatemachine.obj : $(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\recorderstatemachine.obj : $(PROJMENUS_PATH)\menus.h
$(OBJ_PATH)\recorderstatemachine.obj : $(OUTPUT_INCLUDE_PATH)\errors.h

$(OBJ_PATH)\sysdisplay.obj      :   $(PROJDISP_PATH)\sysdisplay.c
$(OBJ_PATH)\sysdisplay.obj      :   $(PROJDISP_PATH)\display.h
$(OBJ_PATH)\sysdisplay.obj      :   $(INC_PATH)\exec.h
$(OBJ_PATH)\sysdisplay.obj      :   $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\sysdisplay.obj      : $(PROJMENUS_PATH)\menus.h


$(OBJ_PATH)\recordsettingsmenu.obj :$(PROJMENUS_PATH)\recordsettingsmenu.c
$(OBJ_PATH)\recordsettingsmenu.obj :$(INC_PATH)\exec.h
$(OBJ_PATH)\recordsettingsmenu.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\recordsettingsmenu.obj : $(PROJMENUS_PATH)\menus.h

ifdef SPECTRUM_ANAL
"$(OUTPUT_RSRC_PATH)\spectrogram.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Menus
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=MenuManager_p,.ptextspectrogram,.ptextplayerlib -F$(OUTPUT_PATH)\$(PROJ).map

$(OUTPUT_RSRC_PATH)\string_spectrogram_menu.src   : $(PROJRSRC_PATH)\strings\string_spectrogram_menu.txt

$(OBJ_PATH)\spectrogram.obj :$(PROJMENUS_PATH)\spectrogram.c
$(OBJ_PATH)\spectrogram.obj :$(PROJMENUS_PATH)\spectrogram.h
$(OBJ_PATH)\spectrogram.obj :$(PROJ_PATH)\resource.inc
$(OBJ_PATH)\spectrogram.obj :$(PROJMENUS_PATH)\buttons.h
$(OBJ_PATH)\spectrogram.obj :$(OUTPUT_INCLUDE_PATH)\messages.h
$(OBJ_PATH)\spectrogram.obj :$(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\spectrogram.obj :$(PROJMENUS_PATH)\playerstatemachine.h
$(OBJ_PATH)\spectrogram.obj :$(PROJDISP_PATH)\display.h
$(OBJ_PATH)\spectrogram.obj  :$(PROJMENUS_PATH)\menus.h

else

$(OUTPUT_RSRC_PATH)\string_spectrogram_menu.src : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\string_spectrogram_menu.src

$(OUTPUT_RSRC_PATH)\spectrogram.src : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\spectrogram.src

endif

"$(OBJ_PATH)\STFM1000TestPresence.obj"  : $(ROOT)\System\MsgModules\Hardware\Tuner\STFM1000\STFM1000TestPresence.c
"$(OBJ_PATH)\STFM1000TestPresence.obj"  : $(ROOT)\System\MsgModules\Hardware\Tuner\STFM1000\STFM1000.h

$(OUTPUT_INCLUDE_PATH)\resource.h:$(PROJ_PATH)\resource.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

$(PROJECT_H) :$(COMMON_PROJ_PATH)\project.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

$(OUTPUT_INCLUDE_PATH)\sysequ.h : $(INC_PATH)\sysequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

$(OUTPUT_INCLUDE_PATH)\errors.h :$(INC_PATH)\errors.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

$(HWEQU_H) : $(HW_REGS_INC_PATH)\hwequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@


#///////////////////////////////////////////////////////////////////////////////
# General System
#///////////////////////////////////////////////////////////////////////////////
$(OBJ_PATH)\updateautoshutdowntimer.obj : $(PROJ_PATH)\system\updateautoshutdowntimer.c

$(OBJ_PATH)\sysfatwrite.obj : $(ROOT)\System\Common\sysfatwrite.asm
$(OBJ_PATH)\changeplayset.obj : $(PROJ_PATH)\system\changeplayset.c
$(OBJ_PATH)\changeplayset.obj : $(INC_PATH)\exec.h
$(OBJ_PATH)\changeplayset.obj : $(INC_PATH)\types.h
$(OBJ_PATH)\changeplayset.obj : $(INC_PATH)\syscoder.h
$(OBJ_PATH)\changeplayset.obj : $(INC_PATH)\sysmem.h
$(OBJ_PATH)\changeplayset.obj : $(OUTPUT_INCLUDE_PATH)\resource.h
$(OBJ_PATH)\changeplayset.obj : $(OUTPUT_INCLUDE_PATH)\messages.h


"$(OBJ_PATH)\miscproject.obj" : $(PROJSYS_PATH)\miscproject.c  $(OUTPUT_INCLUDE_PATH)\errors.h
"$(OBJ_PATH)\fsproj.obj" : $(PROJ_PATH)\fsproj.c  $(PROJ_PATH)\fsproj.h

$(OUTPUT_INCLUDE_PATH)\sysresources.h:$(INC_PATH)\sysresources.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

ifdef D_D3410
$(OBJ_PATH)\cfporthil_resource.obj : $(SYMBOLS_PATH)\cfporthil_resource.asm
endif

ifdef D_MMC
$(OBJ_PATH)\checkremovablemedia.obj : $(SYS_COMMON_PATH)\checkremovablemedia.c
endif










"$(OBJ_PATH)\GetResourceTag.obj" : $(PROJ_PATH)\GetResourceTag.c
"$(OBJ_PATH)\GetResourceTag.obj" : $(INC_PATH)\persist_bits.h


#///////////////////////////////////////////////////////////////////////////////
# Module code extraction from abs
#///////////////////////////////////////////////////////////////////////////////
# Extract named clusters from player.abs & create stmpsys.s (srec)
# extractcluster player.abs -S..\output_3500\stmpsys.s=P_vector, P_sys_cluster, X_sys_cluster, P_Media_DD_Read_cluster,
#                                                      P_Module_overlay_clstr, Y_sys, Y_Extra, Y_Sys2
#   $@ is stmpsys.s : $! is player.abs
#       Note: $1818 below in PXram & PYram cfg registers at x:0xffe8 and x:0xffe9 gives 24KWords of
#       P from each of 2 banks so, 48KWords total P, 24KWords X, 24KWords Y.
#       Total P+X+Y addressable RAM size = 96KWords as required for 35xx family.
$(OUTPUT_STMPSYS_S):$(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System S-rec stmpsys.s for $(DEVICE) device family extracting shown $(PROJ).abs listed clusters:
        $(BIN_PATH)\$(EXTRACT_EXE) $! -S$@=P_vector,P_sys_cluster,X_sys_cluster,P_Media_DD_Read_cluster,P_Module_overlay_clstr,Y_sys,Y_Extra,Y_Sys2 -V$(VERSION) -OFFE8=1818 -OFFE9=1818
        @echo Converting stmpsys.s to stmpsys.sb
        @$(BIN_PATH)\stosbcon $(OUTPUT_PATH)\stmpsys.s $(OUTPUT_PATH)\stmpsys.sb

"$(OUTPUT_RSRC_PATH)\contrastmenu.src"          : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\deletemenu.src"            : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\aboutmenu.src"             : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\backlightmenu.src"         : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\systemmenu.src"            : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\eqmenu.src"                : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\playmodemenu.src"          : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\pwrsettingsmenu.src"       : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\settingsmenu.src"          : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\mainmenu.src"              : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\softtimermod.src"          : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\display.src"               : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\playerstatemachine.src"    : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\musicmenu.src"             : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\fmtunermenu.src"           : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\voicemenu.src"             : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\recorderstatemachine.src"  : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\displaylists.src"          : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
"$(OUTPUT_RSRC_PATH)\record_settings_menu_code_bank.src" : $(OUTPUT_PATH)\$(PROJ).abs menus.loc
        @echo Extract Menus from $(PROJ).abs using loc and map
        @$(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)\

##########################
"$(OUTPUT_RSRC_PATH)\restoresysdrive.src"       : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\RestoreSysDrive.loc
        @echo Extract RestoreSysDrive.src from $(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\RestoreSysDrive.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
 #######################

# Funclets
ifdef MAKE_FUNCLET_RSRC

"$(OUTPUT_RSRC_PATH)\Funclet_DisplaySplashScreen.src" : $(OUTPUT_PATH)\$(PROJ).abs
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_DisplaySplashScreen_P -F$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000TestPresence.src"          : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_STFM1000TestPresence
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_STFM1000_TEST_PRESENCE_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_I2CWriteTunerRegister.src"          : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_I2CWriteTunerRegister
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_I2C_WRITE_TUNER_REGISTER_P -f$(OUTPUT_PATH)\$(PROJ).map

"$(OUTPUT_RSRC_PATH)\Funclet_I2CReset.src"          : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclet_I2CReset
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -M$@=SYSFUNCLET_I2C_RESET_P -f$(OUTPUT_PATH)\$(PROJ).map


"$(OUTPUT_RSRC_PATH)\updatevolume.src"                          : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysResetAllInt.src"                : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysSetupDecVec.src"                : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysSetupEncVec.src"                : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysTimeInit.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_AnalogInit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_UsbConnected.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_ButtonInit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_MixerMasterFadeOut.src"            : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_MixerMasterFadeIn.src"             : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_PowerUpHeadPhones.src"             : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_PowerDownHeadPhones.src"           : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_StartProject.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_ModuleSetTimer.src"                : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_MediaDetection.src"                : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysStopDecoder.src"                : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysStopEncoder.src"                : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysSetDecoder.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysSetEncoder.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_ProcessPT.src"                     : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysSetSpeed.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysGetVolume.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_DisableStmpGeneratedMicBias.src"   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_EnableStmpGeneratedMicBias.src"    : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Funclets
        @echo $(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\funclet.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
             @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\funclet.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\arrangefilename.src" : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\wma_funclet.loc
"$(OUTPUT_RSRC_PATH)\Funclet_restorebbtable.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\changetorootdirectory.src" : $(OUTPUT_PATH)\$(PROJ).abs $(LOC_PATH)\wma_funclet.loc
"$(OUTPUT_RSRC_PATH)\clearcluster.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\createdirectory.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\deletecontent.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\deleterecord.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fastopen.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fcreate.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\filegetattrib.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\filegetdate.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\filesetattrib.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\filesetdate.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\readrootdirrecord.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\setcurrentpos.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fsinit.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fsdriveinit.src" :                 $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\setcwdhandle.src" :                $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fsclearBuf.src" :                  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fsshutdown.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\readdevicerecord.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\checkspaceinrootdir.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fgetfasthandle.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\freehandle.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\updatehandlemode.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ishandlewriteallocated.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\searchfreehandleallocate.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\isfileopen.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\iscurrworkdir.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\changepath.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\chdir.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\chdirFromOffset.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\deletetree.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\deleteallrecords.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\cleardata.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\changetolowleveldir.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getrecordnumber.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fileremove.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\charactersearch.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\stringcompare.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fopen.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fopenw.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fremove.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fremovew.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mkdir.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mkdirw.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\rmdir.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\rmdirw.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\searchdirectory.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\sethandleforsearch.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fclose.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fgetc.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fgets.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fputc.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fputs.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\arrangelongfilename.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\convert_itoa.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\createdirrecord.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\chksum.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\createshortdirrecord.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\discardtrailigperiods.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\extractfilename.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\extractfilenamew.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\extractpathw.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\extractpath.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\findfreerecord.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getnamew.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getname.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\isdirectoryempty.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\isshortnamevalid.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\longdirmatch.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\changecase.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\unicodetooem.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\matchdirrecord.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\matchdirrecordw.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\setcwd.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\setshortfilename.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\generatefilenametail.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\dbcstounicode.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\shortdirmatch.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\uppercase.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\setfilename.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\convertoshortname.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\strcpy.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\strcpyw.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\strlengthw.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\strlength.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ConstructLongFileName.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\strcpyucs3_2.src"              : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\discardtrailigperiodsucs3.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\discardtrailigperiodsw.src"    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getvolumelabel.src"            : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_loadusertime.src"          : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_saveusertime.src"          : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysLRADCBrownoutInit.src"          : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SysBatteryGetLevel.src"          : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\setvolumelabel.src"            : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SteppingVoltageSet.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_Init5VSense.src"          : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_UpdateDCDCDutyCycle.src" : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_ChangePlaySet.src" : $(OUTPUT_PATH)\$(PROJ).abs
    @echo Extract Funclets from $(PROJ).abs per wma_funclet.loc and $(PROJ).map
    @echo @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\wma_funclet.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
          @$(BIN_PATH)\$(EXTRACT_EXE) $(OUTPUT_PATH)\$(PROJ).abs -L$(LOC_PATH)\wma_funclet.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

ifdef D_CLCD
"$(OUTPUT_RSRC_PATH)\Funclet_HalDisplayInit.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SalDisplayInit.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_SalHandleMessage.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_FixBitmapHeader.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_BitmapReadHeader.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_BitmapReadPalette.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_BitmapReadBitMask.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_BitmapPrintNonPalette.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_BitmapPrintPalette.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_DisplayPrintBitmapFile.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\Funclet_DisplayPrintBitmapResource.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract Display Funclets
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\display_funclet.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
else
"$(OUTPUT_RSRC_PATH)\Funclet_HalDisplayInit.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_SalDisplayInit.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_SalHandleMessage.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_FixBitmapHeader.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_BitmapReadHeader.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_BitmapReadPalette.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_BitmapReadBitMask.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_BitmapPrintNonPalette.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_BitmapPrintPalette.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_DisplayPrintBitmapFile.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

"$(OUTPUT_RSRC_PATH)\Funclet_DisplayPrintBitmapResource.src"	:  $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src  $@

endif

ifdef D_JANUS

"$(OUTPUT_RSRC_PATH)\two_adic_inverse.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mp_shift.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mp_significant_bit_count.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\set_immediate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\multiply_immediate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\multiply.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\divide_precondition_1.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\divide_immediate.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ecaffine_exponentiation_tabular.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ecaffine_table_construction.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ecproj5_to_ecaffine.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ecproj5_set_infinite.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ecaffine_on_curve.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ecaffine_addition.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ecaffine_addition_subtraction.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ecaffine_attributes2.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\kfdesc_initialize.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\kimmediate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\kprime_immediater.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\kprime_sqrter.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\kinitialize_prime.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mod_lucasuv.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mod_lucas.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\bucket_multiply.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mod_exp2000.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mod_exp.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\modmul_choices1.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mod_sqrt.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\create_modulus.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\from_modular.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\add_immediate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\add_diff.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\add_full.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\compare_sum_same.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\sub_immediate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mp_initialization.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\new_random_bytes.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\new_random_dword_interval.src" :       $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mp_gcdex.src" :                        $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mp_gcdex_split1.src" :                 $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\new_random_digit_interval.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\new_random_mod.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\new_random_mod_nonzero.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\new_random_digits.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\words_to_ecaffine.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\ecaffine_to_dwords.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_threadunsafepkinit.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\pkinit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_pk_genkeypair.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_pk_encrypt.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_pk_decrypt.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\byte_array_mod_bignum.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\fe2ipmod.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_pk_sign.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_pk_verify.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\random_bytes.src"                  : $(OUTPUT_PATH)\$(PROJ).abs


        @echo Extract Funclets
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\P_pkcrypto_funclets.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\legacy_decryptcontentkey.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_bbx_symmetricverify.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_bbx_legacycipherkeysetup.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_bbx_initialize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_bbx_canbind.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_decryptcontentkey.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\verifychecksum.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_bbx_hashvalue.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_bbx_cipherkeysetup.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_findkeypair.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_b64_decodew.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_b64_encodew.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mac_inv32.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_parselicenseattributes.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_loadlicenseattributes.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_performactions.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_getlicenseinfoandsetup.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_processendofchain.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lic_completelicensechain.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lic_reportactions.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lic_checkclockrollback.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_cphr_init.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drmcrt_iswxdigit.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drmcrt_towlower.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drmcrt_wcsntol.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drmcrt_wcslen.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drmcrt_wcsncpy.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drmcrt_memmove.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\performoperation_part1.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\performoperation_part2.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\performoperation_part3.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\performoperation_part4.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\performoperation_part5.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\performoperation_part6.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression_no_more_tokens.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_iscachedevent.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\isvalidfunction.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\functiongetvalue.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\globalsetvariable.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variabledrmkgetorset.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variabledrmgetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variablemachinegetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variableappgetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variablesecstategetorset.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variablelicensegetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variablecontentgetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variabledevicegetorset.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variablepmlicensegetorset.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_basicheaderchecks.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hdr_getattribute.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_createstore.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_init.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_uninit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_openstore.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_closestore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_deleteslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_slotseek.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_slotwrite.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_slotresize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_initslotenum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_slotenumnext.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_cleanupstore.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdscopyslot_child2child.src"              : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdscopychildpayload.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsdefragmentfile.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdscleanupnamespace.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdscleanupstore.src"                      : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lst_clean.src"                      : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdslocatefreeblockforslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdslockblock2deleteslot.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsunlockblock2deleteslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdscreatenamespace.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsfilepos2blocknum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_writesrn.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_writecommonblockheader.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_writechildblockheader.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_readdatablockheader.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_writedatablockheader.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsexpandstore.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsallocblock.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsfreeblock.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdscreateoversizedslot.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsallocslotinfile.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdswriteblockhdr.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsadjustchildpayload.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsfindleftmostleafblock.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdscreateandopenslot.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsremoveslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdscopyandlocknewslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsrelocateslotandkeepcurrlock.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdstraversenextrightsiblingblock.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdstraverseblocksinpostorder.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsslotresize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_isnull.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsgensrnhash.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsallocblockbuffer.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_readsrn.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsgetputchildblocknum.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdshashkeytoindex.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsloadsrn.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsslotwrite.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsinitslotenum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs


        @echo Extract Funclets
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\P_DRM_Funclets.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)

"$(OUTPUT_RSRC_PATH)\_hdsslotenumnext.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_levl_evaluateexpression.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_levl_performoperations.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lic_getattribute.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lic_getenablingbits.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getlspubkey.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lic_verifysignature.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_loadproritizedlist.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lst_open.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lst_close.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lst_getlicense.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lst_initenum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lst_enumnext.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_lst_enumdelete.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_processextensions.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_processidlist.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_processexclusions.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_processinclusions.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_opl_processcopyoutputleveldata.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_getopllevel.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_opl_processplayoutputleveldata.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_loadattributesintocache.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_openandverifyslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_openkeytokens.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_updateattributetokeninslot.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_closekey.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_gettokenvalue.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_settokenvalue.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_createlicensestatepassword.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_createglobalstorepassword.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_promoteansitounicode.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_pk_symmetricverify.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_pk_symmetriccrypt.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_pk_symmetricsign.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_getversionfromstring.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_stringinsertblanksubstring.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_stringremovesubstring.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_dstrsearch.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_dstrsearchreverse.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_numbertostring.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_getversionasstring.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_stringtoguid.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_decodekid.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_utl_ensuredataalignment.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\setcountedexpirycategory.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\overlappingdates.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_asd_parsev2license.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\neginfdate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\infdate.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\isexpired.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getsecstateattr.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\setexpirycategory.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getv2licenseinfo.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getnextlicense.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\aggregate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_asd_getlicenseaggregatedata.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_scannodeforattributew.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_getxmlnodecdataw.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_xml_getnodeattribute.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_xml_getnodecdata.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_dcp_getattribute.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_dcp_loadpropertiescache.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_verifysymmerticsignature.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_ddc_getdevicecertificate.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_createdevicestore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_freedrmmanagerinternalcontexts.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_setuplicevalobjecttoshare.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_mapdrmerror.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_comparemachineid.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\initmgrcontext.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mgr_initialize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mgr_uninitialize.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mgr_setv2header.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mgr_bind.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mgr_commit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mgr_cleanuplicensestore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\januscleandatastore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mtr_openid.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mtr_incrementcount.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mtr_closecontext.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_openfile.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_setendoffile.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_writefile.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_genrandombytes.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_getfallbacksigningkey.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_getdevicecerttemplate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_setdevicecert.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_getclockresetstate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_setclockresetstate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_getdrmfullpathname.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_getuniqueid.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_getprivatekey.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_getgroupcertprivatekey.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_getdevicecert.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_getdevicecert.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_getsecurestoreglobalpasswordseed.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_snc_openstore.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_snc_closestore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_setkidstoredata.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_snc_deletekid.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_snc_updatekid.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mgr_setv1header.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\functiongetvalue_part1.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\functiongetvalue_fn_datediff.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\eval.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_opennamespace.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_openslot.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_applydiffstostore.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_verifyslothash.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdscreatenamespacestore.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_createstore2.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsprealloc.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_systemtimetofiletime.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_filetimetosystemtime.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\gendevicecertificate.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drmcrt_memmove_mem.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\copyhdsdtore.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getprivkey.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\generatedevicecert.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_b64_decodew_inplace.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_setsystemtime.src" :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_closelockedslot.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\oem_getdevicedatetime.src" :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_setdata.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_systemtime_to_struct_tm.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_struct_tm_to_systemtime.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_checksecureclock.src" :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_getdata.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\localtime.src" :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_getlockeddata.src" :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\variablesecuretimegetorset.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_systemtimetotime_t.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_mtr_updatedata.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\gmtime.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_hdsupdatesrn.src" :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_openandlockslot.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_preparesecureclockdataforwriting.src" :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\strtol.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_preparesecureclockdataforreading.src" :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_sst_setlockeddata.src" :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\drm_clk_initchecksecureclock.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\mktime.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\getbase64decodedkey.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_initslot.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
"$(OUTPUT_RSRC_PATH)\_loadslot.src"    :   $(OUTPUT_PATH)\$(PROJ).abs

        @echo Extract Funclets
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -L$(LOC_PATH)\P_DRM_Funclets1.loc -F$(OUTPUT_PATH)\$(PROJ).map -P$(OUTPUT_RSRC_PATH)
else

"$(OUTPUT_RSRC_PATH)\_iscachedevent.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_iscachedevent.src
"$(OUTPUT_RSRC_PATH)\setcountedexpirycategory.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\setcountedexpirycategory.src
"$(OUTPUT_RSRC_PATH)\two_adic_inverse.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\two_adic_inverse.src
"$(OUTPUT_RSRC_PATH)\mp_shift.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mp_shift.src
"$(OUTPUT_RSRC_PATH)\mp_significant_bit_count.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mp_significant_bit_count.src
"$(OUTPUT_RSRC_PATH)\set_immediate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\set_immediate.src
"$(OUTPUT_RSRC_PATH)\multiply_immediate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\multiply_immediate.src
"$(OUTPUT_RSRC_PATH)\multiply.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\multiply.src
"$(OUTPUT_RSRC_PATH)\divide_precondition_1.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\divide_precondition_1.src
"$(OUTPUT_RSRC_PATH)\divide_immediate.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\divide_immediate.src
"$(OUTPUT_RSRC_PATH)\ecaffine_exponentiation_tabular.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\ecaffine_exponentiation_tabular.src
"$(OUTPUT_RSRC_PATH)\ecaffine_table_construction.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\ecaffine_table_construction.src
"$(OUTPUT_RSRC_PATH)\ecproj5_to_ecaffine.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\ecproj5_to_ecaffine.src
"$(OUTPUT_RSRC_PATH)\ecproj5_set_infinite.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\ecproj5_set_infinite.src
"$(OUTPUT_RSRC_PATH)\ecaffine_on_curve.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\ecaffine_on_curve.src
"$(OUTPUT_RSRC_PATH)\ecaffine_addition.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\ecaffine_addition.src
"$(OUTPUT_RSRC_PATH)\ecaffine_addition_subtraction.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\ecaffine_addition_subtraction.src
"$(OUTPUT_RSRC_PATH)\ecaffine_attributes2.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\ecaffine_attributes2.src
"$(OUTPUT_RSRC_PATH)\kfdesc_initialize.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\kfdesc_initialize.src
"$(OUTPUT_RSRC_PATH)\kimmediate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\kimmediate.src
"$(OUTPUT_RSRC_PATH)\kprime_immediater.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\kprime_immediater.src
"$(OUTPUT_RSRC_PATH)\kprime_sqrter.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\kprime_sqrter.src
"$(OUTPUT_RSRC_PATH)\kinitialize_prime.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\kinitialize_prime.src
"$(OUTPUT_RSRC_PATH)\mod_lucasuv.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mod_lucasuv.src
"$(OUTPUT_RSRC_PATH)\mod_lucas.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mod_lucas.src
"$(OUTPUT_RSRC_PATH)\bucket_multiply.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\bucket_multiply.src
"$(OUTPUT_RSRC_PATH)\mod_exp2000.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mod_exp2000.src
"$(OUTPUT_RSRC_PATH)\mod_exp.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mod_exp.src
"$(OUTPUT_RSRC_PATH)\modmul_choices1.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\modmul_choices1.src
"$(OUTPUT_RSRC_PATH)\mod_sqrt.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mod_sqrt.src
"$(OUTPUT_RSRC_PATH)\create_modulus.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\create_modulus.src
"$(OUTPUT_RSRC_PATH)\from_modular.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\from_modular.src
"$(OUTPUT_RSRC_PATH)\add_immediate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\add_immediate.src
"$(OUTPUT_RSRC_PATH)\add_diff.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\add_diff.src
"$(OUTPUT_RSRC_PATH)\add_full.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\add_full.src
"$(OUTPUT_RSRC_PATH)\compare_sum_same.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\compare_sum_same.src
"$(OUTPUT_RSRC_PATH)\sub_immediate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\sub_immediate.src
"$(OUTPUT_RSRC_PATH)\mp_initialization.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mp_initialization.src
"$(OUTPUT_RSRC_PATH)\new_random_bytes.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\new_random_bytes.src
"$(OUTPUT_RSRC_PATH)\new_random_dword_interval.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\new_random_dword_interval.src
"$(OUTPUT_RSRC_PATH)\mp_gcdex.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mp_gcdex.src
"$(OUTPUT_RSRC_PATH)\mp_gcdex_split1.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mp_gcdex_split1.src
"$(OUTPUT_RSRC_PATH)\new_random_digit_interval.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\new_random_digit_interval.src
"$(OUTPUT_RSRC_PATH)\new_random_mod.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\new_random_mod.src
"$(OUTPUT_RSRC_PATH)\new_random_mod_nonzero.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\new_random_mod_nonzero.src
"$(OUTPUT_RSRC_PATH)\new_random_digits.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\new_random_digits.src
"$(OUTPUT_RSRC_PATH)\words_to_ecaffine.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\words_to_ecaffine.src
"$(OUTPUT_RSRC_PATH)\ecaffine_to_dwords.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\ecaffine_to_dwords.src
"$(OUTPUT_RSRC_PATH)\_threadunsafepkinit.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_threadunsafepkinit.src
"$(OUTPUT_RSRC_PATH)\pkinit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\pkinit.src
"$(OUTPUT_RSRC_PATH)\drm_pk_genkeypair.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_pk_genkeypair.src
"$(OUTPUT_RSRC_PATH)\drm_pk_encrypt.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_pk_encrypt.src
"$(OUTPUT_RSRC_PATH)\drm_pk_decrypt.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_pk_decrypt.src
"$(OUTPUT_RSRC_PATH)\byte_array_mod_bignum.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\byte_array_mod_bignum.src
"$(OUTPUT_RSRC_PATH)\fe2ipmod.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\fe2ipmod.src
"$(OUTPUT_RSRC_PATH)\drm_pk_sign.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_pk_sign.src
"$(OUTPUT_RSRC_PATH)\drm_pk_verify.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_pk_verify.src
"$(OUTPUT_RSRC_PATH)\random_bytes.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\random_bytes.src
"$(OUTPUT_RSRC_PATH)\legacy_decryptcontentkey.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\legacy_decryptcontentkey.src
"$(OUTPUT_RSRC_PATH)\drm_bbx_symmetricverify.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_bbx_symmetricverify.src
"$(OUTPUT_RSRC_PATH)\drm_bbx_legacycipherkeysetup.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_bbx_legacycipherkeysetup.src
"$(OUTPUT_RSRC_PATH)\drm_bbx_initialize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_bbx_initialize.src
"$(OUTPUT_RSRC_PATH)\drm_bbx_canbind.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_bbx_canbind.src
"$(OUTPUT_RSRC_PATH)\_decryptcontentkey.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_decryptcontentkey.src
"$(OUTPUT_RSRC_PATH)\verifychecksum.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\verifychecksum.src
"$(OUTPUT_RSRC_PATH)\drm_bbx_hashvalue.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_bbx_hashvalue.src
"$(OUTPUT_RSRC_PATH)\drm_bbx_cipherkeysetup.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_bbx_cipherkeysetup.src
"$(OUTPUT_RSRC_PATH)\_findkeypair.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_findkeypair.src
"$(OUTPUT_RSRC_PATH)\drm_b64_decodew.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_b64_decodew.src
"$(OUTPUT_RSRC_PATH)\drm_b64_encodew.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_b64_encodew.src
"$(OUTPUT_RSRC_PATH)\drm_mac_inv32.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mac_inv32.src
"$(OUTPUT_RSRC_PATH)\_parselicenseattributes.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_parselicenseattributes.src
"$(OUTPUT_RSRC_PATH)\_loadlicenseattributes.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_loadlicenseattributes.src
"$(OUTPUT_RSRC_PATH)\_performactions.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_performactions.src
"$(OUTPUT_RSRC_PATH)\_getlicenseinfoandsetup.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_getlicenseinfoandsetup.src
"$(OUTPUT_RSRC_PATH)\_processendofchain.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_processendofchain.src
"$(OUTPUT_RSRC_PATH)\drm_lic_completelicensechain.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lic_completelicensechain.src
"$(OUTPUT_RSRC_PATH)\drm_lic_reportactions.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lic_reportactions.src
"$(OUTPUT_RSRC_PATH)\drm_lic_checkclockrollback.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lic_checkclockrollback.src
"$(OUTPUT_RSRC_PATH)\drm_cphr_init.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_cphr_init.src
"$(OUTPUT_RSRC_PATH)\drmcrt_iswxdigit.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drmcrt_iswxdigit.src
"$(OUTPUT_RSRC_PATH)\drmcrt_towlower.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drmcrt_towlower.src
"$(OUTPUT_RSRC_PATH)\drmcrt_wcsntol.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drmcrt_wcsntol.src
"$(OUTPUT_RSRC_PATH)\drmcrt_wcslen.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drmcrt_wcslen.src
"$(OUTPUT_RSRC_PATH)\drmcrt_wcsncpy.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drmcrt_wcsncpy.src
"$(OUTPUT_RSRC_PATH)\drmcrt_memmove.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drmcrt_memmove.src
"$(OUTPUT_RSRC_PATH)\performoperation_part1.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\performoperation_part1.src
"$(OUTPUT_RSRC_PATH)\performoperation_part2.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\performoperation_part2.src
"$(OUTPUT_RSRC_PATH)\performoperation_part3.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\performoperation_part3.src
"$(OUTPUT_RSRC_PATH)\performoperation_part4.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\performoperation_part4.src
"$(OUTPUT_RSRC_PATH)\performoperation_part5.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\performoperation_part5.src
"$(OUTPUT_RSRC_PATH)\performoperation_part6.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\performoperation_part6.src
"$(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression.src
"$(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression_no_more_tokens.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_expr_evaluateexpression_no_more_tokens.src
"$(OUTPUT_RSRC_PATH)\isvalidfunction.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\isvalidfunction.src
"$(OUTPUT_RSRC_PATH)\functiongetvalue.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\functiongetvalue.src
"$(OUTPUT_RSRC_PATH)\globalsetvariable.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\globalsetvariable.src
"$(OUTPUT_RSRC_PATH)\variabledrmkgetorset.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variabledrmkgetorset.src
"$(OUTPUT_RSRC_PATH)\variabledrmgetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variabledrmgetorset.src
"$(OUTPUT_RSRC_PATH)\variablemachinegetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variablemachinegetorset.src
"$(OUTPUT_RSRC_PATH)\variableappgetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variableappgetorset.src
"$(OUTPUT_RSRC_PATH)\variablesecstategetorset.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variablesecstategetorset.src
"$(OUTPUT_RSRC_PATH)\variablelicensegetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variablelicensegetorset.src
"$(OUTPUT_RSRC_PATH)\variablecontentgetorset.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variablecontentgetorset.src
"$(OUTPUT_RSRC_PATH)\variabledevicegetorset.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variabledevicegetorset.src
"$(OUTPUT_RSRC_PATH)\variablepmlicensegetorset.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variablepmlicensegetorset.src
"$(OUTPUT_RSRC_PATH)\_basicheaderchecks.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_basicheaderchecks.src
"$(OUTPUT_RSRC_PATH)\drm_hdr_getattribute.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hdr_getattribute.src
"$(OUTPUT_RSRC_PATH)\drm_hds_createstore.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_createstore.src
"$(OUTPUT_RSRC_PATH)\drm_hds_init.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_init.src
"$(OUTPUT_RSRC_PATH)\drm_hds_uninit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_uninit.src
"$(OUTPUT_RSRC_PATH)\drm_hds_openstore.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_openstore.src
"$(OUTPUT_RSRC_PATH)\drm_hds_closestore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_closestore.src
"$(OUTPUT_RSRC_PATH)\drm_hds_deleteslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_deleteslot.src
"$(OUTPUT_RSRC_PATH)\drm_hds_slotseek.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_slotseek.src
"$(OUTPUT_RSRC_PATH)\drm_hds_slotwrite.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_slotwrite.src
"$(OUTPUT_RSRC_PATH)\drm_hds_slotresize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_slotresize.src
"$(OUTPUT_RSRC_PATH)\drm_hds_initslotenum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_initslotenum.src
"$(OUTPUT_RSRC_PATH)\drm_hds_slotenumnext.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_slotenumnext.src
"$(OUTPUT_RSRC_PATH)\drm_hds_cleanupstore.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_cleanupstore.src
"$(OUTPUT_RSRC_PATH)\_hdscopyslot_child2child.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdscopyslot_child2child.src
"$(OUTPUT_RSRC_PATH)\_hdscopychildpayload.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdscopychildpayload.src
"$(OUTPUT_RSRC_PATH)\_hdsdefragmentfile.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsdefragmentfile.src
"$(OUTPUT_RSRC_PATH)\_hdscleanupnamespace.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdscleanupnamespace.src
"$(OUTPUT_RSRC_PATH)\_hdscleanupstore.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdscleanupstore.src
"$(OUTPUT_RSRC_PATH)\drm_lst_clean.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lst_clean.src
"$(OUTPUT_RSRC_PATH)\_hdslocatefreeblockforslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdslocatefreeblockforslot.src
"$(OUTPUT_RSRC_PATH)\_hdslockblock2deleteslot.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdslockblock2deleteslot.src
"$(OUTPUT_RSRC_PATH)\_hdsunlockblock2deleteslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsunlockblock2deleteslot.src
"$(OUTPUT_RSRC_PATH)\_hdscreatenamespace.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdscreatenamespace.src
"$(OUTPUT_RSRC_PATH)\_hdsfilepos2blocknum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsfilepos2blocknum.src
"$(OUTPUT_RSRC_PATH)\_writesrn.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_writesrn.src
"$(OUTPUT_RSRC_PATH)\_writecommonblockheader.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_writecommonblockheader.src
"$(OUTPUT_RSRC_PATH)\_writechildblockheader.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_writechildblockheader.src
"$(OUTPUT_RSRC_PATH)\_readdatablockheader.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_readdatablockheader.src
"$(OUTPUT_RSRC_PATH)\_writedatablockheader.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_writedatablockheader.src
"$(OUTPUT_RSRC_PATH)\_hdsexpandstore.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsexpandstore.src
"$(OUTPUT_RSRC_PATH)\_hdsallocblock.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsallocblock.src
"$(OUTPUT_RSRC_PATH)\_hdsfreeblock.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsfreeblock.src
"$(OUTPUT_RSRC_PATH)\_hdscreateoversizedslot.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdscreateoversizedslot.src
"$(OUTPUT_RSRC_PATH)\_hdsallocslotinfile.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsallocslotinfile.src
"$(OUTPUT_RSRC_PATH)\_hdswriteblockhdr.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdswriteblockhdr.src
"$(OUTPUT_RSRC_PATH)\_hdsadjustchildpayload.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsadjustchildpayload.src
"$(OUTPUT_RSRC_PATH)\_hdsfindleftmostleafblock.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsfindleftmostleafblock.src
"$(OUTPUT_RSRC_PATH)\_hdscreateandopenslot.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdscreateandopenslot.src
"$(OUTPUT_RSRC_PATH)\_hdsremoveslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsremoveslot.src
"$(OUTPUT_RSRC_PATH)\_hdscopyandlocknewslot.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdscopyandlocknewslot.src
"$(OUTPUT_RSRC_PATH)\_hdsrelocateslotandkeepcurrlock.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsrelocateslotandkeepcurrlock.src
"$(OUTPUT_RSRC_PATH)\_hdstraversenextrightsiblingblock.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdstraversenextrightsiblingblock.src
"$(OUTPUT_RSRC_PATH)\_hdstraverseblocksinpostorder.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdstraverseblocksinpostorder.src
"$(OUTPUT_RSRC_PATH)\_hdsslotresize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsslotresize.src
"$(OUTPUT_RSRC_PATH)\_isnull.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_isnull.src
"$(OUTPUT_RSRC_PATH)\_hdsgensrnhash.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsgensrnhash.src
"$(OUTPUT_RSRC_PATH)\_hdsallocblockbuffer.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsallocblockbuffer.src
"$(OUTPUT_RSRC_PATH)\_readsrn.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_readsrn.src
"$(OUTPUT_RSRC_PATH)\_hdsgetputchildblocknum.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsgetputchildblocknum.src
"$(OUTPUT_RSRC_PATH)\_hdshashkeytoindex.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdshashkeytoindex.src
"$(OUTPUT_RSRC_PATH)\_hdsloadsrn.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsloadsrn.src
"$(OUTPUT_RSRC_PATH)\_hdsslotwrite.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsslotwrite.src
"$(OUTPUT_RSRC_PATH)\_hdsinitslotenum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsinitslotenum.src
"$(OUTPUT_RSRC_PATH)\_hdsslotenumnext.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsslotenumnext.src
"$(OUTPUT_RSRC_PATH)\drm_levl_evaluateexpression.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_levl_evaluateexpression.src
"$(OUTPUT_RSRC_PATH)\drm_levl_performoperations.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_levl_performoperations.src
"$(OUTPUT_RSRC_PATH)\drm_lic_getattribute.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lic_getattribute.src
"$(OUTPUT_RSRC_PATH)\drm_lic_getenablingbits.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lic_getenablingbits.src
"$(OUTPUT_RSRC_PATH)\getlspubkey.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\getlspubkey.src
"$(OUTPUT_RSRC_PATH)\drm_lic_verifysignature.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lic_verifysignature.src
"$(OUTPUT_RSRC_PATH)\_loadproritizedlist.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_loadproritizedlist.src
"$(OUTPUT_RSRC_PATH)\drm_lst_open.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lst_open.src
"$(OUTPUT_RSRC_PATH)\drm_lst_close.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lst_close.src
"$(OUTPUT_RSRC_PATH)\drm_lst_getlicense.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lst_getlicense.src
"$(OUTPUT_RSRC_PATH)\drm_lst_initenum.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lst_initenum.src
"$(OUTPUT_RSRC_PATH)\drm_lst_enumnext.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lst_enumnext.src
"$(OUTPUT_RSRC_PATH)\drm_lst_enumdelete.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_lst_enumdelete.src
"$(OUTPUT_RSRC_PATH)\_processextensions.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_processextensions.src
"$(OUTPUT_RSRC_PATH)\_processidlist.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_processidlist.src
"$(OUTPUT_RSRC_PATH)\_processexclusions.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_processexclusions.src
"$(OUTPUT_RSRC_PATH)\_processinclusions.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_processinclusions.src
"$(OUTPUT_RSRC_PATH)\drm_opl_processcopyoutputleveldata.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_opl_processcopyoutputleveldata.src
"$(OUTPUT_RSRC_PATH)\_getopllevel.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_getopllevel.src
"$(OUTPUT_RSRC_PATH)\drm_opl_processplayoutputleveldata.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_opl_processplayoutputleveldata.src
"$(OUTPUT_RSRC_PATH)\_loadattributesintocache.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_loadattributesintocache.src
"$(OUTPUT_RSRC_PATH)\_openandverifyslot.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_openandverifyslot.src
"$(OUTPUT_RSRC_PATH)\drm_sst_openkeytokens.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_openkeytokens.src
"$(OUTPUT_RSRC_PATH)\_updateattributetokeninslot.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_updateattributetokeninslot.src
"$(OUTPUT_RSRC_PATH)\drm_sst_closekey.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_closekey.src
"$(OUTPUT_RSRC_PATH)\drm_sst_gettokenvalue.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_gettokenvalue.src
"$(OUTPUT_RSRC_PATH)\drm_sst_settokenvalue.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_settokenvalue.src
"$(OUTPUT_RSRC_PATH)\drm_sst_createlicensestatepassword.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_createlicensestatepassword.src
"$(OUTPUT_RSRC_PATH)\drm_sst_createglobalstorepassword.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_createglobalstorepassword.src
"$(OUTPUT_RSRC_PATH)\drm_utl_promoteansitounicode.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_promoteansitounicode.src
"$(OUTPUT_RSRC_PATH)\drm_pk_symmetricverify.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_pk_symmetricverify.src
"$(OUTPUT_RSRC_PATH)\drm_pk_symmetriccrypt.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_pk_symmetriccrypt.src
"$(OUTPUT_RSRC_PATH)\drm_pk_symmetricsign.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_pk_symmetricsign.src
"$(OUTPUT_RSRC_PATH)\drm_utl_getversionfromstring.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_getversionfromstring.src
"$(OUTPUT_RSRC_PATH)\drm_utl_stringinsertblanksubstring.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_stringinsertblanksubstring.src
"$(OUTPUT_RSRC_PATH)\drm_utl_stringremovesubstring.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_stringremovesubstring.src
"$(OUTPUT_RSRC_PATH)\drm_utl_dstrsearch.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_dstrsearch.src
"$(OUTPUT_RSRC_PATH)\drm_utl_dstrsearchreverse.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_dstrsearchreverse.src
"$(OUTPUT_RSRC_PATH)\drm_utl_numbertostring.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_numbertostring.src
"$(OUTPUT_RSRC_PATH)\drm_utl_getversionasstring.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_getversionasstring.src
"$(OUTPUT_RSRC_PATH)\drm_utl_stringtoguid.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_stringtoguid.src
"$(OUTPUT_RSRC_PATH)\drm_utl_decodekid.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_decodekid.src
"$(OUTPUT_RSRC_PATH)\drm_utl_ensuredataalignment.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_utl_ensuredataalignment.src
"$(OUTPUT_RSRC_PATH)\overlappingdates.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\overlappingdates.src
"$(OUTPUT_RSRC_PATH)\drm_asd_parsev2license.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_asd_parsev2license.src
"$(OUTPUT_RSRC_PATH)\neginfdate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\neginfdate.src
"$(OUTPUT_RSRC_PATH)\infdate.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\infdate.src
"$(OUTPUT_RSRC_PATH)\isexpired.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\isexpired.src
"$(OUTPUT_RSRC_PATH)\getsecstateattr.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\getsecstateattr.src
"$(OUTPUT_RSRC_PATH)\setexpirycategory.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\setexpirycategory.src
"$(OUTPUT_RSRC_PATH)\getv2licenseinfo.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\getv2licenseinfo.src
"$(OUTPUT_RSRC_PATH)\getnextlicense.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\getnextlicense.src
"$(OUTPUT_RSRC_PATH)\aggregate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\aggregate.src
"$(OUTPUT_RSRC_PATH)\drm_asd_getlicenseaggregatedata.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_asd_getlicenseaggregatedata.src
"$(OUTPUT_RSRC_PATH)\_scannodeforattributew.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_scannodeforattributew.src
"$(OUTPUT_RSRC_PATH)\_getxmlnodecdataw.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_getxmlnodecdataw.src
"$(OUTPUT_RSRC_PATH)\drm_xml_getnodeattribute.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_xml_getnodeattribute.src
"$(OUTPUT_RSRC_PATH)\drm_xml_getnodecdata.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_xml_getnodecdata.src
"$(OUTPUT_RSRC_PATH)\drm_dcp_getattribute.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_dcp_getattribute.src
"$(OUTPUT_RSRC_PATH)\drm_dcp_loadpropertiescache.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_dcp_loadpropertiescache.src
"$(OUTPUT_RSRC_PATH)\_verifysymmerticsignature.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_verifysymmerticsignature.src
"$(OUTPUT_RSRC_PATH)\drm_ddc_getdevicecertificate.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_ddc_getdevicecertificate.src
"$(OUTPUT_RSRC_PATH)\_createdevicestore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_createdevicestore.src
"$(OUTPUT_RSRC_PATH)\_freedrmmanagerinternalcontexts.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_freedrmmanagerinternalcontexts.src
"$(OUTPUT_RSRC_PATH)\_setuplicevalobjecttoshare.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_setuplicevalobjecttoshare.src
"$(OUTPUT_RSRC_PATH)\_mapdrmerror.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_mapdrmerror.src
"$(OUTPUT_RSRC_PATH)\_comparemachineid.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_comparemachineid.src
"$(OUTPUT_RSRC_PATH)\initmgrcontext.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\initmgrcontext.src
"$(OUTPUT_RSRC_PATH)\drm_mgr_initialize.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mgr_initialize.src
"$(OUTPUT_RSRC_PATH)\drm_mgr_uninitialize.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mgr_uninitialize.src
"$(OUTPUT_RSRC_PATH)\drm_mgr_setv2header.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mgr_setv2header.src
"$(OUTPUT_RSRC_PATH)\drm_mgr_bind.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mgr_bind.src
"$(OUTPUT_RSRC_PATH)\drm_mgr_commit.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mgr_commit.src
"$(OUTPUT_RSRC_PATH)\drm_mgr_cleanuplicensestore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mgr_cleanuplicensestore.src
"$(OUTPUT_RSRC_PATH)\januscleandatastore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\januscleandatastore.src
"$(OUTPUT_RSRC_PATH)\drm_mtr_openid.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mtr_openid.src
"$(OUTPUT_RSRC_PATH)\drm_mtr_incrementcount.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mtr_incrementcount.src
"$(OUTPUT_RSRC_PATH)\drm_mtr_closecontext.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mtr_closecontext.src
"$(OUTPUT_RSRC_PATH)\oem_openfile.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_openfile.src
"$(OUTPUT_RSRC_PATH)\oem_setendoffile.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_setendoffile.src
"$(OUTPUT_RSRC_PATH)\oem_writefile.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_writefile.src
"$(OUTPUT_RSRC_PATH)\oem_genrandombytes.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_genrandombytes.src
"$(OUTPUT_RSRC_PATH)\oem_getfallbacksigningkey.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_getfallbacksigningkey.src
"$(OUTPUT_RSRC_PATH)\oem_getdevicecerttemplate.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_getdevicecerttemplate.src
"$(OUTPUT_RSRC_PATH)\oem_setdevicecert.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_setdevicecert.src
"$(OUTPUT_RSRC_PATH)\oem_getclockresetstate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_getclockresetstate.src
"$(OUTPUT_RSRC_PATH)\oem_setclockresetstate.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_setclockresetstate.src
"$(OUTPUT_RSRC_PATH)\_getdrmfullpathname.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_getdrmfullpathname.src
"$(OUTPUT_RSRC_PATH)\oem_getuniqueid.src"                   : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_getuniqueid.src
"$(OUTPUT_RSRC_PATH)\_getprivatekey.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_getprivatekey.src
"$(OUTPUT_RSRC_PATH)\oem_getgroupcertprivatekey.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_getgroupcertprivatekey.src
"$(OUTPUT_RSRC_PATH)\_getdevicecert.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_getdevicecert.src
"$(OUTPUT_RSRC_PATH)\oem_getdevicecert.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_getdevicecert.src
"$(OUTPUT_RSRC_PATH)\oem_getsecurestoreglobalpasswordseed.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_getsecurestoreglobalpasswordseed.src
"$(OUTPUT_RSRC_PATH)\drm_snc_openstore.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_snc_openstore.src
"$(OUTPUT_RSRC_PATH)\drm_snc_closestore.src"                    : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_snc_closestore.src
"$(OUTPUT_RSRC_PATH)\_setkidstoredata.src"                  : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_setkidstoredata.src
"$(OUTPUT_RSRC_PATH)\drm_snc_deletekid.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_snc_deletekid.src
"$(OUTPUT_RSRC_PATH)\drm_snc_updatekid.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_snc_updatekid.src
"$(OUTPUT_RSRC_PATH)\drm_mgr_setv1header.src"                 : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mgr_setv1header.src
"$(OUTPUT_RSRC_PATH)\functiongetvalue_part1.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\functiongetvalue_part1.src
"$(OUTPUT_RSRC_PATH)\functiongetvalue_fn_datediff.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\functiongetvalue_fn_datediff.src
"$(OUTPUT_RSRC_PATH)\eval.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\eval.src
"$(OUTPUT_RSRC_PATH)\drm_hds_opennamespace.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_opennamespace.src
"$(OUTPUT_RSRC_PATH)\drm_hds_openslot.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_openslot.src
"$(OUTPUT_RSRC_PATH)\_applydiffstostore.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_applydiffstostore.src
"$(OUTPUT_RSRC_PATH)\_verifyslothash.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_verifyslothash.src
"$(OUTPUT_RSRC_PATH)\_hdscreatenamespacestore.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdscreatenamespacestore.src
"$(OUTPUT_RSRC_PATH)\drm_hds_createstore2.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_createstore2.src
"$(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src
"$(OUTPUT_RSRC_PATH)\_hdsprealloc.src"                         : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsprealloc.src
"$(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_hds_openstore2.src
"$(OUTPUT_RSRC_PATH)\oem_systemtimetofiletime.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_systemtimetofiletime.src
"$(OUTPUT_RSRC_PATH)\oem_filetimetosystemtime.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_filetimetosystemtime.src
"$(OUTPUT_RSRC_PATH)\gendevicecertificate.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\gendevicecertificate.src
"$(OUTPUT_RSRC_PATH)\drmcrt_memmove_mem.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drmcrt_memmove_mem.src
"$(OUTPUT_RSRC_PATH)\copyhdsdtore.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\copyhdsdtore.src
"$(OUTPUT_RSRC_PATH)\getprivkey.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\getprivkey.src
"$(OUTPUT_RSRC_PATH)\generatedevicecert.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\generatedevicecert.src
"$(OUTPUT_RSRC_PATH)\drm_b64_decodew_inplace.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_b64_decodew_inplace.src
"$(OUTPUT_RSRC_PATH)\oem_setsystemtime.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_setsystemtime.src
"$(OUTPUT_RSRC_PATH)\drm_sst_closelockedslot.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_closelockedslot.src
"$(OUTPUT_RSRC_PATH)\oem_getdevicedatetime.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\oem_getdevicedatetime.src
"$(OUTPUT_RSRC_PATH)\drm_sst_setdata.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_setdata.src
"$(OUTPUT_RSRC_PATH)\_systemtime_to_struct_tm.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_systemtime_to_struct_tm.src
"$(OUTPUT_RSRC_PATH)\_struct_tm_to_systemtime.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_struct_tm_to_systemtime.src
"$(OUTPUT_RSRC_PATH)\_checksecureclock.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_checksecureclock.src
"$(OUTPUT_RSRC_PATH)\drm_sst_getdata.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_getdata.src
"$(OUTPUT_RSRC_PATH)\localtime.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\localtime.src
"$(OUTPUT_RSRC_PATH)\drm_sst_getlockeddata.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_getlockeddata.src
"$(OUTPUT_RSRC_PATH)\variablesecuretimegetorset.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\variablesecuretimegetorset.src
"$(OUTPUT_RSRC_PATH)\_systemtimetotime_t.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_systemtimetotime_t.src
"$(OUTPUT_RSRC_PATH)\drm_mtr_updatedata.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_mtr_updatedata.src
"$(OUTPUT_RSRC_PATH)\gmtime.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\gmtime.src
"$(OUTPUT_RSRC_PATH)\_hdsupdatesrn.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_hdsupdatesrn.src
"$(OUTPUT_RSRC_PATH)\drm_sst_openandlockslot.src"   :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_openandlockslot.src
"$(OUTPUT_RSRC_PATH)\_preparesecureclockdataforwriting.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_preparesecureclockdataforwriting.src
"$(OUTPUT_RSRC_PATH)\strtol.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\strtol.src
"$(OUTPUT_RSRC_PATH)\_preparesecureclockdataforreading.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_preparesecureclockdataforreading.src
"$(OUTPUT_RSRC_PATH)\drm_sst_setlockeddata.src" :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_sst_setlockeddata.src
"$(OUTPUT_RSRC_PATH)\drm_clk_initchecksecureclock.src"  :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\drm_clk_initchecksecureclock.src
"$(OUTPUT_RSRC_PATH)\mktime.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\mktime.src
"$(OUTPUT_RSRC_PATH)\getbase64decodedkey.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\getbase64decodedkey.src
"$(OUTPUT_RSRC_PATH)\_initslot.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_initslot.src
"$(OUTPUT_RSRC_PATH)\_loadslot.src"    :   $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src  $(OUTPUT_RSRC_PATH)\_loadslot.src

endif

$(OUTPUT_RSRC_PATH)\nanddatadriveinit.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mediastartup.loc
$(OUTPUT_RSRC_PATH)\mediastartup.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mediastartup.loc
        @echo Extract $@
        $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)

#ifdef D_D3410
#$(OUTPUT_RSRC_PATH)\chkdskstartup.src : $(OUTPUT_PATH)\$(PROJ).abs
#     copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\chkdskstartup.src
#endif


ifdef D_MMC
$(OUTPUT_RSRC_PATH)\mmcdatadriveinfo.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcmediainit.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcdatadriveerase.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcread.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcreadwrite.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcmediastartup.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcerase.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcinfo.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcenumerate.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcresetdevice.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcprocesscsd.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcprocesscid.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcprocesscid2.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcdetectpresence.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmcserialnumberinit.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
$(OUTPUT_RSRC_PATH)\mmccheckwriteprotect.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map $(LOC_PATH)\mmcmediastartup.loc
        @echo Extract $@
        $(BIN_PATH)\$(EXTRACT_EXE) $(match .abs $!) -L$(match .loc $!) -F$(match .map $!) -P$(OUTPUT_RSRC_PATH)
else
#### create dummy resources so there are no changes to resource.inc

$(OUTPUT_RSRC_PATH)\funclet_mmctesterase.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
    @echo Extract $@
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\funclet_mmctesterase.src

$(OUTPUT_RSRC_PATH)\funclet_mmchalinit.src : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
    @echo Extract $@
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\funclet_mmchalinit.src

$(OUTPUT_RSRC_PATH)\mmcmediastartup.src : $(OUTPUT_PATH)\$(PROJ).abs
     copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcmediastartup.src

$(OUTPUT_RSRC_PATH)\mmcerase.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcerase.src

$(OUTPUT_RSRC_PATH)\mmcinfo.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcinfo.src

$(OUTPUT_RSRC_PATH)\mmcmediainit.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcmediainit.src

$(OUTPUT_RSRC_PATH)\mmcread.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcread.src

$(OUTPUT_RSRC_PATH)\mmcdatadriveerase.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcdatadriveerase.src

$(OUTPUT_RSRC_PATH)\mmcdatadriveinfo.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcdatadriveinfo.src

$(OUTPUT_RSRC_PATH)\mmcreadwrite.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcreadwrite.src

$(OUTPUT_RSRC_PATH)\mmcenumerate.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcenumerate.src

$(OUTPUT_RSRC_PATH)\mmcresetdevice.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcresetdevice.src

$(OUTPUT_RSRC_PATH)\mmcprocesscsd.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcprocesscsd.src

$(OUTPUT_RSRC_PATH)\mmcprocesscid.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcprocesscid.src

$(OUTPUT_RSRC_PATH)\mmcprocesscid2.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcprocesscid2.src

$(OUTPUT_RSRC_PATH)\mmcdetectpresence.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcdetectpresence.src

$(OUTPUT_RSRC_PATH)\mmcserialnumberinit.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmcserialnumberinit.src

$(OUTPUT_RSRC_PATH)\mmccheckwriteprotect.src : $(OUTPUT_PATH)\$(PROJ).abs
    copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\mmccheckwriteprotect.src
endif      #ifdef MMC

endif #

ifdef D_REMOVE_FM
######################################################
# Create dummy resource for FM funclets
######################################################
"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverInit.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverInit.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncUp.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncUp.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncDown.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncDown.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverStandyby.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverStandyby.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverReplacePreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverReplacePreset.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverErasePreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverErasePreset.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverTuneToPreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverTuneToPreset.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSearchDown.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSearchDown.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000SafePowerDown.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000SafePowerDown.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000OptimizeChannel.src" : $(OUTPUT_PATH)\$(PROJ).abs
         copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000OptimizeChannel.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000I2CInit.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000I2CInit.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverScanStations.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverScanStations.src

"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000SetLnaBias.src"          : $(OUTPUT_PATH)\$(PROJ).abs
         copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_STFM1000SetLnaBias.src

"$(OUTPUT_RSRC_PATH)\decstfmmod.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\decstfmmod.src

"$(OUTPUT_RSRC_PATH)\dec_stfmp.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\dec_stfmp.src

"$(OUTPUT_RSRC_PATH)\dec_stfmx.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\dec_stfmx.src

"$(OUTPUT_RSRC_PATH)\dec_stfmy.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\dec_stfmy.src

"$(OUTPUT_RSRC_PATH)\TunerModule.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\TunerModule.src

"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000LookForPilot.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_STFM1000LookForPilot.src

"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000TunerSearchEnded.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_STFM1000TunerSearchEnded.src

"$(OUTPUT_RSRC_PATH)\fmtunersecondrsrc.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\fmtunersecondrsrc.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSetMonoStereoMode.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSetMonoStereoMode.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp1.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp1.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp2.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000RegPowerUp2.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000FindRssiThreshold.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000FindRssiThreshold.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000DataPathOff.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000DataPathOff.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000ContinueSearching.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000ContinueSearching.src

endif # D_REMOVE_FM

ifdef D_FM_PHILIPS
######################################################
# Create dummy resource for FM funclets
######################################################

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncUp.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncUp.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncDown.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverIncDown.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverStandyby.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverStandyby.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverReplacePreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverReplacePreset.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverErasePreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverErasePreset.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverTuneToPreset.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverTuneToPreset.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSearchDown.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSearchDown.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000SafePowerDown.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000SafePowerDown.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000OptimizeChannel.src" : $(OUTPUT_PATH)\$(PROJ).abs
         copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000OptimizeChannel.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000I2CInit.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerSTFM1000I2CInit.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverScanStations.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverScanStations.src

"$(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSetMonoStereoMode.src" : $(OUTPUT_PATH)\$(PROJ).abs
        copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_TunerDriverSetMonoStereoMode.src

"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000SetLnaBias.src"          : $(OUTPUT_PATH)\$(PROJ).abs
         copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_STFM1000SetLnaBias.src

"$(OUTPUT_RSRC_PATH)\decstfmmod.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\decstfmmod.src

"$(OUTPUT_RSRC_PATH)\dec_stfmp.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\dec_stfmp.src

"$(OUTPUT_RSRC_PATH)\dec_stfmx.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\dec_stfmx.src

"$(OUTPUT_RSRC_PATH)\dec_stfmy.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\dec_stfmy.src

"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000LookForPilot.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_STFM1000LookForPilot.src

"$(OUTPUT_RSRC_PATH)\Funclet_STFM1000TunerSearchEnded.src" : $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).abs $(OUTPUT_PATH)\$(PROJ).map
   copy $(PROJRSRC_PATH)\null.src $(OUTPUT_RSRC_PATH)\Funclet_STFM1000TunerSearchEnded.src

endif # D_FM_PHILIPS

