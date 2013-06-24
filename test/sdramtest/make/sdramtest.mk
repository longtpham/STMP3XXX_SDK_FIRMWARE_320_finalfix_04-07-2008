# WARNING: standard.mk depends upon the following macros existing
# PROJ is basename for all executables -- must be the same as subdir name

PROJ                = sdramtest

# relative pointer to SDK root directory \Mp3BaseCc
RELBASE_PATH        = ..\..\..
ROOT                = $(RELBASE_PATH)

# these macros are project dependent
PROJ_PATH           = $(ROOT)\test\$(PROJ)
PROJSYS_PATH        = $(PROJ_PATH)\system
HWEQU_H             = $(OUTPUT_INCLUDE_PATH)\hwequ.h

## PLAYER ###############################
# Distinguish between Player, Dcc and USBMSC
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
include $(ROOT)\etc\registers.mk

# must follow standard.mk, where base INC_ALL gets defined
INC_ALL= -I$(PROJ_PATH);$(OUTPUT_PATH);$(INC_ALL)
INC_ALL_C = -Wc-I$(PROJ_PATH) -Wc-I$(OUTPUT_INCLUDE_PATH) -Wc-I$(INC_PATH) -Wc-I$(SYS_COMMON_PATH)

### $(BLD) is environment variable; set in batch file
AS_OPTS=$(INC_ALL) -OJMPS  -l -LCeIn -DTASKING $(BLD) 
LK_OPTS= -L$(MP3_PATH)\tasking -L$(BIN_DRIVE)\2410\lib\2410 -w8
CC_OPTS= -Wc-si $(BLD) -w68 -w66 $(INC_ALL_C) -Wa$(INC_ALL)

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

OBJLIST =   $(OBJ_PATH)\sdramtest.obj\
            $(OBJ_PATH)\sdramsupport.obj\
            $(OBJ_PATH)\sdram.obj\
            $(OBJ_PATH)\dac.obj\

ALL : $(OUTPUT_PATH)\$(PROJ).abs 

#///////////////////////////////////////////////////////////////////////////////
# Link
#///////////////////////////////////////////////////////////////////////////////
"$(OUTPUT_PATH)\$(PROJ).out" : "$(CPU_PATH)\testdflt.dsc" $(HWEQULIST) $(OBJLIST) $(PROJ).mk
    @echo Linking $(PROJ).out
        @$(LK_EXE) -o $@ $(LK_OPTS) $(match .obj $!) 

"$(OUTPUT_PATH)\$(PROJ).abs" : "$(OUTPUT_PATH)\$(PROJ).out" "$(CPU_PATH)\testdflt.dsc"
    @echo Creating IEEE-695 absolute file $@
        @$(LC_EXE) -o $@ $(LC_OPTS) -d$(match .dsc $!) $(OUTPUT_PATH)\$(PROJ).out

#///////////////////////////////////////////////////////////////////////////////
# Project-specific files
#///////////////////////////////////////////////////////////////////////////////

$(OBJ_PATH)\sdramtest.obj : $(PROJ_PATH)\sdramtest.c
$(OBJ_PATH)\sdramsupport.obj : $(PROJ_PATH)\sdramsupport.c
$(OBJ_PATH)\sdram.obj : $(PROJ_PATH)\sdram.c
$(OBJ_PATH)\dac.obj : $(PROJ_PATH)\dac.c

$(HWEQU_H) : $(HW_REGS_INC_PATH)\hwequ.inc
    @echo Converting Header File $!
	$(INCTOC_EXE) $! $@



