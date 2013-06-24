ifndef STANDARD_MK
STANDARD_MK = 1
#This is the standard Makefile for STMP SDK Projects

 
#Set the version, if not already set
ifndef VERSION
VERSION=999.999.999
endif

#Enable FAT16 so we get 128MB Nand/SmartMedia support
ifndef D_FAT16
D_FAT16=TRUE
BLD=$(BLD) -DFAT16
endif

#Force a DEBUG build if the BUILDTYPE parameter not specified in player/mtp/usbmsc.bat or ".mk 
ifdef D_BETA
else
ifdef D_RETAIL
# Define RETAIL when requested from app bat file, passing RETAIL def directly to the assembler and compiler. 
# As-is: verified to work in MTP & player. If you change it, verify both apps get the define 
# in c and asm files. sysmacro.asm, syserror.asm, icoll_ResetVectorTable.asm
CC_OPTS=$(CC_OPTS) -Wa-DRETAIL
CC_OPTS=$(CC_OPTS) -Wc-DRETAIL
#AS_OPTS=$(AS_OPTS) -DRETAIL
# Verified as required for SystemHalt macro = error asm macro- asm embedded within C in player.
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DRETAIL
else
ifndef D_DEBUG
D_DEBUG=TRUE
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DDEBUG
BLD=$(BLD) -DDEBUG
endif
CC_OPTS=$(CC_OPTS) -Wc-DDEBUG
endif
endif
D_D3500=TRUE
BLD=$(BLD) -DDEVICE_3500 
CC_OPTS=$(CC_OPTS) -Wc-DDEVICE_3500
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DDEVICE_3500
DEVICE=3500
D_DEVICE_SET=TRUE
############################



# Signify SDK2400 with defines here, if not specified as a player.bat parameter.
# Not suited for mass storage class build
ifdef STMP_BUILD_PLAYER
ifndef D_SDK2400
D_SDK2400=TRUE
BLD=$(BLD) -DSDK2400
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSDK2400
endif
endif
############################


#The code below is removed to prevent a "MMC already defined" error.
#If either of these items are defined, then there is an external device
ifdef D_SMEDIA
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DSMEDIA
CC_OPTS=$(CC_OPTS) -Wa-DSMEDIA -Wc-DSMEDIA
else
ifdef D_MMC
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DMMC
CC_OPTS=$(CC_OPTS) -Wa-DMMC -Wc-DMMC
endif 
endif

#Uncomment the following line to use standard SigmaTel engineering boards
BLD=$(BLD) -DENGR_BD
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DENGR_BD

ifdef D_DRM
else
ifndef D_WMAAPI_NO_DRM
BLD=$(BLD) -DWMAAPI_NO_DRM
C_ASM_OPTS=$(C_ASM_OPTS) -Wa-DWMAAPI_NO_DRM
endif
endif


#Set up the standard paths
BIN_PATH	    =$(ROOT)\bin
CPU_PATH        =$(ROOT)\CPUfiles
INC_PATH        =$(ROOT)\inc
ETC_PATH        =$(ROOT)\etc
LOC_PATH        =$(ROOT)\etc
#LIB_PATH        =$(ROOT)\lib

# MSC/MTP and Player have 2 different library sets. And retail and debug lib sets are the root (4 lib sets total)
# Some tests needed \lib\...player but were linking \lib\...msc since this def was not propogating to here until this comment was inserted.
# Link from retail lib if RETAIL param at app build cmd line. (causes D_DEBUG to no longer be defined in this mk file)
ifdef STMP_BUILD_PLAYER
ifdef D_DEBUG
LIB_PATH        =$(ROOT)\lib\debug\player
else
LIB_PATH        =$(ROOT)\lib\retail\player
endif
else
# non player:  MSC (also for MTP)  ------------
ifdef D_DEBUG
LIB_PATH        =$(ROOT)\lib\debug\msc
else
LIB_PATH        =$(ROOT)\lib\retail\msc
endif
endif

STANDARD_LIB_PATH = $(ROOT)\lib

