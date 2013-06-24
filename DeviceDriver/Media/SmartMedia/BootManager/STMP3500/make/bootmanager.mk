ifndef VERSION
VERSION=999.999
endif

ifndef BIN_DRIVE
BIN_DRIVE=c:
endif

############## note: type 4 now uses type 2 functions, so if one is desired, both must be enabled.
ifdef D_EEPROM
BLD=$(BLD) -DUSE_EEPROM
CC_OPTS=$(CC_OPTS) -Wa-DUSE_EEPROM -Wc-DUSE_EEPROM

# Note that type2 flash should not use an EEPROM since the STMP35xx rom has type 2 devices up to 2GBytes.
# In a later release, we might support both type2 and 3/4 in boot eeprom
# if a larger 64kB boot eeprom is used and usbmsc P RAM boundary is respected.
# But currently this is not neccessary for today's type 2 devices.

# Enable build for Type2, MLC and Renesas MLC.
BLD=$(BLD) -DINCLUDE_TYPE2_NAND -DINCLUDE_MLC_DESCRIPTORS -DINCLUDE_RENESAS_DESCRIPTORS
CC_OPTS=$(CC_OPTS) -Wa-DINCLUDE_TYPE2_NAND -Wc-DINCLUDE_TYPE2_NAND
CC_OPTS=$(CC_OPTS) -Wa-DINCLUDE_MLC_DESCRIPTORS -Wc-DINCLUDE_MLC_DESCRIPTORS
CC_OPTS=$(CC_OPTS) -Wa-DINCLUDE_RENESAS_DESCRIPTORS -Wc-DINCLUDE_RENESAS_DESCRIPTORS

else
     #### non EEPROM: boot mgr build should have all nand types included (while there's room).
BLD=$(BLD) -DINCLUDE_TYPE2_NAND -DINCLUDE_MLC_DESCRIPTORS -DINCLUDE_RENESAS_DESCRIPTORS
CC_OPTS=$(CC_OPTS) -Wa-DINCLUDE_TYPE2_NAND -Wc-DINCLUDE_TYPE2_NAND
CC_OPTS=$(CC_OPTS) -Wa-DINCLUDE_MLC_DESCRIPTORS -Wc-DINCLUDE_MLC_DESCRIPTORS
CC_OPTS=$(CC_OPTS) -Wa-DINCLUDE_RENESAS_DESCRIPTORS -Wc-DINCLUDE_RENESAS_DESCRIPTORS

endif
############## end this EEPROM block


################# BUILD OPTIONS #########################

##### DEVICE ###############
# BLD=$(BLD)
############################

# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name
PROJ    = bootmanager
# points to \Mp3Firmware
RELBASE_PATH=..\..\..\..\..\..
ROOT    = $(RELBASE_PATH)

# these macros are project dependent
PROJ_PATH           = $(ROOT)\DeviceDriver\Media\SmartMedia\BootManager\STMP3500
COMMON_PATH         = $(ROOT)\DeviceDriver\Media\SmartMedia\BootManager\Common
OUTPUT_PATH         = ..\output_3500
OBJ_PATH            = $(OUTPUT_PATH)\obj
LIST_PATH           = $(OUTPUT_PATH)\lst
OUTPUT_INCLUDE_PATH = $(OUTPUT_PATH)\include
PROJRSRC_PATH       = $(PROJ_PATH)\rsrc
PROJMAKE_PATH       = $(PROJ_PATH)\make
USB20_DD_PATH       = $(PROJ_PATH)\USB20\DeviceDriver
USB20_DD_API_PATH   = $(PROJ_PATH)\USB20\API
USB20_CH9_PATH      = $(PROJ_PATH)\USB20\CH9
USB20_DD_INC_PATH   = $(PROJ_PATH)\USB20\Include
SYSTEM_PATH         = $(PROJ_PATH)\System
INCLUDE_PATH        = $(PROJ_PATH)\include
ROM_INCLUDE_PATH    = $(ROOT)\ROM\STMP3500\include
INCLUDE_PATH        = $(ROOT)\inc
SYSTEM_IRQ_PATH     = $(ROOT)\System\hardware\interrupt
BOOT_ECC_PATH       = $(PROJ_PATH)\ECC

#########################################
#
include $(ROOT)\etc\standard.mk
#########################################
include $(ROOT)\etc\registers.mk

# must follow standard.mk, where base INC_ALL gets defined
#INC_ALL=-I$(INC_ALL);$(OUTPUT_INCLUDE_PATH);$(USB20_DD_INC_PATH);$(PROJ_PATH);$(OUTPUT_PATH);$(ROOT)\INC\3500