XREF_PATH       =$(ROOT)\xrefs
DCC_PATH        =$(ROOT)\Dcc
DD_PATH         =$(ROOT)\DeviceDriver
DCC_COMMON_PATH =$(DCC_PATH)\Common
DCCDEVUSB_PATH  =$(DCC_PATH)\Device\USB
DCCDEVSM_PATH   =$(DCC_PATH)\Device\SmartMedia
DCCDEVMMC_PATH  =$(DCC_PATH)\Device\MMC
SYS_PATH        =$(ROOT)\System
SYS_COMMON_PATH =$(ROOT)\System\Common
SYS_DEVICE_PATH =$(ROOT)\System\$(DEVICE_FAMILY)
SYS_EXEC_PATH   =$(ROOT)\System\Exec
DD_SM_SYS       =$(ROOT)\DeviceDriver\Media\SmartMedia\System
DDI_SM_PATH     =$(ROOT)\DeviceDriver\Media\SmartMedia
DDI_SPI_PATH    =$(ROOT)\DeviceDriver\Media\SPI
DDI_MMC_PATH    =$(ROOT)\DeviceDriver\Media\MMC
RESOURCE_PATH   =$(ROOT)\resources
RSRC_MISC_PATH  =$(ROOT)\resources\misc
BITMAP_PATH     =$(ROOT)\Bitmaps
SW_MSGMODULES_PATH=$(ROOT)\System\MsgModules\Software
HW_MSGMODULES_PATH=$(ROOT)\System\MsgModules\Hardware
DD_UI_PATH      =$(ROOT)\DeviceDriver\UserInput
ADC_PATH        =$(ROOT)\DeviceDriver\Analog\adc
DEC_PATH        =$(SW_MSGMODULES_PATH)\decoder
ENC_PATH        =$(SW_MSGMODULES_PATH)\encoder
MP3_PATH        =$(DEC_PATH)\mp3
AUDIBLE_MP3_PATH=$(DEC_PATH)\audiblemp3
WMA_PATH        =$(DEC_PATH)\wma
ADPCM_DEC_PATH  =$(DEC_PATH)\ImaAdpcm
SMVADPCM_DEC_PATH =$(DEC_PATH)\SmvAdpcm
ADPCM_ENC_PATH  =$(ENC_PATH)\ImaAdpcm
ADPCM_ALGO_DEC_PATH = $(ROOT)\Algorithms\Decoders
ADPCM_ALGO_ENC_PATH = $(ROOT)\Algorithms\Encoders
ALGO_COMMON_PATH = $(ROOT)\algorithms\common
STFM_DEC_PATH   =$(DEC_PATH)\Stfm
STFM_ALGO_DEC_PATH = $(ROOT)\Algorithms\Decoders

ifdef JPEG_DECODER
JPEG_ALG_PATH	=$(ROOT)\Algorithms\Decoders\Jpeg
JPEG_LIB_PATH 	=$(JPEG_ALG_PATH)\lib
endif
ifdef SMVJPEG_DECODER
SMVJPEG_ALG_PATH	=$(ROOT)\Algorithms\Decoders\SmvJpeg
SMVJPEG_LIB_PATH 	=$(SMVJPEG_ALG_PATH)\lib
endif

MP3_ALG_PATH    =$(ROOT)\Algorithms\Decoders\Mp3
AUDIBLE_MP3_ALG_PATH    =$(ROOT)\Algorithms\Decoders\AudibleMp3
MP3_LIB_PATH    =$(MP3_ALG_PATH)\lib
WMA_ALG_PATH    =$(ROOT)\Algorithms\Decoders\Wma
WMA_LIB_PATH    =$(WMA_ALG_PATH)\lib
JANUS_ALG_PATH  =$(ROOT)\Algorithms\DRM\janus\src
JANUS_LIB_PATH  =$(ROOT)\Algorithms\DRM\janus\lib
JANUS_SYSTEM_PATH  = $(ROOT)\Algorithms\DRM\Janus\System
DRM_PATH          =  $(ROOT)\Algorithms\DRM\Janus
DDILDL_PATH     = $(ROOT)\DeviceDriver\Media\ddildl
DDILDL_INCLUDE_PATH = $(DDILDL_PATH)\include
SYMBOLS_PATH    = $(SYS_COMMON_PATH)\symbols
ACELP_PATH      =$(DEC_PATH)\acelp
ACELP_ALG_PATH  =$(ROOT)\Algorithms\Decoders\Acelp
ACELP_LIB_PATH  =$(ACELP_ALG_PATH)\lib
JANUS_INCLUDE_PATH =  $(JANUS_ALG_PATH)\h