INC_ALL     = -I$(INC_ALL);$(OUTPUT_INCLUDE_PATH);$(USB20_DD_INC_PATH);$(PROJ_PATH);$(ROOT)\INC\3500
INC_ALL_C   = -Wc-I$(OUTPUT_INCLUDE_PATH)
INC_ALL_C   = -Wc-I$(USB20_DD_PATH) -Wc-I$(USB20_DD_API_PATH) -Wc-I$(USB20_CH9_PATH)\
              -Wc-I$(USB20_DD_INC_PATH) -Wc-I$(INCLUDE_PATH) -Wc-I$(PROJ_PATH)\
              -Wc-I$(PROJ_COMMON_PATH)\
              -Wa-I$(INCLUDE_PATH)  -Wa-I$(USB20_DD_INC_PATH)\
              $(INC_ALL_C)


### $(BLD) is environment variable; set in batch file
AS_OPTS=$(INC_ALL) -OJMNPRS -RDRS -gal -DBRD=REVB -DDPLUS=TRUE -DTASKING $(BLD)

### -f 1 is .abs and -f 2 is .s
LC_OPTS= -f1 -c -M -w0

# COMPILER OPTIONS (precede with -Wc to make sure it gets passed to the compiler from the control program)
# -Wc-si : merge C source and included files into assembly files  ** can change to -Wc-s to remove included files from listing
# -tmp   : add this option to leave the .src when get an error in the assembler part of the .c to .obj
#          WANRING: must remove option and rebuild because the extension .src confuses the resource linker utility
CC_OPTS= -Wc-si $(BLD) -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL) $(C_ASM_OPTS)

# overwritte flags defined in standard make file
CCFLAGS = -c $(CC_OPTS) -L7 -MmxL -Msx -rr6 -rn6 -rm6 -Wc-g -Wc-O2 -Wc-R -Wc-Cs -Wa-gs -Wa-l

# common obj list for all devices
OBJLIST =   $(OBJLIST)\
            $(OBJ_PATH)\bootmanager.obj\
            $(OBJ_PATH)\bootmanagerflash.obj\
            $(OBJ_PATH)\3500bootmanagerusb.obj\
            $(OBJ_PATH)\3500bootmanagermem.obj\
            $(OBJ_PATH)\stackmem.obj\
            $(OBJ_PATH)\stack.obj\
            $(OBJ_PATH)\nanddevicedriver.obj\
            $(OBJ_PATH)\nanddesc.obj\
            $(OBJ_PATH)\nanddevdrv.obj\
            $(OBJ_PATH)\nanddevdrvapi.obj\
            $(OBJ_PATH)\nandhal.obj\
            $(OBJ_PATH)\gpflash_hal.obj\
            $(OBJ_PATH)\nandra_utils.obj\
            $(OBJ_PATH)\NANDHALCorrectECC.obj

# if we're an EEPROM, need to build USB portion.
ifdef D_EEPROM
OBJLIST =   $(OBJLIST)\
            $(OBJ_PATH)\usb20ddmem.obj\
            $(OBJ_PATH)\usb20devapi.obj\
            $(OBJ_PATH)\usb20dd.obj\
            $(OBJ_PATH)\usb20ch9.obj\
            $(OBJ_PATH)\descriptor.obj\
            $(OBJ_PATH)\sysirq.obj
endif

RSRCLIST =

ALL : $(OUTPUT_PATH)\$(PROJ).s

HEADERLIST = $(HEADERLIST)\
             $(OUTPUT_INCLUDE_PATH)\usb20project.h\
             $(OUTPUT_INCLUDE_PATH)\regs3500usb20.h\
             $(OUTPUT_INCLUDE_PATH)\smequ.h\
             $(OUTPUT_INCLUDE_PATH)\regsecc.h

BOOTNAND_DEPENDENCIES= \
    "$(PROJ_PATH)\NANDRA_utils.h" \
    $(COMMON_PATH)\nandstructs.h \
    "$(COMMON_PATH)\nanddevdrvapi.h"

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////

"$(OUTPUT_PATH)\$(PROJ).out" :  $(PROJ_PATH)\make\stmp3500.dsc $(CPU_PATH)\stmp3500.cpu $(CPU_PATH)\stmp3500.mem $(HEADERLIST) $(OBJLIST) $(ALLLIBLIST)
    @echo Linking $(PROJ).out
        $(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) $(LIB_OPT_LIST) $(match .a $!)

"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" $(PROJ_PATH)\make\stmp3500.dsc $(CPU_PATH)\stmp3500.cpu $(CPU_PATH)\stmp3500.mem
    @echo Creating IEEE-695 absolute file $@
        @$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out

#///////////////////////////////////////////////////////////////////////////////
# Project-specific project files
#///////////////////////////////////////////////////////////////////////////////

"$(OBJ_PATH)\bootmanager.obj" : "$(COMMON_PATH)\bootmanager.asm"
"$(OBJ_PATH)\bootmanager.obj" : $(PROJ_PATH)\3500bootmanagermem.xref
"$(OBJ_PATH)\bootmanager.obj" : $(XREF_PATH)\bootmanager.xref