#all files created during build are under OUTPUT_PATH
DEVICE_FAMILY       =3500

OUTPUT_PATH         =..\output_$(DEVICE_FAMILY)
HW_REGS_INC_PATH    =$(INC_PATH)\$(DEVICE_FAMILY)
CHIP_LIB_PATH       =$(LIB_PATH)\$(DEVICE_FAMILY)


OBJ_PATH            =$(OUTPUT_PATH)\obj
LIST_PATH           =$(OUTPUT_PATH)\lst
OUTPUT_RSRC_PATH    =$(OUTPUT_PATH)\rsrc
OUTPUT_INCLUDE_PATH =$(OUTPUT_PATH)\include
# include file used for equates common between dcc and player

COMMON_PROJ_PATH    =..\..

ifndef BIN_DRIVE
BIN_DRIVE=c:
endif

#Set up the base include path
INC_ALL=$(INC_PATH);$(ROOT)\FileSystem\Fat12;$(XREF_PATH);$(SYS_COMMON_PATH);$(SYS_PATH);$(CPU_PATH);$(HW_REGS_INC_PATH);$(SYMBOLS_PATH);$(OUTPUT_INCLUDE_PATH);$(PROJ_PATH);$(JANUS_INCLUDE_PATH)
INC_ALL_C= -Wc-I$(OUTPUT_INCLUDE_PATH) -Wc-I$(PROJ_PATH) -Wc-I$(PROJMENUS_PATH) -Wc-I$(COMMON_PROJ_PATH) -Wc-I$(INC_PATH) -Wc-I$(SYS_COMMON_PATH) -Wc-I$(SYMBOLS_PATH)  $(INC_ALL_C) -Wc-I$(PROJDISP_PATH) -Wc-I$(JANUS_INCLUDE_PATH)


#set the target
TARGET = $(OUTPUT_PATH)\$(PROJ).abs

CC_EXE=cc2410	# control program uses compiler and assembler. 
AS_EXE=as2410
LK_EXE=lk2410
LC_EXE=lc2410
AR_EXE=ar2410
MAKE_EXE=mk2410
EXTRACT_EXE=extractcluster.exe
ENCRYPT_EXE =encrypt.exe
INCTOC_EXE=$(BIN_PATH)\inctoc.exe

# Include the C Compiler Libraries


#The order in which the items below are in ALLLIBLIST is important.  Do not change this



STANDARDLIB = $(STANDARD_LIB_PATH)\librt.a

# We don't really need the floating point stuff for mtp, yet.
ifndef MTP_BUILD
STANDARDLIB =   $(STANDARDLIB)\
                $(STANDARD_LIB_PATH)\libfp.a
endif


ALLLIBLIST = $(GENERALLIB) $(FILESYSLIB) $(DRIVERLIBS) $(SYSTEMLIBS) $(OSLIB) $(STANDARDLIB)
ifdef MS_ADPCM
OTHERBLD= $(OTHERBLD) -DMS_ADPCM
endif

ifdef SMV_ADPCM
OTHERBLD= $(OTHERBLD) -DSMV_ADPCM
endif

ifdef IMA_ADPCM
OTHERBLD= $(OTHERBLD) -DIMA_ADPCM
endif

ifdef WINDOWS_PCM
OTHERBLD= $(OTHERBLD) -DWINDOWS_PCM
endif

# Required COMPILER OPTIONS
# -c 		: do not link, stop at .obj
# -nolib	: do not link with standard libraries
# -L7		: limit hardware stack to 7 levels
# -MmyL		: mixed memory model; default memory space is Y; no stack in L
# -rr6 -rn6 -rm6 : reserve registers (r6,n6,m6) 

# Compiler options must be preceded with -Wc to be ensure they will be passed to the compiler from the control program
# -Wc-O2    : optimize for size; allow debugging
# -Wc-OG 	: !!!!! see *.c : *.obj rule !!!! bug fix from Tasking, should not allow bit manipulation in status register (DSP core errata)
#
# -Wc-g		: generate debug info
# -Wc-R		: forces section names to be generated based on the filename instead of 1 default section
# -Wc-Cs    : forces R7 stack pointer to point to first free location in stack instead of top of stack

# Assembler options must be preceded with -Wa to be ensure they will be passed to the assembler from the control program
# -Wa-l     : create listing
# -Wa-gs    : generate debug info 

#CCFLAGS	= $(CC_OPTS) -c -nolib -L7 -MmyL -rr6 -rn6 -rm6 -Wc-g -Wc-O2 -Wc-R -Wc-Cs -Wa-gs -Wa-l 
CCFLAGS	= $(CC_OPTS) -c -nolib -L7 -MmyL -Wc-g -Wc-O2 -Wc-R -Wc-Cs -Wa-gs -Wa-l  $(OTHERBLD) 

# these flags would be used if using c2410 directly instead of control program
#CFLAGS	= -c -nolib -L7 -MmyL -rr6 -rn6 -rm6 -OacDefGhijLnoprsTUvwxyz -g -R -Cs 
CFLAGS	= -c -nolib -L7 -MmyL -OacDefGhijLnoprsTUvwxyz -g -R -Cs 

# Required ASSEMBLER options
# -gaHl      : assembler source level debug and local symbol debug information
# -RDRS      : no pipeline restrictions are removed
# -ORN       : (R)Do not perform single instruction DO REP optimization; (N) do not remove nops
#
ASFLAGS	= -gaHl -RDRS -OR $(AS_OPTS) $(OTHERBLD)

# Required LOCATOR options
# -fl : IEEE695 (.abs) output format
# -c  : no ROM copy generated for varibale initialization (more room in .abs for overlays)
# -M  : create mapfile
####  don't know why but if the -w8 is left in  player.mak following the same conventions
####  these options are not used...
#  -w8 is warning suppression for all warnings above level 8. 
# -s strips of debug symbols but don't use that here. Instead use mtp.bat/player.bat/usbmsc.bat NO_SYMBOLS param for build speed
LC_OPTS= -f1 -c -M -w8 $(LC_OPTS)

# no c++ files
CPPFLAGS=

RESOURCE_INC_LIST = $(RESOURCE_INC_LIST) $(PROJ_PATH)\resource.inc

RESOURCE_INC =  $(OUTPUT_INCLUDE_PATH)\resource.inc

$(OUTPUT_INCLUDE_PATH)\resource.inc : $(RESOURCE_INC_LIST)
    perl $(BIN_PATH)\resappend.pl $(separate " " $(match .inc $(RESOURCE_INC_LIST))) $(OUTPUT_INCLUDE_PATH)\resource.inc
    perl $(BIN_PATH)\rscrenum.pl $@ > temp.inc
    del $@
    copy temp.inc $@
    del     temp.inc


#This will add the output directories to the targets to be built
ALL : $(OUTPUT_PATH) $(OBJ_PATH) $(LIST_PATH) $(OUTPUT_RSRC_PATH) $(OUTPUT_INCLUDE_PATH)

.INIT : $(OUTPUT_PATH) $(OBJ_PATH) $(LIST_PATH) $(OUTPUT_RSRC_PATH) $(OUTPUT_INCLUDE_PATH)

#Don't delete these targets when the build fails
.PRECIOUS : $(OUTPUT_PATH) $(OBJ_PATH) $(LIST_PATH) $(OUTPUT_RSRC_PATH) $(OUTPUT_INCLUDE_PATH)

$(OUTPUT_INCLUDE_PATH): $(OUTPUT_PATH)
    @$(nexist $@ echo Creating $@)
    @$(nexist $@ mkdir $@)

$(OBJ_PATH)         : $(OUTPUT_PATH)
    @$(nexist $@ echo Creating $@)
    @$(nexist $@ mkdir $@)