"$(OBJ_PATH)\bootmanagerflash.obj" : $(COMMON_PATH)\bootmanagerflash.asm
"$(OBJ_PATH)\bootmanagerflash.obj" : $(PROJ_PATH)\3500bootmanagermem.xref
"$(OBJ_PATH)\bootmanagerflash.obj" : $(XREF_PATH)\bootmanager.xref

"$(OBJ_PATH)\3500bootmanagerusb.obj" : $(PROJ_PATH)\3500bootmanagerusb.asm

"$(OBJ_PATH)\3500bootvec.obj" : $(PROJ_PATH)\3500bootvec.asm

"$(OBJ_PATH)\3500bootmanagermem.obj" : $(PROJ_PATH)\3500bootmanagermem.asm

$(OBJ_PATH)\stackmem.obj : $(COMMON_PATH)\stackmem.asm

$(OBJ_PATH)\stack.obj : $(COMMON_PATH)\stack.asm

$(OBJ_PATH)\nanddevicedriver.obj : $(COMMON_PATH)\nanddevicedriver.asm

$(OBJ_PATH)\nandhal.obj : $(COMMON_PATH)\nandhal.asm

$(OBJ_PATH)\nanddesc.obj : $(COMMON_PATH)\nanddesc.c $(OUTPUT_INCLUDE_PATH)\smequ.h

$(OBJ_PATH)\nanddevdrv.obj : $(COMMON_PATH)\nanddevdrv.c $(BOOTNAND_DEPENDENCIES) $(OUTPUT_INCLUDE_PATH)\smequ.h

$(OBJ_PATH)\nanddevdrvapi.obj : $(COMMON_PATH)\nanddevdrvapi.c $(BOOTNAND_DEPENDENCIES)

$(OBJ_PATH)\gpflash_hal.obj : $(PROJ_PATH)\gpflash_hal.asm

$(OBJ_PATH)\usb20ddmem.obj : $(USB20_DD_PATH)\usb20ddmem.asm

$(OBJ_PATH)\usb20devapi.obj : $(USB20_DD_API_PATH)\usb20devapi.c

$(OBJ_PATH)\usb20dd.obj : $(USB20_DD_PATH)\usb20dd.c

$(OBJ_PATH)\usb20ch9.obj : $(USB20_CH9_PATH)\usb20ch9.c

$(OBJ_PATH)\descriptor.obj : $(USB20_CH9_PATH)\descriptor.c $(OUTPUT_INCLUDE_PATH)\regsrevision.h

#$(OBJ_PATH)\descriptor.obj     : $(PROJ_PATH)\USB20\descriptor.c $(OUTPUT_INCLUDE_PATH)\regsrevision.h

#$(OBJ_PATH)\sysirq.obj : $(SYSTEM_PATH)\sysirq.c
$(OBJ_PATH)\sysirq.obj : $(SYSTEM_IRQ_PATH)\sysirq.c

$(OBJ_PATH)\nandra_utils.obj : $(BOOT_ECC_PATH)\nandra_utils.c $(BOOTNAND_DEPENDENCIES)

"$(OBJ_PATH)\NANDHALCorrectECC.obj" : "$(BOOT_ECC_PATH)\NANDHALCorrectECC.c" $(BOOTNAND_DEPENDENCIES)

$(OUTPUT_INCLUDE_PATH)\smequ.h: $(COMMON_PATH)\smequ.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

$(OUTPUT_INCLUDE_PATH)\usb20project.h:$(USB20_DD_INC_PATH)\usb20project.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@

$(OUTPUT_INCLUDE_PATH)\regs3500usb20.h:$(USB20_DD_INC_PATH)\regs3500usb20.inc
    @echo Converting Header File $!
    $(INCTOC_EXE) $! $@


#///////////////////////////////////////////////////////////////////////////////
# Module code extraction
#///////////////////////////////////////////////////////////////////////////////
# x:FFE5 write is for i2c clock divider. Allows fast EEPROM load during bootup.

"$(OUTPUT_PATH)\$(PROJ).s" : $(OUTPUT_PATH)\$(PROJ).abs
        @echo Extract System S-rec for $(DEVICE) device family
        @$(BIN_PATH)\$(EXTRACT_EXE) $! -S$@=all -V$(VERSION) -OFFE8=1818 -OFFE9=1818 -OFFED=380000 -OFFE5=00003A
        @echo Encrypt $(PROJ).s to $(PROJ).sb
        @$(BIN_PATH)\stosbcon "$(OUTPUT_PATH)\$(PROJ).s" $(OUTPUT_PATH)\$(PROJ).sb
    ifdef D_EEPROM
        @echo Encrypt $(PROJ).s to $(PROJ).se for EEPROM image
        @$(BIN_PATH)\sb -e "$(OUTPUT_PATH)\$(PROJ).s"
    endif