$(OUTPUT_RSRC_PATH) : $(OUTPUT_PATH)
    @$(nexist $@ echo Creating $@)
    @$(nexist $@ mkdir $@)

$(LIST_PATH)        : $(OUTPUT_PATH)
    @$(nexist $@ echo Creating $@)
    @$(nexist $@ mkdir $@)

$(OUTPUT_PATH)      :
    @$(nexist $@ echo Creating $@)
    @$(nexist $@ mkdir $@)

#This cleans up the list files 
#.DONE :
#    @Echo Cleaning up list files (some spurious warnings may occur)
#    @echo ....finished cleaning.

#clears the suffix list
.SUFFIXES:	
#assigns our own.
.SUFFIXES:	.asm .obj .c

.SUFFIXES:  .bmp .jpg .src

.SUFFIXES:  .txt .src

#////////////
# to debug build problems, 
# remove @ preceding $(AS) to echo build options including paths
#////////////
.asm.obj:
    @echo Assembling $<
ifdef D_VERBOSE
	$(AS_EXE) -o $@ -l $(ASFLAGS) $(ASFLAGS_$*) $<
else
	@$(AS_EXE) -o $@ -l $(ASFLAGS) $(ASFLAGS_$*) $<
endif
    @call $(ETC_PATH)\move ${*F}.lst $(LIST_PATH)

.asm.sym:
    @echo Assembling $<
ifdef D_VERBOSE
	$(AS_EXE) -o $@ -l $(ASFLAGS) $(ASFLAGS_$*) $<
else
	@$(AS_EXE) -o $@ -l $(ASFLAGS) $(ASFLAGS_$*) $<
endif
    @call $(ETC_PATH)\move ${*F}.lst $(LIST_PATH)


#####
####   -OG option here to workaround DSP core errata
#####
.c.obj:
    @echo Compiling $<
ifdef D_VERBOSE
    $(CC_EXE) -o $@ -OG $(CCFLAGS) $(CCFLAGS_$*) $<
else
    @$(CC_EXE) -o $@ -OG $(CCFLAGS) $(CCFLAGS_$*) $<
endif
    @call $(ETC_PATH)\move ${*F}.lst $(LIST_PATH)

ifdef COLOR_262K
COLOR_RSRC_TYPE=4
else
ifdef COLOR_65K
COLOR_RSRC_TYPE=3 
else
COLOR_RSRC_TYPE=2
endif
endif

ifdef D_CLCD
.bmp.src: 
    @echo Converting Bitmap $<
    @+cd $(OUTPUT_RSRC_PATH) ;\
    @..\$(BIN_PATH)\buildlcd -b..\$< -$(COLOR_RSRC_TYPE)

.jpg.bmp:
    @echo Converting Jpeg $<
    @$(BIN_PATH)\djpg2bmp $< $@

# uses temp file djpg2bmp.bmp
.jpg.src:
    @echo Converting Jpeg $<
    @+cd $(OUTPUT_RSRC_PATH) ;\
    @..\$(BIN_PATH)\djpg2bmp ..\$< djpg2bmp.bmp ;\
    @..\$(BIN_PATH)\buildlcd -bdjpg2bmp.bmp -o${@F} -$(COLOR_RSRC_TYPE) ;\
    @del /F djpg2bmp.bmp

else

.bmp.src: 
    @echo Converting Bitmap $<
    @+cd $(OUTPUT_RSRC_PATH) ;\
    @..\$(BIN_PATH)\buildres -z -b..\$< ;\
    @del /F ${*F}.asm

endif

.txt.src: 
    @echo Converting Text file $<
    @+cd $(OUTPUT_RSRC_PATH) ;\
    @..\$(BIN_PATH)\buildres -z -s..\$< ;\
    @del /F ${*F}.asm


clean :
        -erase /f /Q $(OUTPUT_INCLUDE_PATH)
        -erase /f /Q $(OUTPUT_RSRC_PATH)
        -erase /f /Q $(OUTPUT_PATH)
        -erase /f /q $(OBJ_PATH)
        -erase /f /q $(LIST_PATH)


endif
